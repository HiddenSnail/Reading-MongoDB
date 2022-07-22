/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/auth_options_gen.h --output build/opt/mongo/db/auth/auth_options_gen.cpp src/mongo/db/auth/auth_options.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/auth/auth_options_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool roleGraphInvalidationIsFatal{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_bc61ef02ecf5881e52820dcd513438ce23b48d27)(InitializerContext*) {
    /**
     * Make role graph invalidation terminate the server
     */
    auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("roleGraphInvalidationIsFatal", roleGraphInvalidationIsFatal);
        return ret;
    })();
    scp_0->setTestOnly();

    return Status::OK();
}
}  // namespace mongo
