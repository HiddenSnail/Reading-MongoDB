/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/ttl_gen.h --output build/opt/mongo/db/ttl_gen.cpp src/mongo/db/ttl.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/ttl_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> ttlMonitorEnabled{true};
AtomicWord<int> ttlMonitorSleepSecs{60};
MONGO_SERVER_PARAMETER_REGISTER(idl_202200dfd83a2b867ca338025844dec7de819b45)(InitializerContext*) {
    /**
     * Enable the TTL monitor.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ttlMonitorEnabled", ttlMonitorEnabled);
        return ret;
    })();

    /**
     * Period of TTL monitor thread (used for testing).
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ttlMonitorSleepSecs", ttlMonitorSleepSecs);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
