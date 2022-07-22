/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/resize_oplog_gen.h --output build/opt/mongo/db/commands/resize_oplog_gen.cpp src/mongo/db/commands/resize_oplog.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/resize_oplog_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ReplSetResizeOplogRequest::kCommandParameterFieldName;
constexpr StringData ReplSetResizeOplogRequest::kDbNameFieldName;
constexpr StringData ReplSetResizeOplogRequest::kMinRetentionHoursFieldName;
constexpr StringData ReplSetResizeOplogRequest::kSizeFieldName;
constexpr StringData ReplSetResizeOplogRequest::kCommandName;

const std::vector<StringData> ReplSetResizeOplogRequest::_knownBSONFields {
    ReplSetResizeOplogRequest::kMinRetentionHoursFieldName,
    ReplSetResizeOplogRequest::kSizeFieldName,
    ReplSetResizeOplogRequest::kCommandName,
};
const std::vector<StringData> ReplSetResizeOplogRequest::_knownOP_MSGFields {
    ReplSetResizeOplogRequest::kDbNameFieldName,
    ReplSetResizeOplogRequest::kMinRetentionHoursFieldName,
    ReplSetResizeOplogRequest::kSizeFieldName,
    ReplSetResizeOplogRequest::kCommandName,
};

ReplSetResizeOplogRequest::ReplSetResizeOplogRequest(const std::int64_t commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}

void ReplSetResizeOplogRequest::validateSize(IDLParserErrorContext& ctxt, const double value)
{
    if (!(value >= 990)) {
        throwComparisonError<double>(ctxt, "size", ">="_sd, value, 990);
    }
    if (!(value <= 1073741824)) {
        throwComparisonError<double>(ctxt, "size", "<="_sd, value, 1073741824);
    }
}

void ReplSetResizeOplogRequest::validateSize(const double value)
{
    if (!(value >= 990)) {
        throwComparisonError<double>("size", ">="_sd, value, 990);
    }
    if (!(value <= 1073741824)) {
        throwComparisonError<double>("size", "<="_sd, value, 1073741824);
    }
}

void ReplSetResizeOplogRequest::validateMinRetentionHours(IDLParserErrorContext& ctxt, const double value)
{
    if (!(value >= 0)) {
        throwComparisonError<double>(ctxt, "minRetentionHours", ">="_sd, value, 0);
    }
}

void ReplSetResizeOplogRequest::validateMinRetentionHours(const double value)
{
    if (!(value >= 0)) {
        throwComparisonError<double>("minRetentionHours", ">="_sd, value, 0);
    }
}


ReplSetResizeOplogRequest ReplSetResizeOplogRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::int64_t localCmdType(-1);
    ReplSetResizeOplogRequest object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ReplSetResizeOplogRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                {
                    auto value = element._numberDouble();
                    validateSize(value);
                    _size = std::move(value);
                }
            }
        }
        else if (fieldName == kMinRetentionHoursFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                {
                    auto value = element._numberDouble();
                    validateMinRetentionHours(value);
                    _minRetentionHours = std::move(value);
                }
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertTypes(commandElement, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
            _commandParameter = commandElement.safeNumberLong();
        }
    }
}

ReplSetResizeOplogRequest ReplSetResizeOplogRequest::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::int64_t localCmdType(-1);
    ReplSetResizeOplogRequest object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ReplSetResizeOplogRequest::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                {
                    auto value = element._numberDouble();
                    validateSize(value);
                    _size = std::move(value);
                }
            }
        }
        else if (fieldName == kMinRetentionHoursFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                {
                    auto value = element._numberDouble();
                    validateMinRetentionHours(value);
                    _minRetentionHours = std::move(value);
                }
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertTypes(commandElement, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
            _commandParameter = commandElement.safeNumberLong();
        }
    }
}

void ReplSetResizeOplogRequest::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append(kCommandParameterFieldName, _commandParameter);
    if (_size.is_initialized()) {
        builder->append(kSizeFieldName, _size.get());
    }

    if (_minRetentionHours.is_initialized()) {
        builder->append(kMinRetentionHoursFieldName, _minRetentionHours.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ReplSetResizeOplogRequest::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append(kCommandParameterFieldName, _commandParameter);
        if (_size.is_initialized()) {
            builder->append(kSizeFieldName, _size.get());
        }

        if (_minRetentionHours.is_initialized()) {
            builder->append(kMinRetentionHoursFieldName, _minRetentionHours.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ReplSetResizeOplogRequest::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
