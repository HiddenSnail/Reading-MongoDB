/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/router_transactions_stats_gen.h --output build/opt/mongo/s/router_transactions_stats_gen.cpp src/mongo/s/router_transactions_stats.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/router_transactions_stats_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData CommitTypeStats::kInitiatedFieldName;
constexpr StringData CommitTypeStats::kSuccessfulFieldName;
constexpr StringData CommitTypeStats::kSuccessfulDurationMicrosFieldName;


CommitTypeStats::CommitTypeStats()  {
    // Used for initialization only
}


CommitTypeStats CommitTypeStats::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CommitTypeStats object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CommitTypeStats::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kInitiatedBit = 0;
    const size_t kSuccessfulBit = 1;
    const size_t kSuccessfulDurationMicrosBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kInitiatedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kInitiatedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kInitiatedBit);

                _initiated = element._numberLong();
            }
        }
        else if (fieldName == kSuccessfulFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kSuccessfulBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSuccessfulBit);

                _successful = element._numberLong();
            }
        }
        else if (fieldName == kSuccessfulDurationMicrosFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kSuccessfulDurationMicrosBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSuccessfulDurationMicrosBit);

                _successfulDurationMicros = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kInitiatedBit]) {
            _initiated = 0;
        }
        if (!usedFields[kSuccessfulBit]) {
            _successful = 0;
        }
        if (!usedFields[kSuccessfulDurationMicrosBit]) {
            _successfulDurationMicros = 0;
        }
    }

}


void CommitTypeStats::serialize(BSONObjBuilder* builder) const {
    builder->append(kInitiatedFieldName, _initiated);

    builder->append(kSuccessfulFieldName, _successful);

    builder->append(kSuccessfulDurationMicrosFieldName, _successfulDurationMicros);

}


BSONObj CommitTypeStats::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData CommitTypes::kNoShardsFieldName;
constexpr StringData CommitTypes::kReadOnlyFieldName;
constexpr StringData CommitTypes::kRecoverWithTokenFieldName;
constexpr StringData CommitTypes::kSingleShardFieldName;
constexpr StringData CommitTypes::kSingleWriteShardFieldName;
constexpr StringData CommitTypes::kTwoPhaseCommitFieldName;


CommitTypes::CommitTypes() : _hasNoShards(false), _hasSingleShard(false), _hasSingleWriteShard(false), _hasReadOnly(false), _hasTwoPhaseCommit(false), _hasRecoverWithToken(false) {
    // Used for initialization only
}
CommitTypes::CommitTypes(mongo::CommitTypeStats noShards, mongo::CommitTypeStats singleShard, mongo::CommitTypeStats singleWriteShard, mongo::CommitTypeStats readOnly, mongo::CommitTypeStats twoPhaseCommit, mongo::CommitTypeStats recoverWithToken) : _noShards(std::move(noShards)), _singleShard(std::move(singleShard)), _singleWriteShard(std::move(singleWriteShard)), _readOnly(std::move(readOnly)), _twoPhaseCommit(std::move(twoPhaseCommit)), _recoverWithToken(std::move(recoverWithToken)), _hasNoShards(true), _hasSingleShard(true), _hasSingleWriteShard(true), _hasReadOnly(true), _hasTwoPhaseCommit(true), _hasRecoverWithToken(true) {
    // Used for initialization only
}


CommitTypes CommitTypes::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CommitTypes object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CommitTypes::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kNoShardsBit = 0;
    const size_t kSingleShardBit = 1;
    const size_t kSingleWriteShardBit = 2;
    const size_t kReadOnlyBit = 3;
    const size_t kTwoPhaseCommitBit = 4;
    const size_t kRecoverWithTokenBit = 5;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kNoShardsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kNoShardsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNoShardsBit);

                _hasNoShards = true;
                IDLParserErrorContext tempContext(kNoShardsFieldName, &ctxt);
                const auto localObject = element.Obj();
                _noShards = mongo::CommitTypeStats::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kSingleShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSingleShardBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSingleShardBit);

                _hasSingleShard = true;
                IDLParserErrorContext tempContext(kSingleShardFieldName, &ctxt);
                const auto localObject = element.Obj();
                _singleShard = mongo::CommitTypeStats::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kSingleWriteShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSingleWriteShardBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSingleWriteShardBit);

                _hasSingleWriteShard = true;
                IDLParserErrorContext tempContext(kSingleWriteShardFieldName, &ctxt);
                const auto localObject = element.Obj();
                _singleWriteShard = mongo::CommitTypeStats::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kReadOnlyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kReadOnlyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kReadOnlyBit);

                _hasReadOnly = true;
                IDLParserErrorContext tempContext(kReadOnlyFieldName, &ctxt);
                const auto localObject = element.Obj();
                _readOnly = mongo::CommitTypeStats::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kTwoPhaseCommitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kTwoPhaseCommitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTwoPhaseCommitBit);

                _hasTwoPhaseCommit = true;
                IDLParserErrorContext tempContext(kTwoPhaseCommitFieldName, &ctxt);
                const auto localObject = element.Obj();
                _twoPhaseCommit = mongo::CommitTypeStats::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kRecoverWithTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kRecoverWithTokenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecoverWithTokenBit);

                _hasRecoverWithToken = true;
                IDLParserErrorContext tempContext(kRecoverWithTokenFieldName, &ctxt);
                const auto localObject = element.Obj();
                _recoverWithToken = mongo::CommitTypeStats::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kNoShardsBit]) {
            ctxt.throwMissingField(kNoShardsFieldName);
        }
        if (!usedFields[kSingleShardBit]) {
            ctxt.throwMissingField(kSingleShardFieldName);
        }
        if (!usedFields[kSingleWriteShardBit]) {
            ctxt.throwMissingField(kSingleWriteShardFieldName);
        }
        if (!usedFields[kReadOnlyBit]) {
            ctxt.throwMissingField(kReadOnlyFieldName);
        }
        if (!usedFields[kTwoPhaseCommitBit]) {
            ctxt.throwMissingField(kTwoPhaseCommitFieldName);
        }
        if (!usedFields[kRecoverWithTokenBit]) {
            ctxt.throwMissingField(kRecoverWithTokenFieldName);
        }
    }

}


void CommitTypes::serialize(BSONObjBuilder* builder) const {
    invariant(_hasNoShards && _hasSingleShard && _hasSingleWriteShard && _hasReadOnly && _hasTwoPhaseCommit && _hasRecoverWithToken);

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kNoShardsFieldName));
        _noShards.serialize(&subObjBuilder);
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kSingleShardFieldName));
        _singleShard.serialize(&subObjBuilder);
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kSingleWriteShardFieldName));
        _singleWriteShard.serialize(&subObjBuilder);
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kReadOnlyFieldName));
        _readOnly.serialize(&subObjBuilder);
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kTwoPhaseCommitFieldName));
        _twoPhaseCommit.serialize(&subObjBuilder);
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kRecoverWithTokenFieldName));
        _recoverWithToken.serialize(&subObjBuilder);
    }

}


BSONObj CommitTypes::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData RouterTransactionsStats::kAbortCauseFieldName;
constexpr StringData RouterTransactionsStats::kCommitTypesFieldName;
constexpr StringData RouterTransactionsStats::kCurrentActiveFieldName;
constexpr StringData RouterTransactionsStats::kCurrentInactiveFieldName;
constexpr StringData RouterTransactionsStats::kCurrentOpenFieldName;
constexpr StringData RouterTransactionsStats::kTotalAbortedFieldName;
constexpr StringData RouterTransactionsStats::kTotalCommittedFieldName;
constexpr StringData RouterTransactionsStats::kTotalContactedParticipantsFieldName;
constexpr StringData RouterTransactionsStats::kTotalParticipantsAtCommitFieldName;
constexpr StringData RouterTransactionsStats::kTotalRequestsTargetedFieldName;
constexpr StringData RouterTransactionsStats::kTotalStartedFieldName;


RouterTransactionsStats::RouterTransactionsStats() : _hasAbortCause(false), _hasCommitTypes(false) {
    // Used for initialization only
}
RouterTransactionsStats::RouterTransactionsStats(mongo::BSONObj abortCause, mongo::CommitTypes commitTypes) : _abortCause(std::move(abortCause)), _commitTypes(std::move(commitTypes)), _hasAbortCause(true), _hasCommitTypes(true) {
    // Used for initialization only
}


RouterTransactionsStats RouterTransactionsStats::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RouterTransactionsStats object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RouterTransactionsStats::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<11> usedFields;
    const size_t kCurrentOpenBit = 0;
    const size_t kCurrentActiveBit = 1;
    const size_t kCurrentInactiveBit = 2;
    const size_t kTotalStartedBit = 3;
    const size_t kTotalCommittedBit = 4;
    const size_t kTotalAbortedBit = 5;
    const size_t kAbortCauseBit = 6;
    const size_t kTotalContactedParticipantsBit = 7;
    const size_t kTotalParticipantsAtCommitBit = 8;
    const size_t kTotalRequestsTargetedBit = 9;
    const size_t kCommitTypesBit = 10;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kCurrentOpenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCurrentOpenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentOpenBit);

                _currentOpen = element._numberLong();
            }
        }
        else if (fieldName == kCurrentActiveFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCurrentActiveBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentActiveBit);

                _currentActive = element._numberLong();
            }
        }
        else if (fieldName == kCurrentInactiveFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCurrentInactiveBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentInactiveBit);

                _currentInactive = element._numberLong();
            }
        }
        else if (fieldName == kTotalStartedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalStartedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalStartedBit);

                _totalStarted = element._numberLong();
            }
        }
        else if (fieldName == kTotalCommittedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalCommittedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalCommittedBit);

                _totalCommitted = element._numberLong();
            }
        }
        else if (fieldName == kTotalAbortedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalAbortedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalAbortedBit);

                _totalAborted = element._numberLong();
            }
        }
        else if (fieldName == kAbortCauseFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kAbortCauseBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAbortCauseBit);

                _hasAbortCause = true;
                _abortCause = element.Obj();
            }
        }
        else if (fieldName == kTotalContactedParticipantsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalContactedParticipantsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalContactedParticipantsBit);

                _totalContactedParticipants = element._numberLong();
            }
        }
        else if (fieldName == kTotalParticipantsAtCommitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalParticipantsAtCommitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalParticipantsAtCommitBit);

                _totalParticipantsAtCommit = element._numberLong();
            }
        }
        else if (fieldName == kTotalRequestsTargetedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalRequestsTargetedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalRequestsTargetedBit);

                _totalRequestsTargeted = element._numberLong();
            }
        }
        else if (fieldName == kCommitTypesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCommitTypesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommitTypesBit);

                _hasCommitTypes = true;
                IDLParserErrorContext tempContext(kCommitTypesFieldName, &ctxt);
                const auto localObject = element.Obj();
                _commitTypes = mongo::CommitTypes::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kCurrentOpenBit]) {
            _currentOpen = 0;
        }
        if (!usedFields[kCurrentActiveBit]) {
            _currentActive = 0;
        }
        if (!usedFields[kCurrentInactiveBit]) {
            _currentInactive = 0;
        }
        if (!usedFields[kTotalStartedBit]) {
            _totalStarted = 0;
        }
        if (!usedFields[kTotalCommittedBit]) {
            _totalCommitted = 0;
        }
        if (!usedFields[kTotalAbortedBit]) {
            _totalAborted = 0;
        }
        if (!usedFields[kAbortCauseBit]) {
            ctxt.throwMissingField(kAbortCauseFieldName);
        }
        if (!usedFields[kTotalContactedParticipantsBit]) {
            _totalContactedParticipants = 0;
        }
        if (!usedFields[kTotalParticipantsAtCommitBit]) {
            _totalParticipantsAtCommit = 0;
        }
        if (!usedFields[kTotalRequestsTargetedBit]) {
            _totalRequestsTargeted = 0;
        }
        if (!usedFields[kCommitTypesBit]) {
            ctxt.throwMissingField(kCommitTypesFieldName);
        }
    }

}


void RouterTransactionsStats::serialize(BSONObjBuilder* builder) const {
    invariant(_hasAbortCause && _hasCommitTypes);

    builder->append(kCurrentOpenFieldName, _currentOpen);

    builder->append(kCurrentActiveFieldName, _currentActive);

    builder->append(kCurrentInactiveFieldName, _currentInactive);

    builder->append(kTotalStartedFieldName, _totalStarted);

    builder->append(kTotalCommittedFieldName, _totalCommitted);

    builder->append(kTotalAbortedFieldName, _totalAborted);

    builder->append(kAbortCauseFieldName, _abortCause);

    builder->append(kTotalContactedParticipantsFieldName, _totalContactedParticipants);

    builder->append(kTotalParticipantsAtCommitFieldName, _totalParticipantsAtCommit);

    builder->append(kTotalRequestsTargetedFieldName, _totalRequestsTargeted);

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kCommitTypesFieldName));
        _commitTypes.serialize(&subObjBuilder);
    }

}


BSONObj RouterTransactionsStats::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
