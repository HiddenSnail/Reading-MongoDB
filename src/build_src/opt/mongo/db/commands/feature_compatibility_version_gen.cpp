/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/feature_compatibility_version_gen.h --output build/opt/mongo/db/commands/feature_compatibility_version_gen.cpp src/mongo/db/commands/feature_compatibility_version.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/feature_compatibility_version_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * featureCompatibilityVersion: Read-only view of current Feature Compatability Version
 */
Status FeatureCompatibilityVersionParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

bool gInternalValidateFeaturesAsMaster{true};
MONGO_SERVER_PARAMETER_REGISTER(idl_9efce71c8b97b064552b82f56e87b6dbfd3bdfa0)(InitializerContext*) {
    /**
     * Read-only view of current Feature Compatability Version
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new FeatureCompatibilityVersionParameter("featureCompatibilityVersion", ServerParameterType::kStartupOnly);
    })();

    /**
     * Startup parameter to ignore featureCompatibilityVersion checks. This parameter cannot be set if the node is started with --replSet. This should never be set by end users.
    
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("internalValidateFeaturesAsMaster", gInternalValidateFeaturesAsMaster);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
