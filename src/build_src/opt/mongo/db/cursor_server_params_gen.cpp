/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/cursor_server_params_gen.h --output build/opt/mongo/db/cursor_server_params_gen.cpp src/mongo/db/cursor_server_params.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/cursor_server_params_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> gClientCursorMonitorFrequencySecs{4};
AtomicWord<long long> gCursorTimeoutMillis{600000};
MONGO_SERVER_PARAMETER_REGISTER(idl_4fba5585dd650379beb7da79494ffdef61084aff)(InitializerContext*) {
    /**
     * Interval at which expired cursors are identified and reaped
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("clientCursorMonitorFrequencySecs", gClientCursorMonitorFrequencySecs);
        return ret;
    })();

    /**
     * Period of time, in milliseconds, after which mortal cursors are killed for inactivity
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("cursorTimeoutMillis", gCursorTimeoutMillis);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
