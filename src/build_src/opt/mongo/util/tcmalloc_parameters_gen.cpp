/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/util/tcmalloc_parameters_gen.h --output build/opt/mongo/util/tcmalloc_parameters_gen.cpp src/mongo/util/tcmalloc_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/util/tcmalloc_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
bool HeapProfilingEnabled{false};
#endif
#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
long long HeapProfilingSampleIntervalBytes{262144};
#endif
/**
 * tcmallocEnableMarkThreadTemporarilyIdle: REMOVED: Setting this parameter has no effect and it will be removed in a future version of MongoDB.
 */
Status TCMallocEnableMarkThreadTemporarilyIdle::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

/**
 * tcmallocReleaseRate: Configure tcmalloc_release_rate
 */
Status TCMallocReleaseRateServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

MONGO_SERVER_PARAMETER_REGISTER(idl_7e41a7d3101a9eccf67b3f8da1ec7e7915fb69a5)(InitializerContext*) {
    /**
     * Configure tcmalloc's max_total_thread_cache_bytes
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new TCMallocMaxTotalThreadCacheBytesServerParameter("tcmallocMaxTotalThreadCacheBytes", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * Configure tcmalloc's aggressive_memory_decommit
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        return new TCMallocAggressiveMemoryDecommitServerParameter("tcmallocAggressiveMemoryDecommit", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * Enable Heap Profiling
     */
#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("heapProfilingEnabled", HeapProfilingEnabled);
        return ret;
    })();
#endif

    /**
     * Configure heap profiling sample interval bytes
     */
#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("heapProfilingSampleIntervalBytes", HeapProfilingSampleIntervalBytes);
        return ret;
    })();
#endif

    /**
     * REMOVED: Setting this parameter has no effect and it will be removed in a future version of MongoDB.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        return new TCMallocEnableMarkThreadTemporarilyIdle("tcmallocEnableMarkThreadTemporarilyIdle", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * Configure tcmalloc_release_rate
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        return new TCMallocReleaseRateServerParameter("tcmallocReleaseRate", ServerParameterType::kStartupAndRuntime);
    })();

    return Status::OK();
}
}  // namespace mongo
