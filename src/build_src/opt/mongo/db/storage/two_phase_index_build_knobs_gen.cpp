/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/two_phase_index_build_knobs_gen.h --output build/opt/mongo/db/storage/two_phase_index_build_knobs_gen.cpp src/mongo/db/storage/two_phase_index_build_knobs.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/storage/two_phase_index_build_knobs_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool enableTwoPhaseIndexBuild{true};
bool enableIndexBuildCommitQuorum{true};
AtomicWord<int> maxNumActiveUserIndexBuilds{3};
MONGO_SERVER_PARAMETER_REGISTER(idl_edbde56ebd1a3db93d5d64bef5d4cdde6c045577)(InitializerContext*) {
    /**
     * Support for using startIndexBuild and commitIndexBuild to build new indexes.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("enableTwoPhaseIndexBuild", enableTwoPhaseIndexBuild);
        return ret;
    })();

    /**
     * Support for using commit quorum for two phase index builds.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("enableIndexBuildCommitQuorum", enableIndexBuildCommitQuorum);
        return ret;
    })();

            /**
             * Specifies the maximum number of active user index builds that can be built simultaneously on the primary node. Index builds initiated by the system are not subject to this limitation.
    Additionally, active index builds initiated by the system count towards the limit and can delay scheduling user index builds even when the number of active user index builds is below the limit.
    
             */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxNumActiveUserIndexBuilds", maxNumActiveUserIndexBuilds);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
