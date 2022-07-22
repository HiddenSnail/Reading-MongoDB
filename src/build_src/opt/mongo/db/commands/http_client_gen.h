/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/http_client_gen.h --output build/opt/mongo/db/commands/http_client_gen.cpp src/mongo/db/commands/http_client.idl
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
 * Result of an HTTP(S) GET request
 */
class HttpClientReply {
public:
    static constexpr auto kBodyFieldName = "body"_sd;
    static constexpr auto kCodeFieldName = "code"_sd;
    static constexpr auto kHeaderFieldName = "header"_sd;

    HttpClientReply();
    HttpClientReply(std::int32_t code, std::string header, std::string body);

    static HttpClientReply parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Status code returned by server
     */
    std::int32_t getCode() const { return _code; }
    void setCode(std::int32_t value) & {  _code = std::move(value); _hasCode = true; }

    /**
     * Headers returned by server
     */
    const StringData getHeader() const& { return _header; }
    void getHeader() && = delete;
    void setHeader(StringData value) & { auto _tmpValue = value.toString();  _header = std::move(_tmpValue); _hasHeader = true; }

    /**
     * Content returned by server
     */
    const StringData getBody() const& { return _body; }
    void getBody() && = delete;
    void setBody(StringData value) & { auto _tmpValue = value.toString();  _body = std::move(_tmpValue); _hasBody = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int32_t _code;
    std::string _header;
    std::string _body;
    bool _hasCode : 1;
    bool _hasHeader : 1;
    bool _hasBody : 1;
};

/**
 * Fetch an HTTP(S) resource via GET
 */
class HttpClientRequest {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kTimeoutSecsFieldName = "timeoutSecs"_sd;
    static constexpr auto kUriFieldName = "uri"_sd;
    static constexpr auto kCommandName = "httpClientRequest"_sd;

    HttpClientRequest();
    HttpClientRequest(std::string uri);

    static HttpClientRequest parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static HttpClientRequest parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * Document location to fetch
     */
    const StringData getUri() const& { return _uri; }
    void getUri() && = delete;
    void setUri(StringData value) & { auto _tmpValue = value.toString();  _uri = std::move(_tmpValue); _hasUri = true; }

    /**
     * Maximum amount of time to wait for response (seconds)
     */
    const boost::optional<std::int32_t> getTimeoutSecs() const& { return _timeoutSecs; }
    void getTimeoutSecs() && = delete;
    void setTimeoutSecs(boost::optional<std::int32_t> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get();
        validateTimeoutSecs(_tmpValue);
        _timeoutSecs = std::move(_tmpValue);
    } else {
        _timeoutSecs = boost::none;
    }
      }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    void validateTimeoutSecs(const std::int32_t value);
    void validateTimeoutSecs(IDLParserErrorContext& ctxt, const std::int32_t value);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::string _uri;
    boost::optional<std::int32_t> _timeoutSecs;
    std::string _dbName;
    bool _hasUri : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
