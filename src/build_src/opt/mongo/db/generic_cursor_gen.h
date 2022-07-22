/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/generic_cursor_gen.h --output build/opt/mongo/db/generic_cursor_gen.cpp src/mongo/db/generic_cursor.idl
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
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing a cursor in either mongod or mongos
 */
class GenericCursor {
public:
    static constexpr auto kAwaitDataFieldName = "awaitData"_sd;
    static constexpr auto kCreatedDateFieldName = "createdDate"_sd;
    static constexpr auto kCursorIdFieldName = "cursorId"_sd;
    static constexpr auto kLastAccessDateFieldName = "lastAccessDate"_sd;
    static constexpr auto kLastKnownCommittedOpTimeFieldName = "lastKnownCommittedOpTime"_sd;
    static constexpr auto kLsidFieldName = "lsid"_sd;
    static constexpr auto kNBatchesReturnedFieldName = "nBatchesReturned"_sd;
    static constexpr auto kNDocsReturnedFieldName = "nDocsReturned"_sd;
    static constexpr auto kNoCursorTimeoutFieldName = "noCursorTimeout"_sd;
    static constexpr auto kNsFieldName = "ns"_sd;
    static constexpr auto kOperationUsingCursorIdFieldName = "operationUsingCursorId"_sd;
    static constexpr auto kOriginatingCommandFieldName = "originatingCommand"_sd;
    static constexpr auto kPlanSummaryFieldName = "planSummary"_sd;
    static constexpr auto kTailableFieldName = "tailable"_sd;

    GenericCursor();

    static GenericCursor parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The cursor id of the cursor.
     */
    const boost::optional<std::int64_t> getCursorId() const& { return _cursorId; }
    void getCursorId() && = delete;
    void setCursorId(boost::optional<std::int64_t> value) & {  _cursorId = std::move(value);  }

    /**
     * The namespace of the cursor.
     */
    const boost::optional<mongo::NamespaceString>& getNs() const& { return _ns; }
    void getNs() && = delete;
    void setNs(boost::optional<mongo::NamespaceString> value) & {  _ns = std::move(value);  }

    /**
     * The time and date the cursor was created.
     */
    const boost::optional<mongo::Date_t>& getCreatedDate() const& { return _createdDate; }
    void getCreatedDate() && = delete;
    void setCreatedDate(boost::optional<mongo::Date_t> value) & {  _createdDate = std::move(value);  }

    /**
     * The last time the cursor was used (was pinned).
     */
    const boost::optional<mongo::Date_t>& getLastAccessDate() const& { return _lastAccessDate; }
    void getLastAccessDate() && = delete;
    void setLastAccessDate(boost::optional<mongo::Date_t> value) & {  _lastAccessDate = std::move(value);  }

    /**
     * The number of docs returned by the cursor.
     */
    const boost::optional<std::int64_t> getNDocsReturned() const& { return _nDocsReturned; }
    void getNDocsReturned() && = delete;
    void setNDocsReturned(boost::optional<std::int64_t> value) & {  _nDocsReturned = std::move(value);  }

    /**
     * The number of batches returned by the cursor.
     */
    const boost::optional<std::int64_t> getNBatchesReturned() const& { return _nBatchesReturned; }
    void getNBatchesReturned() && = delete;
    void setNBatchesReturned(boost::optional<std::int64_t> value) & {  _nBatchesReturned = std::move(value);  }

    /**
     * If true the cursor will not be timed out because of inactivity.
     */
    const boost::optional<bool> getNoCursorTimeout() const& { return _noCursorTimeout; }
    void getNoCursorTimeout() && = delete;
    void setNoCursorTimeout(boost::optional<bool> value) & {  _noCursorTimeout = std::move(value);  }

    /**
     * Whether the cursor is tailable and remains open after exhausting all documents in the find.
     */
    const boost::optional<bool> getTailable() const& { return _tailable; }
    void getTailable() && = delete;
    void setTailable(boost::optional<bool> value) & {  _tailable = std::move(value);  }

    /**
     * Whether this is a tailable and awaitData cursor that will block waiting for new data to be inserted into a capped collection.
     */
    const boost::optional<bool> getAwaitData() const& { return _awaitData; }
    void getAwaitData() && = delete;
    void setAwaitData(boost::optional<bool> value) & {  _awaitData = std::move(value);  }

    /**
     * The bson object containing the command that created the cursor.
     */
    const boost::optional<mongo::BSONObj>& getOriginatingCommand() const& { return _originatingCommand; }
    void getOriginatingCommand() && = delete;
    void setOriginatingCommand(boost::optional<mongo::BSONObj> value) & {  _originatingCommand = std::move(value);  }

    const boost::optional<mongo::LogicalSessionId>& getLsid() const& { return _lsid; }
    void getLsid() && = delete;
    void setLsid(boost::optional<mongo::LogicalSessionId> value) & {  _lsid = std::move(value);  }

    /**
     * The plan summary of this cursor's query.
     */
    const boost::optional<StringData> getPlanSummary() const& { return boost::optional<StringData>{_planSummary}; }
    void getPlanSummary() && = delete;
    void setPlanSummary(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _planSummary = std::move(_tmpValue);
    } else {
        _planSummary = boost::none;
    }
      }

    /**
     * The op ID of the operation pinning the cursor. Will be empty for idle cursors.
     */
    const boost::optional<std::int64_t> getOperationUsingCursorId() const& { return _operationUsingCursorId; }
    void getOperationUsingCursorId() && = delete;
    void setOperationUsingCursorId(boost::optional<std::int64_t> value) & {  _operationUsingCursorId = std::move(value);  }

    /**
     * The commit point known by the server at the time when the last batch was returned.
     */
    const boost::optional<mongo::repl::OpTime>& getLastKnownCommittedOpTime() const& { return _lastKnownCommittedOpTime; }
    void getLastKnownCommittedOpTime() && = delete;
    void setLastKnownCommittedOpTime(boost::optional<mongo::repl::OpTime> value) & {  _lastKnownCommittedOpTime = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<std::int64_t> _cursorId;
    boost::optional<mongo::NamespaceString> _ns;
    boost::optional<mongo::Date_t> _createdDate;
    boost::optional<mongo::Date_t> _lastAccessDate;
    boost::optional<std::int64_t> _nDocsReturned;
    boost::optional<std::int64_t> _nBatchesReturned;
    boost::optional<bool> _noCursorTimeout;
    boost::optional<bool> _tailable;
    boost::optional<bool> _awaitData;
    boost::optional<mongo::BSONObj> _originatingCommand;
    boost::optional<mongo::LogicalSessionId> _lsid;
    boost::optional<std::string> _planSummary;
    boost::optional<std::int64_t> _operationUsingCursorId;
    boost::optional<mongo::repl::OpTime> _lastKnownCommittedOpTime;
};

}  // namespace mongo
