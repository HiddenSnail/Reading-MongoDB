/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/profile_gen.h --output build/opt/mongo/db/commands/profile_gen.cpp src/mongo/db/commands/profile.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/profile_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ProfileCmdRequest::kCommandParameterFieldName;
constexpr StringData ProfileCmdRequest::kDbNameFieldName;
constexpr StringData ProfileCmdRequest::kSampleRateFieldName;
constexpr StringData ProfileCmdRequest::kSlowmsFieldName;
constexpr StringData ProfileCmdRequest::kCommandName;

const std::vector<StringData> ProfileCmdRequest::_knownBSONFields {
    ProfileCmdRequest::kSampleRateFieldName,
    ProfileCmdRequest::kSlowmsFieldName,
    ProfileCmdRequest::kCommandName,
};
const std::vector<StringData> ProfileCmdRequest::_knownOP_MSGFields {
    ProfileCmdRequest::kDbNameFieldName,
    ProfileCmdRequest::kSampleRateFieldName,
    ProfileCmdRequest::kSlowmsFieldName,
    ProfileCmdRequest::kCommandName,
};

ProfileCmdRequest::ProfileCmdRequest(const std::int64_t commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}


ProfileCmdRequest ProfileCmdRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::int64_t localCmdType(-1);
    ProfileCmdRequest object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ProfileCmdRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kSlowmsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _slowms = element.safeNumberLong();
            }
        }
        else if (fieldName == kSampleRateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                _sampleRate = element._numberDouble();
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

ProfileCmdRequest ProfileCmdRequest::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::int64_t localCmdType(-1);
    ProfileCmdRequest object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ProfileCmdRequest::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kSlowmsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _slowms = element.safeNumberLong();
            }
        }
        else if (fieldName == kSampleRateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                _sampleRate = element._numberDouble();
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

void ProfileCmdRequest::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append(kCommandParameterFieldName, _commandParameter);
    if (_slowms.is_initialized()) {
        builder->append(kSlowmsFieldName, _slowms.get());
    }

    if (_sampleRate.is_initialized()) {
        builder->append(kSampleRateFieldName, _sampleRate.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ProfileCmdRequest::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append(kCommandParameterFieldName, _commandParameter);
        if (_slowms.is_initialized()) {
            builder->append(kSlowmsFieldName, _slowms.get());
        }

        if (_sampleRate.is_initialized()) {
            builder->append(kSampleRateFieldName, _sampleRate.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ProfileCmdRequest::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
