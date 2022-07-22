/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/client/shard_remote_gen.h --output build/opt/mongo/s/client/shard_remote_gen.cpp src/mongo/s/client/shard_remote.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/client/shard_remote_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gInternalProhibitShardOperationRetry{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_6466812a9192ce7fb46ce034080129dde2b6b3e3)(InitializerContext*) {
    /**
     * Prohibit retrying shard operations.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalProhibitShardOperationRetry", gInternalProhibitShardOperationRetry);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
