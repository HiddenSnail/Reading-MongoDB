/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/logical_session_id_gen.h --output build/opt/mongo/db/logical_session_id_gen.cpp src/mongo/db/logical_session_id.idl
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
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing a LogicalSessionId
 */
class LogicalSessionId {
public:
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kUidFieldName = "uid"_sd;

    LogicalSessionId();
    LogicalSessionId(mongo::UUID id, mongo::SHA256Block uid);

    static LogicalSessionId parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::UUID& getId() const { return _id; }
    void setId(mongo::UUID value) & {  _id = std::move(value); _hasId = true; }

    const mongo::SHA256Block& getUid() const { return _uid; }
    void setUid(mongo::SHA256Block value) & {  _uid = std::move(value); _hasUid = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _id;
    mongo::SHA256Block _uid;
    bool _hasId : 1;
    bool _hasUid : 1;
};

/**
 * A struct representing a LogicalSessionId to external clients
 */
class LogicalSessionIdToClient {
public:
    static constexpr auto kIdFieldName = "id"_sd;

    LogicalSessionIdToClient();
    LogicalSessionIdToClient(mongo::UUID id);

    static LogicalSessionIdToClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::UUID& getId() const { return _id; }
    void setId(mongo::UUID value) & {  _id = std::move(value); _hasId = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _id;
    bool _hasId : 1;
};

/**
 * A struct representing a LogicalSession reply to external clients
 */
class LogicalSessionToClient {
public:
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kTimeoutMinutesFieldName = "timeoutMinutes"_sd;

    LogicalSessionToClient();
    LogicalSessionToClient(mongo::LogicalSessionIdToClient id, std::int32_t timeoutMinutes);

    static LogicalSessionToClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::LogicalSessionIdToClient& getId() const { return _id; }
    mongo::LogicalSessionIdToClient& getId() { return _id; }
    void setId(mongo::LogicalSessionIdToClient value) & {  _id = std::move(value); _hasId = true; }

    std::int32_t getTimeoutMinutes() const { return _timeoutMinutes; }
    void setTimeoutMinutes(std::int32_t value) & {  _timeoutMinutes = std::move(value); _hasTimeoutMinutes = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::LogicalSessionIdToClient _id;
    std::int32_t _timeoutMinutes;
    bool _hasId : 1;
    bool _hasTimeoutMinutes : 1;
};

/**
 * A struct representing a LogicalSessionRecord
 */
class LogicalSessionRecord {
public:
    static constexpr auto kIdFieldName = "_id"_sd;
    static constexpr auto kLastUseFieldName = "lastUse"_sd;
    static constexpr auto kUserFieldName = "user"_sd;

    LogicalSessionRecord();
    LogicalSessionRecord(mongo::LogicalSessionId id, mongo::Date_t lastUse);

    static LogicalSessionRecord parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::LogicalSessionId& getId() const { return _id; }
    mongo::LogicalSessionId& getId() { return _id; }
    void setId(mongo::LogicalSessionId value) & {  _id = std::move(value); _hasId = true; }

    const mongo::Date_t& getLastUse() const { return _lastUse; }
    void setLastUse(mongo::Date_t value) & {  _lastUse = std::move(value); _hasLastUse = true; }

    const boost::optional<StringData> getUser() const& { return boost::optional<StringData>{_user}; }
    void getUser() && = delete;
    void setUser(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _user = std::move(_tmpValue);
    } else {
        _user = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::LogicalSessionId _id;
    mongo::Date_t _lastUse;
    boost::optional<std::string> _user;
    bool _hasId : 1;
    bool _hasLastUse : 1;
};

/**
 * A struct representing a LogicalSessionId from external clients
 */
class LogicalSessionFromClient {
public:
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kUidFieldName = "uid"_sd;

    LogicalSessionFromClient();
    LogicalSessionFromClient(mongo::UUID id);

    static LogicalSessionFromClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::UUID& getId() const { return _id; }
    void setId(mongo::UUID value) & {  _id = std::move(value); _hasId = true; }

    const boost::optional<mongo::SHA256Block>& getUid() const& { return _uid; }
    void getUid() && = delete;
    void setUid(boost::optional<mongo::SHA256Block> value) & {  _uid = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _id;
    boost::optional<mongo::SHA256Block> _uid;
    bool _hasId : 1;
};

/**
 * Parser for serializing sessionId/txnNumber combination
 */
class OperationSessionInfo {
public:
    static constexpr auto kSessionIdFieldName = "lsid"_sd;
    static constexpr auto kTxnNumberFieldName = "txnNumber"_sd;

    OperationSessionInfo();

    static OperationSessionInfo parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const boost::optional<mongo::LogicalSessionId>& getSessionId() const& { return _sessionId; }
    void getSessionId() && = delete;
    void setSessionId(boost::optional<mongo::LogicalSessionId> value) & {  _sessionId = std::move(value);  }

    /**
     * The transaction number relative to the session in which a particular write operation executes.
     */
    const boost::optional<std::int64_t> getTxnNumber() const& { return _txnNumber; }
    void getTxnNumber() && = delete;
    void setTxnNumber(boost::optional<std::int64_t> value) & {  _txnNumber = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::LogicalSessionId> _sessionId;
    boost::optional<std::int64_t> _txnNumber;
};

/**
 * Parser for pulling out the sessionId/txnNumber combination from commands
 */
class OperationSessionInfoFromClient {
public:
    static constexpr auto kAutocommitFieldName = "autocommit"_sd;
    static constexpr auto kCoordinatorFieldName = "coordinator"_sd;
    static constexpr auto kSessionIdFieldName = "lsid"_sd;
    static constexpr auto kStartTransactionFieldName = "startTransaction"_sd;
    static constexpr auto kTxnNumberFieldName = "txnNumber"_sd;

    OperationSessionInfoFromClient();

    static OperationSessionInfoFromClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const boost::optional<mongo::LogicalSessionFromClient>& getSessionId() const& { return _sessionId; }
    void getSessionId() && = delete;
    void setSessionId(boost::optional<mongo::LogicalSessionFromClient> value) & {  _sessionId = std::move(value);  }

    /**
     * The transaction number relative to the session in which a particular write operation executes.
     */
    const boost::optional<std::int64_t> getTxnNumber() const& { return _txnNumber; }
    void getTxnNumber() && = delete;
    void setTxnNumber(boost::optional<std::int64_t> value) & {  _txnNumber = std::move(value);  }

    const boost::optional<bool> getAutocommit() const& { return _autocommit; }
    void getAutocommit() && = delete;
    void setAutocommit(boost::optional<bool> value) & {  _autocommit = std::move(value);  }

    /**
     * Used to indicate that a command is the start of a multi-statement transaction.
     */
    const boost::optional<bool> getStartTransaction() const& { return _startTransaction; }
    void getStartTransaction() && = delete;
    void setStartTransaction(boost::optional<bool> value) & {  _startTransaction = std::move(value);  }

    /**
     * Indicates that this shard is the coordinator shard for the transaction two-phase commit.
     */
    const boost::optional<bool> getCoordinator() const& { return _coordinator; }
    void getCoordinator() && = delete;
    void setCoordinator(boost::optional<bool> value) & {  _coordinator = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::LogicalSessionFromClient> _sessionId;
    boost::optional<std::int64_t> _txnNumber;
    boost::optional<bool> _autocommit;
    boost::optional<bool> _startTransaction;
    boost::optional<bool> _coordinator;
};

/**
 * Individual result
 */
class SessionsCollectionFetchResultIndividualResult {
public:
    static constexpr auto k_idFieldName = "_id"_sd;

    SessionsCollectionFetchResultIndividualResult();
    SessionsCollectionFetchResultIndividualResult(mongo::LogicalSessionId _id);

    static SessionsCollectionFetchResultIndividualResult parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::LogicalSessionId& get_id() const { return __id; }
    mongo::LogicalSessionId& get_id() { return __id; }
    void set_id(mongo::LogicalSessionId value) & {  __id = std::move(value); _has_id = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::LogicalSessionId __id;
    bool _has_id : 1;
};

/**
 * Cursor object
 */
class SessionsCollectionFetchResultCursor {
public:
    static constexpr auto kFirstBatchFieldName = "firstBatch"_sd;

    SessionsCollectionFetchResultCursor();
    SessionsCollectionFetchResultCursor(std::vector<mongo::SessionsCollectionFetchResultIndividualResult> firstBatch);

    static SessionsCollectionFetchResultCursor parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::SessionsCollectionFetchResultIndividualResult>& getFirstBatch() const& { return _firstBatch; }
    void getFirstBatch() && = delete;
    void setFirstBatch(std::vector<mongo::SessionsCollectionFetchResultIndividualResult> value) & {  _firstBatch = std::move(value); _hasFirstBatch = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::SessionsCollectionFetchResultIndividualResult> _firstBatch;
    bool _hasFirstBatch : 1;
};

/**
 * Parser for pulling out the fetch results from SessionsCollection::fetch
 */
class SessionsCollectionFetchResult {
public:
    static constexpr auto kCursorFieldName = "cursor"_sd;

    SessionsCollectionFetchResult();
    SessionsCollectionFetchResult(mongo::SessionsCollectionFetchResultCursor cursor);

    static SessionsCollectionFetchResult parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::SessionsCollectionFetchResultCursor& getCursor() const { return _cursor; }
    mongo::SessionsCollectionFetchResultCursor& getCursor() { return _cursor; }
    void setCursor(mongo::SessionsCollectionFetchResultCursor value) & {  _cursor = std::move(value); _hasCursor = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::SessionsCollectionFetchResultCursor _cursor;
    bool _hasCursor : 1;
};

/**
 * Id
 */
class SessionsCollectionFetchRequestFilterId {
public:
    static constexpr auto kInFieldName = "$in"_sd;

    SessionsCollectionFetchRequestFilterId();
    SessionsCollectionFetchRequestFilterId(std::vector<mongo::LogicalSessionId> in);

    static SessionsCollectionFetchRequestFilterId parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::LogicalSessionId>& getIn() const& { return _in; }
    void getIn() && = delete;
    void setIn(std::vector<mongo::LogicalSessionId> value) & {  _in = std::move(value); _hasIn = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::LogicalSessionId> _in;
    bool _hasIn : 1;
};

/**
 * filter
 */
class SessionsCollectionFetchRequestFilter {
public:
    static constexpr auto k_idFieldName = "_id"_sd;

    SessionsCollectionFetchRequestFilter();
    SessionsCollectionFetchRequestFilter(mongo::SessionsCollectionFetchRequestFilterId _id);

    static SessionsCollectionFetchRequestFilter parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::SessionsCollectionFetchRequestFilterId& get_id() const { return __id; }
    mongo::SessionsCollectionFetchRequestFilterId& get_id() { return __id; }
    void set_id(mongo::SessionsCollectionFetchRequestFilterId value) & {  __id = std::move(value); _has_id = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::SessionsCollectionFetchRequestFilterId __id;
    bool _has_id : 1;
};

/**
 * projection
 */
class SessionsCollectionFetchRequestProjection {
public:
    static constexpr auto k_idFieldName = "_id"_sd;

    SessionsCollectionFetchRequestProjection();
    SessionsCollectionFetchRequestProjection(std::int32_t _id);

    static SessionsCollectionFetchRequestProjection parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int32_t get_id() const { return __id; }
    void set_id(std::int32_t value) & {  __id = std::move(value); _has_id = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int32_t __id;
    bool _has_id : 1;
};

/**
 * Parser for forming the fetch request for SessionsCollection::fetch
 */
class SessionsCollectionFetchRequest {
public:
    static constexpr auto kBatchSizeFieldName = "batchSize"_sd;
    static constexpr auto kFilterFieldName = "filter"_sd;
    static constexpr auto kFindFieldName = "find"_sd;
    static constexpr auto kLimitFieldName = "limit"_sd;
    static constexpr auto kProjectionFieldName = "projection"_sd;
    static constexpr auto kSingleBatchFieldName = "singleBatch"_sd;

    SessionsCollectionFetchRequest();
    SessionsCollectionFetchRequest(std::string find, mongo::SessionsCollectionFetchRequestFilter filter, mongo::SessionsCollectionFetchRequestProjection projection, std::int32_t batchSize, bool singleBatch, std::int32_t limit);

    static SessionsCollectionFetchRequest parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getFind() const& { return _find; }
    void getFind() && = delete;
    void setFind(StringData value) & { auto _tmpValue = value.toString();  _find = std::move(_tmpValue); _hasFind = true; }

    const mongo::SessionsCollectionFetchRequestFilter& getFilter() const { return _filter; }
    mongo::SessionsCollectionFetchRequestFilter& getFilter() { return _filter; }
    void setFilter(mongo::SessionsCollectionFetchRequestFilter value) & {  _filter = std::move(value); _hasFilter = true; }

    const mongo::SessionsCollectionFetchRequestProjection& getProjection() const { return _projection; }
    mongo::SessionsCollectionFetchRequestProjection& getProjection() { return _projection; }
    void setProjection(mongo::SessionsCollectionFetchRequestProjection value) & {  _projection = std::move(value); _hasProjection = true; }

    std::int32_t getBatchSize() const { return _batchSize; }
    void setBatchSize(std::int32_t value) & {  _batchSize = std::move(value); _hasBatchSize = true; }

    bool getSingleBatch() const { return _singleBatch; }
    void setSingleBatch(bool value) & {  _singleBatch = std::move(value); _hasSingleBatch = true; }

    std::int32_t getLimit() const { return _limit; }
    void setLimit(std::int32_t value) & {  _limit = std::move(value); _hasLimit = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _find;
    mongo::SessionsCollectionFetchRequestFilter _filter;
    mongo::SessionsCollectionFetchRequestProjection _projection;
    std::int32_t _batchSize;
    bool _singleBatch;
    std::int32_t _limit;
    bool _hasFind : 1;
    bool _hasFilter : 1;
    bool _hasProjection : 1;
    bool _hasBatchSize : 1;
    bool _hasSingleBatch : 1;
    bool _hasLimit : 1;
};

constexpr auto kLocalLogicalSessionTimeoutMinutesDefault = 30;

extern int localLogicalSessionTimeoutMinutes;
}  // namespace mongo
