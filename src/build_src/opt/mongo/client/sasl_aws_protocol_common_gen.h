/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sasl_aws_protocol_common_gen.h --output build/opt/mongo/client/sasl_aws_protocol_common_gen.cpp src/mongo/client/sasl_aws_protocol_common.idl
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
namespace awsIam {

/**
 * SASL AWS Client First message
 */
class AwsClientFirst {
public:
    static constexpr auto kGs2_cb_flagFieldName = "p"_sd;
    static constexpr auto kNonceFieldName = "r"_sd;

    AwsClientFirst();
    AwsClientFirst(std::vector<std::uint8_t> nonce, std::int32_t gs2_cb_flag);

    static AwsClientFirst parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const ConstDataRange getNonce() const& { return ConstDataRange(_nonce); }
    void getNonce() && = delete;
    void setNonce(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _nonce = std::move(_tmpValue); _hasNonce = true; }

    std::int32_t getGs2_cb_flag() const { return _gs2_cb_flag; }
    void setGs2_cb_flag(std::int32_t value) & {  _gs2_cb_flag = std::move(value); _hasGs2_cb_flag = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<std::uint8_t> _nonce;
    std::int32_t _gs2_cb_flag;
    bool _hasNonce : 1;
    bool _hasGs2_cb_flag : 1;
};

/**
 * SASL AWS Server First message
 */
class AwsServerFirst {
public:
    static constexpr auto kServerNonceFieldName = "s"_sd;
    static constexpr auto kStsHostFieldName = "h"_sd;

    AwsServerFirst();
    AwsServerFirst(std::vector<std::uint8_t> serverNonce, std::string stsHost);

    static AwsServerFirst parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const ConstDataRange getServerNonce() const& { return ConstDataRange(_serverNonce); }
    void getServerNonce() && = delete;
    void setServerNonce(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _serverNonce = std::move(_tmpValue); _hasServerNonce = true; }

    const StringData getStsHost() const& { return _stsHost; }
    void getStsHost() && = delete;
    void setStsHost(StringData value) & { auto _tmpValue = value.toString();  _stsHost = std::move(_tmpValue); _hasStsHost = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<std::uint8_t> _serverNonce;
    std::string _stsHost;
    bool _hasServerNonce : 1;
    bool _hasStsHost : 1;
};

/**
 * SASL AWS Client Second message
 */
class AwsClientSecond {
public:
    static constexpr auto kAuthHeaderFieldName = "a"_sd;
    static constexpr auto kChannelBindingDataFieldName = "c"_sd;
    static constexpr auto kChannelBindingPrefixFieldName = "p"_sd;
    static constexpr auto kObjectFieldName = "o"_sd;
    static constexpr auto kXAmzDateFieldName = "d"_sd;
    static constexpr auto kXAmzSecurityTokenFieldName = "t"_sd;

    AwsClientSecond();
    AwsClientSecond(std::string authHeader, std::string xAmzDate);

    static AwsClientSecond parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getAuthHeader() const& { return _authHeader; }
    void getAuthHeader() && = delete;
    void setAuthHeader(StringData value) & { auto _tmpValue = value.toString();  _authHeader = std::move(_tmpValue); _hasAuthHeader = true; }

    const StringData getXAmzDate() const& { return _xAmzDate; }
    void getXAmzDate() && = delete;
    void setXAmzDate(StringData value) & { auto _tmpValue = value.toString();  _xAmzDate = std::move(_tmpValue); _hasXAmzDate = true; }

    const boost::optional<StringData> getXAmzSecurityToken() const& { return boost::optional<StringData>{_xAmzSecurityToken}; }
    void getXAmzSecurityToken() && = delete;
    void setXAmzSecurityToken(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _xAmzSecurityToken = std::move(_tmpValue);
    } else {
        _xAmzSecurityToken = boost::none;
    }
      }

    const boost::optional<StringData> getChannelBindingPrefix() const& { return boost::optional<StringData>{_channelBindingPrefix}; }
    void getChannelBindingPrefix() && = delete;
    void setChannelBindingPrefix(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _channelBindingPrefix = std::move(_tmpValue);
    } else {
        _channelBindingPrefix = boost::none;
    }
      }

    const boost::optional<StringData> getChannelBindingData() const& { return boost::optional<StringData>{_channelBindingData}; }
    void getChannelBindingData() && = delete;
    void setChannelBindingData(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _channelBindingData = std::move(_tmpValue);
    } else {
        _channelBindingData = boost::none;
    }
      }

    const boost::optional<mongo::BSONObj>& getObject() const& { return _object; }
    void getObject() && = delete;
    void setObject(boost::optional<mongo::BSONObj> value) & {  _object = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _authHeader;
    std::string _xAmzDate;
    boost::optional<std::string> _xAmzSecurityToken;
    boost::optional<std::string> _channelBindingPrefix;
    boost::optional<std::string> _channelBindingData;
    boost::optional<mongo::BSONObj> _object;
    bool _hasAuthHeader : 1;
    bool _hasXAmzDate : 1;
};

}  // namespace awsIam
}  // namespace mongo
