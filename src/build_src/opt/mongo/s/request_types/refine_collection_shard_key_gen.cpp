/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/refine_collection_shard_key_gen.h --output build/opt/mongo/s/request_types/refine_collection_shard_key_gen.cpp src/mongo/s/request_types/refine_collection_shard_key.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/refine_collection_shard_key_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData RefineCollectionShardKey::kCommandParameterFieldName;
constexpr StringData RefineCollectionShardKey::kDbNameFieldName;
constexpr StringData RefineCollectionShardKey::kKeyFieldName;
constexpr StringData RefineCollectionShardKey::kCommandName;

const std::vector<StringData> RefineCollectionShardKey::_knownBSONFields {
    RefineCollectionShardKey::kKeyFieldName,
    RefineCollectionShardKey::kCommandName,
};
const std::vector<StringData> RefineCollectionShardKey::_knownOP_MSGFields {
    RefineCollectionShardKey::kDbNameFieldName,
    RefineCollectionShardKey::kKeyFieldName,
    RefineCollectionShardKey::kCommandName,
};

RefineCollectionShardKey::RefineCollectionShardKey(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasKey(false), _hasDbName(false) {
    // Used for initialization only
}
RefineCollectionShardKey::RefineCollectionShardKey(const mongo::NamespaceString commandParameter, mongo::BSONObj key) : _commandParameter(std::move(commandParameter)), _key(std::move(key)), _hasKey(true), _hasDbName(true) {
    // Used for initialization only
}


RefineCollectionShardKey RefineCollectionShardKey::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    RefineCollectionShardKey object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RefineCollectionShardKey::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
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

RefineCollectionShardKey RefineCollectionShardKey::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    RefineCollectionShardKey object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void RefineCollectionShardKey::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
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

void RefineCollectionShardKey::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasKey && _hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    builder->append(kKeyFieldName, _key);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest RefineCollectionShardKey::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasKey && _hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        builder->append(kKeyFieldName, _key);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj RefineCollectionShardKey::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData ConfigsvrRefineCollectionShardKey::kCommandParameterFieldName;
constexpr StringData ConfigsvrRefineCollectionShardKey::kDbNameFieldName;
constexpr StringData ConfigsvrRefineCollectionShardKey::kEpochFieldName;
constexpr StringData ConfigsvrRefineCollectionShardKey::kKeyFieldName;
constexpr StringData ConfigsvrRefineCollectionShardKey::kCommandName;

const std::vector<StringData> ConfigsvrRefineCollectionShardKey::_knownBSONFields {
    ConfigsvrRefineCollectionShardKey::kEpochFieldName,
    ConfigsvrRefineCollectionShardKey::kKeyFieldName,
    ConfigsvrRefineCollectionShardKey::kCommandName,
};
const std::vector<StringData> ConfigsvrRefineCollectionShardKey::_knownOP_MSGFields {
    ConfigsvrRefineCollectionShardKey::kDbNameFieldName,
    ConfigsvrRefineCollectionShardKey::kEpochFieldName,
    ConfigsvrRefineCollectionShardKey::kKeyFieldName,
    ConfigsvrRefineCollectionShardKey::kCommandName,
};

ConfigsvrRefineCollectionShardKey::ConfigsvrRefineCollectionShardKey(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasKey(false), _hasEpoch(false), _hasDbName(false) {
    // Used for initialization only
}
ConfigsvrRefineCollectionShardKey::ConfigsvrRefineCollectionShardKey(const mongo::NamespaceString commandParameter, mongo::BSONObj key, mongo::OID epoch) : _commandParameter(std::move(commandParameter)), _key(std::move(key)), _epoch(std::move(epoch)), _hasKey(true), _hasEpoch(true), _hasDbName(true) {
    // Used for initialization only
}


ConfigsvrRefineCollectionShardKey ConfigsvrRefineCollectionShardKey::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    ConfigsvrRefineCollectionShardKey object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrRefineCollectionShardKey::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kEpochFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                _hasEpoch = true;
                _epoch = element.OID();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kEpochFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kEpochFieldName);
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

ConfigsvrRefineCollectionShardKey ConfigsvrRefineCollectionShardKey::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    ConfigsvrRefineCollectionShardKey object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ConfigsvrRefineCollectionShardKey::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kEpochFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                _hasEpoch = true;
                _epoch = element.OID();
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kEpochFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kEpochFieldName);
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

void ConfigsvrRefineCollectionShardKey::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasKey && _hasEpoch && _hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    builder->append(kKeyFieldName, _key);

    builder->append(kEpochFieldName, _epoch);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ConfigsvrRefineCollectionShardKey::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasKey && _hasEpoch && _hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        builder->append(kKeyFieldName, _key);

        builder->append(kEpochFieldName, _epoch);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ConfigsvrRefineCollectionShardKey::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
