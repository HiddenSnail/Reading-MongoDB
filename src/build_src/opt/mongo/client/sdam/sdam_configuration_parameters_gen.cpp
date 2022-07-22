/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sdam/sdam_configuration_parameters_gen.h --output build/opt/mongo/client/sdam/sdam_configuration_parameters_gen.cpp src/mongo/client/sdam/sdam_configuration_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/client/sdam/sdam_configuration_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {
namespace sdam {

int sdamHeartBeatFrequencyMs{10000};
int sdamLocalThreshholdMs{15};
int sdamConnectTimeoutMs{10000};
MONGO_SERVER_PARAMETER_REGISTER(idl_105eca4de5c6fb6d81e31fddc961d13843dd39cd)(InitializerContext*) {
    /**
     * For the 'sdam' replicaSetMonitorProtocol, determines how long to wait between isMaster requests. For the 'streamable' replicaSetMonitorProtocol, duration between rtt measurements.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("heartBeatFrequencyMs", sdamHeartBeatFrequencyMs);
        ret->addBound<idl_server_parameter_detail::GTE>(500);
        return ret;
    })();

    /**
     * Determines the width of the latency window used in server selection.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("localThresholdMs", sdamLocalThreshholdMs);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Determines the connection timeout used in the replica set monitor.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("connectTimeoutMs", sdamConnectTimeoutMs);
        ret->addBound<idl_server_parameter_detail::GTE>(500);
        return ret;
    })();

    return Status::OK();
}
}  // namespace sdam
}  // namespace mongo
