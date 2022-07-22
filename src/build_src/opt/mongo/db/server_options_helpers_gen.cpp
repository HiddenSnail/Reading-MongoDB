/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/server_options_helpers_gen.h --output build/opt/mongo/db/server_options_helpers_gen.cpp src/mongo/db/server_options_helpers.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/server_options_helpers_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

MONGO_SERVER_PARAMETER_REGISTER(idl_8e50acf40a35f01e97bf43b210bd689bd38d1fd5)(InitializerContext*) {
    /**
     * Disable use of secure allocator domains
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("disabledSecureAllocatorDomains", serverGlobalParams.disabledSecureAllocatorDomains);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
