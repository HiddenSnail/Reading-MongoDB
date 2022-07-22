/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/topology_coordinator_gen.h --output build/opt/mongo/db/repl/topology_coordinator_gen.cpp src/mongo/db/repl/topology_coordinator.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/topology_coordinator_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

int gPriorityTakeoverFreshnessWindowSeconds{2};
MONGO_SERVER_PARAMETER_REGISTER(idl_ce5e0aa5bef218299c1fa4f49b3d1c77feaed90f)(InitializerContext*) {
    /**
     * Controls how caught up in replication a secondary with higher priority than the  current primary must be before it will call for a priority takeover election.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("priorityTakeoverFreshnessWindowSeconds", gPriorityTakeoverFreshnessWindowSeconds);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
