/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/rwc_defaults_commands_gen.h --output build/opt/mongo/db/commands/rwc_defaults_commands_gen.cpp src/mongo/db/commands/rwc_defaults_commands.idl
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
#include "mongo/db/read_write_concern_defaults_gen.h"
#include "mongo/db/repl/read_concern_args.h"
#include "mongo/db/write_concern_options.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Describes the response for the getDefaultRWConcern command
 */
class GetDefaultRWConcernResponse {
public:
    static constexpr auto kRWConcernDefaultFieldName = "RWConcernDefault"_sd;
    static constexpr auto kDefaultReadConcernFieldName = "defaultReadConcern"_sd;
    static constexpr auto kDefaultWriteConcernFieldName = "defaultWriteConcern"_sd;
    static constexpr auto kInMemoryFieldName = "inMemory"_sd;
    static constexpr auto kLocalUpdateWallClockTimeFieldName = "localUpdateWallClockTime"_sd;
    static constexpr auto kUpdateOpTimeFieldName = "updateOpTime"_sd;
    static constexpr auto kUpdateWallClockTimeFieldName = "updateWallClockTime"_sd;

    GetDefaultRWConcernResponse();
    GetDefaultRWConcernResponse(mongo::Date_t localUpdateWallClockTime);

    static GetDefaultRWConcernResponse parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Represents a set of read/write concern defaults, and associated metadata
     */
    const RWConcernDefault& getRWConcernDefault() const { return _rWConcernDefault; }
    RWConcernDefault& getRWConcernDefault() { return _rWConcernDefault; }
    void setRWConcernDefault(RWConcernDefault value) & {  _rWConcernDefault = std::move(value);  }

    /**
     * The default read concern
     */
    const  boost::optional<mongo::repl::ReadConcernArgs>& getDefaultReadConcern() const { return _rWConcernDefault.getDefaultReadConcern(); }
    /**
     * The default write concern
     */
    const  boost::optional<mongo::WriteConcernOptions>& getDefaultWriteConcern() const { return _rWConcernDefault.getDefaultWriteConcern(); }
    /**
     * The optime of when the default read or write concern was last set. On replica sets it advances with the primary's optime and on clusters it advances with the config server primary's optime.
     */
    const  boost::optional<mongo::Timestamp>& getUpdateOpTime() const { return _rWConcernDefault.getUpdateOpTime(); }
    /**
     * The wallclock time when the default read or write concern was last set by an administrator. This value is only informational and must not be used for any recency comparisons.
     */
    const  boost::optional<mongo::Date_t>& getUpdateWallClockTime() const { return _rWConcernDefault.getUpdateWallClockTime(); }
    /**
     * Whether the value came from the in-memory cache or it reflects the defaults which were persisted as of the time of invocation.
     */
    const boost::optional<bool> getInMemory() const& { return _inMemory; }
    void getInMemory() && = delete;
    void setInMemory(boost::optional<bool> value) & {  _inMemory = std::move(value);  }

    /**
     * The wallclock time of when the default read or write concern was applied to the cache of the node, which returned the response. This value is only informational and must not be used for any recency comparisons.
     */
    const mongo::Date_t& getLocalUpdateWallClockTime() const { return _localUpdateWallClockTime; }
    void setLocalUpdateWallClockTime(mongo::Date_t value) & {  _localUpdateWallClockTime = std::move(value); _hasLocalUpdateWallClockTime = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    RWConcernDefault _rWConcernDefault;
    boost::optional<bool> _inMemory;
    mongo::Date_t _localUpdateWallClockTime;
    bool _hasLocalUpdateWallClockTime : 1;
};

/**
 * Set the current read/write concern defaults (cluster-wide)
 */
class SetDefaultRWConcern {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kDefaultReadConcernFieldName = "defaultReadConcern"_sd;
    static constexpr auto kDefaultWriteConcernFieldName = "defaultWriteConcern"_sd;
    static constexpr auto kCommandName = "setDefaultRWConcern"_sd;

    SetDefaultRWConcern();

    static SetDefaultRWConcern parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static SetDefaultRWConcern parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * The new default read concern
     */
    const boost::optional<mongo::repl::ReadConcernArgs>& getDefaultReadConcern() const& { return _defaultReadConcern; }
    void getDefaultReadConcern() && = delete;
    void setDefaultReadConcern(boost::optional<mongo::repl::ReadConcernArgs> value) & {  _defaultReadConcern = std::move(value);  }

    /**
     * The new default write concern
     */
    const boost::optional<mongo::WriteConcernOptions>& getDefaultWriteConcern() const& { return _defaultWriteConcern; }
    void getDefaultWriteConcern() && = delete;
    void setDefaultWriteConcern(boost::optional<mongo::WriteConcernOptions> value) & {  _defaultWriteConcern = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    boost::optional<mongo::repl::ReadConcernArgs> _defaultReadConcern;
    boost::optional<mongo::WriteConcernOptions> _defaultWriteConcern;
    std::string _dbName;
    bool _hasDbName : 1;
};

/**
 * Get the current read/write concern defaults being applied by this node
 */
class GetDefaultRWConcern {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kInMemoryFieldName = "inMemory"_sd;
    static constexpr auto kCommandName = "getDefaultRWConcern"_sd;

    GetDefaultRWConcern();

    static GetDefaultRWConcern parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static GetDefaultRWConcern parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * If true, return the locally cached read/write concern defaults
     */
    const boost::optional<bool> getInMemory() const& { return _inMemory; }
    void getInMemory() && = delete;
    void setInMemory(boost::optional<bool> value) & {  _inMemory = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    boost::optional<bool> _inMemory;
    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
