/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/scripting/mozjs/engine_gen.h --output build/opt/mongo/scripting/mozjs/engine_gen.cpp src/mongo/scripting/mozjs/engine.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/scripting/mozjs/engine_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gDisableJavaScriptJIT{true};
AtomicWord<bool> gJavascriptProtection{false};
AtomicWord<int> gJSHeapLimitMB{1100};
MONGO_SERVER_PARAMETER_REGISTER(idl_7e061f03f93b157be49f3f9964a343bd7376e5d5)(InitializerContext*) {
    /**
     * disable the Javascript Just In Time compiler
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("disableJavaScriptJIT", gDisableJavaScriptJIT);
        return ret;
    })();

    /**
     * disable automatic JavaScript function marshalling
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("javascriptProtection", gJavascriptProtection);
        return ret;
    })();

    /**
     * set the js scope's heap size limit
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("jsHeapLimitMB", gJSHeapLimitMB);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
