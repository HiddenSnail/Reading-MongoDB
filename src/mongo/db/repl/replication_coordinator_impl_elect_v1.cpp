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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kReplicationElection

#include "mongo/platform/basic.h"

#include <memory>

#include "mongo/db/repl/replication_coordinator_impl.h"
#include "mongo/db/repl/replication_metrics.h"
#include "mongo/db/repl/topology_coordinator.h"
#include "mongo/db/repl/vote_requester.h"
#include "mongo/logv2/log.h"
#include "mongo/platform/mutex.h"
#include "mongo/util/scopeguard.h"

namespace mongo {
namespace repl {

class ReplicationCoordinatorImpl::LoseElectionGuardV1 {
    LoseElectionGuardV1(const LoseElectionGuardV1&) = delete;
    LoseElectionGuardV1& operator=(const LoseElectionGuardV1&) = delete;

public:
    LoseElectionGuardV1(ReplicationCoordinatorImpl* replCoord) : _replCoord(replCoord) {}

    virtual ~LoseElectionGuardV1() {
        if (_dismissed) {
            return;
        }
        LOGV2(21434, "Lost election due to internal error", "isDryRun"_attr = _isDryRun);
        _replCoord->_topCoord->processLoseElection();
        _replCoord->_voteRequester.reset(nullptr);
        if (_isDryRun && _replCoord->_electionDryRunFinishedEvent.isValid()) {
            _replCoord->_replExecutor->signalEvent(_replCoord->_electionDryRunFinishedEvent);
        }
        if (_replCoord->_electionFinishedEvent.isValid()) {
            _replCoord->_replExecutor->signalEvent(_replCoord->_electionFinishedEvent);
        }

        // Clear the node's election candidate metrics if it loses either the dry-run or actual
        // election, since it will not become primary.
        ReplicationMetrics::get(getGlobalServiceContext()).clearElectionCandidateMetrics();
    }

    void dismiss() {
        _dismissed = true;
    }

protected:
    ReplicationCoordinatorImpl* const _replCoord;
    bool _isDryRun = false;
    bool _dismissed = false;
};

class ReplicationCoordinatorImpl::LoseElectionDryRunGuardV1 : public LoseElectionGuardV1 {
    LoseElectionDryRunGuardV1(const LoseElectionDryRunGuardV1&) = delete;
    LoseElectionDryRunGuardV1& operator=(const LoseElectionDryRunGuardV1&) = delete;

public:
    LoseElectionDryRunGuardV1(ReplicationCoordinatorImpl* replCoord)
        : LoseElectionGuardV1(replCoord) {
        _isDryRun = true;
    }
};

void ReplicationCoordinatorImpl::_startElectSelfV1_inlock(StartElectionReasonEnum reason) {
    invariant(!_voteRequester);

    switch (_rsConfigState) {
        case kConfigSteady:
            break;
        case kConfigInitiating:
        case kConfigReconfiguring:
        case kConfigHBReconfiguring:
            LOGV2_DEBUG(21435, 2, "Not standing for election; processing a configuration change");
            // Transition out of candidate role.
            _topCoord->processLoseElection();
            return;
        default:
            LOGV2_FATAL(28641,
                        "Entered replica set election code while in illegal config state "
                        "{rsConfigState}",
                        "Entered replica set election code while in illegal config state",
                        "rsConfigState"_attr = int(_rsConfigState));
    }

    auto finishedEvent = _makeEvent();
    if (!finishedEvent) {
        return;
    }
    _electionFinishedEvent = finishedEvent;

    auto dryRunFinishedEvent = _makeEvent();
    if (!dryRunFinishedEvent) {
        return;
    }
    _electionDryRunFinishedEvent = dryRunFinishedEvent;

    LoseElectionDryRunGuardV1 lossGuard(this);


    invariant(_rsConfig.getMemberAt(_selfIndex).isElectable());
    const auto lastAppliedOpTime = _getMyLastAppliedOpTime_inlock();

    if (lastAppliedOpTime == OpTime()) {
        LOGV2(21436,
              "Not trying to elect self, "
              "do not yet have a complete set of data from any point in time");
        return;
    }

    long long term = _topCoord->getTerm();
    int primaryIndex = -1;

    if (reason == StartElectionReasonEnum::kStepUpRequestSkipDryRun) {
        long long newTerm = term + 1;
        LOGV2(21437,
              "skipping dry run and running for election in term {newTerm}",
              "Skipping dry run and running for election",
              "newTerm"_attr = newTerm);
        _startRealElection_inlock(newTerm, reason);
        lossGuard.dismiss();
        return;
    }

    LOGV2(
        21438,
        "conducting a dry run election to see if we could be elected. current term: {currentTerm}",
        "Conducting a dry run election to see if we could be elected",
        "currentTerm"_attr = term);
    _voteRequester.reset(new VoteRequester);

    // Only set primaryIndex if the primary's vote is required during the dry run.
    if (reason == StartElectionReasonEnum::kCatchupTakeover) {
        primaryIndex = _topCoord->getCurrentPrimaryIndex();
    }
    StatusWith<executor::TaskExecutor::EventHandle> nextPhaseEvh =
        _voteRequester->start(_replExecutor.get(),
                              _rsConfig,
                              _selfIndex,
                              term,
                              true,  // dry run
                              lastAppliedOpTime,
                              primaryIndex);
    if (nextPhaseEvh.getStatus() == ErrorCodes::ShutdownInProgress) {
        return;
    }
    fassert(28685, nextPhaseEvh.getStatus());
    _replExecutor
        ->onEvent(nextPhaseEvh.getValue(),
                  [=](const executor::TaskExecutor::CallbackArgs&) {
                      _processDryRunResult(term, reason);
                  })
        .status_with_transitional_ignore();
    lossGuard.dismiss();
}

void ReplicationCoordinatorImpl::_processDryRunResult(long long originalTerm,
                                                      StartElectionReasonEnum reason) {
    stdx::lock_guard<Latch> lk(_mutex);
    LoseElectionDryRunGuardV1 lossGuard(this);

    invariant(_voteRequester);

    if (_topCoord->getTerm() != originalTerm) {
        LOGV2(21439,
              "not running for primary, we have been superseded already during dry run. original "
              "term: {originalTerm}, current term: {currentTerm}",
              "Not running for primary, we have been superseded already during dry run",
              "originalTerm"_attr = originalTerm,
              "currentTerm"_attr = _topCoord->getTerm());
        return;
    }

    const auto endResult = _voteRequester->getResult();
    switch (endResult) {
        case VoteRequester::Result::kInsufficientVotes:
            LOGV2(21440, "Not running for primary, we received insufficient votes");
            return;
        case VoteRequester::Result::kStaleTerm:
            LOGV2(21441, "Not running for primary, we have been superseded already");
            return;
        case VoteRequester::Result::kPrimaryRespondedNo:
            LOGV2(21442,
                  "Not running for primary, the current primary responded no in the dry run");
            return;
        case VoteRequester::Result::kCancelled:
            LOGV2(214400, "Not running for primary, election has been cancelled");
            return;
        case VoteRequester::Result::kSuccessfullyElected:
            break;
    }

    long long newTerm = originalTerm + 1;
    LOGV2(21444,
          "dry election run succeeded, running for election in term {newTerm}",
          "Dry election run succeeded, running for election",
          "newTerm"_attr = newTerm);

    _startRealElection_inlock(newTerm, reason);
    lossGuard.dismiss();
}

void ReplicationCoordinatorImpl::_startRealElection_inlock(long long newTerm,
                                                           StartElectionReasonEnum reason) {

    const Date_t now = _replExecutor->now();
    const OpTime lastCommittedOpTime = _topCoord->getLastCommittedOpTime();
    const OpTime lastSeenOpTime = _topCoord->latestKnownOpTime();
    const int numVotesNeeded = _rsConfig.getMajorityVoteCount();
    const double priorityAtElection = _rsConfig.getMemberAt(_selfIndex).getPriority();
    const Milliseconds electionTimeoutMillis = _rsConfig.getElectionTimeoutPeriod();
    const int priorPrimaryIndex = _topCoord->getCurrentPrimaryIndex();
    const boost::optional<int> priorPrimaryMemberId = (priorPrimaryIndex == -1)
        ? boost::none
        : boost::make_optional(_rsConfig.getMemberAt(priorPrimaryIndex).getId().getData());

    ReplicationMetrics::get(getServiceContext())
        .setElectionCandidateMetrics(reason,
                                     now,
                                     newTerm,
                                     lastCommittedOpTime,
                                     lastSeenOpTime,
                                     numVotesNeeded,
                                     priorityAtElection,
                                     electionTimeoutMillis,
                                     priorPrimaryMemberId);
    ReplicationMetrics::get(getServiceContext()).incrementNumElectionsCalledForReason(reason);

    LoseElectionDryRunGuardV1 lossGuard(this);

    TopologyCoordinator::UpdateTermResult updateTermResult;
    _updateTerm_inlock(newTerm, &updateTermResult);
    // This is the only valid result from this term update. If we are here, then we are not a
    // primary, so a stepdown is not possible. We have also not yet learned of a higher term from
    // someone else: seeing an update in the topology coordinator mid-election requires releasing
    // the mutex. This only happens during a dry run, which makes sure to check for term updates.
    invariant(updateTermResult == TopologyCoordinator::UpdateTermResult::kUpdatedTerm);
    // Secure our vote for ourself first
    _topCoord->voteForMyselfV1();

    // Store the vote in persistent storage.
    LastVote lastVote{newTerm, _selfIndex};

    auto cbStatus = _replExecutor->scheduleWork(
        [this, lastVote, reason](const executor::TaskExecutor::CallbackArgs& cbData) {
            _writeLastVoteForMyElection(lastVote, cbData, reason);
        });
    if (cbStatus.getStatus() == ErrorCodes::ShutdownInProgress) {
        return;
    }
    fassert(34421, cbStatus.getStatus());
    lossGuard.dismiss();
}

void ReplicationCoordinatorImpl::_writeLastVoteForMyElection(
    LastVote lastVote,
    const executor::TaskExecutor::CallbackArgs& cbData,
    StartElectionReasonEnum reason) {
    // storeLocalLastVoteDocument can call back in to the replication coordinator,
    // so _mutex must be unlocked here.  However, we cannot return until we
    // lock it because we want to lose the election on cancel or error and
    // doing so requires _mutex.
    auto status = [&] {
        if (!cbData.status.isOK()) {
            return cbData.status;
        }
        auto opCtx = cc().makeOperationContext();
        // Any writes that occur as part of an election should not be subject to Flow Control.
        opCtx->setShouldParticipateInFlowControl(false);
        return _externalState->storeLocalLastVoteDocument(opCtx.get(), lastVote);
    }();

    stdx::lock_guard<Latch> lk(_mutex);
    LoseElectionDryRunGuardV1 lossGuard(this);
    if (status == ErrorCodes::CallbackCanceled) {
        return;
    }

    if (!status.isOK()) {
        LOGV2(21445,
              "failed to store LastVote document when voting for myself: {error}",
              "Failed to store LastVote document when voting for myself",
              "error"_attr = status);
        return;
    }

    if (_topCoord->getTerm() != lastVote.getTerm()) {
        LOGV2(21446,
              "not running for primary, we have been superseded already while writing our last "
              "vote. election term: {electionTerm}, current term: {currentTerm}",
              "Not running for primary, we have been superseded already while writing our last "
              "vote",
              "electionTerm"_attr = lastVote.getTerm(),
              "currentTerm"_attr = _topCoord->getTerm());
        return;
    }
    _startVoteRequester_inlock(lastVote.getTerm(), reason);
    _replExecutor->signalEvent(_electionDryRunFinishedEvent);

    lossGuard.dismiss();
}

void ReplicationCoordinatorImpl::_startVoteRequester_inlock(long long newTerm,
                                                            StartElectionReasonEnum reason) {
    const auto lastAppliedOpTime = _getMyLastAppliedOpTime_inlock();

    _voteRequester.reset(new VoteRequester);
    StatusWith<executor::TaskExecutor::EventHandle> nextPhaseEvh = _voteRequester->start(
        _replExecutor.get(), _rsConfig, _selfIndex, newTerm, false, lastAppliedOpTime, -1);
    if (nextPhaseEvh.getStatus() == ErrorCodes::ShutdownInProgress) {
        return;
    }
    fassert(28643, nextPhaseEvh.getStatus());
    _replExecutor
        ->onEvent(nextPhaseEvh.getValue(),
                  [=](const executor::TaskExecutor::CallbackArgs&) {
                      _onVoteRequestComplete(newTerm, reason);
                  })
        .status_with_transitional_ignore();
}

MONGO_FAIL_POINT_DEFINE(electionHangsBeforeUpdateMemberState);

void ReplicationCoordinatorImpl::_onVoteRequestComplete(long long newTerm,
                                                        StartElectionReasonEnum reason) {
    stdx::lock_guard<Latch> lk(_mutex);
    LoseElectionGuardV1 lossGuard(this);

    invariant(_voteRequester);

    if (_topCoord->getTerm() != newTerm) {
        LOGV2(21447,
              "not becoming primary, we have been superseded already during election. election "
              "term: {electionTerm}, current term: {currentTerm}",
              "Not becoming primary, we have been superseded already during election",
              "electionTerm"_attr = newTerm,
              "currentTerm"_attr = _topCoord->getTerm());
        return;
    }

    const VoteRequester::Result endResult = _voteRequester->getResult();
    invariant(endResult != VoteRequester::Result::kPrimaryRespondedNo);

    switch (endResult) {
        case VoteRequester::Result::kCancelled:
            LOGV2(214480, "Not becoming primary, election has been cancelled");
            return;
        case VoteRequester::Result::kInsufficientVotes:
            LOGV2(21448, "Not becoming primary, we received insufficient votes");
            return;
        case VoteRequester::Result::kStaleTerm:
            LOGV2(21449, "Not becoming primary, we have been superseded already");
            return;
        case VoteRequester::Result::kSuccessfullyElected:
            LOGV2(21450,
                  "election succeeded, assuming primary role in term {term}",
                  "Election succeeded, assuming primary role",
                  "term"_attr = _topCoord->getTerm());
            ReplicationMetrics::get(getServiceContext())
                .incrementNumElectionsSuccessfulForReason(reason);
            break;
        case VoteRequester::Result::kPrimaryRespondedNo:
            // This is impossible because we would only require the primary's
            // vote during a dry run.
            MONGO_UNREACHABLE;
    }

    // Mark all nodes that responded to our vote request as up to avoid immediately
    // relinquishing primary.
    Date_t now = _replExecutor->now();
    _topCoord->resetMemberTimeouts(now, _voteRequester->getResponders());

    _voteRequester.reset();
    auto electionFinishedEvent = _electionFinishedEvent;

    electionHangsBeforeUpdateMemberState.execute([&](const BSONObj& customWait) {
        auto waitForMillis = Milliseconds(customWait["waitForMillis"].numberInt());
        LOGV2(21451,
              "election succeeded - electionHangsBeforeUpdateMemberState fail point "
              "enabled, sleeping {waitFor}",
              "Election succeeded - electionHangsBeforeUpdateMemberState fail point "
              "enabled, sleeping",
              "waitFor"_attr = waitForMillis);
        sleepFor(waitForMillis);
    });

    _postWonElectionUpdateMemberState(lk);
    _replExecutor->signalEvent(electionFinishedEvent);
    lossGuard.dismiss();
}

}  // namespace repl
}  // namespace mongo
