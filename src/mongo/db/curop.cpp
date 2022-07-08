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

// CHECK_LOG_REDACTION

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kDefault

#include "mongo/platform/basic.h"

#include "mongo/db/curop.h"

#include <iomanip>

#include "mongo/bson/mutable/document.h"
#include "mongo/config.h"
#include "mongo/db/auth/authorization_session.h"
#include "mongo/db/client.h"
#include "mongo/db/commands.h"
#include "mongo/db/commands/server_status_metric.h"
#include "mongo/db/concurrency/d_concurrency.h"
#include "mongo/db/concurrency/locker.h"
#include "mongo/db/json.h"
#include "mongo/db/prepare_conflict_tracker.h"
#include "mongo/db/query/getmore_request.h"
#include "mongo/db/query/plan_summary_stats.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/metadata/client_metadata.h"
#include "mongo/rpc/metadata/client_metadata_ismaster.h"
#include "mongo/rpc/metadata/impersonated_user_metadata.h"
#include "mongo/util/hex.h"
#include "mongo/util/log_with_sampling.h"
#include "mongo/util/net/socket_utils.h"
#include "mongo/util/str.h"
#include <mongo/db/stats/timer_stats.h>

namespace mongo {

using std::string;

namespace {

// Lists the $-prefixed query options that can be passed alongside a wrapped query predicate for
// OP_QUERY find. The $orderby field is omitted because "orderby" (no dollar sign) is also allowed,
// and this requires special handling.
const std::vector<const char*> kDollarQueryModifiers = {
    "$hint",
    "$comment",
    "$max",
    "$min",
    "$returnKey",
    "$showDiskLoc",
    "$snapshot",
    "$maxTimeMS",
};

TimerStats oplogGetMoreStats;
ServerStatusMetricField<TimerStats> displayBatchesReceived("repl.network.oplogGetMoresProcessed",
                                                           &oplogGetMoreStats);

}  // namespace

BSONObj upconvertQueryEntry(const BSONObj& query,
                            const NamespaceString& nss,
                            int ntoreturn,
                            int ntoskip) {
    BSONObjBuilder bob;

    bob.append("find", nss.coll());

    // Whether or not the query predicate is wrapped inside a "query" or "$query" field so that
    // other options can be passed alongside the predicate.
    bool predicateIsWrapped = false;

    // Extract the query predicate.
    BSONObj filter;
    if (query["query"].isABSONObj()) {
        predicateIsWrapped = true;
        bob.appendAs(query["query"], "filter");
    } else if (query["$query"].isABSONObj()) {
        predicateIsWrapped = true;
        bob.appendAs(query["$query"], "filter");
    } else if (!query.isEmpty()) {
        bob.append("filter", query);
    }

    if (ntoskip) {
        bob.append("skip", ntoskip);
    }
    if (ntoreturn) {
        bob.append("ntoreturn", ntoreturn);
    }

    // The remainder of the query options are only available if the predicate is passed in wrapped
    // form. If the predicate is not wrapped, we're done.
    if (!predicateIsWrapped) {
        return bob.obj();
    }

    // Extract the sort.
    if (auto elem = query["orderby"]) {
        bob.appendAs(elem, "sort");
    } else if (auto elem = query["$orderby"]) {
        bob.appendAs(elem, "sort");
    }

    // Add $-prefixed OP_QUERY modifiers, like $hint.
    for (auto modifier : kDollarQueryModifiers) {
        if (auto elem = query[modifier]) {
            // Use "+ 1" to omit the leading dollar sign.
            bob.appendAs(elem, modifier + 1);
        }
    }

    return bob.obj();
}

BSONObj upconvertGetMoreEntry(const NamespaceString& nss, CursorId cursorId, int ntoreturn) {
    return GetMoreRequest(nss,
                          cursorId,
                          ntoreturn,
                          boost::none,  // awaitDataTimeout
                          boost::none,  // term
                          boost::none   // lastKnownCommittedOpTime
                          )
        .toBSON();
}

/**
 * This type decorates a Client object with a stack of active CurOp objects.
 *
 * It encapsulates the nesting logic for curops attached to a Client, along with
 * the notion that there is always a root CurOp attached to a Client.
 *
 * The stack itself is represented in the _parent pointers of the CurOp class.
 */
class CurOp::CurOpStack {
    CurOpStack(const CurOpStack&) = delete;
    CurOpStack& operator=(const CurOpStack&) = delete;

public:
    CurOpStack() : _base(nullptr, this) {}

    /**
     * Returns the top of the CurOp stack.
     */
    CurOp* top() const {
        return _top;
    }

    /**
     * Adds "curOp" to the top of the CurOp stack for a client. Called by CurOp's constructor.
     */
    void push(OperationContext* opCtx, CurOp* curOp) {
        invariant(opCtx);
        if (_opCtx) {
            invariant(_opCtx == opCtx);
        } else {
            _opCtx = opCtx;
        }
        stdx::lock_guard<Client> lk(*_opCtx->getClient());
        push_nolock(curOp);
    }

    void push_nolock(CurOp* curOp) {
        invariant(!curOp->_parent);
        curOp->_parent = _top;
        _top = curOp;
    }

    /**
     * Pops the top off the CurOp stack for a Client. Called by CurOp's destructor.
     */
    CurOp* pop() {
        // It is not necessary to lock when popping the final item off of the curop stack. This
        // is because the item at the base of the stack is owned by the stack itself, and is not
        // popped until the stack is being destroyed.  By the time the stack is being destroyed,
        // no other threads can be observing the Client that owns the stack, because it has been
        // removed from its ServiceContext's set of owned clients.  Further, because the last
        // item is popped in the destructor of the stack, and that destructor runs during
        // destruction of the owning client, it is not safe to access other member variables of
        // the client during the final pop.
        const bool shouldLock = _top->_parent;
        if (shouldLock) {
            invariant(_opCtx);
            _opCtx->getClient()->lock();
        }
        invariant(_top);
        CurOp* retval = _top;
        _top = _top->_parent;
        if (shouldLock) {
            _opCtx->getClient()->unlock();
        }
        return retval;
    }

private:
    OperationContext* _opCtx = nullptr;

    // Top of the stack of CurOps for a Client.
    CurOp* _top = nullptr;

    // The bottom-most CurOp for a client.
    const CurOp _base;
};

const OperationContext::Decoration<CurOp::CurOpStack> CurOp::_curopStack =
    OperationContext::declareDecoration<CurOp::CurOpStack>();

CurOp* CurOp::get(const OperationContext* opCtx) {
    return get(*opCtx);
}

CurOp* CurOp::get(const OperationContext& opCtx) {
    return _curopStack(opCtx).top();
}

void CurOp::reportCurrentOpForClient(OperationContext* opCtx,
                                     Client* client,
                                     bool truncateOps,
                                     bool backtraceMode,
                                     BSONObjBuilder* infoBuilder) {
    invariant(client);

    OperationContext* clientOpCtx = client->getOperationContext();

    infoBuilder->append("type", "op");

    const std::string hostName = getHostNameCachedAndPort();
    infoBuilder->append("host", hostName);

    client->reportState(*infoBuilder);
    const auto& clientMetadata = ClientMetadataIsMasterState::get(client).getClientMetadata();

    if (clientMetadata) {
        auto appName = clientMetadata.get().getApplicationName();
        if (!appName.empty()) {
            infoBuilder->append("appName", appName);
        }

        auto clientMetadataDocument = clientMetadata.get().getDocument();
        infoBuilder->append("clientMetadata", clientMetadataDocument);
    }

    // Fill out the rest of the BSONObj with opCtx specific details.
    infoBuilder->appendBool("active", client->hasAnyActiveCurrentOp());
    infoBuilder->append("currentOpTime",
                        opCtx->getServiceContext()->getPreciseClockSource()->now().toString());

    auto authSession = AuthorizationSession::get(client);
    // Depending on whether the authenticated user is the same user which ran the command,
    // this might be "effectiveUsers" or "runBy".
    const auto serializeAuthenticatedUsers = [&](StringData name) {
        if (authSession->isAuthenticated()) {
            BSONArrayBuilder users(infoBuilder->subarrayStart(name));
            for (auto userIt = authSession->getAuthenticatedUserNames(); userIt.more();
                 userIt.next()) {
                userIt->serializeToBSON(&users);
            }
        }
    };

    auto maybeImpersonationData = rpc::getImpersonatedUserMetadata(clientOpCtx);
    if (maybeImpersonationData) {
        BSONArrayBuilder users(infoBuilder->subarrayStart("effectiveUsers"));
        for (const auto& user : maybeImpersonationData->getUsers()) {
            user.serializeToBSON(&users);
        }

        users.doneFast();
        serializeAuthenticatedUsers("runBy"_sd);
    } else {
        serializeAuthenticatedUsers("effectiveUsers"_sd);
    }

    if (clientOpCtx) {
        infoBuilder->append("opid", static_cast<int>(clientOpCtx->getOpID()));

        if (auto opKey = clientOpCtx->getOperationKey()) {
            opKey->appendToBuilder(infoBuilder, "operationKey");
        }

        if (clientOpCtx->isKillPending()) {
            infoBuilder->append("killPending", true);
        }

        if (auto lsid = clientOpCtx->getLogicalSessionId()) {
            BSONObjBuilder lsidBuilder(infoBuilder->subobjStart("lsid"));
            lsid->serialize(&lsidBuilder);
        }

        CurOp::get(clientOpCtx)->reportState(clientOpCtx, infoBuilder, truncateOps);
    }

#ifndef MONGO_CONFIG_USE_RAW_LATCHES
    if (auto diagnostic = DiagnosticInfo::get(*client)) {
        BSONObjBuilder waitingForLatchBuilder(infoBuilder->subobjStart("waitingForLatch"));
        waitingForLatchBuilder.append("timestamp", diagnostic->getTimestamp());
        waitingForLatchBuilder.append("captureName", diagnostic->getCaptureName());
        if (backtraceMode) {
            BSONArrayBuilder backtraceBuilder(waitingForLatchBuilder.subarrayStart("backtrace"));
            /** This branch becomes useful again with SERVER-44091
            for (const auto& frame : diagnostic->makeStackTrace().frames) {
                BSONObjBuilder backtraceObj(backtraceBuilder.subobjStart());
                backtraceObj.append("addr", integerToHex(frame.instructionOffset));
                backtraceObj.append("path", frame.objectPath);
            }
            */
        }
    }
#endif
}

void CurOp::setGenericCursor_inlock(GenericCursor gc) {
    _genericCursor = std::move(gc);
}

CurOp::CurOp(OperationContext* opCtx) : CurOp(opCtx, &_curopStack(opCtx)) {
    // If this is a sub-operation, we store the snapshot of lock stats as the base lock stats of the
    // current operation.
    if (_parent != nullptr)
        _lockStatsBase = opCtx->lockState()->getLockerInfo(boost::none)->stats;
}

CurOp::CurOp(OperationContext* opCtx, CurOpStack* stack) : _stack(stack) {
    if (opCtx) {
        _stack->push(opCtx, this);
    } else {
        _stack->push_nolock(this);
    }
}

CurOp::~CurOp() {
    if (parent() != nullptr)
        parent()->yielded(_numYields);
    invariant(this == _stack->pop());
}

void CurOp::setGenericOpRequestDetails(OperationContext* opCtx,
                                       const NamespaceString& nss,
                                       const Command* command,
                                       BSONObj cmdObj,
                                       NetworkOp op) {
    // Set the _isCommand flags based on network op only. For legacy writes on mongoS, we resolve
    // them to OpMsgRequests and then pass them into the Commands path, so having a valid Command*
    // here does not guarantee that the op was issued from the client using a command protocol.
    const bool isCommand = (op == dbMsg || (op == dbQuery && nss.isCommand()));
    auto logicalOp = (command ? command->getLogicalOp() : networkOpToLogicalOp(op));

    stdx::lock_guard<Client> clientLock(*opCtx->getClient());
    _isCommand = _debug.iscommand = isCommand;
    _logicalOp = _debug.logicalOp = logicalOp;
    _networkOp = _debug.networkOp = op;
    _opDescription = cmdObj;
    _command = command;
    _ns = nss.ns();
}

void CurOp::setMessage_inlock(StringData message) {
    if (_progressMeter.isActive()) {
        LOGV2_ERROR(20527,
                    "Changing message from {old} to {new}",
                    "Updating message",
                    "old"_attr = redact(_message),
                    "new"_attr = redact(message));
        verify(!_progressMeter.isActive());
    }
    _message = message.toString();  // copy
}

ProgressMeter& CurOp::setProgress_inlock(StringData message,
                                         unsigned long long progressMeterTotal,
                                         int secondsBetween) {
    setMessage_inlock(message);
    _progressMeter.reset(progressMeterTotal, secondsBetween);
    _progressMeter.setName(message);
    return _progressMeter;
}

void CurOp::setNS_inlock(StringData ns) {
    _ns = ns.toString();
}

void CurOp::ensureStarted() {
    if (_start == 0) {
        _start = curTimeMicros64();
    }
}

void CurOp::enter_inlock(const char* ns, boost::optional<int> dbProfileLevel) {
    ensureStarted();
    _ns = ns;
    if (dbProfileLevel) {
        raiseDbProfileLevel(*dbProfileLevel);
    }
}

void CurOp::raiseDbProfileLevel(int dbProfileLevel) {
    _dbprofile = std::max(dbProfileLevel, _dbprofile);
}

bool CurOp::completeAndLogOperation(OperationContext* opCtx,
                                    logv2::LogComponent component,
                                    boost::optional<size_t> responseLength,
                                    boost::optional<long long> slowMsOverride,
                                    bool forceLog) {
    const long long slowMs = slowMsOverride.value_or(serverGlobalParams.slowMS);

    // Record the size of the response returned to the client, if applicable.
    if (responseLength) {
        _debug.responseLength = *responseLength;
    }

    // Obtain the total execution time of this operation.
    _end = curTimeMicros64();
    _debug.executionTimeMicros = durationCount<Microseconds>(elapsedTimeExcludingPauses());

    const auto executionTimeMillis = _debug.executionTimeMicros / 1000;

    if (_debug.isReplOplogFetching) {
        oplogGetMoreStats.recordMillis(executionTimeMillis);
    }

    bool shouldLogSlowOp, shouldSample;

    // Log the operation if it is eligible according to the current slowMS and sampleRate settings.
    std::tie(shouldLogSlowOp, shouldSample) = shouldLogSlowOpWithSampling(
        opCtx, component, Milliseconds(executionTimeMillis), Milliseconds(slowMs));

    if (forceLog || shouldLogSlowOp) {
        auto lockerInfo = opCtx->lockState()->getLockerInfo(_lockStatsBase);
        if (_debug.storageStats == nullptr && opCtx->lockState()->wasGlobalLockTaken() &&
            opCtx->getServiceContext()->getStorageEngine()) {
            // Do not fetch operation statistics again if we have already got them (for instance,
            // as a part of stashing the transaction).
            // Take a lock before calling into the storage engine to prevent racing against a
            // shutdown. Any operation that used a storage engine would have at-least held a
            // global lock at one point, hence we limit our lock acquisition to such operations.
            // We can get here and our lock acquisition be timed out or interrupted, log a
            // message if that happens.
            try {
                // Retrieving storage stats should not be blocked by oplog application.
                ShouldNotConflictWithSecondaryBatchApplicationBlock shouldNotConflictBlock(
                    opCtx->lockState());
                Lock::GlobalLock lk(opCtx,
                                    MODE_IS,
                                    Date_t::now() + Milliseconds(500),
                                    Lock::InterruptBehavior::kLeaveUnlocked);
                if (lk.isLocked()) {
                    _debug.storageStats = opCtx->recoveryUnit()->getOperationStatistics();
                } else {
                    LOGV2_WARNING_OPTIONS(
                        20525,
                        {component},
                        "Failed to gather storage statistics for {opId} due to {reason}",
                        "Failed to gather storage statistics for slow operation",
                        "opId"_attr = opCtx->getOpID(),
                        "error"_attr = "lock acquire timeout"_sd);
                }
            } catch (const ExceptionForCat<ErrorCategory::Interruption>& ex) {
                LOGV2_WARNING_OPTIONS(
                    20526,
                    {component},
                    "Failed to gather storage statistics for {opId} due to {reason}",
                    "Failed to gather storage statistics for slow operation",
                    "opId"_attr = opCtx->getOpID(),
                    "error"_attr = redact(ex));
            }
        }

        // Gets the time spent blocked on prepare conflicts.
        auto prepareConflictDurationMicros =
            PrepareConflictTracker::get(opCtx).getPrepareConflictDuration();
        _debug.prepareConflictDurationMillis =
            duration_cast<Milliseconds>(prepareConflictDurationMicros);

        logv2::DynamicAttributes attr;
        _debug.report(opCtx, (lockerInfo ? &lockerInfo->stats : nullptr), &attr);
        LOGV2_OPTIONS(51803, {component}, "Slow query", attr);
    }

    // Return 'true' if this operation should also be added to the profiler.
    return shouldDBProfile(shouldSample);
}

Command::ReadWriteType CurOp::getReadWriteType() const {
    if (_command) {
        return _command->getReadWriteType();
    }
    switch (_logicalOp) {
        case LogicalOp::opGetMore:
        case LogicalOp::opQuery:
            return Command::ReadWriteType::kRead;
        case LogicalOp::opUpdate:
        case LogicalOp::opInsert:
        case LogicalOp::opDelete:
            return Command::ReadWriteType::kWrite;
        default:
            return Command::ReadWriteType::kCommand;
    }
}

namespace {

BSONObj appendCommentField(OperationContext* opCtx, const BSONObj& cmdObj) {
    return opCtx->getComment() && !cmdObj["comment"] ? cmdObj.addField(*opCtx->getComment())
                                                     : cmdObj;
}

/**
 * Appends {<name>: obj} to the provided builder.  If obj is greater than maxSize, appends a string
 * summary of obj as { <name>: { $truncated: "obj" } }. If a comment parameter is present, add it to
 * the truncation object.
 */
void appendAsObjOrString(StringData name,
                         const BSONObj& obj,
                         const boost::optional<size_t> maxSize,
                         BSONObjBuilder* builder) {
    if (!maxSize || static_cast<size_t>(obj.objsize()) <= *maxSize) {
        builder->append(name, obj);
    } else {
        // Generate an abbreviated serialization for the object, by passing false as the
        // "full" argument to obj.toString().
        std::string objToString = obj.toString();
        if (objToString.size() > *maxSize) {
            // objToString is still too long, so we append to the builder a truncated form
            // of objToString concatenated with "...".  Instead of creating a new string
            // temporary, mutate objToString to do this (we know that we can mutate
            // characters in objToString up to and including objToString[maxSize]).
            objToString[*maxSize - 3] = '.';
            objToString[*maxSize - 2] = '.';
            objToString[*maxSize - 1] = '.';
        }

        StringData truncation = StringData(objToString).substr(0, *maxSize);

        // Append the truncated representation of the object to the builder. If a comment parameter
        // is present, write it to the object alongside the truncated op. This object will appear as
        // {$truncated: "{find: \"collection\", filter: {x: 1, ...", comment: "comment text" }
        BSONObjBuilder truncatedBuilder(builder->subobjStart(name));
        truncatedBuilder.append("$truncated", truncation);

        if (auto comment = obj["comment"]) {
            truncatedBuilder.append(comment);
        }

        truncatedBuilder.doneFast();
    }
}
}  // namespace

BSONObj CurOp::truncateAndSerializeGenericCursor(GenericCursor* cursor,
                                                 boost::optional<size_t> maxQuerySize) {
    // This creates a new builder to truncate the object that will go into the curOp output. In
    // order to make sure the object is not too large but not truncate the comment, we only
    // truncate the originatingCommand and not the entire cursor.
    if (maxQuerySize) {
        BSONObjBuilder tempObj;
        appendAsObjOrString(
            "truncatedObj", cursor->getOriginatingCommand().get(), maxQuerySize, &tempObj);
        auto originatingCommand = tempObj.done().getObjectField("truncatedObj");
        cursor->setOriginatingCommand(originatingCommand.getOwned());
    }
    // lsid, ns, and planSummary exist in the top level curop object, so they need to be temporarily
    // removed from the cursor object to avoid duplicating information.
    auto lsid = cursor->getLsid();
    auto ns = cursor->getNs();
    auto originalPlanSummary(cursor->getPlanSummary() ? boost::optional<std::string>(
                                                            cursor->getPlanSummary()->toString())
                                                      : boost::none);
    cursor->setLsid(boost::none);
    cursor->setNs(boost::none);
    cursor->setPlanSummary(boost::none);
    auto serialized = cursor->toBSON();
    cursor->setLsid(lsid);
    cursor->setNs(ns);
    if (originalPlanSummary) {
        cursor->setPlanSummary(StringData(*originalPlanSummary));
    }
    return serialized;
}

void CurOp::reportState(OperationContext* opCtx, BSONObjBuilder* builder, bool truncateOps) {
    if (_start) {
        builder->append("secs_running", durationCount<Seconds>(elapsedTimeTotal()));
        builder->append("microsecs_running", durationCount<Microseconds>(elapsedTimeTotal()));
    }

    builder->append("op", logicalOpToString(_logicalOp));
    builder->append("ns", _ns);

    // When the currentOp command is run, it returns a single response object containing all current
    // operations; this request will fail if the response exceeds the 16MB document limit. By
    // contrast, the $currentOp aggregation stage does not have this restriction. If 'truncateOps'
    // is true, limit the size of each op to 1000 bytes. Otherwise, do not truncate.
    const boost::optional<size_t> maxQuerySize{truncateOps, 1000};

    appendAsObjOrString(
        "command", appendCommentField(opCtx, _opDescription), maxQuerySize, builder);


    if (!_planSummary.empty()) {
        builder->append("planSummary", _planSummary);
    }

    if (_genericCursor) {
        builder->append("cursor",
                        truncateAndSerializeGenericCursor(&(*_genericCursor), maxQuerySize));
    }

    if (!_message.empty()) {
        if (_progressMeter.isActive()) {
            StringBuilder buf;
            buf << _message << " " << _progressMeter.toString();
            builder->append("msg", buf.str());
            BSONObjBuilder sub(builder->subobjStart("progress"));
            sub.appendNumber("done", (long long)_progressMeter.done());
            sub.appendNumber("total", (long long)_progressMeter.total());
            sub.done();
        } else {
            builder->append("msg", _message);
        }
    }

    if (auto n = _debug.additiveMetrics.prepareReadConflicts.load(); n > 0) {
        builder->append("prepareReadConflicts", n);
    }
    if (auto n = _debug.additiveMetrics.writeConflicts.load(); n > 0) {
        builder->append("writeConflicts", n);
    }

    builder->append("numYields", _numYields);

    if (_debug.dataThroughputLastSecond) {
        builder->append("dataThroughputLastSecond", *_debug.dataThroughputLastSecond);
    }

    if (_debug.dataThroughputAverage) {
        builder->append("dataThroughputAverage", *_debug.dataThroughputAverage);
    }
}

namespace {
StringData getProtoString(int op) {
    if (op == dbMsg) {
        return "op_msg";
    } else if (op == dbQuery) {
        return "op_query";
    }
    MONGO_UNREACHABLE;
}
}  // namespace

#define OPDEBUG_TOSTRING_HELP(x) \
    if (x >= 0)                  \
    s << " " #x ":" << (x)
#define OPDEBUG_TOSTRING_HELP_BOOL(x) \
    if (x)                            \
    s << " " #x ":" << (x)
#define OPDEBUG_TOSTRING_HELP_ATOMIC(x, y) \
    if (auto __y = y.load(); __y > 0)      \
    s << " " x ":" << (__y)
#define OPDEBUG_TOSTRING_HELP_OPTIONAL(x, y) \
    if (y)                                   \
    s << " " x ":" << (*y)

string OpDebug::report(OperationContext* opCtx, const SingleThreadedLockStats* lockStats) const {
    Client* client = opCtx->getClient();
    auto& curop = *CurOp::get(opCtx);
    auto flowControlStats = opCtx->lockState()->getFlowControlStats();
    StringBuilder s;
    if (iscommand)
        s << "command ";
    else
        s << networkOpToString(networkOp) << ' ';

    s << curop.getNS();

    const auto& clientMetadata = ClientMetadataIsMasterState::get(client).getClientMetadata();
    if (clientMetadata) {
        auto appName = clientMetadata.get().getApplicationName();
        if (!appName.empty()) {
            s << " appName: \"" << str::escape(appName) << '\"';
        }
    }

    auto query = appendCommentField(opCtx, curop.opDescription());
    if (!query.isEmpty()) {
        s << " command: ";
        if (iscommand) {
            const Command* curCommand = curop.getCommand();
            if (curCommand) {
                mutablebson::Document cmdToLog(query, mutablebson::Document::kInPlaceDisabled);
                curCommand->snipForLogging(&cmdToLog);
                s << curCommand->getName() << " ";
                s << redact(cmdToLog.getObject());
            } else {
                // Should not happen but we need to handle curCommand == NULL gracefully.
                // We don't know what the request payload is intended to be, so it might be
                // sensitive, and we don't know how to redact it properly without a 'Command*'.
                // So we just don't log it at all.
                s << "unrecognized";
            }
        } else {
            s << redact(query);
        }
    }

    auto originatingCommand = curop.originatingCommand();
    if (!originatingCommand.isEmpty()) {
        s << " originatingCommand: " << redact(originatingCommand);
    }

    if (!curop.getPlanSummary().empty()) {
        s << " planSummary: " << curop.getPlanSummary().toString();
    }

    if (prepareConflictDurationMillis > Milliseconds::zero()) {
        s << " prepareConflictDuration: " << prepareConflictDurationMillis;
    }

    if (dataThroughputLastSecond) {
        s << " dataThroughputLastSecond: " << *dataThroughputLastSecond << " MB/sec";
    }

    if (dataThroughputAverage) {
        s << " dataThroughputAverage: " << *dataThroughputAverage << " MB/sec";
    }

    OPDEBUG_TOSTRING_HELP(nShards);
    OPDEBUG_TOSTRING_HELP(cursorid);
    if (mongotCursorId) {
        s << " mongot: " << makeMongotDebugStatsObject().toString();
    }
    OPDEBUG_TOSTRING_HELP(ntoreturn);
    OPDEBUG_TOSTRING_HELP(ntoskip);
    OPDEBUG_TOSTRING_HELP_BOOL(exhaust);

    OPDEBUG_TOSTRING_HELP_OPTIONAL("keysExamined", additiveMetrics.keysExamined);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("docsExamined", additiveMetrics.docsExamined);
    OPDEBUG_TOSTRING_HELP_BOOL(hasSortStage);
    OPDEBUG_TOSTRING_HELP_BOOL(usedDisk);
    OPDEBUG_TOSTRING_HELP_BOOL(fromMultiPlanner);
    if (replanReason) {
        bool replanned = true;
        OPDEBUG_TOSTRING_HELP_BOOL(replanned);
        s << " replanReason:\"" << str::escape(redact(*replanReason)) << "\"";
    }
    OPDEBUG_TOSTRING_HELP_OPTIONAL("nMatched", additiveMetrics.nMatched);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("nModified", additiveMetrics.nModified);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("ninserted", additiveMetrics.ninserted);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("ndeleted", additiveMetrics.ndeleted);
    OPDEBUG_TOSTRING_HELP_BOOL(upsert);
    OPDEBUG_TOSTRING_HELP_BOOL(cursorExhausted);

    OPDEBUG_TOSTRING_HELP_OPTIONAL("keysInserted", additiveMetrics.keysInserted);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("keysDeleted", additiveMetrics.keysDeleted);
    OPDEBUG_TOSTRING_HELP_ATOMIC("prepareReadConflicts", additiveMetrics.prepareReadConflicts);
    OPDEBUG_TOSTRING_HELP_ATOMIC("writeConflicts", additiveMetrics.writeConflicts);

    s << " numYields:" << curop.numYields();
    OPDEBUG_TOSTRING_HELP(nreturned);

    if (queryHash) {
        s << " queryHash:" << unsignedIntToFixedLengthHex(*queryHash);
        invariant(planCacheKey);
        s << " planCacheKey:" << unsignedIntToFixedLengthHex(*planCacheKey);
    }

    if (!errInfo.isOK()) {
        s << " ok:" << 0;
        if (!errInfo.reason().empty()) {
            s << " errMsg:\"" << str::escape(redact(errInfo.reason())) << "\"";
        }
        s << " errName:" << errInfo.code();
        s << " errCode:" << static_cast<int>(errInfo.code());
    }

    if (responseLength > 0) {
        s << " reslen:" << responseLength;
    }

    if (lockStats) {
        BSONObjBuilder locks;
        lockStats->report(&locks);
        s << " locks:" << locks.obj().toString();
    }

    BSONObj flowControlObj = makeFlowControlObject(flowControlStats);
    if (flowControlObj.nFields() > 0) {
        s << " flowControl:" << flowControlObj.toString();
    }

    {
        const auto& readConcern = repl::ReadConcernArgs::get(opCtx);
        if (readConcern.isSpecified()) {
            s << " readConcern:" << readConcern.toBSONInner();
        }
    }

    if (writeConcern && !writeConcern->usedDefault) {
        s << " writeConcern:" << writeConcern->toBSON();
    }

    if (storageStats) {
        s << " storage:" << storageStats->toBSON().toString();
    }

    if (iscommand) {
        s << " protocol:" << getProtoString(networkOp);
    }

    if (remoteOpWaitTime) {
        s << " remoteOpWaitMillis:" << durationCount<Milliseconds>(*remoteOpWaitTime);
    }

    s << " " << (executionTimeMicros / 1000) << "ms";

    return s.str();
}

#define OPDEBUG_TOATTR_HELP(x) \
    if (x >= 0)                \
    pAttrs->add(#x, x)
#define OPDEBUG_TOATTR_HELP_BOOL(x) \
    if (x)                          \
    pAttrs->add(#x, x)
#define OPDEBUG_TOATTR_HELP_ATOMIC(x, y) \
    if (auto __y = y.load(); __y > 0)    \
    pAttrs->add(x, __y)
#define OPDEBUG_TOATTR_HELP_OPTIONAL(x, y) \
    if (y)                                 \
    pAttrs->add(x, *y)

void OpDebug::report(OperationContext* opCtx,
                     const SingleThreadedLockStats* lockStats,
                     logv2::DynamicAttributes* pAttrs) const {
    Client* client = opCtx->getClient();
    auto& curop = *CurOp::get(opCtx);
    auto flowControlStats = opCtx->lockState()->getFlowControlStats();

    if (iscommand) {
        pAttrs->add("type", "command");
    } else {
        pAttrs->add("type", networkOpToString(networkOp));
    }

    pAttrs->addDeepCopy("ns", curop.getNS());

    const auto& clientMetadata = ClientMetadataIsMasterState::get(client).getClientMetadata();
    if (clientMetadata) {
        StringData appName = clientMetadata.get().getApplicationName();
        if (!appName.empty()) {
            pAttrs->add("appName", appName);
        }
    }

    auto query = appendCommentField(opCtx, curop.opDescription());
    if (!query.isEmpty()) {
        if (iscommand) {
            const Command* curCommand = curop.getCommand();
            if (curCommand) {
                mutablebson::Document cmdToLog(query, mutablebson::Document::kInPlaceDisabled);
                curCommand->snipForLogging(&cmdToLog);
                pAttrs->add("command", redact(cmdToLog.getObject()));
            } else {
                // Should not happen but we need to handle curCommand == NULL gracefully.
                // We don't know what the request payload is intended to be, so it might be
                // sensitive, and we don't know how to redact it properly without a 'Command*'.
                // So we just don't log it at all.
                pAttrs->add("command", "unrecognized");
            }
        } else {
            pAttrs->add("command", redact(query));
        }
    }

    auto originatingCommand = curop.originatingCommand();
    if (!originatingCommand.isEmpty()) {
        pAttrs->add("originatingCommand", redact(originatingCommand));
    }

    if (!curop.getPlanSummary().empty()) {
        pAttrs->addDeepCopy("planSummary", curop.getPlanSummary().toString());
    }

    if (prepareConflictDurationMillis > Milliseconds::zero()) {
        pAttrs->add("prepareConflictDuration", prepareConflictDurationMillis);
    }

    if (dataThroughputLastSecond) {
        pAttrs->add("dataThroughputLastSecondMBperSec", *dataThroughputLastSecond);
    }

    if (dataThroughputAverage) {
        pAttrs->add("dataThroughputAverageMBPerSec", *dataThroughputAverage);
    }

    OPDEBUG_TOATTR_HELP(nShards);
    OPDEBUG_TOATTR_HELP(cursorid);
    if (mongotCursorId) {
        pAttrs->add("mongot", makeMongotDebugStatsObject());
    }
    OPDEBUG_TOATTR_HELP(ntoreturn);
    OPDEBUG_TOATTR_HELP(ntoskip);
    OPDEBUG_TOATTR_HELP_BOOL(exhaust);

    OPDEBUG_TOATTR_HELP_OPTIONAL("keysExamined", additiveMetrics.keysExamined);
    OPDEBUG_TOATTR_HELP_OPTIONAL("docsExamined", additiveMetrics.docsExamined);
    OPDEBUG_TOATTR_HELP_BOOL(hasSortStage);
    OPDEBUG_TOATTR_HELP_BOOL(usedDisk);
    OPDEBUG_TOATTR_HELP_BOOL(fromMultiPlanner);
    if (replanReason) {
        bool replanned = true;
        OPDEBUG_TOATTR_HELP_BOOL(replanned);
        pAttrs->add("replanReason", redact(*replanReason));
    }
    OPDEBUG_TOATTR_HELP_OPTIONAL("nMatched", additiveMetrics.nMatched);
    OPDEBUG_TOATTR_HELP_OPTIONAL("nModified", additiveMetrics.nModified);
    OPDEBUG_TOATTR_HELP_OPTIONAL("ninserted", additiveMetrics.ninserted);
    OPDEBUG_TOATTR_HELP_OPTIONAL("ndeleted", additiveMetrics.ndeleted);
    OPDEBUG_TOATTR_HELP_BOOL(upsert);
    OPDEBUG_TOATTR_HELP_BOOL(cursorExhausted);

    OPDEBUG_TOATTR_HELP_OPTIONAL("keysInserted", additiveMetrics.keysInserted);
    OPDEBUG_TOATTR_HELP_OPTIONAL("keysDeleted", additiveMetrics.keysDeleted);
    OPDEBUG_TOATTR_HELP_ATOMIC("prepareReadConflicts", additiveMetrics.prepareReadConflicts);
    OPDEBUG_TOATTR_HELP_ATOMIC("writeConflicts", additiveMetrics.writeConflicts);

    pAttrs->add("numYields", curop.numYields());
    OPDEBUG_TOATTR_HELP(nreturned);

    if (queryHash) {
        pAttrs->addDeepCopy("queryHash", unsignedIntToFixedLengthHex(*queryHash));
        invariant(planCacheKey);
        pAttrs->addDeepCopy("planCacheKey", unsignedIntToFixedLengthHex(*planCacheKey));
    }

    if (!errInfo.isOK()) {
        pAttrs->add("ok", 0);
        if (!errInfo.reason().empty()) {
            pAttrs->add("errMsg", redact(errInfo.reason()));
        }
        pAttrs->addDeepCopy("errName", errInfo.codeString());
        pAttrs->add("errCode", static_cast<int>(errInfo.code()));
    }

    if (responseLength > 0) {
        pAttrs->add("reslen", responseLength);
    }

    if (lockStats) {
        BSONObjBuilder locks;
        lockStats->report(&locks);
        pAttrs->add("locks", locks.obj());
    }

    BSONObj flowControlObj = makeFlowControlObject(flowControlStats);
    if (flowControlObj.nFields() > 0) {
        pAttrs->add("flowControl", flowControlObj);
    }

    {
        const auto& readConcern = repl::ReadConcernArgs::get(opCtx);
        if (readConcern.isSpecified()) {
            pAttrs->add("readConcern", readConcern.toBSONInner());
        }
    }

    if (writeConcern && !writeConcern->usedDefault) {
        pAttrs->add("writeConcern", writeConcern->toBSON());
    }

    if (storageStats) {
        pAttrs->add("storage", storageStats->toBSON());
    }

    if (iscommand) {
        pAttrs->add("protocol", getProtoString(networkOp));
    }

    if (remoteOpWaitTime) {
        pAttrs->add("remoteOpWaitMillis", durationCount<Milliseconds>(*remoteOpWaitTime));
    }

    pAttrs->add("durationMillis", (executionTimeMicros / 1000));
}


#define OPDEBUG_APPEND_NUMBER(x) \
    if (x != -1)                 \
    b.appendNumber(#x, (x))
#define OPDEBUG_APPEND_BOOL(x) \
    if (x)                     \
    b.appendBool(#x, (x))
#define OPDEBUG_APPEND_ATOMIC(x, y)   \
    if (auto __y = y.load(); __y > 0) \
    b.appendNumber(x, __y)
#define OPDEBUG_APPEND_OPTIONAL(x, y) \
    if (y)                            \
    b.appendNumber(x, (*y))

void OpDebug::append(OperationContext* opCtx,
                     const SingleThreadedLockStats& lockStats,
                     FlowControlTicketholder::CurOp flowControlStats,
                     BSONObjBuilder& b) const {
    auto& curop = *CurOp::get(opCtx);
    const size_t maxElementSize = 50 * 1024;

    b.append("op", logicalOpToString(logicalOp));

    NamespaceString nss = NamespaceString(curop.getNS());
    b.append("ns", nss.ns());

    appendAsObjOrString(
        "command", appendCommentField(opCtx, curop.opDescription()), maxElementSize, &b);

    auto originatingCommand = curop.originatingCommand();
    if (!originatingCommand.isEmpty()) {
        appendAsObjOrString("originatingCommand", originatingCommand, maxElementSize, &b);
    }

    OPDEBUG_APPEND_NUMBER(nShards);
    OPDEBUG_APPEND_NUMBER(cursorid);
    if (mongotCursorId) {
        b.append("mongot", makeMongotDebugStatsObject());
    }
    OPDEBUG_APPEND_BOOL(exhaust);

    OPDEBUG_APPEND_OPTIONAL("keysExamined", additiveMetrics.keysExamined);
    OPDEBUG_APPEND_OPTIONAL("docsExamined", additiveMetrics.docsExamined);
    OPDEBUG_APPEND_BOOL(hasSortStage);
    OPDEBUG_APPEND_BOOL(usedDisk);
    OPDEBUG_APPEND_BOOL(fromMultiPlanner);
    if (replanReason) {
        bool replanned = true;
        OPDEBUG_APPEND_BOOL(replanned);
        b.append("replanReason", *replanReason);
    }
    OPDEBUG_APPEND_OPTIONAL("nMatched", additiveMetrics.nMatched);
    OPDEBUG_APPEND_OPTIONAL("nModified", additiveMetrics.nModified);
    OPDEBUG_APPEND_OPTIONAL("ninserted", additiveMetrics.ninserted);
    OPDEBUG_APPEND_OPTIONAL("ndeleted", additiveMetrics.ndeleted);
    OPDEBUG_APPEND_BOOL(upsert);
    OPDEBUG_APPEND_BOOL(cursorExhausted);

    OPDEBUG_APPEND_OPTIONAL("keysInserted", additiveMetrics.keysInserted);
    OPDEBUG_APPEND_OPTIONAL("keysDeleted", additiveMetrics.keysDeleted);
    OPDEBUG_APPEND_ATOMIC("prepareReadConflicts", additiveMetrics.prepareReadConflicts);
    OPDEBUG_APPEND_ATOMIC("writeConflicts", additiveMetrics.writeConflicts);

    OPDEBUG_APPEND_OPTIONAL("dataThroughputLastSecond", dataThroughputLastSecond);
    OPDEBUG_APPEND_OPTIONAL("dataThroughputAverage", dataThroughputAverage);

    b.appendNumber("numYield", curop.numYields());
    OPDEBUG_APPEND_NUMBER(nreturned);

    if (queryHash) {
        b.append("queryHash", unsignedIntToFixedLengthHex(*queryHash));
        invariant(planCacheKey);
        b.append("planCacheKey", unsignedIntToFixedLengthHex(*planCacheKey));
    }

    {
        BSONObjBuilder locks(b.subobjStart("locks"));
        lockStats.report(&locks);
    }

    {
        BSONObj flowControlMetrics = makeFlowControlObject(flowControlStats);
        BSONObjBuilder flowControlBuilder(b.subobjStart("flowControl"));
        flowControlBuilder.appendElements(flowControlMetrics);
    }

    {
        const auto& readConcern = repl::ReadConcernArgs::get(opCtx);
        if (readConcern.isSpecified()) {
            readConcern.appendInfo(&b);
        }
    }

    if (writeConcern && !writeConcern->usedDefault) {
        b.append("writeConcern", writeConcern->toBSON());
    }

    if (storageStats) {
        b.append("storage", storageStats->toBSON());
    }

    if (!errInfo.isOK()) {
        b.appendNumber("ok", 0.0);
        if (!errInfo.reason().empty()) {
            b.append("errMsg", errInfo.reason());
        }
        b.append("errName", ErrorCodes::errorString(errInfo.code()));
        b.append("errCode", errInfo.code());
    }

    OPDEBUG_APPEND_NUMBER(responseLength);
    if (iscommand) {
        b.append("protocol", getProtoString(networkOp));
    }

    if (remoteOpWaitTime) {
        b.append("remoteOpWaitMillis", durationCount<Milliseconds>(*remoteOpWaitTime));
    }

    b.appendIntOrLL("millis", executionTimeMicros / 1000);

    if (!curop.getPlanSummary().empty()) {
        b.append("planSummary", curop.getPlanSummary());
    }

    if (!execStats.isEmpty()) {
        b.append("execStats", execStats);
    }
}

void OpDebug::setPlanSummaryMetrics(const PlanSummaryStats& planSummaryStats) {
    additiveMetrics.keysExamined = planSummaryStats.totalKeysExamined;
    additiveMetrics.docsExamined = planSummaryStats.totalDocsExamined;
    hasSortStage = planSummaryStats.hasSortStage;
    usedDisk = planSummaryStats.usedDisk;
    fromMultiPlanner = planSummaryStats.fromMultiPlanner;
    replanReason = planSummaryStats.replanReason;
}

BSONObj OpDebug::makeFlowControlObject(FlowControlTicketholder::CurOp stats) const {
    BSONObjBuilder builder;
    if (stats.ticketsAcquired > 0) {
        builder.append("acquireCount", stats.ticketsAcquired);
    }

    if (stats.acquireWaitCount > 0) {
        builder.append("acquireWaitCount", stats.acquireWaitCount);
    }

    if (stats.timeAcquiringMicros > 0) {
        builder.append("timeAcquiringMicros", stats.timeAcquiringMicros);
    }

    return builder.obj();
}

BSONObj OpDebug::makeMongotDebugStatsObject() const {
    BSONObjBuilder cursorBuilder;
    invariant(mongotCursorId);
    cursorBuilder.append("cursorid", mongotCursorId.get());
    if (msWaitingForMongot) {
        cursorBuilder.append("timeWaitingMillis", msWaitingForMongot.get());
    }
    return cursorBuilder.obj();
}


namespace {

/**
 * Adds two boost::optional long longs together. Returns boost::none if both 'lhs' and 'rhs' are
 * uninitialized, or the sum of 'lhs' and 'rhs' if they are both initialized. Returns 'lhs' if only
 * 'rhs' is uninitialized and vice-versa.
 */
boost::optional<long long> addOptionalLongs(const boost::optional<long long>& lhs,
                                            const boost::optional<long long>& rhs) {
    if (!rhs) {
        return lhs;
    }
    return lhs ? (*lhs + *rhs) : rhs;
}
}  // namespace

void OpDebug::AdditiveMetrics::add(const AdditiveMetrics& otherMetrics) {
    keysExamined = addOptionalLongs(keysExamined, otherMetrics.keysExamined);
    docsExamined = addOptionalLongs(docsExamined, otherMetrics.docsExamined);
    nMatched = addOptionalLongs(nMatched, otherMetrics.nMatched);
    nModified = addOptionalLongs(nModified, otherMetrics.nModified);
    ninserted = addOptionalLongs(ninserted, otherMetrics.ninserted);
    ndeleted = addOptionalLongs(ndeleted, otherMetrics.ndeleted);
    keysInserted = addOptionalLongs(keysInserted, otherMetrics.keysInserted);
    keysDeleted = addOptionalLongs(keysDeleted, otherMetrics.keysDeleted);
    prepareReadConflicts.fetchAndAdd(otherMetrics.prepareReadConflicts.load());
    writeConflicts.fetchAndAdd(otherMetrics.writeConflicts.load());
}

void OpDebug::AdditiveMetrics::reset() {
    keysExamined = boost::none;
    docsExamined = boost::none;
    nMatched = boost::none;
    nModified = boost::none;
    ninserted = boost::none;
    ndeleted = boost::none;
    keysInserted = boost::none;
    keysDeleted = boost::none;
    prepareReadConflicts.store(0);
    writeConflicts.store(0);
}

bool OpDebug::AdditiveMetrics::equals(const AdditiveMetrics& otherMetrics) const {
    return keysExamined == otherMetrics.keysExamined && docsExamined == otherMetrics.docsExamined &&
        nMatched == otherMetrics.nMatched && nModified == otherMetrics.nModified &&
        ninserted == otherMetrics.ninserted && ndeleted == otherMetrics.ndeleted &&
        keysInserted == otherMetrics.keysInserted && keysDeleted == otherMetrics.keysDeleted &&
        prepareReadConflicts.load() == otherMetrics.prepareReadConflicts.load() &&
        writeConflicts.load() == otherMetrics.writeConflicts.load();
}

void OpDebug::AdditiveMetrics::incrementWriteConflicts(long long n) {
    writeConflicts.fetchAndAdd(n);
}

void OpDebug::AdditiveMetrics::incrementKeysInserted(long long n) {
    if (!keysInserted) {
        keysInserted = 0;
    }
    *keysInserted += n;
}

void OpDebug::AdditiveMetrics::incrementKeysDeleted(long long n) {
    if (!keysDeleted) {
        keysDeleted = 0;
    }
    *keysDeleted += n;
}

void OpDebug::AdditiveMetrics::incrementNinserted(long long n) {
    if (!ninserted) {
        ninserted = 0;
    }
    *ninserted += n;
}

void OpDebug::AdditiveMetrics::incrementPrepareReadConflicts(long long n) {
    prepareReadConflicts.fetchAndAdd(n);
}

string OpDebug::AdditiveMetrics::report() const {
    StringBuilder s;

    OPDEBUG_TOSTRING_HELP_OPTIONAL("keysExamined", keysExamined);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("docsExamined", docsExamined);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("nMatched", nMatched);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("nModified", nModified);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("ninserted", ninserted);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("ndeleted", ndeleted);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("keysInserted", keysInserted);
    OPDEBUG_TOSTRING_HELP_OPTIONAL("keysDeleted", keysDeleted);
    OPDEBUG_TOSTRING_HELP_ATOMIC("prepareReadConflicts", prepareReadConflicts);
    OPDEBUG_TOSTRING_HELP_ATOMIC("writeConflicts", writeConflicts);

    return s.str();
}

void OpDebug::AdditiveMetrics::report(logv2::DynamicAttributes* pAttrs) const {
    OPDEBUG_TOATTR_HELP_OPTIONAL("keysExamined", keysExamined);
    OPDEBUG_TOATTR_HELP_OPTIONAL("docsExamined", docsExamined);
    OPDEBUG_TOATTR_HELP_OPTIONAL("nMatched", nMatched);
    OPDEBUG_TOATTR_HELP_OPTIONAL("nModified", nModified);
    OPDEBUG_TOATTR_HELP_OPTIONAL("ninserted", ninserted);
    OPDEBUG_TOATTR_HELP_OPTIONAL("ndeleted", ndeleted);
    OPDEBUG_TOATTR_HELP_OPTIONAL("keysInserted", keysInserted);
    OPDEBUG_TOATTR_HELP_OPTIONAL("keysDeleted", keysDeleted);
    OPDEBUG_TOATTR_HELP_ATOMIC("prepareReadConflicts", prepareReadConflicts);
    OPDEBUG_TOATTR_HELP_ATOMIC("writeConflicts", writeConflicts);
}

BSONObj OpDebug::AdditiveMetrics::reportBSON() const {
    BSONObjBuilder b;
    OPDEBUG_APPEND_OPTIONAL("keysExamined", keysExamined);
    OPDEBUG_APPEND_OPTIONAL("docsExamined", docsExamined);
    OPDEBUG_APPEND_OPTIONAL("nMatched", nMatched);
    OPDEBUG_APPEND_OPTIONAL("nModified", nModified);
    OPDEBUG_APPEND_OPTIONAL("ninserted", ninserted);
    OPDEBUG_APPEND_OPTIONAL("ndeleted", ndeleted);
    OPDEBUG_APPEND_OPTIONAL("keysInserted", keysInserted);
    OPDEBUG_APPEND_OPTIONAL("keysDeleted", keysDeleted);
    OPDEBUG_APPEND_ATOMIC("prepareReadConflicts", prepareReadConflicts);
    OPDEBUG_APPEND_ATOMIC("writeConflicts", writeConflicts);
    return b.obj();
}

}  // namespace mongo
