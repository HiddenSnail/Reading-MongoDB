/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/ensure_chunk_version_is_greater_than_gen.h --output build/opt/mongo/s/request_types/ensure_chunk_version_is_greater_than_gen.cpp src/mongo/s/request_types/ensure_chunk_version_is_greater_than.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/ensure_chunk_version_is_greater_than_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ConfigsvrEnsureChunkVersionIsGreaterThan::kDbNameFieldName;
constexpr StringData ConfigsvrEnsureChunkVersionIsGreaterThan::kMaxKeyFieldName;
constexpr StringData ConfigsvrEnsureChunkVersionIsGreaterThan::kMinKeyFieldName;
constexpr StringData ConfigsvrEnsureChunkVersionIsGreaterThan::kVersionFieldName;
constexpr StringData ConfigsvrEnsureChunkVersionIsGreaterThan::kCommandName;

const std::vector<StringData> ConfigsvrEnsureChunkVersionIsGreaterThan::_knownBSONFields {
    ConfigsvrEnsureChunkVersionIsGreaterThan::kMaxKeyFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kMinKeyFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kVersionFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kCommandName,
};
const std::vector<StringData> ConfigsvrEnsureChunkVersionIsGreaterThan::_knownOP_MSGFields {
    ConfigsvrEnsureChunkVersionIsGreaterThan::kDbNameFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kMaxKeyFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kMinKeyFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kVersionFieldName,
    ConfigsvrEnsureChunkVersionIsGreaterThan::kCommandName,
};

ConfigsvrEnsureChunkVersionIsGreaterThan::ConfigsvrEnsureChunkVersionIsGreaterThan() : _hasMinKey(false), _hasMaxKey(false), _hasVersion(false), _hasDbName(false) {
    // Used for initialization only
}
ConfigsvrEnsureChunkVersionIsGreaterThan::ConfigsvrEnsureChunkVersionIsGreaterThan(mongo::BSONObj minKey, mongo::BSONObj maxKey, mongo::ChunkVersion version) : _minKey(std::move(minKey)), _maxKey(std::move(maxKey)), _version(std::move(version)), _hasMinKey(true), _hasMaxKey(true), _hasVersion(true), _hasDbName(true) {
    // Used for initialization only
}


ConfigsvrEnsureChunkVersionIsGreaterThan ConfigsvrEnsureChunkVersionIsGreaterThan::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ConfigsvrEnsureChunkVersionIsGreaterThan object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrEnsureChunkVersionIsGreaterThan::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kMinKeyFieldName) {
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
        else if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasVersion = true;
                const BSONObj localObject = element.Obj();
                _version = ChunkVersion::fromBSONThrowing(localObject);
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kMinKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMinKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMaxKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMaxKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kVersionFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kVersionFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

}

ConfigsvrEnsureChunkVersionIsGreaterThan ConfigsvrEnsureChunkVersionIsGreaterThan::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    ConfigsvrEnsureChunkVersionIsGreaterThan object;
    object.parseProtected(ctxt, request);
    return object;
}
void ConfigsvrEnsureChunkVersionIsGreaterThan::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kMinKeyFieldName) {
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
        else if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasVersion = true;
                const BSONObj localObject = element.Obj();
                _version = ChunkVersion::fromBSONThrowing(localObject);
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kMinKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMinKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMaxKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMaxKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kVersionFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kVersionFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

}

void ConfigsvrEnsureChunkVersionIsGreaterThan::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasMinKey && _hasMaxKey && _hasVersion && _hasDbName);

    builder->append("_configsvrEnsureChunkVersionIsGreaterThan"_sd, 1);
    builder->append(kMinKeyFieldName, _minKey);

    builder->append(kMaxKeyFieldName, _maxKey);

    {
        const BSONObj localObject = _version.toBSON();
        builder->append(kVersionFieldName, localObject);
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ConfigsvrEnsureChunkVersionIsGreaterThan::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasMinKey && _hasMaxKey && _hasVersion && _hasDbName);

        builder->append("_configsvrEnsureChunkVersionIsGreaterThan"_sd, 1);
        builder->append(kMinKeyFieldName, _minKey);

        builder->append(kMaxKeyFieldName, _maxKey);

        {
            const BSONObj localObject = _version.toBSON();
            builder->append(kVersionFieldName, localObject);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ConfigsvrEnsureChunkVersionIsGreaterThan::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
