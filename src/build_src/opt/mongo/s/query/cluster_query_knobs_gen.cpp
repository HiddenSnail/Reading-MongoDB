/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/query/cluster_query_knobs_gen.h --output build/opt/mongo/s/query/cluster_query_knobs_gen.cpp src/mongo/s/query/cluster_query_knobs.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/query/cluster_query_knobs_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> internalQueryAlwaysMergeOnPrimaryShard{false};
AtomicWord<bool> internalQueryProhibitMergingOnMongoS{false};
AtomicWord<bool> internalQueryDisableExchange{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_84e42c5cf081c4abd41d2e3bccc88792de5ef166)(InitializerContext*) {
    /**
     * If set to true on mongos, all aggregations delivered to the mongos which require a merging shard will select the primary shard as the merger. False by default, which means that the merging shard will be selected randomly amongst the shards participating in the query. Pipelines capable of merging on mongoS are unaffected by this setting, unless internalQueryProhibitMergingOnMongoS is true.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryAlwaysMergeOnPrimaryShard", internalQueryAlwaysMergeOnPrimaryShard);
        return ret;
    })();

    /**
     * If set to true on mongos, all aggregations which could otherwise merge on the mongos will be obliged to merge on a shard instead. Pipelines which are redirected to the shards will obey the value of internalQueryAlwaysMergeOnPrimaryShard. False by default, meaning that pipelines capable of merging on mongoS will always do so.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryProhibitMergingOnMongoS", internalQueryProhibitMergingOnMongoS);
        return ret;
    })();

    /**
     * If set to true on mongos then the cluster query planner will not produce plans with the exchange. False by default, so the queries run with exchanges.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryDisableExchange", internalQueryDisableExchange);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
