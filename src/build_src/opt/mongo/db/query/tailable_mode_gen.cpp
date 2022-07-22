/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/tailable_mode_gen.h --output build/opt/mongo/db/query/tailable_mode_gen.cpp src/mongo/db/query/tailable_mode.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/query/tailable_mode_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Describes the tailablility of a cursor.
 */
namespace  {
constexpr StringData kTailableMode_kNormal = "normal"_sd;
constexpr StringData kTailableMode_kTailable = "tailable"_sd;
constexpr StringData kTailableMode_kTailableAndAwaitData = "tailableAndAwaitData"_sd;
}  // namespace 

TailableModeEnum TailableMode_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kTailableMode_kNormal) {
        return TailableModeEnum::kNormal;
    }
    if (value == kTailableMode_kTailable) {
        return TailableModeEnum::kTailable;
    }
    if (value == kTailableMode_kTailableAndAwaitData) {
        return TailableModeEnum::kTailableAndAwaitData;
    }
    ctxt.throwBadEnumValue(value);
}

StringData TailableMode_serializer(TailableModeEnum value) {
    if (value == TailableModeEnum::kNormal) {
        return kTailableMode_kNormal;
    }
    if (value == TailableModeEnum::kTailable) {
        return kTailableMode_kTailable;
    }
    if (value == TailableModeEnum::kTailableAndAwaitData) {
        return kTailableMode_kTailableAndAwaitData;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

}  // namespace mongo
