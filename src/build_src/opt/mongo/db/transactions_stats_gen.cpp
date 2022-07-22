/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/transactions_stats_gen.h --output build/opt/mongo/db/transactions_stats_gen.cpp src/mongo/db/transactions_stats.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/transactions_stats_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData LastCommittedTransaction::kOperationCountFieldName;
constexpr StringData LastCommittedTransaction::kOplogOperationBytesFieldName;
constexpr StringData LastCommittedTransaction::kWriteConcernFieldName;


LastCommittedTransaction::LastCommittedTransaction() : _hasWriteConcern(false) {
    // Used for initialization only
}
LastCommittedTransaction::LastCommittedTransaction(mongo::BSONObj writeConcern) : _writeConcern(std::move(writeConcern)), _hasWriteConcern(true) {
    // Used for initialization only
}


LastCommittedTransaction LastCommittedTransaction::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    LastCommittedTransaction object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void LastCommittedTransaction::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kOperationCountBit = 0;
    const size_t kOplogOperationBytesBit = 1;
    const size_t kWriteConcernBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kOperationCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kOperationCountBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOperationCountBit);

                _operationCount = element._numberLong();
            }
        }
        else if (fieldName == kOplogOperationBytesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kOplogOperationBytesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOplogOperationBytesBit);

                _oplogOperationBytes = element._numberLong();
            }
        }
        else if (fieldName == kWriteConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kWriteConcernBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWriteConcernBit);

                _hasWriteConcern = true;
                _writeConcern = element.Obj();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kOperationCountBit]) {
            _operationCount = 0;
        }
        if (!usedFields[kOplogOperationBytesBit]) {
            _oplogOperationBytes = 0;
        }
        if (!usedFields[kWriteConcernBit]) {
            ctxt.throwMissingField(kWriteConcernFieldName);
        }
    }

}


void LastCommittedTransaction::serialize(BSONObjBuilder* builder) const {
    invariant(_hasWriteConcern);

    builder->append(kOperationCountFieldName, _operationCount);

    builder->append(kOplogOperationBytesFieldName, _oplogOperationBytes);

    builder->append(kWriteConcernFieldName, _writeConcern);

}


BSONObj LastCommittedTransaction::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData TransactionsStats::kCurrentActiveFieldName;
constexpr StringData TransactionsStats::kCurrentInactiveFieldName;
constexpr StringData TransactionsStats::kCurrentOpenFieldName;
constexpr StringData TransactionsStats::kCurrentPreparedFieldName;
constexpr StringData TransactionsStats::kLastCommittedTransactionFieldName;
constexpr StringData TransactionsStats::kRetriedCommandsCountFieldName;
constexpr StringData TransactionsStats::kRetriedStatementsCountFieldName;
constexpr StringData TransactionsStats::kTotalAbortedFieldName;
constexpr StringData TransactionsStats::kTotalCommittedFieldName;
constexpr StringData TransactionsStats::kTotalPreparedFieldName;
constexpr StringData TransactionsStats::kTotalPreparedThenAbortedFieldName;
constexpr StringData TransactionsStats::kTotalPreparedThenCommittedFieldName;
constexpr StringData TransactionsStats::kTotalStartedFieldName;
constexpr StringData TransactionsStats::kTransactionsCollectionWriteCountFieldName;


TransactionsStats::TransactionsStats()  {
    // Used for initialization only
}


TransactionsStats TransactionsStats::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TransactionsStats object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TransactionsStats::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<14> usedFields;
    const size_t kRetriedCommandsCountBit = 0;
    const size_t kRetriedStatementsCountBit = 1;
    const size_t kTransactionsCollectionWriteCountBit = 2;
    const size_t kCurrentActiveBit = 3;
    const size_t kCurrentInactiveBit = 4;
    const size_t kCurrentOpenBit = 5;
    const size_t kTotalAbortedBit = 6;
    const size_t kTotalCommittedBit = 7;
    const size_t kTotalStartedBit = 8;
    const size_t kTotalPreparedBit = 9;
    const size_t kTotalPreparedThenCommittedBit = 10;
    const size_t kTotalPreparedThenAbortedBit = 11;
    const size_t kCurrentPreparedBit = 12;
    const size_t kLastCommittedTransactionBit = 13;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kRetriedCommandsCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kRetriedCommandsCountBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRetriedCommandsCountBit);

                _retriedCommandsCount = element._numberLong();
            }
        }
        else if (fieldName == kRetriedStatementsCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kRetriedStatementsCountBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRetriedStatementsCountBit);

                _retriedStatementsCount = element._numberLong();
            }
        }
        else if (fieldName == kTransactionsCollectionWriteCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTransactionsCollectionWriteCountBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTransactionsCollectionWriteCountBit);

                _transactionsCollectionWriteCount = element._numberLong();
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
        else if (fieldName == kCurrentOpenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCurrentOpenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentOpenBit);

                _currentOpen = element._numberLong();
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
        else if (fieldName == kTotalCommittedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalCommittedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalCommittedBit);

                _totalCommitted = element._numberLong();
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
        else if (fieldName == kTotalPreparedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalPreparedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalPreparedBit);

                _totalPrepared = element._numberLong();
            }
        }
        else if (fieldName == kTotalPreparedThenCommittedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalPreparedThenCommittedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalPreparedThenCommittedBit);

                _totalPreparedThenCommitted = element._numberLong();
            }
        }
        else if (fieldName == kTotalPreparedThenAbortedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTotalPreparedThenAbortedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTotalPreparedThenAbortedBit);

                _totalPreparedThenAborted = element._numberLong();
            }
        }
        else if (fieldName == kCurrentPreparedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCurrentPreparedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentPreparedBit);

                _currentPrepared = element._numberLong();
            }
        }
        else if (fieldName == kLastCommittedTransactionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLastCommittedTransactionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastCommittedTransactionBit);

                IDLParserErrorContext tempContext(kLastCommittedTransactionFieldName, &ctxt);
                const auto localObject = element.Obj();
                _lastCommittedTransaction = mongo::LastCommittedTransaction::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kRetriedCommandsCountBit]) {
            _retriedCommandsCount = 0;
        }
        if (!usedFields[kRetriedStatementsCountBit]) {
            _retriedStatementsCount = 0;
        }
        if (!usedFields[kTransactionsCollectionWriteCountBit]) {
            _transactionsCollectionWriteCount = 0;
        }
        if (!usedFields[kCurrentActiveBit]) {
            _currentActive = 0;
        }
        if (!usedFields[kCurrentInactiveBit]) {
            _currentInactive = 0;
        }
        if (!usedFields[kCurrentOpenBit]) {
            _currentOpen = 0;
        }
        if (!usedFields[kTotalAbortedBit]) {
            _totalAborted = 0;
        }
        if (!usedFields[kTotalCommittedBit]) {
            _totalCommitted = 0;
        }
        if (!usedFields[kTotalStartedBit]) {
            _totalStarted = 0;
        }
        if (!usedFields[kTotalPreparedBit]) {
            _totalPrepared = 0;
        }
        if (!usedFields[kTotalPreparedThenCommittedBit]) {
            _totalPreparedThenCommitted = 0;
        }
        if (!usedFields[kTotalPreparedThenAbortedBit]) {
            _totalPreparedThenAborted = 0;
        }
        if (!usedFields[kCurrentPreparedBit]) {
            _currentPrepared = 0;
        }
    }

}


void TransactionsStats::serialize(BSONObjBuilder* builder) const {
    builder->append(kRetriedCommandsCountFieldName, _retriedCommandsCount);

    builder->append(kRetriedStatementsCountFieldName, _retriedStatementsCount);

    builder->append(kTransactionsCollectionWriteCountFieldName, _transactionsCollectionWriteCount);

    builder->append(kCurrentActiveFieldName, _currentActive);

    builder->append(kCurrentInactiveFieldName, _currentInactive);

    builder->append(kCurrentOpenFieldName, _currentOpen);

    builder->append(kTotalAbortedFieldName, _totalAborted);

    builder->append(kTotalCommittedFieldName, _totalCommitted);

    builder->append(kTotalStartedFieldName, _totalStarted);

    builder->append(kTotalPreparedFieldName, _totalPrepared);

    builder->append(kTotalPreparedThenCommittedFieldName, _totalPreparedThenCommitted);

    builder->append(kTotalPreparedThenAbortedFieldName, _totalPreparedThenAborted);

    builder->append(kCurrentPreparedFieldName, _currentPrepared);

    if (_lastCommittedTransaction.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kLastCommittedTransactionFieldName));
        _lastCommittedTransaction.get().serialize(&subObjBuilder);
    }

}


BSONObj TransactionsStats::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
