/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/enable_localhost_auth_bypass_parameter_gen.h --output build/opt/mongo/db/auth/enable_localhost_auth_bypass_parameter_gen.cpp src/mongo/db/auth/enable_localhost_auth_bypass_parameter.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/auth/enable_localhost_auth_bypass_parameter_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool enableLocalhostAuthBypass{true};
MONGO_SERVER_PARAMETER_REGISTER(idl_8ca33f361b0621cd3831637dfaadf09a52d75e2c)(InitializerContext*) {
    /**
     * Enable authorization bypass for localhost connections
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("enableLocalhostAuthBypass", enableLocalhostAuthBypass);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
