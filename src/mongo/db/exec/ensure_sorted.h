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

#include "mongo/db/exec/plan_stage.h"
#include "mongo/db/exec/sort_key_comparator.h"

namespace mongo {

/**
 * Takes the output of its child and drops any results that are not in the sort order specified by
 * 'pattern'. Thus, if the pattern is {a: -1} and the following documents are inputted:
 *      {a: 9}, {a: 6}, {a: 8}, {a, 1}
 * The third document will be dropped so that the output is:
 *      {a: 9}, {a: 6}, {a, 1}
 */
class EnsureSortedStage final : public PlanStage {
public:
    EnsureSortedStage(ExpressionContext* expCtx,
                      BSONObj pattern,
                      WorkingSet* ws,
                      std::unique_ptr<PlanStage> child);

    bool isEOF() final;
    StageState doWork(WorkingSetID* out) final;

    StageType stageType() const final {
        return STAGE_ENSURE_SORTED;
    }

    std::unique_ptr<PlanStageStats> getStats() final;

    const SpecificStats* getSpecificStats() const final;

    static const char* kStageType;

private:
    /**
     * Returns whether the result with the lhsSortKey should come before the result with the
     * rhsSortKey in sort order.
     */
    bool isInOrder(const Value& lhsSortKey, const Value& rhsSortKey) const;

    WorkingSet* _ws;

    // Comparator that is aware of the pattern that we're sorting by.
    SortKeyComparator _sortKeyComparator;

    // The sort key of the previous result.
    Value _prevSortKey;

    EnsureSortedStats _specificStats;
};

}  // namespace mongo
