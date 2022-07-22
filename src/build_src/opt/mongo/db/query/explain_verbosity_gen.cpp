/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/explain_verbosity_gen.h --output build/opt/mongo/db/query/explain_verbosity_gen.cpp src/mongo/db/query/explain_verbosity.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/query/explain_verbosity_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/util/visit_helper.h"

namespace mongo {
namespace explain {

/**
 * The various supported verbosity levels for explain. The order is significant: the enum values are assigned in order of increasing verbosity
 */
namespace  {
constexpr StringData kVerbosity_kQueryPlanner = "queryPlanner"_sd;
constexpr StringData kVerbosity_kExecStats = "executionStats"_sd;
constexpr StringData kVerbosity_kExecAllPlans = "allPlansExecution"_sd;
}  // namespace 

VerbosityEnum Verbosity_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kVerbosity_kQueryPlanner) {
        return VerbosityEnum::kQueryPlanner;
    }
    if (value == kVerbosity_kExecStats) {
        return VerbosityEnum::kExecStats;
    }
    if (value == kVerbosity_kExecAllPlans) {
        return VerbosityEnum::kExecAllPlans;
    }
    ctxt.throwBadEnumValue(value);
}

StringData Verbosity_serializer(VerbosityEnum value) {
    if (value == VerbosityEnum::kQueryPlanner) {
        return kVerbosity_kQueryPlanner;
    }
    if (value == VerbosityEnum::kExecStats) {
        return kVerbosity_kExecStats;
    }
    if (value == VerbosityEnum::kExecAllPlans) {
        return kVerbosity_kExecAllPlans;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

}  // namespace explain
}  // namespace mongo
