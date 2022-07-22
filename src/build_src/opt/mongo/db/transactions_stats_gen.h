/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/transactions_stats_gen.h --output build/opt/mongo/db/transactions_stats_gen.cpp src/mongo/db/transactions_stats.idl
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
#include "mongo/db/repl/optime.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing the server status subsection on the last committed transaction
 */
class LastCommittedTransaction {
public:
    static constexpr auto kOperationCountFieldName = "operationCount"_sd;
    static constexpr auto kOplogOperationBytesFieldName = "oplogOperationBytes"_sd;
    static constexpr auto kWriteConcernFieldName = "writeConcern"_sd;

    LastCommittedTransaction();
    LastCommittedTransaction(mongo::BSONObj writeConcern);

    static LastCommittedTransaction parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getOperationCount() const { return _operationCount; }
    void setOperationCount(std::int64_t value) & {  _operationCount = std::move(value);  }

    std::int64_t getOplogOperationBytes() const { return _oplogOperationBytes; }
    void setOplogOperationBytes(std::int64_t value) & {  _oplogOperationBytes = std::move(value);  }

    const mongo::BSONObj& getWriteConcern() const { return _writeConcern; }
    void setWriteConcern(mongo::BSONObj value) & {  _writeConcern = std::move(value); _hasWriteConcern = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _operationCount{0};
    std::int64_t _oplogOperationBytes{0};
    mongo::BSONObj _writeConcern;
    bool _hasWriteConcern : 1;
};

/**
 * A struct representing the section of the server status command with information about transactions
 */
class TransactionsStats {
public:
    static constexpr auto kCurrentActiveFieldName = "currentActive"_sd;
    static constexpr auto kCurrentInactiveFieldName = "currentInactive"_sd;
    static constexpr auto kCurrentOpenFieldName = "currentOpen"_sd;
    static constexpr auto kCurrentPreparedFieldName = "currentPrepared"_sd;
    static constexpr auto kLastCommittedTransactionFieldName = "lastCommittedTransaction"_sd;
    static constexpr auto kRetriedCommandsCountFieldName = "retriedCommandsCount"_sd;
    static constexpr auto kRetriedStatementsCountFieldName = "retriedStatementsCount"_sd;
    static constexpr auto kTotalAbortedFieldName = "totalAborted"_sd;
    static constexpr auto kTotalCommittedFieldName = "totalCommitted"_sd;
    static constexpr auto kTotalPreparedFieldName = "totalPrepared"_sd;
    static constexpr auto kTotalPreparedThenAbortedFieldName = "totalPreparedThenAborted"_sd;
    static constexpr auto kTotalPreparedThenCommittedFieldName = "totalPreparedThenCommitted"_sd;
    static constexpr auto kTotalStartedFieldName = "totalStarted"_sd;
    static constexpr auto kTransactionsCollectionWriteCountFieldName = "transactionsCollectionWriteCount"_sd;

    TransactionsStats();

    static TransactionsStats parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getRetriedCommandsCount() const { return _retriedCommandsCount; }
    void setRetriedCommandsCount(std::int64_t value) & {  _retriedCommandsCount = std::move(value);  }

    std::int64_t getRetriedStatementsCount() const { return _retriedStatementsCount; }
    void setRetriedStatementsCount(std::int64_t value) & {  _retriedStatementsCount = std::move(value);  }

    std::int64_t getTransactionsCollectionWriteCount() const { return _transactionsCollectionWriteCount; }
    void setTransactionsCollectionWriteCount(std::int64_t value) & {  _transactionsCollectionWriteCount = std::move(value);  }

    std::int64_t getCurrentActive() const { return _currentActive; }
    void setCurrentActive(std::int64_t value) & {  _currentActive = std::move(value);  }

    std::int64_t getCurrentInactive() const { return _currentInactive; }
    void setCurrentInactive(std::int64_t value) & {  _currentInactive = std::move(value);  }

    std::int64_t getCurrentOpen() const { return _currentOpen; }
    void setCurrentOpen(std::int64_t value) & {  _currentOpen = std::move(value);  }

    std::int64_t getTotalAborted() const { return _totalAborted; }
    void setTotalAborted(std::int64_t value) & {  _totalAborted = std::move(value);  }

    std::int64_t getTotalCommitted() const { return _totalCommitted; }
    void setTotalCommitted(std::int64_t value) & {  _totalCommitted = std::move(value);  }

    std::int64_t getTotalStarted() const { return _totalStarted; }
    void setTotalStarted(std::int64_t value) & {  _totalStarted = std::move(value);  }

    std::int64_t getTotalPrepared() const { return _totalPrepared; }
    void setTotalPrepared(std::int64_t value) & {  _totalPrepared = std::move(value);  }

    std::int64_t getTotalPreparedThenCommitted() const { return _totalPreparedThenCommitted; }
    void setTotalPreparedThenCommitted(std::int64_t value) & {  _totalPreparedThenCommitted = std::move(value);  }

    std::int64_t getTotalPreparedThenAborted() const { return _totalPreparedThenAborted; }
    void setTotalPreparedThenAborted(std::int64_t value) & {  _totalPreparedThenAborted = std::move(value);  }

    std::int64_t getCurrentPrepared() const { return _currentPrepared; }
    void setCurrentPrepared(std::int64_t value) & {  _currentPrepared = std::move(value);  }

    const boost::optional<mongo::LastCommittedTransaction>& getLastCommittedTransaction() const& { return _lastCommittedTransaction; }
    void getLastCommittedTransaction() && = delete;
    void setLastCommittedTransaction(boost::optional<mongo::LastCommittedTransaction> value) & {  _lastCommittedTransaction = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _retriedCommandsCount{0};
    std::int64_t _retriedStatementsCount{0};
    std::int64_t _transactionsCollectionWriteCount{0};
    std::int64_t _currentActive{0};
    std::int64_t _currentInactive{0};
    std::int64_t _currentOpen{0};
    std::int64_t _totalAborted{0};
    std::int64_t _totalCommitted{0};
    std::int64_t _totalStarted{0};
    std::int64_t _totalPrepared{0};
    std::int64_t _totalPreparedThenCommitted{0};
    std::int64_t _totalPreparedThenAborted{0};
    std::int64_t _currentPrepared{0};
    boost::optional<mongo::LastCommittedTransaction> _lastCommittedTransaction;
};

}  // namespace mongo
