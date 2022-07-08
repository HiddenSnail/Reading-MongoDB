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

#include "mongo/db/exec/subplan.h"

#include <memory>
#include <vector>

#include "mongo/db/exec/multi_plan.h"
#include "mongo/db/exec/scoped_timer.h"
#include "mongo/db/matcher/extensions_callback_real.h"
#include "mongo/db/query/collection_query_info.h"
#include "mongo/db/query/get_executor.h"
#include "mongo/db/query/plan_executor.h"
#include "mongo/db/query/planner_access.h"
#include "mongo/db/query/planner_analysis.h"
#include "mongo/db/query/query_planner.h"
#include "mongo/db/query/query_planner_common.h"
#include "mongo/db/query/stage_builder.h"
#include "mongo/logv2/log.h"
#include "mongo/util/scopeguard.h"
#include "mongo/util/transitional_tools_do_not_use/vector_spooling.h"

namespace mongo {

using std::endl;
using std::unique_ptr;
using std::vector;

const char* SubplanStage::kStageType = "SUBPLAN";

SubplanStage::SubplanStage(ExpressionContext* expCtx,
                           const Collection* collection,
                           WorkingSet* ws,
                           const QueryPlannerParams& params,
                           CanonicalQuery* cq)
    : RequiresAllIndicesStage(kStageType, expCtx, collection),
      _ws(ws),
      _plannerParams(params),
      _query(cq) {
    invariant(cq);
    invariant(_query->root()->matchType() == MatchExpression::OR);
    invariant(_query->root()->numChildren(),
              "Cannot use a SUBPLAN stage for an $or with no children");
}

bool SubplanStage::canUseSubplanning(const CanonicalQuery& query) {
    const QueryRequest& qr = query.getQueryRequest();
    const MatchExpression* expr = query.root();

    // Hint provided
    if (!qr.getHint().isEmpty()) {
        return false;
    }

    // Min provided
    // Min queries are a special case of hinted queries.
    if (!qr.getMin().isEmpty()) {
        return false;
    }

    // Max provided
    // Similar to min, max queries are a special case of hinted queries.
    if (!qr.getMax().isEmpty()) {
        return false;
    }

    // Tailable cursors won't get cached, just turn into collscans.
    if (query.getQueryRequest().isTailable()) {
        return false;
    }

    // We can only subplan rooted $or queries, and only if they have at least one clause.
    return MatchExpression::OR == expr->matchType() && expr->numChildren() > 0;
}

Status SubplanStage::planSubqueries() {
    _orExpression = _query->root()->shallowClone();
    for (size_t i = 0; i < _plannerParams.indices.size(); ++i) {
        const IndexEntry& ie = _plannerParams.indices[i];
        const auto insertionRes = _indexMap.insert(std::make_pair(ie.identifier, i));
        // Be sure the key was not already in the map.
        invariant(insertionRes.second);
        LOGV2_DEBUG(20598, 5, "Subplanner: index {i} is {ie}", "i"_attr = i, "ie"_attr = ie);
    }

    for (size_t i = 0; i < _orExpression->numChildren(); ++i) {
        // We need a place to shove the results from planning this branch.
        _branchResults.push_back(std::make_unique<BranchPlanningResult>());
        BranchPlanningResult* branchResult = _branchResults.back().get();

        MatchExpression* orChild = _orExpression->getChild(i);

        // Turn the i-th child into its own query.
        auto statusWithCQ = CanonicalQuery::canonicalize(opCtx(), *_query, orChild);
        if (!statusWithCQ.isOK()) {
            str::stream ss;
            ss << "Can't canonicalize subchild " << orChild->debugString() << " "
               << statusWithCQ.getStatus().reason();
            return Status(ErrorCodes::BadValue, ss);
        }

        branchResult->canonicalQuery = std::move(statusWithCQ.getValue());

        // Plan the i-th child. We might be able to find a plan for the i-th child in the plan
        // cache. If there's no cached plan, then we generate and rank plans using the MPS.
        const auto* planCache = CollectionQueryInfo::get(collection()).getPlanCache();

        // Populate branchResult->cachedSolution if an active cachedSolution entry exists.
        if (planCache->shouldCacheQuery(*branchResult->canonicalQuery)) {
            auto planCacheKey = planCache->computeKey(*branchResult->canonicalQuery);
            if (auto cachedSol = planCache->getCacheEntryIfActive(planCacheKey)) {
                // We have a CachedSolution. Store it for later.
                LOGV2_DEBUG(
                    20599,
                    5,
                    "Subplanner: cached plan found for child {i} of {orExpression_numChildren}",
                    "i"_attr = i,
                    "orExpression_numChildren"_attr = _orExpression->numChildren());

                branchResult->cachedSolution = std::move(cachedSol);
            }
        }

        if (!branchResult->cachedSolution) {
            // No CachedSolution found. We'll have to plan from scratch.
            LOGV2_DEBUG(20600,
                        5,
                        "Subplanner: planning child {i} of {orExpression_numChildren}",
                        "i"_attr = i,
                        "orExpression_numChildren"_attr = _orExpression->numChildren());

            // We don't set NO_TABLE_SCAN because peeking at the cache data will keep us from
            // considering any plan that's a collscan.
            invariant(branchResult->solutions.empty());
            auto solutions = QueryPlanner::plan(*branchResult->canonicalQuery, _plannerParams);
            if (!solutions.isOK()) {
                str::stream ss;
                ss << "Can't plan for subchild " << branchResult->canonicalQuery->toString() << " "
                   << solutions.getStatus().reason();
                return Status(ErrorCodes::BadValue, ss);
            }
            branchResult->solutions = std::move(solutions.getValue());

            LOGV2_DEBUG(20601,
                        5,
                        "Subplanner: got {branchResult_solutions_size} solutions",
                        "branchResult_solutions_size"_attr = branchResult->solutions.size());
        }
    }

    return Status::OK();
}

namespace {

/**
 * On success, applies the index tags from 'branchCacheData' (which represent the winning
 * plan for 'orChild') to 'compositeCacheData'.
 */
Status tagOrChildAccordingToCache(PlanCacheIndexTree* compositeCacheData,
                                  SolutionCacheData* branchCacheData,
                                  MatchExpression* orChild,
                                  const std::map<IndexEntry::Identifier, size_t>& indexMap) {
    invariant(compositeCacheData);

    // We want a well-formed *indexed* solution.
    if (nullptr == branchCacheData) {
        // For example, we don't cache things for 2d indices.
        str::stream ss;
        ss << "No cache data for subchild " << orChild->debugString();
        return Status(ErrorCodes::NoQueryExecutionPlans, ss);
    }

    if (SolutionCacheData::USE_INDEX_TAGS_SOLN != branchCacheData->solnType) {
        str::stream ss;
        ss << "No indexed cache data for subchild " << orChild->debugString();
        return Status(ErrorCodes::NoQueryExecutionPlans, ss);
    }

    // Add the index assignments to our original query.
    Status tagStatus =
        QueryPlanner::tagAccordingToCache(orChild, branchCacheData->tree.get(), indexMap);

    if (!tagStatus.isOK()) {
        str::stream ss;
        ss << "Failed to extract indices from subchild " << orChild->debugString();
        return tagStatus.withContext(ss);
    }

    // Add the child's cache data to the cache data we're creating for the main query.
    compositeCacheData->children.push_back(branchCacheData->tree->clone());

    return Status::OK();
}

}  // namespace

Status SubplanStage::choosePlanForSubqueries(PlanYieldPolicy* yieldPolicy) {
    // This is the skeleton of index selections that is inserted into the cache.
    std::unique_ptr<PlanCacheIndexTree> cacheData(new PlanCacheIndexTree());

    for (size_t i = 0; i < _orExpression->numChildren(); ++i) {
        MatchExpression* orChild = _orExpression->getChild(i);
        BranchPlanningResult* branchResult = _branchResults[i].get();

        if (branchResult->cachedSolution.get()) {
            // We can get the index tags we need out of the cache.
            Status tagStatus = tagOrChildAccordingToCache(
                cacheData.get(), branchResult->cachedSolution->plannerData[0], orChild, _indexMap);
            if (!tagStatus.isOK()) {
                return tagStatus;
            }
        } else if (1 == branchResult->solutions.size()) {
            QuerySolution* soln = branchResult->solutions.front().get();
            Status tagStatus = tagOrChildAccordingToCache(
                cacheData.get(), soln->cacheData.get(), orChild, _indexMap);
            if (!tagStatus.isOK()) {
                return tagStatus;
            }
        } else {
            // N solutions, rank them.

            // We already checked for zero solutions in planSubqueries(...).
            invariant(!branchResult->solutions.empty());

            _ws->clear();

            // We pass the SometimesCache option to the MPS because the SubplanStage currently does
            // not use the CachedPlanStage's eviction mechanism. We therefore are more conservative
            // about putting a potentially bad plan into the cache in the subplan path.  We
            // temporarily add the MPS to _children to ensure that we pass down all save/restore
            // messages that can be generated if pickBestPlan yields.
            invariant(_children.empty());
            _children.emplace_back(
                std::make_unique<MultiPlanStage>(expCtx(),
                                                 collection(),
                                                 branchResult->canonicalQuery.get(),
                                                 MultiPlanStage::CachingMode::SometimesCache));
            ON_BLOCK_EXIT([&] {
                invariant(_children.size() == 1);  // Make sure nothing else was added to _children.
                _children.pop_back();
            });
            MultiPlanStage* multiPlanStage = static_cast<MultiPlanStage*>(child().get());

            // Dump all the solutions into the MPS.
            for (size_t ix = 0; ix < branchResult->solutions.size(); ++ix) {
                auto nextPlanRoot = StageBuilder::build(opCtx(),
                                                        collection(),
                                                        *branchResult->canonicalQuery,
                                                        *branchResult->solutions[ix],
                                                        _ws);

                multiPlanStage->addPlan(
                    std::move(branchResult->solutions[ix]), std::move(nextPlanRoot), _ws);
            }

            Status planSelectStat = multiPlanStage->pickBestPlan(yieldPolicy);
            if (!planSelectStat.isOK()) {
                return planSelectStat;
            }

            if (!multiPlanStage->bestPlanChosen()) {
                str::stream ss;
                ss << "Failed to pick best plan for subchild "
                   << branchResult->canonicalQuery->toString();
                return Status(ErrorCodes::NoQueryExecutionPlans, ss);
            }

            QuerySolution* bestSoln = multiPlanStage->bestSolution();

            // Check that we have good cache data. For example, we don't cache things
            // for 2d indices.
            if (nullptr == bestSoln->cacheData.get()) {
                str::stream ss;
                ss << "No cache data for subchild " << orChild->debugString();
                return Status(ErrorCodes::NoQueryExecutionPlans, ss);
            }

            if (SolutionCacheData::USE_INDEX_TAGS_SOLN != bestSoln->cacheData->solnType) {
                str::stream ss;
                ss << "No indexed cache data for subchild " << orChild->debugString();
                return Status(ErrorCodes::NoQueryExecutionPlans, ss);
            }

            // Add the index assignments to our original query.
            Status tagStatus = QueryPlanner::tagAccordingToCache(
                orChild, bestSoln->cacheData->tree.get(), _indexMap);

            if (!tagStatus.isOK()) {
                str::stream ss;
                ss << "Failed to extract indices from subchild " << orChild->debugString();
                return tagStatus.withContext(ss);
            }

            cacheData->children.push_back(bestSoln->cacheData->tree->clone());
        }
    }

    // Must do this before using the planner functionality.
    prepareForAccessPlanning(_orExpression.get());

    // Use the cached index assignments to build solnRoot. Takes ownership of '_orExpression'.
    std::unique_ptr<QuerySolutionNode> solnRoot(QueryPlannerAccess::buildIndexedDataAccess(
        *_query, std::move(_orExpression), _plannerParams.indices, _plannerParams));

    if (!solnRoot) {
        str::stream ss;
        ss << "Failed to build indexed data path for subplanned query\n";
        return Status(ErrorCodes::NoQueryExecutionPlans, ss);
    }

    LOGV2_DEBUG(20602,
                5,
                "Subplanner: fully tagged tree is {solnRoot}",
                "solnRoot"_attr = redact(solnRoot->toString()));

    _compositeSolution =
        QueryPlannerAnalysis::analyzeDataAccess(*_query, _plannerParams, std::move(solnRoot));

    if (nullptr == _compositeSolution.get()) {
        str::stream ss;
        ss << "Failed to analyze subplanned query";
        return Status(ErrorCodes::NoQueryExecutionPlans, ss);
    }

    LOGV2_DEBUG(20603,
                5,
                "Subplanner: Composite solution is {compositeSolution}",
                "compositeSolution"_attr = redact(_compositeSolution->toString()));

    // Use the index tags from planning each branch to construct the composite solution,
    // and set that solution as our child stage.
    _ws->clear();
    auto root = StageBuilder::build(opCtx(), collection(), *_query, *_compositeSolution.get(), _ws);
    invariant(_children.empty());
    _children.emplace_back(std::move(root));

    return Status::OK();
}

Status SubplanStage::choosePlanWholeQuery(PlanYieldPolicy* yieldPolicy) {
    // Clear out the working set. We'll start with a fresh working set.
    _ws->clear();

    // Use the query planning module to plan the whole query.
    auto statusWithSolutions = QueryPlanner::plan(*_query, _plannerParams);
    if (!statusWithSolutions.isOK()) {
        return statusWithSolutions.getStatus().withContext(
            str::stream() << "error processing query: " << _query->toString()
                          << " planner returned error");
    }
    auto solutions = std::move(statusWithSolutions.getValue());

    if (1 == solutions.size()) {
        // Only one possible plan.  Run it.  Build the stages from the solution.
        auto root = StageBuilder::build(opCtx(), collection(), *_query, *solutions[0], _ws);
        invariant(_children.empty());
        _children.emplace_back(std::move(root));

        // This SubplanStage takes ownership of the query solution.
        _compositeSolution = std::move(solutions.back());
        solutions.pop_back();

        return Status::OK();
    } else {
        // Many solutions. Create a MultiPlanStage to pick the best, update the cache,
        // and so on. The working set will be shared by all candidate plans.
        invariant(_children.empty());
        _children.emplace_back(new MultiPlanStage(expCtx(), collection(), _query));
        MultiPlanStage* multiPlanStage = static_cast<MultiPlanStage*>(child().get());

        for (size_t ix = 0; ix < solutions.size(); ++ix) {
            if (solutions[ix]->cacheData.get()) {
                solutions[ix]->cacheData->indexFilterApplied = _plannerParams.indexFiltersApplied;
            }

            auto nextPlanRoot =
                StageBuilder::build(opCtx(), collection(), *_query, *solutions[ix], _ws);

            multiPlanStage->addPlan(std::move(solutions[ix]), std::move(nextPlanRoot), _ws);
        }

        // Delegate the the MultiPlanStage's plan selection facility.
        Status planSelectStat = multiPlanStage->pickBestPlan(yieldPolicy);
        if (!planSelectStat.isOK()) {
            return planSelectStat;
        }

        return Status::OK();
    }
}

Status SubplanStage::pickBestPlan(PlanYieldPolicy* yieldPolicy) {
    // Adds the amount of time taken by pickBestPlan() to executionTimeMillis. There's lots of
    // work that happens here, so this is needed for the time accounting to make sense.
    ScopedTimer timer(getClock(), &_commonStats.executionTimeMillis);

    // During plan selection, the list of indices we are using to plan must remain stable, so the
    // query will die during yield recovery if any index has been dropped. However, once plan
    // selection completes successfully, we no longer need all indices to stick around. The selected
    // plan should safely die on yield recovery if it is using the dropped index.
    //
    // Dismiss the requirement that no indices can be dropped when this method returns.
    ON_BLOCK_EXIT([this] { releaseAllIndicesRequirement(); });

    // Plan each branch of the $or.
    Status subplanningStatus = planSubqueries();
    if (!subplanningStatus.isOK()) {
        return choosePlanWholeQuery(yieldPolicy);
    }

    // Use the multi plan stage to select a winning plan for each branch, and then construct
    // the overall winning plan from the resulting index tags.
    Status subplanSelectStat = choosePlanForSubqueries(yieldPolicy);
    if (!subplanSelectStat.isOK()) {
        if (subplanSelectStat != ErrorCodes::NoQueryExecutionPlans) {
            // Query planning can continue if we failed to find a solution for one of the
            // children. Otherwise, it cannot, as it may no longer be safe to access the collection
            // (and index may have been dropped, we may have exceeded the time limit, etc).
            return subplanSelectStat;
        }
        return choosePlanWholeQuery(yieldPolicy);
    }

    return Status::OK();
}

bool SubplanStage::isEOF() {
    // If we're running we best have a runner.
    invariant(child());
    return child()->isEOF();
}

PlanStage::StageState SubplanStage::doWork(WorkingSetID* out) {
    if (isEOF()) {
        return PlanStage::IS_EOF;
    }

    invariant(child());
    return child()->work(out);
}

unique_ptr<PlanStageStats> SubplanStage::getStats() {
    _commonStats.isEOF = isEOF();
    unique_ptr<PlanStageStats> ret = std::make_unique<PlanStageStats>(_commonStats, STAGE_SUBPLAN);
    ret->children.emplace_back(child()->getStats());
    return ret;
}

bool SubplanStage::branchPlannedFromCache(size_t i) const {
    return nullptr != _branchResults[i]->cachedSolution.get();
}

const SpecificStats* SubplanStage::getSpecificStats() const {
    return nullptr;
}

}  // namespace mongo
