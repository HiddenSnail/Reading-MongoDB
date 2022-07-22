/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/map_reduce_gen.h --output build/opt/mongo/db/commands/map_reduce_gen.cpp src/mongo/db/commands/map_reduce.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/map_reduce_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData MapReduce::kBypassDocumentValidationFieldName;
constexpr StringData MapReduce::kCollationFieldName;
constexpr StringData MapReduce::kDbNameFieldName;
constexpr StringData MapReduce::kFinalizeFieldName;
constexpr StringData MapReduce::kJsModeFieldName;
constexpr StringData MapReduce::kLimitFieldName;
constexpr StringData MapReduce::kMapFieldName;
constexpr StringData MapReduce::kOutOptionsFieldName;
constexpr StringData MapReduce::kQueryFieldName;
constexpr StringData MapReduce::kReduceFieldName;
constexpr StringData MapReduce::kScopeFieldName;
constexpr StringData MapReduce::kSortFieldName;
constexpr StringData MapReduce::kVerboseFieldName;
constexpr StringData MapReduce::kCommandName;

const std::vector<StringData> MapReduce::_knownBSONFields {
    MapReduce::kBypassDocumentValidationFieldName,
    MapReduce::kCollationFieldName,
    MapReduce::kFinalizeFieldName,
    MapReduce::kJsModeFieldName,
    MapReduce::kLimitFieldName,
    MapReduce::kMapFieldName,
    MapReduce::kOutOptionsFieldName,
    MapReduce::kQueryFieldName,
    MapReduce::kReduceFieldName,
    MapReduce::kScopeFieldName,
    MapReduce::kSortFieldName,
    MapReduce::kVerboseFieldName,
    MapReduce::kCommandName,
};
const std::vector<StringData> MapReduce::_knownOP_MSGFields {
    MapReduce::kBypassDocumentValidationFieldName,
    MapReduce::kCollationFieldName,
    MapReduce::kDbNameFieldName,
    MapReduce::kFinalizeFieldName,
    MapReduce::kJsModeFieldName,
    MapReduce::kLimitFieldName,
    MapReduce::kMapFieldName,
    MapReduce::kOutOptionsFieldName,
    MapReduce::kQueryFieldName,
    MapReduce::kReduceFieldName,
    MapReduce::kScopeFieldName,
    MapReduce::kSortFieldName,
    MapReduce::kVerboseFieldName,
    MapReduce::kCommandName,
};

MapReduce::MapReduce(const NamespaceString nss) : _nss(std::move(nss)), _dbName(nss.db().toString()), _hasMap(false), _hasReduce(false), _hasOutOptions(false), _hasDbName(true) {
    // Used for initialization only
}
MapReduce::MapReduce(const NamespaceString nss, mongo::MapReduceJavascriptCode map, mongo::MapReduceJavascriptCode reduce, mongo::MapReduceOutOptions outOptions) : _nss(std::move(nss)), _map(std::move(map)), _reduce(std::move(reduce)), _outOptions(std::move(outOptions)), _dbName(nss.db().toString()), _hasMap(true), _hasReduce(true), _hasOutOptions(true), _hasDbName(true) {
    // Used for initialization only
}


MapReduce MapReduce::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    MapReduce object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void MapReduce::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<13> usedFields;
    const size_t kMapBit = 0;
    const size_t kReduceBit = 1;
    const size_t kOutOptionsBit = 2;
    const size_t kQueryBit = 3;
    const size_t kSortBit = 4;
    const size_t kCollationBit = 5;
    const size_t kLimitBit = 6;
    const size_t kFinalizeBit = 7;
    const size_t kScopeBit = 8;
    const size_t kVerboseBit = 9;
    const size_t kBypassDocumentValidationBit = 10;
    const size_t kJsModeBit = 11;
    const size_t kDbNameBit = 12;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kMapFieldName) {
            if (MONGO_unlikely(usedFields[kMapBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMapBit);

            _hasMap = true;
            _map = MapReduceJavascriptCode::parseFromBSON(element);
        }
        else if (fieldName == kReduceFieldName) {
            if (MONGO_unlikely(usedFields[kReduceBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kReduceBit);

            _hasReduce = true;
            _reduce = MapReduceJavascriptCode::parseFromBSON(element);
        }
        else if (fieldName == kOutOptionsFieldName) {
            if (MONGO_unlikely(usedFields[kOutOptionsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kOutOptionsBit);

            _hasOutOptions = true;
            _outOptions = MapReduceOutOptions::parseFromBSON(element);
        }
        else if (fieldName == kQueryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryBit);

                _query = element.Obj();
            }
        }
        else if (fieldName == kSortFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSortBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSortBit);

                _sort = element.Obj();
            }
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollationBit);

                _collation = element.Obj();
            }
        }
        else if (fieldName == kLimitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kLimitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLimitBit);

                _limit = element.safeNumberLong();
            }
        }
        else if (fieldName == kFinalizeFieldName) {
            if (MONGO_unlikely(usedFields[kFinalizeBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kFinalizeBit);

            _finalize = MapReduceJavascriptCodeOrNull::parseFromBSON(element);
        }
        else if (fieldName == kScopeFieldName) {
            if (MONGO_unlikely(usedFields[kScopeBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kScopeBit);

            _scope = MapReduceGlobalVariableScope::parseFromBSON(element);
        }
        else if (fieldName == kVerboseFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kVerboseBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVerboseBit);

                _verbose = element.boolean();
            }
        }
        else if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _bypassDocumentValidation = element.boolean();
            }
        }
        else if (fieldName == kJsModeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kJsModeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kJsModeBit);

                _jsMode = element.boolean();
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
        if (!usedFields[kMapBit]) {
            ctxt.throwMissingField(kMapFieldName);
        }
        if (!usedFields[kReduceBit]) {
            ctxt.throwMissingField(kReduceFieldName);
        }
        if (!usedFields[kOutOptionsBit]) {
            ctxt.throwMissingField(kOutOptionsFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

MapReduce MapReduce::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    MapReduce object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void MapReduce::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<13> usedFields;
    const size_t kMapBit = 0;
    const size_t kReduceBit = 1;
    const size_t kOutOptionsBit = 2;
    const size_t kQueryBit = 3;
    const size_t kSortBit = 4;
    const size_t kCollationBit = 5;
    const size_t kLimitBit = 6;
    const size_t kFinalizeBit = 7;
    const size_t kScopeBit = 8;
    const size_t kVerboseBit = 9;
    const size_t kBypassDocumentValidationBit = 10;
    const size_t kJsModeBit = 11;
    const size_t kDbNameBit = 12;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kMapFieldName) {
            if (MONGO_unlikely(usedFields[kMapBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMapBit);

            _hasMap = true;
            _map = MapReduceJavascriptCode::parseFromBSON(element);
        }
        else if (fieldName == kReduceFieldName) {
            if (MONGO_unlikely(usedFields[kReduceBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kReduceBit);

            _hasReduce = true;
            _reduce = MapReduceJavascriptCode::parseFromBSON(element);
        }
        else if (fieldName == kOutOptionsFieldName) {
            if (MONGO_unlikely(usedFields[kOutOptionsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kOutOptionsBit);

            _hasOutOptions = true;
            _outOptions = MapReduceOutOptions::parseFromBSON(element);
        }
        else if (fieldName == kQueryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryBit);

                _query = element.Obj();
            }
        }
        else if (fieldName == kSortFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSortBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSortBit);

                _sort = element.Obj();
            }
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollationBit);

                _collation = element.Obj();
            }
        }
        else if (fieldName == kLimitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kLimitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLimitBit);

                _limit = element.safeNumberLong();
            }
        }
        else if (fieldName == kFinalizeFieldName) {
            if (MONGO_unlikely(usedFields[kFinalizeBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kFinalizeBit);

            _finalize = MapReduceJavascriptCodeOrNull::parseFromBSON(element);
        }
        else if (fieldName == kScopeFieldName) {
            if (MONGO_unlikely(usedFields[kScopeBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kScopeBit);

            _scope = MapReduceGlobalVariableScope::parseFromBSON(element);
        }
        else if (fieldName == kVerboseFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kVerboseBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVerboseBit);

                _verbose = element.boolean();
            }
        }
        else if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _bypassDocumentValidation = element.boolean();
            }
        }
        else if (fieldName == kJsModeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kJsModeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kJsModeBit);

                _jsMode = element.boolean();
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
        if (!usedFields[kMapBit]) {
            ctxt.throwMissingField(kMapFieldName);
        }
        if (!usedFields[kReduceBit]) {
            ctxt.throwMissingField(kReduceFieldName);
        }
        if (!usedFields[kOutOptionsBit]) {
            ctxt.throwMissingField(kOutOptionsFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

void MapReduce::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasMap && _hasReduce && _hasOutOptions && _hasDbName);

    invariant(!_nss.isEmpty());
    builder->append("MapReduce"_sd, _nss.coll());

    {
        _map.serializeToBSON(kMapFieldName, builder);
    }

    {
        _reduce.serializeToBSON(kReduceFieldName, builder);
    }

    {
        _outOptions.serializeToBSON(kOutOptionsFieldName, builder);
    }

    if (_query.is_initialized()) {
        builder->append(kQueryFieldName, _query.get());
    }

    if (_sort.is_initialized()) {
        builder->append(kSortFieldName, _sort.get());
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

    if (_limit.is_initialized()) {
        builder->append(kLimitFieldName, _limit.get());
    }

    if (_finalize.is_initialized()) {
        _finalize.get().serializeToBSON(kFinalizeFieldName, builder);
    }

    if (_scope.is_initialized()) {
        _scope.get().serializeToBSON(kScopeFieldName, builder);
    }

    if (_verbose.is_initialized()) {
        builder->append(kVerboseFieldName, _verbose.get());
    }

    if (_bypassDocumentValidation.is_initialized()) {
        builder->append(kBypassDocumentValidationFieldName, _bypassDocumentValidation.get());
    }

    if (_jsMode.is_initialized()) {
        builder->append(kJsModeFieldName, _jsMode.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest MapReduce::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasMap && _hasReduce && _hasOutOptions && _hasDbName);

        invariant(!_nss.isEmpty());
        builder->append("MapReduce"_sd, _nss.coll());

        {
            _map.serializeToBSON(kMapFieldName, builder);
        }

        {
            _reduce.serializeToBSON(kReduceFieldName, builder);
        }

        {
            _outOptions.serializeToBSON(kOutOptionsFieldName, builder);
        }

        if (_query.is_initialized()) {
            builder->append(kQueryFieldName, _query.get());
        }

        if (_sort.is_initialized()) {
            builder->append(kSortFieldName, _sort.get());
        }

        if (_collation.is_initialized()) {
            builder->append(kCollationFieldName, _collation.get());
        }

        if (_limit.is_initialized()) {
            builder->append(kLimitFieldName, _limit.get());
        }

        if (_finalize.is_initialized()) {
            _finalize.get().serializeToBSON(kFinalizeFieldName, builder);
        }

        if (_scope.is_initialized()) {
            _scope.get().serializeToBSON(kScopeFieldName, builder);
        }

        if (_verbose.is_initialized()) {
            builder->append(kVerboseFieldName, _verbose.get());
        }

        if (_bypassDocumentValidation.is_initialized()) {
            builder->append(kBypassDocumentValidationFieldName, _bypassDocumentValidation.get());
        }

        if (_jsMode.is_initialized()) {
            builder->append(kJsModeFieldName, _jsMode.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj MapReduce::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
