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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kSharding

#include "mongo/platform/basic.h"

#include "mongo/db/s/migration_chunk_cloner_source_legacy.h"

#include "mongo/base/status.h"
#include "mongo/client/read_preference.h"
#include "mongo/db/catalog/index_catalog.h"
#include "mongo/db/catalog_raii.h"
#include "mongo/db/dbhelpers.h"
#include "mongo/db/exec/working_set_common.h"
#include "mongo/db/index/index_descriptor.h"
#include "mongo/db/repl/optime.h"
#include "mongo/db/repl/replication_process.h"
#include "mongo/db/s/collection_sharding_runtime.h"
#include "mongo/db/s/migration_source_manager.h"
#include "mongo/db/s/sharding_statistics.h"
#include "mongo/db/s/start_chunk_clone_request.h"
#include "mongo/db/service_context.h"
#include "mongo/executor/remote_command_request.h"
#include "mongo/executor/remote_command_response.h"
#include "mongo/executor/task_executor.h"
#include "mongo/executor/task_executor_pool.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/get_status_from_command_result.h"
#include "mongo/s/client/shard_registry.h"
#include "mongo/s/grid.h"
#include "mongo/util/elapsed_tracker.h"
#include "mongo/util/scopeguard.h"
#include "mongo/util/str.h"
#include "mongo/util/time_support.h"

namespace mongo {
namespace {

const char kRecvChunkStatus[] = "_recvChunkStatus";
const char kRecvChunkCommit[] = "_recvChunkCommit";
const char kRecvChunkAbort[] = "_recvChunkAbort";

const int kMaxObjectPerChunk{250000};
const Hours kMaxWaitToCommitCloneForJumboChunk(6);

MONGO_FAIL_POINT_DEFINE(failTooMuchMemoryUsed);

bool isInRange(const BSONObj& obj,
               const BSONObj& min,
               const BSONObj& max,
               const ShardKeyPattern& shardKeyPattern) {
    BSONObj k = shardKeyPattern.extractShardKeyFromDoc(obj);
    return k.woCompare(min) >= 0 && k.woCompare(max) < 0;
}

BSONObj createRequestWithSessionId(StringData commandName,
                                   const NamespaceString& nss,
                                   const MigrationSessionId& sessionId,
                                   bool waitForSteadyOrDone = false) {
    BSONObjBuilder builder;
    builder.append(commandName, nss.ns());
    builder.append("waitForSteadyOrDone", waitForSteadyOrDone);
    sessionId.append(&builder);
    return builder.obj();
}

BSONObj getDocumentKeyFromReplOperation(repl::ReplOperation replOperation,
                                        repl::OpTypeEnum opType) {
    switch (opType) {
        case repl::OpTypeEnum::kInsert:
        case repl::OpTypeEnum::kDelete:
            return replOperation.getObject();
        case repl::OpTypeEnum::kUpdate:
            return *replOperation.getObject2();
        default:
            MONGO_UNREACHABLE;
    }
    MONGO_UNREACHABLE;
}

char getOpCharForCrudOpType(repl::OpTypeEnum opType) {
    switch (opType) {
        case repl::OpTypeEnum::kInsert:
            return 'i';
        case repl::OpTypeEnum::kUpdate:
            return 'u';
        case repl::OpTypeEnum::kDelete:
            return 'd';
        default:
            MONGO_UNREACHABLE;
    }
    MONGO_UNREACHABLE;
}

}  // namespace

/**
 * Used to commit work for LogOpForSharding. Used to keep track of changes in documents that are
 * part of a chunk being migrated.
 */
class LogOpForShardingHandler final : public RecoveryUnit::Change {
public:
    /**
     * Invariant: idObj should belong to a document that is part of the active chunk being migrated
     */
    LogOpForShardingHandler(MigrationChunkClonerSourceLegacy* cloner,
                            const BSONObj& idObj,
                            const char op,
                            const repl::OpTime& opTime,
                            const repl::OpTime& prePostImageOpTime)
        : _cloner(cloner),
          _idObj(idObj.getOwned()),
          _op(op),
          _opTime(opTime),
          _prePostImageOpTime(prePostImageOpTime) {}

    void commit(boost::optional<Timestamp>) override {
        _cloner->_addToTransferModsQueue(_idObj, _op, _opTime, _prePostImageOpTime);
        _cloner->_decrementOutstandingOperationTrackRequests();
    }

    void rollback() override {
        _cloner->_decrementOutstandingOperationTrackRequests();
    }

private:
    MigrationChunkClonerSourceLegacy* const _cloner;
    const BSONObj _idObj;
    const char _op;
    const repl::OpTime _opTime;
    const repl::OpTime _prePostImageOpTime;
};

void LogTransactionOperationsForShardingHandler::commit(boost::optional<Timestamp>) {
    std::set<NamespaceString> namespacesTouchedByTransaction;

    for (const auto& stmt : _stmts) {
        const auto& nss = stmt.getNss();

        auto csr = CollectionShardingRuntime::get_UNSAFE(_svcCtx, nss);

        auto opCtx = cc().getOperationContext();
        auto csrLock = CollectionShardingRuntime::CSRLock::lockShared(opCtx, csr);

        auto msm = MigrationSourceManager::get(csr, csrLock);
        if (!msm) {
            continue;
        }

        auto cloner = dynamic_cast<MigrationChunkClonerSourceLegacy*>(msm->getCloner().get());

        auto opType = stmt.getOpType();
        auto documentKey = getDocumentKeyFromReplOperation(stmt, opType);

        auto idElement = documentKey["_id"];
        if (idElement.eoo()) {
            LOGV2_WARNING(21994,
                          "Received a document without an _id field, ignoring: {documentKey}",
                          "Received a document without an _id and will ignore that document",
                          "documentKey"_attr = redact(documentKey));
            continue;
        }

        auto const& minKey = cloner->_args.getMinKey();
        auto const& maxKey = cloner->_args.getMaxKey();
        auto const& shardKeyPattern = cloner->_shardKeyPattern;

        if (!isInRange(documentKey, minKey, maxKey, shardKeyPattern)) {
            // If the preImageDoc is not in range but the postImageDoc was, we know that the
            // document has changed shard keys and no longer belongs in the chunk being cloned.
            // We will model the deletion of the preImage document so that the destination chunk
            // does not receive an outdated version of this document.
            if (opType == repl::OpTypeEnum::kUpdate &&
                isInRange(stmt.getPreImageDocumentKey(), minKey, maxKey, shardKeyPattern) &&
                !stmt.getPreImageDocumentKey()["_id"].eoo()) {
                opType = repl::OpTypeEnum::kDelete;
                idElement = stmt.getPreImageDocumentKey()["id"];
            } else {
                continue;
            }
        }

        // Inform the session migration subsystem that a transaction has committed for all involved
        // namespaces.
        if (namespacesTouchedByTransaction.find(nss) == namespacesTouchedByTransaction.end()) {
            cloner->_addToSessionMigrationOptimeQueue(
                _prepareOrCommitOpTime,
                SessionCatalogMigrationSource::EntryAtOpTimeType::kTransaction);

            namespacesTouchedByTransaction.emplace(nss);
        }

        // Pass an empty prePostOpTime to the queue because retryable write history doesn't care
        // about writes in transactions.
        cloner->_addToTransferModsQueue(idElement.wrap(), getOpCharForCrudOpType(opType), {}, {});
    }
}

MigrationChunkClonerSourceLegacy::MigrationChunkClonerSourceLegacy(MoveChunkRequest request,
                                                                   const BSONObj& shardKeyPattern,
                                                                   ConnectionString donorConnStr,
                                                                   HostAndPort recipientHost)
    : _args(std::move(request)),
      _shardKeyPattern(shardKeyPattern),
      _sessionId(MigrationSessionId::generate(_args.getFromShardId().toString(),
                                              _args.getToShardId().toString())),
      _donorConnStr(std::move(donorConnStr)),
      _recipientHost(std::move(recipientHost)),
      _forceJumbo(_args.getForceJumbo() != MoveChunkRequest::ForceJumbo::kDoNotForce) {}

MigrationChunkClonerSourceLegacy::~MigrationChunkClonerSourceLegacy() {
    invariant(_state == kDone);
}

Status MigrationChunkClonerSourceLegacy::startClone(OperationContext* opCtx,
                                                    const UUID& migrationId,
                                                    const LogicalSessionId& lsid,
                                                    TxnNumber txnNumber) {
    invariant(_state == kNew);
    invariant(!opCtx->lockState()->isLocked());

    auto const replCoord = repl::ReplicationCoordinator::get(opCtx);
    if (replCoord->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet) {
        _sessionCatalogSource = std::make_unique<SessionCatalogMigrationSource>(
            opCtx,
            _args.getNss(),
            ChunkRange(_args.getMinKey(), _args.getMaxKey()),
            _shardKeyPattern.getKeyPattern());

        // Prime up the session migration source if there are oplog entries to migrate.
        _sessionCatalogSource->fetchNextOplog(opCtx);
    }

    {
        // Ignore prepare conflicts when we load ids of currently available documents. This is
        // acceptable because we will track changes made by prepared transactions at transaction
        // commit time.
        auto originalPrepareConflictBehavior = opCtx->recoveryUnit()->getPrepareConflictBehavior();

        ON_BLOCK_EXIT([&] {
            opCtx->recoveryUnit()->setPrepareConflictBehavior(originalPrepareConflictBehavior);
        });

        opCtx->recoveryUnit()->setPrepareConflictBehavior(
            PrepareConflictBehavior::kIgnoreConflicts);

        auto storeCurrentLocsStatus = _storeCurrentLocs(opCtx);
        if (storeCurrentLocsStatus == ErrorCodes::ChunkTooBig && _forceJumbo) {
            stdx::lock_guard<Latch> sl(_mutex);
            _jumboChunkCloneState.emplace();
        } else if (!storeCurrentLocsStatus.isOK()) {
            return storeCurrentLocsStatus;
        }
    }

    // Tell the recipient shard to start cloning
    BSONObjBuilder cmdBuilder;

    StartChunkCloneRequest::appendAsCommand(&cmdBuilder,
                                            _args.getNss(),
                                            migrationId,
                                            lsid,
                                            txnNumber,
                                            _sessionId,
                                            _donorConnStr,
                                            _args.getFromShardId(),
                                            _args.getToShardId(),
                                            _args.getMinKey(),
                                            _args.getMaxKey(),
                                            _shardKeyPattern.toBSON(),
                                            _args.getSecondaryThrottle());

    if (serverGlobalParams.featureCompatibility.isVersion(
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44)) {
        // In 4.4, commands sent to shards that accept writeConcern, must always have writeConcern.
        // So if the StartChunkCloneRequest didn't add writeConcern (from secondaryThrottle), then
        // we add the implicit server default writeConcern.
        if (!cmdBuilder.hasField(WriteConcernOptions::kWriteConcernField)) {
            cmdBuilder.append(WriteConcernOptions::kWriteConcernField,
                              WriteConcernOptions::kImplicitDefault);
        }
    }

    auto startChunkCloneResponseStatus = _callRecipient(cmdBuilder.obj());
    if (!startChunkCloneResponseStatus.isOK()) {
        return startChunkCloneResponseStatus.getStatus();
    }

    // TODO (Kal): Setting the state to kCloning below means that if cancelClone was called we will
    // send a cancellation command to the recipient. The reason to limit the cases when we send
    // cancellation is for backwards compatibility with 3.2 nodes, which cannot differentiate
    // between cancellations for different migration sessions. It is thus possible that a second
    // migration from different donor, but the same recipient would certainly abort an already
    // running migration.
    stdx::lock_guard<Latch> sl(_mutex);
    _state = kCloning;

    return Status::OK();
}

// TODO (SERVER-44787): Remove this overload after 4.4 is released.
Status MigrationChunkClonerSourceLegacy::startClone(OperationContext* opCtx) {
    invariant(_state == kNew);
    invariant(!opCtx->lockState()->isLocked());

    auto const replCoord = repl::ReplicationCoordinator::get(opCtx);
    if (replCoord->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet) {
        _sessionCatalogSource = std::make_unique<SessionCatalogMigrationSource>(
            opCtx,
            _args.getNss(),
            ChunkRange(_args.getMinKey(), _args.getMaxKey()),
            _shardKeyPattern.getKeyPattern());

        // Prime up the session migration source if there are oplog entries to migrate.
        _sessionCatalogSource->fetchNextOplog(opCtx);
    }

    {
        // Ignore prepare conflicts when we load ids of currently available documents. This is
        // acceptable because we will track changes made by prepared transactions at transaction
        // commit time.
        auto originalPrepareConflictBehavior = opCtx->recoveryUnit()->getPrepareConflictBehavior();

        ON_BLOCK_EXIT([&] {
            opCtx->recoveryUnit()->setPrepareConflictBehavior(originalPrepareConflictBehavior);
        });

        opCtx->recoveryUnit()->setPrepareConflictBehavior(
            PrepareConflictBehavior::kIgnoreConflicts);

        auto storeCurrentLocsStatus = _storeCurrentLocs(opCtx);
        if (storeCurrentLocsStatus == ErrorCodes::ChunkTooBig && _forceJumbo) {
            stdx::lock_guard<Latch> sl(_mutex);
            _jumboChunkCloneState.emplace();
        } else if (!storeCurrentLocsStatus.isOK()) {
            return storeCurrentLocsStatus;
        }
    }

    // Tell the recipient shard to start cloning
    BSONObjBuilder cmdBuilder;

    StartChunkCloneRequest::appendAsCommand(&cmdBuilder,
                                            _args.getNss(),
                                            _sessionId,
                                            _donorConnStr,
                                            _args.getFromShardId(),
                                            _args.getToShardId(),
                                            _args.getMinKey(),
                                            _args.getMaxKey(),
                                            _shardKeyPattern.toBSON(),
                                            _args.getSecondaryThrottle());

    if (serverGlobalParams.featureCompatibility.isVersion(
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44)) {
        // In 4.4, commands sent to shards that accept writeConcern, must always have writeConcern.
        // So if the StartChunkCloneRequest didn't add writeConcern (from secondaryThrottle), then
        // we add the implicit server default writeConcern.
        if (!cmdBuilder.hasField(WriteConcernOptions::kWriteConcernField)) {
            cmdBuilder.append(WriteConcernOptions::kWriteConcernField,
                              WriteConcernOptions::kImplicitDefault);
        }
    }

    auto startChunkCloneResponseStatus = _callRecipient(cmdBuilder.obj());
    if (!startChunkCloneResponseStatus.isOK()) {
        return startChunkCloneResponseStatus.getStatus();
    }

    // TODO (Kal): Setting the state to kCloning below means that if cancelClone was called we will
    // send a cancellation command to the recipient. The reason to limit the cases when we send
    // cancellation is for backwards compatibility with 3.2 nodes, which cannot differentiate
    // between cancellations for different migration sessions. It is thus possible that a second
    // migration from different donor, but the same recipient would certainly abort an already
    // running migration.
    stdx::lock_guard<Latch> sl(_mutex);
    _state = kCloning;

    return Status::OK();
}

Status MigrationChunkClonerSourceLegacy::awaitUntilCriticalSectionIsAppropriate(
    OperationContext* opCtx, Milliseconds maxTimeToWait) {
    invariant(_state == kCloning);
    invariant(!opCtx->lockState()->isLocked());
    // If this migration is manual migration that specified "force", enter the critical section
    // immediately. This means the entire cloning phase will be done under the critical section.
    if (_jumboChunkCloneState &&
        _args.getForceJumbo() == MoveChunkRequest::ForceJumbo::kForceManual) {
        return Status::OK();
    }

    return _checkRecipientCloningStatus(opCtx, maxTimeToWait);
}

StatusWith<BSONObj> MigrationChunkClonerSourceLegacy::commitClone(OperationContext* opCtx) {
    invariant(_state == kCloning);
    invariant(!opCtx->lockState()->isLocked());
    if (_jumboChunkCloneState && _forceJumbo) {
        if (_args.getForceJumbo() == MoveChunkRequest::ForceJumbo::kForceManual) {
            auto status = _checkRecipientCloningStatus(opCtx, kMaxWaitToCommitCloneForJumboChunk);
            if (!status.isOK()) {
                return status;
            }
        } else {
            invariant(PlanExecutor::IS_EOF == _jumboChunkCloneState->clonerState);
            invariant(_cloneLocs.empty());
        }
    }

    if (_sessionCatalogSource) {
        _sessionCatalogSource->onCommitCloneStarted();
    }

    auto responseStatus =
        _callRecipient(createRequestWithSessionId(kRecvChunkCommit, _args.getNss(), _sessionId));

    if (responseStatus.isOK()) {
        _cleanup(opCtx);

        if (_sessionCatalogSource && _sessionCatalogSource->hasMoreOplog()) {
            return {ErrorCodes::SessionTransferIncomplete,
                    "destination shard finished committing but there are still some session "
                    "metadata that needs to be transferred"};
        }

        return responseStatus;
    }

    cancelClone(opCtx);
    return responseStatus.getStatus();
}

void MigrationChunkClonerSourceLegacy::cancelClone(OperationContext* opCtx) {
    invariant(!opCtx->lockState()->isLocked());

    if (_sessionCatalogSource) {
        _sessionCatalogSource->onCloneCleanup();
    }

    switch (_state) {
        case kDone:
            break;
        case kCloning: {
            const auto status = _callRecipient(createRequestWithSessionId(
                                                   kRecvChunkAbort, _args.getNss(), _sessionId))
                                    .getStatus();
            if (!status.isOK()) {
                LOGV2(21991,
                      "Failed to cancel migration: {error}",
                      "Failed to cancel migration",
                      "error"_attr = redact(status));
            }
        }
        // Intentional fall through
        case kNew:
            _cleanup(opCtx);
            break;
        default:
            MONGO_UNREACHABLE;
    }
}

bool MigrationChunkClonerSourceLegacy::isDocumentInMigratingChunk(const BSONObj& doc) {
    return isInRange(doc, _args.getMinKey(), _args.getMaxKey(), _shardKeyPattern);
}

void MigrationChunkClonerSourceLegacy::onInsertOp(OperationContext* opCtx,
                                                  const BSONObj& insertedDoc,
                                                  const repl::OpTime& opTime) {
    dassert(opCtx->lockState()->isCollectionLockedForMode(_args.getNss(), MODE_IX));

    BSONElement idElement = insertedDoc["_id"];
    if (idElement.eoo()) {
        LOGV2_WARNING(21995,
                      "logInsertOp received a document without an _id field, ignoring inserted "
                      "document: {insertedDoc}",
                      "logInsertOp received a document without an _id field and will ignore that "
                      "document",
                      "insertedDoc"_attr = redact(insertedDoc));
        return;
    }

    if (!isInRange(insertedDoc, _args.getMinKey(), _args.getMaxKey(), _shardKeyPattern)) {
        return;
    }

    if (!_addedOperationToOutstandingOperationTrackRequests()) {
        return;
    }

    if (opCtx->getTxnNumber()) {
        opCtx->recoveryUnit()->registerChange(std::make_unique<LogOpForShardingHandler>(
            this, idElement.wrap(), 'i', opTime, repl::OpTime()));
    } else {
        opCtx->recoveryUnit()->registerChange(std::make_unique<LogOpForShardingHandler>(
            this, idElement.wrap(), 'i', repl::OpTime(), repl::OpTime()));
    }
}

void MigrationChunkClonerSourceLegacy::onUpdateOp(OperationContext* opCtx,
                                                  boost::optional<BSONObj> preImageDoc,
                                                  const BSONObj& postImageDoc,
                                                  const repl::OpTime& opTime,
                                                  const repl::OpTime& prePostImageOpTime) {
    dassert(opCtx->lockState()->isCollectionLockedForMode(_args.getNss(), MODE_IX));

    BSONElement idElement = postImageDoc["_id"];
    if (idElement.eoo()) {
        LOGV2_WARNING(
            21996,
            "logUpdateOp received a document without an _id field, ignoring the updated document: "
            "{postImageDoc}",
            "logUpdateOp received a document without an _id field and will ignore that document",
            "postImageDoc"_attr = redact(postImageDoc));
        return;
    }

    if (!isInRange(postImageDoc, _args.getMinKey(), _args.getMaxKey(), _shardKeyPattern)) {
        // If the preImageDoc is not in range but the postImageDoc was, we know that the document
        // has changed shard keys and no longer belongs in the chunk being cloned. We will model
        // the deletion of the preImage document so that the destination chunk does not receive an
        // outdated version of this document.
        if (preImageDoc &&
            isInRange(*preImageDoc, _args.getMinKey(), _args.getMaxKey(), _shardKeyPattern)) {
            onDeleteOp(opCtx, *preImageDoc, opTime, prePostImageOpTime);
        }
        return;
    }

    if (!_addedOperationToOutstandingOperationTrackRequests()) {
        return;
    }

    if (opCtx->getTxnNumber()) {
        opCtx->recoveryUnit()->registerChange(std::make_unique<LogOpForShardingHandler>(
            this, idElement.wrap(), 'u', opTime, prePostImageOpTime));
    } else {
        opCtx->recoveryUnit()->registerChange(std::make_unique<LogOpForShardingHandler>(
            this, idElement.wrap(), 'u', repl::OpTime(), repl::OpTime()));
    }
}

void MigrationChunkClonerSourceLegacy::onDeleteOp(OperationContext* opCtx,
                                                  const BSONObj& deletedDocId,
                                                  const repl::OpTime& opTime,
                                                  const repl::OpTime& preImageOpTime) {
    dassert(opCtx->lockState()->isCollectionLockedForMode(_args.getNss(), MODE_IX));

    BSONElement idElement = deletedDocId["_id"];
    if (idElement.eoo()) {
        LOGV2_WARNING(
            21997,
            "logDeleteOp received a document without an _id field, ignoring deleted doc: "
            "{deletedDocId}",
            "logDeleteOp received a document without an _id field and will ignore that document",
            "deletedDocId"_attr = redact(deletedDocId));
        return;
    }

    if (!_addedOperationToOutstandingOperationTrackRequests()) {
        return;
    }

    if (opCtx->getTxnNumber()) {
        opCtx->recoveryUnit()->registerChange(std::make_unique<LogOpForShardingHandler>(
            this, idElement.wrap(), 'd', opTime, preImageOpTime));
    } else {
        opCtx->recoveryUnit()->registerChange(std::make_unique<LogOpForShardingHandler>(
            this, idElement.wrap(), 'd', repl::OpTime(), repl::OpTime()));
    }
}

void MigrationChunkClonerSourceLegacy::_addToSessionMigrationOptimeQueue(
    const repl::OpTime& opTime,
    SessionCatalogMigrationSource::EntryAtOpTimeType entryAtOpTimeType) {
    if (auto sessionSource = _sessionCatalogSource.get()) {
        if (!opTime.isNull()) {
            sessionSource->notifyNewWriteOpTime(opTime, entryAtOpTimeType);
        }
    }
}

void MigrationChunkClonerSourceLegacy::_addToTransferModsQueue(
    const BSONObj& idObj,
    const char op,
    const repl::OpTime& opTime,
    const repl::OpTime& prePostImageOpTime) {
    switch (op) {
        case 'd': {
            stdx::lock_guard<Latch> sl(_mutex);
            _deleted.push_back(idObj);
            _memoryUsed += idObj.firstElement().size() + 5;
        } break;

        case 'i':
        case 'u': {
            stdx::lock_guard<Latch> sl(_mutex);
            _reload.push_back(idObj);
            _memoryUsed += idObj.firstElement().size() + 5;
        } break;

        default:
            MONGO_UNREACHABLE;
    }

    _addToSessionMigrationOptimeQueue(
        prePostImageOpTime, SessionCatalogMigrationSource::EntryAtOpTimeType::kRetryableWrite);
    _addToSessionMigrationOptimeQueue(
        opTime, SessionCatalogMigrationSource::EntryAtOpTimeType::kRetryableWrite);
}

bool MigrationChunkClonerSourceLegacy::_addedOperationToOutstandingOperationTrackRequests() {
    stdx::unique_lock<Latch> lk(_mutex);
    if (!_acceptingNewOperationTrackRequests) {
        return false;
    }

    _incrementOutstandingOperationTrackRequests(lk);
    return true;
}

void MigrationChunkClonerSourceLegacy::_drainAllOutstandingOperationTrackRequests(
    stdx::unique_lock<Latch>& lk) {
    invariant(_state == kDone);
    _acceptingNewOperationTrackRequests = false;
    _allOutstandingOperationTrackRequestsDrained.wait(
        lk, [&] { return _outstandingOperationTrackRequests == 0; });
}


void MigrationChunkClonerSourceLegacy::_incrementOutstandingOperationTrackRequests(WithLock) {
    invariant(_acceptingNewOperationTrackRequests);
    ++_outstandingOperationTrackRequests;
}

void MigrationChunkClonerSourceLegacy::_decrementOutstandingOperationTrackRequests() {
    stdx::lock_guard<Latch> sl(_mutex);
    --_outstandingOperationTrackRequests;
    if (_outstandingOperationTrackRequests == 0) {
        _allOutstandingOperationTrackRequestsDrained.notify_all();
    }
}

void MigrationChunkClonerSourceLegacy::_nextCloneBatchFromIndexScan(OperationContext* opCtx,
                                                                    Collection* collection,
                                                                    BSONArrayBuilder* arrBuilder) {
    ElapsedTracker tracker(opCtx->getServiceContext()->getFastClockSource(),
                           internalQueryExecYieldIterations.load(),
                           Milliseconds(internalQueryExecYieldPeriodMS.load()));

    if (!_jumboChunkCloneState->clonerExec) {
        auto exec = uassertStatusOK(_getIndexScanExecutor(opCtx, collection));
        _jumboChunkCloneState->clonerExec = std::move(exec);
    } else {
        _jumboChunkCloneState->clonerExec->reattachToOperationContext(opCtx);
        _jumboChunkCloneState->clonerExec->restoreState();
    }

    BSONObj obj;
    RecordId recordId;
    PlanExecutor::ExecState execState;

    while (PlanExecutor::ADVANCED ==
           (execState = _jumboChunkCloneState->clonerExec->getNext(
                &obj, _jumboChunkCloneState->stashedRecordId ? nullptr : &recordId))) {

        stdx::unique_lock<Latch> lk(_mutex);
        _jumboChunkCloneState->clonerState = execState;
        lk.unlock();

        opCtx->checkForInterrupt();

        // Use the builder size instead of accumulating the document sizes directly so
        // that we take into consideration the overhead of BSONArray indices.
        if (arrBuilder->arrSize() &&
            (arrBuilder->len() + obj.objsize() + 1024) > BSONObjMaxUserSize) {
            _jumboChunkCloneState->clonerExec->enqueue(obj);

            // Stash the recordId we just read to add to the next batch.
            if (!recordId.isNull()) {
                invariant(!_jumboChunkCloneState->stashedRecordId);
                _jumboChunkCloneState->stashedRecordId = std::move(recordId);
            }

            break;
        }

        Snapshotted<BSONObj> doc;
        invariant(collection->findDoc(
            opCtx, _jumboChunkCloneState->stashedRecordId.value_or(recordId), &doc));
        arrBuilder->append(doc.value());
        _jumboChunkCloneState->stashedRecordId = boost::none;

        lk.lock();
        _jumboChunkCloneState->docsCloned++;
        lk.unlock();

        ShardingStatistics::get(opCtx).countDocsClonedOnDonor.addAndFetch(1);
    }

    stdx::unique_lock<Latch> lk(_mutex);
    _jumboChunkCloneState->clonerState = execState;
    lk.unlock();

    _jumboChunkCloneState->clonerExec->saveState();
    _jumboChunkCloneState->clonerExec->detachFromOperationContext();

    if (PlanExecutor::FAILURE == execState)
        uassertStatusOK(WorkingSetCommon::getMemberObjectStatus(obj).withContext(
            "Executor error while scanning for documents belonging to chunk"));
}

void MigrationChunkClonerSourceLegacy::_nextCloneBatchFromCloneLocs(OperationContext* opCtx,
                                                                    Collection* collection,
                                                                    BSONArrayBuilder* arrBuilder) {
    ElapsedTracker tracker(opCtx->getServiceContext()->getFastClockSource(),
                           internalQueryExecYieldIterations.load(),
                           Milliseconds(internalQueryExecYieldPeriodMS.load()));

    stdx::unique_lock<Latch> lk(_mutex);
    auto iter = _cloneLocs.begin();

    for (; iter != _cloneLocs.end(); ++iter) {
        // We must always make progress in this method by at least one document because empty
        // return indicates there is no more initial clone data.
        if (arrBuilder->arrSize() && tracker.intervalHasElapsed()) {
            break;
        }

        auto nextRecordId = *iter;

        lk.unlock();

        Snapshotted<BSONObj> doc;
        if (collection->findDoc(opCtx, nextRecordId, &doc)) {
            // Use the builder size instead of accumulating the document sizes directly so
            // that we take into consideration the overhead of BSONArray indices.
            if (arrBuilder->arrSize() &&
                (arrBuilder->len() + doc.value().objsize() + 1024) > BSONObjMaxUserSize) {

                break;
            }

            arrBuilder->append(doc.value());
            ShardingStatistics::get(opCtx).countDocsClonedOnDonor.addAndFetch(1);
        }

        lk.lock();
    }

    _cloneLocs.erase(_cloneLocs.begin(), iter);
}

uint64_t MigrationChunkClonerSourceLegacy::getCloneBatchBufferAllocationSize() {
    stdx::lock_guard<Latch> sl(_mutex);
    if (_jumboChunkCloneState && _forceJumbo)
        return static_cast<uint64_t>(BSONObjMaxUserSize);

    return std::min(static_cast<uint64_t>(BSONObjMaxUserSize),
                    _averageObjectSizeForCloneLocs * _cloneLocs.size());
}

Status MigrationChunkClonerSourceLegacy::nextCloneBatch(OperationContext* opCtx,
                                                        Collection* collection,
                                                        BSONArrayBuilder* arrBuilder) {
    dassert(opCtx->lockState()->isCollectionLockedForMode(_args.getNss(), MODE_IS));

    // If this chunk is too large to store records in _cloneLocs and the command args specify to
    // attempt to move it, scan the collection directly.
    if (_jumboChunkCloneState && _forceJumbo) {
        try {
            _nextCloneBatchFromIndexScan(opCtx, collection, arrBuilder);
            return Status::OK();
        } catch (const DBException& ex) {
            return ex.toStatus();
        }
    }

    _nextCloneBatchFromCloneLocs(opCtx, collection, arrBuilder);
    return Status::OK();
}

Status MigrationChunkClonerSourceLegacy::nextModsBatch(OperationContext* opCtx,
                                                       Database* db,
                                                       BSONObjBuilder* builder) {
    dassert(opCtx->lockState()->isCollectionLockedForMode(_args.getNss(), MODE_IS));

    std::list<BSONObj> deleteList;
    std::list<BSONObj> updateList;

    {
        // All clone data must have been drained before starting to fetch the incremental changes.
        stdx::unique_lock<Latch> lk(_mutex);
        invariant(_cloneLocs.empty());

        // The "snapshot" for delete and update list must be taken under a single lock. This is to
        // ensure that we will preserve the causal order of writes. Always consume the delete
        // buffer first, before the update buffer. If the delete is causally before the update to
        // the same doc, then there's no problem since we consume the delete buffer first. If the
        // delete is causally after, we will not be able to see the document when we attempt to
        // fetch it, so it's also ok.
        deleteList.splice(deleteList.cbegin(), _deleted);
        updateList.splice(updateList.cbegin(), _reload);
    }

    auto totalDocSize = _xferDeletes(builder, &deleteList, 0);
    totalDocSize = _xferUpdates(opCtx, db, builder, &updateList, totalDocSize);

    builder->append("size", totalDocSize);

    // Put back remaining ids we didn't consume
    stdx::unique_lock<Latch> lk(_mutex);
    _deleted.splice(_deleted.cbegin(), deleteList);
    _reload.splice(_reload.cbegin(), updateList);

    return Status::OK();
}

void MigrationChunkClonerSourceLegacy::_cleanup(OperationContext* opCtx) {
    stdx::unique_lock<Latch> lk(_mutex);
    _state = kDone;

    _drainAllOutstandingOperationTrackRequests(lk);

    _reload.clear();
    _deleted.clear();
}

StatusWith<BSONObj> MigrationChunkClonerSourceLegacy::_callRecipient(const BSONObj& cmdObj) {
    executor::RemoteCommandResponse responseStatus(
        Status{ErrorCodes::InternalError, "Uninitialized value"});

    auto executor = Grid::get(getGlobalServiceContext())->getExecutorPool()->getFixedExecutor();
    auto scheduleStatus = executor->scheduleRemoteCommand(
        executor::RemoteCommandRequest(_recipientHost, "admin", cmdObj, nullptr),
        [&responseStatus](const executor::TaskExecutor::RemoteCommandCallbackArgs& args) {
            responseStatus = args.response;
        });

    // TODO: Update RemoteCommandTargeter on NotMaster errors.
    if (!scheduleStatus.isOK()) {
        return scheduleStatus.getStatus();
    }

    executor->wait(scheduleStatus.getValue());

    if (!responseStatus.isOK()) {
        return responseStatus.status;
    }

    Status commandStatus = getStatusFromCommandResult(responseStatus.data);
    if (!commandStatus.isOK()) {
        return commandStatus;
    }

    return responseStatus.data.getOwned();
}

StatusWith<std::unique_ptr<PlanExecutor, PlanExecutor::Deleter>>
MigrationChunkClonerSourceLegacy::_getIndexScanExecutor(OperationContext* opCtx,
                                                        Collection* const collection) {
    // Allow multiKey based on the invariant that shard keys must be single-valued. Therefore, any
    // multi-key index prefixed by shard key cannot be multikey over the shard key fields.
    const IndexDescriptor* idx =
        collection->getIndexCatalog()->findShardKeyPrefixedIndex(opCtx,
                                                                 _shardKeyPattern.toBSON(),
                                                                 false);  // requireSingleKey
    if (!idx) {
        return {ErrorCodes::IndexNotFound,
                str::stream() << "can't find index with prefix " << _shardKeyPattern.toBSON()
                              << " in storeCurrentLocs for " << _args.getNss().ns()};
    }

    // Assume both min and max non-empty, append MinKey's to make them fit chosen index
    const KeyPattern kp(idx->keyPattern());

    BSONObj min = Helpers::toKeyFormat(kp.extendRangeBound(_args.getMinKey(), false));
    BSONObj max = Helpers::toKeyFormat(kp.extendRangeBound(_args.getMaxKey(), false));

    // We can afford to yield here because any change to the base data that we might miss is already
    // being queued and will migrate in the 'transferMods' stage.
    return InternalPlanner::indexScan(opCtx,
                                      collection,
                                      idx,
                                      min,
                                      max,
                                      BoundInclusion::kIncludeStartKeyOnly,
                                      PlanExecutor::YIELD_AUTO);
}

Status MigrationChunkClonerSourceLegacy::_storeCurrentLocs(OperationContext* opCtx) {
    AutoGetCollection autoColl(opCtx, _args.getNss(), MODE_IS);

    Collection* const collection = autoColl.getCollection();
    if (!collection) {
        return {ErrorCodes::NamespaceNotFound,
                str::stream() << "Collection " << _args.getNss().ns() << " does not exist."};
    }

    auto swExec = _getIndexScanExecutor(opCtx, collection);
    if (!swExec.isOK()) {
        return swExec.getStatus();
    }
    auto exec = std::move(swExec.getValue());

    // Use the average object size to estimate how many objects a full chunk would carry do that
    // while traversing the chunk's range using the sharding index, below there's a fair amount of
    // slack before we determine a chunk is too large because object sizes will vary.
    unsigned long long maxRecsWhenFull;
    long long avgRecSize;

    const long long totalRecs = collection->numRecords(opCtx);
    if (totalRecs > 0) {
        avgRecSize = collection->dataSize(opCtx) / totalRecs;
        // The calls to numRecords() and dataSize() are not atomic so it is possible that the data
        // size becomes smaller than the number of records between the two calls, which would result
        // in average record size of zero
        if (avgRecSize == 0) {
            avgRecSize = BSONObj::kMinBSONLength;
        }
        maxRecsWhenFull = _args.getMaxChunkSizeBytes() / avgRecSize;
        maxRecsWhenFull = 130 * maxRecsWhenFull / 100;  // pad some slack
    } else {
        avgRecSize = 0;
        maxRecsWhenFull = kMaxObjectPerChunk + 1;
    }

    // Do a full traversal of the chunk and don't stop even if we think it is a large chunk we want
    // the number of records to better report, in that case.
    bool isLargeChunk = false;
    unsigned long long recCount = 0;

    BSONObj obj;
    RecordId recordId;
    PlanExecutor::ExecState state;
    while (PlanExecutor::ADVANCED == (state = exec->getNext(&obj, &recordId))) {
        Status interruptStatus = opCtx->checkForInterruptNoAssert();
        if (!interruptStatus.isOK()) {
            return interruptStatus;
        }

        if (!isLargeChunk) {
            stdx::lock_guard<Latch> lk(_mutex);
            _cloneLocs.insert(recordId);
        }

        if (++recCount > maxRecsWhenFull) {
            isLargeChunk = true;

            if (_forceJumbo) {
                _cloneLocs.clear();
                break;
            }
        }
    }

    if (PlanExecutor::FAILURE == state) {
        return WorkingSetCommon::getMemberObjectStatus(obj).withContext(
            "Executor error while scanning for documents belonging to chunk");
    }

    const uint64_t collectionAverageObjectSize = collection->averageObjectSize(opCtx);

    if (isLargeChunk) {
        return {
            ErrorCodes::ChunkTooBig,
            str::stream() << "Cannot move chunk: the maximum number of documents for a chunk is "
                          << maxRecsWhenFull << ", the maximum chunk size is "
                          << _args.getMaxChunkSizeBytes() << ", average document size is "
                          << avgRecSize << ". Found " << recCount << " documents in chunk "
                          << " ns: " << _args.getNss().ns() << " " << _args.getMinKey() << " -> "
                          << _args.getMaxKey()};
    }

    stdx::lock_guard<Latch> lk(_mutex);
    _averageObjectSizeForCloneLocs = collectionAverageObjectSize + 12;

    return Status::OK();
}

long long MigrationChunkClonerSourceLegacy::_xferDeletes(BSONObjBuilder* builder,
                                                         std::list<BSONObj>* removeList,
                                                         long long initialSize) {
    const long long maxSize = 1024 * 1024;

    if (removeList->empty() || initialSize > maxSize) {
        return initialSize;
    }

    long long totalSize = initialSize;
    BSONArrayBuilder arr(builder->subarrayStart("deleted"));

    auto docIdIter = removeList->begin();
    for (; docIdIter != removeList->end() && totalSize < maxSize; ++docIdIter) {
        BSONObj idDoc = *docIdIter;
        arr.append(idDoc);
        totalSize += idDoc.objsize();
    }

    removeList->erase(removeList->begin(), docIdIter);

    arr.done();
    return totalSize;
}

long long MigrationChunkClonerSourceLegacy::_xferUpdates(OperationContext* opCtx,
                                                         Database* db,
                                                         BSONObjBuilder* builder,
                                                         std::list<BSONObj>* updateList,
                                                         long long initialSize) {
    const long long maxSize = 1024 * 1024;

    if (updateList->empty() || initialSize > maxSize) {
        return initialSize;
    }

    const auto& nss = _args.getNss();
    BSONArrayBuilder arr(builder->subarrayStart("reload"));
    long long totalSize = initialSize;

    auto iter = updateList->begin();
    for (; iter != updateList->end() && totalSize < maxSize; ++iter) {
        auto idDoc = *iter;

        BSONObj fullDoc;
        if (Helpers::findById(opCtx, db, nss.ns().c_str(), idDoc, fullDoc)) {
            arr.append(fullDoc);
            totalSize += fullDoc.objsize();
        }
    }

    updateList->erase(updateList->begin(), iter);

    arr.done();
    return totalSize;
}

Status MigrationChunkClonerSourceLegacy::_checkRecipientCloningStatus(OperationContext* opCtx,
                                                                      Milliseconds maxTimeToWait) {
    const auto startTime = Date_t::now();
    int iteration = 0;
    while ((Date_t::now() - startTime) < maxTimeToWait) {
        auto responseStatus = _callRecipient(
            createRequestWithSessionId(kRecvChunkStatus, _args.getNss(), _sessionId, true));
        if (!responseStatus.isOK()) {
            return responseStatus.getStatus().withContext(
                "Failed to contact recipient shard to monitor data transfer");
        }

        const BSONObj& res = responseStatus.getValue();
        if (!res["waited"].boolean()) {
            sleepmillis(1LL << std::min(iteration, 10));
        }
        iteration++;

        stdx::lock_guard<Latch> sl(_mutex);

        const std::size_t cloneLocsRemaining = _cloneLocs.size();

        if (_forceJumbo && _jumboChunkCloneState) {
            LOGV2(21992,
                  "moveChunk data transfer progress: {response} mem used: {memoryUsedBytes} "
                  "documents cloned so far: {docsCloned}",
                  "moveChunk data transfer progress",
                  "response"_attr = redact(res),
                  "memoryUsedBytes"_attr = _memoryUsed,
                  "docsCloned"_attr = _jumboChunkCloneState->docsCloned);
        } else {
            LOGV2(21993,
                  "moveChunk data transfer progress: {response} mem used: {memoryUsedBytes} "
                  "documents remaining to clone: {docsRemainingToClone}",
                  "moveChunk data transfer progress",
                  "response"_attr = redact(res),
                  "memoryUsedBytes"_attr = _memoryUsed,
                  "docsRemainingToClone"_attr = cloneLocsRemaining);
        }

        if (res["state"].String() == "steady") {
            if (cloneLocsRemaining != 0 ||
                (_jumboChunkCloneState && _forceJumbo &&
                 PlanExecutor::IS_EOF != _jumboChunkCloneState->clonerState)) {
                return {ErrorCodes::OperationIncomplete,
                        str::stream() << "Unable to enter critical section because the recipient "
                                         "shard thinks all data is cloned while there are still "
                                         "documents remaining"};
            }

            return Status::OK();
        }

        if (res["state"].String() == "fail") {
            return {ErrorCodes::OperationFailed,
                    str::stream() << "Data transfer error: " << res["errmsg"].str()};
        }

        auto migrationSessionIdStatus = MigrationSessionId::extractFromBSON(res);
        if (!migrationSessionIdStatus.isOK()) {
            return {ErrorCodes::OperationIncomplete,
                    str::stream() << "Unable to retrieve the id of the migration session due to "
                                  << migrationSessionIdStatus.getStatus().toString()};
        }

        if (res["ns"].str() != _args.getNss().ns() ||
            (res.hasField("fromShardId")
                 ? (res["fromShardId"].str() != _args.getFromShardId().toString())
                 : (res["from"].str() != _donorConnStr.toString())) ||
            !res["min"].isABSONObj() || res["min"].Obj().woCompare(_args.getMinKey()) != 0 ||
            !res["max"].isABSONObj() || res["max"].Obj().woCompare(_args.getMaxKey()) != 0 ||
            !_sessionId.matches(migrationSessionIdStatus.getValue())) {
            // This can happen when the destination aborted the migration and received another
            // recvChunk before this thread sees the transition to the abort state. This is
            // currently possible only if multiple migrations are happening at once. This is an
            // unfortunate consequence of the shards not being able to keep track of multiple
            // incoming and outgoing migrations.
            return {ErrorCodes::OperationIncomplete,
                    "Destination shard aborted migration because a new one is running"};
        }

        if (_args.getForceJumbo() != MoveChunkRequest::ForceJumbo::kForceManual &&
            (_memoryUsed > 500 * 1024 * 1024 ||
             (_jumboChunkCloneState && MONGO_unlikely(failTooMuchMemoryUsed.shouldFail())))) {
            // This is too much memory for us to use so we're going to abort the migration
            return {ErrorCodes::ExceededMemoryLimit,
                    "Aborting migration because of high memory usage"};
        }

        Status interruptStatus = opCtx->checkForInterruptNoAssert();
        if (!interruptStatus.isOK()) {
            return interruptStatus;
        }
    }

    return {ErrorCodes::ExceededTimeLimit, "Timed out waiting for the cloner to catch up"};
}

boost::optional<repl::OpTime> MigrationChunkClonerSourceLegacy::nextSessionMigrationBatch(
    OperationContext* opCtx, BSONArrayBuilder* arrBuilder) {
    if (!_sessionCatalogSource) {
        return boost::none;
    }

    repl::OpTime opTimeToWaitIfWaitingForMajority;
    const ChunkRange range(_args.getMinKey(), _args.getMaxKey());

    while (_sessionCatalogSource->hasMoreOplog()) {
        auto result = _sessionCatalogSource->getLastFetchedOplog();

        if (!result.oplog) {
            _sessionCatalogSource->fetchNextOplog(opCtx);
            continue;
        }

        auto newOpTime = result.oplog->getOpTime();
        auto oplogDoc = result.oplog->toBSON();

        // Use the builder size instead of accumulating the document sizes directly so that we
        // take into consideration the overhead of BSONArray indices.
        if (arrBuilder->arrSize() &&
            (arrBuilder->len() + oplogDoc.objsize() + 1024) > BSONObjMaxUserSize) {
            break;
        }

        arrBuilder->append(oplogDoc);

        _sessionCatalogSource->fetchNextOplog(opCtx);

        if (result.shouldWaitForMajority) {
            if (opTimeToWaitIfWaitingForMajority < newOpTime) {
                opTimeToWaitIfWaitingForMajority = newOpTime;
            }
        }
    }

    return boost::make_optional(opTimeToWaitIfWaitingForMajority);
}

std::shared_ptr<Notification<bool>>
MigrationChunkClonerSourceLegacy::getNotificationForNextSessionMigrationBatch() {
    if (!_sessionCatalogSource) {
        return nullptr;
    }

    return _sessionCatalogSource->getNotificationForNewOplog();
}

}  // namespace mongo
