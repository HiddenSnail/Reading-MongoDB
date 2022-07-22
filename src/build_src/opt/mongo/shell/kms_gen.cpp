/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/shell/kms_gen.h --output build/opt/mongo/shell/kms_gen.cpp src/mongo/shell/kms.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/shell/kms_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Enumeration of supported KMS Providers
 */
namespace  {
constexpr StringData kKMSProvider_aws = "aws"_sd;
constexpr StringData kKMSProvider_local = "local"_sd;
}  // namespace 

KMSProviderEnum KMSProvider_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kKMSProvider_aws) {
        return KMSProviderEnum::aws;
    }
    if (value == kKMSProvider_local) {
        return KMSProviderEnum::local;
    }
    ctxt.throwBadEnumValue(value);
}

StringData KMSProvider_serializer(KMSProviderEnum value) {
    if (value == KMSProviderEnum::aws) {
        return kKMSProvider_aws;
    }
    if (value == KMSProviderEnum::local) {
        return kKMSProvider_local;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData AwsKMSError::kMessageFieldName;
constexpr StringData AwsKMSError::kTypeFieldName;


AwsKMSError::AwsKMSError() : _hasType(false), _hasMessage(false) {
    // Used for initialization only
}
AwsKMSError::AwsKMSError(std::string type, std::string message) : _type(std::move(type)), _message(std::move(message)), _hasType(true), _hasMessage(true) {
    // Used for initialization only
}


AwsKMSError AwsKMSError::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsKMSError object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsKMSError::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasType = true;
                _type = element.str();
            }
        }
        else if (fieldName == kMessageFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasMessage = true;
                _message = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kTypeFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTypeFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMessageFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMessageFieldName);
    }

}


void AwsKMSError::serialize(BSONObjBuilder* builder) const {
    invariant(_hasType && _hasMessage);

    builder->append(kTypeFieldName, _type);

    builder->append(kMessageFieldName, _message);

}


BSONObj AwsKMSError::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsKMS::kAccessKeyIdFieldName;
constexpr StringData AwsKMS::kSecretAccessKeyFieldName;
constexpr StringData AwsKMS::kSessionTokenFieldName;
constexpr StringData AwsKMS::kUrlFieldName;


AwsKMS::AwsKMS() : _hasAccessKeyId(false), _hasSecretAccessKey(false) {
    // Used for initialization only
}
AwsKMS::AwsKMS(std::string accessKeyId, std::string secretAccessKey) : _accessKeyId(std::move(accessKeyId)), _secretAccessKey(std::move(secretAccessKey)), _hasAccessKeyId(true), _hasSecretAccessKey(true) {
    // Used for initialization only
}


AwsKMS AwsKMS::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsKMS object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsKMS::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kAccessKeyIdBit = 0;
    const size_t kSecretAccessKeyBit = 1;
    const size_t kSessionTokenBit = 2;
    const size_t kUrlBit = 3;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAccessKeyIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kAccessKeyIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAccessKeyIdBit);

                _hasAccessKeyId = true;
                _accessKeyId = element.str();
            }
        }
        else if (fieldName == kSecretAccessKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kSecretAccessKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSecretAccessKeyBit);

                _hasSecretAccessKey = true;
                _secretAccessKey = element.str();
            }
        }
        else if (fieldName == kSessionTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kSessionTokenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSessionTokenBit);

                _sessionToken = element.str();
            }
        }
        else if (fieldName == kUrlFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kUrlBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUrlBit);

                _url = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kAccessKeyIdBit]) {
            ctxt.throwMissingField(kAccessKeyIdFieldName);
        }
        if (!usedFields[kSecretAccessKeyBit]) {
            ctxt.throwMissingField(kSecretAccessKeyFieldName);
        }
    }

}


void AwsKMS::serialize(BSONObjBuilder* builder) const {
    invariant(_hasAccessKeyId && _hasSecretAccessKey);

    builder->append(kAccessKeyIdFieldName, _accessKeyId);

    builder->append(kSecretAccessKeyFieldName, _secretAccessKey);

    if (_sessionToken.is_initialized()) {
        builder->append(kSessionTokenFieldName, _sessionToken.get());
    }

    if (_url.is_initialized()) {
        builder->append(kUrlFieldName, _url.get());
    }

}


BSONObj AwsKMS::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData LocalKMS::kKeyFieldName;


LocalKMS::LocalKMS() : _hasKey(false) {
    // Used for initialization only
}
LocalKMS::LocalKMS(std::vector<std::uint8_t> key) : _key(std::move(key)), _hasKey(true) {
    // Used for initialization only
}


LocalKMS LocalKMS::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    LocalKMS object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void LocalKMS::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kKeyBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyBit);

                _hasKey = true;
                _key = element._binDataVector();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kKeyBit]) {
            ctxt.throwMissingField(kKeyFieldName);
        }
    }

}


void LocalKMS::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKey);

    {
        ConstDataRange tempCDR(_key);
        builder->append(kKeyFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

}


BSONObj LocalKMS::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData KmsProviders::kAwsFieldName;
constexpr StringData KmsProviders::kLocalFieldName;


KmsProviders::KmsProviders()  {
    // Used for initialization only
}


KmsProviders KmsProviders::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    KmsProviders object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void KmsProviders::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kAwsBit = 0;
    const size_t kLocalBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAwsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kAwsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAwsBit);

                IDLParserErrorContext tempContext(kAwsFieldName, &ctxt);
                const auto localObject = element.Obj();
                _aws = mongo::AwsKMS::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kLocalFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLocalBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLocalBit);

                IDLParserErrorContext tempContext(kLocalFieldName, &ctxt);
                const auto localObject = element.Obj();
                _local = mongo::LocalKMS::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void KmsProviders::serialize(BSONObjBuilder* builder) const {
    if (_aws.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kAwsFieldName));
        _aws.get().serialize(&subObjBuilder);
    }

    if (_local.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kLocalFieldName));
        _local.get().serialize(&subObjBuilder);
    }

}


BSONObj KmsProviders::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ClientSideFLEOptions::kBypassAutoEncryptionFieldName;
constexpr StringData ClientSideFLEOptions::kKeyVaultClientFieldName;
constexpr StringData ClientSideFLEOptions::kKeyVaultNamespaceFieldName;
constexpr StringData ClientSideFLEOptions::kKmsProvidersFieldName;
constexpr StringData ClientSideFLEOptions::kSchemaMapFieldName;


ClientSideFLEOptions::ClientSideFLEOptions() : _hasKeyVaultNamespace(false), _hasKmsProviders(false) {
    // Used for initialization only
}
ClientSideFLEOptions::ClientSideFLEOptions(std::string keyVaultNamespace, mongo::KmsProviders kmsProviders) : _keyVaultNamespace(std::move(keyVaultNamespace)), _kmsProviders(std::move(kmsProviders)), _hasKeyVaultNamespace(true), _hasKmsProviders(true) {
    // Used for initialization only
}


ClientSideFLEOptions ClientSideFLEOptions::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ClientSideFLEOptions object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ClientSideFLEOptions::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kKeyVaultClientBit = 0;
    const size_t kKeyVaultNamespaceBit = 1;
    const size_t kKmsProvidersBit = 2;
    const size_t kSchemaMapBit = 3;
    const size_t kBypassAutoEncryptionBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kKeyVaultClientFieldName) {
            if (MONGO_unlikely(usedFields[kKeyVaultClientBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kKeyVaultClientBit);

            // ignore field
        }
        else if (fieldName == kKeyVaultNamespaceFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kKeyVaultNamespaceBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyVaultNamespaceBit);

                _hasKeyVaultNamespace = true;
                _keyVaultNamespace = element.str();
            }
        }
        else if (fieldName == kKmsProvidersFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kKmsProvidersBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKmsProvidersBit);

                _hasKmsProviders = true;
                IDLParserErrorContext tempContext(kKmsProvidersFieldName, &ctxt);
                const auto localObject = element.Obj();
                _kmsProviders = mongo::KmsProviders::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kSchemaMapFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSchemaMapBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSchemaMapBit);

                _schemaMap = element.Obj();
            }
        }
        else if (fieldName == kBypassAutoEncryptionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kBypassAutoEncryptionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassAutoEncryptionBit);

                _bypassAutoEncryption = element.boolean();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kKeyVaultNamespaceBit]) {
            ctxt.throwMissingField(kKeyVaultNamespaceFieldName);
        }
        if (!usedFields[kKmsProvidersBit]) {
            ctxt.throwMissingField(kKmsProvidersFieldName);
        }
    }

}


void ClientSideFLEOptions::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKeyVaultNamespace && _hasKmsProviders);

    builder->append(kKeyVaultNamespaceFieldName, _keyVaultNamespace);

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kKmsProvidersFieldName));
        _kmsProviders.serialize(&subObjBuilder);
    }

    if (_schemaMap.is_initialized()) {
        builder->append(kSchemaMapFieldName, _schemaMap.get());
    }

    if (_bypassAutoEncryption.is_initialized()) {
        builder->append(kBypassAutoEncryptionFieldName, _bypassAutoEncryption.get());
    }

}


BSONObj ClientSideFLEOptions::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsEncryptResponse::kCiphertextBlobFieldName;
constexpr StringData AwsEncryptResponse::kKeyIdFieldName;


AwsEncryptResponse::AwsEncryptResponse() : _hasCiphertextBlob(false), _hasKeyId(false) {
    // Used for initialization only
}
AwsEncryptResponse::AwsEncryptResponse(std::string ciphertextBlob, std::string keyId) : _ciphertextBlob(std::move(ciphertextBlob)), _keyId(std::move(keyId)), _hasCiphertextBlob(true), _hasKeyId(true) {
    // Used for initialization only
}


AwsEncryptResponse AwsEncryptResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsEncryptResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsEncryptResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kCiphertextBlobFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasCiphertextBlob = true;
                _ciphertextBlob = element.str();
            }
        }
        else if (fieldName == kKeyIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasKeyId = true;
                _keyId = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kCiphertextBlobFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCiphertextBlobFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kKeyIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyIdFieldName);
    }

}


void AwsEncryptResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasCiphertextBlob && _hasKeyId);

    builder->append(kCiphertextBlobFieldName, _ciphertextBlob);

    builder->append(kKeyIdFieldName, _keyId);

}


BSONObj AwsEncryptResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsDecryptResponse::kKeyIdFieldName;
constexpr StringData AwsDecryptResponse::kPlaintextFieldName;


AwsDecryptResponse::AwsDecryptResponse() : _hasPlaintext(false), _hasKeyId(false) {
    // Used for initialization only
}
AwsDecryptResponse::AwsDecryptResponse(std::string plaintext, std::string keyId) : _plaintext(std::move(plaintext)), _keyId(std::move(keyId)), _hasPlaintext(true), _hasKeyId(true) {
    // Used for initialization only
}


AwsDecryptResponse AwsDecryptResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsDecryptResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsDecryptResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kPlaintextFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasPlaintext = true;
                _plaintext = element.str();
            }
        }
        else if (fieldName == kKeyIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasKeyId = true;
                _keyId = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kPlaintextFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kPlaintextFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kKeyIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kKeyIdFieldName);
    }

}


void AwsDecryptResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasPlaintext && _hasKeyId);

    builder->append(kPlaintextFieldName, _plaintext);

    builder->append(kKeyIdFieldName, _keyId);

}


BSONObj AwsDecryptResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsMasterKey::kEndpointFieldName;
constexpr StringData AwsMasterKey::kKeyFieldName;
constexpr StringData AwsMasterKey::kProviderFieldName;
constexpr StringData AwsMasterKey::kRegionFieldName;


AwsMasterKey::AwsMasterKey() : _hasKey(false), _hasRegion(false) {
    // Used for initialization only
}
AwsMasterKey::AwsMasterKey(std::string key, std::string region) : _key(std::move(key)), _region(std::move(region)), _hasKey(true), _hasRegion(true) {
    // Used for initialization only
}


AwsMasterKey AwsMasterKey::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsMasterKey object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsMasterKey::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kProviderBit = 0;
    const size_t kKeyBit = 1;
    const size_t kRegionBit = 2;
    const size_t kEndpointBit = 3;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kProviderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kProviderBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kProviderBit);

                _provider = element.str();
            }
        }
        else if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyBit);

                _hasKey = true;
                _key = element.str();
            }
        }
        else if (fieldName == kRegionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kRegionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRegionBit);

                _hasRegion = true;
                _region = element.str();
            }
        }
        else if (fieldName == kEndpointFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kEndpointBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kEndpointBit);

                _endpoint = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kProviderBit]) {
            _provider = "aws";
        }
        if (!usedFields[kKeyBit]) {
            ctxt.throwMissingField(kKeyFieldName);
        }
        if (!usedFields[kRegionBit]) {
            ctxt.throwMissingField(kRegionFieldName);
        }
    }

}


void AwsMasterKey::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKey && _hasRegion);

    builder->append(kProviderFieldName, _provider);

    builder->append(kKeyFieldName, _key);

    builder->append(kRegionFieldName, _region);

    if (_endpoint.is_initialized()) {
        builder->append(kEndpointFieldName, _endpoint.get());
    }

}


BSONObj AwsMasterKey::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsMasterKeyAndMaterial::kKeyMaterialFieldName;
constexpr StringData AwsMasterKeyAndMaterial::kMasterKeyFieldName;


AwsMasterKeyAndMaterial::AwsMasterKeyAndMaterial() : _hasKeyMaterial(false), _hasMasterKey(false) {
    // Used for initialization only
}
AwsMasterKeyAndMaterial::AwsMasterKeyAndMaterial(std::vector<std::uint8_t> keyMaterial, mongo::AwsMasterKey masterKey) : _keyMaterial(std::move(keyMaterial)), _masterKey(std::move(masterKey)), _hasKeyMaterial(true), _hasMasterKey(true) {
    // Used for initialization only
}


AwsMasterKeyAndMaterial AwsMasterKeyAndMaterial::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsMasterKeyAndMaterial object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsMasterKeyAndMaterial::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kKeyMaterialBit = 0;
    const size_t kMasterKeyBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kKeyMaterialFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kKeyMaterialBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyMaterialBit);

                _hasKeyMaterial = true;
                _keyMaterial = element._binDataVector();
            }
        }
        else if (fieldName == kMasterKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kMasterKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMasterKeyBit);

                _hasMasterKey = true;
                IDLParserErrorContext tempContext(kMasterKeyFieldName, &ctxt);
                const auto localObject = element.Obj();
                _masterKey = mongo::AwsMasterKey::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kKeyMaterialBit]) {
            ctxt.throwMissingField(kKeyMaterialFieldName);
        }
        if (!usedFields[kMasterKeyBit]) {
            ctxt.throwMissingField(kMasterKeyFieldName);
        }
    }

}


void AwsMasterKeyAndMaterial::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKeyMaterial && _hasMasterKey);

    {
        ConstDataRange tempCDR(_keyMaterial);
        builder->append(kKeyMaterialFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kMasterKeyFieldName));
        _masterKey.serialize(&subObjBuilder);
    }

}


BSONObj AwsMasterKeyAndMaterial::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData LocalMasterKey::kProviderFieldName;


LocalMasterKey::LocalMasterKey()  {
    // Used for initialization only
}


LocalMasterKey LocalMasterKey::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    LocalMasterKey object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void LocalMasterKey::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kProviderBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kProviderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kProviderBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kProviderBit);

                _provider = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kProviderBit]) {
            _provider = "local";
        }
    }

}


void LocalMasterKey::serialize(BSONObjBuilder* builder) const {
    builder->append(kProviderFieldName, _provider);

}


BSONObj LocalMasterKey::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData LocalMasterKeyAndMaterial::kKeyMaterialFieldName;
constexpr StringData LocalMasterKeyAndMaterial::kMasterKeyFieldName;


LocalMasterKeyAndMaterial::LocalMasterKeyAndMaterial() : _hasKeyMaterial(false), _hasMasterKey(false) {
    // Used for initialization only
}
LocalMasterKeyAndMaterial::LocalMasterKeyAndMaterial(std::vector<std::uint8_t> keyMaterial, mongo::LocalMasterKey masterKey) : _keyMaterial(std::move(keyMaterial)), _masterKey(std::move(masterKey)), _hasKeyMaterial(true), _hasMasterKey(true) {
    // Used for initialization only
}


LocalMasterKeyAndMaterial LocalMasterKeyAndMaterial::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    LocalMasterKeyAndMaterial object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void LocalMasterKeyAndMaterial::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kKeyMaterialBit = 0;
    const size_t kMasterKeyBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kKeyMaterialFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kKeyMaterialBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyMaterialBit);

                _hasKeyMaterial = true;
                _keyMaterial = element._binDataVector();
            }
        }
        else if (fieldName == kMasterKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kMasterKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMasterKeyBit);

                _hasMasterKey = true;
                IDLParserErrorContext tempContext(kMasterKeyFieldName, &ctxt);
                const auto localObject = element.Obj();
                _masterKey = mongo::LocalMasterKey::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kKeyMaterialBit]) {
            ctxt.throwMissingField(kKeyMaterialFieldName);
        }
        if (!usedFields[kMasterKeyBit]) {
            ctxt.throwMissingField(kMasterKeyFieldName);
        }
    }

}


void LocalMasterKeyAndMaterial::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKeyMaterial && _hasMasterKey);

    {
        ConstDataRange tempCDR(_keyMaterial);
        builder->append(kKeyMaterialFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kMasterKeyFieldName));
        _masterKey.serialize(&subObjBuilder);
    }

}


BSONObj LocalMasterKeyAndMaterial::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData KeyStoreRecord::k_idFieldName;
constexpr StringData KeyStoreRecord::kCreationDateFieldName;
constexpr StringData KeyStoreRecord::kKeyAltNamesFieldName;
constexpr StringData KeyStoreRecord::kKeyMaterialFieldName;
constexpr StringData KeyStoreRecord::kMasterKeyFieldName;
constexpr StringData KeyStoreRecord::kStatusFieldName;
constexpr StringData KeyStoreRecord::kUpdateDateFieldName;
constexpr StringData KeyStoreRecord::kVersionFieldName;


KeyStoreRecord::KeyStoreRecord() : _status(-1), _has_id(false), _hasKeyMaterial(false), _hasCreationDate(false), _hasUpdateDate(false), _hasStatus(false), _hasMasterKey(false) {
    // Used for initialization only
}
KeyStoreRecord::KeyStoreRecord(mongo::UUID _id, std::vector<std::uint8_t> keyMaterial, mongo::Date_t creationDate, mongo::Date_t updateDate, std::int32_t status, mongo::BSONObj masterKey) : __id(std::move(_id)), _keyMaterial(std::move(keyMaterial)), _creationDate(std::move(creationDate)), _updateDate(std::move(updateDate)), _status(std::move(status)), _masterKey(std::move(masterKey)), _has_id(true), _hasKeyMaterial(true), _hasCreationDate(true), _hasUpdateDate(true), _hasStatus(true), _hasMasterKey(true) {
    // Used for initialization only
}


KeyStoreRecord KeyStoreRecord::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    KeyStoreRecord object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void KeyStoreRecord::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<8> usedFields;
    const size_t k_idBit = 0;
    const size_t kKeyMaterialBit = 1;
    const size_t kCreationDateBit = 2;
    const size_t kUpdateDateBit = 3;
    const size_t kStatusBit = 4;
    const size_t kVersionBit = 5;
    const size_t kMasterKeyBit = 6;
    const size_t kKeyAltNamesBit = 7;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[k_idBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(k_idBit);

                _has_id = true;
                __id = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kKeyMaterialFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kKeyMaterialBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyMaterialBit);

                _hasKeyMaterial = true;
                _keyMaterial = element._binDataVector();
            }
        }
        else if (fieldName == kCreationDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kCreationDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCreationDateBit);

                _hasCreationDate = true;
                _creationDate = element.date();
            }
        }
        else if (fieldName == kUpdateDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kUpdateDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUpdateDateBit);

                _hasUpdateDate = true;
                _updateDate = element.date();
            }
        }
        else if (fieldName == kStatusFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStatusBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStatusBit);

                _hasStatus = true;
                _status = element._numberInt();
            }
        }
        else if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                _version = element._numberLong();
            }
        }
        else if (fieldName == kMasterKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kMasterKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMasterKeyBit);

                _hasMasterKey = true;
                _masterKey = element.Obj();
            }
        }
        else if (fieldName == kKeyAltNamesFieldName) {
            if (MONGO_unlikely(usedFields[kKeyAltNamesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kKeyAltNamesBit);

            // ignore field
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[k_idBit]) {
            ctxt.throwMissingField(k_idFieldName);
        }
        if (!usedFields[kKeyMaterialBit]) {
            ctxt.throwMissingField(kKeyMaterialFieldName);
        }
        if (!usedFields[kCreationDateBit]) {
            ctxt.throwMissingField(kCreationDateFieldName);
        }
        if (!usedFields[kUpdateDateBit]) {
            ctxt.throwMissingField(kUpdateDateFieldName);
        }
        if (!usedFields[kStatusBit]) {
            ctxt.throwMissingField(kStatusFieldName);
        }
        if (!usedFields[kVersionBit]) {
            _version = 0;
        }
        if (!usedFields[kMasterKeyBit]) {
            ctxt.throwMissingField(kMasterKeyFieldName);
        }
    }

}


void KeyStoreRecord::serialize(BSONObjBuilder* builder) const {
    invariant(_has_id && _hasKeyMaterial && _hasCreationDate && _hasUpdateDate && _hasStatus && _hasMasterKey);

    {
        ConstDataRange tempCDR = __id.toCDR();
        builder->append(k_idFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        ConstDataRange tempCDR(_keyMaterial);
        builder->append(kKeyMaterialFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

    builder->append(kCreationDateFieldName, _creationDate);

    builder->append(kUpdateDateFieldName, _updateDate);

    builder->append(kStatusFieldName, _status);

    builder->append(kVersionFieldName, _version);

    builder->append(kMasterKeyFieldName, _masterKey);

}


BSONObj KeyStoreRecord::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
