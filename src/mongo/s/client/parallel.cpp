/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kNetwork

#include "mongo/platform/basic.h"

#include "mongo/s/client/parallel.h"

#include "mongo/client/constants.h"
#include "mongo/client/dbclient_cursor.h"
#include "mongo/client/dbclient_rs.h"
#include "mongo/client/replica_set_monitor.h"
#include "mongo/db/bson/dotted_path_support.h"
#include "mongo/db/query/query_request.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/get_status_from_command_result.h"
#include "mongo/s/catalog_cache.h"
#include "mongo/s/client/shard_connection.h"
#include "mongo/s/client/shard_registry.h"
#include "mongo/s/grid.h"
#include "mongo/util/net/socket_exception.h"

namespace mongo {

using std::map;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

namespace dps = ::mongo::dotted_path_support;

namespace {

/**
 * Throws an exception wrapping the error document in this cursor when the error flag is set.
 */
void throwCursorError(DBClientCursor* cursor) {
    verify(cursor);

    if (cursor->hasResultFlag(ResultFlag_ErrSet)) {
        uassertStatusOK(getStatusFromCommandResult(cursor->next()));
    }
}

}  // namespace

struct ParallelConnectionState {
    ParallelConnectionState() : count(0), done(false) {}

    std::string toString() const;

    BSONObj toBSON() const;

    // Please do not reorder. cursor destructor can use conn.
    // On a related note, never attempt to cleanup these pointers manually.
    std::shared_ptr<ShardConnection> conn;
    std::shared_ptr<DBClientCursor> cursor;

    // Version information
    std::shared_ptr<ChunkManager> manager;
    std::shared_ptr<Shard> primary;

    // Cursor status information
    long long count;
    bool done;
};

struct ParallelConnectionMetadata {
    ParallelConnectionMetadata()
        : retryNext(false), initialized(false), finished(false), completed(false), errored(false) {}

    ~ParallelConnectionMetadata() {
        cleanup(true);
    }

    void cleanup(bool full = true);

    std::shared_ptr<ParallelConnectionState> pcState;

    bool retryNext;

    bool initialized;
    bool finished;
    bool completed;

    bool errored;

    BSONObj toBSON() const;

    std::string toString() const {
        return str::stream() << "PCMData : " << toBSON();
    }
};

/**
 * Helper class to manage ownership of opened cursors while merging results.
 *
 * TODO:  Choose one set of ownership semantics so that this isn't needed - merge sort via mapreduce
 * is the main issue since it has no metadata and this holder owns the cursors.
 */
class DBClientCursorHolder {
public:
    DBClientCursorHolder() = default;

    void reset(DBClientCursor* cursor, ParallelConnectionMetadata* pcmData) {
        _cursor.reset(cursor);
        _pcmData.reset(pcmData);
    }

    DBClientCursor* get() {
        return _cursor.get();
    }

    ParallelConnectionMetadata* getMData() {
        return _pcmData.get();
    }

    void release() {
        _cursor.release();
        _pcmData.release();
    }

private:
    std::unique_ptr<DBClientCursor> _cursor;
    std::unique_ptr<ParallelConnectionMetadata> _pcmData;
};

// --------  ParallelSortClusteredCursor -----------

ParallelSortClusteredCursor::ParallelSortClusteredCursor(const QuerySpec& qSpec,
                                                         const CommandInfo& cInfo)
    : _qSpec(qSpec), _cInfo(cInfo), _totalTries(0) {
    _done = false;
    _didInit = false;

    _finishCons();
}

ParallelSortClusteredCursor::ParallelSortClusteredCursor(const set<string>& servers,
                                                         const string& ns,
                                                         const Query& q,
                                                         int options,
                                                         const BSONObj& fields)
    : _servers(servers) {
    _sortKey = q.getSort().copy();
    _needToSkip = 0;

    _done = false;
    _didInit = false;

    // Populate legacy fields
    _ns = ns;
    _query = q.obj.getOwned();
    _options = options;
    _fields = fields.getOwned();
    _batchSize = 0;

    _finishCons();
}

ParallelSortClusteredCursor::~ParallelSortClusteredCursor() {
    // WARNING: Commands (in particular M/R) connect via _oldInit() directly to shards
    bool isDirectShardCursor = _cursorMap.empty();

    // Non-direct shard cursors are owned by the _cursorMap, so we release
    // them in the array here.  Direct shard cursors clean themselves.
    if (!isDirectShardCursor) {
        for (int i = 0; i < _numServers; i++)
            _cursors[i].release();
    }

    delete[] _cursors;
    _cursors = nullptr;

    // Clear out our metadata after removing legacy cursor data
    _cursorMap.clear();

    // Just to be sure
    _done = true;
}

void ParallelSortClusteredCursor::init(OperationContext* opCtx) {
    if (_didInit)
        return;
    _didInit = true;

    if (!_qSpec.isEmpty()) {
        fullInit(opCtx);
    } else {
        // You can only get here by using the legacy constructor
        // TODO: Eliminate this
        _oldInit(opCtx);
    }
}

void ParallelSortClusteredCursor::_finishCons() {
    _numServers = _servers.size();
    _lastFrom = 0;
    _cursors = nullptr;

    if (!_qSpec.isEmpty()) {
        _needToSkip = _qSpec.ntoskip();
        _cursors = nullptr;
        _sortKey = _qSpec.sort();
        _fields = _qSpec.fields();
    }

    // Partition sort key fields into (a) text meta fields and (b) all other fields.
    set<string> textMetaSortKeyFields;
    set<string> normalSortKeyFields;

    // Transform _sortKey fields {a:{$meta:"textScore"}} into {a:-1}, in order to apply the
    // merge sort for text metadata in the correct direction.
    BSONObjBuilder transformedSortKeyBuilder;

    BSONObjIterator sortKeyIt(_sortKey);
    while (sortKeyIt.more()) {
        BSONElement e = sortKeyIt.next();
        if (QueryRequest::isTextScoreMeta(e)) {
            textMetaSortKeyFields.insert(e.fieldName());
            transformedSortKeyBuilder.append(e.fieldName(), -1);
        } else {
            normalSortKeyFields.insert(e.fieldName());
            transformedSortKeyBuilder.append(e);
        }
    }
    _sortKey = transformedSortKeyBuilder.obj();

    // Verify that that all text metadata sort fields are in the projection.  For all other sort
    // fields, copy them into the projection if they are missing (and if projection is
    // negative).
    if (!_sortKey.isEmpty() && !_fields.isEmpty()) {
        BSONObjBuilder b;
        bool isNegative = false;
        {
            BSONObjIterator i(_fields);
            while (i.more()) {
                BSONElement e = i.next();
                b.append(e);

                string fieldName = e.fieldName();

                if (QueryRequest::isTextScoreMeta(e)) {
                    textMetaSortKeyFields.erase(fieldName);
                } else {
                    // exact field
                    bool found = normalSortKeyFields.erase(fieldName);

                    // subfields
                    set<string>::const_iterator begin =
                        normalSortKeyFields.lower_bound(fieldName + ".\x00");
                    set<string>::const_iterator end =
                        normalSortKeyFields.lower_bound(fieldName + ".\xFF");
                    normalSortKeyFields.erase(begin, end);

                    if (!e.trueValue()) {
                        uassert(13431,
                                "have to have sort key in projection and removing it",
                                !found && begin == end);
                    } else if (!e.isABSONObj()) {
                        isNegative = true;
                    }
                }
            }
        }

        if (isNegative) {
            for (set<string>::const_iterator it(normalSortKeyFields.begin()),
                 end(normalSortKeyFields.end());
                 it != end;
                 ++it) {
                b.append(*it, 1);
            }
        }

        _fields = b.obj();
    }

    if (!_qSpec.isEmpty()) {
        _qSpec.setFields(_fields);
    }

    uassert(
        17306, "have to have all text meta sort keys in projection", textMetaSortKeyFields.empty());
}

void ParallelSortClusteredCursor::fullInit(OperationContext* opCtx) {
    startInit(opCtx);
    finishInit(opCtx);
}

void ParallelSortClusteredCursor::_markStaleNS(const NamespaceString& staleNS,
                                               const StaleConfigException& e) {
    if (_staleNSMap.find(staleNS.ns()) == _staleNSMap.end()) {
        _staleNSMap[staleNS.ns()] = 1;
    }

    const int tries = ++_staleNSMap[staleNS.ns()];

    if (tries >= 5) {
        uassertStatusOK(e.toStatus("too many retries of stale version info"));
    }
}

void ParallelSortClusteredCursor::setupVersionAndHandleSlaveOk(
    OperationContext* opCtx,
    std::shared_ptr<ParallelConnectionState> state,
    const ShardId& shardId,
    std::shared_ptr<Shard> primary,
    const NamespaceString& ns,
    const string& vinfo,
    std::shared_ptr<ChunkManager> manager) {
    if (manager) {
        state->manager = manager;
    } else if (primary) {
        state->primary = primary;
    }

    verify(!primary || shardId == primary->getId());

    // Setup conn
    if (!state->conn) {
        const auto shard =
            uassertStatusOK(Grid::get(opCtx)->shardRegistry()->getShard(opCtx, shardId));
        state->conn.reset(new ShardConnection(opCtx, shard->getConnString(), ns.ns(), manager));
    }

    const DBClientBase* rawConn = state->conn->getRawConn();
    bool allowShardVersionFailure = rawConn->type() == ConnectionString::SET &&
        DBClientReplicaSet::isSecondaryQuery(_qSpec.ns(), _qSpec.query(), _qSpec.options());

    // Skip shard version checking if primary is known to be down.
    if (allowShardVersionFailure) {
        const DBClientReplicaSet* replConn = dynamic_cast<const DBClientReplicaSet*>(rawConn);
        invariant(replConn);
        auto rsMonitor = ReplicaSetMonitor::get(replConn->getSetName());
        uassert(16388,
                str::stream() << "cannot access unknown replica set: " << replConn->getSetName(),
                rsMonitor != nullptr);
        if (!rsMonitor->isKnownToHaveGoodPrimary()) {
            state->conn->donotCheckVersion();

            // A side effect of this short circuiting is the mongos will not be able figure out
            // that the primary is now up on it's own and has to rely on other threads to refresh
            // node states.

            static Occasionally sampler;
            if (sampler.tick()) {
                const DBClientReplicaSet* repl = dynamic_cast<const DBClientReplicaSet*>(rawConn);
                dassert(repl);
                LOGV2_WARNING(
                    22695,
                    "Primary for {replicaSetAddress} was down before, bypassing "
                    "setShardVersion. The local replica set view and targeting may be stale.",
                    "Bypassing setShardVersion because replica set view and targeting may be stale "
                    "from primary having been down",
                    "replicaSetAddress"_attr = repl->getServerAddress());
            }

            return;
        }
    }

    try {
        if (state->conn->setVersion()) {
            LOGV2_DEBUG(22678,
                        2,
                        "pcursor: needed to set remote version on connection to value compatible "
                        "with {shardVersion}",
                        "pcursor: needed to set remote version on connection",
                        "shardVersion"_attr = vinfo);
        }
    } catch (const DBException& dbExcep) {
        auto errCode = dbExcep.code();
        if (allowShardVersionFailure &&
            (ErrorCodes::isNotMasterError(errCode) || ErrorCodes::isNetworkError(errCode) ||
             errCode == ErrorCodes::FailedToSatisfyReadPreference)) {
            // It's okay if we don't set the version when talking to a secondary, we can
            // be stale in any case.

            static Occasionally sampler;
            if (sampler.tick()) {
                const DBClientReplicaSet* repl =
                    dynamic_cast<const DBClientReplicaSet*>(state->conn->getRawConn());
                dassert(repl);
                LOGV2_WARNING(
                    22696,
                    "Cannot contact replica set {replicaSetAdress} to check shard version. "
                    "The local replica set view and targeting may be stale",
                    "Cannot contact replica set to check shard version. "
                    "The local replica set view and targeting may be stale",
                    "replicaSetAddress"_attr = repl->getServerAddress());
            }
        } else {
            throw;
        }
    }
}

void ParallelSortClusteredCursor::startInit(OperationContext* opCtx) {
    const bool returnPartial = (_qSpec.options() & QueryOption_PartialResults);
    const NamespaceString nss(!_cInfo.isEmpty() ? _cInfo.versionedNS : _qSpec.ns());

    string prefix;
    if (MONGO_unlikely(shouldLog(logv2::LogSeverity::Debug(2)))) {
        if (_totalTries > 0) {
            prefix = str::stream() << "retrying (" << _totalTries << " tries)";
        } else {
            prefix = "creating";
        }
    }
    LOGV2_DEBUG(22679,
                2,
                "pcursor: {prefix} pcursor over {query} and {command}",
                "pcursor",
                "prefix"_attr = prefix,
                "query"_attr = _qSpec,
                "command"_attr = _cInfo);

    shared_ptr<ChunkManager> manager;
    shared_ptr<Shard> primary;

    {
        auto routingInfoStatus =
            Grid::get(opCtx)->catalogCache()->getCollectionRoutingInfo(opCtx, nss);
        if (routingInfoStatus != ErrorCodes::NamespaceNotFound) {
            auto routingInfo = uassertStatusOK(std::move(routingInfoStatus));
            manager = routingInfo.cm();
            // ParallelSortClusteredCursor has two states - either !cm && primary, which means
            // unsharded collection, or cm && !primary, which means sharded collection.
            if (!manager) {
                primary = routingInfo.db().primary();
            }
        }
    }

    set<ShardId> shardIds;
    string vinfo;

    if (manager) {
        if (MONGO_unlikely(shouldLog(logv2::LogSeverity::Debug(2)))) {
            vinfo = str::stream() << "[" << manager->getns().ns() << " @ "
                                  << manager->getVersion().toString() << "]";
        }

        manager->getShardIdsForQuery(opCtx,
                                     !_cInfo.isEmpty() ? _cInfo.cmdFilter : _qSpec.filter(),
                                     !_cInfo.isEmpty() ? _cInfo.cmdCollation : BSONObj(),
                                     &shardIds);
    } else if (primary) {
        if (MONGO_unlikely(shouldLog(logv2::LogSeverity::Debug(2)))) {
            vinfo = str::stream() << "[unsharded @ " << primary->toString() << "]";
        }

        shardIds.insert(primary->getId());
    }

    // Close all cursors on extra shards first, as these will be invalid
    for (auto& cmEntry : _cursorMap) {
        const auto& shardId = cmEntry.first;

        if (shardIds.find(shardId) == shardIds.end()) {
            LOGV2_DEBUG(22680,
                        2,
                        "pcursor: closing cursor on shard {shardId} as the connection is no longer "
                        "required by {shardVersion}",
                        "pcursor: closing cursor as the connection is no longer required",
                        "shardId"_attr = shardId,
                        "shardVersion"_attr = vinfo);

            cmEntry.second.cleanup(true);
        }
    }

    LOGV2_DEBUG(22681,
                2,
                "pcursor: initializing over {shardsNumber} shards required by {shardVersion}",
                "pcursor: initializing for all shards",
                "shardsNumber"_attr = shardIds.size(),
                "shardVersion"_attr = vinfo);

    // Don't retry indefinitely for whatever reason
    _totalTries++;
    uassert(15986, "too many retries in total", _totalTries < 10);

    for (const ShardId& shardId : shardIds) {
        auto& mdata = _cursorMap[shardId];

        LOGV2_DEBUG(22682,
                    2,
                    "pcursor: initializing on shard {shardId}, current connection state is "
                    "{connectionState}",
                    "pcursor: initializing on shard",
                    "shardId"_attr = shardId,
                    "connectionState"_attr = mdata.toBSON());

        // This may be the first time connecting to this shard, if so we can get an error here
        try {
            if (mdata.initialized) {
                invariant(mdata.pcState);

                auto state = mdata.pcState;

                bool compatiblePrimary = true;
                bool compatibleManager = true;

                if (primary && !state->primary)
                    LOGV2_WARNING(22697, "Collection becoming unsharded detected");
                if (manager && !state->manager)
                    LOGV2_WARNING(22698, "Collection becoming sharded detected");
                if (primary && state->primary && primary != state->primary)
                    LOGV2_WARNING(22699, "Weird shift of primary detected");

                compatiblePrimary = primary && state->primary && primary == state->primary;
                compatibleManager =
                    manager && state->manager && manager->compatibleWith(*state->manager, shardId);

                if (compatiblePrimary || compatibleManager) {
                    // If we're compatible, don't need to retry unless forced
                    if (!mdata.retryNext)
                        continue;
                    // Do partial cleanup
                    mdata.cleanup(false);
                } else {
                    // Force total cleanup of connection if no longer compatible
                    mdata.cleanup(true);
                }
            } else {
                // Cleanup connection if we're not yet initialized
                mdata.cleanup(false);
            }

            mdata.pcState = std::make_shared<ParallelConnectionState>();
            auto state = mdata.pcState;

            setupVersionAndHandleSlaveOk(opCtx, state, shardId, primary, nss, vinfo, manager);

            const string& ns = _qSpec.ns();

            // Setup cursor
            if (!state->cursor) {
                //
                // Here we decide whether to split the query limits up for multiple shards.
                // NOTE: There's a subtle issue here, in that it's possible we target a single
                // shard first, but are stale, and then target multiple shards, or vice-versa.
                // In both these cases, we won't re-use the old cursor created here, since the
                // shard version must have changed on the single shard between queries.
                //

                if (shardIds.size() > 1) {
                    // Query limits split for multiple shards

                    state->cursor.reset(new DBClientCursor(
                        state->conn->get(),
                        NamespaceString(ns),
                        _qSpec.query(),
                        isCommand() ? 1 : 0,  // nToReturn (0 if query indicates multi)
                        0,                    // nToSkip
                        // Does this need to be a ptr?
                        _qSpec.fields().isEmpty() ? nullptr
                                                  : _qSpec.fieldsData(),  // fieldsToReturn
                        _qSpec.options(),                                 // options
                        // NtoReturn is weird.
                        // If zero, it means use default size, so we do that for all cursors
                        // If positive, it's the batch size (we don't want this cursor limiting
                        // results), that's done at a higher level
                        // If negative, it's the batch size, but we don't create a cursor - so we
                        // don't want to create a child cursor either.
                        // Either way, if non-zero, we want to pull back the batch size + the skip
                        // amount as quickly as possible.  Potentially, for a cursor on a single
                        // shard or if we keep better track of chunks, we can actually add the skip
                        // value into the cursor and/or make some assumptions about the return value
                        // size ( (batch size + skip amount) / num_servers ).
                        _qSpec.ntoreturn() == 0
                            ? 0
                            : (_qSpec.ntoreturn() > 0
                                   ? _qSpec.ntoreturn() + _qSpec.ntoskip()
                                   : _qSpec.ntoreturn() - _qSpec.ntoskip())));  // batchSize
                } else {
                    // Single shard query

                    state->cursor.reset(new DBClientCursor(
                        state->conn->get(),
                        NamespaceString(ns),
                        _qSpec.query(),
                        _qSpec.ntoreturn(),  // nToReturn
                        _qSpec.ntoskip(),    // nToSkip
                        // Does this need to be a ptr?
                        _qSpec.fields().isEmpty() ? nullptr
                                                  : _qSpec.fieldsData(),  // fieldsToReturn
                        _qSpec.options(),                                 // options
                        0));                                              // batchSize
                }
            }

            bool lazyInit = state->conn->get()->lazySupported();
            if (lazyInit) {
                // Need to keep track if this is a second or third try for replica sets
                state->cursor->initLazy(mdata.retryNext);
                mdata.retryNext = false;
                mdata.initialized = true;
            } else {
                bool success = state->cursor->init();

                // Without full initialization, throw an exception
                uassert(15987,
                        str::stream()
                            << "could not fully initialize cursor on shard " << shardId
                            << ", current connection state is " << mdata.toBSON().toString(),
                        success);

                mdata.retryNext = false;
                mdata.initialized = true;
                mdata.finished = true;
            }

            LOGV2_DEBUG(22683,
                        2,
                        "pcursor: initialized {commandType} ({lazyInit}) on shard {shardId}, "
                        "current connection state is {connectionState}",
                        "pcursor: initialized command",
                        "commandType"_attr = (isCommand() ? "command" : "query"),
                        "lazyInit"_attr = (lazyInit ? "lazily" : "full"),
                        "shardId"_attr = shardId,
                        "connectionState"_attr = mdata.toBSON());
        } catch (StaleConfigException& e) {
            // Our version isn't compatible with the current version anymore on at least one shard,
            // need to retry immediately
            NamespaceString staleNS(e->getNss());
            _markStaleNS(staleNS, e);

            Grid::get(opCtx)
                ->catalogCache()
                ->invalidateShardOrEntireCollectionEntryForShardedCollection(
                    opCtx, nss, e->getVersionWanted(), e->getVersionReceived(), e->getShardId());

            LOGV2_DEBUG(22684,
                        1,
                        "Error initializing pcursor, stale config for namespace {namespace}, "
                        "caused by {error}, will retry",
                        "Error initializing pcursor, will retry",
                        "namespace"_attr = staleNS,
                        "error"_attr = redact(e));

            // This is somewhat strange
            if (staleNS != nss) {
                LOGV2_WARNING(22700,
                              "Versioned namespace {namespace} doesn't match stale config "
                              "namespace {staleNamespace}",
                              "Versioned namespace doesn't match stale config namespace",
                              "namespace"_attr = nss.ns(),
                              "staleNamespace"_attr = staleNS);
            }

            // Restart with new chunk manager
            startInit(opCtx);
            return;
        } catch (NetworkException& e) {
            LOGV2_WARNING(22701,
                          "Error initializing pcursor on {shardId} with current connection state "
                          "{connectionState} caused by {error}",
                          "Error initializing pcursor",
                          "shardId"_attr = shardId,
                          "connectionState"_attr = mdata.toBSON(),
                          "error"_attr = redact(e));
            mdata.errored = true;
            if (returnPartial) {
                mdata.cleanup(true);
                continue;
            }
            throw;
        } catch (DBException& e) {
            LOGV2_WARNING(22702,
                          "Error initializing pcursor on {shardId} with current connection state "
                          "{connectionState} caused by {error}",
                          "Error initializing pcursor",
                          "shardId"_attr = shardId,
                          "connectionState"_attr = mdata.toBSON(),
                          "error"_attr = redact(e));
            mdata.errored = true;
            if (returnPartial && e.code() == 15925 /* From above! */) {
                mdata.cleanup(true);
                continue;
            }
            throw;
        } catch (std::exception& e) {
            LOGV2_WARNING(22703,
                          "Error initializing pcursor on {shardId} with current connection state "
                          "{connectionState} caused by {error}",
                          "Error initializing pcursor",
                          "shardId"_attr = shardId,
                          "connectionState"_attr = mdata.toBSON(),
                          "error"_attr = redact(e.what()));
            mdata.errored = true;
            throw;
        } catch (...) {
            LOGV2_WARNING(22704,
                          "Error initializing pcursor on {shardId} with current connection state "
                          "{connectionState} caused by {error}",
                          "Error initializing pcursor",
                          "shardId"_attr = shardId,
                          "connectionState"_attr = mdata.toBSON(),
                          "error"_attr = "uknownError");
            mdata.errored = true;
            throw;
        }
    }

    // Sanity check final init'ed connections
    for (const auto& cmEntry : _cursorMap) {
        const auto& shardId = cmEntry.first;
        const auto& mdata = cmEntry.second;

        if (!mdata.pcState) {
            continue;
        }

        // Make sure all state is in shards
        invariant(shardIds.find(shardId) != shardIds.end());
        invariant(mdata.initialized == true);

        if (!mdata.completed) {
            invariant(mdata.pcState->conn->ok());
        }

        invariant(mdata.pcState->cursor);
        invariant(mdata.pcState->primary || mdata.pcState->manager);
        invariant(!mdata.retryNext);

        if (mdata.completed) {
            invariant(mdata.finished);
        }

        if (mdata.finished) {
            invariant(mdata.initialized);
        }

        if (!returnPartial) {
            invariant(mdata.initialized);
        }
    }
}

void ParallelSortClusteredCursor::finishInit(OperationContext* opCtx) {
    bool returnPartial = (_qSpec.options() & QueryOption_PartialResults);
    bool specialVersion = _cInfo.versionedNS.size() > 0;
    string ns = specialVersion ? _cInfo.versionedNS : _qSpec.ns();

    bool retry = false;
    map<string, StaleConfigException> staleNSExceptions;

    LOGV2_DEBUG(22685,
                2,
                "pcursor: finishing over {shardsCount} shards",
                "pcursor: finishing initialization",
                "shardsCount"_attr = _cursorMap.size());

    for (auto& cmEntry : _cursorMap) {
        const auto& shardId = cmEntry.first;
        auto& mdata = cmEntry.second;

        LOGV2_DEBUG(22686,
                    2,
                    "pcursor: finishing initialization on shard {shardId}, current connection "
                    "state is {connectionState}",
                    "pcursor: finishing initialization for shard",
                    "shardId"_attr = shardId,
                    "connectionState"_attr = mdata.toBSON());

        // Ignore empty conns for now
        if (!mdata.pcState)
            continue;

        auto state = mdata.pcState;

        try {
            // Sanity checks
            if (!mdata.completed)
                verify(state->conn && state->conn->ok());
            verify(state->cursor);
            verify(state->manager || state->primary);
            verify(!state->manager || !state->primary);


            // If we weren't init'ing lazily, ignore this
            if (!mdata.finished) {
                mdata.finished = true;

                // Mark the cursor as non-retry by default
                mdata.retryNext = false;

                if (!state->cursor->initLazyFinish(mdata.retryNext)) {
                    if (!mdata.retryNext) {
                        uassert(15988, "error querying server", false);
                    } else {
                        retry = true;
                        continue;
                    }
                }

                mdata.completed = false;
            }

            if (!mdata.completed) {
                mdata.completed = true;

                // Make sure we didn't get an error we should rethrow
                // TODO : Refactor this to something better
                throwCursorStale(state->cursor.get());
                throwCursorError(state->cursor.get());

                // Finalize state
                state->cursor->attach(state->conn.get());  // Closes connection for us

                LOGV2_DEBUG(22687,
                            2,
                            "pcursor: finished on shard {shardId}, current connection state is "
                            "{connectionState}",
                            "pcursor: finished initialization for shard",
                            "shardId"_attr = shardId,
                            "connectionState"_attr = mdata.toBSON());
            }
        } catch (StaleConfigException& e) {
            retry = true;

            std::string staleNS = e->getNss().ns();

            // Will retry all at once
            staleNSExceptions.emplace(staleNS, e);

            // Fully clear this cursor, as it needs to be re-established
            mdata.cleanup(true);
            continue;
        } catch (NetworkException& e) {
            LOGV2_WARNING(22705,
                          "Error finalizing pcursor initialization for shard {shardId} with "
                          "connection state {connectionState} caused by {error}",
                          "Error finalizing pcursor initialization for shard",
                          "shardId"_attr = shardId,
                          "connectionState"_attr = mdata.toBSON(),
                          "error"_attr = redact(e));
            mdata.errored = true;
            if (returnPartial) {
                mdata.cleanup(true);
                continue;
            }
            throw;
        } catch (DBException& e) {
            // NOTE: RECV() WILL NOT THROW A SOCKET EXCEPTION - WE GET THIS AS ERROR 15988 FROM
            // ABOVE
            if (e.code() == 15988) {
                LOGV2_WARNING(22706,
                              "Error finalizing pcursor initialization for shard {shardId} with "
                              "connection state {connectionState} caused by {error}",
                              "Error finalizing pcursor initialization for shard",
                              "shardId"_attr = shardId,
                              "connectionState"_attr = mdata.toBSON(),
                              "error"_attr = redact(e));

                mdata.errored = true;
                if (returnPartial) {
                    mdata.cleanup(true);
                    continue;
                }
                throw;
            } else {
                // the InvalidBSON exception indicates that the BSON is malformed ->
                // don't print/call "mdata.toBSON()" to avoid unexpected errors e.g. a segfault
                if (e.code() == ErrorCodes::InvalidBSON)
                    LOGV2_WARNING(22707,
                                  "Error finalizing pcursor initialization for shard "
                                  "{shardId} caused by {error}",
                                  "Error finalizing pcursor initialization for shard",
                                  "shardId"_attr = shardId,
                                  "error"_attr = redact(e));
                else
                    LOGV2_WARNING(
                        22708,
                        "Error finalizing pcursor initialization for shard {shardId} with "
                        "connection state {connectionState} caused by {error}",
                        "Error finalizing pcursor initialization for shard",
                        "shardId"_attr = shardId,
                        "connectionState"_attr = mdata.toBSON(),
                        "error"_attr = redact(e));
                mdata.errored = true;
                throw;
            }
        } catch (std::exception& e) {
            LOGV2_WARNING(22709,
                          "Error finalizing pcursor initialization for shard {shardId} with "
                          "connection state {connectionState} caused by {error}",
                          "Error finalizing pcursor initialization for shard",
                          "shardId"_attr = shardId,
                          "connectionState"_attr = mdata.toBSON(),
                          "error"_attr = redact(e.what()));
            mdata.errored = true;
            throw;
        } catch (...) {
            LOGV2_WARNING(
                22710,
                "Unknown error finalizing pcursor initialization for shard {shardId} with "
                "connection state {connectionState}",
                "Unknown error finalizing pcursor initialization for shard",
                "shardId"_attr = shardId,
                "connectionState"_attr = mdata.toBSON());
            mdata.errored = true;
            throw;
        }
    }

    // Retry logic for single refresh of namespaces / retry init'ing connections
    if (retry) {
        // Refresh stale namespaces
        if (staleNSExceptions.size()) {
            for (const auto& exEntry : staleNSExceptions) {
                const NamespaceString staleNS(exEntry.first);
                const StaleConfigException& ex = exEntry.second;

                _markStaleNS(staleNS, ex);
                // If we don't have a shardId (a valid case being receiving an exception from a
                // v4.2 binary), we have to set the entire collection as stale.
                if (!ex->getShardId()) {
                    Grid::get(opCtx)->catalogCache()->onEpochChange(staleNS);
                } else {
                    Grid::get(opCtx)->catalogCache()->invalidateShardForShardedCollection(
                        staleNS, *ex->getShardId());
                }

                LOGV2_DEBUG(22688,
                            1,
                            "Found stale config for namespace {namespace} on finishing query "
                            "caused by {error}",
                            "Found stale config for namespace",
                            "namespace"_attr = staleNS,
                            "error"_attr = redact(ex));

                // This is somewhat strange
                if (staleNS.ns() != ns) {
                    LOGV2_WARNING(22711,
                                  "Versioned namespace {namespace} doesn't match stale config "
                                  "namespace {staleNamespace}",
                                  "Versioned namespace doesn't match stale config namespace",
                                  "namespace"_attr = ns,
                                  "staleNamespace"_attr = staleNS);
                }
            }
        }

        // Re-establish connections we need to
        startInit(opCtx);
        finishInit(opCtx);
        return;
    }

    // Sanity check and clean final connections
    for (auto i = _cursorMap.begin(); i != _cursorMap.end();) {
        auto& mdata = i->second;

        // Erase empty stuff
        if (!mdata.pcState) {
            LOGV2(22689,
                  "PCursor erasing empty connection state {connectionState}",
                  "PCursor erasing empty connection state",
                  "connectionState"_attr = mdata.toBSON());
            _cursorMap.erase(i++);
            continue;
        } else {
            ++i;
        }

        // Make sure all state is in shards
        verify(mdata.initialized == true);
        verify(mdata.finished == true);
        verify(mdata.completed == true);
        verify(!mdata.pcState->conn->ok());
        verify(mdata.pcState->cursor);
        verify(mdata.pcState->primary || mdata.pcState->manager);
    }

    // TODO : More cleanup of metadata?

    // LEGACY STUFF NOW

    _cursors = new DBClientCursorHolder[_cursorMap.size()];

    // Put the cursors in the legacy format
    int index = 0;
    for (auto& cmEntry : _cursorMap) {
        const auto& shardId = cmEntry.first;
        auto& mdata = cmEntry.second;

        _cursors[index].reset(mdata.pcState->cursor.get(), &mdata);

        const auto shard =
            uassertStatusOK(Grid::get(opCtx)->shardRegistry()->getShard(opCtx, shardId));
        _servers.insert(shard->getConnString().toString());

        index++;
    }

    _numServers = _cursorMap.size();
}

void ParallelSortClusteredCursor::getQueryShardIds(set<ShardId>& shardIds) const {
    std::transform(_cursorMap.begin(),
                   _cursorMap.end(),
                   std::inserter(shardIds, shardIds.end()),
                   [](const ShardCursorsMap::value_type& pair) { return pair.first; });
}

std::shared_ptr<DBClientCursor> ParallelSortClusteredCursor::getShardCursor(
    const ShardId& shardId) const {
    auto it = _cursorMap.find(shardId);
    if (it == _cursorMap.end()) {
        return nullptr;
    }

    return it->second.pcState->cursor;
}

// DEPRECATED (but still used by map/reduce)
void ParallelSortClusteredCursor::_oldInit(OperationContext* opCtx) {
    // make sure we're not already initialized
    verify(!_cursors);
    _cursors = new DBClientCursorHolder[_numServers];

    bool returnPartial = (_options & QueryOption_PartialResults);

    vector<string> serverHosts(_servers.begin(), _servers.end());
    set<int> retryQueries;
    int finishedQueries = 0;

    vector<shared_ptr<ShardConnection>> conns;
    vector<string> servers;

    // Since we may get all sorts of errors, record them all as they come and throw them later if
    // necessary
    vector<string> staleConfigExs;
    vector<string> socketExs;
    vector<string> otherExs;
    bool allConfigStale = false;

    int retries = -1;

    // Loop through all the queries until we've finished or gotten a socket exception on all of them
    // We break early for non-socket exceptions, and socket exceptions if we aren't returning
    // partial results
    do {
        retries++;

        bool firstPass = retryQueries.size() == 0;

        if (!firstPass) {
            vector<StringData> serverHostsToRetry;
            serverHostsToRetry.reserve(retryQueries.size());
            transform(retryQueries.begin(),
                      retryQueries.end(),
                      std::back_inserter(serverHostsToRetry),
                      [&](const auto& query) { return serverHosts[query]; });
            LOGV2(22690,
                  "Finished queries {finishedQueriesCount}, retrying parallel connection to "
                  "{serverHostsToRetry}",
                  "Retrying parallel connection",
                  "returnPartial"_attr = returnPartial,
                  "finishedQueriesCount"_attr = finishedQueries,
                  "serverHostsToRetry"_attr = serverHostsToRetry);
        }

        size_t num = 0;
        for (vector<string>::const_iterator it = serverHosts.begin(); it != serverHosts.end();
             ++it) {
            size_t i = num++;

            const string& serverHost = *it;

            // If we're not retrying this cursor on later passes, continue
            if (!firstPass && retryQueries.find(i) == retryQueries.end())
                continue;

            const string errLoc = " @ " + serverHost;

            if (firstPass) {
                // This may be the first time connecting to this shard, if so we can get an error
                // here
                try {
                    conns.push_back(shared_ptr<ShardConnection>(new ShardConnection(
                        opCtx, uassertStatusOK(ConnectionString::parse(serverHost)), _ns)));
                } catch (std::exception& e) {
                    socketExs.push_back(e.what() + errLoc);
                    if (!returnPartial) {
                        num--;
                        break;
                    }

                    conns.push_back(shared_ptr<ShardConnection>());
                    continue;
                }

                servers.push_back(serverHost);
            }

            if (conns[i]->setVersion()) {
                conns[i]->done();

                // Version is zero b/c this is deprecated codepath
                staleConfigExs.push_back(str::stream() << "stale config detected for " << _ns
                                                       << " in ParallelCursor::_init " << errLoc);
                break;
            }

            LOGV2_DEBUG(22693,
                        5,
                        "ParallelSortClusteredCursor initialization",
                        "serverHost"_attr = serverHost,
                        "namespace"_attr = _ns,
                        "query"_attr = redact(_query),
                        "fields"_attr = redact(_fields),
                        "options"_attr = _options);

            if (!_cursors[i].get())
                _cursors[i].reset(
                    new DBClientCursor(conns[i]->get(),
                                       NamespaceString(_ns),
                                       _query,
                                       0,                                       // nToReturn
                                       0,                                       // nToSkip
                                       _fields.isEmpty() ? nullptr : &_fields,  // fieldsToReturn
                                       _options,
                                       _batchSize == 0 ? 0 : _batchSize + _needToSkip  // batchSize
                                       ),
                    nullptr);

            try {
                _cursors[i].get()->initLazy(!firstPass);
            } catch (NetworkException& e) {
                socketExs.push_back(e.what() + errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                if (!returnPartial)
                    break;
            } catch (std::exception& e) {
                otherExs.push_back(e.what() + errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                break;
            }
        }

        // Go through all the potentially started cursors and finish initializing them or log any
        // errors and potentially retry
        // TODO:  Better error classification would make this easier, errors are indicated in all
        // sorts of ways here that we need to trap.
        for (size_t i = 0; i < num; i++) {
            const string errLoc = " @ " + serverHosts[i];

            if (!_cursors[i].get() || (!firstPass && retryQueries.find(i) == retryQueries.end())) {
                if (conns[i])
                    conns[i].get()->done();
                continue;
            }

            verify(conns[i]);
            retryQueries.erase(i);

            bool retry = false;

            try {
                if (!_cursors[i].get()->initLazyFinish(retry)) {
                    LOGV2_WARNING(22712,
                                  "Got invalid result from {host}",
                                  "Got invalid result",
                                  "host"_attr = conns[i]->getHost(),
                                  "retrying"_attr = retry);
                    _cursors[i].reset(nullptr, nullptr);

                    if (!retry) {
                        socketExs.push_back(str::stream()
                                            << "error querying server: " << servers[i]);
                        conns[i]->done();
                    } else {
                        retryQueries.insert(i);
                    }

                    continue;
                }
            } catch (StaleConfigException& e) {
                // Our stored configuration data is actually stale, we need to reload it
                // when we throw our exception
                allConfigStale = true;

                staleConfigExs.push_back(
                    (string) "stale config detected when receiving response for " + e.toString() +
                    errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                continue;
            } catch (NetworkException& e) {
                socketExs.push_back(e.what() + errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                continue;
            } catch (std::exception& e) {
                otherExs.push_back(e.what() + errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                continue;
            }

            try {
                _cursors[i].get()->attach(conns[i].get());  // this calls done on conn
                // Rethrow stale config or other errors
                throwCursorStale(_cursors[i].get());
                throwCursorError(_cursors[i].get());

                finishedQueries++;
            } catch (StaleConfigException& e) {
                // Our stored configuration data is actually stale, we need to reload it
                // when we throw our exception
                allConfigStale = true;

                staleConfigExs.push_back((string) "stale config detected for " + e.toString() +
                                         errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                continue;
            } catch (std::exception& e) {
                otherExs.push_back(e.what() + errLoc);
                _cursors[i].reset(nullptr, nullptr);
                conns[i]->done();
                continue;
            }
        }

        // Don't exceed our max retries, should not happen
        verify(retries < 5);
    } while (retryQueries.size() > 0 /* something to retry */ &&
             (socketExs.size() == 0 || returnPartial) /* no conn issues */ &&
             staleConfigExs.size() == 0 /* no config issues */ &&
             otherExs.size() == 0 /* no other issues */);

    // Assert that our conns are all closed!
    for (vector<shared_ptr<ShardConnection>>::iterator i = conns.begin(); i < conns.end(); ++i) {
        verify(!(*i) || !(*i)->ok());
    }

    // Handle errors we got during initialization.
    // If we're returning partial results, we can ignore socketExs, but nothing else
    // Log a warning in any case, so we don't lose these messages
    bool throwException = (socketExs.size() > 0 && !returnPartial) || staleConfigExs.size() > 0 ||
        otherExs.size() > 0;

    if (socketExs.size() > 0 || staleConfigExs.size() > 0 || otherExs.size() > 0) {
        vector<string> errMsgs;

        errMsgs.insert(errMsgs.end(), staleConfigExs.begin(), staleConfigExs.end());
        errMsgs.insert(errMsgs.end(), otherExs.begin(), otherExs.end());
        errMsgs.insert(errMsgs.end(), socketExs.begin(), socketExs.end());

        std::stringstream errMsg;
        errMsg << "could not initialize cursor across all shards because : ";
        for (vector<string>::iterator i = errMsgs.begin(); i != errMsgs.end(); i++) {
            if (i != errMsgs.begin())
                errMsg << " :: and :: ";
            errMsg << *i;
        }

        if (throwException && staleConfigExs.size() > 0) {
            // Version is zero b/c this is deprecated codepath
            uasserted(StaleConfigInfo(NamespaceString(_ns),
                                      ChunkVersion(0, 0, OID()),
                                      ChunkVersion(0, 0, OID()),
                                      boost::none),
                      errMsg.str());
        } else if (throwException) {
            uasserted(14827, errMsg.str());
        } else {
            LOGV2_WARNING(22713,
                          "Error while initializing cursor across all shards caused by {error}",
                          "Error while initializing cursor across all shards",
                          "error"_attr = redact(errMsg.str()));
        }
    }

    if (retries > 0)
        LOGV2(22694,
              "Successfully finished parallel query after {retries} retries",
              "Successfully finished parallel query",
              "retries"_attr = retries);
}

bool ParallelSortClusteredCursor::more() {
    if (_needToSkip > 0) {
        int n = _needToSkip;
        _needToSkip = 0;

        while (n > 0 && more()) {
            next();
            n--;
        }

        _needToSkip = n;
    }

    for (int i = 0; i < _numServers; i++) {
        if (_cursors[i].get() && _cursors[i].get()->more())
            return true;
    }

    return false;
}

BSONObj ParallelSortClusteredCursor::next() {
    BSONObj best = BSONObj();
    int bestFrom = -1;

    for (int j = 0; j < _numServers; j++) {
        // Iterate _numServers times, starting one past the last server we used.
        // This means we actually start at server #1, not #0, but shouldn't matter

        int i = (j + _lastFrom + 1) % _numServers;

        // Check to see if the cursor is finished
        if (!_cursors[i].get() || !_cursors[i].get()->more()) {
            if (_cursors[i].getMData())
                _cursors[i].getMData()->pcState->done = true;
            continue;
        }

        // We know we have at least one result in this cursor
        BSONObj me = _cursors[i].get()->peekFirst();

        // If this is the first non-empty cursor, save the result as best
        if (bestFrom < 0) {
            best = me;
            bestFrom = i;
            if (_sortKey.isEmpty())
                break;
            continue;
        }

        // Otherwise compare the result to the current best result
        int comp = dps::compareObjectsAccordingToSort(best, me, _sortKey, true);
        if (comp < 0)
            continue;

        best = me;
        bestFrom = i;
    }

    _lastFrom = bestFrom;

    uassert(10019, "no more elements", bestFrom >= 0);
    _cursors[bestFrom].get()->next();

    // Make sure the result data won't go away after the next call to more()
    if (!_cursors[bestFrom].get()->moreInCurrentBatch()) {
        best = best.getOwned();
    }

    if (_cursors[bestFrom].getMData())
        _cursors[bestFrom].getMData()->pcState->count++;

    return best;
}

void ParallelConnectionMetadata::cleanup(bool full) {
    if (full || errored)
        retryNext = false;

    if (!retryNext && pcState) {
        if (initialized && !errored) {
            verify(pcState->cursor);
            verify(pcState->conn);

            if (!finished && pcState->conn->ok()) {
                try {
                    // Complete the call if only halfway done
                    bool retry = false;
                    pcState->cursor->initLazyFinish(retry);
                } catch (std::exception&) {
                    LOGV2_WARNING(22714, "Exception closing cursor");
                } catch (...) {
                    LOGV2_WARNING(22715, "Unknown exception closing cursor");
                }
            }
        }

        // Double-check conn is closed
        if (pcState->conn) {
            pcState->conn->done();
        }

        pcState.reset();
    } else
        verify(finished || !initialized);

    initialized = false;
    finished = false;
    completed = false;
    errored = false;
}

BSONObj ParallelConnectionMetadata::toBSON() const {
    return BSON("state" << (pcState ? pcState->toBSON() : BSONObj()) << "retryNext" << retryNext
                        << "init" << initialized << "finish" << finished << "errored" << errored);
}

std::string ParallelConnectionState::toString() const {
    return str::stream() << "PCState : " << toBSON();
}

BSONObj ParallelConnectionState::toBSON() const {
    BSONObj cursorPeek = BSON("no cursor"
                              << "");
    if (cursor) {
        vector<BSONObj> v;
        cursor->peek(v, 1);
        if (v.size() == 0)
            cursorPeek = BSON("no data"
                              << "");
        else
            cursorPeek = BSON("" << v[0]);
    }

    BSONObj stateObj =
        BSON("conn" << (conn ? (conn->ok() ? conn->conn().toString() : "(done)") : "") << "vinfo"
                    << (manager ? (str::stream() << manager->getns().ns() << " @ "
                                                 << manager->getVersion().toString())
                                : primary->toString()));

    // Append cursor data if exists
    BSONObjBuilder stateB;
    stateB.appendElements(stateObj);
    if (!cursor)
        stateB.append("cursor", "(none)");
    else {
        vector<BSONObj> v;
        cursor->peek(v, 1);
        if (v.size() == 0)
            stateB.append("cursor", "(empty)");
        else
            stateB.append("cursor", v[0]);
    }

    stateB.append("count", count);
    stateB.append("done", done);

    return stateB.obj().getOwned();
}

void throwCursorStale(DBClientCursor* cursor) {
    invariant(cursor);

    if (cursor->hasResultFlag(ResultFlag_ShardConfigStale)) {
        BSONObj error;
        cursor->peekError(&error);
        uasserted(StaleConfigInfo::parseFromCommandError(error),
                  "query returned a stale config error");
    }

    if (NamespaceString(cursor->getns()).isCommand()) {
        // Commands that care about versioning (like the count or geoNear command) sometimes return
        // with the stale config error code, but don't set the ShardConfigStale result flag on the
        // cursor.
        //
        // TODO: Standardize stale config reporting.
        BSONObj res = cursor->peekFirst();
        auto status = getStatusFromCommandResult(res);
        if (status == ErrorCodes::StaleConfig) {
            uassertStatusOK(status.withContext("command returned a stale config error"));
        }
    }
}

}  // namespace mongo
