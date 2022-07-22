/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/query/async_results_merger_params_gen.h --output build/opt/mongo/s/query/async_results_merger_params_gen.cpp src/mongo/s/query/async_results_merger_params.idl
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
#include "mongo/db/query/cursor_response.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/shard_id.h"
#include "mongo/util/net/hostandport.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A description of a cursor opened on a remote server.
 */
class RemoteCursor {
public:
    static constexpr auto kCursorResponseFieldName = "cursorResponse"_sd;
    static constexpr auto kHostAndPortFieldName = "hostAndPort"_sd;
    static constexpr auto kShardIdFieldName = "shardId"_sd;

    RemoteCursor();
    RemoteCursor(std::string shardId, mongo::HostAndPort hostAndPort, mongo::CursorResponse cursorResponse);

    static RemoteCursor parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The shardId of the shard on which the cursor resides.
     */
    const StringData getShardId() const& { return _shardId; }
    void getShardId() && = delete;
    void setShardId(StringData value) & { auto _tmpValue = value.toString();  _shardId = std::move(_tmpValue); _hasShardId = true; }

    /**
     * The exact host (within the shard) on which the cursor resides.
     */
    const mongo::HostAndPort& getHostAndPort() const { return _hostAndPort; }
    void setHostAndPort(mongo::HostAndPort value) & {  _hostAndPort = std::move(value); _hasHostAndPort = true; }

    /**
     * The response after establishing a cursor on the remote shard, including the first batch.
     */
    const mongo::CursorResponse& getCursorResponse() const { return _cursorResponse; }
    void setCursorResponse(mongo::CursorResponse value) & {  _cursorResponse = std::move(value); _hasCursorResponse = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _shardId;
    mongo::HostAndPort _hostAndPort;
    mongo::CursorResponse _cursorResponse;
    bool _hasShardId : 1;
    bool _hasHostAndPort : 1;
    bool _hasCursorResponse : 1;
};

/**
 * The parameters needed to establish an AsyncResultsMerger.
 */
class AsyncResultsMergerParams {
public:
    static constexpr auto kOperationSessionInfoFieldName = "OperationSessionInfoFromClient"_sd;
    static constexpr auto kAllowPartialResultsFieldName = "allowPartialResults"_sd;
    static constexpr auto kAutocommitFieldName = "autocommit"_sd;
    static constexpr auto kBatchSizeFieldName = "batchSize"_sd;
    static constexpr auto kCompareWholeSortKeyFieldName = "compareWholeSortKey"_sd;
    static constexpr auto kCoordinatorFieldName = "coordinator"_sd;
    static constexpr auto kNssFieldName = "nss"_sd;
    static constexpr auto kRecordRemoteOpWaitTimeFieldName = "recordRemoteOpWaitTime"_sd;
    static constexpr auto kRemotesFieldName = "remotes"_sd;
    static constexpr auto kSessionIdFieldName = "lsid"_sd;
    static constexpr auto kSortFieldName = "sort"_sd;
    static constexpr auto kStartTransactionFieldName = "startTransaction"_sd;
    static constexpr auto kTailableModeFieldName = "tailableMode"_sd;
    static constexpr auto kTxnNumberFieldName = "txnNumber"_sd;

    AsyncResultsMergerParams();
    AsyncResultsMergerParams(std::vector<mongo::RemoteCursor> remotes, mongo::NamespaceString nss);

    static AsyncResultsMergerParams parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Parser for pulling out the sessionId/txnNumber combination from commands
     */
    const OperationSessionInfoFromClient& getOperationSessionInfo() const { return _operationSessionInfo; }
    OperationSessionInfoFromClient& getOperationSessionInfo() { return _operationSessionInfo; }
    void setOperationSessionInfo(OperationSessionInfoFromClient value) & {  _operationSessionInfo = std::move(value);  }

    const  boost::optional<mongo::LogicalSessionFromClient>& getSessionId() const { return _operationSessionInfo.getSessionId(); }
    /**
     * The transaction number relative to the session in which a particular write operation executes.
     */
    const  boost::optional<std::int64_t> getTxnNumber() const { return _operationSessionInfo.getTxnNumber(); }
    const  boost::optional<bool> getAutocommit() const { return _operationSessionInfo.getAutocommit(); }
    /**
     * Used to indicate that a command is the start of a multi-statement transaction.
     */
    const  boost::optional<bool> getStartTransaction() const { return _operationSessionInfo.getStartTransaction(); }
    /**
     * Indicates that this shard is the coordinator shard for the transaction two-phase commit.
     */
    const  boost::optional<bool> getCoordinator() const { return _operationSessionInfo.getCoordinator(); }
    /**
     * The sort requested on the merging operation. Empty if there is no sort.
     */
    const boost::optional<mongo::BSONObj>& getSort() const& { return _sort; }
    void getSort() && = delete;
    void setSort(boost::optional<mongo::BSONObj> value) & {  _sort = std::move(value);  }

    /**
     * When 'compareWholeSortKey' is true, $sortKey is a scalar value, rather than an object. We extract the sort key {$sortKey: <value>}. The sort key pattern is verified to be {$sortKey: 1}.
     */
    bool getCompareWholeSortKey() const { return _compareWholeSortKey; }
    void setCompareWholeSortKey(bool value) & {  _compareWholeSortKey = std::move(value);  }

    const std::vector<mongo::RemoteCursor>& getRemotes() const& { return _remotes; }
    void getRemotes() && = delete;
    void setRemotes(std::vector<mongo::RemoteCursor> value) & {  _remotes = std::move(value); _hasRemotes = true; }

    /**
     * If set, the tailability mode of this cursor.
     */
    const boost::optional<mongo::TailableModeEnum> getTailableMode() const& { return _tailableMode; }
    void getTailableMode() && = delete;
    void setTailableMode(boost::optional<mongo::TailableModeEnum> value) & {  _tailableMode = std::move(value);  }

    /**
     * The batch size for this cursor.
     */
    const boost::optional<std::int64_t> getBatchSize() const& { return _batchSize; }
    void getBatchSize() && = delete;
    void setBatchSize(boost::optional<std::int64_t> value) & {  _batchSize = std::move(value);  }

    const mongo::NamespaceString& getNss() const { return _nss; }
    void setNss(mongo::NamespaceString value) & {  _nss = std::move(value); _hasNss = true; }

    /**
     * If set, error responses are ignored.
     */
    bool getAllowPartialResults() const { return _allowPartialResults; }
    void setAllowPartialResults(bool value) & {  _allowPartialResults = std::move(value);  }

    /**
     * If set, records the total time spent waiting for remote operations to complete.
     */
    bool getRecordRemoteOpWaitTime() const { return _recordRemoteOpWaitTime; }
    void setRecordRemoteOpWaitTime(bool value) & {  _recordRemoteOpWaitTime = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    OperationSessionInfoFromClient _operationSessionInfo;
    boost::optional<mongo::BSONObj> _sort;
    bool _compareWholeSortKey{false};
    std::vector<mongo::RemoteCursor> _remotes;
    boost::optional<mongo::TailableModeEnum> _tailableMode;
    boost::optional<std::int64_t> _batchSize;
    mongo::NamespaceString _nss;
    bool _allowPartialResults{false};
    bool _recordRemoteOpWaitTime{false};
    bool _hasRemotes : 1;
    bool _hasNss : 1;
};

}  // namespace mongo
