/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/transaction_coordinators_stats_gen.h --output build/opt/mongo/db/s/transaction_coordinators_stats_gen.cpp src/mongo/db/s/transaction_coordinators_stats.idl
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
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct describing the number of transaction coordinators currently in each two-phase commit step
 */
class CurrentInSteps {
public:
    static constexpr auto kDeletingCoordinatorDocFieldName = "deletingCoordinatorDoc"_sd;
    static constexpr auto kWaitingForDecisionAcksFieldName = "waitingForDecisionAcks"_sd;
    static constexpr auto kWaitingForVotesFieldName = "waitingForVotes"_sd;
    static constexpr auto kWritingDecisionFieldName = "writingDecision"_sd;
    static constexpr auto kWritingParticipantListFieldName = "writingParticipantList"_sd;

    CurrentInSteps();

    static CurrentInSteps parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getWritingParticipantList() const { return _writingParticipantList; }
    void setWritingParticipantList(std::int64_t value) & {  _writingParticipantList = std::move(value);  }

    std::int64_t getWaitingForVotes() const { return _waitingForVotes; }
    void setWaitingForVotes(std::int64_t value) & {  _waitingForVotes = std::move(value);  }

    std::int64_t getWritingDecision() const { return _writingDecision; }
    void setWritingDecision(std::int64_t value) & {  _writingDecision = std::move(value);  }

    std::int64_t getWaitingForDecisionAcks() const { return _waitingForDecisionAcks; }
    void setWaitingForDecisionAcks(std::int64_t value) & {  _waitingForDecisionAcks = std::move(value);  }

    std::int64_t getDeletingCoordinatorDoc() const { return _deletingCoordinatorDoc; }
    void setDeletingCoordinatorDoc(std::int64_t value) & {  _deletingCoordinatorDoc = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _writingParticipantList{0};
    std::int64_t _waitingForVotes{0};
    std::int64_t _writingDecision{0};
    std::int64_t _waitingForDecisionAcks{0};
    std::int64_t _deletingCoordinatorDoc{0};
};

/**
 * A struct representing the section of the server status command with information about transaction coordinators
 */
class TransactionCoordinatorsStats {
public:
    static constexpr auto kCurrentInStepsFieldName = "currentInSteps"_sd;
    static constexpr auto kTotalAbortedTwoPhaseCommitFieldName = "totalAbortedTwoPhaseCommit"_sd;
    static constexpr auto kTotalCommittedTwoPhaseCommitFieldName = "totalCommittedTwoPhaseCommit"_sd;
    static constexpr auto kTotalCreatedFieldName = "totalCreated"_sd;
    static constexpr auto kTotalStartedTwoPhaseCommitFieldName = "totalStartedTwoPhaseCommit"_sd;

    TransactionCoordinatorsStats();
    TransactionCoordinatorsStats(mongo::CurrentInSteps currentInSteps);

    static TransactionCoordinatorsStats parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getTotalCreated() const { return _totalCreated; }
    void setTotalCreated(std::int64_t value) & {  _totalCreated = std::move(value);  }

    std::int64_t getTotalStartedTwoPhaseCommit() const { return _totalStartedTwoPhaseCommit; }
    void setTotalStartedTwoPhaseCommit(std::int64_t value) & {  _totalStartedTwoPhaseCommit = std::move(value);  }

    std::int64_t getTotalAbortedTwoPhaseCommit() const { return _totalAbortedTwoPhaseCommit; }
    void setTotalAbortedTwoPhaseCommit(std::int64_t value) & {  _totalAbortedTwoPhaseCommit = std::move(value);  }

    std::int64_t getTotalCommittedTwoPhaseCommit() const { return _totalCommittedTwoPhaseCommit; }
    void setTotalCommittedTwoPhaseCommit(std::int64_t value) & {  _totalCommittedTwoPhaseCommit = std::move(value);  }

    const mongo::CurrentInSteps& getCurrentInSteps() const { return _currentInSteps; }
    mongo::CurrentInSteps& getCurrentInSteps() { return _currentInSteps; }
    void setCurrentInSteps(mongo::CurrentInSteps value) & {  _currentInSteps = std::move(value); _hasCurrentInSteps = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _totalCreated{0};
    std::int64_t _totalStartedTwoPhaseCommit{0};
    std::int64_t _totalAbortedTwoPhaseCommit{0};
    std::int64_t _totalCommittedTwoPhaseCommit{0};
    mongo::CurrentInSteps _currentInSteps;
    bool _hasCurrentInSteps : 1;
};

}  // namespace mongo
