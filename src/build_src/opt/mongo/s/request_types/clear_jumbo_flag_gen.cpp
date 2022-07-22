/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/clear_jumbo_flag_gen.h --output build/opt/mongo/s/request_types/clear_jumbo_flag_gen.cpp src/mongo/s/request_types/clear_jumbo_flag.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/clear_jumbo_flag_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ClearJumboFlag::kCommandParameterFieldName;
constexpr StringData ClearJumboFlag::kBoundsFieldName;
constexpr StringData ClearJumboFlag::kDbNameFieldName;
constexpr StringData ClearJumboFlag::kFindFieldName;
constexpr StringData ClearJumboFlag::kCommandName;

const std::vector<StringData> ClearJumboFlag::_knownBSONFields {
    ClearJumboFlag::kBoundsFieldName,
    ClearJumboFlag::kFindFieldName,
    ClearJumboFlag::kCommandName,
};
const std::vector<StringData> ClearJumboFlag::_knownOP_MSGFields {
    ClearJumboFlag::kBoundsFieldName,
    ClearJumboFlag::kDbNameFieldName,
    ClearJumboFlag::kFindFieldName,
    ClearJumboFlag::kCommandName,
};

ClearJumboFlag::ClearJumboFlag(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}


ClearJumboFlag ClearJumboFlag::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    ClearJumboFlag object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ClearJumboFlag::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kBoundsFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kBoundsFieldName, &ctxt);
            std::vector<mongo::BSONObj> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        values.emplace_back(arrayElement.Obj());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _bounds = std::move(values);
        }
        else if (fieldName == kFindFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _find = element.Obj();
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
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

ClearJumboFlag ClearJumboFlag::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    ClearJumboFlag object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ClearJumboFlag::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kBoundsFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kBoundsFieldName, &ctxt);
            std::vector<mongo::BSONObj> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        values.emplace_back(arrayElement.Obj());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _bounds = std::move(values);
        }
        else if (fieldName == kFindFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _find = element.Obj();
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
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

void ClearJumboFlag::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    if (_bounds.is_initialized()) {
        builder->append(kBoundsFieldName, _bounds.get());
    }

    if (_find.is_initialized()) {
        builder->append(kFindFieldName, _find.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ClearJumboFlag::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        if (_bounds.is_initialized()) {
            builder->append(kBoundsFieldName, _bounds.get());
        }

        if (_find.is_initialized()) {
            builder->append(kFindFieldName, _find.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ClearJumboFlag::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData ConfigsvrClearJumboFlag::kCommandParameterFieldName;
constexpr StringData ConfigsvrClearJumboFlag::kDbNameFieldName;
constexpr StringData ConfigsvrClearJumboFlag::kEpochFieldName;
constexpr StringData ConfigsvrClearJumboFlag::kMaxKeyFieldName;
constexpr StringData ConfigsvrClearJumboFlag::kMinKeyFieldName;
constexpr StringData ConfigsvrClearJumboFlag::kCommandName;

const std::vector<StringData> ConfigsvrClearJumboFlag::_knownBSONFields {
    ConfigsvrClearJumboFlag::kEpochFieldName,
    ConfigsvrClearJumboFlag::kMaxKeyFieldName,
    ConfigsvrClearJumboFlag::kMinKeyFieldName,
    ConfigsvrClearJumboFlag::kCommandName,
};
const std::vector<StringData> ConfigsvrClearJumboFlag::_knownOP_MSGFields {
    ConfigsvrClearJumboFlag::kDbNameFieldName,
    ConfigsvrClearJumboFlag::kEpochFieldName,
    ConfigsvrClearJumboFlag::kMaxKeyFieldName,
    ConfigsvrClearJumboFlag::kMinKeyFieldName,
    ConfigsvrClearJumboFlag::kCommandName,
};

ConfigsvrClearJumboFlag::ConfigsvrClearJumboFlag(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasEpoch(false), _hasMinKey(false), _hasMaxKey(false), _hasDbName(false) {
    // Used for initialization only
}
ConfigsvrClearJumboFlag::ConfigsvrClearJumboFlag(const mongo::NamespaceString commandParameter, mongo::OID epoch, mongo::BSONObj minKey, mongo::BSONObj maxKey) : _commandParameter(std::move(commandParameter)), _epoch(std::move(epoch)), _minKey(std::move(minKey)), _maxKey(std::move(maxKey)), _hasEpoch(true), _hasMinKey(true), _hasMaxKey(true), _hasDbName(true) {
    // Used for initialization only
}


ConfigsvrClearJumboFlag ConfigsvrClearJumboFlag::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    ConfigsvrClearJumboFlag object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrClearJumboFlag::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kEpochFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                _hasEpoch = true;
                _epoch = element.OID();
            }
        }
        else if (fieldName == kMinKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasMinKey = true;
                _minKey = element.Obj();
            }
        }
        else if (fieldName == kMaxKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasMaxKey = true;
                _maxKey = element.Obj();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kEpochFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kEpochFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMinKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMinKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMaxKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMaxKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

ConfigsvrClearJumboFlag ConfigsvrClearJumboFlag::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    ConfigsvrClearJumboFlag object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ConfigsvrClearJumboFlag::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kEpochFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                _hasEpoch = true;
                _epoch = element.OID();
            }
        }
        else if (fieldName == kMinKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasMinKey = true;
                _minKey = element.Obj();
            }
        }
        else if (fieldName == kMaxKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasMaxKey = true;
                _maxKey = element.Obj();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kEpochFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kEpochFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMinKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMinKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMaxKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMaxKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

void ConfigsvrClearJumboFlag::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasEpoch && _hasMinKey && _hasMaxKey && _hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    builder->append(kEpochFieldName, _epoch);

    builder->append(kMinKeyFieldName, _minKey);

    builder->append(kMaxKeyFieldName, _maxKey);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ConfigsvrClearJumboFlag::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasEpoch && _hasMinKey && _hasMaxKey && _hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        builder->append(kEpochFieldName, _epoch);

        builder->append(kMinKeyFieldName, _minKey);

        builder->append(kMaxKeyFieldName, _maxKey);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ConfigsvrClearJumboFlag::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
