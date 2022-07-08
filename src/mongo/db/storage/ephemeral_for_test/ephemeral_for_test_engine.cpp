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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kStorage

#include "mongo/platform/basic.h"

#include "mongo/db/storage/ephemeral_for_test/ephemeral_for_test_engine.h"

#include <memory>

#include "mongo/db/index/index_descriptor.h"
#include "mongo/db/storage/ephemeral_for_test/ephemeral_for_test_btree_impl.h"
#include "mongo/db/storage/ephemeral_for_test/ephemeral_for_test_record_store.h"
#include "mongo/db/storage/ephemeral_for_test/ephemeral_for_test_recovery_unit.h"
#include "mongo/db/storage/journal_listener.h"
#include "mongo/logv2/log.h"

namespace mongo {

RecoveryUnit* EphemeralForTestEngine::newRecoveryUnit() {
    return new EphemeralForTestRecoveryUnit([this]() {
        stdx::lock_guard<Latch> lk(_mutex);
        if (_journalListener) {
            JournalListener::Token token = _journalListener->getToken(nullptr);
            _journalListener->onDurable(token);
        }
    });
}

Status EphemeralForTestEngine::createRecordStore(OperationContext* opCtx,
                                                 StringData ns,
                                                 StringData ident,
                                                 const CollectionOptions& options) {
    // Register the ident in the `_dataMap` (for `getAllIdents`). Remainder of work done in
    // `getRecordStore`.
    stdx::lock_guard<Latch> lk(_mutex);
    _dataMap[ident] = {};
    return Status::OK();
}

std::unique_ptr<RecordStore> EphemeralForTestEngine::getRecordStore(
    OperationContext* opCtx, StringData ns, StringData ident, const CollectionOptions& options) {
    stdx::lock_guard<Latch> lk(_mutex);
    if (options.capped) {
        return std::make_unique<EphemeralForTestRecordStore>(
            ns,
            &_dataMap[ident],
            true,
            options.cappedSize ? options.cappedSize : kDefaultCappedSizeBytes,
            options.cappedMaxDocs ? options.cappedMaxDocs : -1);
    } else {
        return std::make_unique<EphemeralForTestRecordStore>(ns, &_dataMap[ident]);
    }
}

std::unique_ptr<RecordStore> EphemeralForTestEngine::makeTemporaryRecordStore(
    OperationContext* opCtx, StringData ident) {
    stdx::lock_guard<Latch> lk(_mutex);
    _dataMap[ident] = {};
    return std::make_unique<EphemeralForTestRecordStore>(ident, &_dataMap[ident]);
}

Status EphemeralForTestEngine::createSortedDataInterface(OperationContext* opCtx,
                                                         const CollectionOptions& collOptions,
                                                         StringData ident,
                                                         const IndexDescriptor* desc) {
    // Register the ident in `_dataMap` (for `getAllIdents`). Remainder of work done in
    // `getSortedDataInterface`.
    stdx::lock_guard<Latch> lk(_mutex);
    _dataMap[ident] = {};
    return Status::OK();
}

std::unique_ptr<SortedDataInterface> EphemeralForTestEngine::getSortedDataInterface(
    OperationContext* opCtx, StringData ident, const IndexDescriptor* desc) {
    stdx::lock_guard<Latch> lk(_mutex);
    return getEphemeralForTestBtreeImpl(Ordering::make(desc->keyPattern()),
                                        desc->unique(),
                                        desc->parentNS(),
                                        desc->indexName(),
                                        desc->keyPattern(),
                                        desc->collation(),
                                        &_dataMap[ident]);
}

Status EphemeralForTestEngine::dropIdent(OperationContext* opCtx,
                                         RecoveryUnit* ru,
                                         StringData ident) {
    stdx::lock_guard<Latch> lk(_mutex);
    _dataMap.erase(ident);
    return Status::OK();
}

int64_t EphemeralForTestEngine::getIdentSize(OperationContext* opCtx, StringData ident) {
    return 1;
}

std::vector<std::string> EphemeralForTestEngine::getAllIdents(OperationContext* opCtx) const {
    std::vector<std::string> all;
    {
        stdx::lock_guard<Latch> lk(_mutex);
        for (DataMap::const_iterator it = _dataMap.begin(); it != _dataMap.end(); ++it) {
            all.push_back(it->first);
        }
    }
    return all;
}
}  // namespace mongo
