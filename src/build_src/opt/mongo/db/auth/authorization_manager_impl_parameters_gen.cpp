/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/authorization_manager_impl_parameters_gen.h --output build/opt/mongo/db/auth/authorization_manager_impl_parameters_gen.cpp src/mongo/db/auth/authorization_manager_impl_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/auth/authorization_manager_impl_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<long long> authorizationManagerPinnedUsersRefreshIntervalMillis{1000};
MONGO_SERVER_PARAMETER_REGISTER(idl_54f8f7a0f1900bb57ff5770bc33ad13edb7c7d3b)(InitializerContext*) {
    /**
     * Element count limit on the AuthorizationManager's user handle cache.
    
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("authorizationManagerCacheSize", authorizationManagerCacheSize);
        uassertStatusOK(ret->setValue(100));
        return ret;
    })();

    /**
     * A comma-separated sequence of user names.
    
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        return new AuthorizationManagerPinnedUsersServerParameter("authorizationManagerPinnedUsers", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * The number of milliseconds between refreshing pinned users.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("authorizationManagerPinnedUsersRefreshIntervalMillis", authorizationManagerPinnedUsersRefreshIntervalMillis);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
