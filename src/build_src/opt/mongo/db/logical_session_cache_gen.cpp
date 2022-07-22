/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/logical_session_cache_gen.h --output build/opt/mongo/db/logical_session_cache_gen.cpp src/mongo/db/logical_session_cache.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/logical_session_cache_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

int logicalSessionRefreshMillis{300000};
int maxSessions{1000000};
bool disableLogicalSessionCacheRefresh{false};
int gTransactionRecordMinimumLifetimeMinutes{30};
MONGO_SERVER_PARAMETER_REGISTER(idl_b4cee05cbe7cb02eb0d202d8f47939e2d1bca196)(InitializerContext*) {
    /**
     * The interval (in milliseconds) at which the cache refreshes its logical session records against the main session store.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("logicalSessionRefreshMillis", logicalSessionRefreshMillis);
        return ret;
    })();

    /**
     * The maximum number of sessions that can be cached.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("maxSessions", maxSessions);
        return ret;
    })();

    /**
     * Disable the logical session cache refresh (for testing only).
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("disableLogicalSessionCacheRefresh", disableLogicalSessionCacheRefresh);
        return ret;
    })();

    /**
     * The minimum lifetime for a transaction record.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("TransactionRecordMinimumLifetimeMinutes", gTransactionRecordMinimumLifetimeMinutes);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
