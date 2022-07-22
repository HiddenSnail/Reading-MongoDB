/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/query_knobs_gen.h --output build/opt/mongo/db/query/query_knobs_gen.cpp src/mongo/db/query/query_knobs.idl
 */

#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/data_range.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/simple_bsonobj_comparator.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/platform/atomic_proxy.h"
#include "mongo/platform/atomic_word.h"
#include "mongo/rpc/op_msg.h"

namespace mongo {

constexpr auto kInternalQueryPlanEvaluationWorksDefault = 10000;

extern AtomicWord<int> internalQueryPlanEvaluationWorks;
constexpr auto kInternalQueryPlanEvaluationCollFractionDefault = 0.3;

extern AtomicDouble internalQueryPlanEvaluationCollFraction;
constexpr auto kInternalQueryPlanEvaluationMaxResultsDefault = 101;

extern AtomicWord<int> internalQueryPlanEvaluationMaxResults;
constexpr auto kInternalQueryForceIntersectionPlansDefault = false;

extern AtomicWord<bool> internalQueryForceIntersectionPlans;
constexpr auto kInternalQueryPlannerEnableIndexIntersectionDefault = true;

extern AtomicWord<bool> internalQueryPlannerEnableIndexIntersection;
constexpr auto kInternalQueryPlannerEnableHashIntersectionDefault = false;

extern AtomicWord<bool> internalQueryPlannerEnableHashIntersection;
constexpr auto kInternalQueryCacheSizeDefault = 5000;

extern AtomicWord<int> internalQueryCacheSize;
constexpr auto kInternalQueryCacheFeedbacksStoredDefault = 20;

extern AtomicWord<int> internalQueryCacheFeedbacksStored;
constexpr auto kInternalQueryCacheEvictionRatioDefault = 10.0;

extern AtomicDouble internalQueryCacheEvictionRatio;
constexpr auto kInternalQueryCacheWorksGrowthCoefficientDefault = 2.0;

extern AtomicDouble internalQueryCacheWorksGrowthCoefficient;
constexpr auto kInternalQueryCacheDisableInactiveEntriesDefault = false;

extern AtomicWord<bool> internalQueryCacheDisableInactiveEntries;
constexpr auto kInternalQueryPlannerMaxIndexedSolutionsDefault = 64;

extern AtomicWord<int> internalQueryPlannerMaxIndexedSolutions;
constexpr auto kInternalQueryEnumerationMaxOrSolutionsDefault = 10;

extern AtomicWord<int> internalQueryEnumerationMaxOrSolutions;
constexpr auto kInternalQueryEnumerationMaxIntersectPerAndDefault = 3;

extern AtomicWord<int> internalQueryEnumerationMaxIntersectPerAnd;
constexpr auto kInternalQueryPlanOrChildrenIndependentlyDefault = true;

extern AtomicWord<bool> internalQueryPlanOrChildrenIndependently;
constexpr auto kInternalQueryMaxScansToExplodeDefault = 200;

extern AtomicWord<int> internalQueryMaxScansToExplode;
constexpr auto kInternalQueryPlannerGenerateCoveredWholeIndexScansDefault = false;

extern AtomicWord<bool> internalQueryPlannerGenerateCoveredWholeIndexScans;
constexpr auto kInternalQueryIgnoreUnknownJSONSchemaKeywordsDefault = false;

extern AtomicWord<bool> internalQueryIgnoreUnknownJSONSchemaKeywords;
constexpr auto kInternalQueryMaxBlockingSortMemoryUsageBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> internalQueryMaxBlockingSortMemoryUsageBytes;
constexpr auto kInternalQueryExecYieldIterationsDefault = 1000;

extern AtomicWord<int> internalQueryExecYieldIterations;
constexpr auto kInternalQueryExecYieldPeriodMSDefault = 10;

extern AtomicWord<int> internalQueryExecYieldPeriodMS;
constexpr auto kInternalQueryFacetBufferSizeBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> internalQueryFacetBufferSizeBytes;
constexpr auto kInternalLookupStageIntermediateDocumentMaxSizeBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<long long> internalLookupStageIntermediateDocumentMaxSizeBytes;
constexpr auto kInternalDocumentSourceGroupMaxMemoryBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<long long> internalDocumentSourceGroupMaxMemoryBytes;
extern AtomicWord<int> internalInsertMaxBatchSize;
constexpr auto kInternalDocumentSourceCursorBatchSizeBytesDefault = 4 * 1024 * 1024;

extern AtomicWord<int> internalDocumentSourceCursorBatchSizeBytes;
constexpr auto kInternalDocumentSourceLookupCacheSizeBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> internalDocumentSourceLookupCacheSizeBytes;
constexpr auto kInternalQueryProhibitBlockingMergeOnMongoSDefault = false;

extern AtomicWord<bool> internalQueryProhibitBlockingMergeOnMongoS;
constexpr auto kInternalQueryAllowShardedLookupDefault = false;

extern AtomicWord<bool> internalQueryAllowShardedLookup;
constexpr auto kInternalQueryMaxJsEmitBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> internalQueryMaxJsEmitBytes;
constexpr auto kInternalQueryMaxPushBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> internalQueryMaxPushBytes;
constexpr auto kInternalQueryMaxAddToSetBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> internalQueryMaxAddToSetBytes;
constexpr auto kInternalQueryJavaScriptHeapSizeLimitMBDefault = 100;

extern AtomicWord<int> internalQueryJavaScriptHeapSizeLimitMB;
constexpr auto kInternalQueryJavaScriptFnTimeoutMillisDefault = 60 * 1000;

extern AtomicWord<int> internalQueryJavaScriptFnTimeoutMillis;
constexpr auto kInternalQueryDesugarWhereToFunctionDefault = false;

extern AtomicWord<bool> internalQueryDesugarWhereToFunction;
}  // namespace mongo
