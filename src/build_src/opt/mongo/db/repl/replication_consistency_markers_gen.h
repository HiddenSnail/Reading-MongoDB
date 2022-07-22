/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/replication_consistency_markers_gen.h --output build/opt/mongo/db/repl/replication_consistency_markers_gen.cpp src/mongo/db/repl/replication_consistency_markers.idl
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
namespace repl {

/**
 * A document in which the server stores its minValid and appliedThrough information.
 */
class MinValidDocument {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kAppliedThroughFieldName = "begin"_sd;
    static constexpr auto kInitialSyncFlagFieldName = "doingInitialSync"_sd;
    static constexpr auto kMinValidTermFieldName = "t"_sd;
    static constexpr auto kMinValidTimestampFieldName = "ts"_sd;

    MinValidDocument();
    MinValidDocument(mongo::Timestamp minValidTimestamp, std::int64_t minValidTerm);

    static MinValidDocument parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The timestamp for the OpTime at which the data on disk will be consistent
     */
    const mongo::Timestamp& getMinValidTimestamp() const { return _minValidTimestamp; }
    void setMinValidTimestamp(mongo::Timestamp value) & {  _minValidTimestamp = std::move(value); _hasMinValidTimestamp = true; }

    /**
     * The term for the OpTime at which the data on disk will be consistent; -1 for PV0.
     */
    std::int64_t getMinValidTerm() const { return _minValidTerm; }
    void setMinValidTerm(std::int64_t value) & {  _minValidTerm = std::move(value); _hasMinValidTerm = true; }

    /**
     * The OpTime of the last oplog entry we applied
     */
    const boost::optional<mongo::repl::OpTime>& getAppliedThrough() const& { return _appliedThrough; }
    void getAppliedThrough() && = delete;
    void setAppliedThrough(boost::optional<mongo::repl::OpTime> value) & {  _appliedThrough = std::move(value);  }

    /**
     * A flag for if we are in the middle of initial sync
     */
    const boost::optional<bool> getInitialSyncFlag() const& { return _initialSyncFlag; }
    void getInitialSyncFlag() && = delete;
    void setInitialSyncFlag(boost::optional<bool> value) & {  _initialSyncFlag = std::move(value);  }

    /**
     * An objectid that is not used but is automatically generated
     */
    const boost::optional<mongo::OID>& get_id() const& { return __id; }
    void get_id() && = delete;
    void set_id(boost::optional<mongo::OID> value) & {  __id = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::Timestamp _minValidTimestamp;
    std::int64_t _minValidTerm;
    boost::optional<mongo::repl::OpTime> _appliedThrough;
    boost::optional<bool> _initialSyncFlag;
    boost::optional<mongo::OID> __id;
    bool _hasMinValidTimestamp : 1;
    bool _hasMinValidTerm : 1;
};

/**
 * A document in which the server stores information on where to truncate the oplog on unclean shutdown.
 */
class OplogTruncateAfterPointDocument {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kOplogTruncateAfterPointFieldName = "oplogTruncateAfterPoint"_sd;

    OplogTruncateAfterPointDocument();
    OplogTruncateAfterPointDocument(mongo::Timestamp oplogTruncateAfterPoint, std::string _id);

    static OplogTruncateAfterPointDocument parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The timestamp of an oplog entry after which the oplog may be inconsistent (have holes due to parallel writes).
     */
    const mongo::Timestamp& getOplogTruncateAfterPoint() const { return _oplogTruncateAfterPoint; }
    void setOplogTruncateAfterPoint(mongo::Timestamp value) & {  _oplogTruncateAfterPoint = std::move(value); _hasOplogTruncateAfterPoint = true; }

    /**
     * Always set to 'oplogTruncateAfterPoint' to easily retrieve it.
     */
    const StringData get_id() const& { return __id; }
    void get_id() && = delete;
    void set_id(StringData value) & { auto _tmpValue = value.toString();  __id = std::move(_tmpValue); _has_id = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::Timestamp _oplogTruncateAfterPoint;
    std::string __id;
    bool _hasOplogTruncateAfterPoint : 1;
    bool _has_id : 1;
};

/**
 * A document in which the server stores data related to the initial sync of the server.
 */
class InitialSyncIdDocument {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kWallTimeFieldName = "wallTime"_sd;

    InitialSyncIdDocument();
    InitialSyncIdDocument(mongo::UUID _id);

    static InitialSyncIdDocument parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * An arbitrary unique identifier associated with the initial sync of the server.
     */
    const mongo::UUID& get_id() const { return __id; }
    void set_id(mongo::UUID value) & {  __id = std::move(value); _has_id = true; }

    /**
     * The walltime at which the initial sync document was written.
     */
    const boost::optional<mongo::Date_t>& getWallTime() const& { return _wallTime; }
    void getWallTime() && = delete;
    void setWallTime(boost::optional<mongo::Date_t> value) & {  _wallTime = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID __id;
    boost::optional<mongo::Date_t> _wallTime;
    bool _has_id : 1;
};

}  // namespace repl
}  // namespace mongo
