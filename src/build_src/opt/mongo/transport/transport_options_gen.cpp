/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/transport/transport_options_gen.h --output build/opt/mongo/transport/transport_options_gen.cpp src/mongo/transport/transport_options.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/transport/transport_options_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {
namespace transport {

bool gTCPFastOpenServer{true};
int gTCPFastOpenQueueSize{1024};
bool gTCPFastOpenClient{true};
MONGO_SERVER_PARAMETER_REGISTER(idl_1e9193224ed5ec820f54cb5266727c81508f11f3)(InitializerContext*) {
    /**
     * Enable TCP Fast Open for inbound connections
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("tcpFastOpenServer", gTCPFastOpenServer);
        return ret;
    })();

    /**
     * TCP Fast Open queue length
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("tcpFastOpenQueueSize", gTCPFastOpenQueueSize);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Enable TCP Fast Open when connecting to remote servers
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("tcpFastOpenClient", gTCPFastOpenClient);
        return ret;
    })();

    return Status::OK();
}
}  // namespace transport
}  // namespace mongo
