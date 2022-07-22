/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/client/shard_connection_gen.h --output build/opt/mongo/s/client/shard_connection_gen.cpp src/mongo/s/client/shard_connection.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/client/shard_connection_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

int gMaxShardedConnsPerHost{200};
int gMaxShardedInUseConnsPerHost{std::numeric_limits<int>::max()};
int gShardedConnPoolIdleTimeout{std::numeric_limits<int>::max()};
MONGO_SERVER_PARAMETER_REGISTER(idl_8e3863b0202e12fb8c8306d9da3f1be4fb06a388)(InitializerContext*) {
    /**
     * Maximum connections per host the sharded conn pool should store
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("connPoolMaxShardedConnsPerHost", gMaxShardedConnsPerHost);
        return ret;
    })();

    /**
     * Maximum in-use connections per host in the sharded connection pool
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("connPoolMaxShardedInUseConnsPerHost", gMaxShardedInUseConnsPerHost);
        return ret;
    })();

    /**
     * Amount of time, in minutes, to keep idle connections in the sharded connection pool
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("shardedConnPoolIdleTimeoutMinutes", gShardedConnPoolIdleTimeout);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
