/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/replication_coordinator_impl_gen.h --output build/opt/mongo/db/repl/replication_coordinator_impl_gen.cpp src/mongo/db/repl/replication_coordinator_impl.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/replication_coordinator_impl_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {
namespace repl {

AtomicWord<int> numInitialSyncAttempts{10};
AtomicWord<bool> enableElectionHandoff{true};
AtomicWord<bool> waitForStepDownOnNonCommandShutdown{true};
AtomicWord<int> periodicNoopIntervalSecs{10};
bool gTestingSnapshotBehaviorInIsolation;
MONGO_SERVER_PARAMETER_REGISTER(idl_43d4ca86eda1d8935717c5b47ead487b5ef931d9)(InitializerContext*) {
    /**
     * <- Number of attempts to make at replica set initial synchronization.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("numInitialSyncAttempts", numInitialSyncAttempts);
        return ret;
    })();

    /**
     * <- Reduce the downtime after the primary steps down from either the rs.stepDown() method or the replSetStepDown command.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("enableElectionHandoff", enableElectionHandoff);
        return ret;
    })();

    /**
     * <- Whether the primary waits to complete a stepdown and to hand off its duties to a secondary before shutting down in response to SIGTERM. If false, the primary shuts down more abruptly.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("waitForStepDownOnNonCommandShutdown", waitForStepDownOnNonCommandShutdown);
        return ret;
    })();

    /**
     * <- Number of seconds between noop writer writes.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("periodicNoopIntervalSecs", periodicNoopIntervalSecs);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        ret->addBound<idl_server_parameter_detail::LTE>(10);
        return ret;
    })();

    /**
     * Allows for testing of snapshot behavior by skipping the replication related checks and isolating the storage/query side of snapshotting.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("testingSnapshotBehaviorInIsolation", gTestingSnapshotBehaviorInIsolation);
        return ret;
    })();

    return Status::OK();
}
}  // namespace repl
}  // namespace mongo
