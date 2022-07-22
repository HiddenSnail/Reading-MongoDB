/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/index/index_build_interceptor_gen.h --output build/opt/mongo/db/index/index_build_interceptor_gen.cpp src/mongo/db/index/index_build_interceptor.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/index/index_build_interceptor_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> maxIndexBuildDrainBatchSize{1000};
AtomicWord<int> maxIndexBuildDrainMemoryUsageMegabytes{16};
MONGO_SERVER_PARAMETER_REGISTER(idl_f1c34760c430b0f964e6038f6c54226c173e6265)(InitializerContext*) {
    /**
     * Limits the number of keys that a hybrid index build on one collection will apply per batch during the drain phase. A higher value prevents the operation for yielding locks and storage engine resources.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxIndexBuildDrainBatchSize", maxIndexBuildDrainBatchSize);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Limits the amount of memory that a hybrid index build on one collection may consume during the drain phase. A higher value will exert more cache pressure on the storage engine.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxIndexBuildDrainMemoryUsageMegabytes", maxIndexBuildDrainMemoryUsageMegabytes);
        ret->addBound<idl_server_parameter_detail::LT>(2048);
        ret->addBound<idl_server_parameter_detail::GTE>(16);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
