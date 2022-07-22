/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/http_client_gen.h --output build/opt/mongo/db/commands/http_client_gen.cpp src/mongo/db/commands/http_client.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/http_client_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData HttpClientReply::kBodyFieldName;
constexpr StringData HttpClientReply::kCodeFieldName;
constexpr StringData HttpClientReply::kHeaderFieldName;


HttpClientReply::HttpClientReply() : _code(-1), _hasCode(false), _hasHeader(false), _hasBody(false) {
    // Used for initialization only
}
HttpClientReply::HttpClientReply(std::int32_t code, std::string header, std::string body) : _code(std::move(code)), _header(std::move(header)), _body(std::move(body)), _hasCode(true), _hasHeader(true), _hasBody(true) {
    // Used for initialization only
}


HttpClientReply HttpClientReply::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    HttpClientReply object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void HttpClientReply::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kCodeBit = 0;
    const size_t kHeaderBit = 1;
    const size_t kBodyBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kCodeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kCodeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCodeBit);

                _hasCode = true;
                _code = element._numberInt();
            }
        }
        else if (fieldName == kHeaderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kHeaderBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kHeaderBit);

                _hasHeader = true;
                _header = element.str();
            }
        }
        else if (fieldName == kBodyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kBodyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBodyBit);

                _hasBody = true;
                _body = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kCodeBit]) {
            ctxt.throwMissingField(kCodeFieldName);
        }
        if (!usedFields[kHeaderBit]) {
            ctxt.throwMissingField(kHeaderFieldName);
        }
        if (!usedFields[kBodyBit]) {
            ctxt.throwMissingField(kBodyFieldName);
        }
    }

}


void HttpClientReply::serialize(BSONObjBuilder* builder) const {
    invariant(_hasCode && _hasHeader && _hasBody);

    builder->append(kCodeFieldName, _code);

    builder->append(kHeaderFieldName, _header);

    builder->append(kBodyFieldName, _body);

}


BSONObj HttpClientReply::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData HttpClientRequest::kDbNameFieldName;
constexpr StringData HttpClientRequest::kTimeoutSecsFieldName;
constexpr StringData HttpClientRequest::kUriFieldName;
constexpr StringData HttpClientRequest::kCommandName;

const std::vector<StringData> HttpClientRequest::_knownBSONFields {
    HttpClientRequest::kTimeoutSecsFieldName,
    HttpClientRequest::kUriFieldName,
    HttpClientRequest::kCommandName,
};
const std::vector<StringData> HttpClientRequest::_knownOP_MSGFields {
    HttpClientRequest::kDbNameFieldName,
    HttpClientRequest::kTimeoutSecsFieldName,
    HttpClientRequest::kUriFieldName,
    HttpClientRequest::kCommandName,
};

HttpClientRequest::HttpClientRequest() : _hasUri(false), _hasDbName(false) {
    // Used for initialization only
}
HttpClientRequest::HttpClientRequest(std::string uri) : _uri(std::move(uri)), _hasUri(true), _hasDbName(true) {
    // Used for initialization only
}

void HttpClientRequest::validateTimeoutSecs(IDLParserErrorContext& ctxt, const std::int32_t value)
{
    if (!(value > 0)) {
        throwComparisonError<std::int32_t>(ctxt, "timeoutSecs", ">"_sd, value, 0);
    }
}

void HttpClientRequest::validateTimeoutSecs(const std::int32_t value)
{
    if (!(value > 0)) {
        throwComparisonError<std::int32_t>("timeoutSecs", ">"_sd, value, 0);
    }
}


HttpClientRequest HttpClientRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    HttpClientRequest object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void HttpClientRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kUriBit = 0;
    const size_t kTimeoutSecsBit = 1;
    const size_t kDbNameBit = 2;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kUriFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kUriBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUriBit);

                _hasUri = true;
                _uri = element.str();
            }
        }
        else if (fieldName == kTimeoutSecsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kTimeoutSecsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTimeoutSecsBit);

                {
                    auto value = element._numberInt();
                    validateTimeoutSecs(value);
                    _timeoutSecs = std::move(value);
                }
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kUriBit]) {
            ctxt.throwMissingField(kUriFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

HttpClientRequest HttpClientRequest::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    HttpClientRequest object;
    object.parseProtected(ctxt, request);
    return object;
}
void HttpClientRequest::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<3> usedFields;
    const size_t kUriBit = 0;
    const size_t kTimeoutSecsBit = 1;
    const size_t kDbNameBit = 2;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kUriFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kUriBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUriBit);

                _hasUri = true;
                _uri = element.str();
            }
        }
        else if (fieldName == kTimeoutSecsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kTimeoutSecsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTimeoutSecsBit);

                {
                    auto value = element._numberInt();
                    validateTimeoutSecs(value);
                    _timeoutSecs = std::move(value);
                }
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kUriBit]) {
            ctxt.throwMissingField(kUriFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void HttpClientRequest::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasUri && _hasDbName);

    builder->append("httpClientRequest"_sd, 1);
    builder->append(kUriFieldName, _uri);

    if (_timeoutSecs.is_initialized()) {
        builder->append(kTimeoutSecsFieldName, _timeoutSecs.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest HttpClientRequest::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasUri && _hasDbName);

        builder->append("httpClientRequest"_sd, 1);
        builder->append(kUriFieldName, _uri);

        if (_timeoutSecs.is_initialized()) {
            builder->append(kTimeoutSecsFieldName, _timeoutSecs.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj HttpClientRequest::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
