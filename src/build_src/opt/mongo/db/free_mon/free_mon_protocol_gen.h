/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_protocol_gen.h --output build/opt/mongo/db/free_mon/free_mon_protocol_gen.cpp src/mongo/db/free_mon/free_mon_protocol.idl
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
 * Metrics Encoding Methods
 */
enum class MetricsEncodingEnum : std::int32_t {
    snappy ,
};

MetricsEncodingEnum MetricsEncoding_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData MetricsEncoding_serializer(MetricsEncodingEnum value);

/**
 * Registration Request to Cloud Server
 */
class FreeMonRegistrationRequest {
public:
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kLocalTimeFieldName = "localTime"_sd;
    static constexpr auto kPayloadFieldName = "payload"_sd;
    static constexpr auto kTagsFieldName = "tags"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    FreeMonRegistrationRequest();
    FreeMonRegistrationRequest(std::int64_t version, mongo::BSONObj payload, mongo::Date_t localTime);

    static FreeMonRegistrationRequest parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Protocol version, initial version is 1
     */
    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value); _hasVersion = true; }

    /**
     * Payload of registration information
     */
    const mongo::BSONObj& getPayload() const { return _payload; }
    void setPayload(mongo::BSONObj value) & {  _payload = std::move(value); _hasPayload = true; }

    /**
     * Existing Registration Id
     */
    const boost::optional<StringData> getId() const& { return boost::optional<StringData>{_id}; }
    void getId() && = delete;
    void setId(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _id = std::move(_tmpValue);
    } else {
        _id = boost::none;
    }
      }

    /**
     * Local time at registration send
     */
    const mongo::Date_t& getLocalTime() const { return _localTime; }
    void setLocalTime(mongo::Date_t value) & {  _localTime = std::move(value); _hasLocalTime = true; }

    /**
     * Tags
     */
    const boost::optional<std::vector<StringData>> getTags() const& { if (_tags.is_initialized()) {
        return transformVector(_tags.get());
    } else {
        return boost::none;
    }
     }
    void getTags() && = delete;
    void setTags(boost::optional<std::vector<StringData>> value) & { if (value.is_initialized()) {
        auto _tmpValue = transformVector(value.get());
        
        _tags = std::move(_tmpValue);
    } else {
        _tags = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _version;
    mongo::BSONObj _payload;
    boost::optional<std::string> _id;
    mongo::Date_t _localTime;
    boost::optional<std::vector<std::string>> _tags;
    bool _hasVersion : 1;
    bool _hasPayload : 1;
    bool _hasLocalTime : 1;
};

/**
 * Registration Response from Cloud Server
 */
class FreeMonRegistrationResponse {
public:
    static constexpr auto kHaltMetricsUploadingFieldName = "haltMetricsUploading"_sd;
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kInformationalURLFieldName = "informationalURL"_sd;
    static constexpr auto kMessageFieldName = "message"_sd;
    static constexpr auto kReportingIntervalFieldName = "reportingInterval"_sd;
    static constexpr auto kUserReminderFieldName = "userReminder"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    FreeMonRegistrationResponse();
    FreeMonRegistrationResponse(std::int64_t version, bool haltMetricsUploading, std::string id, std::string informationalURL, std::string message, std::int64_t reportingInterval);

    static FreeMonRegistrationResponse parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Protocol version, initial version is 1
     */
    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value); _hasVersion = true; }

    /**
     * True indicates it should not proceed to metrics uploading
     */
    bool getHaltMetricsUploading() const { return _haltMetricsUploading; }
    void setHaltMetricsUploading(bool value) & {  _haltMetricsUploading = std::move(value); _hasHaltMetricsUploading = true; }

    /**
     * Existing Registration Id
     */
    const StringData getId() const& { return _id; }
    void getId() && = delete;
    void setId(StringData value) & { auto _tmpValue = value.toString();  _id = std::move(_tmpValue); _hasId = true; }

    /**
     * Informational HTTP web page for metrics
     */
    const StringData getInformationalURL() const& { return _informationalURL; }
    void getInformationalURL() && = delete;
    void setInformationalURL(StringData value) & { auto _tmpValue = value.toString();  _informationalURL = std::move(_tmpValue); _hasInformationalURL = true; }

    /**
     * Informational message for shell to display to user
     */
    const StringData getMessage() const& { return _message; }
    void getMessage() && = delete;
    void setMessage(StringData value) & { auto _tmpValue = value.toString();  _message = std::move(_tmpValue); _hasMessage = true; }

    /**
     * Metrics Reporting interval in seconds
     */
    std::int64_t getReportingInterval() const { return _reportingInterval; }
    void setReportingInterval(std::int64_t value) & {  _reportingInterval = std::move(value); _hasReportingInterval = true; }

    /**
     * Informational message to display to user to remind them about the service
     */
    const boost::optional<StringData> getUserReminder() const& { return boost::optional<StringData>{_userReminder}; }
    void getUserReminder() && = delete;
    void setUserReminder(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _userReminder = std::move(_tmpValue);
    } else {
        _userReminder = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _version;
    bool _haltMetricsUploading;
    std::string _id;
    std::string _informationalURL;
    std::string _message;
    std::int64_t _reportingInterval;
    boost::optional<std::string> _userReminder;
    bool _hasVersion : 1;
    bool _hasHaltMetricsUploading : 1;
    bool _hasId : 1;
    bool _hasInformationalURL : 1;
    bool _hasMessage : 1;
    bool _hasReportingInterval : 1;
};

/**
 * Metrics Request to Cloud Server
 */
class FreeMonMetricsRequest {
public:
    static constexpr auto kEncodingFieldName = "encoding"_sd;
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kLocalTimeFieldName = "localTime"_sd;
    static constexpr auto kMetricsFieldName = "metrics"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    FreeMonMetricsRequest();
    FreeMonMetricsRequest(std::int64_t version, std::string id, mongo::Date_t localTime, mongo::MetricsEncodingEnum encoding, std::vector<std::uint8_t> metrics);

    static FreeMonMetricsRequest parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Protocol version, initial version is 1
     */
    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value); _hasVersion = true; }

    /**
     * Registration Id
     */
    const StringData getId() const& { return _id; }
    void getId() && = delete;
    void setId(StringData value) & { auto _tmpValue = value.toString();  _id = std::move(_tmpValue); _hasId = true; }

    /**
     * Local time at metrics send
     */
    const mongo::Date_t& getLocalTime() const { return _localTime; }
    void setLocalTime(mongo::Date_t value) & {  _localTime = std::move(value); _hasLocalTime = true; }

    /**
     * Compression Encoding
     */
    mongo::MetricsEncodingEnum getEncoding() const { return _encoding; }
    void setEncoding(mongo::MetricsEncodingEnum value) & {  _encoding = std::move(value); _hasEncoding = true; }

    /**
     * Metrics Blob
     */
    const ConstDataRange getMetrics() const& { return ConstDataRange(_metrics); }
    void getMetrics() && = delete;
    void setMetrics(ConstDataRange value) & { auto _tmpValue = std::vector<std::uint8_t>(reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.length());  _metrics = std::move(_tmpValue); _hasMetrics = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _version;
    std::string _id;
    mongo::Date_t _localTime;
    mongo::MetricsEncodingEnum _encoding;
    std::vector<std::uint8_t> _metrics;
    bool _hasVersion : 1;
    bool _hasId : 1;
    bool _hasLocalTime : 1;
    bool _hasEncoding : 1;
    bool _hasMetrics : 1;
};

/**
 * Metrics Response from Cloud Server
 */
class FreeMonMetricsResponse {
public:
    static constexpr auto kHaltMetricsUploadingFieldName = "haltMetricsUploading"_sd;
    static constexpr auto kIdFieldName = "id"_sd;
    static constexpr auto kInformationalURLFieldName = "informationalURL"_sd;
    static constexpr auto kMessageFieldName = "message"_sd;
    static constexpr auto kPermanentlyDeleteFieldName = "permanentlyDelete"_sd;
    static constexpr auto kReportingIntervalFieldName = "reportingInterval"_sd;
    static constexpr auto kResendRegistrationFieldName = "resendRegistration"_sd;
    static constexpr auto kUserReminderFieldName = "userReminder"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    FreeMonMetricsResponse();
    FreeMonMetricsResponse(std::int64_t version, bool haltMetricsUploading, bool permanentlyDelete, std::int64_t reportingInterval);

    static FreeMonMetricsResponse parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Protocol version, initial version is 1
     */
    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value); _hasVersion = true; }

    /**
     * True indicates it should not proceed to metrics uploading
     */
    bool getHaltMetricsUploading() const { return _haltMetricsUploading; }
    void setHaltMetricsUploading(bool value) & {  _haltMetricsUploading = std::move(value); _hasHaltMetricsUploading = true; }

    /**
     * True indicates it permanently delete the local state
     */
    bool getPermanentlyDelete() const { return _permanentlyDelete; }
    void setPermanentlyDelete(bool value) & {  _permanentlyDelete = std::move(value); _hasPermanentlyDelete = true; }

    /**
     * Metrics Reporting interval in seconds
     */
    std::int64_t getReportingInterval() const { return _reportingInterval; }
    void setReportingInterval(std::int64_t value) & {  _reportingInterval = std::move(value); _hasReportingInterval = true; }

    /**
     * Existing Registration Id
     */
    const boost::optional<StringData> getId() const& { return boost::optional<StringData>{_id}; }
    void getId() && = delete;
    void setId(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _id = std::move(_tmpValue);
    } else {
        _id = boost::none;
    }
      }

    /**
     * Informational message for shell to display to user
     */
    const boost::optional<StringData> getMessage() const& { return boost::optional<StringData>{_message}; }
    void getMessage() && = delete;
    void setMessage(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _message = std::move(_tmpValue);
    } else {
        _message = boost::none;
    }
      }

    /**
     * Informational HTTP web page for metrics
     */
    const boost::optional<StringData> getInformationalURL() const& { return boost::optional<StringData>{_informationalURL}; }
    void getInformationalURL() && = delete;
    void setInformationalURL(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _informationalURL = std::move(_tmpValue);
    } else {
        _informationalURL = boost::none;
    }
      }

    /**
     * Message to display to user to remind them about service
     */
    const boost::optional<StringData> getUserReminder() const& { return boost::optional<StringData>{_userReminder}; }
    void getUserReminder() && = delete;
    void setUserReminder(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _userReminder = std::move(_tmpValue);
    } else {
        _userReminder = boost::none;
    }
      }

    /**
     * If true, resend registration to server
     */
    const boost::optional<bool> getResendRegistration() const& { return _resendRegistration; }
    void getResendRegistration() && = delete;
    void setResendRegistration(boost::optional<bool> value) & {  _resendRegistration = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _version;
    bool _haltMetricsUploading;
    bool _permanentlyDelete;
    std::int64_t _reportingInterval;
    boost::optional<std::string> _id;
    boost::optional<std::string> _message;
    boost::optional<std::string> _informationalURL;
    boost::optional<std::string> _userReminder;
    boost::optional<bool> _resendRegistration;
    bool _hasVersion : 1;
    bool _hasHaltMetricsUploading : 1;
    bool _hasPermanentlyDelete : 1;
    bool _hasReportingInterval : 1;
};

}  // namespace mongo
