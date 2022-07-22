/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/health_log_gen.h --output build/opt/mongo/db/catalog/health_log_gen.cpp src/mongo/db/catalog/health_log.idl
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
 * The severity of a healthlog entry.
 */
enum class SeverityEnum : std::int32_t {
    Info ,
    Warning ,
    Error ,
};

SeverityEnum Severity_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData Severity_serializer(SeverityEnum value);

/**
 * The scope covered by a healthlog entry.
 */
enum class ScopeEnum : std::int32_t {
    Cluster ,
    Node ,
    Database ,
    Collection ,
    Index ,
    Document ,
};

ScopeEnum Scope_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData Scope_serializer(ScopeEnum value);

/**
 * An entry in system.local.healthlog.
 */
class HealthLogEntry {
public:
    static constexpr auto kDataFieldName = "data"_sd;
    static constexpr auto kMsgFieldName = "msg"_sd;
    static constexpr auto kNssFieldName = "namespace"_sd;
    static constexpr auto kOperationFieldName = "operation"_sd;
    static constexpr auto kScopeFieldName = "scope"_sd;
    static constexpr auto kSeverityFieldName = "severity"_sd;
    static constexpr auto kTimestampFieldName = "timestamp"_sd;

    HealthLogEntry();
    HealthLogEntry(mongo::NamespaceString nss, mongo::Date_t timestamp, mongo::SeverityEnum severity, std::string msg, mongo::ScopeEnum scope, std::string operation, mongo::BSONObj data);

    static HealthLogEntry parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::NamespaceString& getNss() const { return _nss; }
    void setNss(mongo::NamespaceString value) & {  _nss = std::move(value); _hasNss = true; }

    const mongo::Date_t& getTimestamp() const { return _timestamp; }
    void setTimestamp(mongo::Date_t value) & {  _timestamp = std::move(value); _hasTimestamp = true; }

    mongo::SeverityEnum getSeverity() const { return _severity; }
    void setSeverity(mongo::SeverityEnum value) & {  _severity = std::move(value); _hasSeverity = true; }

    const StringData getMsg() const& { return _msg; }
    void getMsg() && = delete;
    void setMsg(StringData value) & { auto _tmpValue = value.toString();  _msg = std::move(_tmpValue); _hasMsg = true; }

    mongo::ScopeEnum getScope() const { return _scope; }
    void setScope(mongo::ScopeEnum value) & {  _scope = std::move(value); _hasScope = true; }

    const StringData getOperation() const& { return _operation; }
    void getOperation() && = delete;
    void setOperation(StringData value) & { auto _tmpValue = value.toString();  _operation = std::move(_tmpValue); _hasOperation = true; }

    const mongo::BSONObj& getData() const { return _data; }
    void setData(mongo::BSONObj value) & {  _data = std::move(value); _hasData = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::NamespaceString _nss;
    mongo::Date_t _timestamp;
    mongo::SeverityEnum _severity;
    std::string _msg;
    mongo::ScopeEnum _scope;
    std::string _operation;
    mongo::BSONObj _data;
    bool _hasNss : 1;
    bool _hasTimestamp : 1;
    bool _hasSeverity : 1;
    bool _hasMsg : 1;
    bool _hasScope : 1;
    bool _hasOperation : 1;
    bool _hasData : 1;
};

}  // namespace mongo
