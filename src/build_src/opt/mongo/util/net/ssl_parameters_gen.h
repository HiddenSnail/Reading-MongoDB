/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/util/net/ssl_parameters_gen.h --output build/opt/mongo/util/net/ssl_parameters_gen.cpp src/mongo/util/net/ssl_parameters.idl
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
#include "mongo/crypto/sha256_block.h"
#include "mongo/db/auth/role_name.h"
#include "mongo/db/auth/user_name.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/net/ssl_options.h"
#include "mongo/util/net/ssl_parameters.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * 
 */
class TLSCATrust {
public:
    static constexpr auto kRolesFieldName = "roles"_sd;
    static constexpr auto kSha256FieldName = "sha256"_sd;

    TLSCATrust();
    TLSCATrust(mongo::SHA256Block sha256, std::vector<mongo::RoleName> roles);

    static TLSCATrust parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::SHA256Block& getSha256() const { return _sha256; }
    void setSha256(mongo::SHA256Block value) & {  _sha256 = std::move(value); _hasSha256 = true; }

    const std::vector<mongo::RoleName>& getRoles() const& { return _roles; }
    void getRoles() && = delete;
    void setRoles(std::vector<mongo::RoleName> value) & {  _roles = std::move(value); _hasRoles = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::SHA256Block _sha256;
    std::vector<mongo::RoleName> _roles;
    bool _hasSha256 : 1;
    bool _hasRoles : 1;
};

constexpr auto kOcspEnabledDefault = true;

extern bool tlsOCSPEnabled;
constexpr auto kOcspCacheSizeDefault = 300;

extern std::int32_t tlsOCSPCacheSize;
constexpr auto kOcspValidationRefreshPeriodSecsDefault = -1;

extern int kOCSPValidationRefreshPeriodSecs;
constexpr auto kTlsOCSPVerifyTimeoutSecsDefault = 5;

extern int gTLSOCSPVerifyTimeoutSecs;
constexpr auto kTlsOCSPStaplingTimeoutSecsDefault = -1;

extern int gTLSOCSPStaplingTimeoutSecs;
class SSLModeServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

class TLSModeServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

class ClusterMemberDNOverride : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

constexpr auto kTlsX509ExpirationWarningThresholdDaysDefault = 30;

extern std::int32_t tlsX509ExpirationWarningThresholdDays;
class TLSCATrustsSetParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

}  // namespace mongo
