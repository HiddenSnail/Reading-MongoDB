/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kQuery

#include "mongo/platform/basic.h"

#include "mongo/db/query/get_executor.h"

#include <boost/optional.hpp>
#include <limits>
#include <memory>

#include "mongo/base/error_codes.h"
#include "mongo/base/parse_number.h"
#include "mongo/db/catalog/index_catalog.h"
#include "mongo/db/exec/cached_plan.h"
#include "mongo/db/exec/collection_scan.h"
#include "mongo/db/exec/count.h"
#include "mongo/db/exec/delete.h"
#include "mongo/db/exec/eof.h"
#include "mongo/db/exec/idhack.h"
#include "mongo/db/exec/multi_plan.h"
#include "mongo/db/exec/projection.h"
#include "mongo/db/exec/projection_executor_utils.h"
#include "mongo/db/exec/record_store_fast_count.h"
#include "mongo/db/exec/return_key.h"
#include "mongo/db/exec/shard_filter.h"
#include "mongo/db/exec/sort_key_generator.h"
#include "mongo/db/exec/subplan.h"
#include "mongo/db/exec/update_stage.h"
#include "mongo/db/exec/upsert_stage.h"
#include "mongo/db/index/index_descriptor.h"
#include "mongo/db/index/wildcard_access_method.h"
#include "mongo/db/index_names.h"
#include "mongo/db/matcher/extensions_callback_noop.h"
#include "mongo/db/matcher/extensions_callback_real.h"
#include "mongo/db/query/canonical_query.h"
#include "mongo/db/query/canonical_query_encoder.h"
#include "mongo/db/query/collation/collator_factory_interface.h"
#include "mongo/db/query/collection_query_info.h"
#include "mongo/db/query/explain.h"
#include "mongo/db/query/index_bounds_builder.h"
#include "mongo/db/query/internal_plans.h"
#include "mongo/db/query/plan_cache.h"
#include "mongo/db/query/plan_executor.h"
#include "mongo/db/query/planner_access.h"
#include "mongo/db/query/planner_analysis.h"
#include "mongo/db/query/planner_ixselect.h"
#include "mongo/db/query/planner_wildcard_helpers.h"
#include "mongo/db/query/projection_parser.h"
#include "mongo/db/query/query_knobs_gen.h"
#include "mongo/db/query/query_planner.h"
#include "mongo/db/query/query_planner_common.h"
#include "mongo/db/query/query_settings.h"
#include "mongo/db/query/stage_builder.h"
#include "mongo/db/repl/optime.h"
#include "mongo/db/repl/replication_coordinator.h"
#include "mongo/db/s/collection_sharding_state.h"
#include "mongo/db/s/operation_sharding_state.h"
#include "mongo/db/server_options.h"
#include "mongo/db/service_context.h"
#include "mongo/db/storage/storage_options.h"
#include "mongo/logv2/log.h"
#include "mongo/scripting/engine.h"
#include "mongo/util/str.h"

namespace mongo {

using std::string;
using std::unique_ptr;
using std::vector;

boost::intrusive_ptr<ExpressionContext> makeExpressionContextForGetExecutor(
    OperationContext* opCtx, const BSONObj& requestCollation, const NamespaceString& nss) {
    invariant(opCtx);

    auto expCtx = make_intrusive<ExpressionContext>(opCtx, nullptr, nss);
    if (!requestCollation.isEmpty()) {
        auto statusWithCollator = CollatorFactoryInterface::get(expCtx->opCtx->getServiceContext())
                                      ->makeFromBSON(requestCollation);
        expCtx->setCollator(uassertStatusOK(std::move(statusWithCollator)));
    }
    return expCtx;
}

// static
void filterAllowedIndexEntries(const AllowedIndicesFilter& allowedIndicesFilter,
                               std::vector<IndexEntry>* indexEntries) {
    invariant(indexEntries);

    // Filter index entries
    // Check BSON objects in AllowedIndices::_indexKeyPatterns against IndexEntry::keyPattern.
    // Removes IndexEntrys that do not match _indexKeyPatterns.
    std::vector<IndexEntry> temp;
    for (std::vector<IndexEntry>::const_iterator i = indexEntries->begin();
         i != indexEntries->end();
         ++i) {
        const IndexEntry& indexEntry = *i;
        if (allowedIndicesFilter.allows(indexEntry)) {
            // Copy index entry into temp vector if found in query settings.
            temp.push_back(indexEntry);
        }
    }

    // Update results.
    temp.swap(*indexEntries);
}

namespace {
namespace wcp = ::mongo::wildcard_planning;
// The body is below in the "count hack" section but getExecutor calls it.
bool turnIxscanIntoCount(QuerySolution* soln);
}  // namespace

bool isAnyComponentOfPathMultikey(const BSONObj& indexKeyPattern,
                                  bool isMultikey,
                                  const MultikeyPaths& indexMultikeyInfo,
                                  StringData path) {
    if (!isMultikey) {
        return false;
    }

    size_t keyPatternFieldIndex = 0;
    bool found = false;
    if (indexMultikeyInfo.empty()) {
        // There is no path-level multikey information available, so we must assume 'path' is
        // multikey.
        return true;
    }

    for (auto&& elt : indexKeyPattern) {
        if (elt.fieldNameStringData() == path) {
            found = true;
            break;
        }
        keyPatternFieldIndex++;
    }
    invariant(found);

    invariant(indexMultikeyInfo.size() > keyPatternFieldIndex);
    return !indexMultikeyInfo[keyPatternFieldIndex].empty();
}

IndexEntry indexEntryFromIndexCatalogEntry(OperationContext* opCtx,
                                           const IndexCatalogEntry& ice,
                                           const CanonicalQuery* canonicalQuery) {
    auto desc = ice.descriptor();
    invariant(desc);

    auto accessMethod = ice.accessMethod();
    invariant(accessMethod);

    const bool isMultikey = desc->isMultikey();

    const WildcardProjection* wildcardProjection = nullptr;
    std::set<FieldRef> multikeyPathSet;
    if (desc->getIndexType() == IndexType::INDEX_WILDCARD) {
        wildcardProjection =
            static_cast<const WildcardAccessMethod*>(accessMethod)->getWildcardProjection();
        if (isMultikey) {
            MultikeyMetadataAccessStats mkAccessStats;

            if (canonicalQuery) {
                stdx::unordered_set<std::string> fields;
                QueryPlannerIXSelect::getFields(canonicalQuery->root(), &fields);
                const auto projectedFields = projection_executor_utils::applyProjectionToFields(
                    wildcardProjection->exec(), fields);

                multikeyPathSet =
                    accessMethod->getMultikeyPathSet(opCtx, projectedFields, &mkAccessStats);
            } else {
                multikeyPathSet = accessMethod->getMultikeyPathSet(opCtx, &mkAccessStats);
            }

            LOGV2_DEBUG(20920,
                        2,
                        "Multikey path metadata range index scan stats: {{ index: {index}, "
                        "numSeeks: {numSeeks}, keysExamined: {keysExamined}}}",
                        "Multikey path metadata range index scan stats",
                        "index"_attr = desc->indexName(),
                        "numSeeks"_attr = mkAccessStats.keysExamined,
                        "keysExamined"_attr = mkAccessStats.keysExamined);
        }
    }

    return {desc->keyPattern(),
            desc->getIndexType(),
            isMultikey,
            // The fixed-size vector of multikey paths stored in the index catalog.
            ice.getMultikeyPaths(opCtx),
            // The set of multikey paths from special metadata keys stored in the index itself.
            // Indexes that have these metadata keys do not store a fixed-size vector of multikey
            // metadata in the index catalog. Depending on the index type, an index uses one of
            // these mechanisms (or neither), but not both.
            multikeyPathSet,
            desc->isSparse(),
            desc->unique(),
            IndexEntry::Identifier{desc->indexName()},
            ice.getFilterExpression(),
            desc->infoObj(),
            ice.getCollator(),
            wildcardProjection};
}

/**
 * If query supports index filters, filter params.indices according to any index filters that have
 * been configured. In addition, sets that there were indeed index filters applied.
 */
void applyIndexFilters(Collection* collection,
                       const CanonicalQuery& canonicalQuery,
                       QueryPlannerParams* plannerParams) {
    if (!IDHackStage::supportsQuery(collection, canonicalQuery)) {
        QuerySettings* querySettings = CollectionQueryInfo::get(collection).getQuerySettings();
        const auto key = canonicalQuery.encodeKey();

        // Filter index catalog if index filters are specified for query.
        // Also, signal to planner that application hint should be ignored.
        if (boost::optional<AllowedIndicesFilter> allowedIndicesFilter =
                querySettings->getAllowedIndicesFilter(key)) {
            filterAllowedIndexEntries(*allowedIndicesFilter, &plannerParams->indices);
            plannerParams->indexFiltersApplied = true;
        }
    }
}

void fillOutPlannerParams(OperationContext* opCtx,
                          Collection* collection,
                          CanonicalQuery* canonicalQuery,
                          QueryPlannerParams* plannerParams) {
    invariant(canonicalQuery);
    // If it's not NULL, we may have indices.  Access the catalog and fill out IndexEntry(s)
    std::unique_ptr<IndexCatalog::IndexIterator> ii =
        collection->getIndexCatalog()->getIndexIterator(opCtx, false);
    while (ii->more()) {
        const IndexCatalogEntry* ice = ii->next();

        // Skip the addition of hidden indexes to prevent use in query planning.
        if (ice->descriptor()->hidden())
            continue;
        plannerParams->indices.push_back(
            indexEntryFromIndexCatalogEntry(opCtx, *ice, canonicalQuery));
    }

    // If query supports index filters, filter params.indices by indices in query settings.
    // Ignore index filters when it is possible to use the id-hack.
    applyIndexFilters(collection, *canonicalQuery, plannerParams);

    // We will not output collection scans unless there are no indexed solutions. NO_TABLE_SCAN
    // overrides this behavior by not outputting a collscan even if there are no indexed
    // solutions.
    if (storageGlobalParams.noTableScan.load()) {
        const auto& nss = canonicalQuery->nss();
        // There are certain cases where we ignore this restriction:
        bool ignore =
            canonicalQuery->getQueryObj().isEmpty() || nss.isSystem() || nss.isOnInternalDb();
        if (!ignore) {
            plannerParams->options |= QueryPlannerParams::NO_TABLE_SCAN;
        }
    }

    // If the caller wants a shard filter, make sure we're actually sharded.
    if (plannerParams->options & QueryPlannerParams::INCLUDE_SHARD_FILTER) {
        auto collDesc =
            CollectionShardingState::get(opCtx, canonicalQuery->nss())->getCollectionDescription();
        if (collDesc.isSharded()) {
            plannerParams->shardKey = collDesc.getKeyPattern();
        } else {
            // If there's no metadata don't bother w/the shard filter since we won't know what
            // the key pattern is anyway...
            plannerParams->options &= ~QueryPlannerParams::INCLUDE_SHARD_FILTER;
        }
    }

    if (internalQueryPlannerEnableIndexIntersection.load()) {
        plannerParams->options |= QueryPlannerParams::INDEX_INTERSECTION;
    }

    if (internalQueryPlannerGenerateCoveredWholeIndexScans.load()) {
        plannerParams->options |= QueryPlannerParams::GENERATE_COVERED_IXSCANS;
    }

    plannerParams->options |= QueryPlannerParams::SPLIT_LIMITED_SORT;

    if (shouldWaitForOplogVisibility(
            opCtx, collection, canonicalQuery->getQueryRequest().isTailable())) {
        plannerParams->options |= QueryPlannerParams::OPLOG_SCAN_WAIT_FOR_VISIBLE;
    }
}

bool shouldWaitForOplogVisibility(OperationContext* opCtx,
                                  const Collection* collection,
                                  bool tailable) {

    // Only non-tailable cursors on the oplog are affected. Only forward cursors, not reverse
    // cursors, are affected, but this is checked when the cursor is opened.
    if (!collection->ns().isOplog() || tailable) {
        return false;
    }

    // Only primaries should require readers to wait for oplog visibility. In any other replication
    // state, readers read at the most visible oplog timestamp. The reason why readers on primaries
    // need to wait is because multiple optimes can be allocated for operations before their entries
    // are written to the storage engine. "Holes" will appear when an operation with a later optime
    // commits before an operation with an earlier optime, and readers should wait so that all data
    // is consistent.
    //
    // Secondaries can't wait for oplog visibility without the PBWM lock because it can introduce a
    // hang while a batch application is in progress. The wait is done while holding a global lock,
    // and the oplog visibility timestamp is updated at the end of every batch on a secondary,
    // signalling the wait to complete. If a replication worker had a global lock and temporarily
    // released it, a reader could acquire the lock to read the oplog. If the secondary reader were
    // to wait for the oplog visibility timestamp to be updated, it would wait for a replication
    // batch that would never complete because it couldn't reacquire its own lock, the global lock
    // held by the waiting reader.
    return repl::ReplicationCoordinator::get(opCtx)->canAcceptWritesForDatabase(opCtx, "admin");
}

namespace {

struct PrepareExecutionResult {
    PrepareExecutionResult(unique_ptr<CanonicalQuery> canonicalQuery,
                           unique_ptr<QuerySolution> querySolution,
                           unique_ptr<PlanStage> root)
        : canonicalQuery(std::move(canonicalQuery)),
          querySolution(std::move(querySolution)),
          root(std::move(root)) {}

    unique_ptr<CanonicalQuery> canonicalQuery;
    unique_ptr<QuerySolution> querySolution;
    unique_ptr<PlanStage> root;
};

/**
 * Build an execution tree for the query described in 'canonicalQuery'.
 *
 * If an execution tree could be created, then returns a PrepareExecutionResult that wraps:
 * - The CanonicalQuery describing the query operation. This may be equal to the original canonical
 *   query, or may be modified. This will never be null.
 * - A QuerySolution, representing the associated query solution. This may be null, in certain
 *   circumstances where the constructed execution tree does not have an associated query solution.
 * - A PlanStage, representing the root of the constructed execution tree. This will never be null.
 *
 * If an execution tree could not be created, returns an error Status.
 */
StatusWith<PrepareExecutionResult> prepareExecution(OperationContext* opCtx,
                                                    Collection* collection,
                                                    WorkingSet* ws,
                                                    unique_ptr<CanonicalQuery> canonicalQuery,
                                                    size_t plannerOptions) {
    invariant(canonicalQuery);
    unique_ptr<PlanStage> root;

    // This can happen as we're called by internal clients as well.
    if (nullptr == collection) {
        const string& ns = canonicalQuery->ns();
        LOGV2_DEBUG(20921,
                    2,
                    "Collection {namespace} does not exist. Using EOF plan: {query}",
                    "Collection does not exist. Using EOF plan",
                    "namespace"_attr = ns,
                    "query"_attr = redact(canonicalQuery->toStringShort()));
        root = std::make_unique<EOFStage>(canonicalQuery->getExpCtx().get());
        return PrepareExecutionResult(std::move(canonicalQuery), nullptr, std::move(root));
    }

    // Fill out the planning params.  We use these for both cached solutions and non-cached.
    QueryPlannerParams plannerParams;
    plannerParams.options = plannerOptions;
    fillOutPlannerParams(opCtx, collection, canonicalQuery.get(), &plannerParams);

    // If the canonical query does not have a user-specified collation and no one has given the
    // CanonicalQuery a collation already, set it from the collection default.
    if (canonicalQuery->getQueryRequest().getCollation().isEmpty() &&
        canonicalQuery->getCollator() == nullptr && collection->getDefaultCollator()) {
        canonicalQuery->setCollator(collection->getDefaultCollator()->clone());
    }

    const IndexDescriptor* descriptor = collection->getIndexCatalog()->findIdIndex(opCtx);

    // If we have an _id index we can use an idhack plan.
    if (descriptor && IDHackStage::supportsQuery(collection, *canonicalQuery)) {
        LOGV2_DEBUG(20922,
                    2,
                    "Using idhack: {canonicalQuery_Short}",
                    "canonicalQuery_Short"_attr = redact(canonicalQuery->toStringShort()));

        root = std::make_unique<IDHackStage>(
            canonicalQuery->getExpCtx().get(), canonicalQuery.get(), ws, descriptor);

        // Might have to filter out orphaned docs.
        if (plannerParams.options & QueryPlannerParams::INCLUDE_SHARD_FILTER) {
            root = std::make_unique<ShardFilterStage>(
                canonicalQuery->getExpCtx().get(),
                CollectionShardingState::get(opCtx, canonicalQuery->nss())
                    ->getOwnershipFilter(opCtx),
                ws,
                std::move(root));
        }

        const auto* cqProjection = canonicalQuery->getProj();

        // Add a SortKeyGeneratorStage if the query requested sortKey metadata.
        if (canonicalQuery->metadataDeps()[DocumentMetadataFields::kSortKey]) {
            root = std::make_unique<SortKeyGeneratorStage>(
                canonicalQuery->getExpCtx().get(),
                std::move(root),
                ws,
                canonicalQuery->getQueryRequest().getSort());
        }

        if (canonicalQuery->getQueryRequest().returnKey()) {
            // If returnKey was requested, add ReturnKeyStage to return only the index keys in the
            // resulting documents. If a projection was also specified, it will be ignored, with
            // the exception the $meta sortKey projection, which can be used along with the
            // returnKey.
            root = std::make_unique<ReturnKeyStage>(
                canonicalQuery->getExpCtx().get(),
                cqProjection
                    ? QueryPlannerCommon::extractSortKeyMetaFieldsFromProjection(*cqProjection)
                    : std::vector<FieldPath>{},
                ws,
                canonicalQuery->getExpCtx()->sortKeyFormat,
                std::move(root));
        } else if (cqProjection) {
            // There might be a projection. The idhack stage will always fetch the full
            // document, so we don't support covered projections. However, we might use the
            // simple inclusion fast path.
            // Stuff the right data into the params depending on what proj impl we use.
            if (!cqProjection->isSimple()) {
                root = std::make_unique<ProjectionStageDefault>(
                    canonicalQuery->getExpCtx(),
                    canonicalQuery->getQueryRequest().getProj(),
                    canonicalQuery->getProj(),
                    ws,
                    std::move(root));
            } else {
                root = std::make_unique<ProjectionStageSimple>(
                    canonicalQuery->getExpCtx().get(),
                    canonicalQuery->getQueryRequest().getProj(),
                    canonicalQuery->getProj(),
                    ws,
                    std::move(root));
            }
        }

        return PrepareExecutionResult(std::move(canonicalQuery), nullptr, std::move(root));
    }

    // Tailable: If the query requests tailable the collection must be capped.
    if (canonicalQuery->getQueryRequest().isTailable()) {
        if (!collection->isCapped()) {
            return Status(ErrorCodes::BadValue,
                          "error processing query: " + canonicalQuery->toString() +
                              " tailable cursor requested on non capped collection");
        }
    }

    // Check that the query should be cached.
    if (CollectionQueryInfo::get(collection).getPlanCache()->shouldCacheQuery(*canonicalQuery)) {
        // Fill in opDebug information.
        const auto planCacheKey =
            CollectionQueryInfo::get(collection).getPlanCache()->computeKey(*canonicalQuery);
        CurOp::get(opCtx)->debug().queryHash =
            canonical_query_encoder::computeHash(planCacheKey.getStableKeyStringData());
        CurOp::get(opCtx)->debug().planCacheKey =
            canonical_query_encoder::computeHash(planCacheKey.toString());

        // Try to look up a cached solution for the query.
        if (auto cs = CollectionQueryInfo::get(collection)
                          .getPlanCache()
                          ->getCacheEntryIfActive(planCacheKey)) {
            // We have a CachedSolution.  Have the planner turn it into a QuerySolution.
            auto statusWithQs = QueryPlanner::planFromCache(*canonicalQuery, plannerParams, *cs);

            if (statusWithQs.isOK()) {
                auto querySolution = std::move(statusWithQs.getValue());
                if ((plannerParams.options & QueryPlannerParams::IS_COUNT) &&
                    turnIxscanIntoCount(querySolution.get())) {
                    LOGV2_DEBUG(20923,
                                2,
                                "Using fast count: {query}",
                                "Using fast count",
                                "query"_attr = redact(canonicalQuery->toStringShort()));
                }

                auto root =
                    StageBuilder::build(opCtx, collection, *canonicalQuery, *querySolution, ws);

                // Add a CachedPlanStage on top of the previous root.
                //
                // 'decisionWorks' is used to determine whether the existing cache entry should
                // be evicted, and the query replanned.
                auto cachedPlanStage =
                    std::make_unique<CachedPlanStage>(canonicalQuery->getExpCtx().get(),
                                                      collection,
                                                      ws,
                                                      canonicalQuery.get(),
                                                      plannerParams,
                                                      cs->decisionWorks,
                                                      std::move(root));
                return PrepareExecutionResult(std::move(canonicalQuery),
                                              std::move(querySolution),
                                              std::move(cachedPlanStage));
            }
        }
    }

    if (internalQueryPlanOrChildrenIndependently.load() &&
        SubplanStage::canUseSubplanning(*canonicalQuery)) {
        LOGV2_DEBUG(20924,
                    2,
                    "Running query as sub-queries: {query}",
                    "Running query as sub-queries",
                    "query"_attr = redact(canonicalQuery->toStringShort()));

        root = std::make_unique<SubplanStage>(
            canonicalQuery->getExpCtx().get(), collection, ws, plannerParams, canonicalQuery.get());
        return PrepareExecutionResult(std::move(canonicalQuery), nullptr, std::move(root));
    }

    auto statusWithSolutions = QueryPlanner::plan(*canonicalQuery, plannerParams);
    if (!statusWithSolutions.isOK()) {
        return statusWithSolutions.getStatus().withContext(
            str::stream() << "error processing query: " << canonicalQuery->toString()
                          << " planner returned error");
    }
    auto solutions = std::move(statusWithSolutions.getValue());
    // The planner should have returned an error status if there are no solutions.
    invariant(solutions.size() > 0);

    // See if one of our solutions is a fast count hack in disguise.
    if (plannerParams.options & QueryPlannerParams::IS_COUNT) {
        for (size_t i = 0; i < solutions.size(); ++i) {
            if (turnIxscanIntoCount(solutions[i].get())) {
                // We're not going to cache anything that's fast count.
                auto root =
                    StageBuilder::build(opCtx, collection, *canonicalQuery, *solutions[i], ws);

                LOGV2_DEBUG(20925,
                            2,
                            "Using fast count: {query}, planSummary: {planSummary}",
                            "Using fast count",
                            "query"_attr = redact(canonicalQuery->toStringShort()),
                            "planSummary"_attr = Explain::getPlanSummary(root.get()));

                return PrepareExecutionResult(
                    std::move(canonicalQuery), std::move(solutions[i]), std::move(root));
            }
        }
    }

    if (1 == solutions.size()) {
        // Only one possible plan.  Run it.  Build the stages from the solution.
        auto root = StageBuilder::build(opCtx, collection, *canonicalQuery, *solutions[0], ws);

        LOGV2_DEBUG(20926,
                    2,
                    "Only one plan is available; it will be run but will not be cached. {query}, "
                    "planSummary: {planSummary}",
                    "Only one plan is available; it will be run but will not be cached",
                    "query"_attr = redact(canonicalQuery->toStringShort()),
                    "planSummary"_attr = Explain::getPlanSummary(root.get()));

        return PrepareExecutionResult(
            std::move(canonicalQuery), std::move(solutions[0]), std::move(root));
    } else {
        // Many solutions. Create a MultiPlanStage to pick the best, update the cache,
        // and so on. The working set will be shared by all candidate plans.
        auto multiPlanStage = std::make_unique<MultiPlanStage>(
            canonicalQuery->getExpCtx().get(), collection, canonicalQuery.get());

        for (size_t ix = 0; ix < solutions.size(); ++ix) {
            if (solutions[ix]->cacheData.get()) {
                solutions[ix]->cacheData->indexFilterApplied = plannerParams.indexFiltersApplied;
            }

            auto nextPlanRoot =
                StageBuilder::build(opCtx, collection, *canonicalQuery, *solutions[ix], ws);

            // Takes ownership of 'nextPlanRoot'.
            multiPlanStage->addPlan(std::move(solutions[ix]), std::move(nextPlanRoot), ws);
        }

        root = std::move(multiPlanStage);
        return PrepareExecutionResult(std::move(canonicalQuery), nullptr, std::move(root));
    }
}

}  // namespace

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutor(
    OperationContext* opCtx,
    Collection* collection,
    unique_ptr<CanonicalQuery> canonicalQuery,
    PlanExecutor::YieldPolicy yieldPolicy,
    size_t plannerOptions) {
    unique_ptr<WorkingSet> ws = std::make_unique<WorkingSet>();
    StatusWith<PrepareExecutionResult> executionResult =
        prepareExecution(opCtx, collection, ws.get(), std::move(canonicalQuery), plannerOptions);
    if (!executionResult.isOK()) {
        return executionResult.getStatus();
    }
    invariant(executionResult.getValue().root);
    // We must have a tree of stages in order to have a valid plan executor, but the query
    // solution may be null.
    return PlanExecutor::make(std::move(executionResult.getValue().canonicalQuery),
                              std::move(ws),
                              std::move(executionResult.getValue().root),
                              collection,
                              yieldPolicy,
                              NamespaceString(),
                              std::move(executionResult.getValue().querySolution));
}

//
// Find
//

namespace {

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> _getExecutorFind(
    OperationContext* opCtx,
    Collection* collection,
    unique_ptr<CanonicalQuery> canonicalQuery,
    PlanExecutor::YieldPolicy yieldPolicy,
    size_t plannerOptions) {

    if (OperationShardingState::isOperationVersioned(opCtx)) {
        plannerOptions |= QueryPlannerParams::INCLUDE_SHARD_FILTER;
    }
    return getExecutor(opCtx, collection, std::move(canonicalQuery), yieldPolicy, plannerOptions);
}

}  // namespace

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorFind(
    OperationContext* opCtx,
    Collection* collection,
    unique_ptr<CanonicalQuery> canonicalQuery,
    bool permitYield,
    size_t plannerOptions) {
    auto yieldPolicy = (permitYield && !opCtx->inMultiDocumentTransaction())
        ? PlanExecutor::YIELD_AUTO
        : PlanExecutor::INTERRUPT_ONLY;
    return _getExecutorFind(
        opCtx, collection, std::move(canonicalQuery), yieldPolicy, plannerOptions);
}

StatusWith<std::unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorLegacyFind(
    OperationContext* opCtx,
    Collection* collection,
    std::unique_ptr<CanonicalQuery> canonicalQuery) {
    return _getExecutorFind(opCtx,
                            collection,
                            std::move(canonicalQuery),
                            PlanExecutor::YIELD_AUTO,
                            QueryPlannerParams::DEFAULT);
}

namespace {

/**
 * Wrap the specified 'root' plan stage in a ProjectionStage. Does not take ownership of any
 * arguments other than root.
 *
 * If the projection was valid, then return Status::OK() with a pointer to the newly created
 * ProjectionStage. Otherwise, return a status indicating the error reason.
 */
StatusWith<unique_ptr<PlanStage>> applyProjection(OperationContext* opCtx,
                                                  const NamespaceString& nsString,
                                                  CanonicalQuery* cq,
                                                  const BSONObj& projObj,
                                                  bool allowPositional,
                                                  WorkingSet* ws,
                                                  unique_ptr<PlanStage> root) {
    invariant(!projObj.isEmpty());

    projection_ast::Projection proj =
        projection_ast::parse(cq->getExpCtx(),
                              projObj.getOwned(),
                              cq->root(),
                              cq->getQueryObj(),
                              ProjectionPolicies::findProjectionPolicies());

    // ProjectionExec requires the MatchDetails from the query expression when the projection
    // uses the positional operator. Since the query may no longer match the newly-updated
    // document, we forbid this case.
    if (!allowPositional && proj.requiresMatchDetails()) {
        return {ErrorCodes::BadValue,
                "cannot use a positional projection and return the new document"};
    }

    cq->requestAdditionalMetadata(proj.metadataDeps());

    // $meta sortKey is not allowed to be projected in findAndModify commands.
    if (cq->metadataDeps()[DocumentMetadataFields::kSortKey]) {
        return {ErrorCodes::BadValue,
                "Cannot use a $meta sortKey projection in findAndModify commands."};
    }

    return {std::make_unique<ProjectionStageDefault>(
        cq->getExpCtx(), projObj, &proj, ws, std::unique_ptr<PlanStage>(root.release()))};
}

}  // namespace

//
// Delete
//

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorDelete(
    OpDebug* opDebug,
    Collection* collection,
    ParsedDelete* parsedDelete,
    boost::optional<ExplainOptions::Verbosity> verbosity) {
    auto expCtx = parsedDelete->expCtx();
    OperationContext* opCtx = expCtx->opCtx;
    const DeleteRequest* request = parsedDelete->getRequest();

    const NamespaceString& nss(request->getNamespaceString());
    if (!request->isGod()) {
        if (nss.isSystem() && opCtx->lockState()->shouldConflictWithSecondaryBatchApplication()) {
            uassert(12050, "cannot delete from system namespace", nss.isLegalClientSystemNS());
        }
    }

    if (collection && collection->isCapped()) {
        return Status(ErrorCodes::IllegalOperation,
                      str::stream() << "cannot remove from a capped collection: " << nss.ns());
    }

    bool userInitiatedWritesAndNotPrimary = opCtx->writesAreReplicated() &&
        !repl::ReplicationCoordinator::get(opCtx)->canAcceptWritesFor(opCtx, nss);

    if (userInitiatedWritesAndNotPrimary) {
        return Status(ErrorCodes::PrimarySteppedDown,
                      str::stream() << "Not primary while removing from " << nss.ns());
    }

    auto deleteStageParams = std::make_unique<DeleteStageParams>();
    deleteStageParams->isMulti = request->isMulti();
    deleteStageParams->fromMigrate = request->isFromMigrate();
    deleteStageParams->isExplain = request->isExplain();
    deleteStageParams->returnDeleted = request->shouldReturnDeleted();
    deleteStageParams->sort = request->getSort();
    deleteStageParams->opDebug = opDebug;
    deleteStageParams->stmtId = request->getStmtId();

    unique_ptr<WorkingSet> ws = std::make_unique<WorkingSet>();
    const PlanExecutor::YieldPolicy policy = parsedDelete->yieldPolicy();

    if (!collection) {
        // Treat collections that do not exist as empty collections. Return a PlanExecutor which
        // contains an EOF stage.
        LOGV2_DEBUG(20927,
                    2,
                    "Collection {namespace} does not exist. Using EOF stage: {query}",
                    "Collection does not exist. Using EOF stage",
                    "namespace"_attr = nss.ns(),
                    "query"_attr = redact(request->getQuery()));
        return PlanExecutor::make(
            opCtx, std::move(ws), std::make_unique<EOFStage>(expCtx.get()), nullptr, policy, nss);
    }

    if (!parsedDelete->hasParsedQuery()) {

        // Only consider using the idhack if no hint was provided.
        if (request->getHint().isEmpty()) {
            // This is the idhack fast-path for getting a PlanExecutor without doing the work to
            // create a CanonicalQuery.
            const BSONObj& unparsedQuery = request->getQuery();

            const IndexDescriptor* descriptor = collection->getIndexCatalog()->findIdIndex(opCtx);

            // Construct delete request collator.
            std::unique_ptr<CollatorInterface> collator;
            if (!request->getCollation().isEmpty()) {
                auto statusWithCollator = CollatorFactoryInterface::get(opCtx->getServiceContext())
                                              ->makeFromBSON(request->getCollation());
                if (!statusWithCollator.isOK()) {
                    return statusWithCollator.getStatus();
                }
                collator = std::move(statusWithCollator.getValue());
            }
            const bool hasCollectionDefaultCollation = request->getCollation().isEmpty() ||
                CollatorInterface::collatorsMatch(collator.get(), collection->getDefaultCollator());

            if (descriptor && CanonicalQuery::isSimpleIdQuery(unparsedQuery) &&
                request->getProj().isEmpty() && hasCollectionDefaultCollation) {
                LOGV2_DEBUG(20928,
                            2,
                            "Using idhack: {query}",
                            "Using idhack",
                            "query"_attr = redact(unparsedQuery));

                auto idHackStage = std::make_unique<IDHackStage>(
                    expCtx.get(), unparsedQuery["_id"].wrap(), ws.get(), descriptor);
                unique_ptr<DeleteStage> root =
                    std::make_unique<DeleteStage>(expCtx.get(),
                                                  std::move(deleteStageParams),
                                                  ws.get(),
                                                  collection,
                                                  idHackStage.release());
                return PlanExecutor::make(
                    opCtx, std::move(ws), std::move(root), collection, policy);
            }
        }

        // If we're here then we don't have a parsed query, but we're also not eligible for
        // the idhack fast path. We need to force canonicalization now.
        Status cqStatus = parsedDelete->parseQueryToCQ();
        if (!cqStatus.isOK()) {
            return cqStatus;
        }
    }

    // This is the regular path for when we have a CanonicalQuery.
    unique_ptr<CanonicalQuery> cq(parsedDelete->releaseParsedQuery());

    // Transfer the explain verbosity level into the expression context.
    cq->getExpCtx()->explain = verbosity;

    // The underlying query plan must preserve the record id, since it will be needed in order to
    // identify the record to update.
    const size_t defaultPlannerOptions = QueryPlannerParams::PRESERVE_RECORD_ID;

    StatusWith<PrepareExecutionResult> executionResult =
        prepareExecution(opCtx, collection, ws.get(), std::move(cq), defaultPlannerOptions);
    if (!executionResult.isOK()) {
        return executionResult.getStatus();
    }
    cq = std::move(executionResult.getValue().canonicalQuery);
    unique_ptr<QuerySolution> querySolution = std::move(executionResult.getValue().querySolution);
    unique_ptr<PlanStage> root = std::move(executionResult.getValue().root);

    deleteStageParams->canonicalQuery = cq.get();

    invariant(root);
    root = std::make_unique<DeleteStage>(
        cq->getExpCtx().get(), std::move(deleteStageParams), ws.get(), collection, root.release());

    if (!request->getProj().isEmpty()) {
        invariant(request->shouldReturnDeleted());

        const bool allowPositional = true;
        StatusWith<unique_ptr<PlanStage>> projStatus = applyProjection(
            opCtx, nss, cq.get(), request->getProj(), allowPositional, ws.get(), std::move(root));
        if (!projStatus.isOK()) {
            return projStatus.getStatus();
        }
        root = std::move(projStatus.getValue());
    }

    // We must have a tree of stages in order to have a valid plan executor, but the query
    // solution may be null.
    return PlanExecutor::make(std::move(cq),
                              std::move(ws),
                              std::move(root),
                              collection,
                              policy,
                              NamespaceString(),
                              std::move(querySolution));
}

//
// Update
//

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorUpdate(
    OpDebug* opDebug,
    Collection* collection,
    ParsedUpdate* parsedUpdate,
    boost::optional<ExplainOptions::Verbosity> verbosity) {
    auto expCtx = parsedUpdate->expCtx();
    OperationContext* opCtx = expCtx->opCtx;

    const UpdateRequest* request = parsedUpdate->getRequest();
    UpdateDriver* driver = parsedUpdate->getDriver();

    const NamespaceString& nss = request->getNamespaceString();

    if (nss.isSystem() && opCtx->lockState()->shouldConflictWithSecondaryBatchApplication()) {
        uassert(10156,
                str::stream() << "cannot update a system namespace: " << nss.ns(),
                nss.isLegalClientSystemNS());
    }

    // If there is no collection and this is an upsert, callers are supposed to create
    // the collection prior to calling this method. Explain, however, will never do
    // collection or database creation.
    if (!collection && request->isUpsert()) {
        invariant(request->isExplain());
    }

    // If the parsed update does not have a user-specified collation, set it from the collection
    // default.
    if (collection && parsedUpdate->getRequest()->getCollation().isEmpty() &&
        collection->getDefaultCollator()) {
        parsedUpdate->setCollator(collection->getDefaultCollator()->clone());
    }

    // If this is a user-issued update, then we want to return an error: you cannot perform
    // writes on a secondary. If this is an update to a secondary from the replication system,
    // however, then we make an exception and let the write proceed.
    bool userInitiatedWritesAndNotPrimary = opCtx->writesAreReplicated() &&
        !repl::ReplicationCoordinator::get(opCtx)->canAcceptWritesFor(opCtx, nss);

    if (userInitiatedWritesAndNotPrimary) {
        return Status(ErrorCodes::PrimarySteppedDown,
                      str::stream() << "Not primary while performing update on " << nss.ns());
    }

    const PlanExecutor::YieldPolicy policy = parsedUpdate->yieldPolicy();

    unique_ptr<WorkingSet> ws = std::make_unique<WorkingSet>();
    UpdateStageParams updateStageParams(request, driver, opDebug);

    // If the collection doesn't exist, then return a PlanExecutor for a no-op EOF plan. We have
    // should have already enforced upstream that in this case either the upsert flag is false, or
    // we are an explain. If the collection doesn't exist, we're not an explain, and the upsert flag
    // is true, we expect the caller to have created the collection already.
    if (!collection) {
        LOGV2_DEBUG(20929,
                    2,
                    "Collection {namespace} does not exist. Using EOF stage: {query}",
                    "Collection does not exist. Using EOF stage",
                    "namespace"_attr = nss.ns(),
                    "query"_attr = redact(request->getQuery()));
        return PlanExecutor::make(
            opCtx, std::move(ws), std::make_unique<EOFStage>(expCtx.get()), nullptr, policy, nss);
    }

    // Pass index information to the update driver, so that it can determine for us whether the
    // update affects indices.
    const auto& updateIndexData = CollectionQueryInfo::get(collection).getIndexKeys(opCtx);
    driver->refreshIndexKeys(&updateIndexData);

    if (!parsedUpdate->hasParsedQuery()) {

        // Only consider using the idhack if no hint was provided.
        if (request->getHint().isEmpty()) {
            // This is the idhack fast-path for getting a PlanExecutor without doing the work
            // to create a CanonicalQuery.
            const BSONObj& unparsedQuery = request->getQuery();

            const IndexDescriptor* descriptor = collection->getIndexCatalog()->findIdIndex(opCtx);

            const bool hasCollectionDefaultCollation = CollatorInterface::collatorsMatch(
                expCtx->getCollator(), collection->getDefaultCollator());

            if (descriptor && CanonicalQuery::isSimpleIdQuery(unparsedQuery) &&
                request->getProj().isEmpty() && hasCollectionDefaultCollation) {
                LOGV2_DEBUG(20930,
                            2,
                            "Using idhack: {query}",
                            "Using idhack",
                            "query"_attr = redact(unparsedQuery));

                // Working set 'ws' is discarded. InternalPlanner::updateWithIdHack() makes its own
                // WorkingSet.
                return InternalPlanner::updateWithIdHack(opCtx,
                                                         collection,
                                                         updateStageParams,
                                                         descriptor,
                                                         unparsedQuery["_id"].wrap(),
                                                         policy);
            }
        }

        // If we're here then we don't have a parsed query, but we're also not eligible for
        // the idhack fast path. We need to force canonicalization now.
        Status cqStatus = parsedUpdate->parseQueryToCQ();
        if (!cqStatus.isOK()) {
            return cqStatus;
        }
    }

    // This is the regular path for when we have a CanonicalQuery.
    unique_ptr<CanonicalQuery> cq(parsedUpdate->releaseParsedQuery());

    // Transfer the explain verbosity level into the expression context.
    cq->getExpCtx()->explain = verbosity;

    // The underlying query plan must preserve the record id, since it will be needed in order to
    // identify the record to update.
    const size_t defaultPlannerOptions = QueryPlannerParams::PRESERVE_RECORD_ID;

    StatusWith<PrepareExecutionResult> executionResult =
        prepareExecution(opCtx, collection, ws.get(), std::move(cq), defaultPlannerOptions);
    if (!executionResult.isOK()) {
        return executionResult.getStatus();
    }
    cq = std::move(executionResult.getValue().canonicalQuery);
    unique_ptr<QuerySolution> querySolution = std::move(executionResult.getValue().querySolution);
    unique_ptr<PlanStage> root = std::move(executionResult.getValue().root);

    invariant(root);
    updateStageParams.canonicalQuery = cq.get();

    const bool isUpsert = updateStageParams.request->isUpsert();
    root =
        (isUpsert
             ? std::make_unique<UpsertStage>(
                   cq->getExpCtx().get(), updateStageParams, ws.get(), collection, root.release())
             : std::make_unique<UpdateStage>(
                   cq->getExpCtx().get(), updateStageParams, ws.get(), collection, root.release()));

    if (!request->getProj().isEmpty()) {
        invariant(request->shouldReturnAnyDocs());

        // If the plan stage is to return the newly-updated version of the documents, then it
        // is invalid to use a positional projection because the query expression need not
        // match the array element after the update has been applied.
        const bool allowPositional = request->shouldReturnOldDocs();
        StatusWith<unique_ptr<PlanStage>> projStatus = applyProjection(
            opCtx, nss, cq.get(), request->getProj(), allowPositional, ws.get(), std::move(root));
        if (!projStatus.isOK()) {
            return projStatus.getStatus();
        }
        root = std::move(projStatus.getValue());
    }

    // We must have a tree of stages in order to have a valid plan executor, but the query
    // solution may be null. Takes ownership of all args other than 'collection' and 'opCtx'
    return PlanExecutor::make(std::move(cq),
                              std::move(ws),
                              std::move(root),
                              collection,
                              policy,
                              NamespaceString(),
                              std::move(querySolution));
}

//
// Count hack
//

namespace {

/**
 * Returns 'true' if the provided solution 'soln' can be rewritten to use
 * a fast counting stage.  Mutates the tree in 'soln->root'.
 *
 * Otherwise, returns 'false'.
 */
bool turnIxscanIntoCount(QuerySolution* soln) {
    QuerySolutionNode* root = soln->root.get();

    // Root should be an ixscan or fetch w/o any filters.
    if (!(STAGE_FETCH == root->getType() || STAGE_IXSCAN == root->getType())) {
        return false;
    }

    if (STAGE_FETCH == root->getType() && nullptr != root->filter.get()) {
        return false;
    }

    // If the root is a fetch, its child should be an ixscan
    if (STAGE_FETCH == root->getType() && STAGE_IXSCAN != root->children[0]->getType()) {
        return false;
    }

    IndexScanNode* isn = (STAGE_FETCH == root->getType())
        ? static_cast<IndexScanNode*>(root->children[0])
        : static_cast<IndexScanNode*>(root);

    // No filters allowed and side-stepping isSimpleRange for now.  TODO: do we ever see
    // isSimpleRange here?  because we could well use it.  I just don't think we ever do see
    // it.

    if (nullptr != isn->filter.get() || isn->bounds.isSimpleRange) {
        return false;
    }

    // Make sure the bounds are OK.
    BSONObj startKey;
    bool startKeyInclusive;
    BSONObj endKey;
    bool endKeyInclusive;

    if (!IndexBoundsBuilder::isSingleInterval(
            isn->bounds, &startKey, &startKeyInclusive, &endKey, &endKeyInclusive)) {
        return false;
    }

    // Since count scans return no data, they are always forward scans. Index scans, on the other
    // hand, may need to scan the index in reverse order in order to obtain a sort. If the index
    // scan direction is backwards, then we need to swap the start and end of the count scan bounds.
    if (isn->direction < 0) {
        startKey.swap(endKey);
        std::swap(startKeyInclusive, endKeyInclusive);
    }

    // Make the count node that we replace the fetch + ixscan with.
    CountScanNode* csn = new CountScanNode(isn->index);
    csn->startKey = startKey;
    csn->startKeyInclusive = startKeyInclusive;
    csn->endKey = endKey;
    csn->endKeyInclusive = endKeyInclusive;
    // Takes ownership of 'cn' and deletes the old root.
    soln->root.reset(csn);
    return true;
}

/**
 * Returns true if indices contains an index that can be used with DistinctNode (the "fast distinct
 * hack" node, which can be used only if there is an empty query predicate).  Sets indexOut to the
 * array index of PlannerParams::indices.  Look for the index for the fewest fields.  Criteria for
 * suitable index is that the index should be of type BTREE or HASHED and the index cannot be a
 * partial index.
 *
 * Multikey indices are not suitable for DistinctNode when the projection is on an array element.
 * Arrays are flattened in a multikey index which makes it impossible for the distinct scan stage
 * (plan stage generated from DistinctNode) to select the requested element by array index.
 *
 * Multikey indices cannot be used for the fast distinct hack if the field is dotted.  Currently the
 * solution generated for the distinct hack includes a projection stage and the projection stage
 * cannot be covered with a dotted field.
 */
bool getDistinctNodeIndex(const std::vector<IndexEntry>& indices,
                          const std::string& field,
                          const CollatorInterface* collator,
                          size_t* indexOut) {
    invariant(indexOut);
    int minFields = std::numeric_limits<int>::max();
    for (size_t i = 0; i < indices.size(); ++i) {
        // Skip indices with non-matching collator.
        if (!CollatorInterface::collatorsMatch(indices[i].collator, collator)) {
            continue;
        }
        // Skip partial indices.
        if (indices[i].filterExpr) {
            continue;
        }
        // Skip indices where the first key is not 'field'.
        auto firstIndexField = indices[i].keyPattern.firstElement();
        if (firstIndexField.fieldNameStringData() != StringData(field)) {
            continue;
        }
        // Skip the index if the first key is a "plugin" such as "hashed", "2dsphere", and so on.
        if (!firstIndexField.isNumber()) {
            continue;
        }
        // Compound hashed indexes can use distinct scan if the first field is 1 or -1. For the
        // other special indexes, the 1 or -1 index fields may be stored as a function of the data
        // rather than the raw data itself. Storing f(d) instead of 'd' precludes the distinct_scan
        // due to the possibility that f(d1) == f(d2).  Therefore, after fetching the base data,
        // either d1 or d2 would be incorrectly missing from the result set.
        auto indexPluginName = IndexNames::findPluginName(indices[i].keyPattern);
        switch (IndexNames::nameToType(indexPluginName)) {
            case IndexType::INDEX_BTREE:
            case IndexType::INDEX_HASHED:
                break;
            default:
                // All other index types are not eligible.
                continue;
        }

        int nFields = indices[i].keyPattern.nFields();
        // Pick the index with the lowest number of fields.
        if (nFields < minFields) {
            minFields = nFields;
            *indexOut = i;
        }
    }
    return minFields != std::numeric_limits<int>::max();
}

}  // namespace

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorCount(
    const boost::intrusive_ptr<ExpressionContext>& expCtx,
    Collection* collection,
    const CountCommand& request,
    bool explain,
    const NamespaceString& nss) {
    OperationContext* opCtx = expCtx->opCtx;
    unique_ptr<WorkingSet> ws = std::make_unique<WorkingSet>();

    auto qr = std::make_unique<QueryRequest>(nss);
    qr->setFilter(request.getQuery());
    auto collation = request.getCollation().value_or(BSONObj());
    qr->setCollation(collation);
    qr->setHint(request.getHint());
    qr->setExplain(explain);

    auto statusWithCQ = CanonicalQuery::canonicalize(
        opCtx,
        std::move(qr),
        expCtx,
        collection ? static_cast<const ExtensionsCallback&>(
                         ExtensionsCallbackReal(opCtx, &collection->ns()))
                   : static_cast<const ExtensionsCallback&>(ExtensionsCallbackNoop()),
        MatchExpressionParser::kAllowAllSpecialFeatures);

    if (!statusWithCQ.isOK()) {
        return statusWithCQ.getStatus();
    }
    unique_ptr<CanonicalQuery> cq = std::move(statusWithCQ.getValue());

    const auto yieldPolicy = opCtx->inMultiDocumentTransaction() ? PlanExecutor::INTERRUPT_ONLY
                                                                 : PlanExecutor::YIELD_AUTO;

    const auto skip = request.getSkip().value_or(0);
    const auto limit = request.getLimit().value_or(0);

    if (!collection) {
        // Treat collections that do not exist as empty collections. Note that the explain reporting
        // machinery always assumes that the root stage for a count operation is a CountStage, so in
        // this case we put a CountStage on top of an EOFStage.
        unique_ptr<PlanStage> root = std::make_unique<CountStage>(
            expCtx.get(), collection, limit, skip, ws.get(), new EOFStage(expCtx.get()));
        return PlanExecutor::make(opCtx, std::move(ws), std::move(root), nullptr, yieldPolicy, nss);
    }

    // If the query is empty, then we can determine the count by just asking the collection
    // for its number of records. This is implemented by the CountStage, and we don't need
    // to create a child for the count stage in this case.
    //
    // If there is a hint, then we can't use a trival count plan as described above.
    const bool isEmptyQueryPredicate =
        cq->root()->matchType() == MatchExpression::AND && cq->root()->numChildren() == 0;
    const bool useRecordStoreCount = isEmptyQueryPredicate && request.getHint().isEmpty();

    if (useRecordStoreCount) {
        unique_ptr<PlanStage> root =
            std::make_unique<RecordStoreFastCountStage>(expCtx.get(), collection, skip, limit);
        return PlanExecutor::make(opCtx, std::move(ws), std::move(root), nullptr, yieldPolicy, nss);
    }

    size_t plannerOptions = QueryPlannerParams::IS_COUNT;
    if (OperationShardingState::isOperationVersioned(opCtx)) {
        plannerOptions |= QueryPlannerParams::INCLUDE_SHARD_FILTER;
    }

    StatusWith<PrepareExecutionResult> executionResult =
        prepareExecution(opCtx, collection, ws.get(), std::move(cq), plannerOptions);
    if (!executionResult.isOK()) {
        return executionResult.getStatus();
    }
    cq = std::move(executionResult.getValue().canonicalQuery);
    unique_ptr<QuerySolution> querySolution = std::move(executionResult.getValue().querySolution);
    unique_ptr<PlanStage> root = std::move(executionResult.getValue().root);

    invariant(root);

    // Make a CountStage to be the new root.
    root = std::make_unique<CountStage>(
        expCtx.get(), collection, limit, skip, ws.get(), root.release());
    // We must have a tree of stages in order to have a valid plan executor, but the query
    // solution may be NULL. Takes ownership of all args other than 'collection' and 'opCtx'
    return PlanExecutor::make(std::move(cq),
                              std::move(ws),
                              std::move(root),
                              collection,
                              yieldPolicy,
                              NamespaceString(),
                              std::move(querySolution));
}

//
// Distinct hack
//

bool turnIxscanIntoDistinctIxscan(QuerySolution* soln,
                                  const string& field,
                                  bool strictDistinctOnly) {
    QuerySolutionNode* root = soln->root.get();

    // We can attempt to convert a plan if it follows one of these patterns (starting from the
    // root):
    //   1. PROJECT=>FETCH=>IXSCAN
    //   2. FETCH=>IXSCAN
    //   3. PROJECT=>IXSCAN
    QuerySolutionNode* projectNode = nullptr;
    IndexScanNode* indexScanNode = nullptr;
    FetchNode* fetchNode = nullptr;

    switch (root->getType()) {
        case STAGE_PROJECTION_DEFAULT:
        case STAGE_PROJECTION_COVERED:
        case STAGE_PROJECTION_SIMPLE:
            projectNode = root;
            break;
        case STAGE_FETCH:
            fetchNode = static_cast<FetchNode*>(root);
            break;
        default:
            return false;
    }

    if (!fetchNode && (STAGE_FETCH == root->children[0]->getType())) {
        fetchNode = static_cast<FetchNode*>(root->children[0]);
    }

    if (fetchNode && (STAGE_IXSCAN == fetchNode->children[0]->getType())) {
        indexScanNode = static_cast<IndexScanNode*>(fetchNode->children[0]);
    } else if (projectNode && (STAGE_IXSCAN == projectNode->children[0]->getType())) {
        indexScanNode = static_cast<IndexScanNode*>(projectNode->children[0]);
    }

    if (!indexScanNode) {
        return false;
    }

    // If the fetch has a filter, we're out of luck. We can't skip all keys with a given value,
    // since one of them may key a document that passes the filter.
    if (fetchNode && fetchNode->filter) {
        return false;
    }

    if (indexScanNode->index.type == IndexType::INDEX_WILDCARD) {
        // If the query is on a field other than the distinct key, we may have generated a $** plan
        // which does not actually contain the distinct key field.
        if (field != std::next(indexScanNode->index.keyPattern.begin())->fieldName()) {
            return false;
        }
        // If the query includes object bounds, we cannot turn this IXSCAN into a DISTINCT_SCAN.
        // Wildcard indexes contain multiple keys per object, one for each subpath in ascending
        // (Path, Value, RecordId) order. If the distinct fields in two successive documents are
        // objects with the same leaf path values but in different field order, e.g. {a: 1, b: 2}
        // and {b: 2, a: 1}, we would therefore only return the first document and skip the other.
        if (wcp::isWildcardObjectSubpathScan(indexScanNode)) {
            return false;
        }
    }

    // An additional filter must be applied to the data in the key, so we can't just skip
    // all the keys with a given value; we must examine every one to find the one that (may)
    // pass the filter.
    if (indexScanNode->filter) {
        return false;
    }

    // We only set this when we have special query modifiers (.max() or .min()) or other
    // special cases.  Don't want to handle the interactions between those and distinct.
    // Don't think this will ever really be true but if it somehow is, just ignore this
    // soln.
    if (indexScanNode->bounds.isSimpleRange) {
        return false;
    }

    // Figure out which field we're skipping to the next value of.
    int fieldNo = 0;
    BSONObjIterator it(indexScanNode->index.keyPattern);
    while (it.more()) {
        if (field == it.next().fieldName()) {
            break;
        }
        ++fieldNo;
    }

    if (strictDistinctOnly) {
        // If the "distinct" field is not the first field in the index bounds then the only way we
        // can guarantee that we'll never see duplicate values for the distinct field is to make
        // sure every field before the distinct field has equality bounds. For example, a
        // DISTINCT_SCAN on 'b' over the {a: 1, b: 1} index will scan a particular 'b' value
        // multiple times if that 'b' value exists in documents with different 'a' values. The
        // equality bounds on 'a' prevent the scan from seeing duplicate 'b' values by ensuring the
        // scan is limited to a single value for the 'a' field.
        for (size_t i = 0; i < static_cast<size_t>(fieldNo); ++i) {
            invariant(i < indexScanNode->bounds.size());
            if (indexScanNode->bounds.fields[i].intervals.size() != 1 ||
                !indexScanNode->bounds.fields[i].intervals[0].isPoint()) {
                return false;
            }
        }
    }

    // We should not use a distinct scan if the field over which we are computing the distinct is
    // multikey.
    if (indexScanNode->index.multikey) {
        const auto& multikeyPaths = indexScanNode->index.multikeyPaths;
        if (multikeyPaths.empty()) {
            // We don't have path-level multikey information available.
            return false;
        }

        if (!multikeyPaths[fieldNo].empty()) {
            // Path-level multikey information indicates that the distinct key contains at least one
            // array component.
            return false;
        }
    }

    // Make a new DistinctNode. We will swap this for the ixscan in the provided solution.
    auto distinctNode = std::make_unique<DistinctNode>(indexScanNode->index);
    distinctNode->direction = indexScanNode->direction;
    distinctNode->bounds = indexScanNode->bounds;
    distinctNode->queryCollator = indexScanNode->queryCollator;
    distinctNode->fieldNo = fieldNo;

    if (fetchNode) {
        // If the original plan had PROJECT and FETCH stages, we can get rid of the PROJECT
        // transforming the plan from PROJECT=>FETCH=>IXSCAN to FETCH=>DISTINCT_SCAN.
        if (projectNode) {
            invariant(projectNode == root);
            projectNode = nullptr;

            invariant(STAGE_FETCH == root->children[0]->getType());
            invariant(STAGE_IXSCAN == root->children[0]->children[0]->getType());

            // Detach the fetch from its parent projection.
            root->children.clear();

            // Make the fetch the new root. This destroys the project stage.
            soln->root.reset(fetchNode);
        }

        // Whenver we have a FETCH node, the IXSCAN is its child. We detach the IXSCAN from the
        // solution tree and take ownership of it, so that it gets destroyed when we leave this
        // scope.
        std::unique_ptr<IndexScanNode> ownedIsn(indexScanNode);
        indexScanNode = nullptr;

        // Attach the distinct node in the index scan's place.
        fetchNode->children[0] = distinctNode.release();
    } else {
        // There is no fetch node. The PROJECT=>IXSCAN tree should become PROJECT=>DISTINCT_SCAN.
        invariant(projectNode == root);
        invariant(STAGE_IXSCAN == root->children[0]->getType());

        // Take ownership of the index scan node, detaching it from the solution tree.
        std::unique_ptr<IndexScanNode> ownedIsn(indexScanNode);

        // Attach the distinct node in the index scan's place.
        root->children[0] = distinctNode.release();
    }

    return true;
}

namespace {

// Get the list of indexes that include the "distinct" field.
QueryPlannerParams fillOutPlannerParamsForDistinct(OperationContext* opCtx,
                                                   Collection* collection,
                                                   size_t plannerOptions,
                                                   const ParsedDistinct& parsedDistinct) {
    QueryPlannerParams plannerParams;
    plannerParams.options = QueryPlannerParams::NO_TABLE_SCAN | plannerOptions;

    // If the caller did not request a "strict" distinct scan then we may choose a plan which
    // unwinds arrays and treats each element in an array as its own key.
    const bool mayUnwindArrays = !(plannerOptions & QueryPlannerParams::STRICT_DISTINCT_ONLY);
    std::unique_ptr<IndexCatalog::IndexIterator> ii =
        collection->getIndexCatalog()->getIndexIterator(opCtx, false);
    auto query = parsedDistinct.getQuery()->getQueryRequest().getFilter();
    while (ii->more()) {
        const IndexCatalogEntry* ice = ii->next();
        const IndexDescriptor* desc = ice->descriptor();

        // Skip the addition of hidden indexes to prevent use in query planning.
        if (desc->hidden())
            continue;
        if (desc->keyPattern().hasField(parsedDistinct.getKey())) {
            if (!mayUnwindArrays &&
                isAnyComponentOfPathMultikey(desc->keyPattern(),
                                             desc->isMultikey(),
                                             desc->getMultikeyPaths(opCtx),
                                             parsedDistinct.getKey())) {
                // If the caller requested "strict" distinct that does not "pre-unwind" arrays,
                // then an index which is multikey on the distinct field may not be used. This is
                // because when indexing an array each element gets inserted individually. Any plan
                // which involves scanning the index will have effectively "unwound" all arrays.
                continue;
            }

            plannerParams.indices.push_back(
                indexEntryFromIndexCatalogEntry(opCtx, *ice, parsedDistinct.getQuery()));
        } else if (desc->getIndexType() == IndexType::INDEX_WILDCARD && !query.isEmpty()) {
            // Check whether the $** projection captures the field over which we are distinct-ing.
            auto* proj = static_cast<const WildcardAccessMethod*>(ice->accessMethod())
                             ->getWildcardProjection()
                             ->exec();
            if (projection_executor_utils::applyProjectionToOneField(proj,
                                                                     parsedDistinct.getKey())) {
                plannerParams.indices.push_back(
                    indexEntryFromIndexCatalogEntry(opCtx, *ice, parsedDistinct.getQuery()));
            }

            // It is not necessary to do any checks about 'mayUnwindArrays' in this case, because:
            // 1) If there is no predicate on the distinct(), a wildcard indices may not be used.
            // 2) distinct() _with_ a predicate may not be answered with a DISTINCT_SCAN on _any_
            // multikey index.

            // So, we will not distinct scan a wildcard index that's multikey on the distinct()
            // field, regardless of the value of 'mayUnwindArrays'.
        }
    }

    const CanonicalQuery* canonicalQuery = parsedDistinct.getQuery();
    const BSONObj& hint = canonicalQuery->getQueryRequest().getHint();

    applyIndexFilters(collection, *canonicalQuery, &plannerParams);

    // If there exists an index filter, we ignore all hints. Else, we only keep the index specified
    // by the hint. Since we cannot have an index with name $natural, that case will clear the
    // plannerParams.indices.
    if (!plannerParams.indexFiltersApplied && !hint.isEmpty()) {
        std::vector<IndexEntry> temp =
            QueryPlannerIXSelect::findIndexesByHint(hint, plannerParams.indices);
        temp.swap(plannerParams.indices);
    }

    return plannerParams;
}

/**
 * A simple DISTINCT_SCAN has an empty query and no sort, so we just need to find a suitable index
 * that has the "distinct" field as the first component of its key pattern.
 *
 * If a suitable solution is found, this function will create and return a new executor. In order to
 * do so, it releases the CanonicalQuery from the 'parsedDistinct' input. If no solution is found,
 * the return value is StatusOK with a nullptr value, and the 'parsedDistinct' CanonicalQuery
 * remains valid. This function may also return a failed status code, in which case the caller
 * should assume that the 'parsedDistinct' CanonicalQuery is no longer valid.
 */
StatusWith<std::unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorForSimpleDistinct(
    OperationContext* opCtx,
    Collection* collection,
    const QueryPlannerParams& plannerParams,
    PlanExecutor::YieldPolicy yieldPolicy,
    ParsedDistinct* parsedDistinct) {
    invariant(parsedDistinct->getQuery());
    auto collator = parsedDistinct->getQuery()->getCollator();

    // If there's no query, we can just distinct-scan one of the indices. Not every index in
    // plannerParams.indices may be suitable. Refer to getDistinctNodeIndex().
    size_t distinctNodeIndex = 0;
    if (!parsedDistinct->getQuery()->getQueryRequest().getFilter().isEmpty() ||
        parsedDistinct->getQuery()->getSortPattern() ||
        !getDistinctNodeIndex(
            plannerParams.indices, parsedDistinct->getKey(), collator, &distinctNodeIndex)) {
        // Not a "simple" DISTINCT_SCAN or no suitable index was found.
        return {nullptr};
    }

    auto dn = std::make_unique<DistinctNode>(plannerParams.indices[distinctNodeIndex]);
    dn->direction = 1;
    IndexBoundsBuilder::allValuesBounds(dn->index.keyPattern, &dn->bounds);
    dn->queryCollator = collator;
    dn->fieldNo = 0;

    // An index with a non-simple collation requires a FETCH stage.
    std::unique_ptr<QuerySolutionNode> solnRoot = std::move(dn);
    if (plannerParams.indices[distinctNodeIndex].collator) {
        if (!solnRoot->fetched()) {
            auto fetch = std::make_unique<FetchNode>();
            fetch->children.push_back(solnRoot.release());
            solnRoot = std::move(fetch);
        }
    }

    QueryPlannerParams params;

    auto soln = QueryPlannerAnalysis::analyzeDataAccess(
        *parsedDistinct->getQuery(), params, std::move(solnRoot));
    invariant(soln);

    unique_ptr<WorkingSet> ws = std::make_unique<WorkingSet>();
    auto root =
        StageBuilder::build(opCtx, collection, *parsedDistinct->getQuery(), *soln, ws.get());

    LOGV2_DEBUG(20931,
                2,
                "Using fast distinct: {query}, planSummary: {planSummary}",
                "Using fast distinct",
                "query"_attr = redact(parsedDistinct->getQuery()->toStringShort()),
                "planSummary"_attr = Explain::getPlanSummary(root.get()));

    return PlanExecutor::make(parsedDistinct->releaseQuery(),
                              std::move(ws),
                              std::move(root),
                              collection,
                              yieldPolicy,
                              NamespaceString(),
                              std::move(soln));
}

// Checks each solution in the 'solutions' vector to see if one includes an IXSCAN that can be
// rewritten as a DISTINCT_SCAN, assuming we want distinct scan behavior on the getKey() property of
// the 'parsedDistinct' argument.
//
// If a suitable solution is found, this function will create and return a new executor. In order to
// do so, it releases the CanonicalQuery from the 'parsedDistinct' input. If no solution is found,
// the return value is StatusOK with a nullptr value, and the 'parsedDistinct' CanonicalQuery
// remains valid. This function may also return a failed status code, in which case the caller
// should assume that the 'parsedDistinct' CanonicalQuery is no longer valid.
//
// See the declaration of turnIxscanIntoDistinctIxscan() for an explanation of the
// 'strictDistinctOnly' parameter.
StatusWith<std::unique_ptr<PlanExecutor, PlanExecutor::Deleter>>
getExecutorDistinctFromIndexSolutions(OperationContext* opCtx,
                                      Collection* collection,
                                      std::vector<std::unique_ptr<QuerySolution>> solutions,
                                      PlanExecutor::YieldPolicy yieldPolicy,
                                      ParsedDistinct* parsedDistinct,
                                      bool strictDistinctOnly) {
    // We look for a solution that has an ixscan we can turn into a distinctixscan
    for (size_t i = 0; i < solutions.size(); ++i) {
        if (turnIxscanIntoDistinctIxscan(
                solutions[i].get(), parsedDistinct->getKey(), strictDistinctOnly)) {
            // Build and return the SSR over solutions[i].
            unique_ptr<WorkingSet> ws = std::make_unique<WorkingSet>();
            unique_ptr<QuerySolution> currentSolution = std::move(solutions[i]);
            auto root = StageBuilder::build(
                opCtx, collection, *parsedDistinct->getQuery(), *currentSolution, ws.get());

            LOGV2_DEBUG(20932,
                        2,
                        "Using fast distinct: {query}, planSummary: {planSummary}",
                        "Using fast distinct",
                        "query"_attr = redact(parsedDistinct->getQuery()->toStringShort()),
                        "planSummary"_attr = Explain::getPlanSummary(root.get()));

            return PlanExecutor::make(parsedDistinct->releaseQuery(),
                                      std::move(ws),
                                      std::move(root),
                                      collection,
                                      yieldPolicy,
                                      NamespaceString(),
                                      std::move(currentSolution));
        }
    }

    // Indicate that, although there was no error, we did not find a DISTINCT_SCAN solution.
    return {nullptr};
}

/**
 * Makes a clone of 'cq' but without any projection, then runs getExecutor on the clone.
 */
StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorWithoutProjection(
    OperationContext* opCtx,
    Collection* collection,
    const CanonicalQuery* cq,
    PlanExecutor::YieldPolicy yieldPolicy,
    size_t plannerOptions) {
    auto qr = std::make_unique<QueryRequest>(cq->getQueryRequest());
    qr->setProj(BSONObj());

    const boost::intrusive_ptr<ExpressionContext> expCtx;
    const ExtensionsCallbackReal extensionsCallback(opCtx, &collection->ns());
    auto cqWithoutProjection =
        CanonicalQuery::canonicalize(opCtx,
                                     std::move(qr),
                                     expCtx,
                                     extensionsCallback,
                                     MatchExpressionParser::kAllowAllSpecialFeatures);

    return getExecutor(
        opCtx, collection, std::move(cqWithoutProjection.getValue()), yieldPolicy, plannerOptions);
}
}  // namespace

StatusWith<unique_ptr<PlanExecutor, PlanExecutor::Deleter>> getExecutorDistinct(
    Collection* collection, size_t plannerOptions, ParsedDistinct* parsedDistinct) {
    auto expCtx = parsedDistinct->getQuery()->getExpCtx();
    OperationContext* opCtx = expCtx->opCtx;
    const auto yieldPolicy = opCtx->inMultiDocumentTransaction() ? PlanExecutor::INTERRUPT_ONLY
                                                                 : PlanExecutor::YIELD_AUTO;

    if (!collection) {
        // Treat collections that do not exist as empty collections.
        return PlanExecutor::make(parsedDistinct->releaseQuery(),
                                  std::make_unique<WorkingSet>(),
                                  std::make_unique<EOFStage>(expCtx.get()),
                                  collection,
                                  yieldPolicy);
    }

    // TODO: check for idhack here?

    // When can we do a fast distinct hack?
    // 1. There is a plan with just one leaf and that leaf is an ixscan.
    // 2. The ixscan indexes the field we're interested in.
    // 2a: We are correct if the index contains the field but for now we look for prefix.
    // 3. The query is covered/no fetch.
    //
    // We go through normal planning (with limited parameters) to see if we can produce
    // a soln with the above properties.

    auto plannerParams =
        fillOutPlannerParamsForDistinct(opCtx, collection, plannerOptions, *parsedDistinct);

    // If there are no suitable indices for the distinct hack bail out now into regular planning
    // with no projection.
    if (plannerParams.indices.empty()) {
        if (plannerOptions & QueryPlannerParams::STRICT_DISTINCT_ONLY) {
            // STRICT_DISTINCT_ONLY indicates that we should not return any plan if we can't return
            // a DISTINCT_SCAN plan.
            return {nullptr};
        } else {
            // Note that, when not in STRICT_DISTINCT_ONLY mode, the caller doesn't care about the
            // projection, only that the planner does not produce a FETCH if it's possible to cover
            // the fields in the projection. That's definitely not possible in this case, so we
            // dispense with the projection.
            return getExecutorWithoutProjection(
                opCtx, collection, parsedDistinct->getQuery(), yieldPolicy, plannerOptions);
        }
    }

    //
    // If we're here, we have an index that includes the field we're distinct-ing over.
    //

    auto executorWithStatus =
        getExecutorForSimpleDistinct(opCtx, collection, plannerParams, yieldPolicy, parsedDistinct);
    if (!executorWithStatus.isOK() || executorWithStatus.getValue()) {
        // We either got a DISTINCT plan or a fatal error.
        return executorWithStatus;
    } else {
        // A "simple" DISTINCT plan wasn't possible, but we can try again with the QueryPlanner.
    }

    // Ask the QueryPlanner for a list of solutions that scan one of the indexes from
    // fillOutPlannerParamsForDistinct() (i.e., the indexes that include the distinct field).
    auto statusWithSolutions = QueryPlanner::plan(*parsedDistinct->getQuery(), plannerParams);
    if (!statusWithSolutions.isOK()) {
        if (plannerOptions & QueryPlannerParams::STRICT_DISTINCT_ONLY) {
            return {nullptr};
        } else {
            return getExecutor(
                opCtx, collection, parsedDistinct->releaseQuery(), yieldPolicy, plannerOptions);
        }
    }
    auto solutions = std::move(statusWithSolutions.getValue());

    // See if any of the solutions can be rewritten using a DISTINCT_SCAN. Note that, if the
    // STRICT_DISTINCT_ONLY flag is not set, we may get a DISTINCT_SCAN plan that filters out some
    // but not all duplicate values of the distinct field, meaning that the output from this
    // executor will still need deduplication.
    executorWithStatus = getExecutorDistinctFromIndexSolutions(
        opCtx,
        collection,
        std::move(solutions),
        yieldPolicy,
        parsedDistinct,
        (plannerOptions & QueryPlannerParams::STRICT_DISTINCT_ONLY));
    if (!executorWithStatus.isOK() || executorWithStatus.getValue()) {
        // We either got a DISTINCT plan or a fatal error.
        return executorWithStatus;
    } else if (!(plannerOptions & QueryPlannerParams::STRICT_DISTINCT_ONLY)) {
        // We did not find a solution that we could convert to a DISTINCT_SCAN, so we fall back to
        // regular planning. Note that, when not in STRICT_DISTINCT_ONLY mode, the caller doesn't
        // care about the projection, only that the planner does not produce a FETCH if it's
        // possible to cover the fields in the projection. That's definitely not possible in this
        // case, so we dispense with the projection.
        return getExecutorWithoutProjection(
            opCtx, collection, parsedDistinct->getQuery(), yieldPolicy, plannerOptions);
    } else {
        // We did not find a solution that we could convert to DISTINCT_SCAN, and the
        // STRICT_DISTINCT_ONLY prohibits us from using any other kind of plan, so we return
        // nullptr.
        return {nullptr};
    }
}

}  // namespace mongo
