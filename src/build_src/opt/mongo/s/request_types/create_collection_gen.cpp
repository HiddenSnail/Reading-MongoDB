/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/create_collection_gen.h --output build/opt/mongo/s/request_types/create_collection_gen.cpp src/mongo/s/request_types/create_collection.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/create_collection_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ConfigsvrCreateCollection::kCommandParameterFieldName;
constexpr StringData ConfigsvrCreateCollection::kDbNameFieldName;
constexpr StringData ConfigsvrCreateCollection::kOptionsFieldName;
constexpr StringData ConfigsvrCreateCollection::kCommandName;

const std::vector<StringData> ConfigsvrCreateCollection::_knownBSONFields {
    ConfigsvrCreateCollection::kOptionsFieldName,
    ConfigsvrCreateCollection::kCommandName,
};
const std::vector<StringData> ConfigsvrCreateCollection::_knownOP_MSGFields {
    ConfigsvrCreateCollection::kDbNameFieldName,
    ConfigsvrCreateCollection::kOptionsFieldName,
    ConfigsvrCreateCollection::kCommandName,
};

ConfigsvrCreateCollection::ConfigsvrCreateCollection(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}


ConfigsvrCreateCollection ConfigsvrCreateCollection::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    ConfigsvrCreateCollection object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrCreateCollection::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

        if (fieldName == kOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _options = element.Obj();
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

ConfigsvrCreateCollection ConfigsvrCreateCollection::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    ConfigsvrCreateCollection object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ConfigsvrCreateCollection::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

        if (fieldName == kOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _options = element.Obj();
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

void ConfigsvrCreateCollection::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    if (_options.is_initialized()) {
        builder->append(kOptionsFieldName, _options.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ConfigsvrCreateCollection::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        if (_options.is_initialized()) {
            builder->append(kOptionsFieldName, _options.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ConfigsvrCreateCollection::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
