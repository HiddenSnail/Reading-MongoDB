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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kQuery

#include "mongo/platform/basic.h"

#include <memory>
#include <string>

#include "mongo/db/auth/authorization_session.h"
#include "mongo/db/catalog/collection.h"
#include "mongo/db/client.h"
#include "mongo/db/clientcursor.h"
#include "mongo/db/commands.h"
#include "mongo/db/curop.h"
#include "mongo/db/curop_failpoint_helpers.h"
#include "mongo/db/cursor_manager.h"
#include "mongo/db/db_raii.h"
#include "mongo/db/exec/working_set_common.h"
#include "mongo/db/query/cursor_response.h"
#include "mongo/db/query/find.h"
#include "mongo/db/query/find_common.h"
#include "mongo/db/query/getmore_request.h"
#include "mongo/db/query/plan_executor.h"
#include "mongo/db/query/plan_summary_stats.h"
#include "mongo/db/read_concern.h"
#include "mongo/db/repl/oplog.h"
#include "mongo/db/repl/replication_coordinator.h"
#include "mongo/db/repl/speculative_majority_read_info.h"
#include "mongo/db/service_context.h"
#include "mongo/db/stats/counters.h"
#include "mongo/db/stats/top.h"
#include "mongo/logv2/log.h"
#include "mongo/s/chunk_version.h"
#include "mongo/util/fail_point.h"
#include "mongo/util/scopeguard.h"
#include "mongo/util/time_support.h"

namespace mongo {

namespace {

MONGO_FAIL_POINT_DEFINE(rsStopGetMoreCmd);
MONGO_FAIL_POINT_DEFINE(GetMoreHangBeforeReadLock);

// The timeout when waiting for linearizable read concern on a getMore command.
static constexpr int kLinearizableReadConcernTimeout = 15000;

// getMore can run with any readConcern, because cursor-creating commands like find can run with any
// readConcern.  However, since getMore automatically uses the readConcern of the command that
// created the cursor, it is not appropriate to apply the default readConcern (just as
// client-specified readConcern isn't appropriate).
static const ReadConcernSupportResult kSupportsReadConcernResult{
    Status::OK(),
    {{ErrorCodes::InvalidOptions,
      "default read concern not permitted (getMore uses the cursor's read concern)"}}};

/**
 * Validates that the lsid of 'opCtx' matches that of 'cursor'. This must be called after
 * authenticating, so that it is safe to report the lsid of 'cursor'.
 */
void validateLSID(OperationContext* opCtx, const GetMoreRequest& request, ClientCursor* cursor) {
    uassert(50736,
            str::stream() << "Cannot run getMore on cursor " << request.cursorid
                          << ", which was not created in a session, in session "
                          << *opCtx->getLogicalSessionId(),
            !opCtx->getLogicalSessionId() || cursor->getSessionId());

    uassert(50737,
            str::stream() << "Cannot run getMore on cursor " << request.cursorid
                          << ", which was created in session " << *cursor->getSessionId()
                          << ", without an lsid",
            opCtx->getLogicalSessionId() || !cursor->getSessionId());

    uassert(50738,
            str::stream() << "Cannot run getMore on cursor " << request.cursorid
                          << ", which was created in session " << *cursor->getSessionId()
                          << ", in session " << *opCtx->getLogicalSessionId(),
            !opCtx->getLogicalSessionId() || !cursor->getSessionId() ||
                (opCtx->getLogicalSessionId() == cursor->getSessionId()));
}

/**
 * Validates that the txnNumber of 'opCtx' matches that of 'cursor'. This must be called after
 * authenticating, so that it is safe to report the txnNumber of 'cursor'.
 */
void validateTxnNumber(OperationContext* opCtx,
                       const GetMoreRequest& request,
                       ClientCursor* cursor) {
    uassert(50739,
            str::stream() << "Cannot run getMore on cursor " << request.cursorid
                          << ", which was not created in a transaction, in transaction "
                          << *opCtx->getTxnNumber(),
            !opCtx->getTxnNumber() || cursor->getTxnNumber());

    uassert(50740,
            str::stream() << "Cannot run getMore on cursor " << request.cursorid
                          << ", which was created in transaction " << *cursor->getTxnNumber()
                          << ", without a txnNumber",
            opCtx->getTxnNumber() || !cursor->getTxnNumber());

    uassert(50741,
            str::stream() << "Cannot run getMore on cursor " << request.cursorid
                          << ", which was created in transaction " << *cursor->getTxnNumber()
                          << ", in transaction " << *opCtx->getTxnNumber(),
            !opCtx->getTxnNumber() || !cursor->getTxnNumber() ||
                (*opCtx->getTxnNumber() == *cursor->getTxnNumber()));
}

/**
 * Apply the read concern from the cursor to this operation.
 */
void applyCursorReadConcern(OperationContext* opCtx, repl::ReadConcernArgs rcArgs) {
    const auto replicationMode = repl::ReplicationCoordinator::get(opCtx)->getReplicationMode();

    // Select the appropriate read source. If we are in a transaction with read concern majority,
    // this will already be set to kNoTimestamp, so don't set it again.
    if (replicationMode == repl::ReplicationCoordinator::modeReplSet &&
        rcArgs.getLevel() == repl::ReadConcernLevel::kMajorityReadConcern &&
        !opCtx->inMultiDocumentTransaction()) {
        switch (rcArgs.getMajorityReadMechanism()) {
            case repl::ReadConcernArgs::MajorityReadMechanism::kMajoritySnapshot: {
                // Make sure we read from the majority snapshot.
                opCtx->recoveryUnit()->abandonSnapshot();
                opCtx->recoveryUnit()->setTimestampReadSource(
                    RecoveryUnit::ReadSource::kMajorityCommitted);
                uassertStatusOK(opCtx->recoveryUnit()->obtainMajorityCommittedSnapshot());
                break;
            }
            case repl::ReadConcernArgs::MajorityReadMechanism::kSpeculative: {
                // Mark the operation as speculative and select the correct read source.
                repl::SpeculativeMajorityReadInfo::get(opCtx).setIsSpeculativeRead();
                opCtx->recoveryUnit()->abandonSnapshot();
                opCtx->recoveryUnit()->setTimestampReadSource(RecoveryUnit::ReadSource::kNoOverlap);
                break;
            }
        }
    }

    // For cursor commands that take locks internally, the read concern on the
    // OperationContext may affect the timestamp read source selected by the storage engine.
    // We place the cursor read concern onto the OperationContext so the lock acquisition
    // respects the cursor's read concern.
    {
        stdx::lock_guard<Client> lk(*opCtx->getClient());
        repl::ReadConcernArgs::get(opCtx) = rcArgs;
    }
}

/**
 * Sets a deadline on the operation if the originating command had a maxTimeMS specified or if this
 * is a tailable, awaitData cursor.
 */
void setUpOperationDeadline(OperationContext* opCtx,
                            const ClientCursor& cursor,
                            const GetMoreRequest& request,
                            bool disableAwaitDataFailpointActive) {

    // We assume that cursors created through a DBDirectClient are always used from their
    // original OperationContext, so we do not need to move time to and from the cursor.
    if (!opCtx->getClient()->isInDirectClient()) {
        // There is no time limit set directly on this getMore command. If the cursor is
        // awaitData, then we supply a default time of one second. Otherwise we roll over
        // any leftover time from the maxTimeMS of the operation that spawned this cursor,
        // applying it to this getMore.
        if (cursor.isAwaitData() && !disableAwaitDataFailpointActive) {
            awaitDataState(opCtx).waitForInsertsDeadline =
                opCtx->getServiceContext()->getPreciseClockSource()->now() +
                request.awaitDataTimeout.value_or(Seconds{1});
        } else if (cursor.getLeftoverMaxTimeMicros() < Microseconds::max()) {
            opCtx->setDeadlineAfterNowBy(cursor.getLeftoverMaxTimeMicros(),
                                         ErrorCodes::MaxTimeMSExpired);
        }
    }
}
/**
 * Sets up the OperationContext in order to correctly inherit options like the read concern from the
 * cursor to this operation.
 */
void setUpOperationContextStateForGetMore(OperationContext* opCtx,
                                          const ClientCursor& cursor,
                                          const GetMoreRequest& request,
                                          bool disableAwaitDataFailpointActive) {
    applyCursorReadConcern(opCtx, cursor.getReadConcernArgs());
    opCtx->setWriteConcern(cursor.getWriteConcernOptions());
    setUpOperationDeadline(opCtx, cursor, request, disableAwaitDataFailpointActive);

    // If the originating command had a 'comment' field, we extract it and set it on opCtx. Note
    // that if the 'getMore' command itself has a 'comment' field, we give precedence to it.
    auto comment = cursor.getOriginatingCommandObj()["comment"];
    if (!opCtx->getComment() && comment) {
        opCtx->setComment(comment.wrap());
    }
}

/**
 * A command for running getMore() against an existing cursor registered with a CursorManager.
 * Used to generate the next batch of results for a ClientCursor.
 *
 * Can be used in combination with any cursor-generating command (e.g. find, aggregate,
 * listIndexes).
 */
class GetMoreCmd final : public Command {
public:
    GetMoreCmd() : Command("getMore") {}

    std::unique_ptr<CommandInvocation> parse(OperationContext* opCtx,
                                             const OpMsgRequest& opMsgRequest) override {
        return std::make_unique<Invocation>(this, opMsgRequest);
    }

    class Invocation final : public CommandInvocation {
    public:
        Invocation(Command* cmd, const OpMsgRequest& request)
            : CommandInvocation(cmd),
              _request(uassertStatusOK(
                  GetMoreRequest::parseFromBSON(request.getDatabase().toString(), request.body))) {}

    private:
        bool supportsWriteConcern() const override {
            return false;
        }

        ReadConcernSupportResult supportsReadConcern(repl::ReadConcernLevel level) const override {
            return kSupportsReadConcernResult;
        }

        bool allowsAfterClusterTime() const override {
            return false;
        }

        bool canIgnorePrepareConflicts() const override {
            return true;
        }

        NamespaceString ns() const override {
            return _request.nss;
        }

        void doCheckAuthorization(OperationContext* opCtx) const override {
            uassertStatusOK(AuthorizationSession::get(opCtx->getClient())
                                ->checkAuthForGetMore(_request.nss,
                                                      _request.cursorid,
                                                      _request.term.is_initialized()));
        }

        /**
         * Uses 'cursor' and 'request' to fill out 'nextBatch' with the batch of result documents to
         * be returned by this getMore.
         *
         * Returns the number of documents in the batch in *numResults, which must be initialized to
         * zero by the caller. Returns the final ExecState returned by the cursor in *state.
         *
         * Returns an OK status if the batch was successfully generated, and a non-OK status if the
         * PlanExecutor encounters a failure.
         */
        Status generateBatch(OperationContext* opCtx,
                             ClientCursor* cursor,
                             const GetMoreRequest& request,
                             CursorResponseBuilder* nextBatch,
                             PlanExecutor::ExecState* state,
                             std::uint64_t* numResults) {
            PlanExecutor* exec = cursor->getExecutor();

            // If an awaitData getMore is killed during this process due to our max time expiring at
            // an interrupt point, we just continue as normal and return rather than reporting a
            // timeout to the user.
            Document doc;
            try {
                while (!FindCommon::enoughForGetMore(request.batchSize.value_or(0), *numResults) &&
                       PlanExecutor::ADVANCED == (*state = exec->getNext(&doc, nullptr))) {
                    auto* expCtx = exec->getExpCtx().get();
                    // Note that "needsMerge" implies a find or aggregate operation, which should
                    // always have a non-NULL 'expCtx' value.
                    BSONObj obj = cursor->needsMerge()
                        ? doc.toBsonWithMetaData(expCtx->sortKeyFormat)
                        : doc.toBson();

                    // If adding this object will cause us to exceed the message size limit, then we
                    // stash it for later.
                    if (!FindCommon::haveSpaceForNext(obj, *numResults, nextBatch->bytesUsed())) {
                        exec->enqueue(obj);
                        break;
                    }

                    // As soon as we get a result, this operation no longer waits.
                    awaitDataState(opCtx).shouldWaitForInserts = false;

                    // If this executor produces a postBatchResumeToken, add it to the response.
                    nextBatch->setPostBatchResumeToken(exec->getPostBatchResumeToken());
                    nextBatch->append(obj);
                    (*numResults)++;
                }
            } catch (const ExceptionFor<ErrorCodes::CloseChangeStream>&) {
                // FAILURE state will make getMore command close the cursor even if it's tailable.
                *state = PlanExecutor::FAILURE;
                return Status::OK();
            }

            switch (*state) {
                case PlanExecutor::FAILURE: {
                    // We should always have a valid status member object at this point.
                    auto status = WorkingSetCommon::getMemberObjectStatus(doc);
                    invariant(!status.isOK());
                    // Log an error message and then perform the cleanup.
                    LOGV2_WARNING(
                        20478,
                        "getMore command executor error: {state}, status: {error}, stats: {stats}",
                        "getMore command executor error",
                        "state"_attr = PlanExecutor::statestr(*state),
                        "error"_attr = status,
                        "stats"_attr = redact(Explain::getWinningPlanStats(exec)));

                    nextBatch->abandon();
                    return status;
                }
                case PlanExecutor::IS_EOF:
                    // The latest oplog timestamp may advance even when there are no results. Ensure
                    // that we have the latest postBatchResumeToken produced by the plan executor.
                    nextBatch->setPostBatchResumeToken(exec->getPostBatchResumeToken());
                default:
                    return Status::OK();
            }

            MONGO_UNREACHABLE;
        }

        void acquireLocksAndIterateCursor(OperationContext* opCtx,
                                          rpc::ReplyBuilderInterface* reply,
                                          CursorManager* cursorManager,
                                          ClientCursorPin& cursorPin,
                                          CurOp* curOp) {
            // Cursors come in one of two flavors:
            //
            // - Cursors which read from a single collection, such as those generated via the
            //   find command. For these cursors, we hold the appropriate collection lock for the
            //   duration of the getMore using AutoGetCollectionForRead. These cursors have the
            //   'kLockExternally' lock policy.
            //
            // - Cursors which may read from many collections, e.g. those generated via the
            //   aggregate command, or which do not read from a collection at all, e.g. those
            //   generated by the listIndexes command. We don't need to acquire locks to use these
            //   cursors, since they either manage locking themselves or don't access data protected
            //   by collection locks. These cursors have the 'kLocksInternally' lock policy.
            //
            // While we only need to acquire locks for 'kLockExternally' cursors, we need to create
            // an AutoStatsTracker in either case. This is responsible for updating statistics in
            // CurOp and Top. We avoid using AutoGetCollectionForReadCommand because we may need to
            // drop and reacquire locks when the cursor is awaitData, but we don't want to update
            // the stats twice.
            boost::optional<AutoGetCollectionForRead> readLock;
            boost::optional<AutoStatsTracker> statsTracker;

            {
                // We call RecoveryUnit::setTimestampReadSource() before acquiring a lock on the
                // collection via AutoGetCollectionForRead in order to ensure the comparison to the
                // collection's minimum visible snapshot is accurate.
                PlanExecutor* exec = cursorPin->getExecutor();
                const auto* cq = exec->getCanonicalQuery();

                if (auto clusterTime =
                        (cq ? cq->getQueryRequest().getReadAtClusterTime() : boost::none)) {
                    // We don't compare 'clusterTime' to the last applied opTime or to the
                    // all-committed timestamp because the testing infrastructure won't use the
                    // $_internalReadAtClusterTime option in any test suite where rollback is
                    // expected to occur.

                    // The $_internalReadAtClusterTime option causes any storage-layer cursors
                    // created during plan execution to read from a consistent snapshot of data at
                    // the supplied clusterTime, even across yields.
                    opCtx->recoveryUnit()->setTimestampReadSource(
                        RecoveryUnit::ReadSource::kProvided, clusterTime);

                    // The $_internalReadAtClusterTime option also causes any storage-layer cursors
                    // created during plan execution to block on prepared transactions. Since the
                    // getMore command ignores prepare conflicts by default, change the behavior.
                    opCtx->recoveryUnit()->setPrepareConflictBehavior(
                        PrepareConflictBehavior::kEnforce);
                }
            }
            if (cursorPin->lockPolicy() == ClientCursorParams::LockPolicy::kLocksInternally) {
                if (!_request.nss.isCollectionlessCursorNamespace()) {
                    const boost::optional<int> dbProfilingLevel = boost::none;
                    statsTracker.emplace(opCtx,
                                         _request.nss,
                                         Top::LockType::NotLocked,
                                         AutoStatsTracker::LogMode::kUpdateTopAndCurOp,
                                         dbProfilingLevel);
                }
            } else {
                invariant(cursorPin->lockPolicy() ==
                          ClientCursorParams::LockPolicy::kLockExternally);

                if (MONGO_unlikely(GetMoreHangBeforeReadLock.shouldFail())) {
                    LOGV2(20477,
                          "GetMoreHangBeforeReadLock fail point enabled. Blocking until fail "
                          "point is disabled");
                    GetMoreHangBeforeReadLock.pauseWhileSet(opCtx);
                }

                // Lock the backing collection by using the executor's namespace. Note that it may
                // be different from the cursor's namespace. One such possible scenario is when
                // getMore() is executed against a view. Technically, views are pipelines and under
                // normal circumstances use 'kLocksInternally' policy, so we shouldn't be getting
                // into here in the first place. However, if the pipeline was optimized away and
                // replaced with a query plan, its lock policy would have also been changed to
                // 'kLockExternally'. So, we'll use the executor's namespace to take the lock (which
                // is always the backing collection namespace), but will use the namespace provided
                // in the user request for profiling.
                // Otherwise, these two namespaces will match.
                readLock.emplace(opCtx, cursorPin->getExecutor()->nss());

                const int doNotChangeProfilingLevel = 0;
                statsTracker.emplace(opCtx,
                                     _request.nss,
                                     Top::LockType::ReadLocked,
                                     AutoStatsTracker::LogMode::kUpdateTopAndCurOp,
                                     readLock->getDb() ? readLock->getDb()->getProfilingLevel()
                                                       : doNotChangeProfilingLevel);

                // Check whether we are allowed to read from this node after acquiring our locks.
                uassertStatusOK(repl::ReplicationCoordinator::get(opCtx)->checkCanServeReadsFor(
                    opCtx, _request.nss, true));
            }

            // A user can only call getMore on their own cursor. If there were multiple users
            // authenticated when the cursor was created, then at least one of them must be
            // authenticated in order to run getMore on the cursor.
            auto authzSession = AuthorizationSession::get(opCtx->getClient());
            if (!authzSession->isCoauthorizedWith(cursorPin->getAuthenticatedUsers())) {
                uasserted(ErrorCodes::Unauthorized,
                          str::stream() << "cursor id " << _request.cursorid
                                        << " was not created by the authenticated user");
            }

            // Ensure that the client still has the privileges to run the originating command.
            if (!authzSession->isAuthorizedForPrivileges(cursorPin->getOriginatingPrivileges())) {
                uasserted(ErrorCodes::Unauthorized,
                          str::stream()
                              << "not authorized for getMore with cursor id " << _request.cursorid);
            }

            if (_request.nss != cursorPin->nss()) {
                uasserted(ErrorCodes::Unauthorized,
                          str::stream() << "Requested getMore on namespace '" << _request.nss.ns()
                                        << "', but cursor belongs to a different namespace "
                                        << cursorPin->nss().ns());
            }

            // Ensure the lsid and txnNumber of the getMore match that of the originating command.
            validateLSID(opCtx, _request, cursorPin.getCursor());
            validateTxnNumber(opCtx, _request, cursorPin.getCursor());

            if (_request.nss.isOplog() && MONGO_unlikely(rsStopGetMoreCmd.shouldFail())) {
                uasserted(ErrorCodes::CommandFailed,
                          str::stream() << "getMore on " << _request.nss.ns()
                                        << " rejected due to active fail point rsStopGetMoreCmd");
            }

            // Validation related to awaitData.
            if (cursorPin->isAwaitData()) {
                invariant(cursorPin->isTailable());
            }

            if (_request.awaitDataTimeout && !cursorPin->isAwaitData()) {
                uasserted(ErrorCodes::BadValue,
                          "cannot set maxTimeMS on getMore command for a non-awaitData cursor");
            }

            // On early return, get rid of the cursor.
            auto cursorFreer = makeGuard([&] { cursorPin.deleteUnderlying(); });

            // If the 'waitAfterPinningCursorBeforeGetMoreBatch' fail point is enabled, set the
            // 'msg' field of this operation's CurOp to signal that we've hit this point and then
            // repeatedly release and re-acquire the collection readLock at regular intervals until
            // the failpoint is released. This is done in order to avoid deadlocks caused by the
            // pinned-cursor failpoints in this file (see SERVER-21997).
            std::function<void()> dropAndReacquireReadLock = [&readLock, opCtx, this]() {
                // Make sure an interrupted operation does not prevent us from reacquiring the lock.
                UninterruptibleLockGuard noInterrupt(opCtx->lockState());
                readLock.reset();
                readLock.emplace(opCtx, _request.nss);
            };
            if (MONGO_unlikely(waitAfterPinningCursorBeforeGetMoreBatch.shouldFail())) {
                CurOpFailpointHelpers::waitWhileFailPointEnabled(
                    &waitAfterPinningCursorBeforeGetMoreBatch,
                    opCtx,
                    "waitAfterPinningCursorBeforeGetMoreBatch",
                    dropAndReacquireReadLock,
                    false,
                    _request.nss);
            }

            const bool disableAwaitDataFailpointActive =
                MONGO_unlikely(disableAwaitDataForGetMoreCmd.shouldFail());

            // Inherit properties like readConcern and maxTimeMS from our originating cursor.
            setUpOperationContextStateForGetMore(
                opCtx, *cursorPin.getCursor(), _request, disableAwaitDataFailpointActive);

            if (!cursorPin->isAwaitData()) {
                opCtx->checkForInterrupt();  // May trigger maxTimeAlwaysTimeOut fail point.
            }

            PlanExecutor* exec = cursorPin->getExecutor();
            const auto* cq = exec->getCanonicalQuery();
            if (cq && cq->getQueryRequest().isReadOnce()) {
                // The readOnce option causes any storage-layer cursors created during plan
                // execution to assume read data will not be needed again and need not be cached.
                opCtx->recoveryUnit()->setReadOnce(true);
            }
            exec->reattachToOperationContext(opCtx);
            exec->restoreState();

            auto planSummary = Explain::getPlanSummary(exec);
            {
                stdx::lock_guard<Client> lk(*opCtx->getClient());
                curOp->setPlanSummary_inlock(planSummary);

                // Ensure that the original query or command object is available in the slow query
                // log, profiler and currentOp.
                auto originatingCommand = cursorPin->getOriginatingCommandObj();
                if (!originatingCommand.isEmpty()) {
                    curOp->setOriginatingCommand_inlock(originatingCommand);
                }

                // Update the genericCursor stored in curOp with the new cursor stats.
                curOp->setGenericCursor_inlock(cursorPin->toGenericCursor());
            }

            // If the 'failGetMoreAfterCursorCheckout' failpoint is enabled, throw an exception with
            // the given 'errorCode' value, or ErrorCodes::InternalError if 'errorCode' is omitted.
            failGetMoreAfterCursorCheckout.executeIf(
                [](const BSONObj& data) {
                    auto errorCode = (data["errorCode"] ? data["errorCode"].safeNumberLong()
                                                        : ErrorCodes::InternalError);
                    uasserted(errorCode, "Hit the 'failGetMoreAfterCursorCheckout' failpoint");
                },
                [&opCtx, &cursorPin](const BSONObj& data) {
                    auto dataForFailCommand =
                        data.addField(BSON("failCommands" << BSON_ARRAY("getMore")).firstElement());
                    auto* getMoreCommand = CommandHelpers::findCommand("getMore");
                    return CommandHelpers::shouldActivateFailCommandFailPoint(
                        dataForFailCommand, cursorPin->nss(), getMoreCommand, opCtx->getClient());
                });

            CursorId respondWithId = 0;

            CursorResponseBuilder nextBatch(reply, CursorResponseBuilder::Options());
            BSONObj obj;
            PlanExecutor::ExecState state = PlanExecutor::ADVANCED;
            std::uint64_t numResults = 0;

            // We report keysExamined and docsExamined to OpDebug for a given getMore operation. To
            // obtain these values we need to take a diff of the pre-execution and post-execution
            // metrics, as they accumulate over the course of a cursor's lifetime.
            PlanSummaryStats preExecutionStats;
            Explain::getSummaryStats(*exec, &preExecutionStats);

            // Mark this as an AwaitData operation if appropriate.
            if (cursorPin->isAwaitData() && !disableAwaitDataFailpointActive) {
                auto lastKnownCommittedOpTime = _request.lastKnownCommittedOpTime;
                if (opCtx->isExhaust() && cursorPin->getLastKnownCommittedOpTime()) {
                    // Use the commit point of the last batch for exhaust cursors.
                    lastKnownCommittedOpTime = cursorPin->getLastKnownCommittedOpTime();
                }
                if (lastKnownCommittedOpTime) {
                    clientsLastKnownCommittedOpTime(opCtx) = lastKnownCommittedOpTime.get();
                }

                awaitDataState(opCtx).shouldWaitForInserts = true;
            }

            // We're about to begin running the PlanExecutor in order to fill the getMore batch. If
            // the 'waitWithPinnedCursorDuringGetMoreBatch' failpoint is active, set the 'msg' field
            // of this operation's CurOp to signal that we've hit this point and then spin until the
            // failpoint is released.
            std::function<void()> saveAndRestoreStateWithReadLockReacquisition =
                [exec, dropAndReacquireReadLock]() {
                    exec->saveState();
                    dropAndReacquireReadLock();
                    exec->restoreState();
                };

            waitWithPinnedCursorDuringGetMoreBatch.execute([&](const BSONObj& data) {
                CurOpFailpointHelpers::waitWhileFailPointEnabled(
                    &waitWithPinnedCursorDuringGetMoreBatch,
                    opCtx,
                    "waitWithPinnedCursorDuringGetMoreBatch",
                    data["shouldNotdropLock"].booleanSafe()
                        ? []() {} /*empty function*/
                        : saveAndRestoreStateWithReadLockReacquisition,
                    false,
                    _request.nss);
            });

            uassertStatusOK(generateBatch(
                opCtx, cursorPin.getCursor(), _request, &nextBatch, &state, &numResults));

            PlanSummaryStats postExecutionStats;
            Explain::getSummaryStats(*exec, &postExecutionStats);
            postExecutionStats.totalKeysExamined -= preExecutionStats.totalKeysExamined;
            postExecutionStats.totalDocsExamined -= preExecutionStats.totalDocsExamined;
            curOp->debug().setPlanSummaryMetrics(postExecutionStats);

            // We do not report 'execStats' for aggregation or other cursors with the
            // 'kLocksInternally' policy, both in the original request and subsequent getMore. It
            // would be useful to have this info for an aggregation, but the source PlanExecutor
            // could be destroyed before we know if we need 'execStats' and we do not want to
            // generate the stats eagerly for all operations due to cost.
            if (cursorPin->lockPolicy() != ClientCursorParams::LockPolicy::kLocksInternally &&
                curOp->shouldDBProfile()) {
                BSONObjBuilder execStatsBob;
                Explain::getWinningPlanStats(exec, &execStatsBob);
                curOp->debug().execStats = execStatsBob.obj();
            }

            if (shouldSaveCursorGetMore(state, exec, cursorPin->isTailable())) {
                respondWithId = _request.cursorid;

                exec->saveState();
                exec->detachFromOperationContext();

                cursorPin->setLeftoverMaxTimeMicros(opCtx->getRemainingMaxTimeMicros());
                cursorPin->incNReturnedSoFar(numResults);
                cursorPin->incNBatches();

                if (opCtx->isExhaust() && !clientsLastKnownCommittedOpTime(opCtx).isNull()) {
                    // Set the commit point of the latest batch.
                    auto replCoord = repl::ReplicationCoordinator::get(opCtx);
                    cursorPin->setLastKnownCommittedOpTime(replCoord->getLastCommittedOpTime());
                }
            } else {
                curOp->debug().cursorExhausted = true;
            }

            nextBatch.done(respondWithId, _request.nss.ns());

            // Ensure log and profiler include the number of results returned in this getMore's
            // response batch.
            curOp->debug().nreturned = numResults;

            if (respondWithId) {
                cursorFreer.dismiss();

                if (opCtx->isExhaust()) {
                    // Indicate that an exhaust message should be generated and the previous BSONObj
                    // command parameters should be reused as the next BSONObj command parameters.
                    reply->setNextInvocation(boost::none);
                }
            }
        }

        void run(OperationContext* opCtx, rpc::ReplyBuilderInterface* reply) override {
            // Counted as a getMore, not as a command.
            globalOpCounters.gotGetMore();
            auto curOp = CurOp::get(opCtx);
            curOp->debug().cursorid = _request.cursorid;

            // Validate term before acquiring locks, if provided.
            if (_request.term && _request.nss == NamespaceString::kRsOplogNamespace) {
                auto replCoord = repl::ReplicationCoordinator::get(opCtx);
                // Note: updateTerm returns ok if term stayed the same.
                uassertStatusOK(replCoord->updateTerm(opCtx, *_request.term));
                // If the term field is present in an oplog request, it means this is an oplog
                // getMore for replication oplog fetching because the term field is only allowed for
                // internal clients (see checkAuthForGetMore).
                curOp->debug().isReplOplogFetching = true;
            }

            auto cursorManager = CursorManager::get(opCtx);
            auto cursorPin = uassertStatusOK(cursorManager->pinCursor(opCtx, _request.cursorid));

            // Get the read concern level here in case the cursor is exhausted while iterating.
            const auto isLinearizableReadConcern = cursorPin->getReadConcernArgs().getLevel() ==
                repl::ReadConcernLevel::kLinearizableReadConcern;

            acquireLocksAndIterateCursor(opCtx, reply, cursorManager, cursorPin, curOp);

            if (isLinearizableReadConcern) {
                // waitForLinearizableReadConcern performs a NoOp write and waits for that write
                // to have been majority committed. awaitReplication requires that we release all
                // locks to prevent blocking for a long time while doing network activity. Since
                // getMores do not have support for a maxTimeout duration, we hardcode the timeout
                // to avoid waiting indefinitely.
                uassertStatusOK(
                    mongo::waitForLinearizableReadConcern(opCtx, kLinearizableReadConcernTimeout));
            }

            // We're about to unpin or delete the cursor as the ClientCursorPin goes out of scope.
            // If the 'waitBeforeUnpinningOrDeletingCursorAfterGetMoreBatch' failpoint is active, we
            // set the 'msg' field of this operation's CurOp to signal that we've hit this point and
            // then spin until the failpoint is released.
            if (MONGO_unlikely(waitBeforeUnpinningOrDeletingCursorAfterGetMoreBatch.shouldFail())) {
                CurOpFailpointHelpers::waitWhileFailPointEnabled(
                    &waitBeforeUnpinningOrDeletingCursorAfterGetMoreBatch,
                    opCtx,
                    "waitBeforeUnpinningOrDeletingCursorAfterGetMoreBatch");
            }
        }

        const GetMoreRequest _request;
    };

    bool maintenanceOk() const override {
        return false;
    }

    AllowedOnSecondary secondaryAllowed(ServiceContext*) const override {
        return AllowedOnSecondary::kAlways;
    }

    ReadWriteType getReadWriteType() const override {
        return ReadWriteType::kRead;
    }

    std::string help() const override {
        return "retrieve more results from an existing cursor";
    }

    LogicalOp getLogicalOp() const override {
        return LogicalOp::opGetMore;
    }

    std::size_t reserveBytesForReply() const override {
        // The extra 1K is an artifact of how we construct batches. We consider a batch to be full
        // when it exceeds the goal batch size. In the case that we are just below the limit and
        // then read a large document, the extra 1K helps prevent a final realloc+memcpy.
        return FindCommon::kMaxBytesToReturnToClientAtOnce + 1024u;
    }

    /**
     * A getMore command increments the getMore counter, not the command counter.
     */
    bool shouldAffectCommandCounter() const override {
        return false;
    }

    bool adminOnly() const override {
        return false;
    }
} getMoreCmd;

}  // namespace
}  // namespace mongo
