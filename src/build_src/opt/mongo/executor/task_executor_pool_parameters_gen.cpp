/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/executor/task_executor_pool_parameters_gen.h --output build/opt/mongo/executor/task_executor_pool_parameters_gen.cpp src/mongo/executor/task_executor_pool_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/executor/task_executor_pool_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> taskExecutorPoolSize{1};
MONGO_SERVER_PARAMETER_REGISTER(idl_04d883c8528a53efbc77b1d992d772881a31c2e1)(InitializerContext*) {
    /**
     * If set to less than or equal to 0, then the suggested pool size will be determined by the number of cores. If set to greater than 0, that value will be used as the pool size.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("taskExecutorPoolSize", taskExecutorPoolSize);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
