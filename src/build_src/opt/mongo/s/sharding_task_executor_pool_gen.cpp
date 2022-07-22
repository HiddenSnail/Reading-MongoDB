/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/sharding_task_executor_pool_gen.h --output build/opt/mongo/s/sharding_task_executor_pool_gen.cpp src/mongo/s/sharding_task_executor_pool.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/sharding_task_executor_pool_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

MONGO_SERVER_PARAMETER_REGISTER(idl_47322fe18df7894bc662bd7c55c6f0be8aa37c43)(InitializerContext*) {
    /**
     * <- The minimum number of connections for each executor in the pool for the sharding grid.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolMinSize", ShardingTaskExecutorPoolController::gParameters.minConnections);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        uassertStatusOK(ret->setValue(1));
        return ret;
    })();

    /**
     * <- The maximum number of connections for each executor in the pool for the sharding grid.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolMaxSize", ShardingTaskExecutorPoolController::gParameters.maxConnections);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        uassertStatusOK(ret->setValue(32767));
        return ret;
    })();

    /**
     * <- The maximum number of in-flight connections for each executor in the pool for the sharding grid.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolMaxConnecting", ShardingTaskExecutorPoolController::gParameters.maxConnecting);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        uassertStatusOK(ret->setValue(2));
        return ret;
    })();

    /**
     * <- The timeout for dropping a host for each executor in the pool for the sharding grid.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolHostTimeoutMS", ShardingTaskExecutorPoolController::gParameters.hostTimeoutMS);
        ret->addValidator(ShardingTaskExecutorPoolController::validateHostTimeout);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        uassertStatusOK(ret->setValue(300000));
        return ret;
    })();

    /**
     * <- The timeout before a connection needs to be refreshed for each executor in the pool for the sharding grid.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolRefreshRequirementMS", ShardingTaskExecutorPoolController::gParameters.toRefreshTimeoutMS);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        uassertStatusOK(ret->setValue(60000));
        return ret;
    })();

    /**
     * <- The timeout for refreshing a connection for each executor in the pool for the sharding grid.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolRefreshTimeoutMS", ShardingTaskExecutorPoolController::gParameters.pendingTimeoutMS);
        ret->addValidator(ShardingTaskExecutorPoolController::validatePendingTimeout);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        uassertStatusOK(ret->setValue(20000));
        return ret;
    })();

    /**
     * <- Enables ReplicaSet member connection matching.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("ShardingTaskExecutorPoolReplicaSetMatching", ShardingTaskExecutorPoolController::gParameters.matchingStrategyString);
        ret->setOnUpdate(ShardingTaskExecutorPoolController::onUpdateMatchingStrategy);
        uassertStatusOK(ret->setValue("matchPrimaryNode"));
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
