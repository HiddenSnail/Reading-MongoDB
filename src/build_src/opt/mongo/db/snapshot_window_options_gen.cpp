/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/snapshot_window_options_gen.h --output build/opt/mongo/db/snapshot_window_options_gen.cpp src/mongo/db/snapshot_window_options.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/snapshot_window_options_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

MONGO_SERVER_PARAMETER_REGISTER(idl_9f9852bf8c7821940266b14ab747993125c57886)(InitializerContext*) {
    /**
     * Maximum target snapshot history window, in seconds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxTargetSnapshotHistoryWindowInSeconds", snapshotWindowParams.maxTargetSnapshotHistoryWindowInSeconds);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Cache pressure threshold
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("cachePressureThreshold", snapshotWindowParams.cachePressureThreshold);
        ret->addBound<idl_server_parameter_detail::LTE>(100);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Snapshot window multiplicative decrease
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("snapshotWindowMultiplicativeDecrease", snapshotWindowParams.snapshotWindowMultiplicativeDecrease);
        ret->addBound<idl_server_parameter_detail::LT>(1.0);
        ret->addBound<idl_server_parameter_detail::GT>(0.0);
        return ret;
    })();

    /**
     * Snapshot window multiplicative increase, in seconds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("snapshotWindowAdditiveIncreaseSeconds", snapshotWindowParams.snapshotWindowAdditiveIncreaseSeconds);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Minimum duration between snapshot window increment, in milliseconds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("minMillisBetweenSnapshotWindowInc", snapshotWindowParams.minMillisBetweenSnapshotWindowInc);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Minimum duration between snapshot window decrement, in milliseconds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("minMillisBetweenSnapshotWindowDec", snapshotWindowParams.minMillisBetweenSnapshotWindowDec);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Check cache pressure period, in seconds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("checkCachePressurePeriodSeconds", snapshotWindowParams.checkCachePressurePeriodSeconds);
        ret->setOnUpdate(std::ref(SnapshotWindowParams::observeCheckCachePressurePeriodSeconds));
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
