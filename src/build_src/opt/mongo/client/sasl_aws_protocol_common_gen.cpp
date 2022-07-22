/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sasl_aws_protocol_common_gen.h --output build/opt/mongo/client/sasl_aws_protocol_common_gen.cpp src/mongo/client/sasl_aws_protocol_common.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/client/sasl_aws_protocol_common_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace awsIam {

constexpr StringData AwsClientFirst::kGs2_cb_flagFieldName;
constexpr StringData AwsClientFirst::kNonceFieldName;


AwsClientFirst::AwsClientFirst() : _gs2_cb_flag(-1), _hasNonce(false), _hasGs2_cb_flag(false) {
    // Used for initialization only
}
AwsClientFirst::AwsClientFirst(std::vector<std::uint8_t> nonce, std::int32_t gs2_cb_flag) : _nonce(std::move(nonce)), _gs2_cb_flag(std::move(gs2_cb_flag)), _hasNonce(true), _hasGs2_cb_flag(true) {
    // Used for initialization only
}


AwsClientFirst AwsClientFirst::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsClientFirst object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsClientFirst::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kNonceBit = 0;
    const size_t kGs2_cb_flagBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kNonceFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kNonceBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNonceBit);

                _hasNonce = true;
                _nonce = element._binDataVector();
            }
        }
        else if (fieldName == kGs2_cb_flagFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kGs2_cb_flagBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kGs2_cb_flagBit);

                _hasGs2_cb_flag = true;
                _gs2_cb_flag = element._numberInt();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kNonceBit]) {
            ctxt.throwMissingField(kNonceFieldName);
        }
        if (!usedFields[kGs2_cb_flagBit]) {
            ctxt.throwMissingField(kGs2_cb_flagFieldName);
        }
    }

}


void AwsClientFirst::serialize(BSONObjBuilder* builder) const {
    invariant(_hasNonce && _hasGs2_cb_flag);

    {
        ConstDataRange tempCDR(_nonce);
        builder->append(kNonceFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

    builder->append(kGs2_cb_flagFieldName, _gs2_cb_flag);

}


BSONObj AwsClientFirst::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsServerFirst::kServerNonceFieldName;
constexpr StringData AwsServerFirst::kStsHostFieldName;


AwsServerFirst::AwsServerFirst() : _hasServerNonce(false), _hasStsHost(false) {
    // Used for initialization only
}
AwsServerFirst::AwsServerFirst(std::vector<std::uint8_t> serverNonce, std::string stsHost) : _serverNonce(std::move(serverNonce)), _stsHost(std::move(stsHost)), _hasServerNonce(true), _hasStsHost(true) {
    // Used for initialization only
}


AwsServerFirst AwsServerFirst::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsServerFirst object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsServerFirst::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kServerNonceBit = 0;
    const size_t kStsHostBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kServerNonceFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kServerNonceBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kServerNonceBit);

                _hasServerNonce = true;
                _serverNonce = element._binDataVector();
            }
        }
        else if (fieldName == kStsHostFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kStsHostBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStsHostBit);

                _hasStsHost = true;
                _stsHost = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kServerNonceBit]) {
            ctxt.throwMissingField(kServerNonceFieldName);
        }
        if (!usedFields[kStsHostBit]) {
            ctxt.throwMissingField(kStsHostFieldName);
        }
    }

}


void AwsServerFirst::serialize(BSONObjBuilder* builder) const {
    invariant(_hasServerNonce && _hasStsHost);

    {
        ConstDataRange tempCDR(_serverNonce);
        builder->append(kServerNonceFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

    builder->append(kStsHostFieldName, _stsHost);

}


BSONObj AwsServerFirst::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AwsClientSecond::kAuthHeaderFieldName;
constexpr StringData AwsClientSecond::kChannelBindingDataFieldName;
constexpr StringData AwsClientSecond::kChannelBindingPrefixFieldName;
constexpr StringData AwsClientSecond::kObjectFieldName;
constexpr StringData AwsClientSecond::kXAmzDateFieldName;
constexpr StringData AwsClientSecond::kXAmzSecurityTokenFieldName;


AwsClientSecond::AwsClientSecond() : _hasAuthHeader(false), _hasXAmzDate(false) {
    // Used for initialization only
}
AwsClientSecond::AwsClientSecond(std::string authHeader, std::string xAmzDate) : _authHeader(std::move(authHeader)), _xAmzDate(std::move(xAmzDate)), _hasAuthHeader(true), _hasXAmzDate(true) {
    // Used for initialization only
}


AwsClientSecond AwsClientSecond::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AwsClientSecond object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AwsClientSecond::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kAuthHeaderBit = 0;
    const size_t kXAmzDateBit = 1;
    const size_t kXAmzSecurityTokenBit = 2;
    const size_t kChannelBindingPrefixBit = 3;
    const size_t kChannelBindingDataBit = 4;
    const size_t kObjectBit = 5;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAuthHeaderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kAuthHeaderBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAuthHeaderBit);

                _hasAuthHeader = true;
                _authHeader = element.str();
            }
        }
        else if (fieldName == kXAmzDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kXAmzDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kXAmzDateBit);

                _hasXAmzDate = true;
                _xAmzDate = element.str();
            }
        }
        else if (fieldName == kXAmzSecurityTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kXAmzSecurityTokenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kXAmzSecurityTokenBit);

                _xAmzSecurityToken = element.str();
            }
        }
        else if (fieldName == kChannelBindingPrefixFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kChannelBindingPrefixBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kChannelBindingPrefixBit);

                _channelBindingPrefix = element.str();
            }
        }
        else if (fieldName == kChannelBindingDataFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kChannelBindingDataBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kChannelBindingDataBit);

                _channelBindingData = element.str();
            }
        }
        else if (fieldName == kObjectFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kObjectBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kObjectBit);

                _object = element.Obj();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kAuthHeaderBit]) {
            ctxt.throwMissingField(kAuthHeaderFieldName);
        }
        if (!usedFields[kXAmzDateBit]) {
            ctxt.throwMissingField(kXAmzDateFieldName);
        }
    }

}


void AwsClientSecond::serialize(BSONObjBuilder* builder) const {
    invariant(_hasAuthHeader && _hasXAmzDate);

    builder->append(kAuthHeaderFieldName, _authHeader);

    builder->append(kXAmzDateFieldName, _xAmzDate);

    if (_xAmzSecurityToken.is_initialized()) {
        builder->append(kXAmzSecurityTokenFieldName, _xAmzSecurityToken.get());
    }

    if (_channelBindingPrefix.is_initialized()) {
        builder->append(kChannelBindingPrefixFieldName, _channelBindingPrefix.get());
    }

    if (_channelBindingData.is_initialized()) {
        builder->append(kChannelBindingDataFieldName, _channelBindingData.get());
    }

    if (_object.is_initialized()) {
        builder->append(kObjectFieldName, _object.get());
    }

}


BSONObj AwsClientSecond::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace awsIam
}  // namespace mongo
