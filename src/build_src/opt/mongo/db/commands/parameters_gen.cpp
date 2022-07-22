/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/parameters_gen.h --output build/opt/mongo/db/commands/parameters_gen.cpp src/mongo/db/commands/parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

MONGO_SERVER_PARAMETER_REGISTER(idl_bbbe7c9d9b0ab52d9cad6eeff4857bac156d2b45)(InitializerContext*) {
    /**
     * Sets quiet logging mode
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("quiet", serverGlobalParams.quiet);
        return ret;
    })();

    /**
     * Configures mongod to log full code stack traces for every database and socket C++ exception
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kRuntimeOnly>("traceExceptions", DBException::traceExceptions);
        return ret;
    })();

    /**
     * Specifies the verbosity of logging
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        return new LogLevelServerParameter("logLevel", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * Sets the verbosity levels of various components for log messages
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        return new LogComponentVerbosityServerParameter("logComponentVerbosity", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * automationServiceDescriptor
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        return new AutomationServiceDescriptorServerParameter("automationServiceDescriptor", ServerParameterType::kStartupAndRuntime);
    })();

    return Status::OK();
}
}  // namespace mongo
