/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/txn_cmds_gen.h --output build/opt/mongo/db/commands/txn_cmds_gen.cpp src/mongo/db/commands/txn_cmds.idl
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
#include "mongo/s/shard_id.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Contains fields a participant attaches in responses to successful transaction statements
 */
class TxnResponseMetadata {
public:
    static constexpr auto kReadOnlyFieldName = "readOnly"_sd;

    TxnResponseMetadata();
    TxnResponseMetadata(bool readOnly);

    static TxnResponseMetadata parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * True if the shard has the transaction in progress but has not done a write for it
     */
    bool getReadOnly() const { return _readOnly; }
    void setReadOnly(bool value) & {  _readOnly = std::move(value); _hasReadOnly = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    bool _readOnly;
    bool _hasReadOnly : 1;
};

/**
 * Contains info for retrying the commit of a sharded transaction
 */
class TxnRecoveryToken {
public:
    static constexpr auto kRecoveryShardIdFieldName = "recoveryShardId"_sd;

    TxnRecoveryToken();

    static TxnRecoveryToken parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The id of the shard that can make progress committing the transaction
     */
    const boost::optional<mongo::ShardId>& getRecoveryShardId() const& { return _recoveryShardId; }
    void getRecoveryShardId() && = delete;
    void setRecoveryShardId(boost::optional<mongo::ShardId> value) & {  _recoveryShardId = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::ShardId> _recoveryShardId;
};

/**
 * A document representing the 'o' field of a 'commitTransaction' oplog entry.
 */
class CommitTransactionOplogObject {
public:
    static constexpr auto kCommitTimestampFieldName = "commitTimestamp"_sd;
    static constexpr auto kCommitTransactionFieldName = "commitTransaction"_sd;
    static constexpr auto kCountFieldName = "count"_sd;

    CommitTransactionOplogObject();

    static CommitTransactionOplogObject parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int32_t getCommitTransaction() const { return _commitTransaction; }
    void setCommitTransaction(std::int32_t value) & {  _commitTransaction = std::move(value);  }

    /**
     * Specifies the timestamp at which the multi-document transaction should be committed.  Required for prepared transactions, not permitted for unprepared ones.
     */
    const boost::optional<mongo::Timestamp>& getCommitTimestamp() const& { return _commitTimestamp; }
    void getCommitTimestamp() && = delete;
    void setCommitTimestamp(boost::optional<mongo::Timestamp> value) & {  _commitTimestamp = std::move(value);  }

    /**
     * The number of in-transaction operations applied in a transaction. This field is required only for unprepared transactions.
     */
    const boost::optional<std::int64_t> getCount() const& { return _count; }
    void getCount() && = delete;
    void setCount(boost::optional<std::int64_t> value) & {  _count = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int32_t _commitTransaction{1};
    boost::optional<mongo::Timestamp> _commitTimestamp;
    boost::optional<std::int64_t> _count;
};

/**
 * A document representing the 'o' field of an 'abortTransaction' oplog entry.
 */
class AbortTransactionOplogObject {
public:
    static constexpr auto kAbortTransactionFieldName = "abortTransaction"_sd;

    AbortTransactionOplogObject();

    static AbortTransactionOplogObject parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int32_t getAbortTransaction() const { return _abortTransaction; }
    void setAbortTransaction(std::int32_t value) & {  _abortTransaction = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int32_t _abortTransaction{1};
};

/**
 * commitTransaction Command
 */
class CommitTransaction {
public:
    static constexpr auto kCommitTimestampFieldName = "commitTimestamp"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kRecoveryTokenFieldName = "recoveryToken"_sd;
    static constexpr auto kCommandName = "commitTransaction"_sd;

    CommitTransaction();

    static CommitTransaction parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static CommitTransaction parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * Timestamp at which to commit the transaction. Required for prepared transactions and illegal for non-prepared ones.
     */
    const boost::optional<mongo::Timestamp>& getCommitTimestamp() const& { return _commitTimestamp; }
    void getCommitTimestamp() && = delete;
    void setCommitTimestamp(boost::optional<mongo::Timestamp> value) & {  _commitTimestamp = std::move(value);  }

    /**
     * A mongos that doesn't know about this transaction can attempt to make progress on commit by processing using the info in the recoveryToken.
     */
    const boost::optional<mongo::TxnRecoveryToken>& getRecoveryToken() const& { return _recoveryToken; }
    void getRecoveryToken() && = delete;
    void setRecoveryToken(boost::optional<mongo::TxnRecoveryToken> value) & {  _recoveryToken = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    boost::optional<mongo::Timestamp> _commitTimestamp;
    boost::optional<mongo::TxnRecoveryToken> _recoveryToken;
    std::string _dbName;
    bool _hasDbName : 1;
};

/**
 * abortTransaction Command
 */
class AbortTransaction {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "abortTransaction"_sd;

    AbortTransaction();

    static AbortTransaction parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static AbortTransaction parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
