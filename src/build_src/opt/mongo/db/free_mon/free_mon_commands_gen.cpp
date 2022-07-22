/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_commands_gen.h --output build/opt/mongo/db/free_mon/free_mon_commands_gen.cpp src/mongo/db/free_mon/free_mon_commands.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/free_mon/free_mon_commands_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Action types
 */
namespace  {
constexpr StringData kSetFreeMonAction_enable = "enable"_sd;
constexpr StringData kSetFreeMonAction_disable = "disable"_sd;
}  // namespace 

SetFreeMonActionEnum SetFreeMonAction_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kSetFreeMonAction_enable) {
        return SetFreeMonActionEnum::enable;
    }
    if (value == kSetFreeMonAction_disable) {
        return SetFreeMonActionEnum::disable;
    }
    ctxt.throwBadEnumValue(value);
}

StringData SetFreeMonAction_serializer(SetFreeMonActionEnum value) {
    if (value == SetFreeMonActionEnum::enable) {
        return kSetFreeMonAction_enable;
    }
    if (value == SetFreeMonActionEnum::disable) {
        return kSetFreeMonAction_disable;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData SetFreeMonitoring::kActionFieldName;
constexpr StringData SetFreeMonitoring::kDbNameFieldName;
constexpr StringData SetFreeMonitoring::kCommandName;

const std::vector<StringData> SetFreeMonitoring::_knownBSONFields {
    SetFreeMonitoring::kActionFieldName,
    SetFreeMonitoring::kCommandName,
};
const std::vector<StringData> SetFreeMonitoring::_knownOP_MSGFields {
    SetFreeMonitoring::kActionFieldName,
    SetFreeMonitoring::kDbNameFieldName,
    SetFreeMonitoring::kCommandName,
};

SetFreeMonitoring::SetFreeMonitoring() : _hasAction(false), _hasDbName(false) {
    // Used for initialization only
}
SetFreeMonitoring::SetFreeMonitoring(mongo::SetFreeMonActionEnum action) : _action(std::move(action)), _hasAction(true), _hasDbName(true) {
    // Used for initialization only
}


SetFreeMonitoring SetFreeMonitoring::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    SetFreeMonitoring object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void SetFreeMonitoring::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kActionBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kActionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kActionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kActionBit);

                _hasAction = true;
                IDLParserErrorContext tempContext(kActionFieldName, &ctxt);
                _action = SetFreeMonAction_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kActionBit]) {
            ctxt.throwMissingField(kActionFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

SetFreeMonitoring SetFreeMonitoring::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    SetFreeMonitoring object;
    object.parseProtected(ctxt, request);
    return object;
}
void SetFreeMonitoring::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<2> usedFields;
    const size_t kActionBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kActionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kActionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kActionBit);

                _hasAction = true;
                IDLParserErrorContext tempContext(kActionFieldName, &ctxt);
                _action = SetFreeMonAction_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kActionBit]) {
            ctxt.throwMissingField(kActionFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void SetFreeMonitoring::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasAction && _hasDbName);

    builder->append("setFreeMonitoring"_sd, 1);
    {
        builder->append(kActionFieldName, SetFreeMonAction_serializer(_action));
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest SetFreeMonitoring::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasAction && _hasDbName);

        builder->append("setFreeMonitoring"_sd, 1);
        {
            builder->append(kActionFieldName, SetFreeMonAction_serializer(_action));
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj SetFreeMonitoring::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData GetFreeMonitoringStatus::kDbNameFieldName;
constexpr StringData GetFreeMonitoringStatus::kCommandName;

const std::vector<StringData> GetFreeMonitoringStatus::_knownBSONFields {
    GetFreeMonitoringStatus::kCommandName,
};
const std::vector<StringData> GetFreeMonitoringStatus::_knownOP_MSGFields {
    GetFreeMonitoringStatus::kDbNameFieldName,
    GetFreeMonitoringStatus::kCommandName,
};

GetFreeMonitoringStatus::GetFreeMonitoringStatus() : _hasDbName(false) {
    // Used for initialization only
}


GetFreeMonitoringStatus GetFreeMonitoringStatus::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    GetFreeMonitoringStatus object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void GetFreeMonitoringStatus::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

GetFreeMonitoringStatus GetFreeMonitoringStatus::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    GetFreeMonitoringStatus object;
    object.parseProtected(ctxt, request);
    return object;
}
void GetFreeMonitoringStatus::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void GetFreeMonitoringStatus::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append("getFreeMonitoringStatus"_sd, 1);
    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest GetFreeMonitoringStatus::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append("getFreeMonitoringStatus"_sd, 1);
        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj GetFreeMonitoringStatus::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
