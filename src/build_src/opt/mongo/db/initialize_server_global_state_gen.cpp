/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/initialize_server_global_state_gen.h --output build/opt/mongo/db/initialize_server_global_state_gen.cpp src/mongo/db/initialize_server_global_state.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/initialize_server_global_state_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int32_t> gMaxLogAttributeSizeKB{logv2::constants::kDefaultMaxAttributeOutputSizeKB};
/**
 * honorSystemUmask: Use the system provided umask, rather than overriding with processUmask config value
 */
Status HonorSystemUMaskServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

/**
 * processUmask: Override system umask for group/other permissions when honorSystemUmask is false. User umask is always preserved.

 */
Status ProcessUMaskServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

MONGO_SERVER_PARAMETER_REGISTER(idl_7cb0d331a6ea6e908743e23105ee120202a6e968)(InitializerContext*) {
    /**
     * Max log attribute size in kilobytes
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxLogSizeKB", gMaxLogAttributeSizeKB);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Use the system provided umask, rather than overriding with processUmask config value
     */
#if !defined(_WIN32)
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        return new HonorSystemUMaskServerParameter("honorSystemUmask", ServerParameterType::kStartupOnly);
    })();
#endif

    /**
     * Override system umask for group/other permissions when honorSystemUmask is false. User umask is always preserved.
    
     */
#if !defined(_WIN32)
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        return new ProcessUMaskServerParameter("processUmask", ServerParameterType::kStartupOnly);
    })();
#endif

    return Status::OK();
}
}  // namespace mongo
