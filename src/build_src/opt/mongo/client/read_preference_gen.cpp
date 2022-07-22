/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/read_preference_gen.h --output build/opt/mongo/client/read_preference_gen.cpp src/mongo/client/read_preference.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/client/read_preference_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Enumeration representing Read Preference Modes
 */
namespace  {
constexpr StringData kReadPreference_PrimaryOnly = "primary"_sd;
constexpr StringData kReadPreference_PrimaryPreferred = "primaryPreferred"_sd;
constexpr StringData kReadPreference_SecondaryOnly = "secondary"_sd;
constexpr StringData kReadPreference_SecondaryPreferred = "secondaryPreferred"_sd;
constexpr StringData kReadPreference_Nearest = "nearest"_sd;
}  // namespace 

ReadPreferenceEnum ReadPreference_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kReadPreference_PrimaryOnly) {
        return ReadPreferenceEnum::PrimaryOnly;
    }
    if (value == kReadPreference_PrimaryPreferred) {
        return ReadPreferenceEnum::PrimaryPreferred;
    }
    if (value == kReadPreference_SecondaryOnly) {
        return ReadPreferenceEnum::SecondaryOnly;
    }
    if (value == kReadPreference_SecondaryPreferred) {
        return ReadPreferenceEnum::SecondaryPreferred;
    }
    if (value == kReadPreference_Nearest) {
        return ReadPreferenceEnum::Nearest;
    }
    ctxt.throwBadEnumValue(value);
}

StringData ReadPreference_serializer(ReadPreferenceEnum value) {
    if (value == ReadPreferenceEnum::PrimaryOnly) {
        return kReadPreference_PrimaryOnly;
    }
    if (value == ReadPreferenceEnum::PrimaryPreferred) {
        return kReadPreference_PrimaryPreferred;
    }
    if (value == ReadPreferenceEnum::SecondaryOnly) {
        return kReadPreference_SecondaryOnly;
    }
    if (value == ReadPreferenceEnum::SecondaryPreferred) {
        return kReadPreference_SecondaryPreferred;
    }
    if (value == ReadPreferenceEnum::Nearest) {
        return kReadPreference_Nearest;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

}  // namespace mongo
