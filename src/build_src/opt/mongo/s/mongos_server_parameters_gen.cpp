/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/mongos_server_parameters_gen.h --output build/opt/mongo/s/mongos_server_parameters_gen.cpp src/mongo/s/mongos_server_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/mongos_server_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool gLoadRoutingTableOnStartup{true};
bool gWarmMinConnectionsInShardingTaskExecutorPoolOnStartup{true};
int gWarmMinConnectionsInShardingTaskExecutorPoolOnStartupWaitMS{2000};
/**
 * readHedgingMode: Enables hedged reads.
 */
Status HedgingModeServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

AtomicWord<int> gMaxTimeMSForHedgedReads{150};
bool gEnableFinerGrainedCatalogCacheRefresh{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_509541ad8d8ee10b0466f3f7708c64a1309b5620)(InitializerContext*) {
    /**
     * Enables precaching of the mongos routing table on startup.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("loadRoutingTableOnStartup", gLoadRoutingTableOnStartup);
        return ret;
    })();

    /**
     * Enables prewarming of the connection pool.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("warmMinConnectionsInShardingTaskExecutorPoolOnStartup", gWarmMinConnectionsInShardingTaskExecutorPoolOnStartup);
        return ret;
    })();

    /**
     * How long to wait for all hosts to have at least one connection.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("warmMinConnectionsInShardingTaskExecutorPoolOnStartupWaitMS", gWarmMinConnectionsInShardingTaskExecutorPoolOnStartupWaitMS);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Enables hedged reads.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        return new HedgingModeServerParameter("readHedgingMode", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * The maxTimeMS to set on hedged reads.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxTimeMSForHedgedReads", gMaxTimeMSForHedgedReads);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Enables the finer grained catalog cache refresh behavior.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("enableFinerGrainedCatalogCacheRefresh", gEnableFinerGrainedCatalogCacheRefresh);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
