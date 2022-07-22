/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/replication_metrics_gen.h --output build/opt/mongo/db/repl/replication_metrics_gen.cpp src/mongo/db/repl/replication_metrics.idl
 */

#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/data_range.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/simple_bsonobj_comparator.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/repl/election_reason_counter_parser.h"
#include "mongo/db/repl/optime.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace repl {

/**
 * Reason why a node called for an election
 */
enum class StartElectionReasonEnum : std::int32_t {
    kElectionTimeout ,
    kPriorityTakeover ,
    kStepUpRequest ,
    kStepUpRequestSkipDryRun ,
    kCatchupTakeover ,
    kSingleNodePromptElection ,
};

StartElectionReasonEnum StartElectionReason_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData StartElectionReason_serializer(StartElectionReasonEnum value);

/**
 * Stores metrics related to all the elections a node has called
 */
class ElectionMetrics {
public:
    static constexpr auto kAverageCatchUpOpsFieldName = "averageCatchUpOps"_sd;
    static constexpr auto kCatchUpTakeoverFieldName = "catchUpTakeover"_sd;
    static constexpr auto kElectionTimeoutFieldName = "electionTimeout"_sd;
    static constexpr auto kFreezeTimeoutFieldName = "freezeTimeout"_sd;
    static constexpr auto kNumCatchUpsFieldName = "numCatchUps"_sd;
    static constexpr auto kNumCatchUpsAlreadyCaughtUpFieldName = "numCatchUpsAlreadyCaughtUp"_sd;
    static constexpr auto kNumCatchUpsFailedWithErrorFieldName = "numCatchUpsFailedWithError"_sd;
    static constexpr auto kNumCatchUpsFailedWithNewTermFieldName = "numCatchUpsFailedWithNewTerm"_sd;
    static constexpr auto kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdFieldName = "numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd"_sd;
    static constexpr auto kNumCatchUpsSkippedFieldName = "numCatchUpsSkipped"_sd;
    static constexpr auto kNumCatchUpsSucceededFieldName = "numCatchUpsSucceeded"_sd;
    static constexpr auto kNumCatchUpsTimedOutFieldName = "numCatchUpsTimedOut"_sd;
    static constexpr auto kNumStepDownsCausedByHigherTermFieldName = "numStepDownsCausedByHigherTerm"_sd;
    static constexpr auto kPriorityTakeoverFieldName = "priorityTakeover"_sd;
    static constexpr auto kStepUpCmdFieldName = "stepUpCmd"_sd;

    ElectionMetrics();
    ElectionMetrics(mongo::repl::ElectionReasonCounter stepUpCmd, mongo::repl::ElectionReasonCounter priorityTakeover, mongo::repl::ElectionReasonCounter catchUpTakeover, mongo::repl::ElectionReasonCounter electionTimeout, mongo::repl::ElectionReasonCounter freezeTimeout);

    static ElectionMetrics parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Election reason counter for step up requests
     */
    const mongo::repl::ElectionReasonCounter& getStepUpCmd() const { return _stepUpCmd; }
    mongo::repl::ElectionReasonCounter& getStepUpCmd() { return _stepUpCmd; }
    void setStepUpCmd(mongo::repl::ElectionReasonCounter value) & {  _stepUpCmd = std::move(value); _hasStepUpCmd = true; }

    /**
     * Election reason counter for priority takeovers
     */
    const mongo::repl::ElectionReasonCounter& getPriorityTakeover() const { return _priorityTakeover; }
    mongo::repl::ElectionReasonCounter& getPriorityTakeover() { return _priorityTakeover; }
    void setPriorityTakeover(mongo::repl::ElectionReasonCounter value) & {  _priorityTakeover = std::move(value); _hasPriorityTakeover = true; }

    /**
     * Election reason counter for catchup takeovers
     */
    const mongo::repl::ElectionReasonCounter& getCatchUpTakeover() const { return _catchUpTakeover; }
    mongo::repl::ElectionReasonCounter& getCatchUpTakeover() { return _catchUpTakeover; }
    void setCatchUpTakeover(mongo::repl::ElectionReasonCounter value) & {  _catchUpTakeover = std::move(value); _hasCatchUpTakeover = true; }

    /**
     * Election reason counter for when the node didn't see a primary in the past electionTimeoutPeriod milliseconds
     */
    const mongo::repl::ElectionReasonCounter& getElectionTimeout() const { return _electionTimeout; }
    mongo::repl::ElectionReasonCounter& getElectionTimeout() { return _electionTimeout; }
    void setElectionTimeout(mongo::repl::ElectionReasonCounter value) & {  _electionTimeout = std::move(value); _hasElectionTimeout = true; }

    /**
     * Election reason counter for single node replica set prompt elections
     */
    const mongo::repl::ElectionReasonCounter& getFreezeTimeout() const { return _freezeTimeout; }
    mongo::repl::ElectionReasonCounter& getFreezeTimeout() { return _freezeTimeout; }
    void setFreezeTimeout(mongo::repl::ElectionReasonCounter value) & {  _freezeTimeout = std::move(value); _hasFreezeTimeout = true; }

    /**
     * Number of times this node stepped down because it saw a higher term
     */
    std::int64_t getNumStepDownsCausedByHigherTerm() const { return _numStepDownsCausedByHigherTerm; }
    void setNumStepDownsCausedByHigherTerm(std::int64_t value) & {  _numStepDownsCausedByHigherTerm = std::move(value);  }

    /**
     * Number of elections that required the primary to catch up because it was behind
     */
    std::int64_t getNumCatchUps() const { return _numCatchUps; }
    void setNumCatchUps(std::int64_t value) & {  _numCatchUps = std::move(value);  }

    /**
     * Number of times primary catchup concluded because the primary caught up to the latest known optime successfully
     */
    std::int64_t getNumCatchUpsSucceeded() const { return _numCatchUpsSucceeded; }
    void setNumCatchUpsSucceeded(std::int64_t value) & {  _numCatchUpsSucceeded = std::move(value);  }

    /**
     * Number of times primary catchup concluded because the primary was already caught up
     */
    std::int64_t getNumCatchUpsAlreadyCaughtUp() const { return _numCatchUpsAlreadyCaughtUp; }
    void setNumCatchUpsAlreadyCaughtUp(std::int64_t value) & {  _numCatchUpsAlreadyCaughtUp = std::move(value);  }

    /**
     * Number of times primary catchup concluded because it was skipped
     */
    std::int64_t getNumCatchUpsSkipped() const { return _numCatchUpsSkipped; }
    void setNumCatchUpsSkipped(std::int64_t value) & {  _numCatchUpsSkipped = std::move(value);  }

    /**
     * Number of times primary catchup concluded because it timed out
     */
    std::int64_t getNumCatchUpsTimedOut() const { return _numCatchUpsTimedOut; }
    void setNumCatchUpsTimedOut(std::int64_t value) & {  _numCatchUpsTimedOut = std::move(value);  }

    /**
     * Number of times primary catchup concluded because it failed with an error
     */
    std::int64_t getNumCatchUpsFailedWithError() const { return _numCatchUpsFailedWithError; }
    void setNumCatchUpsFailedWithError(std::int64_t value) & {  _numCatchUpsFailedWithError = std::move(value);  }

    /**
     * Number of times primary catchup concluded because the primary stepped down on seeing a higher term
     */
    std::int64_t getNumCatchUpsFailedWithNewTerm() const { return _numCatchUpsFailedWithNewTerm; }
    void setNumCatchUpsFailedWithNewTerm(std::int64_t value) & {  _numCatchUpsFailedWithNewTerm = std::move(value);  }

    /**
     * Number of times primary catchup concluded because of the replSetAbortPrimaryCatchUp command
     */
    std::int64_t getNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd() const { return _numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd; }
    void setNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd(std::int64_t value) & {  _numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd = std::move(value);  }

    /**
     * Average number of ops applied during catchup
     */
    double getAverageCatchUpOps() const { return _averageCatchUpOps; }
    void setAverageCatchUpOps(double value) & {  _averageCatchUpOps = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::repl::ElectionReasonCounter _stepUpCmd;
    mongo::repl::ElectionReasonCounter _priorityTakeover;
    mongo::repl::ElectionReasonCounter _catchUpTakeover;
    mongo::repl::ElectionReasonCounter _electionTimeout;
    mongo::repl::ElectionReasonCounter _freezeTimeout;
    std::int64_t _numStepDownsCausedByHigherTerm{0};
    std::int64_t _numCatchUps{0};
    std::int64_t _numCatchUpsSucceeded{0};
    std::int64_t _numCatchUpsAlreadyCaughtUp{0};
    std::int64_t _numCatchUpsSkipped{0};
    std::int64_t _numCatchUpsTimedOut{0};
    std::int64_t _numCatchUpsFailedWithError{0};
    std::int64_t _numCatchUpsFailedWithNewTerm{0};
    std::int64_t _numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd{0};
    double _averageCatchUpOps{0.0};
    bool _hasStepUpCmd : 1;
    bool _hasPriorityTakeover : 1;
    bool _hasCatchUpTakeover : 1;
    bool _hasElectionTimeout : 1;
    bool _hasFreezeTimeout : 1;
};

/**
 * Stores metrics that are specific to the last election in which the node was a candidate
 */
class ElectionCandidateMetrics {
public:
    static constexpr auto kElectionTermFieldName = "electionTerm"_sd;
    static constexpr auto kElectionTimeoutMillisFieldName = "electionTimeoutMillis"_sd;
    static constexpr auto kLastCommittedOpTimeAtElectionFieldName = "lastCommittedOpTimeAtElection"_sd;
    static constexpr auto kLastElectionDateFieldName = "lastElectionDate"_sd;
    static constexpr auto kLastElectionReasonFieldName = "lastElectionReason"_sd;
    static constexpr auto kLastSeenOpTimeAtElectionFieldName = "lastSeenOpTimeAtElection"_sd;
    static constexpr auto kNewTermStartDateFieldName = "newTermStartDate"_sd;
    static constexpr auto kNumCatchUpOpsFieldName = "numCatchUpOps"_sd;
    static constexpr auto kNumVotesNeededFieldName = "numVotesNeeded"_sd;
    static constexpr auto kPriorPrimaryMemberIdFieldName = "priorPrimaryMemberId"_sd;
    static constexpr auto kPriorityAtElectionFieldName = "priorityAtElection"_sd;
    static constexpr auto kTargetCatchupOpTimeFieldName = "targetCatchupOpTime"_sd;
    static constexpr auto kWMajorityWriteAvailabilityDateFieldName = "wMajorityWriteAvailabilityDate"_sd;

    ElectionCandidateMetrics();
    ElectionCandidateMetrics(mongo::repl::StartElectionReasonEnum lastElectionReason, mongo::Date_t lastElectionDate, std::int64_t electionTerm, mongo::repl::OpTime lastCommittedOpTimeAtElection, mongo::repl::OpTime lastSeenOpTimeAtElection, std::int32_t numVotesNeeded, double priorityAtElection, std::int64_t electionTimeoutMillis);

    static ElectionCandidateMetrics parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Reason why the node called for the election
     */
    mongo::repl::StartElectionReasonEnum getLastElectionReason() const { return _lastElectionReason; }
    void setLastElectionReason(mongo::repl::StartElectionReasonEnum value) & {  _lastElectionReason = std::move(value); _hasLastElectionReason = true; }

    /**
     * Time the node called for the election
     */
    const mongo::Date_t& getLastElectionDate() const { return _lastElectionDate; }
    void setLastElectionDate(mongo::Date_t value) & {  _lastElectionDate = std::move(value); _hasLastElectionDate = true; }

    /**
     * Proposed new term for this election
     */
    std::int64_t getElectionTerm() const { return _electionTerm; }
    void setElectionTerm(std::int64_t value) & {  _electionTerm = std::move(value); _hasElectionTerm = true; }

    /**
     * Last OpTime the node committed before calling the election
     */
    const mongo::repl::OpTime& getLastCommittedOpTimeAtElection() const { return _lastCommittedOpTimeAtElection; }
    void setLastCommittedOpTimeAtElection(mongo::repl::OpTime value) & {  _lastCommittedOpTimeAtElection = std::move(value); _hasLastCommittedOpTimeAtElection = true; }

    /**
     * Last OpTime the node saw before calling the election
     */
    const mongo::repl::OpTime& getLastSeenOpTimeAtElection() const { return _lastSeenOpTimeAtElection; }
    void setLastSeenOpTimeAtElection(mongo::repl::OpTime value) & {  _lastSeenOpTimeAtElection = std::move(value); _hasLastSeenOpTimeAtElection = true; }

    /**
     * Number of votes the node needed to win the election
     */
    std::int32_t getNumVotesNeeded() const { return _numVotesNeeded; }
    void setNumVotesNeeded(std::int32_t value) & {  _numVotesNeeded = std::move(value); _hasNumVotesNeeded = true; }

    /**
     * Priority of the node at the time of the election
     */
    double getPriorityAtElection() const { return _priorityAtElection; }
    void setPriorityAtElection(double value) & {  _priorityAtElection = std::move(value); _hasPriorityAtElection = true; }

    /**
     * Timeout duration before a node initiates a new election
     */
    std::int64_t getElectionTimeoutMillis() const { return _electionTimeoutMillis; }
    void setElectionTimeoutMillis(std::int64_t value) & {  _electionTimeoutMillis = std::move(value); _hasElectionTimeoutMillis = true; }

    /**
     * MemberId of the prior primary, according to the node. Not set if there was no prior primary
     */
    const boost::optional<std::int32_t> getPriorPrimaryMemberId() const& { return _priorPrimaryMemberId; }
    void getPriorPrimaryMemberId() && = delete;
    void setPriorPrimaryMemberId(boost::optional<std::int32_t> value) & {  _priorPrimaryMemberId = std::move(value);  }

    /**
     * The node's target opTime for catchup
     */
    const boost::optional<mongo::repl::OpTime>& getTargetCatchupOpTime() const& { return _targetCatchupOpTime; }
    void getTargetCatchupOpTime() && = delete;
    void setTargetCatchupOpTime(boost::optional<mongo::repl::OpTime> value) & {  _targetCatchupOpTime = std::move(value);  }

    /**
     * Number of ops applied during catchup when the primary successfully catches up
     */
    const boost::optional<std::int64_t> getNumCatchUpOps() const& { return _numCatchUpOps; }
    void getNumCatchUpOps() && = delete;
    void setNumCatchUpOps(boost::optional<std::int64_t> value) & {  _numCatchUpOps = std::move(value);  }

    /**
     * Time the new term oplog entry was written
     */
    const boost::optional<mongo::Date_t>& getNewTermStartDate() const& { return _newTermStartDate; }
    void getNewTermStartDate() && = delete;
    void setNewTermStartDate(boost::optional<mongo::Date_t> value) & {  _newTermStartDate = std::move(value);  }

    /**
     * Time w:majority write concern is satisfied for new term oplog entry
     */
    const boost::optional<mongo::Date_t>& getWMajorityWriteAvailabilityDate() const& { return _wMajorityWriteAvailabilityDate; }
    void getWMajorityWriteAvailabilityDate() && = delete;
    void setWMajorityWriteAvailabilityDate(boost::optional<mongo::Date_t> value) & {  _wMajorityWriteAvailabilityDate = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::repl::StartElectionReasonEnum _lastElectionReason;
    mongo::Date_t _lastElectionDate;
    std::int64_t _electionTerm;
    mongo::repl::OpTime _lastCommittedOpTimeAtElection;
    mongo::repl::OpTime _lastSeenOpTimeAtElection;
    std::int32_t _numVotesNeeded;
    double _priorityAtElection;
    std::int64_t _electionTimeoutMillis;
    boost::optional<std::int32_t> _priorPrimaryMemberId;
    boost::optional<mongo::repl::OpTime> _targetCatchupOpTime;
    boost::optional<std::int64_t> _numCatchUpOps;
    boost::optional<mongo::Date_t> _newTermStartDate;
    boost::optional<mongo::Date_t> _wMajorityWriteAvailabilityDate;
    bool _hasLastElectionReason : 1;
    bool _hasLastElectionDate : 1;
    bool _hasElectionTerm : 1;
    bool _hasLastCommittedOpTimeAtElection : 1;
    bool _hasLastSeenOpTimeAtElection : 1;
    bool _hasNumVotesNeeded : 1;
    bool _hasPriorityAtElection : 1;
    bool _hasElectionTimeoutMillis : 1;
};

/**
 * Stores metrics that are specific to the last election in which the node voted
 */
class ElectionParticipantMetrics {
public:
    static constexpr auto kElectionCandidateMemberIdFieldName = "electionCandidateMemberId"_sd;
    static constexpr auto kElectionTermFieldName = "electionTerm"_sd;
    static constexpr auto kLastAppliedOpTimeAtElectionFieldName = "lastAppliedOpTimeAtElection"_sd;
    static constexpr auto kLastVoteDateFieldName = "lastVoteDate"_sd;
    static constexpr auto kMaxAppliedOpTimeInSetFieldName = "maxAppliedOpTimeInSet"_sd;
    static constexpr auto kNewTermAppliedDateFieldName = "newTermAppliedDate"_sd;
    static constexpr auto kNewTermStartDateFieldName = "newTermStartDate"_sd;
    static constexpr auto kPriorityAtElectionFieldName = "priorityAtElection"_sd;
    static constexpr auto kVoteReasonFieldName = "voteReason"_sd;
    static constexpr auto kVotedForCandidateFieldName = "votedForCandidate"_sd;

    ElectionParticipantMetrics();
    ElectionParticipantMetrics(bool votedForCandidate, std::int64_t electionTerm, mongo::Date_t lastVoteDate, std::int32_t electionCandidateMemberId, std::string voteReason, mongo::repl::OpTime lastAppliedOpTimeAtElection, mongo::repl::OpTime maxAppliedOpTimeInSet, double priorityAtElection);

    static ElectionParticipantMetrics parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * States if the node has voted yes or no for the candidate in this election
     */
    bool getVotedForCandidate() const { return _votedForCandidate; }
    void setVotedForCandidate(bool value) & {  _votedForCandidate = std::move(value); _hasVotedForCandidate = true; }

    /**
     * The term of the candidate that is running for election
     */
    std::int64_t getElectionTerm() const { return _electionTerm; }
    void setElectionTerm(std::int64_t value) & {  _electionTerm = std::move(value); _hasElectionTerm = true; }

    /**
     * Time the node voted
     */
    const mongo::Date_t& getLastVoteDate() const { return _lastVoteDate; }
    void setLastVoteDate(mongo::Date_t value) & {  _lastVoteDate = std::move(value); _hasLastVoteDate = true; }

    /**
     * MemberId of the node requesting a vote
     */
    std::int32_t getElectionCandidateMemberId() const { return _electionCandidateMemberId; }
    void setElectionCandidateMemberId(std::int32_t value) & {  _electionCandidateMemberId = std::move(value); _hasElectionCandidateMemberId = true; }

    /**
     * Reason why the node voted the way it did
     */
    const StringData getVoteReason() const& { return _voteReason; }
    void getVoteReason() && = delete;
    void setVoteReason(StringData value) & { auto _tmpValue = value.toString();  _voteReason = std::move(_tmpValue); _hasVoteReason = true; }

    /**
     * Latest applied OpTime at the time of voting
     */
    const mongo::repl::OpTime& getLastAppliedOpTimeAtElection() const { return _lastAppliedOpTimeAtElection; }
    void setLastAppliedOpTimeAtElection(mongo::repl::OpTime value) & {  _lastAppliedOpTimeAtElection = std::move(value); _hasLastAppliedOpTimeAtElection = true; }

    /**
     * Highest applied time of any node in this replica set, as currently known by this node
     */
    const mongo::repl::OpTime& getMaxAppliedOpTimeInSet() const { return _maxAppliedOpTimeInSet; }
    void setMaxAppliedOpTimeInSet(mongo::repl::OpTime value) & {  _maxAppliedOpTimeInSet = std::move(value); _hasMaxAppliedOpTimeInSet = true; }

    /**
     * The node's priority at the time of the election
     */
    double getPriorityAtElection() const { return _priorityAtElection; }
    void setPriorityAtElection(double value) & {  _priorityAtElection = std::move(value); _hasPriorityAtElection = true; }

    /**
     * Time the new term oplog entry was written by the primary
     */
    const boost::optional<mongo::Date_t>& getNewTermStartDate() const& { return _newTermStartDate; }
    void getNewTermStartDate() && = delete;
    void setNewTermStartDate(boost::optional<mongo::Date_t> value) & {  _newTermStartDate = std::move(value);  }

    /**
     * Time this node applied the new term oplog entry
     */
    const boost::optional<mongo::Date_t>& getNewTermAppliedDate() const& { return _newTermAppliedDate; }
    void getNewTermAppliedDate() && = delete;
    void setNewTermAppliedDate(boost::optional<mongo::Date_t> value) & {  _newTermAppliedDate = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    bool _votedForCandidate;
    std::int64_t _electionTerm;
    mongo::Date_t _lastVoteDate;
    std::int32_t _electionCandidateMemberId;
    std::string _voteReason;
    mongo::repl::OpTime _lastAppliedOpTimeAtElection;
    mongo::repl::OpTime _maxAppliedOpTimeInSet;
    double _priorityAtElection;
    boost::optional<mongo::Date_t> _newTermStartDate;
    boost::optional<mongo::Date_t> _newTermAppliedDate;
    bool _hasVotedForCandidate : 1;
    bool _hasElectionTerm : 1;
    bool _hasLastVoteDate : 1;
    bool _hasElectionCandidateMemberId : 1;
    bool _hasVoteReason : 1;
    bool _hasLastAppliedOpTimeAtElection : 1;
    bool _hasMaxAppliedOpTimeInSet : 1;
    bool _hasPriorityAtElection : 1;
};

}  // namespace repl
}  // namespace mongo
