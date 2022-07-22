/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/dbcheck_gen.h --output build/opt/mongo/db/repl/dbcheck_gen.cpp src/mongo/db/repl/dbcheck.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/dbcheck_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * The type of dbCheck oplog entry.
 */
namespace  {
constexpr StringData kOplogEntries_Batch = "batch"_sd;
constexpr StringData kOplogEntries_Collection = "collection"_sd;
}  // namespace 

OplogEntriesEnum OplogEntries_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kOplogEntries_Batch) {
        return OplogEntriesEnum::Batch;
    }
    if (value == kOplogEntries_Collection) {
        return OplogEntriesEnum::Collection;
    }
    ctxt.throwBadEnumValue(value);
}

StringData OplogEntries_serializer(OplogEntriesEnum value) {
    if (value == OplogEntriesEnum::Batch) {
        return kOplogEntries_Batch;
    }
    if (value == OplogEntriesEnum::Collection) {
        return kOplogEntries_Collection;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData DbCheckSingleInvocation::kCollFieldName;
constexpr StringData DbCheckSingleInvocation::kMaxCountFieldName;
constexpr StringData DbCheckSingleInvocation::kMaxCountPerSecondFieldName;
constexpr StringData DbCheckSingleInvocation::kMaxKeyFieldName;
constexpr StringData DbCheckSingleInvocation::kMaxSizeFieldName;
constexpr StringData DbCheckSingleInvocation::kMinKeyFieldName;


DbCheckSingleInvocation::DbCheckSingleInvocation() : _hasColl(false) {
    // Used for initialization only
}
DbCheckSingleInvocation::DbCheckSingleInvocation(std::string coll) : _coll(std::move(coll)), _hasColl(true) {
    // Used for initialization only
}


DbCheckSingleInvocation DbCheckSingleInvocation::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DbCheckSingleInvocation object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DbCheckSingleInvocation::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kCollBit = 0;
    const size_t kMinKeyBit = 1;
    const size_t kMaxKeyBit = 2;
    const size_t kMaxCountBit = 3;
    const size_t kMaxSizeBit = 4;
    const size_t kMaxCountPerSecondBit = 5;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kCollFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kCollBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollBit);

                _hasColl = true;
                _coll = element.str();
            }
        }
        else if (fieldName == kMinKeyFieldName) {
            if (MONGO_unlikely(usedFields[kMinKeyBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMinKeyBit);

            _minKey = BSONKey::parseFromBSON(element);
        }
        else if (fieldName == kMaxKeyFieldName) {
            if (MONGO_unlikely(usedFields[kMaxKeyBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMaxKeyBit);

            _maxKey = BSONKey::parseFromBSON(element);
        }
        else if (fieldName == kMaxCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxCountBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxCountBit);

                _maxCount = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxSizeBit);

                _maxSize = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxCountPerSecondFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxCountPerSecondBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxCountPerSecondBit);

                _maxCountPerSecond = element.safeNumberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kCollBit]) {
            ctxt.throwMissingField(kCollFieldName);
        }
        if (!usedFields[kMinKeyBit]) {
            _minKey = BSONKey::min();
        }
        if (!usedFields[kMaxKeyBit]) {
            _maxKey = BSONKey::max();
        }
        if (!usedFields[kMaxCountBit]) {
            _maxCount = std::numeric_limits<int64_t>::max();
        }
        if (!usedFields[kMaxSizeBit]) {
            _maxSize = std::numeric_limits<int64_t>::max();
        }
        if (!usedFields[kMaxCountPerSecondBit]) {
            _maxCountPerSecond = std::numeric_limits<int64_t>::max();
        }
    }

}


void DbCheckSingleInvocation::serialize(BSONObjBuilder* builder) const {
    invariant(_hasColl);

    builder->append(kCollFieldName, _coll);

    {
        _minKey.serializeToBSON(kMinKeyFieldName, builder);
    }

    {
        _maxKey.serializeToBSON(kMaxKeyFieldName, builder);
    }

    builder->append(kMaxCountFieldName, _maxCount);

    builder->append(kMaxSizeFieldName, _maxSize);

    builder->append(kMaxCountPerSecondFieldName, _maxCountPerSecond);

}


BSONObj DbCheckSingleInvocation::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData DbCheckAllInvocation::kMaxCountPerSecondFieldName;
constexpr StringData DbCheckAllInvocation::kTagFieldName;


DbCheckAllInvocation::DbCheckAllInvocation() : _tag(-1), _hasTag(false) {
    // Used for initialization only
}
DbCheckAllInvocation::DbCheckAllInvocation(std::int64_t tag) : _tag(std::move(tag)), _hasTag(true) {
    // Used for initialization only
}


DbCheckAllInvocation DbCheckAllInvocation::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DbCheckAllInvocation object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DbCheckAllInvocation::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kTagBit = 0;
    const size_t kMaxCountPerSecondBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kTagFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kTagBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTagBit);

                _hasTag = true;
                _tag = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxCountPerSecondFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxCountPerSecondBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxCountPerSecondBit);

                _maxCountPerSecond = element.safeNumberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kTagBit]) {
            ctxt.throwMissingField(kTagFieldName);
        }
        if (!usedFields[kMaxCountPerSecondBit]) {
            _maxCountPerSecond = std::numeric_limits<int64_t>::max();
        }
    }

}


void DbCheckAllInvocation::serialize(BSONObjBuilder* builder) const {
    invariant(_hasTag);

    builder->append(kTagFieldName, _tag);

    builder->append(kMaxCountPerSecondFieldName, _maxCountPerSecond);

}


BSONObj DbCheckAllInvocation::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData DbCheckOplogBatch::kMaxKeyFieldName;
constexpr StringData DbCheckOplogBatch::kMaxRateFieldName;
constexpr StringData DbCheckOplogBatch::kMd5FieldName;
constexpr StringData DbCheckOplogBatch::kMinKeyFieldName;
constexpr StringData DbCheckOplogBatch::kNssFieldName;
constexpr StringData DbCheckOplogBatch::kTypeFieldName;


DbCheckOplogBatch::DbCheckOplogBatch() : _hasNss(false), _hasType(false), _hasMd5(false), _hasMinKey(false), _hasMaxKey(false) {
    // Used for initialization only
}
DbCheckOplogBatch::DbCheckOplogBatch(mongo::NamespaceString nss, mongo::OplogEntriesEnum type, std::string md5, mongo::BSONKey minKey, mongo::BSONKey maxKey) : _nss(std::move(nss)), _type(std::move(type)), _md5(std::move(md5)), _minKey(std::move(minKey)), _maxKey(std::move(maxKey)), _hasNss(true), _hasType(true), _hasMd5(true), _hasMinKey(true), _hasMaxKey(true) {
    // Used for initialization only
}


DbCheckOplogBatch DbCheckOplogBatch::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DbCheckOplogBatch object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DbCheckOplogBatch::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kNssBit = 0;
    const size_t kTypeBit = 1;
    const size_t kMd5Bit = 2;
    const size_t kMinKeyBit = 3;
    const size_t kMaxKeyBit = 4;
    const size_t kMaxRateBit = 5;

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
        else if (fieldName == kTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kTypeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTypeBit);

                _hasType = true;
                IDLParserErrorContext tempContext(kTypeFieldName, &ctxt);
                _type = OplogEntries_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kMd5FieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kMd5Bit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMd5Bit);

                _hasMd5 = true;
                _md5 = element.str();
            }
        }
        else if (fieldName == kMinKeyFieldName) {
            if (MONGO_unlikely(usedFields[kMinKeyBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMinKeyBit);

            _hasMinKey = true;
            _minKey = BSONKey::parseFromBSON(element);
        }
        else if (fieldName == kMaxKeyFieldName) {
            if (MONGO_unlikely(usedFields[kMaxKeyBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMaxKeyBit);

            _hasMaxKey = true;
            _maxKey = BSONKey::parseFromBSON(element);
        }
        else if (fieldName == kMaxRateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxRateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxRateBit);

                _maxRate = element.safeNumberLong();
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
        if (!usedFields[kTypeBit]) {
            ctxt.throwMissingField(kTypeFieldName);
        }
        if (!usedFields[kMd5Bit]) {
            ctxt.throwMissingField(kMd5FieldName);
        }
        if (!usedFields[kMinKeyBit]) {
            ctxt.throwMissingField(kMinKeyFieldName);
        }
        if (!usedFields[kMaxKeyBit]) {
            ctxt.throwMissingField(kMaxKeyFieldName);
        }
    }

}


void DbCheckOplogBatch::serialize(BSONObjBuilder* builder) const {
    invariant(_hasNss && _hasType && _hasMd5 && _hasMinKey && _hasMaxKey);

    {
        builder->append(kNssFieldName, _nss.toString());
    }

    {
        builder->append(kTypeFieldName, OplogEntries_serializer(_type));
    }

    builder->append(kMd5FieldName, _md5);

    {
        _minKey.serializeToBSON(kMinKeyFieldName, builder);
    }

    {
        _maxKey.serializeToBSON(kMaxKeyFieldName, builder);
    }

    if (_maxRate.is_initialized()) {
        builder->append(kMaxRateFieldName, _maxRate.get());
    }

}


BSONObj DbCheckOplogBatch::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData DbCheckOplogCollection::kIndexesFieldName;
constexpr StringData DbCheckOplogCollection::kNextFieldName;
constexpr StringData DbCheckOplogCollection::kNssFieldName;
constexpr StringData DbCheckOplogCollection::kOptionsFieldName;
constexpr StringData DbCheckOplogCollection::kPrevFieldName;
constexpr StringData DbCheckOplogCollection::kTypeFieldName;
constexpr StringData DbCheckOplogCollection::kUuidFieldName;


DbCheckOplogCollection::DbCheckOplogCollection() : _hasNss(false), _hasType(false), _hasUuid(false), _hasIndexes(false), _hasOptions(false) {
    // Used for initialization only
}
DbCheckOplogCollection::DbCheckOplogCollection(mongo::NamespaceString nss, mongo::OplogEntriesEnum type, mongo::UUID uuid, std::vector<mongo::BSONObj> indexes, mongo::BSONObj options) : _nss(std::move(nss)), _type(std::move(type)), _uuid(std::move(uuid)), _indexes(std::move(indexes)), _options(std::move(options)), _hasNss(true), _hasType(true), _hasUuid(true), _hasIndexes(true), _hasOptions(true) {
    // Used for initialization only
}


DbCheckOplogCollection DbCheckOplogCollection::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DbCheckOplogCollection object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DbCheckOplogCollection::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<7> usedFields;
    const size_t kNssBit = 0;
    const size_t kTypeBit = 1;
    const size_t kUuidBit = 2;
    const size_t kPrevBit = 3;
    const size_t kNextBit = 4;
    const size_t kIndexesBit = 5;
    const size_t kOptionsBit = 6;

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
        else if (fieldName == kTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kTypeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTypeBit);

                _hasType = true;
                IDLParserErrorContext tempContext(kTypeFieldName, &ctxt);
                _type = OplogEntries_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kUuidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUuidBit);

                _hasUuid = true;
                _uuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kPrevFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kPrevBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPrevBit);

                _prev = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kNextFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kNextBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNextBit);

                _next = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kIndexesFieldName) {
            if (MONGO_unlikely(usedFields[kIndexesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kIndexesBit);

            _hasIndexes = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kIndexesFieldName, &ctxt);
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
            _indexes = std::move(values);
        }
        else if (fieldName == kOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kOptionsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOptionsBit);

                _hasOptions = true;
                _options = element.Obj();
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
        if (!usedFields[kTypeBit]) {
            ctxt.throwMissingField(kTypeFieldName);
        }
        if (!usedFields[kUuidBit]) {
            ctxt.throwMissingField(kUuidFieldName);
        }
        if (!usedFields[kIndexesBit]) {
            ctxt.throwMissingField(kIndexesFieldName);
        }
        if (!usedFields[kOptionsBit]) {
            ctxt.throwMissingField(kOptionsFieldName);
        }
    }

}


void DbCheckOplogCollection::serialize(BSONObjBuilder* builder) const {
    invariant(_hasNss && _hasType && _hasUuid && _hasIndexes && _hasOptions);

    {
        builder->append(kNssFieldName, _nss.toString());
    }

    {
        builder->append(kTypeFieldName, OplogEntries_serializer(_type));
    }

    {
        ConstDataRange tempCDR = _uuid.toCDR();
        builder->append(kUuidFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    if (_prev.is_initialized()) {
        ConstDataRange tempCDR = _prev.get().toCDR();
        builder->append(kPrevFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    if (_next.is_initialized()) {
        ConstDataRange tempCDR = _next.get().toCDR();
        builder->append(kNextFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        builder->append(kIndexesFieldName, _indexes);
    }

    builder->append(kOptionsFieldName, _options);

}


BSONObj DbCheckOplogCollection::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
