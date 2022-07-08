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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kCommand

#include "mongo/platform/basic.h"

#include "mongo/db/service_entry_point_common.h"

#include "mongo/base/checked_cast.h"
#include "mongo/bson/mutable/document.h"
#include "mongo/bson/util/bson_extract.h"
#include "mongo/client/server_is_master_monitor.h"
#include "mongo/db/audit.h"
#include "mongo/db/auth/authorization_session.h"
#include "mongo/db/auth/impersonation_session.h"
#include "mongo/db/client.h"
#include "mongo/db/command_can_run_here.h"
#include "mongo/db/commands.h"
#include "mongo/db/commands/test_commands_enabled.h"
#include "mongo/db/commands/txn_cmds_gen.h"
#include "mongo/db/curop.h"
#include "mongo/db/curop_failpoint_helpers.h"
#include "mongo/db/curop_metrics.h"
#include "mongo/db/cursor_manager.h"
#include "mongo/db/dbdirectclient.h"
#include "mongo/db/error_labels.h"
#include "mongo/db/initialize_operation_session_info.h"
#include "mongo/db/introspect.h"
#include "mongo/db/jsobj.h"
#include "mongo/db/lasterror.h"
#include "mongo/db/logical_clock.h"
#include "mongo/db/logical_session_id.h"
#include "mongo/db/logical_session_id_helpers.h"
#include "mongo/db/logical_time_validator.h"
#include "mongo/db/ops/write_ops.h"
#include "mongo/db/ops/write_ops_exec.h"
#include "mongo/db/query/find.h"
#include "mongo/db/read_concern.h"
#include "mongo/db/read_write_concern_defaults.h"
#include "mongo/db/repl/optime.h"
#include "mongo/db/repl/read_concern_args.h"
#include "mongo/db/repl/repl_client_info.h"
#include "mongo/db/repl/replication_coordinator.h"
#include "mongo/db/repl/speculative_majority_read_info.h"
#include "mongo/db/repl/storage_interface.h"
#include "mongo/db/run_op_kill_cursors.h"
#include "mongo/db/s/operation_sharding_state.h"
#include "mongo/db/s/sharded_connection_info.h"
#include "mongo/db/s/sharding_state.h"
#include "mongo/db/s/transaction_coordinator_factory.h"
#include "mongo/db/service_entry_point_common.h"
#include "mongo/db/session_catalog_mongod.h"
#include "mongo/db/snapshot_window_util.h"
#include "mongo/db/stats/counters.h"
#include "mongo/db/stats/server_read_concern_metrics.h"
#include "mongo/db/stats/top.h"
#include "mongo/db/transaction_participant.h"
#include "mongo/db/transaction_validation.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/factory.h"
#include "mongo/rpc/get_status_from_command_result.h"
#include "mongo/rpc/message.h"
#include "mongo/rpc/metadata.h"
#include "mongo/rpc/metadata/logical_time_metadata.h"
#include "mongo/rpc/metadata/oplog_query_metadata.h"
#include "mongo/rpc/metadata/repl_set_metadata.h"
#include "mongo/rpc/metadata/tracking_metadata.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/rpc/reply_builder_interface.h"
#include "mongo/transport/ismaster_metrics.h"
#include "mongo/transport/session.h"
#include "mongo/util/duration.h"
#include "mongo/util/fail_point.h"
#include "mongo/util/scopeguard.h"

#include <fmt/format.h>

using namespace fmt::literals;

namespace mongo {

MONGO_FAIL_POINT_DEFINE(rsStopGetMore);
MONGO_FAIL_POINT_DEFINE(respondWithNotPrimaryInCommandDispatch);
MONGO_FAIL_POINT_DEFINE(skipCheckingForNotMasterInCommandDispatch);
MONGO_FAIL_POINT_DEFINE(sleepMillisAfterCommandExecutionBegins);
MONGO_FAIL_POINT_DEFINE(waitAfterNewStatementBlocksBehindPrepare);
MONGO_FAIL_POINT_DEFINE(waitAfterCommandFinishesExecution);
MONGO_FAIL_POINT_DEFINE(failWithErrorCodeInRunCommand);

// Tracks the number of times a legacy unacknowledged write failed due to
// not master error resulted in network disconnection.
Counter64 notMasterLegacyUnackWrites;
ServerStatusMetricField<Counter64> displayNotMasterLegacyUnackWrites(
    "repl.network.notMasterLegacyUnacknowledgedWrites", &notMasterLegacyUnackWrites);

// Tracks the number of times an unacknowledged write failed due to not master error
// resulted in network disconnection.
Counter64 notMasterUnackWrites;
ServerStatusMetricField<Counter64> displayNotMasterUnackWrites(
    "repl.network.notMasterUnacknowledgedWrites", &notMasterUnackWrites);

namespace {

void generateLegacyQueryErrorResponse(const AssertionException& exception,
                                      const QueryMessage& queryMessage,
                                      CurOp* curop,
                                      Message* response) {
    curop->debug().errInfo = exception.toStatus();

    if (queryMessage.query.valid(BSONVersion::kLatest))
        LOGV2_OPTIONS(51777,
                      {logv2::LogComponent::kQuery},
                      "Assertion {error} ns: {namespace} query: {query}",
                      "Assertion for valid query",
                      "error"_attr = exception,
                      "namespace"_attr = queryMessage.ns,
                      "query"_attr = redact(queryMessage.query));
    else
        LOGV2_OPTIONS(51778,
                      {logv2::LogComponent::kQuery},
                      "Assertion {error} ns: {namespace} query object is corrupt",
                      "Assertion for query with corrupted object",
                      "error"_attr = exception,
                      "namespace"_attr = queryMessage.ns);

    if (queryMessage.ntoskip || queryMessage.ntoreturn) {
        LOGV2_OPTIONS(21952,
                      {logv2::LogComponent::kQuery},
                      "Query's nToSkip = {nToSkip} and nToReturn = {nToReturn}",
                      "Assertion for query with nToSkip and/or nToReturn",
                      "nToSkip"_attr = queryMessage.ntoskip,
                      "nToReturn"_attr = queryMessage.ntoreturn);
    }

    BSONObjBuilder err;
    err.append("$err", exception.reason());
    err.append("code", exception.code());
    err.append("ok", 0.0);
    auto const extraInfo = exception.extraInfo();
    if (extraInfo) {
        extraInfo->serialize(&err);
    }
    BSONObj errObj = err.done();

    const bool isStaleConfig = exception.code() == ErrorCodes::StaleConfig;
    if (isStaleConfig) {
        LOGV2_OPTIONS(21953,
                      {logv2::LogComponent::kQuery},
                      "Stale version detected during query over {namespace}: {error}",
                      "Detected stale version while querying namespace",
                      "namespace"_attr = queryMessage.ns,
                      "error"_attr = errObj);
    }

    BufBuilder bb;
    bb.skip(sizeof(QueryResult::Value));
    bb.appendBuf((void*)errObj.objdata(), errObj.objsize());

    // TODO: call replyToQuery() from here instead of this!!! see dbmessage.h
    QueryResult::View msgdata = bb.buf();
    QueryResult::View qr = msgdata;
    qr.setResultFlags(ResultFlag_ErrSet);
    if (isStaleConfig) {
        qr.setResultFlags(qr.getResultFlags() | ResultFlag_ShardConfigStale);
    }
    qr.msgdata().setLen(bb.len());
    qr.msgdata().setOperation(opReply);
    qr.setCursorId(0);
    qr.setStartingFrom(0);
    qr.setNReturned(1);
    response->setData(bb.release());
}

void registerError(OperationContext* opCtx, const DBException& exception) {
    LastError::get(opCtx->getClient()).setLastError(exception.code(), exception.reason());
    CurOp::get(opCtx)->debug().errInfo = exception.toStatus();
}

void generateErrorResponse(OperationContext* opCtx,
                           rpc::ReplyBuilderInterface* replyBuilder,
                           const DBException& exception,
                           const BSONObj& replyMetadata,
                           BSONObj extraFields = {}) {
    registerError(opCtx, exception);

    // We could have thrown an exception after setting fields in the builder,
    // so we need to reset it to a clean state just to be sure.
    replyBuilder->reset();
    replyBuilder->setCommandReply(exception.toStatus(), extraFields);
    replyBuilder->getBodyBuilder().appendElements(replyMetadata);
}

/**
 * Guard object for making a good-faith effort to enter maintenance mode and leave it when it
 * goes out of scope.
 *
 * Sometimes we cannot set maintenance mode, in which case the call to setMaintenanceMode will
 * return a non-OK status.  This class does not treat that case as an error which means that
 * anybody using it is assuming it is ok to continue execution without maintenance mode.
 *
 * TODO: This assumption needs to be audited and documented, or this behavior should be moved
 * elsewhere.
 */
class MaintenanceModeSetter {
    MaintenanceModeSetter(const MaintenanceModeSetter&) = delete;
    MaintenanceModeSetter& operator=(const MaintenanceModeSetter&) = delete;

public:
    MaintenanceModeSetter(OperationContext* opCtx)
        : _opCtx(opCtx),
          _maintenanceModeSet(
              repl::ReplicationCoordinator::get(_opCtx)->setMaintenanceMode(true).isOK()) {}

    ~MaintenanceModeSetter() {
        if (_maintenanceModeSet) {
            repl::ReplicationCoordinator::get(_opCtx)
                ->setMaintenanceMode(false)
                .transitional_ignore();
        }
    }

private:
    OperationContext* const _opCtx;
    const bool _maintenanceModeSet;
};

/**
 * Given the specified command, returns an effective read concern which should be used or an error
 * if the read concern is not valid for the command.
 * Note that the validation performed is not necessarily exhaustive.
 */
StatusWith<repl::ReadConcernArgs> _extractReadConcern(OperationContext* opCtx,
                                                      const CommandInvocation* invocation,
                                                      const BSONObj& cmdObj,
                                                      bool startTransaction,
                                                      bool isInternalClient) {
    repl::ReadConcernArgs readConcernArgs;

    auto readConcernParseStatus = readConcernArgs.initialize(cmdObj);
    if (!readConcernParseStatus.isOK()) {
        return readConcernParseStatus;
    }

    bool clientSuppliedReadConcern = readConcernArgs.isSpecified();
    bool customDefaultWasApplied = false;
    auto readConcernSupport = invocation->supportsReadConcern(readConcernArgs.getLevel());
    if (readConcernSupport.defaultReadConcernPermit.isOK() &&
        (startTransaction || !opCtx->inMultiDocumentTransaction()) &&
        repl::ReplicationCoordinator::get(opCtx)->isReplEnabled() &&
        !opCtx->getClient()->isInDirectClient()) {

        if (serverGlobalParams.featureCompatibility.isVersion(
                ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44)) {
            if (isInternalClient) {
                // ReadConcern should always be explicitly specified by operations received from
                // internal clients (ie. from a mongos or mongod), even if it is empty (ie.
                // readConcern: {}, meaning to use the implicit server defaults).
                uassert(
                    4569200,
                    "received command without explicit readConcern on an internalClient connection {}"_format(
                        redact(cmdObj.toString())),
                    readConcernArgs.isSpecified());
            } else if (serverGlobalParams.clusterRole == ClusterRole::ShardServer ||
                       serverGlobalParams.clusterRole == ClusterRole::ConfigServer) {
                if (!readConcernArgs.isSpecified()) {
                    // TODO: Disabled until after SERVER-44539, to avoid log spam.
                    // LOGV2(21954, "Missing readConcern on {command}", "Missing readConcern "
                    // "for command", "command"_attr = invocation->definition()->getName());
                }
            }
        }

        if (serverGlobalParams.clusterRole != ClusterRole::ShardServer &&
            serverGlobalParams.clusterRole != ClusterRole::ConfigServer) {
            // A member in a regular replica set.  Since these servers receive client queries, in
            // this context empty RC (ie. readConcern: {}) means the same as if absent/unspecified,
            // which is to apply the CWRWC defaults if present.  This means we just test isEmpty(),
            // since this covers both isSpecified() && !isSpecified()
            if (!isInternalClient && readConcernArgs.isEmpty()) {
                const auto rcDefault = ReadWriteConcernDefaults::get(opCtx->getServiceContext())
                                           .getDefaultReadConcern(opCtx);
                if (rcDefault) {
                    customDefaultWasApplied = true;
                    readConcernArgs = std::move(*rcDefault);
                    LOGV2_DEBUG(21955,
                                2,
                                "Applying default readConcern on {command} of {readConcernDefault} "
                                "on {command}",
                                "Applying default readConcern on command",
                                "readConcernDefault"_attr = *rcDefault,
                                "command"_attr = invocation->definition()->getName());
                    // Update the readConcernSupport, since the default RC was applied.
                    readConcernSupport =
                        invocation->supportsReadConcern(readConcernArgs.getLevel());
                }
            }
        }
    }

    // It's fine for clients to provide any provenance value to mongod. But if they haven't, then an
    // appropriate provenance needs to be determined.
    auto& provenance = readConcernArgs.getProvenance();
    if (!provenance.hasSource()) {
        if (clientSuppliedReadConcern) {
            provenance.setSource(ReadWriteConcernProvenance::Source::clientSupplied);
        } else if (customDefaultWasApplied) {
            provenance.setSource(ReadWriteConcernProvenance::Source::customDefault);
        } else {
            provenance.setSource(ReadWriteConcernProvenance::Source::implicitDefault);
        }
    }

    // If we are starting a transaction, we need to check whether the read concern is
    // appropriate for running a transaction.
    if (startTransaction) {
        if (!isReadConcernLevelAllowedInTransaction(readConcernArgs.getLevel())) {
            return {ErrorCodes::InvalidOptions,
                    "The readConcern level must be either 'local' (default), 'majority' or "
                    "'snapshot' in "
                    "order to run in a transaction"};
        }
        if (readConcernArgs.getArgsOpTime()) {
            return {ErrorCodes::InvalidOptions,
                    str::stream() << "The readConcern cannot specify '"
                                  << repl::ReadConcernArgs::kAfterOpTimeFieldName
                                  << "' in a transaction"};
        }
    }

    // Otherwise, if there is a read concern present - either user-specified or from the default -
    // then check whether the command supports it. If there is no explicit read concern level, then
    // it is implicitly "local". There is no need to check whether this is supported, because all
    // commands either support "local" or upconvert the absent readConcern to a stronger level that
    // they do support; for instance, $changeStream upconverts to RC level "majority".
    //
    // Individual transaction statements are checked later on, after we've unstashed the
    // transaction resources.
    if (!opCtx->inMultiDocumentTransaction() && readConcernArgs.hasLevel()) {
        if (!readConcernSupport.readConcernSupport.isOK()) {
            return readConcernSupport.readConcernSupport.withContext(
                str::stream() << "Command " << invocation->definition()->getName()
                              << " does not support " << readConcernArgs.toString());
        }
    }

    // If this command invocation asked for 'majority' read concern, supports blocking majority
    // reads, and storage engine support for majority reads is disabled, then we set the majority
    // read mechanism appropriately i.e. we utilize "speculative" read behavior.
    if (readConcernArgs.getLevel() == repl::ReadConcernLevel::kMajorityReadConcern &&
        invocation->allowsSpeculativeMajorityReads() &&
        !serverGlobalParams.enableMajorityReadConcern) {
        readConcernArgs.setMajorityReadMechanism(
            repl::ReadConcernArgs::MajorityReadMechanism::kSpeculative);
    }

    return readConcernArgs;
}

/**
 * For replica set members it returns the last known op time from opCtx. Otherwise will return
 * uninitialized cluster time.
 */
LogicalTime getClientOperationTime(OperationContext* opCtx) {
    auto const replCoord = repl::ReplicationCoordinator::get(opCtx);
    const bool isReplSet =
        replCoord->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet;

    if (!isReplSet) {
        return LogicalTime();
    }

    return LogicalTime(
        repl::ReplClientInfo::forClient(opCtx->getClient()).getMaxKnownOperationTime());
}

/**
 * Returns the proper operationTime for a command. To construct the operationTime for replica set
 * members, it uses the last optime in the oplog for writes, last committed optime for majority
 * reads, and the last applied optime for every other read. An uninitialized cluster time is
 * returned for non replica set members.
 *
 * The latest in-memory clusterTime is returned if the start operationTime is uninitialized.
 */
LogicalTime computeOperationTime(OperationContext* opCtx, LogicalTime startOperationTime) {
    auto const replCoord = repl::ReplicationCoordinator::get(opCtx);
    const bool isReplSet =
        replCoord->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet;
    invariant(isReplSet);

    auto operationTime = getClientOperationTime(opCtx);
    invariant(operationTime >= startOperationTime);

    // If the last operationTime has not changed, consider this command a read, and, for replica set
    // members, construct the operationTime with the proper optime for its read concern level.
    if (operationTime == startOperationTime) {
        auto& readConcernArgs = repl::ReadConcernArgs::get(opCtx);

        // Note: ReadConcernArgs::getLevel returns kLocal if none was set.
        if (readConcernArgs.getLevel() == repl::ReadConcernLevel::kMajorityReadConcern) {
            operationTime = LogicalTime(replCoord->getLastCommittedOpTime().getTimestamp());
        } else {
            operationTime = LogicalTime(replCoord->getMyLastAppliedOpTime().getTimestamp());
        }
    }

    return operationTime;
}

/**
 * Computes the proper $clusterTime and operationTime values to include in the command response and
 * appends them to it. $clusterTime is added as metadata and operationTime as a command body field.
 *
 * The command body BSONObjBuilder is either the builder for the command body itself, or a builder
 * for extra fields to be added to the reply when generating an error response.
 */
void appendClusterAndOperationTime(OperationContext* opCtx,
                                   BSONObjBuilder* commandBodyFieldsBob,
                                   BSONObjBuilder* metadataBob,
                                   LogicalTime startTime) {
    if (repl::ReplicationCoordinator::get(opCtx)->getReplicationMode() !=
            repl::ReplicationCoordinator::modeReplSet ||
        !LogicalClock::get(opCtx)->isEnabled()) {
        return;
    }

    // Authorized clients always receive operationTime and dummy signed $clusterTime.
    if (LogicalTimeValidator::isAuthorizedToAdvanceClock(opCtx)) {
        auto operationTime = computeOperationTime(opCtx, startTime);
        auto signedTime = SignedLogicalTime(
            LogicalClock::get(opCtx)->getClusterTime(), TimeProofService::TimeProof(), 0);

        dassert(signedTime.getTime() >= operationTime);
        rpc::LogicalTimeMetadata(signedTime).writeToMetadata(metadataBob);

        LOGV2_DEBUG(
            21957,
            5,
            "Appending operationTime to cmd response for authorized client: {operationTime}",
            "operationTime"_attr = operationTime);
        operationTime.appendAsOperationTime(commandBodyFieldsBob);

        return;
    }

    // Servers without validators (e.g. a shard server not yet added to a cluster) do not return
    // logical times to unauthorized clients.
    auto validator = LogicalTimeValidator::get(opCtx);
    if (!validator) {
        return;
    }

    auto operationTime = computeOperationTime(opCtx, startTime);
    auto signedTime = validator->trySignLogicalTime(LogicalClock::get(opCtx)->getClusterTime());

    // If there were no keys, do not return $clusterTime or operationTime to unauthorized clients.
    if (signedTime.getKeyId() == 0) {
        return;
    }

    dassert(signedTime.getTime() >= operationTime);
    rpc::LogicalTimeMetadata(signedTime).writeToMetadata(metadataBob);

    LOGV2_DEBUG(21958,
                5,
                "Appending operationTime to cmd response: {operationTime}",
                "operationTime"_attr = operationTime);
    operationTime.appendAsOperationTime(commandBodyFieldsBob);
}

void appendErrorLabelsAndTopologyVersion(OperationContext* opCtx,
                                         BSONObjBuilder* commandBodyFieldsBob,
                                         const OperationSessionInfoFromClient& sessionOptions,
                                         const std::string& commandName,
                                         boost::optional<ErrorCodes::Error> code,
                                         boost::optional<ErrorCodes::Error> wcCode,
                                         bool isInternalClient) {
    auto errorLabels =
        getErrorLabels(opCtx, sessionOptions, commandName, code, wcCode, isInternalClient);
    commandBodyFieldsBob->appendElements(errorLabels);

    auto isNotMasterError = false;
    if (code) {
        isNotMasterError = ErrorCodes::isA<ErrorCategory::NotMasterError>(*code);
    }

    if (!isNotMasterError && wcCode) {
        isNotMasterError = ErrorCodes::isA<ErrorCategory::NotMasterError>(*wcCode);
    }

    const auto replCoord = repl::ReplicationCoordinator::get(opCtx);
    if (replCoord->getReplicationMode() != repl::ReplicationCoordinator::modeReplSet ||
        !isNotMasterError) {
        return;
    }
    const auto topologyVersion = replCoord->getTopologyVersion();
    BSONObjBuilder topologyVersionBuilder(commandBodyFieldsBob->subobjStart("topologyVersion"));
    topologyVersion.serialize(&topologyVersionBuilder);
}

namespace {
void _abortUnpreparedOrStashPreparedTransaction(
    OperationContext* opCtx, TransactionParticipant::Participant* txnParticipant) {
    const bool isPrepared = txnParticipant->transactionIsPrepared();
    try {
        if (isPrepared)
            txnParticipant->stashTransactionResources(opCtx);
        else if (txnParticipant->transactionIsOpen())
            txnParticipant->abortTransaction(opCtx);
    } catch (...) {
        // It is illegal for this to throw so we catch and log this here for diagnosability.
        LOGV2_FATAL_CONTINUE(21974,
                             "Caught exception during transaction {txnNumber} {operation} "
                             "{logicalSessionId}: {error}",
                             "Unable to stash/abort transaction",
                             "operation"_attr = (isPrepared ? "stash" : "abort"),
                             "txnNumber"_attr = opCtx->getTxnNumber(),
                             "logicalSessionId"_attr = opCtx->getLogicalSessionId()->toBSON(),
                             "error"_attr = exceptionToStatus());
        std::terminate();
    }
}
}  // namespace

void invokeWithSessionCheckedOut(OperationContext* opCtx,
                                 const OpMsgRequest& request,
                                 CommandInvocation* invocation,
                                 const OperationSessionInfoFromClient& sessionOptions,
                                 rpc::ReplyBuilderInterface* replyBuilder) {
    // This constructor will check out the session. It handles the appropriate state management
    // for both multi-statement transactions and retryable writes. Currently, only requests with
    // a transaction number will check out the session.
    MongoDOperationContextSession sessionTxnState(opCtx);
    auto txnParticipant = TransactionParticipant::get(opCtx);

    if (!opCtx->getClient()->isInDirectClient()) {
        bool beganOrContinuedTxn{false};
        // This loop allows new transactions on a session to block behind a previous prepared
        // transaction on that session.
        while (!beganOrContinuedTxn) {
            try {
                txnParticipant.beginOrContinue(opCtx,
                                               *sessionOptions.getTxnNumber(),
                                               sessionOptions.getAutocommit(),
                                               sessionOptions.getStartTransaction());
                beganOrContinuedTxn = true;
            } catch (const ExceptionFor<ErrorCodes::PreparedTransactionInProgress>&) {
                auto prepareCompleted = txnParticipant.onExitPrepare();

                CurOpFailpointHelpers::waitWhileFailPointEnabled(
                    &waitAfterNewStatementBlocksBehindPrepare,
                    opCtx,
                    "waitAfterNewStatementBlocksBehindPrepare");

                // Check the session back in and wait for ongoing prepared transaction to complete.
                MongoDOperationContextSession::checkIn(opCtx);
                prepareCompleted.wait(opCtx);
                // Wait for the prepared commit or abort oplog entry to be visible in the oplog.
                // This will prevent a new transaction from missing the transaction table update for
                // the previous prepared commit or abort due to an oplog hole.
                auto storageInterface = repl::StorageInterface::get(opCtx);
                storageInterface->waitForAllEarlierOplogWritesToBeVisible(opCtx);
                MongoDOperationContextSession::checkOut(opCtx);
            }
        }

        // Create coordinator if needed. If "startTransaction" is present, it must be true.
        if (sessionOptions.getStartTransaction()) {
            // If this shard has been selected as the coordinator, set up the coordinator state
            // to be ready to receive votes.
            if (sessionOptions.getCoordinator() == boost::optional<bool>(true)) {
                createTransactionCoordinator(opCtx, *sessionOptions.getTxnNumber());
            }
        }

        // Release the transaction lock resources and abort storage transaction for unprepared
        // transactions on failure to unstash the transaction resources to opCtx. We don't want to
        // have this error guard for beginOrContinue as it can abort the transaction for any
        // accidental invalid statements in the transaction.
        auto abortOnError = makeGuard([&txnParticipant, opCtx] {
            if (txnParticipant.transactionIsInProgress()) {
                txnParticipant.abortTransaction(opCtx);
            }
        });

        txnParticipant.unstashTransactionResources(opCtx, invocation->definition()->getName());

        // Unstash success.
        abortOnError.dismiss();
    }

    auto guard = makeGuard([opCtx, &txnParticipant] {
        _abortUnpreparedOrStashPreparedTransaction(opCtx, &txnParticipant);
    });

    if (!opCtx->getClient()->isInDirectClient()) {
        const auto& readConcernArgs = repl::ReadConcernArgs::get(opCtx);

        // For replica sets, we do not receive the readConcernArgs of our parent transaction
        // statements until we unstash the transaction resources. The below check is necessary to
        // ensure commands, including those occurring after the first statement in their respective
        // transactions, are checked for readConcern support. Presently, only `create` and
        // `createIndexes` do not support readConcern inside transactions.
        // TODO(SERVER-46971): Consider how to extend this check to other commands.
        auto cmdName = invocation->definition()->getName();
        auto readConcernSupport = invocation->supportsReadConcern(readConcernArgs.getLevel());
        if (readConcernArgs.hasLevel() &&
            (cmdName == "create"_sd || cmdName == "createIndexes"_sd)) {
            if (!readConcernSupport.readConcernSupport.isOK()) {
                uassertStatusOK(readConcernSupport.readConcernSupport.withContext(
                    str::stream() << "Command " << cmdName
                                  << " does not support this transaction's "
                                  << readConcernArgs.toString()));
            }
        }
    }

    try {
        CommandHelpers::runCommandInvocation(opCtx, request, invocation, replyBuilder);
    } catch (const ExceptionFor<ErrorCodes::CommandOnShardedViewNotSupportedOnMongod>&) {
        // Exceptions are used to resolve views in a sharded cluster, so they should be handled
        // specially to avoid unnecessary aborts.

        // If "startTransaction" is present, it must be true.
        if (sessionOptions.getStartTransaction()) {
            // If the first command a shard receives in a transactions fails with this code, the
            // shard may not be included in the final participant list if the router's retry after
            // resolving the view does not re-target it, which is possible if the underlying
            // collection is sharded. The shard's transaction should be preemptively aborted to
            // avoid leaving it orphaned in this case, which is fine even if it is re-targeted
            // because the retry will include "startTransaction" again and "restart" a transaction
            // at the active txnNumber.
            throw;
        }

        // If this shard has completed an earlier statement for this transaction, it must already be
        // in the transaction's participant list, so it is guaranteed to learn its outcome.
        txnParticipant.stashTransactionResources(opCtx);
        guard.dismiss();
        throw;
    } catch (const ExceptionFor<ErrorCodes::WouldChangeOwningShard>&) {
        txnParticipant.stashTransactionResources(opCtx);
        txnParticipant.resetRetryableWriteState(opCtx);
        guard.dismiss();
        throw;
    }

    if (auto okField = replyBuilder->getBodyBuilder().asTempObj()["ok"]) {
        // If ok is present, use its truthiness.
        if (!okField.trueValue()) {
            return;
        }
    }

    // Stash or commit the transaction when the command succeeds.
    txnParticipant.stashTransactionResources(opCtx);
    guard.dismiss();

    if (serverGlobalParams.clusterRole == ClusterRole::ShardServer ||
        serverGlobalParams.clusterRole == ClusterRole::ConfigServer) {
        auto txnResponseMetadata = txnParticipant.getResponseMetadata();
        auto bodyBuilder = replyBuilder->getBodyBuilder();
        txnResponseMetadata.serialize(&bodyBuilder);
    }
}

bool runCommandImpl(OperationContext* opCtx,
                    CommandInvocation* invocation,
                    const OpMsgRequest& request,
                    rpc::ReplyBuilderInterface* replyBuilder,
                    LogicalTime startOperationTime,
                    const ServiceEntryPointCommon::Hooks& behaviors,
                    BSONObjBuilder* extraFieldsBuilder,
                    const OperationSessionInfoFromClient& sessionOptions) {
    const Command* command = invocation->definition();
    auto bytesToReserve = command->reserveBytesForReply();
// SERVER-22100: In Windows DEBUG builds, the CRT heap debugging overhead, in conjunction with the
// additional memory pressure introduced by reply buffer pre-allocation, causes the concurrency
// suite to run extremely slowly. As a workaround we do not pre-allocate in Windows DEBUG builds.
#ifdef _WIN32
    if (kDebugBuild)
        bytesToReserve = 0;
#endif
    replyBuilder->reserveBytes(bytesToReserve);

    const auto isInternalClient = opCtx->getClient()->session() &&
        (opCtx->getClient()->session()->getTags() & transport::Session::kInternalClient);

    const bool shouldCheckOutSession =
        sessionOptions.getTxnNumber() && !shouldCommandSkipSessionCheckout(command->getName());

    // getMore operations inherit a WriteConcern from their originating cursor. For example, if the
    // originating command was an aggregate with a $out and batchSize: 0. Note that if the command
    // only performed reads then we will not need to wait at all.
    const bool shouldWaitForWriteConcern =
        invocation->supportsWriteConcern() || command->getLogicalOp() == LogicalOp::opGetMore;

    if (shouldWaitForWriteConcern) {
        auto lastOpBeforeRun = repl::ReplClientInfo::forClient(opCtx->getClient()).getLastOp();

        // Change the write concern while running the command.
        const auto oldWC = opCtx->getWriteConcern();
        ON_BLOCK_EXIT([&] { opCtx->setWriteConcern(oldWC); });

        boost::optional<WriteConcernOptions> extractedWriteConcern;
        if (command->getLogicalOp() == LogicalOp::opGetMore) {
            // WriteConcern will be set up during command processing, it must not be specified on
            // the command body.
            behaviors.uassertCommandDoesNotSpecifyWriteConcern(request.body);
        } else {
            // WriteConcern should always be explicitly specified by operations received on shard
            // and config servers, even if it is empty (ie. writeConcern: {}).  In this context
            // (shard/config servers) an empty WC indicates the operation should use the implicit
            // server defaults.  So, warn if the operation has not specified writeConcern and is on
            // a shard/config server.
            if (!opCtx->getClient()->isInDirectClient() &&
                (!opCtx->inMultiDocumentTransaction() ||
                 isTransactionCommand(command->getName()))) {
                if (serverGlobalParams.featureCompatibility.isVersion(
                        ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44)) {
                    if (isInternalClient) {
                        // WriteConcern should always be explicitly specified by operations received
                        // from internal clients (ie. from a mongos or mongod), even if it is empty
                        // (ie. writeConcern: {}, which is equivalent to { w: 1, wtimeout: 0 }).
                        uassert(
                            4569201,
                            "received command without explicit writeConcern on an internalClient connection {}"_format(
                                redact(request.body.toString())),
                            request.body.hasField(WriteConcernOptions::kWriteConcernField));
                    } else if (serverGlobalParams.clusterRole == ClusterRole::ShardServer ||
                               serverGlobalParams.clusterRole == ClusterRole::ConfigServer) {
                        if (!request.body.hasField(WriteConcernOptions::kWriteConcernField)) {
                            // TODO: Disabled until after SERVER-44539, to avoid log spam.
                            // LOGV2(21959, "Missing writeConcern on {command}", "Missing "
                            // "writeConcern on command", "command"_attr = command->getName());
                        }
                    }
                }
            }
            extractedWriteConcern.emplace(
                uassertStatusOK(extractWriteConcern(opCtx, request.body, isInternalClient)));
            if (sessionOptions.getAutocommit()) {
                validateWriteConcernForTransaction(*extractedWriteConcern,
                                                   invocation->definition()->getName());
            }

            // Ensure that the WC being set on the opCtx has provenance.
            invariant(extractedWriteConcern->getProvenance().hasSource(),
                      str::stream() << "unexpected unset provenance on writeConcern: "
                                    << extractedWriteConcern->toBSON());

            opCtx->setWriteConcern(*extractedWriteConcern);
        }

        auto waitForWriteConcern = [&](auto&& bb) {
            bool reallyWait = true;
            failCommand.executeIf(
                [&](const BSONObj& data) {
                    bb.append(data["writeConcernError"]);
                    reallyWait = false;
                    if (data.hasField(kErrorLabelsFieldName) &&
                        data[kErrorLabelsFieldName].type() == Array) {
                        // Propagate error labels specified in the failCommand failpoint to the
                        // OperationContext decoration to override getErrorLabels() behaviors.
                        invariant(!errorLabelsOverride(opCtx));
                        errorLabelsOverride(opCtx).emplace(
                            data.getObjectField(kErrorLabelsFieldName).getOwned());
                    }
                },
                [&](const BSONObj& data) {
                    return CommandHelpers::shouldActivateFailCommandFailPoint(
                               data, invocation, opCtx->getClient()) &&
                        data.hasField("writeConcernError");
                });
            if (reallyWait) {
                CurOp::get(opCtx)->debug().writeConcern.emplace(opCtx->getWriteConcern());
                behaviors.waitForWriteConcern(opCtx, invocation, lastOpBeforeRun, bb);
            }
        };

        try {
            if (auto scoped = failWithErrorCodeInRunCommand.scoped();
                MONGO_unlikely(scoped.isActive())) {
                const auto errorCode = scoped.getData()["errorCode"].numberInt();
                LOGV2(21960,
                      "failWithErrorCodeInRunCommand enabled - failing command with error "
                      "code: {errorCode}",
                      "failWithErrorCodeInRunCommand enabled, failing command",
                      "errorCode"_attr = errorCode);
                BSONObjBuilder errorBuilder;
                errorBuilder.append("ok", 0.0);
                errorBuilder.append("code", errorCode);
                errorBuilder.append("errmsg", "failWithErrorCodeInRunCommand enabled.");
                replyBuilder->setCommandReply(errorBuilder.obj());
            } else if (shouldCheckOutSession) {
                invokeWithSessionCheckedOut(
                    opCtx, request, invocation, sessionOptions, replyBuilder);
            } else {
                CommandHelpers::runCommandInvocation(opCtx, request, invocation, replyBuilder);
            }
        } catch (const DBException& ex) {
            // Do no-op write before returning NoSuchTransaction if command has writeConcern.
            if (ex.toStatus().code() == ErrorCodes::NoSuchTransaction &&
                !opCtx->getWriteConcern().usedDefault) {
                TransactionParticipant::performNoopWrite(opCtx, "NoSuchTransaction error");
            }
            waitForWriteConcern(*extraFieldsBuilder);
            throw;
        }

        waitForWriteConcern(replyBuilder->getBodyBuilder());

        // With the exception of getMores inheriting the WriteConcern from the originating command,
        // nothing in run() should change the writeConcern.
        if (command->getLogicalOp() == LogicalOp::opGetMore) {
            dassert(!extractedWriteConcern,
                    "opGetMore contained unexpected extracted write concern");
        } else {
            dassert(extractedWriteConcern, "no extracted write concern");
            dassert(opCtx->getWriteConcern() == extractedWriteConcern,
                    "opCtx wc: {} extracted wc: {}"_format(
                        opCtx->getWriteConcern().toBSON().jsonString(),
                        extractedWriteConcern->toBSON().jsonString()));
        }
    } else {
        behaviors.uassertCommandDoesNotSpecifyWriteConcern(request.body);
        if (shouldCheckOutSession) {
            invokeWithSessionCheckedOut(opCtx, request, invocation, sessionOptions, replyBuilder);
        } else {
            CommandHelpers::runCommandInvocation(opCtx, request, invocation, replyBuilder);
        }
    }

    // This fail point blocks all commands which are running on the specified namespace, or which
    // are present in the given list of commands.If no namespace or command list are provided,then
    // the failpoint will block all commands.
    waitAfterCommandFinishesExecution.execute([&](const BSONObj& data) {
        auto ns = data["ns"].valueStringDataSafe();
        auto commands =
            data.hasField("commands") ? data["commands"].Array() : std::vector<BSONElement>();

        // If 'ns' or 'commands' is not set, block for all the namespaces or commands respectively.
        if ((ns.empty() || invocation->ns().ns() == ns) &&
            (commands.empty() ||
             std::any_of(commands.begin(), commands.end(), [&request](auto& element) {
                 return element.valueStringDataSafe() == request.getCommandName();
             }))) {
            CurOpFailpointHelpers::waitWhileFailPointEnabled(
                &waitAfterCommandFinishesExecution, opCtx, "waitAfterCommandFinishesExecution");
        }
    });

    behaviors.waitForLinearizableReadConcern(opCtx);

    // Wait for data to satisfy the read concern level, if necessary.
    behaviors.waitForSpeculativeMajorityReadConcern(opCtx);

    const bool ok = [&] {
        auto body = replyBuilder->getBodyBuilder();
        return CommandHelpers::extractOrAppendOk(body);
    }();
    behaviors.attachCurOpErrInfo(opCtx, replyBuilder->getBodyBuilder().asTempObj());

    {
        boost::optional<ErrorCodes::Error> code;
        boost::optional<ErrorCodes::Error> wcCode;
        auto body = replyBuilder->getBodyBuilder();
        auto response = body.asTempObj();
        auto codeField = response["code"];
        if (!ok && codeField.isNumber()) {
            code = ErrorCodes::Error(codeField.numberInt());
        }
        if (response.hasField("writeConcernError")) {
            wcCode = ErrorCodes::Error(response["writeConcernError"]["code"].numberInt());
        }
        appendErrorLabelsAndTopologyVersion(
            opCtx, &body, sessionOptions, command->getName(), code, wcCode, isInternalClient);
    }

    auto commandBodyBob = replyBuilder->getBodyBuilder();
    behaviors.appendReplyMetadata(opCtx, request, &commandBodyBob);
    appendClusterAndOperationTime(opCtx, &commandBodyBob, &commandBodyBob, startOperationTime);
    return ok;
}

/**
 * Executes a command after stripping metadata, performing authorization checks,
 * handling audit impersonation, and (potentially) setting maintenance mode. This method
 * also checks that the command is permissible to run on the node given its current
 * replication state. All the logic here is independent of any particular command; any
 * functionality relevant to a specific command should be confined to its run() method.
 */
void execCommandDatabase(OperationContext* opCtx,
                         Command* command,
                         const OpMsgRequest& request,
                         rpc::ReplyBuilderInterface* replyBuilder,
                         const ServiceEntryPointCommon::Hooks& behaviors) {
    CommandHelpers::uassertShouldAttemptParse(opCtx, command, request);
    BSONObjBuilder extraFieldsBuilder;
    auto startOperationTime = getClientOperationTime(opCtx);

    std::shared_ptr<CommandInvocation> invocation = command->parse(opCtx, request);
    CommandInvocation::set(opCtx, invocation);

    OperationSessionInfoFromClient sessionOptions;

    const auto isInternalClient = opCtx->getClient()->session() &&
        (opCtx->getClient()->session()->getTags() & transport::Session::kInternalClient);

    try {
        {
            stdx::lock_guard<Client> lk(*opCtx->getClient());
            CurOp::get(opCtx)->setCommand_inlock(command);
        }

        sleepMillisAfterCommandExecutionBegins.execute([&](const BSONObj& data) {
            auto numMillis = data["millis"].numberInt();
            auto commands = data["commands"].Obj().getFieldNames<std::set<std::string>>();
            // Only sleep for one of the specified commands.
            if (commands.find(command->getName()) != commands.end()) {
                mongo::sleepmillis(numMillis);
            }
        });

        rpc::readRequestMetadata(opCtx, request.body, command->requiresAuth());
        rpc::TrackingMetadata::get(opCtx).initWithOperName(command->getName());

        auto const replCoord = repl::ReplicationCoordinator::get(opCtx);
        sessionOptions = initializeOperationSessionInfo(
            opCtx,
            request.body,
            command->requiresAuth(),
            command->attachLogicalSessionsToOpCtx(),
            replCoord->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet,
            opCtx->getServiceContext()->getStorageEngine()->supportsDocLocking());

        CommandHelpers::evaluateFailCommandFailPoint(opCtx, invocation.get());

        const auto dbname = request.getDatabase().toString();
        uassert(
            ErrorCodes::InvalidNamespace,
            str::stream() << "Invalid database name: '" << dbname << "'",
            NamespaceString::validDBName(dbname, NamespaceString::DollarInDbNameBehavior::Allow));


        const auto allowTransactionsOnConfigDatabase =
            (serverGlobalParams.clusterRole == ClusterRole::ConfigServer ||
             serverGlobalParams.clusterRole == ClusterRole::ShardServer);

        validateSessionOptions(sessionOptions,
                               command->getName(),
                               invocation->ns(),
                               allowTransactionsOnConfigDatabase);

        std::unique_ptr<MaintenanceModeSetter> mmSetter;

        BSONElement cmdOptionMaxTimeMSField;
        BSONElement maxTimeMSOpOnlyField;
        BSONElement allowImplicitCollectionCreationField;
        BSONElement helpField;

        StringMap<int> topLevelFields;
        for (auto&& element : request.body) {
            StringData fieldName = element.fieldNameStringData();
            if (fieldName == QueryRequest::cmdOptionMaxTimeMS) {
                cmdOptionMaxTimeMSField = element;
            } else if (fieldName == QueryRequest::kMaxTimeMSOpOnlyField) {
                uassert(ErrorCodes::InvalidOptions,
                        "Can not specify maxTimeMSOpOnly for non internal clients",
                        isInternalClient);
                maxTimeMSOpOnlyField = element;
            } else if (fieldName == "allowImplicitCollectionCreation") {
                allowImplicitCollectionCreationField = element;
            } else if (fieldName == CommandHelpers::kHelpFieldName) {
                helpField = element;
            } else if (fieldName == "comment") {
                opCtx->setComment(element.wrap());
            } else if (fieldName == QueryRequest::queryOptionMaxTimeMS) {
                uasserted(ErrorCodes::InvalidOptions,
                          "no such command option $maxTimeMs; use maxTimeMS instead");
            }

            uassert(ErrorCodes::FailedToParse,
                    str::stream() << "Parsed command object contains duplicate top level key: "
                                  << fieldName,
                    topLevelFields[fieldName]++ == 0);
        }

        if (CommandHelpers::isHelpRequest(helpField)) {
            CurOp::get(opCtx)->ensureStarted();
            // We disable last-error for help requests due to SERVER-11492, because config
            // servers use help requests to determine which commands are database writes, and so
            // must be forwarded to all config servers.
            LastError::get(opCtx->getClient()).disable();
            Command::generateHelpResponse(opCtx, replyBuilder, *command);
            return;
        }

        ImpersonationSessionGuard guard(opCtx);
        invocation->checkAuthorization(opCtx, request);

        const bool iAmPrimary = replCoord->canAcceptWritesForDatabase_UNSAFE(opCtx, dbname);

        if (!opCtx->getClient()->isInDirectClient() &&
            !MONGO_unlikely(skipCheckingForNotMasterInCommandDispatch.shouldFail())) {
            const bool inMultiDocumentTransaction = (sessionOptions.getAutocommit() == false);
            auto allowed = command->secondaryAllowed(opCtx->getServiceContext());
            bool alwaysAllowed = allowed == Command::AllowedOnSecondary::kAlways;
            bool couldHaveOptedIn =
                allowed == Command::AllowedOnSecondary::kOptIn && !inMultiDocumentTransaction;
            bool optedIn =
                couldHaveOptedIn && ReadPreferenceSetting::get(opCtx).canRunOnSecondary();
            bool canRunHere = commandCanRunHere(opCtx, dbname, command, inMultiDocumentTransaction);
            if (!canRunHere && couldHaveOptedIn) {
                uasserted(ErrorCodes::NotMasterNoSlaveOk, "not master and slaveOk=false");
            }

            if (MONGO_unlikely(respondWithNotPrimaryInCommandDispatch.shouldFail())) {
                uassert(ErrorCodes::NotMaster, "not primary", canRunHere);
            } else {
                uassert(ErrorCodes::NotMaster, "not master", canRunHere);
            }

            if (!command->maintenanceOk() &&
                replCoord->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet &&
                !replCoord->canAcceptWritesForDatabase_UNSAFE(opCtx, dbname) &&
                !replCoord->getMemberState().secondary()) {

                uassert(ErrorCodes::NotMasterOrSecondary,
                        "node is recovering",
                        !replCoord->getMemberState().recovering());
                uassert(ErrorCodes::NotMasterOrSecondary,
                        "node is not in primary or recovering state",
                        replCoord->getMemberState().primary());
                // Check ticket SERVER-21432, slaveOk commands are allowed in drain mode
                uassert(ErrorCodes::NotMasterOrSecondary,
                        "node is in drain mode",
                        optedIn || alwaysAllowed);
            }
        }

        if (command->adminOnly()) {
            LOGV2_DEBUG(21961,
                        2,
                        "Admin only command: {command}",
                        "Admin only command",
                        "command"_attr = request.getCommandName());
        }

        if (command->maintenanceMode()) {
            mmSetter.reset(new MaintenanceModeSetter(opCtx));
        }

        if (command->shouldAffectCommandCounter()) {
            OpCounters* opCounters = &globalOpCounters;
            opCounters->gotCommand();
        }

        // Parse the 'maxTimeMS' command option, and use it to set a deadline for the operation
        // on the OperationContext. The 'maxTimeMS' option unfortunately has a different meaning
        // for a getMore command, where it is used to communicate the maximum time to wait for
        // new inserts on tailable cursors, not as a deadline for the operation.
        // TODO SERVER-34277 Remove the special handling for maxTimeMS for getMores. This will
        // require introducing a new 'max await time' parameter for getMore, and eventually
        // banning maxTimeMS altogether on a getMore command.
        int maxTimeMS = uassertStatusOK(QueryRequest::parseMaxTimeMS(cmdOptionMaxTimeMSField));
        int maxTimeMSOpOnly = uassertStatusOK(QueryRequest::parseMaxTimeMS(maxTimeMSOpOnlyField));

        if ((maxTimeMS > 0 || maxTimeMSOpOnly > 0) &&
            command->getLogicalOp() != LogicalOp::opGetMore) {
            uassert(40119,
                    "Illegal attempt to set operation deadline within DBDirectClient",
                    !opCtx->getClient()->isInDirectClient());
            if (maxTimeMSOpOnly > 0 && (maxTimeMS == 0 || maxTimeMSOpOnly < maxTimeMS)) {
                opCtx->storeMaxTimeMS(Milliseconds{maxTimeMS});
                opCtx->setDeadlineAfterNowBy(Milliseconds{maxTimeMSOpOnly},
                                             ErrorCodes::MaxTimeMSExpired);
            } else if (maxTimeMS > 0) {
                opCtx->setDeadlineAfterNowBy(Milliseconds{maxTimeMS}, ErrorCodes::MaxTimeMSExpired);
            }
        }

        auto& readConcernArgs = repl::ReadConcernArgs::get(opCtx);

        // If the parent operation runs in a transaction, we don't override the read concern.
        auto skipReadConcern =
            opCtx->getClient()->isInDirectClient() && opCtx->inMultiDocumentTransaction();
        bool startTransaction = static_cast<bool>(sessionOptions.getStartTransaction());
        if (!skipReadConcern) {
            auto newReadConcernArgs = uassertStatusOK(_extractReadConcern(
                opCtx, invocation.get(), request.body, startTransaction, isInternalClient));

            // Ensure that the RC being set on the opCtx has provenance.
            invariant(newReadConcernArgs.getProvenance().hasSource(),
                      str::stream() << "unexpected unset provenance on readConcern: "
                                    << newReadConcernArgs.toBSONInner());

            uassert(ErrorCodes::InvalidOptions,
                    "Only the first command in a transaction may specify a readConcern",
                    startTransaction || !opCtx->inMultiDocumentTransaction() ||
                        newReadConcernArgs.isEmpty());

            {
                // We must obtain the client lock to set the ReadConcernArgs on the operation
                // context as it may be concurrently read by CurrentOp.
                stdx::lock_guard<Client> lk(*opCtx->getClient());
                readConcernArgs = std::move(newReadConcernArgs);
            }
        }

        uassert(ErrorCodes::InvalidOptions,
                "read concern level snapshot is only valid in a transaction",
                opCtx->inMultiDocumentTransaction() ||
                    readConcernArgs.getLevel() != repl::ReadConcernLevel::kSnapshotReadConcern);

        if (startTransaction) {
            opCtx->lockState()->setSharedLocksShouldTwoPhaseLock(true);
            opCtx->lockState()->setShouldConflictWithSecondaryBatchApplication(false);
        }

        // Remember whether or not this operation is starting a transaction, in case something
        // later in the execution needs to adjust its behavior based on this.
        opCtx->setIsStartingMultiDocumentTransaction(startTransaction);

        auto& oss = OperationShardingState::get(opCtx);

        if (!opCtx->getClient()->isInDirectClient() &&
            readConcernArgs.getLevel() != repl::ReadConcernLevel::kAvailableReadConcern &&
            (iAmPrimary ||
             (readConcernArgs.hasLevel() || readConcernArgs.getArgsAfterClusterTime()))) {
            oss.initializeClientRoutingVersionsFromCommand(invocation->ns(), request.body);

            auto const shardingState = ShardingState::get(opCtx);
            if (oss.hasShardVersion() || oss.hasDbVersion()) {
                uassertStatusOK(shardingState->canAcceptShardedCommands());
            }

            behaviors.advanceConfigOpTimeFromRequestMetadata(opCtx);
        }

        oss.setAllowImplicitCollectionCreation(allowImplicitCollectionCreationField);
        auto scoped = behaviors.scopedOperationCompletionShardingActions(opCtx);

        // This may trigger the maxTimeAlwaysTimeOut failpoint.
        auto status = opCtx->checkForInterruptNoAssert();

        // We still proceed if the primary stepped down, but accept other kinds of
        // interruptions. We defer to individual commands to allow themselves to be
        // interruptible by stepdowns, since commands like 'voteRequest' should conversely
        // continue executing.
        if (status != ErrorCodes::PrimarySteppedDown &&
            status != ErrorCodes::InterruptedDueToReplStateChange) {
            uassertStatusOK(status);
        }


        CurOp::get(opCtx)->ensureStarted();

        command->incrementCommandsExecuted();

        if (shouldLog(logv2::LogComponent::kTracking, logv2::LogSeverity::Debug(1)) &&
            rpc::TrackingMetadata::get(opCtx).getParentOperId()) {
            LOGV2_DEBUG_OPTIONS(4615605,
                                1,
                                {logv2::LogComponent::kTracking},
                                "Command metadata: {trackingMetadata}",
                                "Command metadata",
                                "trackingMetadata"_attr = rpc::TrackingMetadata::get(opCtx));
            rpc::TrackingMetadata::get(opCtx).setIsLogged(true);
        }

        behaviors.waitForReadConcern(opCtx, invocation.get(), request);
        behaviors.setPrepareConflictBehaviorForReadConcern(opCtx, invocation.get());

        try {
            if (!runCommandImpl(opCtx,
                                invocation.get(),
                                request,
                                replyBuilder,
                                startOperationTime,
                                behaviors,
                                &extraFieldsBuilder,
                                sessionOptions)) {
                command->incrementCommandsFailed();
            }
        } catch (const DBException& e) {
            command->incrementCommandsFailed();
            if (e.code() == ErrorCodes::Unauthorized) {
                CommandHelpers::auditLogAuthEvent(opCtx, invocation.get(), request, e.code());
            }
            throw;
        }
    } catch (const DBException& e) {
        if (e.code() == ErrorCodes::SnapshotTooOld) {
            // SnapshotTooOld errors should never be thrown unless we are using a storage engine
            // that supports snapshot read concern.
            auto engine = opCtx->getServiceContext()->getStorageEngine();
            invariant(engine && engine->supportsReadConcernSnapshot());

            // SnapshotTooOld errors indicate that PIT ops are failing to find an available
            // snapshot at their specified atClusterTime. Therefore, we'll try to increase the
            // snapshot history window that the storage engine maintains in order to increase
            // the likelihood of successful future PIT atClusterTime requests.
            SnapshotWindowUtil::incrementSnapshotTooOldErrorCount();
            SnapshotWindowUtil::increaseTargetSnapshotWindowSize(opCtx);
        } else {
            behaviors.handleException(e, opCtx);
        }

        // Append the error labels for transient transaction errors.
        auto response = extraFieldsBuilder.asTempObj();
        boost::optional<ErrorCodes::Error> wcCode;
        if (response.hasField("writeConcernError")) {
            wcCode = ErrorCodes::Error(response["writeConcernError"]["code"].numberInt());
        }
        appendErrorLabelsAndTopologyVersion(opCtx,
                                            &extraFieldsBuilder,
                                            sessionOptions,
                                            command->getName(),
                                            e.code(),
                                            wcCode,
                                            isInternalClient);

        BSONObjBuilder metadataBob;
        behaviors.appendReplyMetadata(opCtx, request, &metadataBob);

        // The read concern may not have yet been placed on the operation context, so attempt to
        // parse it here, so if it is valid it can be used to compute the proper operationTime.
        auto& readConcernArgs = repl::ReadConcernArgs::get(opCtx);
        if (readConcernArgs.isEmpty()) {
            auto readConcernArgsStatus = _extractReadConcern(opCtx,
                                                             invocation.get(),
                                                             request.body,
                                                             false /*startTransaction*/,
                                                             isInternalClient);
            if (readConcernArgsStatus.isOK()) {
                // We must obtain the client lock to set the ReadConcernArgs on the operation
                // context as it may be concurrently read by CurrentOp.
                stdx::lock_guard<Client> lk(*opCtx->getClient());
                readConcernArgs = readConcernArgsStatus.getValue();
            }
        }
        appendClusterAndOperationTime(opCtx, &extraFieldsBuilder, &metadataBob, startOperationTime);

        LOGV2_DEBUG(21962,
                    1,
                    "Assertion while executing command '{command}' on database '{db}' with "
                    "arguments '{commandArgs}': {error}",
                    "Assertion while executing command",
                    "command"_attr = request.getCommandName(),
                    "db"_attr = request.getDatabase(),
                    "commandArgs"_attr = redact(
                        ServiceEntryPointCommon::getRedactedCopyForLogging(command, request.body)),
                    "error"_attr = redact(e.toString()));

        generateErrorResponse(opCtx, replyBuilder, e, metadataBob.obj(), extraFieldsBuilder.obj());

        if (ErrorCodes::isA<ErrorCategory::CloseConnectionError>(e.code())) {
            // Rethrow the exception to the top to signal that the client connection should be
            // closed.
            throw;
        }
    }
}

/**
 * Fills out CurOp / OpDebug with basic command info.
 */
void curOpCommandSetup(OperationContext* opCtx, const OpMsgRequest& request) {
    auto curop = CurOp::get(opCtx);
    curop->debug().iscommand = true;

    // We construct a legacy $cmd namespace so we can fill in curOp using
    // the existing logic that existed for OP_QUERY commands
    NamespaceString nss(request.getDatabase(), "$cmd");

    stdx::lock_guard<Client> lk(*opCtx->getClient());
    curop->setOpDescription_inlock(request.body);
    curop->markCommand_inlock();
    curop->setNS_inlock(nss.ns());
}

DbResponse receivedCommands(OperationContext* opCtx,
                            const Message& message,
                            const ServiceEntryPointCommon::Hooks& behaviors) {
    auto replyBuilder = rpc::makeReplyBuilder(rpc::protocolForMessage(message));
    OpMsgRequest request;
    Command* c = nullptr;
    [&] {
        try {  // Parse.
            request = rpc::opMsgRequestFromAnyProtocol(message);
        } catch (const DBException& ex) {
            // If this error needs to fail the connection, propagate it out.
            if (ErrorCodes::isConnectionFatalMessageParseError(ex.code()))
                throw;

            BSONObjBuilder metadataBob;
            behaviors.appendReplyMetadataOnError(opCtx, &metadataBob);

            BSONObjBuilder extraFieldsBuilder;
            appendClusterAndOperationTime(
                opCtx, &extraFieldsBuilder, &metadataBob, LogicalTime::kUninitialized);

            // Otherwise, reply with the parse error. This is useful for cases where parsing fails
            // due to user-supplied input, such as the document too deep error. Since we failed
            // during parsing, we can't log anything about the command.
            LOGV2_DEBUG(21963,
                        1,
                        "Assertion while parsing command: {error}",
                        "Assertion while parsing command",
                        "error"_attr = ex.toString());

            generateErrorResponse(
                opCtx, replyBuilder.get(), ex, metadataBob.obj(), extraFieldsBuilder.obj());

            return;  // From lambda. Don't try executing if parsing failed.
        }

        try {  // Execute.
            curOpCommandSetup(opCtx, request);

            // In the absence of a Command object, no redaction is possible. Therefore
            // to avoid displaying potentially sensitive information in the logs,
            // we restrict the log message to the name of the unrecognized command.
            // However, the complete command object will still be echoed to the client.
            if (!(c = CommandHelpers::findCommand(request.getCommandName()))) {
                globalCommandRegistry()->incrementUnknownCommands();
                std::string msg = str::stream()
                    << "no such command: '" << request.getCommandName() << "'";
                LOGV2_DEBUG(21964,
                            2,
                            "No such command: {command}",
                            "Command not found in registry",
                            "command"_attr = request.getCommandName());
                uasserted(ErrorCodes::CommandNotFound, str::stream() << msg);
            }

            LOGV2_DEBUG(21965,
                        2,
                        "Run command {db}.$cmd {commandArgs}",
                        "About to run the command",
                        "db"_attr = request.getDatabase(),
                        "commandArgs"_attr = redact(
                            ServiceEntryPointCommon::getRedactedCopyForLogging(c, request.body)));

            {
                // Try to set this as early as possible, as soon as we have figured out the
                // command.
                stdx::lock_guard<Client> lk(*opCtx->getClient());
                CurOp::get(opCtx)->setLogicalOp_inlock(c->getLogicalOp());
            }

            opCtx->setExhaust(OpMsg::isFlagSet(message, OpMsg::kExhaustSupported));

            const auto session = opCtx->getClient()->session();
            if (session) {
                if (!opCtx->isExhaust() || c->getName() != "isMaster"_sd) {
                    InExhaustIsMaster::get(session.get())->setInExhaustIsMaster(false);
                }
            }

            execCommandDatabase(opCtx, c, request, replyBuilder.get(), behaviors);
        } catch (const DBException& ex) {
            BSONObjBuilder metadataBob;
            behaviors.appendReplyMetadataOnError(opCtx, &metadataBob);

            BSONObjBuilder extraFieldsBuilder;
            appendClusterAndOperationTime(
                opCtx, &extraFieldsBuilder, &metadataBob, LogicalTime::kUninitialized);

            LOGV2_DEBUG(21966,
                        1,
                        "Assertion while executing command '{command}' on database '{db}': {error}",
                        "Assertion while executing command",
                        "command"_attr = request.getCommandName(),
                        "db"_attr = request.getDatabase(),
                        "error"_attr = ex.toString());

            generateErrorResponse(
                opCtx, replyBuilder.get(), ex, metadataBob.obj(), extraFieldsBuilder.obj());

            if (ErrorCodes::isA<ErrorCategory::CloseConnectionError>(ex.code())) {
                // Rethrow the exception to the top to signal that the client connection should be
                // closed.
                throw;
            }
        }
    }();

    if (OpMsg::isFlagSet(message, OpMsg::kMoreToCome)) {
        // Close the connection to get client to go through server selection again.
        if (LastError::get(opCtx->getClient()).hadNotMasterError()) {
            if (c && c->getReadWriteType() == Command::ReadWriteType::kWrite)
                notMasterUnackWrites.increment();
            uasserted(ErrorCodes::NotMaster,
                      str::stream()
                          << "Not-master error while processing '" << request.getCommandName()
                          << "' operation  on '" << request.getDatabase() << "' database via "
                          << "fire-and-forget command execution.");
        }
        return {};  // Don't reply.
    }

    DbResponse dbResponse;

    if (OpMsg::isFlagSet(message, OpMsg::kExhaustSupported)) {
        auto responseObj = replyBuilder->getBodyBuilder().asTempObj();

        if (responseObj.getField("ok").trueValue()) {
            dbResponse.shouldRunAgainForExhaust = replyBuilder->shouldRunAgainForExhaust();
            dbResponse.nextInvocation = replyBuilder->getNextInvocation();
        }
    }

    dbResponse.response = replyBuilder->done();
    CurOp::get(opCtx)->debug().responseLength = dbResponse.response.header().dataLen();

    return dbResponse;
}

DbResponse receivedQuery(OperationContext* opCtx,
                         const NamespaceString& nss,
                         Client& c,
                         const Message& m,
                         const ServiceEntryPointCommon::Hooks& behaviors) {
    invariant(!nss.isCommand());
    globalOpCounters.gotQuery();
    ServerReadConcernMetrics::get(opCtx)->recordReadConcern(repl::ReadConcernArgs::get(opCtx));

    DbMessage d(m);
    QueryMessage q(d);

    CurOp& op = *CurOp::get(opCtx);
    DbResponse dbResponse;

    try {
        Client* client = opCtx->getClient();
        Status status = AuthorizationSession::get(client)->checkAuthForFind(nss, false);
        audit::logQueryAuthzCheck(client, nss, q.query, status.code());
        uassertStatusOK(status);

        dbResponse.shouldRunAgainForExhaust = runQuery(opCtx, q, nss, dbResponse.response);
    } catch (const AssertionException& e) {
        behaviors.handleException(e, opCtx);

        dbResponse.response.reset();
        generateLegacyQueryErrorResponse(e, q, &op, &dbResponse.response);
    }

    op.debug().responseLength = dbResponse.response.header().dataLen();
    return dbResponse;
}

void receivedKillCursors(OperationContext* opCtx, const Message& m) {
    LastError::get(opCtx->getClient()).disable();
    DbMessage dbmessage(m);
    int n = dbmessage.pullInt();

    static constexpr int kSoftKillLimit = 2000;
    static constexpr int kHardKillLimit = 29999;

    if (n > kHardKillLimit) {
        LOGV2_ERROR(4615607,
                    "Received killCursors, n={numCursors}",
                    "Received killCursors, exceeded kHardKillLimit",
                    "numCursors"_attr = n,
                    "kHardKillLimit"_attr = kHardKillLimit);
        uasserted(51250, "Must kill fewer than {} cursors"_format(kHardKillLimit));
    }

    if (n > kSoftKillLimit) {
        LOGV2_WARNING(4615606,
                      "Received killCursors, n={numCursors}",
                      "Received killCursors, exceeded kSoftKillLimit",
                      "numCursors"_attr = n,
                      "kSoftKillLimit"_attr = kSoftKillLimit);
    }

    uassert(31289, str::stream() << "must kill at least 1 cursor, n=" << n, n >= 1);
    massert(13658,
            str::stream() << "bad kill cursors size: " << m.dataSize(),
            m.dataSize() == 8 + (8 * n));

    const char* cursorArray = dbmessage.getArray(n);
    int found = runOpKillCursors(opCtx, static_cast<size_t>(n), cursorArray);

    if (shouldLog(logv2::LogSeverity::Debug(1)) || found != n) {
        LOGV2_DEBUG(21967,
                    found == n ? 1 : 0,
                    "killCursors: found {found} of {numCursors}",
                    "killCursors found fewer cursors to kill than requested",
                    "found"_attr = found,
                    "numCursors"_attr = n);
    }
}

void receivedInsert(OperationContext* opCtx, const NamespaceString& nsString, const Message& m) {
    auto insertOp = InsertOp::parseLegacy(m);
    invariant(insertOp.getNamespace() == nsString);

    for (const auto& obj : insertOp.getDocuments()) {
        Status status =
            AuthorizationSession::get(opCtx->getClient())->checkAuthForInsert(opCtx, nsString);
        audit::logInsertAuthzCheck(opCtx->getClient(), nsString, obj, status.code());
        uassertStatusOK(status);
    }
    performInserts(opCtx, insertOp);
}

void receivedUpdate(OperationContext* opCtx, const NamespaceString& nsString, const Message& m) {
    auto updateOp = UpdateOp::parseLegacy(m);
    auto& singleUpdate = updateOp.getUpdates()[0];
    invariant(updateOp.getNamespace() == nsString);

    Status status = AuthorizationSession::get(opCtx->getClient())
                        ->checkAuthForUpdate(opCtx,
                                             nsString,
                                             singleUpdate.getQ(),
                                             singleUpdate.getU(),
                                             singleUpdate.getUpsert());
    audit::logUpdateAuthzCheck(opCtx->getClient(),
                               nsString,
                               singleUpdate.getQ(),
                               singleUpdate.getU(),
                               singleUpdate.getUpsert(),
                               singleUpdate.getMulti(),
                               status.code());
    uassertStatusOK(status);

    performUpdates(opCtx, updateOp);
}

void receivedDelete(OperationContext* opCtx, const NamespaceString& nsString, const Message& m) {
    auto deleteOp = DeleteOp::parseLegacy(m);
    auto& singleDelete = deleteOp.getDeletes()[0];
    invariant(deleteOp.getNamespace() == nsString);

    Status status = AuthorizationSession::get(opCtx->getClient())
                        ->checkAuthForDelete(opCtx, nsString, singleDelete.getQ());
    audit::logDeleteAuthzCheck(opCtx->getClient(), nsString, singleDelete.getQ(), status.code());
    uassertStatusOK(status);

    performDeletes(opCtx, deleteOp);
}

DbResponse receivedGetMore(OperationContext* opCtx,
                           const Message& m,
                           CurOp& curop,
                           bool* shouldLogOpDebug) {
    globalOpCounters.gotGetMore();
    DbMessage d(m);

    const char* ns = d.getns();
    int ntoreturn = d.pullInt();
    uassert(
        34419, str::stream() << "Invalid ntoreturn for OP_GET_MORE: " << ntoreturn, ntoreturn >= 0);
    long long cursorid = d.pullInt64();

    curop.debug().ntoreturn = ntoreturn;
    curop.debug().cursorid = cursorid;

    {
        stdx::lock_guard<Client> lk(*opCtx->getClient());
        CurOp::get(opCtx)->setNS_inlock(ns);
    }

    bool exhaust = false;
    bool isCursorAuthorized = false;

    DbResponse dbresponse;
    try {
        const NamespaceString nsString(ns);
        uassert(ErrorCodes::InvalidNamespace,
                str::stream() << "Invalid ns [" << ns << "]",
                nsString.isValid());

        Status status = AuthorizationSession::get(opCtx->getClient())
                            ->checkAuthForGetMore(nsString, cursorid, false);
        audit::logGetMoreAuthzCheck(opCtx->getClient(), nsString, cursorid, status.code());
        uassertStatusOK(status);

        while (MONGO_unlikely(rsStopGetMore.shouldFail())) {
            sleepmillis(0);
        }

        dbresponse.response =
            getMore(opCtx, ns, ntoreturn, cursorid, &exhaust, &isCursorAuthorized);
    } catch (AssertionException& e) {
        if (isCursorAuthorized) {
            // Make sure that killCursorGlobal does not throw an exception if it is interrupted.
            UninterruptibleLockGuard noInterrupt(opCtx->lockState());

            // If an error was thrown prior to auth checks, then the cursor should remain alive
            // in order to prevent an unauthorized user from resulting in the death of a cursor.
            // In other error cases, the cursor is dead and should be cleaned up.
            //
            // If killing the cursor fails, ignore the error and don't try again. The cursor
            // should be reaped by the client cursor timeout thread.
            CursorManager::get(opCtx)
                ->killCursor(opCtx, cursorid, false /* shouldAudit */)
                .ignore();
        }

        BSONObjBuilder err;
        err.append("$err", e.reason());
        err.append("code", e.code());
        BSONObj errObj = err.obj();

        curop.debug().errInfo = e.toStatus();

        dbresponse = replyToQuery(errObj, ResultFlag_ErrSet);
        curop.debug().responseLength = dbresponse.response.header().dataLen();
        curop.debug().nreturned = 1;
        *shouldLogOpDebug = true;
        return dbresponse;
    }

    curop.debug().responseLength = dbresponse.response.header().dataLen();
    auto queryResult = QueryResult::ConstView(dbresponse.response.buf());
    curop.debug().nreturned = queryResult.getNReturned();

    if (exhaust) {
        curop.debug().exhaust = true;
        dbresponse.shouldRunAgainForExhaust = true;
    }

    return dbresponse;
}

}  // namespace

BSONObj ServiceEntryPointCommon::getRedactedCopyForLogging(const Command* command,
                                                           const BSONObj& cmdObj) {
    mutablebson::Document cmdToLog(cmdObj, mutablebson::Document::kInPlaceDisabled);
    command->snipForLogging(&cmdToLog);
    BSONObjBuilder bob;
    cmdToLog.writeTo(&bob);
    return bob.obj();
}

DbResponse ServiceEntryPointCommon::handleRequest(OperationContext* opCtx,
                                                  const Message& m,
                                                  const Hooks& behaviors) {
    // before we lock...
    NetworkOp op = m.operation();
    bool isCommand = false;

    DbMessage dbmsg(m);

    Client& c = *opCtx->getClient();

    if (c.isInDirectClient()) {
        if (!opCtx->getLogicalSessionId() || !opCtx->getTxnNumber()) {
            invariant(!opCtx->inMultiDocumentTransaction() &&
                      !opCtx->lockState()->inAWriteUnitOfWork());
        }
    } else {
        LastError::get(c).startRequest();
        AuthorizationSession::get(c)->startRequest(opCtx);

        // We should not be holding any locks at this point
        invariant(!opCtx->lockState()->isLocked());
    }

    const char* ns = dbmsg.messageShouldHaveNs() ? dbmsg.getns() : nullptr;
    const NamespaceString nsString = ns ? NamespaceString(ns) : NamespaceString();

    if (op == dbQuery) {
        if (nsString.isCommand()) {
            isCommand = true;
        }
    } else if (op == dbMsg) {
        isCommand = true;
    }

    CurOp& currentOp = *CurOp::get(opCtx);
    {
        stdx::lock_guard<Client> lk(*opCtx->getClient());
        // Commands handling code will reset this if the operation is a command
        // which is logically a basic CRUD operation like query, insert, etc.
        currentOp.setNetworkOp_inlock(op);
        currentOp.setLogicalOp_inlock(networkOpToLogicalOp(op));
    }

    OpDebug& debug = currentOp.debug();

    boost::optional<long long> slowMsOverride;
    bool forceLog = false;

    DbResponse dbresponse;
    if (op == dbMsg || (op == dbQuery && isCommand)) {
        dbresponse = receivedCommands(opCtx, m, behaviors);
        // IsMaster should take kMaxAwaitTimeMs at most, log if it takes twice that.
        if (auto command = currentOp.getCommand();
            command && (command->getName() == "ismaster" || command->getName() == "isMaster")) {
            slowMsOverride =
                2 * durationCount<Milliseconds>(SingleServerIsMasterMonitor::kMaxAwaitTime);
        }
    } else if (op == dbQuery) {
        invariant(!isCommand);
        opCtx->markKillOnClientDisconnect();

        dbresponse = receivedQuery(opCtx, nsString, c, m, behaviors);
    } else if (op == dbGetMore) {
        dbresponse = receivedGetMore(opCtx, m, currentOp, &forceLog);
    } else {
        // The remaining operations do not return any response. They are fire-and-forget.
        try {
            if (op == dbKillCursors) {
                currentOp.ensureStarted();
                slowMsOverride = 10;
                receivedKillCursors(opCtx, m);
            } else if (op != dbInsert && op != dbUpdate && op != dbDelete) {
                LOGV2(21968,
                      "Operation isn't supported: {operation}",
                      "Operation is not supported",
                      "operation"_attr = static_cast<int>(op));
                currentOp.done();
                forceLog = true;
            } else {
                if (!opCtx->getClient()->isInDirectClient()) {
                    uassert(18663,
                            str::stream() << "legacy writeOps not longer supported for "
                                          << "versioned connections, ns: " << nsString.ns()
                                          << ", op: " << networkOpToString(op),
                            !ShardedConnectionInfo::get(&c, false));
                }

                if (!nsString.isValid()) {
                    uassert(16257, str::stream() << "Invalid ns [" << ns << "]", false);
                } else if (op == dbInsert) {
                    receivedInsert(opCtx, nsString, m);
                } else if (op == dbUpdate) {
                    receivedUpdate(opCtx, nsString, m);
                } else if (op == dbDelete) {
                    receivedDelete(opCtx, nsString, m);
                } else {
                    MONGO_UNREACHABLE;
                }
            }
        } catch (const AssertionException& ue) {
            LastError::get(c).setLastError(ue.code(), ue.reason());
            LOGV2_DEBUG(21969,
                        3,
                        "Caught Assertion in {networkOp}, continuing: {error}",
                        "Assertion in fire-and-forget operation",
                        "networkOp"_attr = networkOpToString(op),
                        "error"_attr = redact(ue));
            debug.errInfo = ue.toStatus();
        }
        // A NotMaster error can be set either within receivedInsert/receivedUpdate/receivedDelete
        // or within the AssertionException handler above.  Either way, we want to throw an
        // exception here, which will cause the client to be disconnected.
        if (LastError::get(opCtx->getClient()).hadNotMasterError()) {
            notMasterLegacyUnackWrites.increment();
            uasserted(ErrorCodes::NotMaster,
                      str::stream()
                          << "Not-master error while processing '" << networkOpToString(op)
                          << "' operation  on '" << nsString << "' namespace via legacy "
                          << "fire-and-forget command execution.");
        }
    }

    // Mark the op as complete, and log it if appropriate. Returns a boolean indicating whether
    // this op should be sampled for profiling.
    const bool shouldSample = currentOp.completeAndLogOperation(
        opCtx, MONGO_LOGV2_DEFAULT_COMPONENT, dbresponse.response.size(), slowMsOverride, forceLog);

    Top::get(opCtx->getServiceContext())
        .incrementGlobalLatencyStats(
            opCtx,
            durationCount<Microseconds>(currentOp.elapsedTimeExcludingPauses()),
            currentOp.getReadWriteType());

    if (currentOp.shouldDBProfile(shouldSample)) {
        // Performance profiling is on
        if (opCtx->lockState()->isReadLocked()) {
            LOGV2_DEBUG(21970, 1, "Note: not profiling because of recursive read lock");
        } else if (c.isInDirectClient()) {
            LOGV2_DEBUG(21971, 1, "Note: not profiling because we are in DBDirectClient");
        } else if (behaviors.lockedForWriting()) {
            // TODO SERVER-26825: Fix race condition where fsyncLock is acquired post
            // lockedForWriting() call but prior to profile collection lock acquisition.
            LOGV2_DEBUG(21972, 1, "Note: not profiling because doing fsync+lock");
        } else if (storageGlobalParams.readOnly) {
            LOGV2_DEBUG(21973, 1, "Note: not profiling because server is read-only");
        } else {
            invariant(!opCtx->lockState()->inAWriteUnitOfWork());
            profile(opCtx, op);
        }
    }

    recordCurOpMetrics(opCtx);
    return dbresponse;
}

ServiceEntryPointCommon::Hooks::~Hooks() = default;

}  // namespace mongo
