/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_storage_gen.h --output build/opt/mongo/db/free_mon/free_mon_storage_gen.cpp src/mongo/db/free_mon/free_mon_storage.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/free_mon/free_mon_storage_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Action types
 */
namespace  {
constexpr StringData kStorageState_disabled = "disabled"_sd;
constexpr StringData kStorageState_enabled = "enabled"_sd;
constexpr StringData kStorageState_pending = "pending"_sd;
}  // namespace 

StorageStateEnum StorageState_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kStorageState_disabled) {
        return StorageStateEnum::disabled;
    }
    if (value == kStorageState_enabled) {
        return StorageStateEnum::enabled;
    }
    if (value == kStorageState_pending) {
        return StorageStateEnum::pending;
    }
    ctxt.throwBadEnumValue(value);
}

StringData StorageState_serializer(StorageStateEnum value) {
    if (value == StorageStateEnum::disabled) {
        return kStorageState_disabled;
    }
    if (value == StorageStateEnum::enabled) {
        return kStorageState_enabled;
    }
    if (value == StorageStateEnum::pending) {
        return kStorageState_pending;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData FreeMonStorageState::k_idFieldName;
constexpr StringData FreeMonStorageState::kInformationalURLFieldName;
constexpr StringData FreeMonStorageState::kMessageFieldName;
constexpr StringData FreeMonStorageState::kRegistrationIdFieldName;
constexpr StringData FreeMonStorageState::kStateFieldName;
constexpr StringData FreeMonStorageState::kUserReminderFieldName;
constexpr StringData FreeMonStorageState::kVersionFieldName;


FreeMonStorageState::FreeMonStorageState() : _version(-1), _hasVersion(false), _hasRegistrationId(false), _hasInformationalURL(false), _hasMessage(false), _hasUserReminder(false) {
    // Used for initialization only
}
FreeMonStorageState::FreeMonStorageState(std::int64_t version, std::string registrationId, std::string informationalURL, std::string message, std::string userReminder) : _version(std::move(version)), _registrationId(std::move(registrationId)), _informationalURL(std::move(informationalURL)), _message(std::move(message)), _userReminder(std::move(userReminder)), _hasVersion(true), _hasRegistrationId(true), _hasInformationalURL(true), _hasMessage(true), _hasUserReminder(true) {
    // Used for initialization only
}


FreeMonStorageState FreeMonStorageState::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    FreeMonStorageState object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void FreeMonStorageState::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                __id = element.str();
            }
        }
        else if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                _hasVersion = true;
                _version = element._numberLong();
            }
        }
        else if (fieldName == kStateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                IDLParserErrorContext tempContext(kStateFieldName, &ctxt);
                _state = StorageState_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kRegistrationIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasRegistrationId = true;
                _registrationId = element.str();
            }
        }
        else if (fieldName == kInformationalURLFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasInformationalURL = true;
                _informationalURL = element.str();
            }
        }
        else if (fieldName == kMessageFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasMessage = true;
                _message = element.str();
            }
        }
        else if (fieldName == kUserReminderFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasUserReminder = true;
                _userReminder = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(k_idFieldName) == usedFields.end())) {
        __id = "free_monitoring";
    }
    if (MONGO_unlikely(usedFields.find(kVersionFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kVersionFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kStateFieldName) == usedFields.end())) {
        _state = mongo::StorageStateEnum::disabled;
    }
    if (MONGO_unlikely(usedFields.find(kRegistrationIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kRegistrationIdFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kInformationalURLFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kInformationalURLFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kMessageFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kMessageFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kUserReminderFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kUserReminderFieldName);
    }

}


void FreeMonStorageState::serialize(BSONObjBuilder* builder) const {
    invariant(_hasVersion && _hasRegistrationId && _hasInformationalURL && _hasMessage && _hasUserReminder);

    builder->append(k_idFieldName, __id);

    builder->append(kVersionFieldName, _version);

    {
        builder->append(kStateFieldName, StorageState_serializer(_state));
    }

    builder->append(kRegistrationIdFieldName, _registrationId);

    builder->append(kInformationalURLFieldName, _informationalURL);

    builder->append(kMessageFieldName, _message);

    builder->append(kUserReminderFieldName, _userReminder);

}


BSONObj FreeMonStorageState::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
