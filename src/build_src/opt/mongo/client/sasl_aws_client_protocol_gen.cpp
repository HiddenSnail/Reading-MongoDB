/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sasl_aws_client_protocol_gen.h --output build/opt/mongo/client/sasl_aws_client_protocol_gen.cpp src/mongo/client/sasl_aws_client_protocol.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/client/sasl_aws_client_protocol_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData Ec2SecurityCredentials::kAccessKeyIdFieldName;
constexpr StringData Ec2SecurityCredentials::kCodeFieldName;
constexpr StringData Ec2SecurityCredentials::kExpirationFieldName;
constexpr StringData Ec2SecurityCredentials::kLastUpdatedFieldName;
constexpr StringData Ec2SecurityCredentials::kSecretAccessKeyFieldName;
constexpr StringData Ec2SecurityCredentials::kTokenFieldName;
constexpr StringData Ec2SecurityCredentials::kTypeFieldName;


Ec2SecurityCredentials::Ec2SecurityCredentials() : _hasCode(false), _hasLastUpdated(false), _hasType(false), _hasAccessKeyId(false), _hasSecretAccessKey(false), _hasToken(false), _hasExpiration(false) {
    // Used for initialization only
}
Ec2SecurityCredentials::Ec2SecurityCredentials(std::string code, std::string lastUpdated, std::string type, std::string accessKeyId, std::string secretAccessKey, std::string token, std::string expiration) : _code(std::move(code)), _lastUpdated(std::move(lastUpdated)), _type(std::move(type)), _accessKeyId(std::move(accessKeyId)), _secretAccessKey(std::move(secretAccessKey)), _token(std::move(token)), _expiration(std::move(expiration)), _hasCode(true), _hasLastUpdated(true), _hasType(true), _hasAccessKeyId(true), _hasSecretAccessKey(true), _hasToken(true), _hasExpiration(true) {
    // Used for initialization only
}


Ec2SecurityCredentials Ec2SecurityCredentials::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    Ec2SecurityCredentials object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void Ec2SecurityCredentials::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kCodeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasCode = true;
                _code = element.str();
            }
        }
        else if (fieldName == kLastUpdatedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasLastUpdated = true;
                _lastUpdated = element.str();
            }
        }
        else if (fieldName == kTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasType = true;
                _type = element.str();
            }
        }
        else if (fieldName == kAccessKeyIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasAccessKeyId = true;
                _accessKeyId = element.str();
            }
        }
        else if (fieldName == kSecretAccessKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasSecretAccessKey = true;
                _secretAccessKey = element.str();
            }
        }
        else if (fieldName == kTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasToken = true;
                _token = element.str();
            }
        }
        else if (fieldName == kExpirationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasExpiration = true;
                _expiration = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kCodeFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCodeFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kLastUpdatedFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kLastUpdatedFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kTypeFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTypeFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kAccessKeyIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kAccessKeyIdFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kSecretAccessKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kSecretAccessKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kTokenFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTokenFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kExpirationFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kExpirationFieldName);
    }

}


void Ec2SecurityCredentials::serialize(BSONObjBuilder* builder) const {
    invariant(_hasCode && _hasLastUpdated && _hasType && _hasAccessKeyId && _hasSecretAccessKey && _hasToken && _hasExpiration);

    builder->append(kCodeFieldName, _code);

    builder->append(kLastUpdatedFieldName, _lastUpdated);

    builder->append(kTypeFieldName, _type);

    builder->append(kAccessKeyIdFieldName, _accessKeyId);

    builder->append(kSecretAccessKeyFieldName, _secretAccessKey);

    builder->append(kTokenFieldName, _token);

    builder->append(kExpirationFieldName, _expiration);

}


BSONObj Ec2SecurityCredentials::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData EcsTaskSecurityCredentials::kAccessKeyIdFieldName;
constexpr StringData EcsTaskSecurityCredentials::kExpirationFieldName;
constexpr StringData EcsTaskSecurityCredentials::kRoleArnFieldName;
constexpr StringData EcsTaskSecurityCredentials::kSecretAccessKeyFieldName;
constexpr StringData EcsTaskSecurityCredentials::kTokenFieldName;


EcsTaskSecurityCredentials::EcsTaskSecurityCredentials() : _hasAccessKeyId(false), _hasExpiration(false), _hasRoleArn(false), _hasSecretAccessKey(false), _hasToken(false) {
    // Used for initialization only
}
EcsTaskSecurityCredentials::EcsTaskSecurityCredentials(std::string accessKeyId, std::string expiration, std::string roleArn, std::string secretAccessKey, std::string token) : _accessKeyId(std::move(accessKeyId)), _expiration(std::move(expiration)), _roleArn(std::move(roleArn)), _secretAccessKey(std::move(secretAccessKey)), _token(std::move(token)), _hasAccessKeyId(true), _hasExpiration(true), _hasRoleArn(true), _hasSecretAccessKey(true), _hasToken(true) {
    // Used for initialization only
}


EcsTaskSecurityCredentials EcsTaskSecurityCredentials::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    EcsTaskSecurityCredentials object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void EcsTaskSecurityCredentials::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kAccessKeyIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasAccessKeyId = true;
                _accessKeyId = element.str();
            }
        }
        else if (fieldName == kExpirationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasExpiration = true;
                _expiration = element.str();
            }
        }
        else if (fieldName == kRoleArnFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasRoleArn = true;
                _roleArn = element.str();
            }
        }
        else if (fieldName == kSecretAccessKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasSecretAccessKey = true;
                _secretAccessKey = element.str();
            }
        }
        else if (fieldName == kTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasToken = true;
                _token = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kAccessKeyIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kAccessKeyIdFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kExpirationFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kExpirationFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kRoleArnFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kRoleArnFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kSecretAccessKeyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kSecretAccessKeyFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kTokenFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTokenFieldName);
    }

}


void EcsTaskSecurityCredentials::serialize(BSONObjBuilder* builder) const {
    invariant(_hasAccessKeyId && _hasExpiration && _hasRoleArn && _hasSecretAccessKey && _hasToken);

    builder->append(kAccessKeyIdFieldName, _accessKeyId);

    builder->append(kExpirationFieldName, _expiration);

    builder->append(kRoleArnFieldName, _roleArn);

    builder->append(kSecretAccessKeyFieldName, _secretAccessKey);

    builder->append(kTokenFieldName, _token);

}


BSONObj EcsTaskSecurityCredentials::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
