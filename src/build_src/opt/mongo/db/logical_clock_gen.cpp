/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/logical_clock_gen.h --output build/opt/mongo/db/logical_clock_gen.cpp src/mongo/db/logical_clock.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/logical_clock_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

long long gMaxAcceptableLogicalClockDriftSecs{(365 * 24 * 60 * 60)};
MONGO_SERVER_PARAMETER_REGISTER(idl_abacb19e07e7490c3007cf8665de157e7167fb93)(InitializerContext*) {
    /**
     * The value defines the maximum difference, in seconds, between the current clusterTime and a new clusterTime. Default value is 1 year.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("maxAcceptableLogicalClockDriftSecs", gMaxAcceptableLogicalClockDriftSecs);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
