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

#pragma once


#include "mongo/db/catalog/collection.h"
#include "mongo/db/exec/requires_collection_stage.h"
#include "mongo/db/jsobj.h"
#include "mongo/db/ops/parsed_update.h"
#include "mongo/db/ops/update_request.h"
#include "mongo/db/ops/update_result.h"
#include "mongo/db/s/collection_sharding_state.h"
#include "mongo/db/update/update_driver.h"

namespace mongo {

class OperationContext;
class OpDebug;
struct PlanSummaryStats;

struct UpdateStageParams {
    UpdateStageParams(const UpdateRequest* r, UpdateDriver* d, OpDebug* o)
        : request(r), driver(d), opDebug(o), canonicalQuery(nullptr) {}

    // Contains update parameters like whether it's a multi update or an upsert. Not owned.
    // Must outlive the UpdateStage.
    const UpdateRequest* request;

    // Contains the logic for applying mods to documents. Not owned. Must outlive
    // the UpdateStage.
    UpdateDriver* driver;

    // Needed to pass to Collection::updateDocument(...).
    OpDebug* opDebug;

    // Not owned here.
    CanonicalQuery* canonicalQuery;

private:
    // Default constructor not allowed.
    UpdateStageParams();
};

/**
 * Execution stage responsible for updates to documents. If the prior or newly-updated version of
 * the document was requested to be returned, then ADVANCED is returned after updating a document.
 * Otherwise, NEED_TIME is returned after updating a document if further updates are pending,
 * and IS_EOF is returned if no documents were found or all updates have been performed.
 *
 * Callers of doWork() must be holding a write lock.
 */
class UpdateStage : public RequiresMutableCollectionStage {
    UpdateStage(const UpdateStage&) = delete;
    UpdateStage& operator=(const UpdateStage&) = delete;

public:
    UpdateStage(ExpressionContext* expCtx,
                const UpdateStageParams& params,
                WorkingSet* ws,
                Collection* collection,
                PlanStage* child);

    bool isEOF() override;
    StageState doWork(WorkingSetID* out) override;

    StageType stageType() const final {
        return STAGE_UPDATE;
    }

    std::unique_ptr<PlanStageStats> getStats() final;

    const SpecificStats* getSpecificStats() const final;

    static const char* kStageType;

    /**
     * Gets a pointer to the UpdateStats inside 'exec'.
     *
     * The 'exec' must have an UPDATE stage as its root stage, and the plan must be EOF before
     * calling this method.
     */
    static const UpdateStats* getUpdateStats(const PlanExecutor* exec);

    /**
     * Populate 'opDebug' with stats from 'updateStats' describing the execution of this update.
     */
    static void recordUpdateStatsInOpDebug(const UpdateStats* updateStats, OpDebug* opDebug);

    /**
     * Converts 'updateStats' into an UpdateResult.
     */
    static UpdateResult makeUpdateResult(const UpdateStats* updateStats);

    /**
     * Returns true if an update failure due to a given DuplicateKey error is eligible for retry.
     * Requires that parsedUpdate.hasParsedQuery() is true.
     */
    static bool shouldRetryDuplicateKeyException(const ParsedUpdate& parsedUpdate,
                                                 const DuplicateKeyErrorInfo& errorInfo);

protected:
    UpdateStage(ExpressionContext* expCtx,
                const UpdateStageParams& params,
                WorkingSet* ws,
                Collection* collection);

    void doSaveStateRequiresCollection() final {}

    void doRestoreStateRequiresCollection() final;

    void _ensureIdFieldIsFirst(mutablebson::Document* doc, bool generateOIDIfMissing);

    void _assertPathsNotArray(const mutablebson::Document& document, const FieldRefSet& paths);

    UpdateStageParams _params;

    // Not owned by us.
    WorkingSet* _ws;

    // Stats
    UpdateStats _specificStats;

    // True if the request should be checked for an update to the shard key.
    bool _shouldCheckForShardKeyUpdate;

    // True if updated documents should be validated with storage_validation::storageValid().
    bool _enforceOkForStorage;

    // These get reused for each update.
    mutablebson::Document& _doc;
    mutablebson::DamageVector _damages;

private:
    static const UpdateStats kEmptyUpdateStats;

    /**
     * Returns whether a given MatchExpression contains is a MatchType::EQ or a MatchType::AND node
     * with only MatchType::EQ children.
     */
    static bool matchContainsOnlyAndedEqualityNodes(const MatchExpression& root);

    /**
     * Computes the result of applying mods to the document 'oldObj' at RecordId 'recordId' in
     * memory, then commits these changes to the database. Returns a possibly unowned copy
     * of the newly-updated version of the document.
     */
    BSONObj transformAndUpdate(const Snapshotted<BSONObj>& oldObj, RecordId& recordId);

    /**
     * Stores 'idToRetry' in '_idRetrying' so the update can be retried during the next call to
     * doWork(). Always returns NEED_YIELD and sets 'out' to WorkingSet::INVALID_ID.
     */
    StageState prepareToRetryWSM(WorkingSetID idToRetry, WorkingSetID* out);

    /**
     * Checks that the updated doc has all required shard key fields and throws if it does not.
     *
     * Also checks if the updated doc still belongs to this node and throws if it does not. If the
     * doc no longer belongs to this shard, this means that one or more shard key field values have
     * been updated to a value belonging to a chunk that is not owned by this shard. We cannot apply
     * this update atomically.
     *
     * If the update changes shard key fields but the new shard key remains on the same node,
     * returns true. If the update does not change shard key fields, returns false.
     */
    bool checkUpdateChangesShardKeyFields(ScopedCollectionDescription collDesc,
                                          const Snapshotted<BSONObj>& oldObj);

    // If not WorkingSet::INVALID_ID, we use this rather than asking our child what to do next.
    WorkingSetID _idRetrying;

    // If not WorkingSet::INVALID_ID, we return this member to our caller.
    WorkingSetID _idReturning;

    // If the update was in-place, we may see it again.  This only matters if we're doing
    // a multi-update; if we're not doing a multi-update we stop after one update and we
    // won't see any more docs.
    //
    // For example: If we're scanning an index {x:1} and performing {$inc:{x:5}}, we'll keep
    // moving the document forward and it will continue to reappear in our index scan.
    // Unless the index is multikey, the underlying query machinery won't de-dup.
    //
    // If the update wasn't in-place we may see it again.  Our query may return the new
    // document and we wouldn't want to update that.
    //
    // So, no matter what, we keep track of where the doc wound up.
    typedef stdx::unordered_set<RecordId, RecordId::Hasher> RecordIdSet;
    const std::unique_ptr<RecordIdSet> _updatedRecordIds;
};

}  // namespace mongo
