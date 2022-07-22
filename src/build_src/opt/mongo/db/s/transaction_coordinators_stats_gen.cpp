/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/transaction_coordinators_stats_gen.h --output build/opt/mongo/db/s/transaction_coordinators_stats_gen.cpp src/mongo/db/s/transaction_coordinators_stats.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/s/transaction_coordinators_stats_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData CurrentInSteps::kDeletingCoordinatorDocFieldName;
constexpr StringData CurrentInSteps::kWaitingForDecisionAcksFieldName;
constexpr StringData CurrentInSteps::kWaitingForVotesFieldName;
constexpr StringData CurrentInSteps::kWritingDecisionFieldName;
constexpr StringData CurrentInSteps::kWritingParticipantListFieldName;


CurrentInSteps::CurrentInSteps()  {
    // Used for initialization only
}


CurrentInSteps CurrentInSteps::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CurrentInSteps object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CurrentInSteps::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kWritingParticipantListBit = 0;
    const size_t kWaitingForVotesBit = 1;
    const size_t kWritingDecisionBit = 2;
    const size_t kWaitingForDecisionAcksBit = 3;
    const size_t kDeletingCoordinatorDocBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kWritingParticipantListFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kWritingParticipantListBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWritingParticipantListBit);

                _writingParticipantList = element._numberLong();
            }
        }
        else if (fieldName == kWaitingForVotesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kWaitingForVotesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWaitingForVotesBit);

                _waitingForVotes = element._numberLong();
            }
        }
        else if (fieldName == kWritingDecisionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kWritingDecisionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWritingDecisionBit);

                _writingDecision = element._numberLong();
            }
        }
        else if (fieldName == kWaitingForDecisionAcksFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kWaitingForDecisionAcksBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWaitingForDecisionAcksBit);

                _waitingForDecisionAcks = element._numberLong();
            }
        }
        else if (fieldName == kDeletingCoordinatorDocFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kDeletingCoordinatorDocBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDeletingCoordinatorDocBit);

                _deletingCoordinatorDoc = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kWritingParticipantListBit]) {
            _writingParticipantList = 0;
        }
        if (!usedFields[kWaitingForVotesBit]) {
            _waitingForVotes = 0;
        }
        if (!usedFields[kWritingDecisionBit]) {
            _writingDecision = 0;
        }
        if (!usedFields[kWaitingForDecisionAcksBit]) {
            _waitingForDecisionAcks = 0;
        }
        if (!usedFields[kDeletingCoordinatorDocBit]) {
            _deletingCoordinatorDoc = 0;
        }
    }

}


void CurrentInSteps::serialize(BSONObjBuilder* builder) const {
    builder->append(kWritingParticipantListFieldName, _writingParticipantList);

    builder->append(kWaitingForVotesFieldName, _waitingForVotes);

    builder->append(kWritingDecisionFieldName, _writingDecision);

    builder->append(kWaitingForDecisionAcksFieldName, _waitingForDecisionAcks);

    builder->append(kDeletingCoordinatorDocFieldName, _deletingCoordinatorDoc);

}


BSONObj CurrentInSteps::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData TransactionCoordinatorsStats::kCurrentInStepsFieldName;
constexpr StringData TransactionCoordinatorsStats::kTotalAbortedTwoPhaseCommitFieldName;
constexpr StringData TransactionCoordinatorsStats::kTotalCommittedTwoPhaseCommitFieldName;
constexpr StringData TransactionCoordinatorsStats::kTotalCreatedFieldName;
constexpr StringData TransactionCoordinatorsStats::kTotalStartedTwoPhaseCommitFieldName;


TransactionCoordinatorsStats::TransactionCoordinatorsStats() : _hasCurrentInSteps(false) {
    // Used for initialization only
}
TransactionCoordinatorsStats::TransactionCoordinatorsStats(mongo::CurrentInSteps currentInSteps) : _currentInSteps(std::move(currentInSteps)), _hasCurrentInSteps(true) {
    // Used for initialization only
}


TransactionCoordinatorsStats TransactionCoordinatorsStats::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TransactionCoordinatorsStats object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TransactionCoordinatorsStats::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kTotalCreatedBit = 0;
    const size_t kTotalStartedTwoPhaseCommitBit = 1;
    const size_t kTotalAbortedTwoPhaseCommitBit = 2;
    const size_t kTotalCommittedTwoPhaseCommitBit = 3;
    const size_t kCurrentInStepsBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kTotalCreatedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalCreatedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalCreatedBit);

                _totalCreated = element._numberLong();
            }
        }
        else if (fieldName == kTotalStartedTwoPhaseCommitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalStartedTwoPhaseCommitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalStartedTwoPhaseCommitBit);

                _totalStartedTwoPhaseCommit = element._numberLong();
            }
        }
        else if (fieldName == kTotalAbortedTwoPhaseCommitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalAbortedTwoPhaseCommitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalAbortedTwoPhaseCommitBit);

                _totalAbortedTwoPhaseCommit = element._numberLong();
            }
        }
        else if (fieldName == kTotalCommittedTwoPhaseCommitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalCommittedTwoPhaseCommitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalCommittedTwoPhaseCommitBit);

                _totalCommittedTwoPhaseCommit = element._numberLong();
            }
        }
        else if (fieldName == kCurrentInStepsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCurrentInStepsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentInStepsBit);

                _hasCurrentInSteps = true;
                IDLParserErrorContext tempContext(kCurrentInStepsFieldName, &ctxt);
                const auto localObject = element.Obj();
                _currentInSteps = mongo::CurrentInSteps::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kTotalCreatedBit]) {
            _totalCreated = 0;
        }
        if (!usedFields[kTotalStartedTwoPhaseCommitBit]) {
            _totalStartedTwoPhaseCommit = 0;
        }
        if (!usedFields[kTotalAbortedTwoPhaseCommitBit]) {
            _totalAbortedTwoPhaseCommit = 0;
        }
        if (!usedFields[kTotalCommittedTwoPhaseCommitBit]) {
            _totalCommittedTwoPhaseCommit = 0;
        }
        if (!usedFields[kCurrentInStepsBit]) {
            ctxt.throwMissingField(kCurrentInStepsFieldName);
        }
    }

}


void TransactionCoordinatorsStats::serialize(BSONObjBuilder* builder) const {
    invariant(_hasCurrentInSteps);

    builder->append(kTotalCreatedFieldName, _totalCreated);

    builder->append(kTotalStartedTwoPhaseCommitFieldName, _totalStartedTwoPhaseCommit);

    builder->append(kTotalAbortedTwoPhaseCommitFieldName, _totalAbortedTwoPhaseCommit);

    builder->append(kTotalCommittedTwoPhaseCommitFieldName, _totalCommittedTwoPhaseCommit);

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kCurrentInStepsFieldName));
        _currentInSteps.serialize(&subObjBuilder);
    }

}


BSONObj TransactionCoordinatorsStats::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
