/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/util/password_params_gen.h --output build/opt/mongo/util/password_params_gen.cpp src/mongo/util/password_params.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/util/password_params_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool newLineAfterPasswordPromptForTest{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_401cf46619f3449c9a9e481d87a615ba17ee763c)(InitializerContext*) {
    /**
     * The askPassword function will print a new line after the "Enter password:" prompt so that tests see the prompt in log output immediately.
    
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("newLineAfterPasswordPromptForTest", newLineAfterPasswordPromptForTest);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
