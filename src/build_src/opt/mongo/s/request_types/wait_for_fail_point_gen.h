/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/wait_for_fail_point_gen.h --output build/opt/mongo/s/request_types/wait_for_fail_point_gen.cpp src/mongo/s/request_types/wait_for_fail_point.idl
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
 * wait for a fail point to be entered a certain number of times
 */
class WaitForFailPoint {
public:
    static constexpr auto kCommandParameterFieldName = "waitForFailPoint"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kMaxTimeMSFieldName = "maxTimeMS"_sd;
    static constexpr auto kTimesEnteredFieldName = "timesEntered"_sd;
    static constexpr auto kCommandName = "waitForFailPoint"_sd;

    explicit WaitForFailPoint(const std::string commandParameter);
    explicit WaitForFailPoint(const std::string commandParameter, std::int64_t timesEntered, std::int64_t maxTimeMS);

    static WaitForFailPoint parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static WaitForFailPoint parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const StringData getCommandParameter() const& { return _commandParameter; }
    void getCommandParameter() && = delete;

    /**
     * The target number of times the fail point has been entered.
     */
    std::int64_t getTimesEntered() const { return _timesEntered; }
    void setTimesEntered(std::int64_t value) & {  _timesEntered = std::move(value); _hasTimesEntered = true; }

    /**
     * The wait timeout in milliseconds.
     */
    std::int64_t getMaxTimeMS() const { return _maxTimeMS; }
    void setMaxTimeMS(std::int64_t value) & {  _maxTimeMS = std::move(value); _hasMaxTimeMS = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    std::string _commandParameter;

    std::int64_t _timesEntered;
    std::int64_t _maxTimeMS;
    std::string _dbName;
    bool _hasTimesEntered : 1;
    bool _hasMaxTimeMS : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
