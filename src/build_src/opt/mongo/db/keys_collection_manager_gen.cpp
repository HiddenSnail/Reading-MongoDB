/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/keys_collection_manager_gen.h --output build/opt/mongo/db/keys_collection_manager_gen.cpp src/mongo/db/keys_collection_manager.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/keys_collection_manager_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

int KeysRotationIntervalSec{3 * 30 * 24 * 60 * 60};
MONGO_SERVER_PARAMETER_REGISTER(idl_cd2f134060cbbb0f81278968ffb673db6d49b2fa)(InitializerContext*) {
    /**
     * Specifies the number of seconds for which an HMAC signing key is valid before rotating to the next one. This parameter is intended primarily to facilitate authentication testing.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("KeysRotationIntervalSec", KeysRotationIntervalSec);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
