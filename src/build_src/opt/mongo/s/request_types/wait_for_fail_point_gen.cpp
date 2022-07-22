/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/wait_for_fail_point_gen.h --output build/opt/mongo/s/request_types/wait_for_fail_point_gen.cpp src/mongo/s/request_types/wait_for_fail_point.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/wait_for_fail_point_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData WaitForFailPoint::kCommandParameterFieldName;
constexpr StringData WaitForFailPoint::kDbNameFieldName;
constexpr StringData WaitForFailPoint::kMaxTimeMSFieldName;
constexpr StringData WaitForFailPoint::kTimesEnteredFieldName;
constexpr StringData WaitForFailPoint::kCommandName;

const std::vector<StringData> WaitForFailPoint::_knownBSONFields {
    WaitForFailPoint::kMaxTimeMSFieldName,
    WaitForFailPoint::kTimesEnteredFieldName,
    WaitForFailPoint::kCommandName,
};
const std::vector<StringData> WaitForFailPoint::_knownOP_MSGFields {
    WaitForFailPoint::kDbNameFieldName,
    WaitForFailPoint::kMaxTimeMSFieldName,
    WaitForFailPoint::kTimesEnteredFieldName,
    WaitForFailPoint::kCommandName,
};

WaitForFailPoint::WaitForFailPoint(const std::string commandParameter) : _commandParameter(std::move(commandParameter)), _timesEntered(-1), _maxTimeMS(-1), _hasTimesEntered(false), _hasMaxTimeMS(false), _hasDbName(false) {
    // Used for initialization only
}
WaitForFailPoint::WaitForFailPoint(const std::string commandParameter, std::int64_t timesEntered, std::int64_t maxTimeMS) : _commandParameter(std::move(commandParameter)), _timesEntered(std::move(timesEntered)), _maxTimeMS(std::move(maxTimeMS)), _hasTimesEntered(true), _hasMaxTimeMS(true), _hasDbName(true) {
    // Used for initialization only
}


WaitForFailPoint WaitForFailPoint::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::string localCmdType;
    WaitForFailPoint object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void WaitForFailPoint::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kTimesEnteredFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _hasTimesEntered = true;
                _timesEntered = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxTimeMSFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _hasMaxTimeMS = true;
                _maxTimeMS = element.safeNumberLong();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kTimesEnteredFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTimesEnteredFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMaxTimeMSFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMaxTimeMSFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = commandElement.str();
        }
    }
}

WaitForFailPoint WaitForFailPoint::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::string localCmdType;
    WaitForFailPoint object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void WaitForFailPoint::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kTimesEnteredFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _hasTimesEntered = true;
                _timesEntered = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxTimeMSFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _hasMaxTimeMS = true;
                _maxTimeMS = element.safeNumberLong();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kTimesEnteredFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTimesEnteredFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMaxTimeMSFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMaxTimeMSFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = commandElement.str();
        }
    }
}

void WaitForFailPoint::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasTimesEntered && _hasMaxTimeMS && _hasDbName);

    builder->append(kCommandParameterFieldName, _commandParameter);
    builder->append(kTimesEnteredFieldName, _timesEntered);

    builder->append(kMaxTimeMSFieldName, _maxTimeMS);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest WaitForFailPoint::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasTimesEntered && _hasMaxTimeMS && _hasDbName);

        builder->append(kCommandParameterFieldName, _commandParameter);
        builder->append(kTimesEnteredFieldName, _timesEntered);

        builder->append(kMaxTimeMSFieldName, _maxTimeMS);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj WaitForFailPoint::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
