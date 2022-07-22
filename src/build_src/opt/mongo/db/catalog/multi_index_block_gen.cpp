/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/multi_index_block_gen.h --output build/opt/mongo/db/catalog/multi_index_block_gen.cpp src/mongo/db/catalog/multi_index_block.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/catalog/multi_index_block_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> useReadOnceCursorsForIndexBuilds{true};
AtomicWord<bool> enableHybridIndexBuilds{true};
AtomicWord<int> maxIndexBuildMemoryUsageMegabytes{200};
MONGO_SERVER_PARAMETER_REGISTER(idl_c325e5775ad1b65651b63414b494e1bbc482e962)(InitializerContext*) {
    /**
     * When true, enables uses of read once cursors for index builds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("useReadOnceCursorsForIndexBuilds", useReadOnceCursorsForIndexBuilds);
        return ret;
    })();

    /**
     * Build all indexes by performing external sorting while accepting concurrent client writes
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("enableHybridIndexBuilds", enableHybridIndexBuilds);
        return ret;
    })();

    /**
     * Limits the amount of memory that simultaneous index builds on one collection may consume for the duration of the builds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxIndexBuildMemoryUsageMegabytes", maxIndexBuildMemoryUsageMegabytes);
        ret->addBound<idl_server_parameter_detail::GTE>(50);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
