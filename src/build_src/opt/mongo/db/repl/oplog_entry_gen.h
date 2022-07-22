/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/oplog_entry_gen.h --output build/opt/mongo/db/repl/oplog_entry_gen.cpp src/mongo/db/repl/oplog_entry.idl
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
#include "mongo/crypto/sha256_block.h"
#include "mongo/db/logical_session_id_gen.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/repl/optime.h"
#include "mongo/db/repl/optime_and_wall_time_base_gen.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace repl {

/**
 * The type of an operation in the oplog
 */
enum class OpTypeEnum : std::int32_t {
    kCommand ,
    kInsert ,
    kUpdate ,
    kDelete ,
    kNoop ,
};

OpTypeEnum OpType_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData OpType_serializer(OpTypeEnum value);

/**
 * A document that represents an operation in a transaction. Should never be used directly in server code. Instead, create an instance of ReplOperation.
 */
class DurableReplOperation {
public:
    static constexpr auto kNssFieldName = "ns"_sd;
    static constexpr auto kObjectFieldName = "o"_sd;
    static constexpr auto kObject2FieldName = "o2"_sd;
    static constexpr auto kOpTypeFieldName = "op"_sd;
    static constexpr auto kPreImageOpTimeFieldName = "preImageOpTime"_sd;
    static constexpr auto kUpsertFieldName = "b"_sd;
    static constexpr auto kUuidFieldName = "ui"_sd;

    DurableReplOperation();
    DurableReplOperation(mongo::repl::OpTypeEnum opType, mongo::NamespaceString nss, mongo::BSONObj object);

    static DurableReplOperation parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The operation type
     */
    mongo::repl::OpTypeEnum getOpType() const { return _opType; }
    void setOpType(mongo::repl::OpTypeEnum value) & {  _opType = std::move(value); _hasOpType = true; }

    /**
     * The namespace on which to apply the operation
     */
    const mongo::NamespaceString& getNss() const { return _nss; }
    void setNss(mongo::NamespaceString value) & {  _nss = std::move(value); _hasNss = true; }

    /**
     * The UUID of the collection
     */
    const boost::optional<mongo::UUID>& getUuid() const& { return _uuid; }
    void getUuid() && = delete;
    void setUuid(boost::optional<mongo::UUID> value) & {  _uuid = std::move(value);  }

    /**
     * The operation applied
     */
    const mongo::BSONObj& getObject() const { return _object; }
    void setObject(mongo::BSONObj value) & {  _object = std::move(value); _hasObject = true; }

    /**
     * Additional information about the operation applied
     */
    const boost::optional<mongo::BSONObj>& getObject2() const& { return _object2; }
    void getObject2() && = delete;
    void setObject2(boost::optional<mongo::BSONObj> value) & {  _object2 = std::move(value);  }

    /**
     * If true, treats this update operation as an upsert. Used when extracting individual update oplog entries from the oplog entry of an applyOps command that was executed with alwaysUpsert true (the default). Originally added for backwards compatibility with updates from 3.6 and before.
     */
    const boost::optional<bool> getUpsert() const& { return _upsert; }
    void getUpsert() && = delete;
    void setUpsert(boost::optional<bool> value) & {  _upsert = std::move(value);  }

    /**
     * The optime of another oplog entry that contains the document before an update/remove was applied.
     */
    const boost::optional<mongo::repl::OpTime>& getPreImageOpTime() const& { return _preImageOpTime; }
    void getPreImageOpTime() && = delete;
    void setPreImageOpTime(boost::optional<mongo::repl::OpTime> value) & {  _preImageOpTime = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::repl::OpTypeEnum _opType;
    mongo::NamespaceString _nss;
    boost::optional<mongo::UUID> _uuid;
    mongo::BSONObj _object;
    boost::optional<mongo::BSONObj> _object2;
    boost::optional<bool> _upsert;
    boost::optional<mongo::repl::OpTime> _preImageOpTime;
    bool _hasOpType : 1;
    bool _hasNss : 1;
    bool _hasObject : 1;
};

/**
 * A document in which the server stores an oplog entry.
 */
class OplogEntryBase {
public:
    static constexpr auto kDurableReplOperationFieldName = "DurableReplOperation"_sd;
    static constexpr auto kOpTimeAndWallTimeBaseFieldName = "OpTimeAndWallTimeBase"_sd;
    static constexpr auto kOperationSessionInfoFieldName = "OperationSessionInfo"_sd;
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kFromMigrateFieldName = "fromMigrate"_sd;
    static constexpr auto kHashFieldName = "h"_sd;
    static constexpr auto kNssFieldName = "ns"_sd;
    static constexpr auto kObjectFieldName = "o"_sd;
    static constexpr auto kObject2FieldName = "o2"_sd;
    static constexpr auto kOpTypeFieldName = "op"_sd;
    static constexpr auto kPostImageOpTimeFieldName = "postImageOpTime"_sd;
    static constexpr auto kPreImageOpTimeFieldName = "preImageOpTime"_sd;
    static constexpr auto kPrevWriteOpTimeInTransactionFieldName = "prevOpTime"_sd;
    static constexpr auto kSessionIdFieldName = "lsid"_sd;
    static constexpr auto kStatementIdFieldName = "stmtId"_sd;
    static constexpr auto kTermFieldName = "t"_sd;
    static constexpr auto kTimestampFieldName = "ts"_sd;
    static constexpr auto kTxnNumberFieldName = "txnNumber"_sd;
    static constexpr auto kUpsertFieldName = "b"_sd;
    static constexpr auto kUuidFieldName = "ui"_sd;
    static constexpr auto kVersionFieldName = "v"_sd;
    static constexpr auto kWallClockTimeFieldName = "wall"_sd;

    OplogEntryBase();

    static OplogEntryBase parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Parser for serializing sessionId/txnNumber combination
     */
    const OperationSessionInfo& getOperationSessionInfo() const { return _operationSessionInfo; }
    OperationSessionInfo& getOperationSessionInfo() { return _operationSessionInfo; }
    void setOperationSessionInfo(OperationSessionInfo value) & {  _operationSessionInfo = std::move(value);  }

    const  boost::optional<mongo::LogicalSessionId>& getSessionId() const { return _operationSessionInfo.getSessionId(); }
    /**
     * The transaction number relative to the session in which a particular write operation executes.
     */
    const  boost::optional<std::int64_t> getTxnNumber() const { return _operationSessionInfo.getTxnNumber(); }
    /**
     * A document that represents an operation in a transaction. Should never be used directly in server code. Instead, create an instance of ReplOperation.
     */
    const DurableReplOperation& getDurableReplOperation() const { return _durableReplOperation; }
    DurableReplOperation& getDurableReplOperation() { return _durableReplOperation; }
    void setDurableReplOperation(DurableReplOperation value) & {  _durableReplOperation = std::move(value);  }

    /**
     * The operation type
     */
     mongo::repl::OpTypeEnum getOpType() const { return _durableReplOperation.getOpType(); }
    /**
     * The namespace on which to apply the operation
     */
    const  mongo::NamespaceString& getNss() const { return _durableReplOperation.getNss(); }
    /**
     * The UUID of the collection
     */
    const  boost::optional<mongo::UUID>& getUuid() const { return _durableReplOperation.getUuid(); }
    /**
     * The operation applied
     */
    const  mongo::BSONObj& getObject() const { return _durableReplOperation.getObject(); }
    /**
     * Additional information about the operation applied
     */
    const  boost::optional<mongo::BSONObj>& getObject2() const { return _durableReplOperation.getObject2(); }
    /**
     * If true, treats this update operation as an upsert. Used when extracting individual update oplog entries from the oplog entry of an applyOps command that was executed with alwaysUpsert true (the default). Originally added for backwards compatibility with updates from 3.6 and before.
     */
    const  boost::optional<bool> getUpsert() const { return _durableReplOperation.getUpsert(); }
    /**
     * The optime of another oplog entry that contains the document before an update/remove was applied.
     */
    const  boost::optional<mongo::repl::OpTime>& getPreImageOpTime() const { return _durableReplOperation.getPreImageOpTime(); }
    /**
     * A document that stores the time-related fields in an oplog entry. Should never be used directly in server code. Instead, create an instance of OpTimeAndWallTime.
     */
    const OpTimeAndWallTimeBase& getOpTimeAndWallTimeBase() const { return _opTimeAndWallTimeBase; }
    OpTimeAndWallTimeBase& getOpTimeAndWallTimeBase() { return _opTimeAndWallTimeBase; }
    void setOpTimeAndWallTimeBase(OpTimeAndWallTimeBase value) & {  _opTimeAndWallTimeBase = std::move(value);  }

    /**
     * The time when the oplog entry was created
     */
    const  mongo::Timestamp& getTimestamp() const { return _opTimeAndWallTimeBase.getTimestamp(); }
    /**
     * The term of the primary that created the oplog entry
     */
    const  boost::optional<std::int64_t> getTerm() const { return _opTimeAndWallTimeBase.getTerm(); }
    /**
     * A wallclock time with MS resolution
     */
    const  mongo::Date_t& getWallClockTime() const { return _opTimeAndWallTimeBase.getWallClockTime(); }
    /**
     * The hash of the oplog entry
     */
    const boost::optional<std::int64_t> getHash() const& { return _hash; }
    void getHash() && = delete;
    void setHash(boost::optional<std::int64_t> value) & {  _hash = std::move(value);  }

    /**
     * The version of the oplog
     */
    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value);  }

    /**
     * An operation caused by a chunk migration
     */
    const boost::optional<bool> getFromMigrate() const& { return _fromMigrate; }
    void getFromMigrate() && = delete;
    void setFromMigrate(boost::optional<bool> value) & {  _fromMigrate = std::move(value);  }

    /**
     * An optional _id field for tests that manually insert oplog entries
     */
    const boost::optional<mongo::OID>& get_id() const& { return __id; }
    void get_id() && = delete;
    void set_id(boost::optional<mongo::OID> value) & {  __id = std::move(value);  }

    /**
     * Identifier of the transaction statement which generated this oplog entry
     */
    const boost::optional<std::int32_t> getStatementId() const& { return _statementId; }
    void getStatementId() && = delete;
    void setStatementId(boost::optional<std::int32_t> value) & {  _statementId = std::move(value);  }

    /**
     * The opTime of the previous write with the same transaction.
     */
    const boost::optional<mongo::repl::OpTime>& getPrevWriteOpTimeInTransaction() const& { return _prevWriteOpTimeInTransaction; }
    void getPrevWriteOpTimeInTransaction() && = delete;
    void setPrevWriteOpTimeInTransaction(boost::optional<mongo::repl::OpTime> value) & {  _prevWriteOpTimeInTransaction = std::move(value);  }

    /**
     * The optime of another oplog entry that contains the document after an update was applied.
     */
    const boost::optional<mongo::repl::OpTime>& getPostImageOpTime() const& { return _postImageOpTime; }
    void getPostImageOpTime() && = delete;
    void setPostImageOpTime(boost::optional<mongo::repl::OpTime> value) & {  _postImageOpTime = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    OperationSessionInfo _operationSessionInfo;
    DurableReplOperation _durableReplOperation;
    OpTimeAndWallTimeBase _opTimeAndWallTimeBase;
    boost::optional<std::int64_t> _hash;
    std::int64_t _version{2};
    boost::optional<bool> _fromMigrate;
    boost::optional<mongo::OID> __id;
    boost::optional<std::int32_t> _statementId;
    boost::optional<mongo::repl::OpTime> _prevWriteOpTimeInTransaction;
    boost::optional<mongo::repl::OpTime> _postImageOpTime;
};

}  // namespace repl
}  // namespace mongo
