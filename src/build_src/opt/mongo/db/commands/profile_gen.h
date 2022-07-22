/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/profile_gen.h --output build/opt/mongo/db/commands/profile_gen.cpp src/mongo/db/commands/profile.idl
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
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Parser for the 'profile' command.
 */
class ProfileCmdRequest {
public:
    static constexpr auto kCommandParameterFieldName = "profile"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kSampleRateFieldName = "sampleRate"_sd;
    static constexpr auto kSlowmsFieldName = "slowms"_sd;
    static constexpr auto kCommandName = "profile"_sd;

    explicit ProfileCmdRequest(const std::int64_t commandParameter);

    static ProfileCmdRequest parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ProfileCmdRequest parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    std::int64_t getCommandParameter() const { return _commandParameter; }

    /**
     * The threshold, in milliseconds, at which ops are profiled and logged.
     */
    const boost::optional<std::int64_t> getSlowms() const& { return _slowms; }
    void getSlowms() && = delete;
    void setSlowms(boost::optional<std::int64_t> value) & {  _slowms = std::move(value);  }

    /**
     * The fraction of eligible ops, in the range [0,1), to be logged/profiled.
     */
    const boost::optional<double> getSampleRate() const& { return _sampleRate; }
    void getSampleRate() && = delete;
    void setSampleRate(boost::optional<double> value) & {  _sampleRate = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    std::int64_t _commandParameter;

    boost::optional<std::int64_t> _slowms;
    boost::optional<double> _sampleRate;
    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
