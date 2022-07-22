/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/clone_collection_options_from_primary_shard_gen.h --output build/opt/mongo/s/request_types/clone_collection_options_from_primary_shard_gen.cpp src/mongo/s/request_types/clone_collection_options_from_primary_shard.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/clone_collection_options_from_primary_shard_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData CloneCollectionOptionsFromPrimaryShard::kCommandParameterFieldName;
constexpr StringData CloneCollectionOptionsFromPrimaryShard::kDbNameFieldName;
constexpr StringData CloneCollectionOptionsFromPrimaryShard::kPrimaryShardFieldName;
constexpr StringData CloneCollectionOptionsFromPrimaryShard::kCommandName;

const std::vector<StringData> CloneCollectionOptionsFromPrimaryShard::_knownBSONFields {
    CloneCollectionOptionsFromPrimaryShard::kPrimaryShardFieldName,
    CloneCollectionOptionsFromPrimaryShard::kCommandName,
};
const std::vector<StringData> CloneCollectionOptionsFromPrimaryShard::_knownOP_MSGFields {
    CloneCollectionOptionsFromPrimaryShard::kDbNameFieldName,
    CloneCollectionOptionsFromPrimaryShard::kPrimaryShardFieldName,
    CloneCollectionOptionsFromPrimaryShard::kCommandName,
};

CloneCollectionOptionsFromPrimaryShard::CloneCollectionOptionsFromPrimaryShard(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasPrimaryShard(false), _hasDbName(false) {
    // Used for initialization only
}
CloneCollectionOptionsFromPrimaryShard::CloneCollectionOptionsFromPrimaryShard(const mongo::NamespaceString commandParameter, std::string primaryShard) : _commandParameter(std::move(commandParameter)), _primaryShard(std::move(primaryShard)), _hasPrimaryShard(true), _hasDbName(true) {
    // Used for initialization only
}


CloneCollectionOptionsFromPrimaryShard CloneCollectionOptionsFromPrimaryShard::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    CloneCollectionOptionsFromPrimaryShard object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CloneCollectionOptionsFromPrimaryShard::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kPrimaryShardBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kPrimaryShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kPrimaryShardBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPrimaryShardBit);

                _hasPrimaryShard = true;
                _primaryShard = element.str();
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
        if (!usedFields[kPrimaryShardBit]) {
            ctxt.throwMissingField(kPrimaryShardFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

CloneCollectionOptionsFromPrimaryShard CloneCollectionOptionsFromPrimaryShard::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    CloneCollectionOptionsFromPrimaryShard object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void CloneCollectionOptionsFromPrimaryShard::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<2> usedFields;
    const size_t kPrimaryShardBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kPrimaryShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kPrimaryShardBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPrimaryShardBit);

                _hasPrimaryShard = true;
                _primaryShard = element.str();
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
        if (!usedFields[kPrimaryShardBit]) {
            ctxt.throwMissingField(kPrimaryShardFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

void CloneCollectionOptionsFromPrimaryShard::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasPrimaryShard && _hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    builder->append(kPrimaryShardFieldName, _primaryShard);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest CloneCollectionOptionsFromPrimaryShard::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasPrimaryShard && _hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        builder->append(kPrimaryShardFieldName, _primaryShard);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj CloneCollectionOptionsFromPrimaryShard::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
