/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/authorization_manager_global_parameters_gen.h --output build/opt/mongo/db/auth/authorization_manager_global_parameters_gen.cpp src/mongo/db/auth/authorization_manager_global_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/auth/authorization_manager_global_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * authSchemaVersion: Read-only value describing the current auth schema version
 */
Status AuthzVersionParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

bool gStartupAuthSchemaValidation{true};
bool allowRolesFromX509Certificates{true};
MONGO_SERVER_PARAMETER_REGISTER(idl_5f995c9e532682907ecf6de561a1ef7854e056fd)(InitializerContext*) {
    /**
     * Read-only value describing the current auth schema version
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new AuthzVersionParameter("authSchemaVersion", ServerParameterType::kStartupOnly);
    })();

    /**
     * Validate auth schema on startup
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("startupAuthSchemaValidation", gStartupAuthSchemaValidation);
        return ret;
    })();

    /**
     * Whether to allow roles contained in X509 certificates if X509 authentication is enabled
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("allowRolesFromX509Certificates", allowRolesFromX509Certificates);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
