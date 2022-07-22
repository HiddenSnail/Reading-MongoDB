/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/transport/service_executor_gen.h --output build/opt/mongo/transport/service_executor_gen.cpp src/mongo/transport/service_executor.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/transport/service_executor_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {
namespace transport {

AtomicWord<int> synchronousServiceExecutorRecursionLimit{8};
AtomicWord<int> adaptiveServiceExecutorReservedThreads{-1};
AtomicWord<int> adaptiveServiceExecutorRunTimeMillis{5000};
AtomicWord<int> adaptiveServiceExecutorRunTimeJitterMillis{500};
AtomicWord<int> adaptiveServiceExecutorStuckThreadTimeoutMillis{250};
AtomicWord<int> adaptiveServiceExecutorMaxQueueLatencyMicros{500};
AtomicWord<int> adaptiveServiceExecutorIdlePctThreshold{60};
AtomicWord<int> adaptiveServiceExecutorRecursionLimit{8};
AtomicWord<int> reservedServiceExecutorRecursionLimit{8};
MONGO_SERVER_PARAMETER_REGISTER(idl_7ffdacdacfd04bf86d553bc3bd3458d88773deb4)(InitializerContext*) {
    /**
     * Tasks may recurse further if their recursion depth is less than this value.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("synchronousServiceExecutorRecursionLimit", synchronousServiceExecutorRecursionLimit);
        return ret;
    })();

    /**
     * The executor will always keep this many threads around. If the value is -1, then it will be set to number of cores / 2.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorReservedThreads", adaptiveServiceExecutorReservedThreads);
        return ret;
    })();

    /**
     * Each worker thread will allow ASIO to run for this many milliseconds before checking whether it should exit.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorRunTimeMillis", adaptiveServiceExecutorRunTimeMillis);
        return ret;
    })();

    /**
     * The period that a worker thread runs will be offset by at most this value so that not all threads are scheduled at the same time.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorRunTimeJitterMillis", adaptiveServiceExecutorRunTimeJitterMillis);
        return ret;
    })();

    /**
     * The maximum amount of time the controller thread will sleep before doing any stuck-thread detection.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorStuckThreadTimeoutMillis", adaptiveServiceExecutorStuckThreadTimeoutMillis);
        return ret;
    })();

    /**
     * The maximum allowed latency between when a task is scheduled and a thread is started to service it.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorMaxQueueLatencyMicros", adaptiveServiceExecutorMaxQueueLatencyMicros);
        return ret;
    })();

    /**
     * The percent boundary of work time over run time below which worker threads will exit.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorIdlePctThreshold", adaptiveServiceExecutorIdlePctThreshold);
        return ret;
    })();

    /**
     * Tasks may recurse further if their recursion depth is less than this value.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("adaptiveServiceExecutorRecursionLimit", adaptiveServiceExecutorRecursionLimit);
        return ret;
    })();

    /**
     * Tasks may recurse further if their recursion depth is less than this value.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_8 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("reservedServiceExecutorRecursionLimit", reservedServiceExecutorRecursionLimit);
        return ret;
    })();

    return Status::OK();
}
}  // namespace transport
}  // namespace mongo
