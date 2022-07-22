/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sasl_aws_client_protocol_gen.h --output build/opt/mongo/client/sasl_aws_client_protocol_gen.cpp src/mongo/client/sasl_aws_client_protocol.idl
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
 * Security Credentials from EC2 Instance Metadata
 */
class Ec2SecurityCredentials {
public:
    static constexpr auto kAccessKeyIdFieldName = "AccessKeyId"_sd;
    static constexpr auto kCodeFieldName = "Code"_sd;
    static constexpr auto kExpirationFieldName = "Expiration"_sd;
    static constexpr auto kLastUpdatedFieldName = "LastUpdated"_sd;
    static constexpr auto kSecretAccessKeyFieldName = "SecretAccessKey"_sd;
    static constexpr auto kTokenFieldName = "Token"_sd;
    static constexpr auto kTypeFieldName = "Type"_sd;

    Ec2SecurityCredentials();
    Ec2SecurityCredentials(std::string code, std::string lastUpdated, std::string type, std::string accessKeyId, std::string secretAccessKey, std::string token, std::string expiration);

    static Ec2SecurityCredentials parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getCode() const& { return _code; }
    void getCode() && = delete;
    void setCode(StringData value) & { auto _tmpValue = value.toString();  _code = std::move(_tmpValue); _hasCode = true; }

    const StringData getLastUpdated() const& { return _lastUpdated; }
    void getLastUpdated() && = delete;
    void setLastUpdated(StringData value) & { auto _tmpValue = value.toString();  _lastUpdated = std::move(_tmpValue); _hasLastUpdated = true; }

    const StringData getType() const& { return _type; }
    void getType() && = delete;
    void setType(StringData value) & { auto _tmpValue = value.toString();  _type = std::move(_tmpValue); _hasType = true; }

    const StringData getAccessKeyId() const& { return _accessKeyId; }
    void getAccessKeyId() && = delete;
    void setAccessKeyId(StringData value) & { auto _tmpValue = value.toString();  _accessKeyId = std::move(_tmpValue); _hasAccessKeyId = true; }

    const StringData getSecretAccessKey() const& { return _secretAccessKey; }
    void getSecretAccessKey() && = delete;
    void setSecretAccessKey(StringData value) & { auto _tmpValue = value.toString();  _secretAccessKey = std::move(_tmpValue); _hasSecretAccessKey = true; }

    const StringData getToken() const& { return _token; }
    void getToken() && = delete;
    void setToken(StringData value) & { auto _tmpValue = value.toString();  _token = std::move(_tmpValue); _hasToken = true; }

    const StringData getExpiration() const& { return _expiration; }
    void getExpiration() && = delete;
    void setExpiration(StringData value) & { auto _tmpValue = value.toString();  _expiration = std::move(_tmpValue); _hasExpiration = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _code;
    std::string _lastUpdated;
    std::string _type;
    std::string _accessKeyId;
    std::string _secretAccessKey;
    std::string _token;
    std::string _expiration;
    bool _hasCode : 1;
    bool _hasLastUpdated : 1;
    bool _hasType : 1;
    bool _hasAccessKeyId : 1;
    bool _hasSecretAccessKey : 1;
    bool _hasToken : 1;
    bool _hasExpiration : 1;
};

/**
 * Security Credentials from ECS Task
 */
class EcsTaskSecurityCredentials {
public:
    static constexpr auto kAccessKeyIdFieldName = "AccessKeyId"_sd;
    static constexpr auto kExpirationFieldName = "Expiration"_sd;
    static constexpr auto kRoleArnFieldName = "RoleArn"_sd;
    static constexpr auto kSecretAccessKeyFieldName = "SecretAccessKey"_sd;
    static constexpr auto kTokenFieldName = "Token"_sd;

    EcsTaskSecurityCredentials();
    EcsTaskSecurityCredentials(std::string accessKeyId, std::string expiration, std::string roleArn, std::string secretAccessKey, std::string token);

    static EcsTaskSecurityCredentials parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getAccessKeyId() const& { return _accessKeyId; }
    void getAccessKeyId() && = delete;
    void setAccessKeyId(StringData value) & { auto _tmpValue = value.toString();  _accessKeyId = std::move(_tmpValue); _hasAccessKeyId = true; }

    const StringData getExpiration() const& { return _expiration; }
    void getExpiration() && = delete;
    void setExpiration(StringData value) & { auto _tmpValue = value.toString();  _expiration = std::move(_tmpValue); _hasExpiration = true; }

    const StringData getRoleArn() const& { return _roleArn; }
    void getRoleArn() && = delete;
    void setRoleArn(StringData value) & { auto _tmpValue = value.toString();  _roleArn = std::move(_tmpValue); _hasRoleArn = true; }

    const StringData getSecretAccessKey() const& { return _secretAccessKey; }
    void getSecretAccessKey() && = delete;
    void setSecretAccessKey(StringData value) & { auto _tmpValue = value.toString();  _secretAccessKey = std::move(_tmpValue); _hasSecretAccessKey = true; }

    const StringData getToken() const& { return _token; }
    void getToken() && = delete;
    void setToken(StringData value) & { auto _tmpValue = value.toString();  _token = std::move(_tmpValue); _hasToken = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _accessKeyId;
    std::string _expiration;
    std::string _roleArn;
    std::string _secretAccessKey;
    std::string _token;
    bool _hasAccessKeyId : 1;
    bool _hasExpiration : 1;
    bool _hasRoleArn : 1;
    bool _hasSecretAccessKey : 1;
    bool _hasToken : 1;
};

}  // namespace mongo
