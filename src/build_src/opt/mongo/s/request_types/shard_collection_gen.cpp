/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/shard_collection_gen.h --output build/opt/mongo/s/request_types/shard_collection_gen.cpp src/mongo/s/request_types/shard_collection.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/shard_collection_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ShardCollection::kCollationFieldName;
constexpr StringData ShardCollection::kKeyFieldName;
constexpr StringData ShardCollection::kNumInitialChunksFieldName;
constexpr StringData ShardCollection::kPresplitHashedZonesFieldName;
constexpr StringData ShardCollection::kShardCollectionFieldName;
constexpr StringData ShardCollection::kShardcollectionFieldName;
constexpr StringData ShardCollection::kUniqueFieldName;


ShardCollection::ShardCollection() : _hasKey(false) {
    // Used for initialization only
}
ShardCollection::ShardCollection(mongo::BSONObj key) : _key(std::move(key)), _hasKey(true) {
    // Used for initialization only
}


ShardCollection ShardCollection::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ShardCollection object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ShardCollection::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kShardCollectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _shardCollection = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kShardcollectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _shardcollection = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kUniqueFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _unique = element.boolean();
            }
        }
        else if (fieldName == kNumInitialChunksFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _numInitialChunks = element.safeNumberLong();
            }
        }
        else if (fieldName == kPresplitHashedZonesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _presplitHashedZones = element.boolean();
            }
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _collation = element.Obj();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kUniqueFieldName) == usedFields.end())) {
        _unique = false;
    }
    if (MONGO_unlikely(usedFields.find(kNumInitialChunksFieldName) == usedFields.end())) {
        _numInitialChunks = 0;
    }
    if (MONGO_unlikely(usedFields.find(kPresplitHashedZonesFieldName) == usedFields.end())) {
        _presplitHashedZones = false;
    }

}


void ShardCollection::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKey);

    if (_shardCollection.is_initialized()) {
        builder->append(kShardCollectionFieldName, _shardCollection.get().toString());
    }

    if (_shardcollection.is_initialized()) {
        builder->append(kShardcollectionFieldName, _shardcollection.get().toString());
    }

    builder->append(kKeyFieldName, _key);

    builder->append(kUniqueFieldName, _unique);

    builder->append(kNumInitialChunksFieldName, _numInitialChunks);

    builder->append(kPresplitHashedZonesFieldName, _presplitHashedZones);

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

}


BSONObj ShardCollection::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ConfigsvrShardCollectionRequest::k_configsvrShardCollectionFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kCollationFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kGetUUIDfromPrimaryShardFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kInitialSplitPointsFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kKeyFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kNumInitialChunksFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kPresplitHashedZonesFieldName;
constexpr StringData ConfigsvrShardCollectionRequest::kUniqueFieldName;


ConfigsvrShardCollectionRequest::ConfigsvrShardCollectionRequest() : _hasKey(false) {
    // Used for initialization only
}
ConfigsvrShardCollectionRequest::ConfigsvrShardCollectionRequest(mongo::BSONObj key) : _key(std::move(key)), _hasKey(true) {
    // Used for initialization only
}


ConfigsvrShardCollectionRequest ConfigsvrShardCollectionRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ConfigsvrShardCollectionRequest object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrShardCollectionRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_configsvrShardCollectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                __configsvrShardCollection = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kUniqueFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _unique = element.boolean();
            }
        }
        else if (fieldName == kNumInitialChunksFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _numInitialChunks = element.safeNumberLong();
            }
        }
        else if (fieldName == kPresplitHashedZonesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _presplitHashedZones = element.boolean();
            }
        }
        else if (fieldName == kInitialSplitPointsFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kInitialSplitPointsFieldName, &ctxt);
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
            _initialSplitPoints = std::move(values);
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _collation = element.Obj();
            }
        }
        else if (fieldName == kGetUUIDfromPrimaryShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _getUUIDfromPrimaryShard = element.boolean();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kUniqueFieldName) == usedFields.end())) {
        _unique = false;
    }
    if (MONGO_unlikely(usedFields.find(kNumInitialChunksFieldName) == usedFields.end())) {
        _numInitialChunks = 0;
    }
    if (MONGO_unlikely(usedFields.find(kPresplitHashedZonesFieldName) == usedFields.end())) {
        _presplitHashedZones = false;
    }
    if (MONGO_unlikely(usedFields.find(kGetUUIDfromPrimaryShardFieldName) == usedFields.end())) {
        _getUUIDfromPrimaryShard = true;
    }

}


void ConfigsvrShardCollectionRequest::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKey);

    if (__configsvrShardCollection.is_initialized()) {
        builder->append(k_configsvrShardCollectionFieldName, __configsvrShardCollection.get().toString());
    }

    builder->append(kKeyFieldName, _key);

    builder->append(kUniqueFieldName, _unique);

    builder->append(kNumInitialChunksFieldName, _numInitialChunks);

    builder->append(kPresplitHashedZonesFieldName, _presplitHashedZones);

    if (_initialSplitPoints.is_initialized()) {
        builder->append(kInitialSplitPointsFieldName, _initialSplitPoints.get());
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

    builder->append(kGetUUIDfromPrimaryShardFieldName, _getUUIDfromPrimaryShard);

}


BSONObj ConfigsvrShardCollectionRequest::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ConfigsvrShardCollectionResponse::kCollectionUUIDFieldName;
constexpr StringData ConfigsvrShardCollectionResponse::kCollectionshardedFieldName;


ConfigsvrShardCollectionResponse::ConfigsvrShardCollectionResponse() : _hasCollectionsharded(false) {
    // Used for initialization only
}
ConfigsvrShardCollectionResponse::ConfigsvrShardCollectionResponse(std::string collectionsharded) : _collectionsharded(std::move(collectionsharded)), _hasCollectionsharded(true) {
    // Used for initialization only
}


ConfigsvrShardCollectionResponse ConfigsvrShardCollectionResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ConfigsvrShardCollectionResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrShardCollectionResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kCollectionshardedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasCollectionsharded = true;
                _collectionsharded = element.str();
            }
        }
        else if (fieldName == kCollectionUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _collectionUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kCollectionshardedFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCollectionshardedFieldName);
    }

}


void ConfigsvrShardCollectionResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasCollectionsharded);

    builder->append(kCollectionshardedFieldName, _collectionsharded);

    if (_collectionUUID.is_initialized()) {
        ConstDataRange tempCDR = _collectionUUID.get().toCDR();
        builder->append(kCollectionUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

}


BSONObj ConfigsvrShardCollectionResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ShardsvrShardCollection::k_shardsvrShardCollectionFieldName;
constexpr StringData ShardsvrShardCollection::kCollationFieldName;
constexpr StringData ShardsvrShardCollection::kGetUUIDfromPrimaryShardFieldName;
constexpr StringData ShardsvrShardCollection::kInitialSplitPointsFieldName;
constexpr StringData ShardsvrShardCollection::kKeyFieldName;
constexpr StringData ShardsvrShardCollection::kNumInitialChunksFieldName;
constexpr StringData ShardsvrShardCollection::kPresplitHashedZonesFieldName;
constexpr StringData ShardsvrShardCollection::kUniqueFieldName;


ShardsvrShardCollection::ShardsvrShardCollection() : _hasKey(false) {
    // Used for initialization only
}
ShardsvrShardCollection::ShardsvrShardCollection(mongo::BSONObj key) : _key(std::move(key)), _hasKey(true) {
    // Used for initialization only
}


ShardsvrShardCollection ShardsvrShardCollection::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ShardsvrShardCollection object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ShardsvrShardCollection::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_shardsvrShardCollectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                __shardsvrShardCollection = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasKey = true;
                _key = element.Obj();
            }
        }
        else if (fieldName == kUniqueFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _unique = element.boolean();
            }
        }
        else if (fieldName == kNumInitialChunksFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _numInitialChunks = element.safeNumberLong();
            }
        }
        else if (fieldName == kPresplitHashedZonesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _presplitHashedZones = element.boolean();
            }
        }
        else if (fieldName == kInitialSplitPointsFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kInitialSplitPointsFieldName, &ctxt);
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
            _initialSplitPoints = std::move(values);
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _collation = element.Obj();
            }
        }
        else if (fieldName == kGetUUIDfromPrimaryShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _getUUIDfromPrimaryShard = element.boolean();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kUniqueFieldName) == usedFields.end())) {
        _unique = false;
    }
    if (MONGO_unlikely(usedFields.find(kNumInitialChunksFieldName) == usedFields.end())) {
        _numInitialChunks = 0;
    }
    if (MONGO_unlikely(usedFields.find(kPresplitHashedZonesFieldName) == usedFields.end())) {
        _presplitHashedZones = false;
    }
    if (MONGO_unlikely(usedFields.find(kGetUUIDfromPrimaryShardFieldName) == usedFields.end())) {
        _getUUIDfromPrimaryShard = true;
    }

}


void ShardsvrShardCollection::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKey);

    if (__shardsvrShardCollection.is_initialized()) {
        builder->append(k_shardsvrShardCollectionFieldName, __shardsvrShardCollection.get().toString());
    }

    builder->append(kKeyFieldName, _key);

    builder->append(kUniqueFieldName, _unique);

    builder->append(kNumInitialChunksFieldName, _numInitialChunks);

    builder->append(kPresplitHashedZonesFieldName, _presplitHashedZones);

    if (_initialSplitPoints.is_initialized()) {
        builder->append(kInitialSplitPointsFieldName, _initialSplitPoints.get());
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

    builder->append(kGetUUIDfromPrimaryShardFieldName, _getUUIDfromPrimaryShard);

}


BSONObj ShardsvrShardCollection::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ShardsvrShardCollectionResponse::kCollectionUUIDFieldName;
constexpr StringData ShardsvrShardCollectionResponse::kCollectionshardedFieldName;


ShardsvrShardCollectionResponse::ShardsvrShardCollectionResponse() : _hasCollectionsharded(false) {
    // Used for initialization only
}
ShardsvrShardCollectionResponse::ShardsvrShardCollectionResponse(std::string collectionsharded) : _collectionsharded(std::move(collectionsharded)), _hasCollectionsharded(true) {
    // Used for initialization only
}


ShardsvrShardCollectionResponse ShardsvrShardCollectionResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ShardsvrShardCollectionResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ShardsvrShardCollectionResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kCollectionshardedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasCollectionsharded = true;
                _collectionsharded = element.str();
            }
        }
        else if (fieldName == kCollectionUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _collectionUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kCollectionshardedFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCollectionshardedFieldName);
    }

}


void ShardsvrShardCollectionResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasCollectionsharded);

    builder->append(kCollectionshardedFieldName, _collectionsharded);

    if (_collectionUUID.is_initialized()) {
        ConstDataRange tempCDR = _collectionUUID.get().toCDR();
        builder->append(kCollectionUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

}


BSONObj ShardsvrShardCollectionResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ConfigsvrCommitShardCollection::k_configsvrCommitShardCollectionFieldName;
constexpr StringData ConfigsvrCommitShardCollection::kCollectionUUIDFieldName;


ConfigsvrCommitShardCollection::ConfigsvrCommitShardCollection() : _has_configsvrCommitShardCollection(false) {
    // Used for initialization only
}
ConfigsvrCommitShardCollection::ConfigsvrCommitShardCollection(std::string _configsvrCommitShardCollection) : __configsvrCommitShardCollection(std::move(_configsvrCommitShardCollection)), _has_configsvrCommitShardCollection(true) {
    // Used for initialization only
}


ConfigsvrCommitShardCollection ConfigsvrCommitShardCollection::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ConfigsvrCommitShardCollection object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrCommitShardCollection::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_configsvrCommitShardCollectionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _has_configsvrCommitShardCollection = true;
                __configsvrCommitShardCollection = element.str();
            }
        }
        else if (fieldName == kCollectionUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _collectionUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(k_configsvrCommitShardCollectionFieldName) == usedFields.end())) {
        ctxt.throwMissingField(k_configsvrCommitShardCollectionFieldName);
    }

}


void ConfigsvrCommitShardCollection::serialize(BSONObjBuilder* builder) const {
    invariant(_has_configsvrCommitShardCollection);

    builder->append(k_configsvrCommitShardCollectionFieldName, __configsvrCommitShardCollection);

    if (_collectionUUID.is_initialized()) {
        ConstDataRange tempCDR = _collectionUUID.get().toCDR();
        builder->append(kCollectionUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

}


BSONObj ConfigsvrCommitShardCollection::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
