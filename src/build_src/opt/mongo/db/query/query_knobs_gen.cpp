/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/query_knobs_gen.h --output build/opt/mongo/db/query/query_knobs_gen.cpp src/mongo/db/query/query_knobs.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/query/query_knobs_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> internalQueryPlanEvaluationWorks{10000};
AtomicDouble internalQueryPlanEvaluationCollFraction{0.3};
AtomicWord<int> internalQueryPlanEvaluationMaxResults{101};
AtomicWord<bool> internalQueryForceIntersectionPlans{false};
AtomicWord<bool> internalQueryPlannerEnableIndexIntersection{true};
AtomicWord<bool> internalQueryPlannerEnableHashIntersection{false};
AtomicWord<int> internalQueryCacheSize{5000};
AtomicWord<int> internalQueryCacheFeedbacksStored{20};
AtomicDouble internalQueryCacheEvictionRatio{10.0};
AtomicDouble internalQueryCacheWorksGrowthCoefficient{2.0};
AtomicWord<bool> internalQueryCacheDisableInactiveEntries{false};
AtomicWord<int> internalQueryPlannerMaxIndexedSolutions{64};
AtomicWord<int> internalQueryEnumerationMaxOrSolutions{10};
AtomicWord<int> internalQueryEnumerationMaxIntersectPerAnd{3};
AtomicWord<bool> internalQueryPlanOrChildrenIndependently{true};
AtomicWord<int> internalQueryMaxScansToExplode{200};
AtomicWord<bool> internalQueryPlannerGenerateCoveredWholeIndexScans{false};
AtomicWord<bool> internalQueryIgnoreUnknownJSONSchemaKeywords{false};
AtomicWord<int> internalQueryMaxBlockingSortMemoryUsageBytes{100 * 1024 * 1024};
AtomicWord<int> internalQueryExecYieldIterations{1000};
AtomicWord<int> internalQueryExecYieldPeriodMS{10};
AtomicWord<int> internalQueryFacetBufferSizeBytes{100 * 1024 * 1024};
AtomicWord<long long> internalLookupStageIntermediateDocumentMaxSizeBytes{100 * 1024 * 1024};
AtomicWord<long long> internalDocumentSourceGroupMaxMemoryBytes{100 * 1024 * 1024};
AtomicWord<int> internalInsertMaxBatchSize{internalQueryExecYieldIterations.load() / 2};
AtomicWord<int> internalDocumentSourceCursorBatchSizeBytes{4 * 1024 * 1024};
AtomicWord<int> internalDocumentSourceLookupCacheSizeBytes{100 * 1024 * 1024};
AtomicWord<bool> internalQueryProhibitBlockingMergeOnMongoS{false};
AtomicWord<bool> internalQueryAllowShardedLookup{false};
AtomicWord<int> internalQueryMaxJsEmitBytes{100 * 1024 * 1024};
AtomicWord<int> internalQueryMaxPushBytes{100 * 1024 * 1024};
AtomicWord<int> internalQueryMaxAddToSetBytes{100 * 1024 * 1024};
AtomicWord<int> internalQueryJavaScriptHeapSizeLimitMB{100};
AtomicWord<int> internalQueryJavaScriptFnTimeoutMillis{60 * 1000};
AtomicWord<bool> internalQueryDesugarWhereToFunction{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_aa629c1d5e7ce5614728bd1d4f3e04442ceffe4e)(InitializerContext*) {
    /**
     * Max number of times we call work() on plans before comparing them, for small collections.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlanEvaluationWorks", internalQueryPlanEvaluationWorks);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * For large collections, the number times we work() candidate plans is taken as this fraction of the collection size.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlanEvaluationCollFraction", internalQueryPlanEvaluationCollFraction);
        ret->addBound<idl_server_parameter_detail::LTE>(1.0);
        ret->addBound<idl_server_parameter_detail::GTE>(0.0);
        return ret;
    })();

    /**
     * Stop working plans once a plan returns this many results.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlanEvaluationMaxResults", internalQueryPlanEvaluationMaxResults);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Do we give a big ranking bonus to intersection plans?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryForceIntersectionPlans", internalQueryForceIntersectionPlans);
        return ret;
    })();

    /**
     * Do we have ixisect on at all?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlannerEnableIndexIntersection", internalQueryPlannerEnableIndexIntersection);
        return ret;
    })();

    /**
     * Do we use hash-based intersection for rooted $and queries?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlannerEnableHashIntersection", internalQueryPlannerEnableHashIntersection);
        return ret;
    })();

    /**
     * How many entries in the cache?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryCacheSize", internalQueryCacheSize);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * How many feedback entries do we collect before possibly evicting from the cache based on bad performance?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryCacheFeedbacksStored", internalQueryCacheFeedbacksStored);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * How many times more works must we perform in order to justify plan cache eviction and replanning?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_8 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryCacheEvictionRatio", internalQueryCacheEvictionRatio);
        ret->addBound<idl_server_parameter_detail::GTE>(0.0);
        return ret;
    })();

    /**
     * How quickly the the 'works' value in an inactive cache entry will grow. It grows exponentially. The value of this server parameter is the base.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_9 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryCacheWorksGrowthCoefficient", internalQueryCacheWorksGrowthCoefficient);
        ret->addBound<idl_server_parameter_detail::GT>(1.0);
        return ret;
    })();

    /**
     * Whether or not cache entries can be marked as 'inactive'.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_10 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryCacheDisableInactiveEntries", internalQueryCacheDisableInactiveEntries);
        return ret;
    })();

    /**
     * How many indexed solutions will QueryPlanner::plan output?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_11 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlannerMaxIndexedSolutions", internalQueryPlannerMaxIndexedSolutions);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * How many solutions will the enumerator consider at each OR?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_12 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryEnumerationMaxOrSolutions", internalQueryEnumerationMaxOrSolutions);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * How many intersections will the enumerator consider at each AND?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_13 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryEnumerationMaxIntersectPerAnd", internalQueryEnumerationMaxIntersectPerAnd);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Do we want to plan each child of the OR independently?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_14 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlanOrChildrenIndependently", internalQueryPlanOrChildrenIndependently);
        return ret;
    })();

    /**
     * How many index scans are we willing to produce in order to obtain a sort order during explodeForSort?
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_15 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryMaxScansToExplode", internalQueryMaxScansToExplode);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Allow the planner to generate covered whole index scans, rather than falling back to a COLLSCAN.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_16 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryPlannerGenerateCoveredWholeIndexScans", internalQueryPlannerGenerateCoveredWholeIndexScans);
        return ret;
    })();

    /**
     * Ignore unknown JSON Schema keywords.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_17 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryIgnoreUnknownJSONSchemaKeywords", internalQueryIgnoreUnknownJSONSchemaKeywords);
        return ret;
    })();

    /**
     * The maximum amount of memory a query (e.g. a find or aggregate command) is willing to use to execute a blocking sort, measured in bytes. If disk use is allowed, then it may be possible to sort more data, but this limit will still constrain the memory consumption.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_18 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryMaxBlockingSortMemoryUsageBytes", internalQueryMaxBlockingSortMemoryUsageBytes);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Yield after this many "should yield?" checks.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_19 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryExecYieldIterations", internalQueryExecYieldIterations);
        return ret;
    })();

    /**
     * Yield if it's been at least this many milliseconds since we last yielded.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_20 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryExecYieldPeriodMS", internalQueryExecYieldPeriodMS);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * The number of bytes to buffer at once during a $facet stage.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_21 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryFacetBufferSizeBytes", internalQueryFacetBufferSizeBytes);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Maximum size of the result set that we cache from the foreign collection during a $lookup.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_22 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalLookupStageIntermediateDocumentMaxSizeBytes", internalLookupStageIntermediateDocumentMaxSizeBytes);
        ret->addBound<idl_server_parameter_detail::GTE>(([]{ constexpr auto value = BSONObjMaxInternalSize; return value; })());
        return ret;
    })();

    /**
     * Maximum size of the data that the $group aggregation stage will cache in-memory before spilling to disk.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_23 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalDocumentSourceGroupMaxMemoryBytes", internalDocumentSourceGroupMaxMemoryBytes);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Maximum number of documents that we will insert in a single batch.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_24 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalInsertMaxBatchSize", internalInsertMaxBatchSize);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Maximum amount of data that DocumentSourceCursor will cache from the underlying PlanExecutor before pipeline processing.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_25 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalDocumentSourceCursorBatchSizeBytes", internalDocumentSourceCursorBatchSizeBytes);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Maximum amount of non-correlated foreign-collection data that the $lookup stage will cache before abandoning the cache and executing the full pipeline on each iteration.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_26 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalDocumentSourceLookupCacheSizeBytes", internalDocumentSourceLookupCacheSizeBytes);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * If true, blocking stages such as $group or non-merging $sort will be prohibited from running on mongoS.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_27 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryProhibitBlockingMergeOnMongoS", internalQueryProhibitBlockingMergeOnMongoS);
        return ret;
    })();

    /**
     * If true, activates the incomplete sharded $lookup feature.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_28 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryAllowShardedLookup", internalQueryAllowShardedLookup);
        return ret;
    })();

    /**
     * Limits the vector of values emitted from a single document's call to JsEmit to the given size in bytes.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_29 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryMaxJsEmitBytes", internalQueryMaxJsEmitBytes);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Limits the vector of values pushed into a single array while grouping with the $push accumulator.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_30 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryMaxPushBytes", internalQueryMaxPushBytes);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Limits the vector of values pushed into a single array while grouping with the $addToSet accumulator.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_31 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryMaxAddToSetBytes", internalQueryMaxAddToSetBytes);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Limits the JavaScript heap size used in aggregation. Will defer to the global 'jsHeapLimitMB' limit if the global limit is smaller.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_32 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryJavaScriptHeapSizeLimitMB", internalQueryJavaScriptHeapSizeLimitMB);
        return ret;
    })();

    /**
     * Limits the maximum allowed time a user-defined javascript function can run in a query.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_33 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryJavaScriptFnTimeoutMillis", internalQueryJavaScriptFnTimeoutMillis);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * When true, desugars $where to $expr/$function.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_34 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryDesugarWhereToFunction", internalQueryDesugarWhereToFunction);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
