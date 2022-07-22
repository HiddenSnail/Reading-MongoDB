/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/user_cache_invalidator_job_parameters_gen.h --output build/opt/mongo/db/auth/user_cache_invalidator_job_parameters_gen.cpp src/mongo/db/auth/user_cache_invalidator_job_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/auth/user_cache_invalidator_job_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> userCacheInvalidationIntervalSecs{30};
MONGO_SERVER_PARAMETER_REGISTER(idl_a306e06485bbb8e48c1b069ecc9be88ecfa81281)(InitializerContext*) {
    /**
     * On a mongos instance, specifies the interval (in seconds) at which the mongos instance checks to determine whether the in-memory cache of user objects has stale data, and if so, clears the cache. If there are no changes to user objects, mongos will not clear the cache.
    
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("userCacheInvalidationIntervalSecs", userCacheInvalidationIntervalSecs);
        ret->setOnUpdate(userCacheInvalidationIntervalSecsNotify);
        ret->addBound<idl_server_parameter_detail::LTE>(86400);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
