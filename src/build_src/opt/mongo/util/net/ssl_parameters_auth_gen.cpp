/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/util/net/ssl_parameters_auth_gen.h --output build/opt/mongo/util/net/ssl_parameters_auth_gen.cpp src/mongo/util/net/ssl_parameters_auth.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/util/net/ssl_parameters_auth_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * clusterAuthMode: Transition from sendKeyFile to sendX509, or sendX509 to x509 clusterAuthModes
 */
Status ClusterAuthModeServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

MONGO_SERVER_PARAMETER_REGISTER(idl_fa997c98ba838b297ed772cb9690fb529e43425c)(InitializerContext*) {
    /**
     * Transition from sendKeyFile to sendX509, or sendX509 to x509 clusterAuthModes
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new ClusterAuthModeServerParameter("clusterAuthMode", ServerParameterType::kRuntimeOnly);
    })();

    return Status::OK();
}
}  // namespace mongo
