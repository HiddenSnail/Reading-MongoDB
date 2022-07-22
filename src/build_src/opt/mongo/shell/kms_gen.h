/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/shell/kms_gen.h --output build/opt/mongo/shell/kms_gen.cpp src/mongo/shell/kms.idl
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
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Enumeration of supported KMS Providers
 */
enum class KMSProviderEnum : std::int32_t {
    aws ,
    local ,
};

KMSProviderEnum KMSProvider_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData KMSProvider_serializer(KMSProviderEnum value);

/**
 * AWS KMS error
 */
class AwsKMSError {
public:
    static constexpr auto kMessageFieldName = "Message"_sd;
    static constexpr auto kTypeFieldName = "__type"_sd;

    AwsKMSError();
    AwsKMSError(std::string type, std::string message);

    static AwsKMSError parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getType() const& { return _type; }
    void getType() && = delete;
    void setType(StringData value) & { auto _tmpValue = value.toString();  _type = std::move(_tmpValue); _hasType = true; }

    const StringData getMessage() const& { return _message; }
    void getMessage() && = delete;
    void setMessage(StringData value) & { auto _tmpValue = value.toString();  _message = std::move(_tmpValue); _hasMessage = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _type;
    std::string _message;
    bool _hasType : 1;
    bool _hasMessage : 1;
};

/**
 * AWS KMS config
 */
class AwsKMS {
public:
    static constexpr auto kAccessKeyIdFieldName = "accessKeyId"_sd;
    static constexpr auto kSecretAccessKeyFieldName = "secretAccessKey"_sd;
    static constexpr auto kSessionTokenFieldName = "sessionToken"_sd;
    static constexpr auto kUrlFieldName = "url"_sd;

    AwsKMS();
    AwsKMS(std::string accessKeyId, std::string secretAccessKey);

    static AwsKMS parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getAccessKeyId() const& { return _accessKeyId; }
    void getAccessKeyId() && = delete;
    void setAccessKeyId(StringData value) & { auto _tmpValue = value.toString();  _accessKeyId = std::move(_tmpValue); _hasAccessKeyId = true; }

    const StringData getSecretAccessKey() const& { return _secretAccessKey; }
    void getSecretAccessKey() && = delete;
    void setSecretAccessKey(StringData value) & { auto _tmpValue = value.toString();  _secretAccessKey = std::move(_tmpValue); _hasSecretAccessKey = true; }

    const boost::optional<StringData> getSessionToken() const& { return boost::optional<StringData>{_sessionToken}; }
    void getSessionToken() && = delete;
    void setSessionToken(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _sessionToken = std::move(_tmpValue);
    } else {
        _sessionToken = boost::none;
    }
      }

    const boost::optional<StringData> getUrl() const& { return boost::optional<StringData>{_url}; }
    void getUrl() && = delete;
    void setUrl(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _url = std::move(_tmpValue);
    } else {
        _url = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _accessKeyId;
    std::string _secretAccessKey;
    boost::optional<std::string> _sessionToken;
    boost::optional<std::string> _url;
    bool _hasAccessKeyId : 1;
    bool _hasSecretAccessKey : 1;
};

/**
 * Local KMS config
 */
class LocalKMS {
public:
    static constexpr auto kKeyFieldName = "key"_sd;

    LocalKMS();
    LocalKMS(std::vector<std::uint8_t> key);

    static LocalKMS parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const ConstDataRange getKey() const& { return ConstDataRange(_key); }
    void getKey() && = delete;
    void setKey(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _key = std::move(_tmpValue); _hasKey = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<std::uint8_t> _key;
    bool _hasKey : 1;
};

/**
 * Supported KMS Providers
 */
class KmsProviders {
public:
    static constexpr auto kAwsFieldName = "aws"_sd;
    static constexpr auto kLocalFieldName = "local"_sd;

    KmsProviders();

    static KmsProviders parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const boost::optional<mongo::AwsKMS>& getAws() const& { return _aws; }
    void getAws() && = delete;
    void setAws(boost::optional<mongo::AwsKMS> value) & {  _aws = std::move(value);  }

    const boost::optional<mongo::LocalKMS>& getLocal() const& { return _local; }
    void getLocal() && = delete;
    void setLocal(boost::optional<mongo::LocalKMS> value) & {  _local = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::AwsKMS> _aws;
    boost::optional<mongo::LocalKMS> _local;
};

/**
 * FLE Options inputted through the Mongo constructor in the shell
 */
class ClientSideFLEOptions {
public:
    static constexpr auto kBypassAutoEncryptionFieldName = "bypassAutoEncryption"_sd;
    static constexpr auto kKeyVaultClientFieldName = "keyVaultClient"_sd;
    static constexpr auto kKeyVaultNamespaceFieldName = "keyVaultNamespace"_sd;
    static constexpr auto kKmsProvidersFieldName = "kmsProviders"_sd;
    static constexpr auto kSchemaMapFieldName = "schemaMap"_sd;

    ClientSideFLEOptions();
    ClientSideFLEOptions(std::string keyVaultNamespace, mongo::KmsProviders kmsProviders);

    static ClientSideFLEOptions parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getKeyVaultNamespace() const& { return _keyVaultNamespace; }
    void getKeyVaultNamespace() && = delete;
    void setKeyVaultNamespace(StringData value) & { auto _tmpValue = value.toString();  _keyVaultNamespace = std::move(_tmpValue); _hasKeyVaultNamespace = true; }

    const mongo::KmsProviders& getKmsProviders() const { return _kmsProviders; }
    mongo::KmsProviders& getKmsProviders() { return _kmsProviders; }
    void setKmsProviders(mongo::KmsProviders value) & {  _kmsProviders = std::move(value); _hasKmsProviders = true; }

    const boost::optional<mongo::BSONObj>& getSchemaMap() const& { return _schemaMap; }
    void getSchemaMap() && = delete;
    void setSchemaMap(boost::optional<mongo::BSONObj> value) & {  _schemaMap = std::move(value);  }

    const boost::optional<bool> getBypassAutoEncryption() const& { return _bypassAutoEncryption; }
    void getBypassAutoEncryption() && = delete;
    void setBypassAutoEncryption(boost::optional<bool> value) & {  _bypassAutoEncryption = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _keyVaultNamespace;
    mongo::KmsProviders _kmsProviders;
    boost::optional<mongo::BSONObj> _schemaMap;
    boost::optional<bool> _bypassAutoEncryption;
    bool _hasKeyVaultNamespace : 1;
    bool _hasKmsProviders : 1;
};

/**
 * Response from AWS KMS Encrypt request, i.e. TrentService.Encrypt
 */
class AwsEncryptResponse {
public:
    static constexpr auto kCiphertextBlobFieldName = "CiphertextBlob"_sd;
    static constexpr auto kKeyIdFieldName = "KeyId"_sd;

    AwsEncryptResponse();
    AwsEncryptResponse(std::string ciphertextBlob, std::string keyId);

    static AwsEncryptResponse parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getCiphertextBlob() const& { return _ciphertextBlob; }
    void getCiphertextBlob() && = delete;
    void setCiphertextBlob(StringData value) & { auto _tmpValue = value.toString();  _ciphertextBlob = std::move(_tmpValue); _hasCiphertextBlob = true; }

    const StringData getKeyId() const& { return _keyId; }
    void getKeyId() && = delete;
    void setKeyId(StringData value) & { auto _tmpValue = value.toString();  _keyId = std::move(_tmpValue); _hasKeyId = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _ciphertextBlob;
    std::string _keyId;
    bool _hasCiphertextBlob : 1;
    bool _hasKeyId : 1;
};

/**
 * Response from AWS KMS Decrypt request, i.e. TrentService.Decrypt
 */
class AwsDecryptResponse {
public:
    static constexpr auto kKeyIdFieldName = "KeyId"_sd;
    static constexpr auto kPlaintextFieldName = "Plaintext"_sd;

    AwsDecryptResponse();
    AwsDecryptResponse(std::string plaintext, std::string keyId);

    static AwsDecryptResponse parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getPlaintext() const& { return _plaintext; }
    void getPlaintext() && = delete;
    void setPlaintext(StringData value) & { auto _tmpValue = value.toString();  _plaintext = std::move(_tmpValue); _hasPlaintext = true; }

    const StringData getKeyId() const& { return _keyId; }
    void getKeyId() && = delete;
    void setKeyId(StringData value) & { auto _tmpValue = value.toString();  _keyId = std::move(_tmpValue); _hasKeyId = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _plaintext;
    std::string _keyId;
    bool _hasPlaintext : 1;
    bool _hasKeyId : 1;
};

/**
 * AWS KMS Key Store Description
 */
class AwsMasterKey {
public:
    static constexpr auto kEndpointFieldName = "endpoint"_sd;
    static constexpr auto kKeyFieldName = "key"_sd;
    static constexpr auto kProviderFieldName = "provider"_sd;
    static constexpr auto kRegionFieldName = "region"_sd;

    AwsMasterKey();
    AwsMasterKey(std::string key, std::string region);

    static AwsMasterKey parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getProvider() const& { return _provider; }
    void getProvider() && = delete;
    void setProvider(StringData value) & { auto _tmpValue = value.toString();  _provider = std::move(_tmpValue);  }

    const StringData getKey() const& { return _key; }
    void getKey() && = delete;
    void setKey(StringData value) & { auto _tmpValue = value.toString();  _key = std::move(_tmpValue); _hasKey = true; }

    const StringData getRegion() const& { return _region; }
    void getRegion() && = delete;
    void setRegion(StringData value) & { auto _tmpValue = value.toString();  _region = std::move(_tmpValue); _hasRegion = true; }

    const boost::optional<StringData> getEndpoint() const& { return boost::optional<StringData>{_endpoint}; }
    void getEndpoint() && = delete;
    void setEndpoint(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _endpoint = std::move(_tmpValue);
    } else {
        _endpoint = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _provider{"aws"};
    std::string _key;
    std::string _region;
    boost::optional<std::string> _endpoint;
    bool _hasKey : 1;
    bool _hasRegion : 1;
};

/**
 * AWS KMS Key Material Description
 */
class AwsMasterKeyAndMaterial {
public:
    static constexpr auto kKeyMaterialFieldName = "keyMaterial"_sd;
    static constexpr auto kMasterKeyFieldName = "masterKey"_sd;

    AwsMasterKeyAndMaterial();
    AwsMasterKeyAndMaterial(std::vector<std::uint8_t> keyMaterial, mongo::AwsMasterKey masterKey);

    static AwsMasterKeyAndMaterial parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const ConstDataRange getKeyMaterial() const& { return ConstDataRange(_keyMaterial); }
    void getKeyMaterial() && = delete;
    void setKeyMaterial(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _keyMaterial = std::move(_tmpValue); _hasKeyMaterial = true; }

    const mongo::AwsMasterKey& getMasterKey() const { return _masterKey; }
    mongo::AwsMasterKey& getMasterKey() { return _masterKey; }
    void setMasterKey(mongo::AwsMasterKey value) & {  _masterKey = std::move(value); _hasMasterKey = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<std::uint8_t> _keyMaterial;
    mongo::AwsMasterKey _masterKey;
    bool _hasKeyMaterial : 1;
    bool _hasMasterKey : 1;
};

/**
 * Local KMS Key Store Description
 */
class LocalMasterKey {
public:
    static constexpr auto kProviderFieldName = "provider"_sd;

    LocalMasterKey();

    static LocalMasterKey parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getProvider() const& { return _provider; }
    void getProvider() && = delete;
    void setProvider(StringData value) & { auto _tmpValue = value.toString();  _provider = std::move(_tmpValue);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _provider{"local"};
};

/**
 * Local KMS Key Material Description
 */
class LocalMasterKeyAndMaterial {
public:
    static constexpr auto kKeyMaterialFieldName = "keyMaterial"_sd;
    static constexpr auto kMasterKeyFieldName = "masterKey"_sd;

    LocalMasterKeyAndMaterial();
    LocalMasterKeyAndMaterial(std::vector<std::uint8_t> keyMaterial, mongo::LocalMasterKey masterKey);

    static LocalMasterKeyAndMaterial parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const ConstDataRange getKeyMaterial() const& { return ConstDataRange(_keyMaterial); }
    void getKeyMaterial() && = delete;
    void setKeyMaterial(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _keyMaterial = std::move(_tmpValue); _hasKeyMaterial = true; }

    const mongo::LocalMasterKey& getMasterKey() const { return _masterKey; }
    mongo::LocalMasterKey& getMasterKey() { return _masterKey; }
    void setMasterKey(mongo::LocalMasterKey value) & {  _masterKey = std::move(value); _hasMasterKey = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<std::uint8_t> _keyMaterial;
    mongo::LocalMasterKey _masterKey;
    bool _hasKeyMaterial : 1;
    bool _hasMasterKey : 1;
};

/**
 * A V0 Key Store Record
 */
class KeyStoreRecord {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kCreationDateFieldName = "creationDate"_sd;
    static constexpr auto kKeyAltNamesFieldName = "keyAltNames"_sd;
    static constexpr auto kKeyMaterialFieldName = "keyMaterial"_sd;
    static constexpr auto kMasterKeyFieldName = "masterKey"_sd;
    static constexpr auto kStatusFieldName = "status"_sd;
    static constexpr auto kUpdateDateFieldName = "updateDate"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    KeyStoreRecord();
    KeyStoreRecord(mongo::UUID _id, std::vector<std::uint8_t> keyMaterial, mongo::Date_t creationDate, mongo::Date_t updateDate, std::int32_t status, mongo::BSONObj masterKey);

    static KeyStoreRecord parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::UUID& get_id() const { return __id; }
    void set_id(mongo::UUID value) & {  __id = std::move(value); _has_id = true; }

    const ConstDataRange getKeyMaterial() const& { return ConstDataRange(_keyMaterial); }
    void getKeyMaterial() && = delete;
    void setKeyMaterial(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _keyMaterial = std::move(_tmpValue); _hasKeyMaterial = true; }

    const mongo::Date_t& getCreationDate() const { return _creationDate; }
    void setCreationDate(mongo::Date_t value) & {  _creationDate = std::move(value); _hasCreationDate = true; }

    const mongo::Date_t& getUpdateDate() const { return _updateDate; }
    void setUpdateDate(mongo::Date_t value) & {  _updateDate = std::move(value); _hasUpdateDate = true; }

    std::int32_t getStatus() const { return _status; }
    void setStatus(std::int32_t value) & {  _status = std::move(value); _hasStatus = true; }

    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value);  }

    const mongo::BSONObj& getMasterKey() const { return _masterKey; }
    void setMasterKey(mongo::BSONObj value) & {  _masterKey = std::move(value); _hasMasterKey = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID __id;
    std::vector<std::uint8_t> _keyMaterial;
    mongo::Date_t _creationDate;
    mongo::Date_t _updateDate;
    std::int32_t _status;
    std::int64_t _version{0};
    mongo::BSONObj _masterKey;
    bool _has_id : 1;
    bool _hasKeyMaterial : 1;
    bool _hasCreationDate : 1;
    bool _hasUpdateDate : 1;
    bool _hasStatus : 1;
    bool _hasMasterKey : 1;
};

}  // namespace mongo
