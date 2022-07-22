/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/replica_set_monitor_server_parameters_gen.h --output build/opt/mongo/client/replica_set_monitor_server_parameters_gen.cpp src/mongo/client/replica_set_monitor_server_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/client/replica_set_monitor_server_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * replicaSetMonitorProtocol: Select which replica set monitor protocol to use - the new 'streamable' protocol that is both SDAM compliant and allows for awaitable isMaster with exhaust, the 'sdam' compliant protocol or the old 'scanning' protocol.
 */
Status RSMProtocolServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

MONGO_SERVER_PARAMETER_REGISTER(idl_8795e1259a1a80810a081d4403a1a42e74f26f4f)(InitializerContext*) {
    /**
     * Select which replica set monitor protocol to use - the new 'streamable' protocol that is both SDAM compliant and allows for awaitable isMaster with exhaust, the 'sdam' compliant protocol or the old 'scanning' protocol.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new RSMProtocolServerParameter("replicaSetMonitorProtocol", ServerParameterType::kStartupOnly);
    })();

    return Status::OK();
}
}  // namespace mongo
