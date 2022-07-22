/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/matcher/schema/encrypt_schema_gen.h --output build/opt/mongo/db/matcher/schema/encrypt_schema_gen.cpp src/mongo/db/matcher/schema/encrypt_schema.idl
 */

#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/data_range.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/simple_bsonobj_comparator.h"
#include "mongo/db/matcher/matcher_type_set.h"
#include "mongo/db/matcher/schema/encrypt_schema_types.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * The algorithm used to encrypt fields for field level encryption.
 */
enum class FleAlgorithmEnum : std::int32_t {
    kDeterministic ,
    kRandom ,
};

FleAlgorithmEnum FleAlgorithm_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData FleAlgorithm_serializer(FleAlgorithmEnum value);

/**
 * The algorithm used to encrypt fields for field level encryption represented as an unsigned integer.
 */
enum class FleAlgorithmInt : std::int32_t {
    kDeterministic  = 1,
    kRandom  = 2,
};

FleAlgorithmInt FleAlgorithmInt_parse(const IDLParserErrorContext& ctxt, std::int32_t value);
std::int32_t FleAlgorithmInt_serializer(FleAlgorithmInt value);

/**
 * Represents the fields that users can specify within the encryptMetadata subobject in a JSON Schema.
 */
class EncryptionInfo {
public:
    static constexpr auto kAlgorithmFieldName = "algorithm"_sd;
    static constexpr auto kBsonTypeFieldName = "bsonType"_sd;
    static constexpr auto kKeyIdFieldName = "keyId"_sd;

    EncryptionInfo();

    static EncryptionInfo parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Specifies the type of the to-be-encrypted value.
     */
    const boost::optional<mongo::BSONTypeSet>& getBsonType() const& { return _bsonType; }
    void getBsonType() && = delete;
    void setBsonType(boost::optional<mongo::BSONTypeSet> value) & {  _bsonType = std::move(value);  }

    /**
     * The encryption algorithm to be used.
     */
    const boost::optional<mongo::FleAlgorithmEnum> getAlgorithm() const& { return _algorithm; }
    void getAlgorithm() && = delete;
    void setAlgorithm(boost::optional<mongo::FleAlgorithmEnum> value) & {  _algorithm = std::move(value);  }

    /**
     * A JSONPointer to a key or an array of UUIDs identifying a set of keys.
     */
    const boost::optional<mongo::EncryptSchemaKeyId>& getKeyId() const& { return _keyId; }
    void getKeyId() && = delete;
    void setKeyId(boost::optional<mongo::EncryptSchemaKeyId> value) & {  _keyId = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::BSONTypeSet> _bsonType;
    boost::optional<mongo::FleAlgorithmEnum> _algorithm;
    boost::optional<mongo::EncryptSchemaKeyId> _keyId;
};

/**
 * The fields that can be applied to children in the schema with the encrypt keyword.
 */
class EncryptionMetadata {
public:
    static constexpr auto kAlgorithmFieldName = "algorithm"_sd;
    static constexpr auto kKeyIdFieldName = "keyId"_sd;

    EncryptionMetadata();

    static EncryptionMetadata parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The encryption algorithm to be used.
     */
    const boost::optional<mongo::FleAlgorithmEnum> getAlgorithm() const& { return _algorithm; }
    void getAlgorithm() && = delete;
    void setAlgorithm(boost::optional<mongo::FleAlgorithmEnum> value) & {  _algorithm = std::move(value);  }

    /**
     * A JSONPointer to a key or an array of UUIDs identifying a set of keys.
     */
    const boost::optional<mongo::EncryptSchemaKeyId>& getKeyId() const& { return _keyId; }
    void getKeyId() && = delete;
    void setKeyId(boost::optional<mongo::EncryptSchemaKeyId> value) & {  _keyId = std::move(value);  }


    friend bool operator==(const EncryptionMetadata& left, const EncryptionMetadata& right) {
        return left._algorithm == right._algorithm && left._keyId == right._keyId;
    }

    friend bool operator!=(const EncryptionMetadata& left, const EncryptionMetadata& right) {
        return left._algorithm != right._algorithm || left._keyId != right._keyId;
    }
    friend bool operator<(const EncryptionMetadata& left, const EncryptionMetadata& right) {
        return left._algorithm < right._algorithm || (!(right._algorithm < left._algorithm) && (left._keyId < right._keyId));
    }
    friend bool operator>(const EncryptionMetadata& left, const EncryptionMetadata& right) {
        return right < left;
    }
    friend bool operator<=(const EncryptionMetadata& left, const EncryptionMetadata& right) {
        return !(right < left);
    }
    friend bool operator>=(const EncryptionMetadata& left, const EncryptionMetadata& right) {
        return !(left < right);
    }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::FleAlgorithmEnum> _algorithm;
    boost::optional<mongo::EncryptSchemaKeyId> _keyId;
};

/**
 * Implements Encryption BinData (subtype 6) sub-subtype 0, the intent-to-encrypt mapping. Contains a value to encrypt and a description of how it should be encrypted.
 */
class EncryptionPlaceholder {
public:
    static constexpr auto kAlgorithmFieldName = "a"_sd;
    static constexpr auto kKeyAltNameFieldName = "ka"_sd;
    static constexpr auto kKeyIdFieldName = "ki"_sd;
    static constexpr auto kValueFieldName = "v"_sd;

    EncryptionPlaceholder();
    EncryptionPlaceholder(mongo::FleAlgorithmInt algorithm, mongo::EncryptSchemaAnyType value);

    static EncryptionPlaceholder parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The encryption algorithm to be used.
     */
    mongo::FleAlgorithmInt getAlgorithm() const { return _algorithm; }
    void setAlgorithm(mongo::FleAlgorithmInt value) & {  _algorithm = std::move(value); _hasAlgorithm = true; }

    /**
     * Used to query the key vault by _id. If omitted, ka must be specified.
     */
    const boost::optional<mongo::UUID>& getKeyId() const& { return _keyId; }
    void getKeyId() && = delete;
    void setKeyId(boost::optional<mongo::UUID> value) & {  _keyId = std::move(value);  }

    /**
     * Used to query the key vault by keyAltName. If omitted, ki must be specified.
     */
    const boost::optional<StringData> getKeyAltName() const& { return boost::optional<StringData>{_keyAltName}; }
    void getKeyAltName() && = delete;
    void setKeyAltName(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _keyAltName = std::move(_tmpValue);
    } else {
        _keyAltName = boost::none;
    }
      }

    /**
     * value to encrypt
     */
    const mongo::EncryptSchemaAnyType& getValue() const { return _value; }
    void setValue(mongo::EncryptSchemaAnyType value) & {  _value = std::move(value); _hasValue = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::FleAlgorithmInt _algorithm;
    boost::optional<mongo::UUID> _keyId;
    boost::optional<std::string> _keyAltName;
    mongo::EncryptSchemaAnyType _value;
    bool _hasAlgorithm : 1;
    bool _hasValue : 1;
};

}  // namespace mongo
