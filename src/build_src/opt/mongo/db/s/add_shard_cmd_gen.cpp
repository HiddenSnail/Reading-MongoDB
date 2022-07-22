/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/add_shard_cmd_gen.h --output build/opt/mongo/db/s/add_shard_cmd_gen.cpp src/mongo/db/s/add_shard_cmd.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/s/add_shard_cmd_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ShardIdentity::kClusterIdFieldName;
constexpr StringData ShardIdentity::kConfigsvrConnectionStringFieldName;
constexpr StringData ShardIdentity::kShardNameFieldName;


ShardIdentity::ShardIdentity() : _hasShardName(false), _hasClusterId(false), _hasConfigsvrConnectionString(false) {
    // Used for initialization only
}
ShardIdentity::ShardIdentity(std::string shardName, mongo::OID clusterId, mongo::ConnectionString configsvrConnectionString) : _shardName(std::move(shardName)), _clusterId(std::move(clusterId)), _configsvrConnectionString(std::move(configsvrConnectionString)), _hasShardName(true), _hasClusterId(true), _hasConfigsvrConnectionString(true) {
    // Used for initialization only
}


ShardIdentity ShardIdentity::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ShardIdentity object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ShardIdentity::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kShardNameBit = 0;
    const size_t kClusterIdBit = 1;
    const size_t kConfigsvrConnectionStringBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kShardNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kShardNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kShardNameBit);

                _hasShardName = true;
                _shardName = element.str();
            }
        }
        else if (fieldName == kClusterIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                if (MONGO_unlikely(usedFields[kClusterIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kClusterIdBit);

                _hasClusterId = true;
                _clusterId = element.OID();
            }
        }
        else if (fieldName == kConfigsvrConnectionStringFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kConfigsvrConnectionStringBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kConfigsvrConnectionStringBit);

                _hasConfigsvrConnectionString = true;
                _configsvrConnectionString = ConnectionString::deserialize(element.valueStringData());
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kShardNameBit]) {
            ctxt.throwMissingField(kShardNameFieldName);
        }
        if (!usedFields[kClusterIdBit]) {
            ctxt.throwMissingField(kClusterIdFieldName);
        }
        if (!usedFields[kConfigsvrConnectionStringBit]) {
            ctxt.throwMissingField(kConfigsvrConnectionStringFieldName);
        }
    }

}


void ShardIdentity::serialize(BSONObjBuilder* builder) const {
    invariant(_hasShardName && _hasClusterId && _hasConfigsvrConnectionString);

    builder->append(kShardNameFieldName, _shardName);

    builder->append(kClusterIdFieldName, _clusterId);

    {
        builder->append(kConfigsvrConnectionStringFieldName, _configsvrConnectionString.toString());
    }

}


BSONObj ShardIdentity::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AddShard::kDbNameFieldName;
constexpr StringData AddShard::kShardIdentityFieldName;
constexpr StringData AddShard::kCommandName;

const std::vector<StringData> AddShard::_knownBSONFields {
    AddShard::kShardIdentityFieldName,
    AddShard::kCommandName,
};
const std::vector<StringData> AddShard::_knownOP_MSGFields {
    AddShard::kDbNameFieldName,
    AddShard::kShardIdentityFieldName,
    AddShard::kCommandName,
};

AddShard::AddShard() : _hasShardIdentity(false), _hasDbName(false) {
    // Used for initialization only
}
AddShard::AddShard(mongo::ShardIdentity shardIdentity) : _shardIdentity(std::move(shardIdentity)), _hasShardIdentity(true), _hasDbName(true) {
    // Used for initialization only
}


AddShard AddShard::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AddShard object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AddShard::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kShardIdentityBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kShardIdentityFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kShardIdentityBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kShardIdentityBit);

                _hasShardIdentity = true;
                IDLParserErrorContext tempContext(kShardIdentityFieldName, &ctxt);
                const auto localObject = element.Obj();
                _shardIdentity = mongo::ShardIdentity::parse(tempContext, localObject);
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
        if (!usedFields[kShardIdentityBit]) {
            ctxt.throwMissingField(kShardIdentityFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

AddShard AddShard::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    AddShard object;
    object.parseProtected(ctxt, request);
    return object;
}
void AddShard::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<2> usedFields;
    const size_t kShardIdentityBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kShardIdentityFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kShardIdentityBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kShardIdentityBit);

                _hasShardIdentity = true;
                IDLParserErrorContext tempContext(kShardIdentityFieldName, &ctxt);
                const auto localObject = element.Obj();
                _shardIdentity = mongo::ShardIdentity::parse(tempContext, localObject);
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
        if (!usedFields[kShardIdentityBit]) {
            ctxt.throwMissingField(kShardIdentityFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void AddShard::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasShardIdentity && _hasDbName);

    builder->append("_addShard"_sd, 1);
    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kShardIdentityFieldName));
        _shardIdentity.serialize(&subObjBuilder);
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest AddShard::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasShardIdentity && _hasDbName);

        builder->append("_addShard"_sd, 1);
        {
            BSONObjBuilder subObjBuilder(builder->subobjStart(kShardIdentityFieldName));
            _shardIdentity.serialize(&subObjBuilder);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj AddShard::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
