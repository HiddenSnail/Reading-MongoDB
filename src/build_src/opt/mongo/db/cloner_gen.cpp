/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/cloner_gen.h --output build/opt/mongo/db/cloner_gen.cpp src/mongo/db/cloner.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/cloner_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gSkipCorruptDocumentsWhenCloning;
MONGO_SERVER_PARAMETER_REGISTER(idl_36d28736d35580e6e74eb5c6ba4521de1ae46d76)(InitializerContext*) {
    /**
     * Skip corrupt documents when cloning
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("skipCorruptDocumentsWhenCloning", gSkipCorruptDocumentsWhenCloning);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
