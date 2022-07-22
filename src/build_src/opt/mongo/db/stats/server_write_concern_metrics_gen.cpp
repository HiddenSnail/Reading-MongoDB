/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/stats/server_write_concern_metrics_gen.h --output build/opt/mongo/db/stats/server_write_concern_metrics_gen.cpp src/mongo/db/stats/server_write_concern_metrics.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/stats/server_write_concern_metrics_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool reportOpWriteConcernCountersInServerStatus{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_dab8c7d3d630154b7836f7887c1e546f26cb9e07)(InitializerContext*) {
    /**
     * opWriteConcernCounters are not tracked unless this is true. This is false by default because the mutex acquisition impacts performance when SSL is disabled. (When SSL is enabled, other mutexes dominate.)
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("reportOpWriteConcernCountersInServerStatus", reportOpWriteConcernCountersInServerStatus);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
