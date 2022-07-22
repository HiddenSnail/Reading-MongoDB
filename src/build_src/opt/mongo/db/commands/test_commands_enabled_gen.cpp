/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/test_commands_enabled_gen.h --output build/opt/mongo/db/commands/test_commands_enabled_gen.cpp src/mongo/db/commands/test_commands_enabled.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/test_commands_enabled_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool gEnableTestCommands{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_1cedc99fabceb96b870a29891061a9399ce9881d)(InitializerContext*) {
    /**
     * Enable test commands
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("enableTestCommands", gEnableTestCommands);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
