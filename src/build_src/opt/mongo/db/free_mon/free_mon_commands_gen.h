/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_commands_gen.h --output build/opt/mongo/db/free_mon/free_mon_commands_gen.cpp src/mongo/db/free_mon/free_mon_commands.idl
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
 * Action types
 */
enum class SetFreeMonActionEnum : std::int32_t {
    enable ,
    disable ,
};

SetFreeMonActionEnum SetFreeMonAction_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData SetFreeMonAction_serializer(SetFreeMonActionEnum value);

/**
 * setFreeMonitoring Command
 */
class SetFreeMonitoring {
public:
    static constexpr auto kActionFieldName = "action"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "setFreeMonitoring"_sd;

    SetFreeMonitoring();
    SetFreeMonitoring(mongo::SetFreeMonActionEnum action);

    static SetFreeMonitoring parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static SetFreeMonitoring parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * Action to take
     */
    mongo::SetFreeMonActionEnum getAction() const { return _action; }
    void setAction(mongo::SetFreeMonActionEnum value) & {  _action = std::move(value); _hasAction = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    mongo::SetFreeMonActionEnum _action;
    std::string _dbName;
    bool _hasAction : 1;
    bool _hasDbName : 1;
};

/**
 * getFreeMonitoringStatus Command
 */
class GetFreeMonitoringStatus {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "getFreeMonitoringStatus"_sd;

    GetFreeMonitoringStatus();

    static GetFreeMonitoringStatus parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static GetFreeMonitoringStatus parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
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
