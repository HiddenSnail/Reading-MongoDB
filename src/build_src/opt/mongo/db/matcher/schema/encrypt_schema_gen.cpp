/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/matcher/schema/encrypt_schema_gen.h --output build/opt/mongo/db/matcher/schema/encrypt_schema_gen.cpp src/mongo/db/matcher/schema/encrypt_schema.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/matcher/schema/encrypt_schema_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * The algorithm used to encrypt fields for field level encryption.
 */
namespace  {
constexpr StringData kFleAlgorithm_kDeterministic = "AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic"_sd;
constexpr StringData kFleAlgorithm_kRandom = "AEAD_AES_256_CBC_HMAC_SHA_512-Random"_sd;
}  // namespace 

FleAlgorithmEnum FleAlgorithm_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kFleAlgorithm_kDeterministic) {
        return FleAlgorithmEnum::kDeterministic;
    }
    if (value == kFleAlgorithm_kRandom) {
        return FleAlgorithmEnum::kRandom;
    }
    ctxt.throwBadEnumValue(value);
}

StringData FleAlgorithm_serializer(FleAlgorithmEnum value) {
    if (value == FleAlgorithmEnum::kDeterministic) {
        return kFleAlgorithm_kDeterministic;
    }
    if (value == FleAlgorithmEnum::kRandom) {
        return kFleAlgorithm_kRandom;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

/**
 * The algorithm used to encrypt fields for field level encryption represented as an unsigned integer.
 */
FleAlgorithmInt FleAlgorithmInt_parse(const IDLParserErrorContext& ctxt, std::int32_t value) {
    
    if (!(value >= static_cast<std::underlying_type<FleAlgorithmInt>::type>(
        FleAlgorithmInt::kDeterministic) &&
        value <= static_cast<std::underlying_type<FleAlgorithmInt>::type>(
            FleAlgorithmInt::kRandom))) {
        ctxt.throwBadEnumValue(value);
    } else {
        return static_cast<FleAlgorithmInt>(value);
    }
}

std::int32_t FleAlgorithmInt_serializer(FleAlgorithmInt value) {
    return static_cast<std::int32_t>(value);
}

constexpr StringData EncryptionInfo::kAlgorithmFieldName;
constexpr StringData EncryptionInfo::kBsonTypeFieldName;
constexpr StringData EncryptionInfo::kKeyIdFieldName;


EncryptionInfo::EncryptionInfo()  {
    // Used for initialization only
}


EncryptionInfo EncryptionInfo::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    EncryptionInfo object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void EncryptionInfo::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kBsonTypeBit = 0;
    const size_t kAlgorithmBit = 1;
    const size_t kKeyIdBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kBsonTypeFieldName) {
            if (MONGO_unlikely(usedFields[kBsonTypeBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kBsonTypeBit);

            _bsonType = BSONTypeSet::parseFromBSON(element);
        }
        else if (fieldName == kAlgorithmFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kAlgorithmBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAlgorithmBit);

                IDLParserErrorContext tempContext(kAlgorithmFieldName, &ctxt);
                _algorithm = FleAlgorithm_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kKeyIdFieldName) {
            if (MONGO_unlikely(usedFields[kKeyIdBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kKeyIdBit);

            _keyId = EncryptSchemaKeyId::parseFromBSON(element);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void EncryptionInfo::serialize(BSONObjBuilder* builder) const {
    if (_bsonType.is_initialized()) {
        _bsonType.get().serializeToBSON(kBsonTypeFieldName, builder);
    }

    if (_algorithm.is_initialized()) {
        builder->append(kAlgorithmFieldName, FleAlgorithm_serializer(_algorithm.get()));
    }

    if (_keyId.is_initialized()) {
        _keyId.get().serializeToBSON(kKeyIdFieldName, builder);
    }

}


BSONObj EncryptionInfo::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData EncryptionMetadata::kAlgorithmFieldName;
constexpr StringData EncryptionMetadata::kKeyIdFieldName;


EncryptionMetadata::EncryptionMetadata()  {
    // Used for initialization only
}


EncryptionMetadata EncryptionMetadata::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    EncryptionMetadata object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void EncryptionMetadata::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kAlgorithmBit = 0;
    const size_t kKeyIdBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAlgorithmFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kAlgorithmBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAlgorithmBit);

                IDLParserErrorContext tempContext(kAlgorithmFieldName, &ctxt);
                _algorithm = FleAlgorithm_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kKeyIdFieldName) {
            if (MONGO_unlikely(usedFields[kKeyIdBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kKeyIdBit);

            _keyId = EncryptSchemaKeyId::parseFromBSON(element);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void EncryptionMetadata::serialize(BSONObjBuilder* builder) const {
    if (_algorithm.is_initialized()) {
        builder->append(kAlgorithmFieldName, FleAlgorithm_serializer(_algorithm.get()));
    }

    if (_keyId.is_initialized()) {
        _keyId.get().serializeToBSON(kKeyIdFieldName, builder);
    }

}


BSONObj EncryptionMetadata::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData EncryptionPlaceholder::kAlgorithmFieldName;
constexpr StringData EncryptionPlaceholder::kKeyAltNameFieldName;
constexpr StringData EncryptionPlaceholder::kKeyIdFieldName;
constexpr StringData EncryptionPlaceholder::kValueFieldName;


EncryptionPlaceholder::EncryptionPlaceholder() : _hasAlgorithm(false), _hasValue(false) {
    // Used for initialization only
}
EncryptionPlaceholder::EncryptionPlaceholder(mongo::FleAlgorithmInt algorithm, mongo::EncryptSchemaAnyType value) : _algorithm(std::move(algorithm)), _value(std::move(value)), _hasAlgorithm(true), _hasValue(true) {
    // Used for initialization only
}


EncryptionPlaceholder EncryptionPlaceholder::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    EncryptionPlaceholder object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void EncryptionPlaceholder::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kAlgorithmBit = 0;
    const size_t kKeyIdBit = 1;
    const size_t kKeyAltNameBit = 2;
    const size_t kValueBit = 3;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAlgorithmFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kAlgorithmBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAlgorithmBit);

                _hasAlgorithm = true;
                IDLParserErrorContext tempContext(kAlgorithmFieldName, &ctxt);
                _algorithm = FleAlgorithmInt_parse(tempContext, element._numberInt());
            }
        }
        else if (fieldName == kKeyIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kKeyIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyIdBit);

                _keyId = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kKeyAltNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kKeyAltNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyAltNameBit);

                _keyAltName = element.str();
            }
        }
        else if (fieldName == kValueFieldName) {
            if (MONGO_unlikely(usedFields[kValueBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kValueBit);

            _hasValue = true;
            _value = EncryptSchemaAnyType::parseFromBSON(element);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kAlgorithmBit]) {
            ctxt.throwMissingField(kAlgorithmFieldName);
        }
        if (!usedFields[kValueBit]) {
            ctxt.throwMissingField(kValueFieldName);
        }
    }

}


void EncryptionPlaceholder::serialize(BSONObjBuilder* builder) const {
    invariant(_hasAlgorithm && _hasValue);

    {
        builder->append(kAlgorithmFieldName, FleAlgorithmInt_serializer(_algorithm));
    }

    if (_keyId.is_initialized()) {
        ConstDataRange tempCDR = _keyId.get().toCDR();
        builder->append(kKeyIdFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    if (_keyAltName.is_initialized()) {
        builder->append(kKeyAltNameFieldName, _keyAltName.get());
    }

    {
        _value.serializeToBSON(kValueFieldName, builder);
    }

}


BSONObj EncryptionPlaceholder::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
