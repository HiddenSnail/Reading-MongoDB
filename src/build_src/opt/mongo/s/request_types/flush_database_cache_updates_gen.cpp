/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/flush_database_cache_updates_gen.h --output build/opt/mongo/s/request_types/flush_database_cache_updates_gen.cpp src/mongo/s/request_types/flush_database_cache_updates.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/flush_database_cache_updates_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData _flushDatabaseCacheUpdates::kCommandParameterFieldName;
constexpr StringData _flushDatabaseCacheUpdates::kDbNameFieldName;
constexpr StringData _flushDatabaseCacheUpdates::kSyncFromConfigFieldName;
constexpr StringData _flushDatabaseCacheUpdates::kCommandName;

const std::vector<StringData> _flushDatabaseCacheUpdates::_knownBSONFields {
    _flushDatabaseCacheUpdates::kSyncFromConfigFieldName,
    _flushDatabaseCacheUpdates::kCommandName,
};
const std::vector<StringData> _flushDatabaseCacheUpdates::_knownOP_MSGFields {
    _flushDatabaseCacheUpdates::kDbNameFieldName,
    _flushDatabaseCacheUpdates::kSyncFromConfigFieldName,
    _flushDatabaseCacheUpdates::kCommandName,
};

_flushDatabaseCacheUpdates::_flushDatabaseCacheUpdates(const std::string commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}


_flushDatabaseCacheUpdates _flushDatabaseCacheUpdates::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::string localCmdType;
    _flushDatabaseCacheUpdates object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void _flushDatabaseCacheUpdates::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kSyncFromConfigBit = 0;
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

        if (fieldName == kSyncFromConfigFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kSyncFromConfigBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSyncFromConfigBit);

                _syncFromConfig = element.boolean();
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
        if (!usedFields[kSyncFromConfigBit]) {
            _syncFromConfig = true;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = commandElement.str();
        }
    }
}

_flushDatabaseCacheUpdates _flushDatabaseCacheUpdates::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::string localCmdType;
    _flushDatabaseCacheUpdates object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void _flushDatabaseCacheUpdates::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<2> usedFields;
    const size_t kSyncFromConfigBit = 0;
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

        if (fieldName == kSyncFromConfigFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kSyncFromConfigBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSyncFromConfigBit);

                _syncFromConfig = element.boolean();
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
        if (!usedFields[kSyncFromConfigBit]) {
            _syncFromConfig = true;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = commandElement.str();
        }
    }
}

void _flushDatabaseCacheUpdates::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append(kCommandParameterFieldName, _commandParameter);
    builder->append(kSyncFromConfigFieldName, _syncFromConfig);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest _flushDatabaseCacheUpdates::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append(kCommandParameterFieldName, _commandParameter);
        builder->append(kSyncFromConfigFieldName, _syncFromConfig);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj _flushDatabaseCacheUpdates::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
