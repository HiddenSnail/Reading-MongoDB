/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/scripting/deadline_monitor_gen.h --output build/opt/mongo/scripting/deadline_monitor_gen.cpp src/mongo/scripting/deadline_monitor.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/scripting/deadline_monitor_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> gScriptingEngineInterruptIntervalMS{1000};
MONGO_SERVER_PARAMETER_REGISTER(idl_d19b31960fb45750777a81561bfde91056eb2bd0)(InitializerContext*) {
    /**
     * The number of milliseconds between executions of the deadline monitor.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("scriptingEngineInterruptIntervalMS", gScriptingEngineInterruptIntervalMS);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
