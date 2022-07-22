/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/global_conn_pool_gen.h --output build/opt/mongo/client/global_conn_pool_gen.cpp src/mongo/client/global_conn_pool.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/client/global_conn_pool_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

int maxConnsPerHost{200};
int maxInUseConnsPerHost{std::numeric_limits<int>::max()};
int globalConnPoolIdleTimeout{std::numeric_limits<int>::max()};
MONGO_SERVER_PARAMETER_REGISTER(idl_ecb07a8437f03ceb4fe24aa53669f33cffc782fe)(InitializerContext*) {
            /**
             * New in version 2.6.
    Available for both mongod and mongos.
    Sets the maximum size of the legacy connection pools for outgoing connections to other mongod instances in the global connection pool. The size of a pool does not prevent the creation of additional connections, but does prevent a connection pool from retaining connections in excess of the value of connPoolMaxConnsPerHost.
    Only adjust this setting if your driver does not pool connections and you're using authentication in the context of a sharded cluster.
    
             */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("connPoolMaxConnsPerHost", maxConnsPerHost);
        return ret;
    })();

            /**
             * New in version 3.6.3.
    Available for both mongod and mongos.
    Sets the maximum number of in-use connections at any given time for for outgoing connections to other mongod instances in the legacy global connection pool.
    By default, the parameter is unset.
    
             */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("connPoolMaxInUseConnsPerHost", maxInUseConnsPerHost);
        return ret;
    })();

            /**
             * New in version 3.6.3.
    Available for both mongod and mongos.
    Sets the time limit that connection in the legacy global connection pool can remain idle before being closed.
    By default, the parameter is unset.
    
             */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("globalConnPoolIdleTimeoutMinutes", globalConnPoolIdleTimeout);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
