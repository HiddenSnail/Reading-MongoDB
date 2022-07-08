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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kStorage

#include "mongo/platform/basic.h"

#include "mongo/db/catalog/validate_adaptor.h"

#include "mongo/bson/bsonobj.h"
#include "mongo/db/catalog/collection.h"
#include "mongo/db/catalog/index_catalog.h"
#include "mongo/db/catalog/index_consistency.h"
#include "mongo/db/catalog/throttle_cursor.h"
#include "mongo/db/curop.h"
#include "mongo/db/index/index_access_method.h"
#include "mongo/db/index/index_descriptor.h"
#include "mongo/db/index/wildcard_access_method.h"
#include "mongo/db/matcher/expression.h"
#include "mongo/db/multi_key_path_tracker.h"
#include "mongo/db/operation_context.h"
#include "mongo/db/storage/key_string.h"
#include "mongo/db/storage/record_store.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/object_check.h"
#include "mongo/util/fail_point.h"

namespace mongo {

namespace {

MONGO_FAIL_POINT_DEFINE(crashOnMultikeyValidateFailure);

const long long kInterruptIntervalNumRecords = 4096;
const long long kInterruptIntervalNumBytes = 50 * 1024 * 1024;  // 50MB.

}  // namespace

Status ValidateAdaptor::validateRecord(OperationContext* opCtx,
                                       const RecordId& recordId,
                                       const RecordData& record,
                                       size_t* dataSize) {
    BSONObj recordBson;
    try {
        recordBson = record.toBson();
    } catch (...) {
        return exceptionToStatus();
    }

    if (MONGO_unlikely(_validateState->extraLoggingForTest())) {
        LOGV2(4666601, "[validate]", "recordId"_attr = recordId, "recordData"_attr = recordBson);
    }

    const Status status = validateBSON(
        recordBson.objdata(), recordBson.objsize(), Validator<BSONObj>::enabledBSONVersion());
    if (status.isOK()) {
        *dataSize = recordBson.objsize();
    } else {
        return status;
    }

    const IndexCatalog* indexCatalog = _validateState->getCollection()->getIndexCatalog();
    if (!indexCatalog->haveAnyIndexes()) {
        return status;
    }

    for (const auto& index : _validateState->getIndexes()) {
        const IndexDescriptor* descriptor = index->descriptor();
        const IndexAccessMethod* iam = index->accessMethod();

        if (descriptor->isPartial()) {
            const IndexCatalogEntry* ice = indexCatalog->getEntry(descriptor);
            if (!ice->getFilterExpression()->matchesBSON(recordBson)) {
                continue;
            }
        }

        KeyStringSet documentKeySet;
        KeyStringSet multikeyMetadataKeys;
        MultikeyPaths documentMultikeyPaths;
        iam->getKeys(recordBson,
                     IndexAccessMethod::GetKeysMode::kEnforceConstraints,
                     IndexAccessMethod::GetKeysContext::kAddingKeys,
                     &documentKeySet,
                     &multikeyMetadataKeys,
                     &documentMultikeyPaths,
                     recordId,
                     IndexAccessMethod::kNoopOnSuppressedErrorFn);

        if (!descriptor->isMultikey() &&
            iam->shouldMarkIndexAsMultikey(
                documentKeySet.size(),
                {multikeyMetadataKeys.begin(), multikeyMetadataKeys.end()},
                documentMultikeyPaths)) {
            std::string msg = str::stream()
                << "Index " << descriptor->indexName() << " is not multi-key but has more than one"
                << " key in document " << recordId;
            ValidateResults& curRecordResults = (*_indexNsResultsMap)[descriptor->indexName()];
            curRecordResults.errors.push_back(msg);
            curRecordResults.valid = false;
            if (crashOnMultikeyValidateFailure.shouldFail()) {
                invariant(false, msg);
            }
        }

        if (descriptor->isMultikey()) {
            const MultikeyPaths& indexPaths = descriptor->getMultikeyPaths(opCtx);
            if (!MultikeyPathTracker::covers(indexPaths, documentMultikeyPaths)) {
                std::string msg = str::stream()
                    << "Index " << descriptor->indexName()
                    << " multi-key paths do not cover a document. RecordId: " << recordId;
                ValidateResults& curRecordResults = (*_indexNsResultsMap)[descriptor->indexName()];
                curRecordResults.errors.push_back(msg);
                curRecordResults.valid = false;
            }
        }

        IndexInfo& indexInfo = _indexConsistency->getIndexInfo(descriptor->indexName());
        for (const auto& keyString : multikeyMetadataKeys) {
            try {
                _indexConsistency->addMultikeyMetadataPath(keyString, &indexInfo);
            } catch (...) {
                return exceptionToStatus();
            }
        }

        for (const auto& keyString : documentKeySet) {
            try {
                _totalIndexKeys++;
                _indexConsistency->addDocKey(opCtx, keyString, &indexInfo, recordId);
            } catch (...) {
                return exceptionToStatus();
            }
        }
    }
    return status;
}

namespace {
// Ensures that index entries are in increasing or decreasing order.
void _validateKeyOrder(OperationContext* opCtx,
                       const IndexCatalogEntry* index,
                       const KeyString::Value& currKey,
                       const KeyString::Value& prevKey,
                       ValidateResults* results) {
    auto descriptor = index->descriptor();
    bool unique = descriptor->unique();

    // KeyStrings will be in strictly increasing order because all keys are sorted and they are in
    // the format (Key, RID), and all RecordIDs are unique.
    if (currKey.compare(prevKey) <= 0) {
        if (results && results->valid) {
            results->errors.push_back(str::stream()
                                      << "index '" << descriptor->indexName()
                                      << "' is not in strictly ascending or descending order");
        }
        if (results) {
            results->valid = false;
        }
        return;
    }

    if (unique) {
        // Unique indexes must not have duplicate keys.
        int cmp = currKey.compareWithoutRecordId(prevKey);
        if (cmp != 0) {
            return;
        }

        if (results && results->valid) {
            auto bsonKey = KeyString::toBson(currKey, Ordering::make(descriptor->keyPattern()));
            auto firstRecordId =
                KeyString::decodeRecordIdAtEnd(prevKey.getBuffer(), prevKey.getSize());
            auto secondRecordId =
                KeyString::decodeRecordIdAtEnd(currKey.getBuffer(), currKey.getSize());
            results->errors.push_back(str::stream() << "Unique index '" << descriptor->indexName()
                                                    << "' has duplicate key: " << bsonKey
                                                    << ", first record: " << firstRecordId
                                                    << ", second record: " << secondRecordId);
        }
        if (results) {
            results->valid = false;
        }
    }
}
}  // namespace

void ValidateAdaptor::traverseIndex(OperationContext* opCtx,
                                    const IndexCatalogEntry* index,
                                    int64_t* numTraversedKeys,
                                    ValidateResults* results) {
    const IndexDescriptor* descriptor = index->descriptor();
    auto indexName = descriptor->indexName();
    IndexInfo& indexInfo = _indexConsistency->getIndexInfo(indexName);
    int64_t numKeys = 0;

    bool isFirstEntry = true;

    // The progress meter will be inactive after traversing the record store to allow the message
    // and the total to be set to different values.
    if (!_progress->isActive()) {
        const char* curopMessage = "Validate: scanning index entries";
        stdx::unique_lock<Client> lk(*opCtx->getClient());
        _progress.set(CurOp::get(opCtx)->setProgress_inlock(curopMessage, _totalIndexKeys));
    }

    const KeyString::Version version =
        index->accessMethod()->getSortedDataInterface()->getKeyStringVersion();
    KeyString::Builder firstKeyString(
        version, BSONObj(), indexInfo.ord, KeyString::Discriminator::kExclusiveBefore);

    KeyString::Value prevIndexKeyStringValue;

    // Ensure that this index has an open index cursor.
    const auto indexCursorIt = _validateState->getIndexCursors().find(indexName);
    invariant(indexCursorIt != _validateState->getIndexCursors().end());

    const std::unique_ptr<SortedDataInterfaceThrottleCursor>& indexCursor = indexCursorIt->second;
    for (auto indexEntry = indexCursor->seekForKeyString(opCtx, firstKeyString.getValueCopy());
         indexEntry;
         indexEntry = indexCursor->nextKeyString(opCtx)) {

        if (!isFirstEntry) {
            _validateKeyOrder(
                opCtx, index, indexEntry->keyString, prevIndexKeyStringValue, results);
        }

        const RecordId kWildcardMultikeyMetadataRecordId{
            RecordId::ReservedId::kWildcardMultikeyMetadataId};
        if (descriptor->getIndexType() == IndexType::INDEX_WILDCARD &&
            indexEntry->loc == kWildcardMultikeyMetadataRecordId) {
            _indexConsistency->removeMultikeyMetadataPath(indexEntry->keyString, &indexInfo);
            _progress->hit();
            numKeys++;
            continue;
        }

        _indexConsistency->addIndexKey(indexEntry->keyString, &indexInfo, indexEntry->loc);
        _progress->hit();
        numKeys++;
        isFirstEntry = false;
        prevIndexKeyStringValue = indexEntry->keyString;

        if (numKeys % kInterruptIntervalNumRecords == 0) {
            // Periodically checks for interrupts and yields.
            opCtx->checkForInterrupt();
            _validateState->yield(opCtx);
        }
    }

    if (results && _indexConsistency->getMultikeyMetadataPathCount(&indexInfo) > 0) {
        results->errors.push_back(str::stream()
                                  << "Index '" << descriptor->indexName()
                                  << "' has one or more missing multikey metadata index keys");
        results->valid = false;
    }

    if (numTraversedKeys) {
        *numTraversedKeys = numKeys;
    }
}

void ValidateAdaptor::traverseRecordStore(OperationContext* opCtx,
                                          ValidateResults* results,
                                          BSONObjBuilder* output) {
    _numRecords = 0;  // need to reset it because this function can be called more than once.
    long long dataSizeTotal = 0;
    long long interruptIntervalNumBytes = 0;
    long long nInvalid = 0;

    results->valid = true;
    RecordId prevRecordId;

    // In case validation occurs twice and the progress meter persists after index traversal
    if (_progress.get() && _progress->isActive()) {
        _progress->finished();
    }

    // Because the progress meter is intended as an approximation, it's sufficient to get the number
    // of records when we begin traversing, even if this number may deviate from the final number.
    const char* curopMessage = "Validate: scanning documents";
    const auto totalRecords = _validateState->getCollection()->getRecordStore()->numRecords(opCtx);
    {
        stdx::unique_lock<Client> lk(*opCtx->getClient());
        _progress.set(CurOp::get(opCtx)->setProgress_inlock(curopMessage, totalRecords));
    }

    const std::unique_ptr<SeekableRecordThrottleCursor>& traverseRecordStoreCursor =
        _validateState->getTraverseRecordStoreCursor();
    for (auto record =
             traverseRecordStoreCursor->seekExact(opCtx, _validateState->getFirstRecordId());
         record;
         record = traverseRecordStoreCursor->next(opCtx)) {
        _progress->hit();
        ++_numRecords;
        auto dataSize = record->data.size();
        interruptIntervalNumBytes += dataSize;
        dataSizeTotal += dataSize;
        size_t validatedSize = 0;
        Status status = validateRecord(opCtx, record->id, record->data, &validatedSize);

        // Checks to ensure isInRecordIdOrder() is being used properly.
        if (prevRecordId.isValid()) {
            invariant(prevRecordId < record->id);
        }

        // validatedSize = dataSize is not a general requirement as some storage engines may use
        // padding, but we still require that they return the unpadded record data.
        if (!status.isOK() || validatedSize != static_cast<size_t>(dataSize)) {
            if (!status.isOK() && validatedSize != static_cast<size_t>(dataSize)) {
                LOGV2(4835000,
                      "Document corruption details - Multiple causes for document validation "
                      "failure; error status and size mismatch",
                      "recordId"_attr = record->id,
                      "validatedBytes"_attr = validatedSize,
                      "recordBytes"_attr = dataSize,
                      "error"_attr = status);
            } else if (!status.isOK()) {
                LOGV2(4835001,
                      "Document corruption details - Document validation failed with error",
                      "recordId"_attr = record->id,
                      "error"_attr = status);
            } else {
                LOGV2(4835002,
                      "Document corruption details - Document validation failure; size mismatch",
                      "recordId"_attr = record->id,
                      "validatedBytes"_attr = validatedSize,
                      "recordBytes"_attr = dataSize);
            }

            // Only log once
            if (results->valid) {
                results->errors.push_back("Detected one or more invalid documents. See logs.");
                results->valid = false;
            }
            nInvalid++;
        }

        prevRecordId = record->id;

        if (_numRecords % kInterruptIntervalNumRecords == 0 ||
            interruptIntervalNumBytes >= kInterruptIntervalNumBytes) {
            // Periodically checks for interrupts and yields.
            opCtx->checkForInterrupt();
            _validateState->yield(opCtx);

            if (interruptIntervalNumBytes >= kInterruptIntervalNumBytes) {
                interruptIntervalNumBytes = 0;
            }
        }
    }

    // Do not update the record store stats if we're in the background as we've validated a
    // checkpoint and it may not have the most up-to-date changes.
    if (results->valid && !_validateState->isBackground()) {
        _validateState->getCollection()->getRecordStore()->updateStatsAfterRepair(
            opCtx, _numRecords, dataSizeTotal);
    }

    _progress->finished();

    output->appendNumber("nInvalidDocuments", nInvalid);
    output->appendNumber("nrecords", _numRecords);
}

void ValidateAdaptor::validateIndexKeyCount(const IndexDescriptor* idx, ValidateResults& results) {
    // Fetch the total number of index entries we previously found traversing the index.
    const std::string indexName = idx->indexName();
    IndexInfo* indexInfo = &_indexConsistency->getIndexInfo(indexName);
    auto numTotalKeys = indexInfo->numKeys;

    // Do not fail on finding too few index entries compared to collection entries when full:false.
    bool hasTooFewKeys = false;
    bool noErrorOnTooFewKeys = !_validateState->isFullIndexValidation();

    if (idx->isIdIndex() && numTotalKeys != _numRecords) {
        hasTooFewKeys = (numTotalKeys < _numRecords);
        std::string msg = str::stream()
            << "number of _id index entries (" << numTotalKeys
            << ") does not match the number of documents in the index (" << _numRecords << ")";
        if (noErrorOnTooFewKeys && (numTotalKeys < _numRecords)) {
            results.warnings.push_back(msg);
        } else {
            results.errors.push_back(msg);
            results.valid = false;
        }
    }

    // Confirm that the number of index entries is not greater than the number of documents in the
    // collection. This check is only valid for indexes that are not multikey (indexed arrays
    // produce an index key per array entry) and not $** indexes which can produce index keys for
    // multiple paths within a single document.
    if (results.valid && !idx->isMultikey() && idx->getIndexType() != IndexType::INDEX_WILDCARD &&
        numTotalKeys > _numRecords) {
        std::string err = str::stream()
            << "index " << idx->indexName() << " is not multi-key, but has more entries ("
            << numTotalKeys << ") than documents in the index (" << _numRecords << ")";
        results.errors.push_back(err);
        results.valid = false;
    }

    // Ignore any indexes with a special access method. If an access method name is given, the
    // index may be a full text, geo or special index plugin with different semantics.
    if (results.valid && !idx->isSparse() && !idx->isPartial() && !idx->isIdIndex() &&
        idx->getAccessMethodName() == "" && numTotalKeys < _numRecords) {
        hasTooFewKeys = true;
        std::string msg = str::stream()
            << "index " << idx->indexName() << " is not sparse or partial, but has fewer entries ("
            << numTotalKeys << ") than documents in the index (" << _numRecords << ")";
        if (noErrorOnTooFewKeys) {
            results.warnings.push_back(msg);
        } else {
            results.errors.push_back(msg);
            results.valid = false;
        }
    }

    if (!_validateState->isFullIndexValidation() && hasTooFewKeys) {
        std::string warning = str::stream()
            << "index " << idx->indexName() << " has fewer keys than records."
            << " Please re-run the validate command with {full: true}";
        results.warnings.push_back(warning);
    }
}
}  // namespace mongo
