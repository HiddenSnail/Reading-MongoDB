/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_storage_gen.h --output build/opt/mongo/db/free_mon/free_mon_storage_gen.cpp src/mongo/db/free_mon/free_mon_storage.idl
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
 * Action types
 */
enum class StorageStateEnum : std::int32_t {
    disabled ,
    enabled ,
    pending ,
};

StorageStateEnum StorageState_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData StorageState_serializer(StorageStateEnum value);

/**
 * Persisted document in admin.system.version
 */
class FreeMonStorageState {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kInformationalURLFieldName = "informationalURL"_sd;
    static constexpr auto kMessageFieldName = "message"_sd;
    static constexpr auto kRegistrationIdFieldName = "registrationId"_sd;
    static constexpr auto kStateFieldName = "state"_sd;
    static constexpr auto kUserReminderFieldName = "userReminder"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    FreeMonStorageState();
    FreeMonStorageState(std::int64_t version, std::string registrationId, std::string informationalURL, std::string message, std::string userReminder);

    static FreeMonStorageState parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Key of the Free Monitoring singleton document
     */
    const StringData get_id() const& { return __id; }
    void get_id() && = delete;
    void set_id(StringData value) & { auto _tmpValue = value.toString();  __id = std::move(_tmpValue);  }

    /**
     * Storage version, initial version is 1
     */
    std::int64_t getVersion() const { return _version; }
    void setVersion(std::int64_t value) & {  _version = std::move(value); _hasVersion = true; }

    /**
     * Indicates whether it is disabled or enabled
     */
    mongo::StorageStateEnum getState() const { return _state; }
    void setState(mongo::StorageStateEnum value) & {  _state = std::move(value);  }

    /**
     * Registration Id
     */
    const StringData getRegistrationId() const& { return _registrationId; }
    void getRegistrationId() && = delete;
    void setRegistrationId(StringData value) & { auto _tmpValue = value.toString();  _registrationId = std::move(_tmpValue); _hasRegistrationId = true; }

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
     * Message to display to user to remind them about service
     */
    const StringData getUserReminder() const& { return _userReminder; }
    void getUserReminder() && = delete;
    void setUserReminder(StringData value) & { auto _tmpValue = value.toString();  _userReminder = std::move(_tmpValue); _hasUserReminder = true; }


    friend bool operator==(const FreeMonStorageState& left, const FreeMonStorageState& right) {
        return left.__id == right.__id && left._version == right._version && left._state == right._state && left._registrationId == right._registrationId && left._informationalURL == right._informationalURL && left._message == right._message && left._userReminder == right._userReminder;
    }

    friend bool operator!=(const FreeMonStorageState& left, const FreeMonStorageState& right) {
        return left.__id != right.__id || left._version != right._version || left._state != right._state || left._registrationId != right._registrationId || left._informationalURL != right._informationalURL || left._message != right._message || left._userReminder != right._userReminder;
    }
    friend bool operator<(const FreeMonStorageState& left, const FreeMonStorageState& right) {
        return left.__id < right.__id || (!(right.__id < left.__id) && (left._version < right._version || (!(right._version < left._version) && (left._state < right._state || (!(right._state < left._state) && (left._registrationId < right._registrationId || (!(right._registrationId < left._registrationId) && (left._informationalURL < right._informationalURL || (!(right._informationalURL < left._informationalURL) && (left._message < right._message || (!(right._message < left._message) && (left._userReminder < right._userReminder))))))))))));
    }
    friend bool operator>(const FreeMonStorageState& left, const FreeMonStorageState& right) {
        return right < left;
    }
    friend bool operator<=(const FreeMonStorageState& left, const FreeMonStorageState& right) {
        return !(right < left);
    }
    friend bool operator>=(const FreeMonStorageState& left, const FreeMonStorageState& right) {
        return !(left < right);
    }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string __id{"free_monitoring"};
    std::int64_t _version;
    mongo::StorageStateEnum _state{mongo::StorageStateEnum::disabled};
    std::string _registrationId;
    std::string _informationalURL;
    std::string _message;
    std::string _userReminder;
    bool _hasVersion : 1;
    bool _hasRegistrationId : 1;
    bool _hasInformationalURL : 1;
    bool _hasMessage : 1;
    bool _hasUserReminder : 1;
};

}  // namespace mongo
