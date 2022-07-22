/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/replication_metrics_gen.h --output build/opt/mongo/db/repl/replication_metrics_gen.cpp src/mongo/db/repl/replication_metrics.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/replication_metrics_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

/**
 * Reason why a node called for an election
 */
namespace  {
constexpr StringData kStartElectionReason_kElectionTimeout = "electionTimeout"_sd;
constexpr StringData kStartElectionReason_kPriorityTakeover = "priorityTakeover"_sd;
constexpr StringData kStartElectionReason_kStepUpRequest = "stepUpRequest"_sd;
constexpr StringData kStartElectionReason_kStepUpRequestSkipDryRun = "stepUpRequestSkipDryRun"_sd;
constexpr StringData kStartElectionReason_kCatchupTakeover = "catchupTakeover"_sd;
constexpr StringData kStartElectionReason_kSingleNodePromptElection = "singleNodePromptElection"_sd;
}  // namespace 

StartElectionReasonEnum StartElectionReason_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kStartElectionReason_kElectionTimeout) {
        return StartElectionReasonEnum::kElectionTimeout;
    }
    if (value == kStartElectionReason_kPriorityTakeover) {
        return StartElectionReasonEnum::kPriorityTakeover;
    }
    if (value == kStartElectionReason_kStepUpRequest) {
        return StartElectionReasonEnum::kStepUpRequest;
    }
    if (value == kStartElectionReason_kStepUpRequestSkipDryRun) {
        return StartElectionReasonEnum::kStepUpRequestSkipDryRun;
    }
    if (value == kStartElectionReason_kCatchupTakeover) {
        return StartElectionReasonEnum::kCatchupTakeover;
    }
    if (value == kStartElectionReason_kSingleNodePromptElection) {
        return StartElectionReasonEnum::kSingleNodePromptElection;
    }
    ctxt.throwBadEnumValue(value);
}

StringData StartElectionReason_serializer(StartElectionReasonEnum value) {
    if (value == StartElectionReasonEnum::kElectionTimeout) {
        return kStartElectionReason_kElectionTimeout;
    }
    if (value == StartElectionReasonEnum::kPriorityTakeover) {
        return kStartElectionReason_kPriorityTakeover;
    }
    if (value == StartElectionReasonEnum::kStepUpRequest) {
        return kStartElectionReason_kStepUpRequest;
    }
    if (value == StartElectionReasonEnum::kStepUpRequestSkipDryRun) {
        return kStartElectionReason_kStepUpRequestSkipDryRun;
    }
    if (value == StartElectionReasonEnum::kCatchupTakeover) {
        return kStartElectionReason_kCatchupTakeover;
    }
    if (value == StartElectionReasonEnum::kSingleNodePromptElection) {
        return kStartElectionReason_kSingleNodePromptElection;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData ElectionMetrics::kAverageCatchUpOpsFieldName;
constexpr StringData ElectionMetrics::kCatchUpTakeoverFieldName;
constexpr StringData ElectionMetrics::kElectionTimeoutFieldName;
constexpr StringData ElectionMetrics::kFreezeTimeoutFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsAlreadyCaughtUpFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsFailedWithErrorFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsFailedWithNewTermFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsSkippedFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsSucceededFieldName;
constexpr StringData ElectionMetrics::kNumCatchUpsTimedOutFieldName;
constexpr StringData ElectionMetrics::kNumStepDownsCausedByHigherTermFieldName;
constexpr StringData ElectionMetrics::kPriorityTakeoverFieldName;
constexpr StringData ElectionMetrics::kStepUpCmdFieldName;


ElectionMetrics::ElectionMetrics() : _hasStepUpCmd(false), _hasPriorityTakeover(false), _hasCatchUpTakeover(false), _hasElectionTimeout(false), _hasFreezeTimeout(false) {
    // Used for initialization only
}
ElectionMetrics::ElectionMetrics(mongo::repl::ElectionReasonCounter stepUpCmd, mongo::repl::ElectionReasonCounter priorityTakeover, mongo::repl::ElectionReasonCounter catchUpTakeover, mongo::repl::ElectionReasonCounter electionTimeout, mongo::repl::ElectionReasonCounter freezeTimeout) : _stepUpCmd(std::move(stepUpCmd)), _priorityTakeover(std::move(priorityTakeover)), _catchUpTakeover(std::move(catchUpTakeover)), _electionTimeout(std::move(electionTimeout)), _freezeTimeout(std::move(freezeTimeout)), _hasStepUpCmd(true), _hasPriorityTakeover(true), _hasCatchUpTakeover(true), _hasElectionTimeout(true), _hasFreezeTimeout(true) {
    // Used for initialization only
}


ElectionMetrics ElectionMetrics::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ElectionMetrics object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ElectionMetrics::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<15> usedFields;
    const size_t kStepUpCmdBit = 0;
    const size_t kPriorityTakeoverBit = 1;
    const size_t kCatchUpTakeoverBit = 2;
    const size_t kElectionTimeoutBit = 3;
    const size_t kFreezeTimeoutBit = 4;
    const size_t kNumStepDownsCausedByHigherTermBit = 5;
    const size_t kNumCatchUpsBit = 6;
    const size_t kNumCatchUpsSucceededBit = 7;
    const size_t kNumCatchUpsAlreadyCaughtUpBit = 8;
    const size_t kNumCatchUpsSkippedBit = 9;
    const size_t kNumCatchUpsTimedOutBit = 10;
    const size_t kNumCatchUpsFailedWithErrorBit = 11;
    const size_t kNumCatchUpsFailedWithNewTermBit = 12;
    const size_t kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdBit = 13;
    const size_t kAverageCatchUpOpsBit = 14;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kStepUpCmdFieldName) {
            if (MONGO_unlikely(usedFields[kStepUpCmdBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStepUpCmdBit);

            _hasStepUpCmd = true;
            _stepUpCmd = repl::parseElectionReasonCounter(element);
        }
        else if (fieldName == kPriorityTakeoverFieldName) {
            if (MONGO_unlikely(usedFields[kPriorityTakeoverBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kPriorityTakeoverBit);

            _hasPriorityTakeover = true;
            _priorityTakeover = repl::parseElectionReasonCounter(element);
        }
        else if (fieldName == kCatchUpTakeoverFieldName) {
            if (MONGO_unlikely(usedFields[kCatchUpTakeoverBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kCatchUpTakeoverBit);

            _hasCatchUpTakeover = true;
            _catchUpTakeover = repl::parseElectionReasonCounter(element);
        }
        else if (fieldName == kElectionTimeoutFieldName) {
            if (MONGO_unlikely(usedFields[kElectionTimeoutBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kElectionTimeoutBit);

            _hasElectionTimeout = true;
            _electionTimeout = repl::parseElectionReasonCounter(element);
        }
        else if (fieldName == kFreezeTimeoutFieldName) {
            if (MONGO_unlikely(usedFields[kFreezeTimeoutBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kFreezeTimeoutBit);

            _hasFreezeTimeout = true;
            _freezeTimeout = repl::parseElectionReasonCounter(element);
        }
        else if (fieldName == kNumStepDownsCausedByHigherTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumStepDownsCausedByHigherTermBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumStepDownsCausedByHigherTermBit);

                _numStepDownsCausedByHigherTerm = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsBit);

                _numCatchUps = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsSucceededFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsSucceededBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsSucceededBit);

                _numCatchUpsSucceeded = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsAlreadyCaughtUpFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsAlreadyCaughtUpBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsAlreadyCaughtUpBit);

                _numCatchUpsAlreadyCaughtUp = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsSkippedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsSkippedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsSkippedBit);

                _numCatchUpsSkipped = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsTimedOutFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsTimedOutBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsTimedOutBit);

                _numCatchUpsTimedOut = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsFailedWithErrorFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsFailedWithErrorBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsFailedWithErrorBit);

                _numCatchUpsFailedWithError = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsFailedWithNewTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsFailedWithNewTermBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsFailedWithNewTermBit);

                _numCatchUpsFailedWithNewTerm = element._numberLong();
            }
        }
        else if (fieldName == kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdBit);

                _numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd = element._numberLong();
            }
        }
        else if (fieldName == kAverageCatchUpOpsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                if (MONGO_unlikely(usedFields[kAverageCatchUpOpsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAverageCatchUpOpsBit);

                _averageCatchUpOps = element._numberDouble();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kStepUpCmdBit]) {
            ctxt.throwMissingField(kStepUpCmdFieldName);
        }
        if (!usedFields[kPriorityTakeoverBit]) {
            ctxt.throwMissingField(kPriorityTakeoverFieldName);
        }
        if (!usedFields[kCatchUpTakeoverBit]) {
            ctxt.throwMissingField(kCatchUpTakeoverFieldName);
        }
        if (!usedFields[kElectionTimeoutBit]) {
            ctxt.throwMissingField(kElectionTimeoutFieldName);
        }
        if (!usedFields[kFreezeTimeoutBit]) {
            ctxt.throwMissingField(kFreezeTimeoutFieldName);
        }
        if (!usedFields[kNumStepDownsCausedByHigherTermBit]) {
            _numStepDownsCausedByHigherTerm = 0;
        }
        if (!usedFields[kNumCatchUpsBit]) {
            _numCatchUps = 0;
        }
        if (!usedFields[kNumCatchUpsSucceededBit]) {
            _numCatchUpsSucceeded = 0;
        }
        if (!usedFields[kNumCatchUpsAlreadyCaughtUpBit]) {
            _numCatchUpsAlreadyCaughtUp = 0;
        }
        if (!usedFields[kNumCatchUpsSkippedBit]) {
            _numCatchUpsSkipped = 0;
        }
        if (!usedFields[kNumCatchUpsTimedOutBit]) {
            _numCatchUpsTimedOut = 0;
        }
        if (!usedFields[kNumCatchUpsFailedWithErrorBit]) {
            _numCatchUpsFailedWithError = 0;
        }
        if (!usedFields[kNumCatchUpsFailedWithNewTermBit]) {
            _numCatchUpsFailedWithNewTerm = 0;
        }
        if (!usedFields[kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdBit]) {
            _numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd = 0;
        }
        if (!usedFields[kAverageCatchUpOpsBit]) {
            _averageCatchUpOps = 0.0;
        }
    }

}


void ElectionMetrics::serialize(BSONObjBuilder* builder) const {
    invariant(_hasStepUpCmd && _hasPriorityTakeover && _hasCatchUpTakeover && _hasElectionTimeout && _hasFreezeTimeout);

    {
        serializeElectionReasonCounterToBSON(_stepUpCmd, kStepUpCmdFieldName, builder);
    }

    {
        serializeElectionReasonCounterToBSON(_priorityTakeover, kPriorityTakeoverFieldName, builder);
    }

    {
        serializeElectionReasonCounterToBSON(_catchUpTakeover, kCatchUpTakeoverFieldName, builder);
    }

    {
        serializeElectionReasonCounterToBSON(_electionTimeout, kElectionTimeoutFieldName, builder);
    }

    {
        serializeElectionReasonCounterToBSON(_freezeTimeout, kFreezeTimeoutFieldName, builder);
    }

    builder->append(kNumStepDownsCausedByHigherTermFieldName, _numStepDownsCausedByHigherTerm);

    builder->append(kNumCatchUpsFieldName, _numCatchUps);

    builder->append(kNumCatchUpsSucceededFieldName, _numCatchUpsSucceeded);

    builder->append(kNumCatchUpsAlreadyCaughtUpFieldName, _numCatchUpsAlreadyCaughtUp);

    builder->append(kNumCatchUpsSkippedFieldName, _numCatchUpsSkipped);

    builder->append(kNumCatchUpsTimedOutFieldName, _numCatchUpsTimedOut);

    builder->append(kNumCatchUpsFailedWithErrorFieldName, _numCatchUpsFailedWithError);

    builder->append(kNumCatchUpsFailedWithNewTermFieldName, _numCatchUpsFailedWithNewTerm);

    builder->append(kNumCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmdFieldName, _numCatchUpsFailedWithReplSetAbortPrimaryCatchUpCmd);

    builder->append(kAverageCatchUpOpsFieldName, _averageCatchUpOps);

}


BSONObj ElectionMetrics::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ElectionCandidateMetrics::kElectionTermFieldName;
constexpr StringData ElectionCandidateMetrics::kElectionTimeoutMillisFieldName;
constexpr StringData ElectionCandidateMetrics::kLastCommittedOpTimeAtElectionFieldName;
constexpr StringData ElectionCandidateMetrics::kLastElectionDateFieldName;
constexpr StringData ElectionCandidateMetrics::kLastElectionReasonFieldName;
constexpr StringData ElectionCandidateMetrics::kLastSeenOpTimeAtElectionFieldName;
constexpr StringData ElectionCandidateMetrics::kNewTermStartDateFieldName;
constexpr StringData ElectionCandidateMetrics::kNumCatchUpOpsFieldName;
constexpr StringData ElectionCandidateMetrics::kNumVotesNeededFieldName;
constexpr StringData ElectionCandidateMetrics::kPriorPrimaryMemberIdFieldName;
constexpr StringData ElectionCandidateMetrics::kPriorityAtElectionFieldName;
constexpr StringData ElectionCandidateMetrics::kTargetCatchupOpTimeFieldName;
constexpr StringData ElectionCandidateMetrics::kWMajorityWriteAvailabilityDateFieldName;


ElectionCandidateMetrics::ElectionCandidateMetrics() : _electionTerm(-1), _numVotesNeeded(-1), _priorityAtElection(-1), _electionTimeoutMillis(-1), _hasLastElectionReason(false), _hasLastElectionDate(false), _hasElectionTerm(false), _hasLastCommittedOpTimeAtElection(false), _hasLastSeenOpTimeAtElection(false), _hasNumVotesNeeded(false), _hasPriorityAtElection(false), _hasElectionTimeoutMillis(false) {
    // Used for initialization only
}
ElectionCandidateMetrics::ElectionCandidateMetrics(mongo::repl::StartElectionReasonEnum lastElectionReason, mongo::Date_t lastElectionDate, std::int64_t electionTerm, mongo::repl::OpTime lastCommittedOpTimeAtElection, mongo::repl::OpTime lastSeenOpTimeAtElection, std::int32_t numVotesNeeded, double priorityAtElection, std::int64_t electionTimeoutMillis) : _lastElectionReason(std::move(lastElectionReason)), _lastElectionDate(std::move(lastElectionDate)), _electionTerm(std::move(electionTerm)), _lastCommittedOpTimeAtElection(std::move(lastCommittedOpTimeAtElection)), _lastSeenOpTimeAtElection(std::move(lastSeenOpTimeAtElection)), _numVotesNeeded(std::move(numVotesNeeded)), _priorityAtElection(std::move(priorityAtElection)), _electionTimeoutMillis(std::move(electionTimeoutMillis)), _hasLastElectionReason(true), _hasLastElectionDate(true), _hasElectionTerm(true), _hasLastCommittedOpTimeAtElection(true), _hasLastSeenOpTimeAtElection(true), _hasNumVotesNeeded(true), _hasPriorityAtElection(true), _hasElectionTimeoutMillis(true) {
    // Used for initialization only
}


ElectionCandidateMetrics ElectionCandidateMetrics::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ElectionCandidateMetrics object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ElectionCandidateMetrics::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<13> usedFields;
    const size_t kLastElectionReasonBit = 0;
    const size_t kLastElectionDateBit = 1;
    const size_t kElectionTermBit = 2;
    const size_t kLastCommittedOpTimeAtElectionBit = 3;
    const size_t kLastSeenOpTimeAtElectionBit = 4;
    const size_t kNumVotesNeededBit = 5;
    const size_t kPriorityAtElectionBit = 6;
    const size_t kElectionTimeoutMillisBit = 7;
    const size_t kPriorPrimaryMemberIdBit = 8;
    const size_t kTargetCatchupOpTimeBit = 9;
    const size_t kNumCatchUpOpsBit = 10;
    const size_t kNewTermStartDateBit = 11;
    const size_t kWMajorityWriteAvailabilityDateBit = 12;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kLastElectionReasonFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kLastElectionReasonBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastElectionReasonBit);

                _hasLastElectionReason = true;
                IDLParserErrorContext tempContext(kLastElectionReasonFieldName, &ctxt);
                _lastElectionReason = repl::StartElectionReason_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kLastElectionDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kLastElectionDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastElectionDateBit);

                _hasLastElectionDate = true;
                _lastElectionDate = element.date();
            }
        }
        else if (fieldName == kElectionTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kElectionTermBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kElectionTermBit);

                _hasElectionTerm = true;
                _electionTerm = element._numberLong();
            }
        }
        else if (fieldName == kLastCommittedOpTimeAtElectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLastCommittedOpTimeAtElectionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastCommittedOpTimeAtElectionBit);

                _hasLastCommittedOpTimeAtElection = true;
                const BSONObj localObject = element.Obj();
                _lastCommittedOpTimeAtElection = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kLastSeenOpTimeAtElectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLastSeenOpTimeAtElectionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastSeenOpTimeAtElectionBit);

                _hasLastSeenOpTimeAtElection = true;
                const BSONObj localObject = element.Obj();
                _lastSeenOpTimeAtElection = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kNumVotesNeededFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kNumVotesNeededBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumVotesNeededBit);

                _hasNumVotesNeeded = true;
                _numVotesNeeded = element._numberInt();
            }
        }
        else if (fieldName == kPriorityAtElectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                if (MONGO_unlikely(usedFields[kPriorityAtElectionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPriorityAtElectionBit);

                _hasPriorityAtElection = true;
                _priorityAtElection = element._numberDouble();
            }
        }
        else if (fieldName == kElectionTimeoutMillisFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kElectionTimeoutMillisBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kElectionTimeoutMillisBit);

                _hasElectionTimeoutMillis = true;
                _electionTimeoutMillis = element._numberLong();
            }
        }
        else if (fieldName == kPriorPrimaryMemberIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kPriorPrimaryMemberIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPriorPrimaryMemberIdBit);

                _priorPrimaryMemberId = element._numberInt();
            }
        }
        else if (fieldName == kTargetCatchupOpTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kTargetCatchupOpTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTargetCatchupOpTimeBit);

                const BSONObj localObject = element.Obj();
                _targetCatchupOpTime = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kNumCatchUpOpsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumCatchUpOpsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumCatchUpOpsBit);

                _numCatchUpOps = element._numberLong();
            }
        }
        else if (fieldName == kNewTermStartDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kNewTermStartDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNewTermStartDateBit);

                _newTermStartDate = element.date();
            }
        }
        else if (fieldName == kWMajorityWriteAvailabilityDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kWMajorityWriteAvailabilityDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWMajorityWriteAvailabilityDateBit);

                _wMajorityWriteAvailabilityDate = element.date();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kLastElectionReasonBit]) {
            ctxt.throwMissingField(kLastElectionReasonFieldName);
        }
        if (!usedFields[kLastElectionDateBit]) {
            ctxt.throwMissingField(kLastElectionDateFieldName);
        }
        if (!usedFields[kElectionTermBit]) {
            ctxt.throwMissingField(kElectionTermFieldName);
        }
        if (!usedFields[kLastCommittedOpTimeAtElectionBit]) {
            ctxt.throwMissingField(kLastCommittedOpTimeAtElectionFieldName);
        }
        if (!usedFields[kLastSeenOpTimeAtElectionBit]) {
            ctxt.throwMissingField(kLastSeenOpTimeAtElectionFieldName);
        }
        if (!usedFields[kNumVotesNeededBit]) {
            ctxt.throwMissingField(kNumVotesNeededFieldName);
        }
        if (!usedFields[kPriorityAtElectionBit]) {
            ctxt.throwMissingField(kPriorityAtElectionFieldName);
        }
        if (!usedFields[kElectionTimeoutMillisBit]) {
            ctxt.throwMissingField(kElectionTimeoutMillisFieldName);
        }
    }

}


void ElectionCandidateMetrics::serialize(BSONObjBuilder* builder) const {
    invariant(_hasLastElectionReason && _hasLastElectionDate && _hasElectionTerm && _hasLastCommittedOpTimeAtElection && _hasLastSeenOpTimeAtElection && _hasNumVotesNeeded && _hasPriorityAtElection && _hasElectionTimeoutMillis);

    {
        builder->append(kLastElectionReasonFieldName, StartElectionReason_serializer(_lastElectionReason));
    }

    builder->append(kLastElectionDateFieldName, _lastElectionDate);

    builder->append(kElectionTermFieldName, _electionTerm);

    {
        const BSONObj localObject = _lastCommittedOpTimeAtElection.toBSON();
        builder->append(kLastCommittedOpTimeAtElectionFieldName, localObject);
    }

    {
        const BSONObj localObject = _lastSeenOpTimeAtElection.toBSON();
        builder->append(kLastSeenOpTimeAtElectionFieldName, localObject);
    }

    builder->append(kNumVotesNeededFieldName, _numVotesNeeded);

    builder->append(kPriorityAtElectionFieldName, _priorityAtElection);

    builder->append(kElectionTimeoutMillisFieldName, _electionTimeoutMillis);

    if (_priorPrimaryMemberId.is_initialized()) {
        builder->append(kPriorPrimaryMemberIdFieldName, _priorPrimaryMemberId.get());
    }

    if (_targetCatchupOpTime.is_initialized()) {
        const BSONObj localObject = _targetCatchupOpTime.get().toBSON();
        builder->append(kTargetCatchupOpTimeFieldName, localObject);
    }

    if (_numCatchUpOps.is_initialized()) {
        builder->append(kNumCatchUpOpsFieldName, _numCatchUpOps.get());
    }

    if (_newTermStartDate.is_initialized()) {
        builder->append(kNewTermStartDateFieldName, _newTermStartDate.get());
    }

    if (_wMajorityWriteAvailabilityDate.is_initialized()) {
        builder->append(kWMajorityWriteAvailabilityDateFieldName, _wMajorityWriteAvailabilityDate.get());
    }

}


BSONObj ElectionCandidateMetrics::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ElectionParticipantMetrics::kElectionCandidateMemberIdFieldName;
constexpr StringData ElectionParticipantMetrics::kElectionTermFieldName;
constexpr StringData ElectionParticipantMetrics::kLastAppliedOpTimeAtElectionFieldName;
constexpr StringData ElectionParticipantMetrics::kLastVoteDateFieldName;
constexpr StringData ElectionParticipantMetrics::kMaxAppliedOpTimeInSetFieldName;
constexpr StringData ElectionParticipantMetrics::kNewTermAppliedDateFieldName;
constexpr StringData ElectionParticipantMetrics::kNewTermStartDateFieldName;
constexpr StringData ElectionParticipantMetrics::kPriorityAtElectionFieldName;
constexpr StringData ElectionParticipantMetrics::kVoteReasonFieldName;
constexpr StringData ElectionParticipantMetrics::kVotedForCandidateFieldName;


ElectionParticipantMetrics::ElectionParticipantMetrics() : _votedForCandidate(false), _electionTerm(-1), _electionCandidateMemberId(-1), _priorityAtElection(-1), _hasVotedForCandidate(false), _hasElectionTerm(false), _hasLastVoteDate(false), _hasElectionCandidateMemberId(false), _hasVoteReason(false), _hasLastAppliedOpTimeAtElection(false), _hasMaxAppliedOpTimeInSet(false), _hasPriorityAtElection(false) {
    // Used for initialization only
}
ElectionParticipantMetrics::ElectionParticipantMetrics(bool votedForCandidate, std::int64_t electionTerm, mongo::Date_t lastVoteDate, std::int32_t electionCandidateMemberId, std::string voteReason, mongo::repl::OpTime lastAppliedOpTimeAtElection, mongo::repl::OpTime maxAppliedOpTimeInSet, double priorityAtElection) : _votedForCandidate(std::move(votedForCandidate)), _electionTerm(std::move(electionTerm)), _lastVoteDate(std::move(lastVoteDate)), _electionCandidateMemberId(std::move(electionCandidateMemberId)), _voteReason(std::move(voteReason)), _lastAppliedOpTimeAtElection(std::move(lastAppliedOpTimeAtElection)), _maxAppliedOpTimeInSet(std::move(maxAppliedOpTimeInSet)), _priorityAtElection(std::move(priorityAtElection)), _hasVotedForCandidate(true), _hasElectionTerm(true), _hasLastVoteDate(true), _hasElectionCandidateMemberId(true), _hasVoteReason(true), _hasLastAppliedOpTimeAtElection(true), _hasMaxAppliedOpTimeInSet(true), _hasPriorityAtElection(true) {
    // Used for initialization only
}


ElectionParticipantMetrics ElectionParticipantMetrics::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ElectionParticipantMetrics object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ElectionParticipantMetrics::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<10> usedFields;
    const size_t kVotedForCandidateBit = 0;
    const size_t kElectionTermBit = 1;
    const size_t kLastVoteDateBit = 2;
    const size_t kElectionCandidateMemberIdBit = 3;
    const size_t kVoteReasonBit = 4;
    const size_t kLastAppliedOpTimeAtElectionBit = 5;
    const size_t kMaxAppliedOpTimeInSetBit = 6;
    const size_t kPriorityAtElectionBit = 7;
    const size_t kNewTermStartDateBit = 8;
    const size_t kNewTermAppliedDateBit = 9;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kVotedForCandidateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kVotedForCandidateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVotedForCandidateBit);

                _hasVotedForCandidate = true;
                _votedForCandidate = element.boolean();
            }
        }
        else if (fieldName == kElectionTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kElectionTermBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kElectionTermBit);

                _hasElectionTerm = true;
                _electionTerm = element._numberLong();
            }
        }
        else if (fieldName == kLastVoteDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kLastVoteDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastVoteDateBit);

                _hasLastVoteDate = true;
                _lastVoteDate = element.date();
            }
        }
        else if (fieldName == kElectionCandidateMemberIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kElectionCandidateMemberIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kElectionCandidateMemberIdBit);

                _hasElectionCandidateMemberId = true;
                _electionCandidateMemberId = element._numberInt();
            }
        }
        else if (fieldName == kVoteReasonFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kVoteReasonBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVoteReasonBit);

                _hasVoteReason = true;
                _voteReason = element.str();
            }
        }
        else if (fieldName == kLastAppliedOpTimeAtElectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLastAppliedOpTimeAtElectionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastAppliedOpTimeAtElectionBit);

                _hasLastAppliedOpTimeAtElection = true;
                const BSONObj localObject = element.Obj();
                _lastAppliedOpTimeAtElection = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kMaxAppliedOpTimeInSetFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kMaxAppliedOpTimeInSetBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxAppliedOpTimeInSetBit);

                _hasMaxAppliedOpTimeInSet = true;
                const BSONObj localObject = element.Obj();
                _maxAppliedOpTimeInSet = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kPriorityAtElectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                if (MONGO_unlikely(usedFields[kPriorityAtElectionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPriorityAtElectionBit);

                _hasPriorityAtElection = true;
                _priorityAtElection = element._numberDouble();
            }
        }
        else if (fieldName == kNewTermStartDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kNewTermStartDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNewTermStartDateBit);

                _newTermStartDate = element.date();
            }
        }
        else if (fieldName == kNewTermAppliedDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kNewTermAppliedDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNewTermAppliedDateBit);

                _newTermAppliedDate = element.date();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kVotedForCandidateBit]) {
            ctxt.throwMissingField(kVotedForCandidateFieldName);
        }
        if (!usedFields[kElectionTermBit]) {
            ctxt.throwMissingField(kElectionTermFieldName);
        }
        if (!usedFields[kLastVoteDateBit]) {
            ctxt.throwMissingField(kLastVoteDateFieldName);
        }
        if (!usedFields[kElectionCandidateMemberIdBit]) {
            ctxt.throwMissingField(kElectionCandidateMemberIdFieldName);
        }
        if (!usedFields[kVoteReasonBit]) {
            ctxt.throwMissingField(kVoteReasonFieldName);
        }
        if (!usedFields[kLastAppliedOpTimeAtElectionBit]) {
            ctxt.throwMissingField(kLastAppliedOpTimeAtElectionFieldName);
        }
        if (!usedFields[kMaxAppliedOpTimeInSetBit]) {
            ctxt.throwMissingField(kMaxAppliedOpTimeInSetFieldName);
        }
        if (!usedFields[kPriorityAtElectionBit]) {
            ctxt.throwMissingField(kPriorityAtElectionFieldName);
        }
    }

}


void ElectionParticipantMetrics::serialize(BSONObjBuilder* builder) const {
    invariant(_hasVotedForCandidate && _hasElectionTerm && _hasLastVoteDate && _hasElectionCandidateMemberId && _hasVoteReason && _hasLastAppliedOpTimeAtElection && _hasMaxAppliedOpTimeInSet && _hasPriorityAtElection);

    builder->append(kVotedForCandidateFieldName, _votedForCandidate);

    builder->append(kElectionTermFieldName, _electionTerm);

    builder->append(kLastVoteDateFieldName, _lastVoteDate);

    builder->append(kElectionCandidateMemberIdFieldName, _electionCandidateMemberId);

    builder->append(kVoteReasonFieldName, _voteReason);

    {
        const BSONObj localObject = _lastAppliedOpTimeAtElection.toBSON();
        builder->append(kLastAppliedOpTimeAtElectionFieldName, localObject);
    }

    {
        const BSONObj localObject = _maxAppliedOpTimeInSet.toBSON();
        builder->append(kMaxAppliedOpTimeInSetFieldName, localObject);
    }

    builder->append(kPriorityAtElectionFieldName, _priorityAtElection);

    if (_newTermStartDate.is_initialized()) {
        builder->append(kNewTermStartDateFieldName, _newTermStartDate.get());
    }

    if (_newTermAppliedDate.is_initialized()) {
        builder->append(kNewTermAppliedDateFieldName, _newTermAppliedDate.get());
    }

}


BSONObj ElectionParticipantMetrics::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
