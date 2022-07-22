/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/flow_control_parameters_gen.h --output build/opt/mongo/db/storage/flow_control_parameters_gen.cpp src/mongo/db/storage/flow_control_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/storage/flow_control_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gFlowControlEnabled{true};
AtomicWord<std::int32_t> gFlowControlTargetLagSeconds{10};
AtomicWord<double> gFlowControlThresholdLagPercentage{0.5};
int gFlowControlMaxSamples{1000000};
AtomicWord<int> gFlowControlSamplePeriod{1000};
AtomicWord<int> gFlowControlMinTicketsPerSecond{100};
AtomicWord<double> gFlowControlDecayConstant{0.5};
AtomicWord<double> gFlowControlFudgeFactor{0.95};
AtomicWord<int> gFlowControlTicketAdderConstant{1000};
AtomicWord<double> gFlowControlTicketMultiplierConstant{1.05};
AtomicWord<int> gFlowControlWarnThresholdSeconds{10};
MONGO_SERVER_PARAMETER_REGISTER(idl_3b9508fab768588483ab14f4f15556d8043cd16a)(InitializerContext*) {
    /**
     * Enable flow control
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("enableFlowControl", gFlowControlEnabled);
        return ret;
    })();

    /**
     * Target maximum majority committed lag with flow control enabled
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlTargetLagSeconds", gFlowControlTargetLagSeconds);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Threshold percentage of target lag where flow control will become engaged
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlThresholdLagPercentage", gFlowControlThresholdLagPercentage);
        ret->addBound<idl_server_parameter_detail::LTE>(1.0);
        ret->addBound<idl_server_parameter_detail::GTE>(0.0);
        return ret;
    })();

    /**
     * The maximum number of samples the flow control algorithm will hold onto.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("flowControlMaxSamples", gFlowControlMaxSamples);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * How often flow control samples optimes. Smaller periods have higher resolution, but will suffer degredation sooner in the event of commit point lag.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlSamplePeriod", gFlowControlSamplePeriod);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * The minimum number of tickets flow control will hand out per second. A smaller value will have better resolution on batch-write workloads, but may unintentionally delay system operations.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlMinTicketsPerSecond", gFlowControlMinTicketsPerSecond);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * The rate at which flow control will throttle based on how lagged the commit point is in time relative to the flow control threshold.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlDecayConstant", gFlowControlDecayConstant);
        ret->addBound<idl_server_parameter_detail::LT>(1.0);
        ret->addBound<idl_server_parameter_detail::GT>(0.0);
        return ret;
    })();

    /**
     * When commit point lag is close to the threshold lag, the primary should track the sustainer rate, with some small penalty. This value represents that penalty. A value of 1.0 represents no penalty, a value close to 0.0 represents a large penalty. Reducing oscillations should keep this value close to 1.0.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlFudgeFactor", gFlowControlFudgeFactor);
        ret->addBound<idl_server_parameter_detail::GT>(0.0);
        ret->addBound<idl_server_parameter_detail::LTE>(1.0);
        return ret;
    })();

    /**
     * When the commit point lag is small, flow control will add at least this many tickets per second. This provides meaningful ticket increase when immediately recovering from stalling events.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_8 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlTicketAdderConstant", gFlowControlTicketAdderConstant);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * When the commit point lag is small, flow control will increase its ticket allocation by this factor. This provides meaningful ticket increase in more general recovery scenarios.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_9 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlTicketMultiplierConstant", gFlowControlTicketMultiplierConstant);
        ret->addBound<idl_server_parameter_detail::GT>(1.0);
        return ret;
    })();

    /**
     * If flow control detects the replica set is lagged and the sustainer point is not moving, it will eventually log a warning. This value controls how much time the flow control is in this state before it logs. A value of zero will disable the warnings.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_10 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("flowControlWarnThresholdSeconds", gFlowControlWarnThresholdSeconds);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
