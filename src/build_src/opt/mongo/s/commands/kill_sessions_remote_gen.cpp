/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/commands/kill_sessions_remote_gen.h --output build/opt/mongo/s/commands/kill_sessions_remote_gen.cpp src/mongo/s/commands/kill_sessions_remote.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/commands/kill_sessions_remote_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

int gKillSessionsMaxConcurrency{100};
int gKillSessionsPerHostTimeoutMS{60000};
MONGO_SERVER_PARAMETER_REGISTER(idl_746eef40d01ee29ed7d1ebc24f46b15ce89614cb)(InitializerContext*) {
    /**
     * The maximum number of hosts to for killSessionsRemote to kill sessions on at the same time.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("KillSessionsMaxConcurrency", gKillSessionsMaxConcurrency);
        return ret;
    })();

    /**
     * The maximum number of miliseconds for killSessionsRemote to wait for host to respond.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("KillSessionsPerHostTimeoutMS", gKillSessionsPerHostTimeoutMS);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
