/**
 *    Copyright (C) 2019-present MongoDB, Inc.
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

#include "mongo/db/exec/return_key.h"

#include "mongo/db/exec/document_value/document.h"
#include "mongo/db/exec/working_set_common.h"
#include "mongo/db/pipeline/field_path.h"
#include "mongo/logv2/log.h"

namespace mongo {
PlanStage::StageState ReturnKeyStage::doWork(WorkingSetID* out) {
    WorkingSetID id = WorkingSet::INVALID_ID;
    StageState status = child()->work(&id);

    if (PlanStage::ADVANCED == status) {
        WorkingSetMember* member = _ws.get(id);
        Status indexKeyStatus = _extractIndexKey(member);

        if (!indexKeyStatus.isOK()) {
            LOGV2_WARNING(4615614,
                          "Couldn't execute {stage}, status = {indexKeyStatus}",
                          "stage"_attr = kStageName,
                          "indexKeyStatus"_attr = redact(indexKeyStatus));
            *out = WorkingSetCommon::allocateStatusMember(&_ws, indexKeyStatus);
            return PlanStage::FAILURE;
        }

        *out = id;
    } else if (PlanStage::FAILURE == status) {
        // The stage which produces a failure is responsible for allocating a working set member
        // with error details.
        invariant(WorkingSet::INVALID_ID != id);
        *out = id;
    } else if (PlanStage::NEED_YIELD == status) {
        *out = id;
    }

    return status;
}

std::unique_ptr<PlanStageStats> ReturnKeyStage::getStats() {
    _commonStats.isEOF = isEOF();

    auto ret = std::make_unique<PlanStageStats>(_commonStats, stageType());
    ret->specific = std::make_unique<ReturnKeyStats>(_specificStats);
    ret->children.emplace_back(child()->getStats());
    return ret;
}

Status ReturnKeyStage::_extractIndexKey(WorkingSetMember* member) {
    if (!_sortKeyMetaFields.empty()) {
        invariant(member->metadata().hasSortKey());
    }

    auto indexKey = member->metadata().hasIndexKey() ? member->metadata().getIndexKey() : BSONObj();

    MutableDocument md;

    for (auto&& elem : indexKey) {
        md.addField(elem.fieldNameStringData(), Value(elem));
    }

    for (const auto& fieldPath : _sortKeyMetaFields) {
        if (!member->metadata().hasSortKey()) {
            md.setNestedField(fieldPath, Value{});
            continue;
        }

        switch (_sortKeyFormat) {
            case SortKeyFormat::k44SortKey:
                md.setNestedField(
                    fieldPath,
                    Value(DocumentMetadataFields::serializeSortKeyAsArray(
                        member->metadata().isSingleElementKey(), member->metadata().getSortKey())));
                break;
            case SortKeyFormat::k42SortKey:
                md.setNestedField(
                    fieldPath,
                    Value(DocumentMetadataFields::serializeSortKeyAsObject(
                        member->metadata().isSingleElementKey(), member->metadata().getSortKey())));
                break;
            default:
                MONGO_UNREACHABLE;
        }
    }

    member->keyData.clear();
    member->recordId = {};
    member->doc = {{}, md.freeze()};
    member->transitionToOwnedObj();

    return Status::OK();
}
}  // namespace mongo
