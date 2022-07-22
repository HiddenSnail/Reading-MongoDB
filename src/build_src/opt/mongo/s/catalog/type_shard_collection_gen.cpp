/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/catalog/type_shard_collection_gen.h --output build/opt/mongo/s/catalog/type_shard_collection_gen.cpp src/mongo/s/catalog/type_shard_collection.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/catalog/type_shard_collection_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ShardCollectionTypeBase::kDefaultCollationFieldName;
constexpr StringData ShardCollectionTypeBase::kEnterCriticalSectionCounterFieldName;
constexpr StringData ShardCollectionTypeBase::kEpochFieldName;
constexpr StringData ShardCollectionTypeBase::kKeyPatternFieldName;
constexpr StringData ShardCollectionTypeBase::kLastRefreshedCollectionVersionFieldName;
constexpr StringData ShardCollectionTypeBase::kNssFieldName;
constexpr StringData ShardCollectionTypeBase::kRefreshingFieldName;
constexpr StringData ShardCollectionTypeBase::kUniqueFieldName;
constexpr StringData ShardCollectionTypeBase::kUuidFieldName;


ShardCollectionTypeBase::ShardCollectionTypeBase() : _unique(false), _hasNss(false), _hasEpoch(false), _hasKeyPattern(false), _hasUnique(false) {
    // Used for initialization only
}
ShardCollectionTypeBase::ShardCollectionTypeBase(mongo::NamespaceString nss, mongo::OID epoch, mongo::KeyPattern keyPattern, bool unique) : _nss(std::move(nss)), _epoch(std::move(epoch)), _keyPattern(std::move(keyPattern)), _unique(std::move(unique)), _hasNss(true), _hasEpoch(true), _hasKeyPattern(true), _hasUnique(true) {
    // Used for initialization only
}


ShardCollectionTypeBase ShardCollectionTypeBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ShardCollectionTypeBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ShardCollectionTypeBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<9> usedFields;
    const size_t kNssBit = 0;
    const size_t kUuidBit = 1;
    const size_t kEpochBit = 2;
    const size_t kKeyPatternBit = 3;
    const size_t kDefaultCollationBit = 4;
    const size_t kUniqueBit = 5;
    const size_t kRefreshingBit = 6;
    const size_t kLastRefreshedCollectionVersionBit = 7;
    const size_t kEnterCriticalSectionCounterBit = 8;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kNssFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNssBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNssBit);

                _hasNss = true;
                _nss = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kUuidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUuidBit);

                _uuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kEpochFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                if (MONGO_unlikely(usedFields[kEpochBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kEpochBit);

                _hasEpoch = true;
                _epoch = element.OID();
            }
        }
        else if (fieldName == kKeyPatternFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kKeyPatternBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyPatternBit);

                _hasKeyPattern = true;
                const BSONObj localObject = element.Obj();
                _keyPattern = KeyPattern::fromBSON(localObject);
            }
        }
        else if (fieldName == kDefaultCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kDefaultCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDefaultCollationBit);

                const BSONObj localObject = element.Obj();
                _defaultCollation = BSONObj::getOwned(localObject);
            }
        }
        else if (fieldName == kUniqueFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kUniqueBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUniqueBit);

                _hasUnique = true;
                _unique = element.boolean();
            }
        }
        else if (fieldName == kRefreshingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kRefreshingBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRefreshingBit);

                _refreshing = element.boolean();
            }
        }
        else if (fieldName == kLastRefreshedCollectionVersionFieldName) {
            if (MONGO_unlikely(usedFields[kLastRefreshedCollectionVersionBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kLastRefreshedCollectionVersionBit);

            _lastRefreshedCollectionVersion = ChunkVersion::legacyFromBSONThrowing(element);
        }
        else if (fieldName == kEnterCriticalSectionCounterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kEnterCriticalSectionCounterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kEnterCriticalSectionCounterBit);

                _enterCriticalSectionCounter = element._numberInt();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kNssBit]) {
            ctxt.throwMissingField(kNssFieldName);
        }
        if (!usedFields[kEpochBit]) {
            ctxt.throwMissingField(kEpochFieldName);
        }
        if (!usedFields[kKeyPatternBit]) {
            ctxt.throwMissingField(kKeyPatternFieldName);
        }
        if (!usedFields[kDefaultCollationBit]) {
            _defaultCollation = BSONObj();
        }
        if (!usedFields[kUniqueBit]) {
            ctxt.throwMissingField(kUniqueFieldName);
        }
    }

}


void ShardCollectionTypeBase::serialize(BSONObjBuilder* builder) const {
    invariant(_hasNss && _hasEpoch && _hasKeyPattern && _hasUnique);

    {
        builder->append(kNssFieldName, _nss.toString());
    }

    if (_uuid.is_initialized()) {
        ConstDataRange tempCDR = _uuid.get().toCDR();
        builder->append(kUuidFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    builder->append(kEpochFieldName, _epoch);

    {
        const BSONObj localObject = _keyPattern.toBSON();
        builder->append(kKeyPatternFieldName, localObject);
    }

    builder->append(kDefaultCollationFieldName, _defaultCollation);

    builder->append(kUniqueFieldName, _unique);

    if (_refreshing.is_initialized()) {
        builder->append(kRefreshingFieldName, _refreshing.get());
    }

    if (_lastRefreshedCollectionVersion.is_initialized()) {
        _lastRefreshedCollectionVersion.get().legacyToBSON(kLastRefreshedCollectionVersionFieldName, builder);
    }

    if (_enterCriticalSectionCounter.is_initialized()) {
        builder->append(kEnterCriticalSectionCounterFieldName, _enterCriticalSectionCounter.get());
    }

}


BSONObj ShardCollectionTypeBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
