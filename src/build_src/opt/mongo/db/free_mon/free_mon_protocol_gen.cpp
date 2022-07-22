/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_protocol_gen.h --output build/opt/mongo/db/free_mon/free_mon_protocol_gen.cpp src/mongo/db/free_mon/free_mon_protocol.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/free_mon/free_mon_protocol_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Metrics Encoding Methods
 */
namespace  {
constexpr StringData kMetricsEncoding_snappy = "snappy"_sd;
}  // namespace 

MetricsEncodingEnum MetricsEncoding_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kMetricsEncoding_snappy) {
        return MetricsEncodingEnum::snappy;
    }
    ctxt.throwBadEnumValue(value);
}

StringData MetricsEncoding_serializer(MetricsEncodingEnum value) {
    if (value == MetricsEncodingEnum::snappy) {
        return kMetricsEncoding_snappy;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData FreeMonRegistrationRequest::kIdFieldName;
constexpr StringData FreeMonRegistrationRequest::kLocalTimeFieldName;
constexpr StringData FreeMonRegistrationRequest::kPayloadFieldName;
constexpr StringData FreeMonRegistrationRequest::kTagsFieldName;
constexpr StringData FreeMonRegistrationRequest::kVersionFieldName;


FreeMonRegistrationRequest::FreeMonRegistrationRequest() : _version(-1), _hasVersion(false), _hasPayload(false), _hasLocalTime(false) {
    // Used for initialization only
}
FreeMonRegistrationRequest::FreeMonRegistrationRequest(std::int64_t version, mongo::BSONObj payload, mongo::Date_t localTime) : _version(std::move(version)), _payload(std::move(payload)), _localTime(std::move(localTime)), _hasVersion(true), _hasPayload(true), _hasLocalTime(true) {
    // Used for initialization only
}


FreeMonRegistrationRequest FreeMonRegistrationRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    FreeMonRegistrationRequest object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void FreeMonRegistrationRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kVersionBit = 0;
    const size_t kPayloadBit = 1;
    const size_t kIdBit = 2;
    const size_t kLocalTimeBit = 3;
    const size_t kTagsBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                _hasVersion = true;
                _version = element._numberLong();
            }
        }
        else if (fieldName == kPayloadFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPayloadBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPayloadBit);

                _hasPayload = true;
                _payload = element.Obj();
            }
        }
        else if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdBit);

                _id = element.str();
            }
        }
        else if (fieldName == kLocalTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kLocalTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLocalTimeBit);

                _hasLocalTime = true;
                _localTime = element.date();
            }
        }
        else if (fieldName == kTagsFieldName) {
            if (MONGO_unlikely(usedFields[kTagsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kTagsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kTagsFieldName, &ctxt);
            std::vector<std::string> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, String)) {
                        values.emplace_back(arrayElement.str());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _tags = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kVersionBit]) {
            ctxt.throwMissingField(kVersionFieldName);
        }
        if (!usedFields[kPayloadBit]) {
            ctxt.throwMissingField(kPayloadFieldName);
        }
        if (!usedFields[kLocalTimeBit]) {
            ctxt.throwMissingField(kLocalTimeFieldName);
        }
    }

}


void FreeMonRegistrationRequest::serialize(BSONObjBuilder* builder) const {
    invariant(_hasVersion && _hasPayload && _hasLocalTime);

    builder->append(kVersionFieldName, _version);

    builder->append(kPayloadFieldName, _payload);

    if (_id.is_initialized()) {
        builder->append(kIdFieldName, _id.get());
    }

    builder->append(kLocalTimeFieldName, _localTime);

    if (_tags.is_initialized()) {
        builder->append(kTagsFieldName, _tags.get());
    }

}


BSONObj FreeMonRegistrationRequest::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData FreeMonRegistrationResponse::kHaltMetricsUploadingFieldName;
constexpr StringData FreeMonRegistrationResponse::kIdFieldName;
constexpr StringData FreeMonRegistrationResponse::kInformationalURLFieldName;
constexpr StringData FreeMonRegistrationResponse::kMessageFieldName;
constexpr StringData FreeMonRegistrationResponse::kReportingIntervalFieldName;
constexpr StringData FreeMonRegistrationResponse::kUserReminderFieldName;
constexpr StringData FreeMonRegistrationResponse::kVersionFieldName;


FreeMonRegistrationResponse::FreeMonRegistrationResponse() : _version(-1), _haltMetricsUploading(false), _reportingInterval(-1), _hasVersion(false), _hasHaltMetricsUploading(false), _hasId(false), _hasInformationalURL(false), _hasMessage(false), _hasReportingInterval(false) {
    // Used for initialization only
}
FreeMonRegistrationResponse::FreeMonRegistrationResponse(std::int64_t version, bool haltMetricsUploading, std::string id, std::string informationalURL, std::string message, std::int64_t reportingInterval) : _version(std::move(version)), _haltMetricsUploading(std::move(haltMetricsUploading)), _id(std::move(id)), _informationalURL(std::move(informationalURL)), _message(std::move(message)), _reportingInterval(std::move(reportingInterval)), _hasVersion(true), _hasHaltMetricsUploading(true), _hasId(true), _hasInformationalURL(true), _hasMessage(true), _hasReportingInterval(true) {
    // Used for initialization only
}


FreeMonRegistrationResponse FreeMonRegistrationResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    FreeMonRegistrationResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void FreeMonRegistrationResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<7> usedFields;
    const size_t kVersionBit = 0;
    const size_t kHaltMetricsUploadingBit = 1;
    const size_t kIdBit = 2;
    const size_t kInformationalURLBit = 3;
    const size_t kMessageBit = 4;
    const size_t kReportingIntervalBit = 5;
    const size_t kUserReminderBit = 6;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                _hasVersion = true;
                _version = element._numberLong();
            }
        }
        else if (fieldName == kHaltMetricsUploadingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kHaltMetricsUploadingBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kHaltMetricsUploadingBit);

                _hasHaltMetricsUploading = true;
                _haltMetricsUploading = element.boolean();
            }
        }
        else if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdBit);

                _hasId = true;
                _id = element.str();
            }
        }
        else if (fieldName == kInformationalURLFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kInformationalURLBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kInformationalURLBit);

                _hasInformationalURL = true;
                _informationalURL = element.str();
            }
        }
        else if (fieldName == kMessageFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kMessageBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMessageBit);

                _hasMessage = true;
                _message = element.str();
            }
        }
        else if (fieldName == kReportingIntervalFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kReportingIntervalBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kReportingIntervalBit);

                _hasReportingInterval = true;
                _reportingInterval = element._numberLong();
            }
        }
        else if (fieldName == kUserReminderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kUserReminderBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUserReminderBit);

                _userReminder = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kVersionBit]) {
            ctxt.throwMissingField(kVersionFieldName);
        }
        if (!usedFields[kHaltMetricsUploadingBit]) {
            ctxt.throwMissingField(kHaltMetricsUploadingFieldName);
        }
        if (!usedFields[kIdBit]) {
            ctxt.throwMissingField(kIdFieldName);
        }
        if (!usedFields[kInformationalURLBit]) {
            ctxt.throwMissingField(kInformationalURLFieldName);
        }
        if (!usedFields[kMessageBit]) {
            ctxt.throwMissingField(kMessageFieldName);
        }
        if (!usedFields[kReportingIntervalBit]) {
            ctxt.throwMissingField(kReportingIntervalFieldName);
        }
    }

}


void FreeMonRegistrationResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasVersion && _hasHaltMetricsUploading && _hasId && _hasInformationalURL && _hasMessage && _hasReportingInterval);

    builder->append(kVersionFieldName, _version);

    builder->append(kHaltMetricsUploadingFieldName, _haltMetricsUploading);

    builder->append(kIdFieldName, _id);

    builder->append(kInformationalURLFieldName, _informationalURL);

    builder->append(kMessageFieldName, _message);

    builder->append(kReportingIntervalFieldName, _reportingInterval);

    if (_userReminder.is_initialized()) {
        builder->append(kUserReminderFieldName, _userReminder.get());
    }

}


BSONObj FreeMonRegistrationResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData FreeMonMetricsRequest::kEncodingFieldName;
constexpr StringData FreeMonMetricsRequest::kIdFieldName;
constexpr StringData FreeMonMetricsRequest::kLocalTimeFieldName;
constexpr StringData FreeMonMetricsRequest::kMetricsFieldName;
constexpr StringData FreeMonMetricsRequest::kVersionFieldName;


FreeMonMetricsRequest::FreeMonMetricsRequest() : _version(-1), _hasVersion(false), _hasId(false), _hasLocalTime(false), _hasEncoding(false), _hasMetrics(false) {
    // Used for initialization only
}
FreeMonMetricsRequest::FreeMonMetricsRequest(std::int64_t version, std::string id, mongo::Date_t localTime, mongo::MetricsEncodingEnum encoding, std::vector<std::uint8_t> metrics) : _version(std::move(version)), _id(std::move(id)), _localTime(std::move(localTime)), _encoding(std::move(encoding)), _metrics(std::move(metrics)), _hasVersion(true), _hasId(true), _hasLocalTime(true), _hasEncoding(true), _hasMetrics(true) {
    // Used for initialization only
}


FreeMonMetricsRequest FreeMonMetricsRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    FreeMonMetricsRequest object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void FreeMonMetricsRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kVersionBit = 0;
    const size_t kIdBit = 1;
    const size_t kLocalTimeBit = 2;
    const size_t kEncodingBit = 3;
    const size_t kMetricsBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                _hasVersion = true;
                _version = element._numberLong();
            }
        }
        else if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdBit);

                _hasId = true;
                _id = element.str();
            }
        }
        else if (fieldName == kLocalTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kLocalTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLocalTimeBit);

                _hasLocalTime = true;
                _localTime = element.date();
            }
        }
        else if (fieldName == kEncodingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kEncodingBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kEncodingBit);

                _hasEncoding = true;
                IDLParserErrorContext tempContext(kEncodingFieldName, &ctxt);
                _encoding = MetricsEncoding_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kMetricsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, BinDataGeneral))) {
                if (MONGO_unlikely(usedFields[kMetricsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMetricsBit);

                _hasMetrics = true;
                _metrics = element._binDataVector();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kVersionBit]) {
            ctxt.throwMissingField(kVersionFieldName);
        }
        if (!usedFields[kIdBit]) {
            ctxt.throwMissingField(kIdFieldName);
        }
        if (!usedFields[kLocalTimeBit]) {
            ctxt.throwMissingField(kLocalTimeFieldName);
        }
        if (!usedFields[kEncodingBit]) {
            ctxt.throwMissingField(kEncodingFieldName);
        }
        if (!usedFields[kMetricsBit]) {
            ctxt.throwMissingField(kMetricsFieldName);
        }
    }

}


void FreeMonMetricsRequest::serialize(BSONObjBuilder* builder) const {
    invariant(_hasVersion && _hasId && _hasLocalTime && _hasEncoding && _hasMetrics);

    builder->append(kVersionFieldName, _version);

    builder->append(kIdFieldName, _id);

    builder->append(kLocalTimeFieldName, _localTime);

    {
        builder->append(kEncodingFieldName, MetricsEncoding_serializer(_encoding));
    }

    {
        ConstDataRange tempCDR(_metrics);
        builder->append(kMetricsFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), BinDataGeneral));
    }

}


BSONObj FreeMonMetricsRequest::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData FreeMonMetricsResponse::kHaltMetricsUploadingFieldName;
constexpr StringData FreeMonMetricsResponse::kIdFieldName;
constexpr StringData FreeMonMetricsResponse::kInformationalURLFieldName;
constexpr StringData FreeMonMetricsResponse::kMessageFieldName;
constexpr StringData FreeMonMetricsResponse::kPermanentlyDeleteFieldName;
constexpr StringData FreeMonMetricsResponse::kReportingIntervalFieldName;
constexpr StringData FreeMonMetricsResponse::kResendRegistrationFieldName;
constexpr StringData FreeMonMetricsResponse::kUserReminderFieldName;
constexpr StringData FreeMonMetricsResponse::kVersionFieldName;


FreeMonMetricsResponse::FreeMonMetricsResponse() : _version(-1), _haltMetricsUploading(false), _permanentlyDelete(false), _reportingInterval(-1), _hasVersion(false), _hasHaltMetricsUploading(false), _hasPermanentlyDelete(false), _hasReportingInterval(false) {
    // Used for initialization only
}
FreeMonMetricsResponse::FreeMonMetricsResponse(std::int64_t version, bool haltMetricsUploading, bool permanentlyDelete, std::int64_t reportingInterval) : _version(std::move(version)), _haltMetricsUploading(std::move(haltMetricsUploading)), _permanentlyDelete(std::move(permanentlyDelete)), _reportingInterval(std::move(reportingInterval)), _hasVersion(true), _hasHaltMetricsUploading(true), _hasPermanentlyDelete(true), _hasReportingInterval(true) {
    // Used for initialization only
}


FreeMonMetricsResponse FreeMonMetricsResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    FreeMonMetricsResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void FreeMonMetricsResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<9> usedFields;
    const size_t kVersionBit = 0;
    const size_t kHaltMetricsUploadingBit = 1;
    const size_t kPermanentlyDeleteBit = 2;
    const size_t kReportingIntervalBit = 3;
    const size_t kIdBit = 4;
    const size_t kMessageBit = 5;
    const size_t kInformationalURLBit = 6;
    const size_t kUserReminderBit = 7;
    const size_t kResendRegistrationBit = 8;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                _hasVersion = true;
                _version = element._numberLong();
            }
        }
        else if (fieldName == kHaltMetricsUploadingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kHaltMetricsUploadingBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kHaltMetricsUploadingBit);

                _hasHaltMetricsUploading = true;
                _haltMetricsUploading = element.boolean();
            }
        }
        else if (fieldName == kPermanentlyDeleteFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kPermanentlyDeleteBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPermanentlyDeleteBit);

                _hasPermanentlyDelete = true;
                _permanentlyDelete = element.boolean();
            }
        }
        else if (fieldName == kReportingIntervalFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kReportingIntervalBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kReportingIntervalBit);

                _hasReportingInterval = true;
                _reportingInterval = element._numberLong();
            }
        }
        else if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdBit);

                _id = element.str();
            }
        }
        else if (fieldName == kMessageFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kMessageBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMessageBit);

                _message = element.str();
            }
        }
        else if (fieldName == kInformationalURLFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kInformationalURLBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kInformationalURLBit);

                _informationalURL = element.str();
            }
        }
        else if (fieldName == kUserReminderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kUserReminderBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUserReminderBit);

                _userReminder = element.str();
            }
        }
        else if (fieldName == kResendRegistrationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kResendRegistrationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kResendRegistrationBit);

                _resendRegistration = element.boolean();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kVersionBit]) {
            ctxt.throwMissingField(kVersionFieldName);
        }
        if (!usedFields[kHaltMetricsUploadingBit]) {
            ctxt.throwMissingField(kHaltMetricsUploadingFieldName);
        }
        if (!usedFields[kPermanentlyDeleteBit]) {
            ctxt.throwMissingField(kPermanentlyDeleteFieldName);
        }
        if (!usedFields[kReportingIntervalBit]) {
            ctxt.throwMissingField(kReportingIntervalFieldName);
        }
    }

}


void FreeMonMetricsResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasVersion && _hasHaltMetricsUploading && _hasPermanentlyDelete && _hasReportingInterval);

    builder->append(kVersionFieldName, _version);

    builder->append(kHaltMetricsUploadingFieldName, _haltMetricsUploading);

    builder->append(kPermanentlyDeleteFieldName, _permanentlyDelete);

    builder->append(kReportingIntervalFieldName, _reportingInterval);

    if (_id.is_initialized()) {
        builder->append(kIdFieldName, _id.get());
    }

    if (_message.is_initialized()) {
        builder->append(kMessageFieldName, _message.get());
    }

    if (_informationalURL.is_initialized()) {
        builder->append(kInformationalURLFieldName, _informationalURL.get());
    }

    if (_userReminder.is_initialized()) {
        builder->append(kUserReminderFieldName, _userReminder.get());
    }

    if (_resendRegistration.is_initialized()) {
        builder->append(kResendRegistrationFieldName, _resendRegistration.get());
    }

}


BSONObj FreeMonMetricsResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
