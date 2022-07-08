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

#include "mongo/db/catalog/database_impl.h"

#include <algorithm>
#include <memory>
#include <vector>

#include <boost/filesystem/operations.hpp>

#include "mongo/base/init.h"
#include "mongo/db/audit.h"
#include "mongo/db/background.h"
#include "mongo/db/catalog/collection_catalog.h"
#include "mongo/db/catalog/collection_catalog_helper.h"
#include "mongo/db/catalog/collection_impl.h"
#include "mongo/db/catalog/collection_options.h"
#include "mongo/db/catalog/database_holder.h"
#include "mongo/db/catalog/drop_indexes.h"
#include "mongo/db/catalog/index_catalog.h"
#include "mongo/db/catalog/uncommitted_collections.h"
#include "mongo/db/clientcursor.h"
#include "mongo/db/commands/feature_compatibility_version_parser.h"
#include "mongo/db/concurrency/d_concurrency.h"
#include "mongo/db/concurrency/write_conflict_exception.h"
#include "mongo/db/curop.h"
#include "mongo/db/index/index_access_method.h"
#include "mongo/db/index_builds_coordinator.h"
#include "mongo/db/introspect.h"
#include "mongo/db/op_observer.h"
#include "mongo/db/query/collation/collator_factory_interface.h"
#include "mongo/db/repl/drop_pending_collection_reaper.h"
#include "mongo/db/repl/oplog.h"
#include "mongo/db/repl/replication_coordinator.h"
#include "mongo/db/s/operation_sharding_state.h"
#include "mongo/db/server_options.h"
#include "mongo/db/service_context.h"
#include "mongo/db/stats/top.h"
#include "mongo/db/storage/durable_catalog.h"
#include "mongo/db/storage/recovery_unit.h"
#include "mongo/db/storage/storage_engine.h"
#include "mongo/db/storage/storage_engine_init.h"
#include "mongo/db/storage/storage_options.h"
#include "mongo/db/system_index.h"
#include "mongo/db/views/view_catalog.h"
#include "mongo/logv2/log.h"
#include "mongo/platform/random.h"
#include "mongo/s/cannot_implicitly_create_collection_info.h"
#include "mongo/util/assert_util.h"
#include "mongo/util/fail_point.h"

namespace mongo {

namespace {
MONGO_FAIL_POINT_DEFINE(throwWCEDuringTxnCollCreate);
MONGO_FAIL_POINT_DEFINE(hangBeforeLoggingCreateCollection);
MONGO_FAIL_POINT_DEFINE(hangAndFailAfterCreateCollectionReservesOpTime);
MONGO_FAIL_POINT_DEFINE(openCreateCollectionWindowFp);

Status validateDBNameForWindows(StringData dbname) {
    const std::vector<std::string> windowsReservedNames = {
        "con",  "prn",  "aux",  "nul",  "com1", "com2", "com3", "com4", "com5", "com6", "com7",
        "com8", "com9", "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9"};

    std::string lower(dbname.toString());
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (std::count(windowsReservedNames.begin(), windowsReservedNames.end(), lower))
        return Status(ErrorCodes::BadValue,
                      str::stream() << "db name \"" << dbname << "\" is a reserved name");
    return Status::OK();
}

// Random number generator used to create unique collection namespaces suitable for temporary
// collections.
PseudoRandom uniqueCollectionNamespacePseudoRandom(Date_t::now().asInt64());

Mutex uniqueCollectionNamespaceMutex = MONGO_MAKE_LATCH("DatabaseUniqueCollectionNamespaceMutex");
}  // namespace

Status DatabaseImpl::validateDBName(StringData dbname) {
    if (dbname.size() <= 0)
        return Status(ErrorCodes::BadValue, "db name is empty");

    if (dbname.size() >= 64)
        return Status(ErrorCodes::BadValue, "db name is too long");

    if (dbname.find('.') != std::string::npos)
        return Status(ErrorCodes::BadValue, "db name cannot contain a .");

    if (dbname.find(' ') != std::string::npos)
        return Status(ErrorCodes::BadValue, "db name cannot contain a space");

#ifdef _WIN32
    return validateDBNameForWindows(dbname);
#endif

    return Status::OK();
}

DatabaseImpl::DatabaseImpl(const StringData name, uint64_t epoch)
    : _name(name.toString()),
      _epoch(epoch),
      _viewsName(_name + "." + DurableViewCatalog::viewsCollectionName().toString()) {
    auto durableViewCatalog = std::make_unique<DurableViewCatalogImpl>(this);
    auto viewCatalog = std::make_unique<ViewCatalog>(std::move(durableViewCatalog));

    ViewCatalog::set(this, std::move(viewCatalog));
    _profile.store(serverGlobalParams.defaultProfile);
}

void DatabaseImpl::init(OperationContext* const opCtx) const {
    Status status = validateDBName(_name);

    if (!status.isOK()) {
        LOGV2_WARNING(20325,
                      "tried to open invalid db: {name}",
                      "Tried to open invalid db",
                      "db"_attr = _name);
        uasserted(10028, status.toString());
    }

    auto& catalog = CollectionCatalog::get(opCtx);
    for (const auto& uuid : catalog.getAllCollectionUUIDsFromDb(_name)) {
        auto collection = catalog.lookupCollectionByUUID(opCtx, uuid);
        invariant(collection);
        // If this is called from the repair path, the collection is already initialized.
        if (!collection->isInitialized())
            collection->init(opCtx);
    }

    // At construction time of the viewCatalog, the CollectionCatalog map wasn't initialized yet,
    // so no system.views collection would be found. Now that we're sufficiently initialized, reload
    // the viewCatalog to populate its in-memory state. If there are problems with the catalog
    // contents as might be caused by incorrect mongod versions or similar, they are found right
    // away.
    auto views = ViewCatalog::get(this);
    Status reloadStatus = views->reload(opCtx, ViewCatalogLookupBehavior::kValidateDurableViews);
    if (!reloadStatus.isOK()) {
        LOGV2_WARNING_OPTIONS(20326,
                              {logv2::LogTag::kStartupWarnings},
                              "Unable to parse views; remove any invalid views "
                              "from the collection to restore server functionality",
                              "error"_attr = redact(reloadStatus),
                              "namespace"_attr = _viewsName);
    }
}

void DatabaseImpl::clearTmpCollections(OperationContext* opCtx) const {
    invariant(opCtx->lockState()->isDbLockedForMode(name(), MODE_IX));

    CollectionCatalog::CollectionInfoFn callback = [&](const Collection* collection) {
        try {
            WriteUnitOfWork wuow(opCtx);
            Status status = dropCollection(opCtx, collection->ns(), {});
            if (!status.isOK()) {
                LOGV2_WARNING(20327,
                              "could not drop temp collection '{namespace}': {error}",
                              "Could not drop temp collection",
                              "namespace"_attr = collection->ns(),
                              "error"_attr = redact(status));
            }
            wuow.commit();
        } catch (const WriteConflictException&) {
            LOGV2_WARNING(
                20328,
                "could not drop temp collection '{namespace}' due to WriteConflictException",
                "Could not drop temp collection due to WriteConflictException",
                "namespace"_attr = collection->ns());
            opCtx->recoveryUnit()->abandonSnapshot();
        }
        return true;
    };

    CollectionCatalog::CollectionInfoFn predicate = [&](const Collection* collection) {
        return DurableCatalog::get(opCtx)
            ->getCollectionOptions(opCtx, collection->getCatalogId())
            .temp;
    };

    catalog::forEachCollectionFromDb(opCtx, name(), MODE_X, callback, predicate);
}

Status DatabaseImpl::setProfilingLevel(OperationContext* opCtx, int newLevel) {
    auto currLevel = _profile.load();

    if (currLevel == newLevel) {
        return Status::OK();
    }

    if (newLevel == 0) {
        _profile.store(0);
        return Status::OK();
    }

    if (newLevel < 0 || newLevel > 2) {
        return Status(ErrorCodes::BadValue, "profiling level has to be >=0 and <= 2");
    }

    // Can't support profiling without supporting capped collections.
    if (!opCtx->getServiceContext()->getStorageEngine()->supportsCappedCollections()) {
        return Status(ErrorCodes::CommandNotSupported,
                      "the storage engine doesn't support profiling.");
    }

    Status status = createProfileCollection(opCtx, this);

    if (!status.isOK()) {
        return status;
    }

    _profile.store(newLevel);

    return Status::OK();
}

void DatabaseImpl::setDropPending(OperationContext* opCtx, bool dropPending) {
    auto mode = dropPending ? MODE_X : MODE_IX;
    invariant(opCtx->lockState()->isDbLockedForMode(name(), mode));
    _dropPending.store(dropPending);
}

bool DatabaseImpl::isDropPending(OperationContext* opCtx) const {
    invariant(opCtx->lockState()->isDbLockedForMode(name(), MODE_IS));
    return _dropPending.load();
}

void DatabaseImpl::getStats(OperationContext* opCtx, BSONObjBuilder* output, double scale) const {

    long long nCollections = 0;
    long long nViews = 0;
    long long objects = 0;
    long long size = 0;
    long long storageSize = 0;
    long long indexes = 0;
    long long indexSize = 0;

    invariant(opCtx->lockState()->isDbLockedForMode(name(), MODE_IS));

    catalog::forEachCollectionFromDb(
        opCtx, name(), MODE_IS, [&](const Collection* collection) -> bool {
            nCollections += 1;
            objects += collection->numRecords(opCtx);
            size += collection->dataSize(opCtx);

            BSONObjBuilder temp;
            storageSize += collection->getRecordStore()->storageSize(opCtx, &temp);

            indexes += collection->getIndexCatalog()->numIndexesTotal(opCtx);
            indexSize += collection->getIndexSize(opCtx);

            return true;
        });

    ViewCatalog::get(this)->iterate(opCtx, [&](const ViewDefinition& view) { nViews += 1; });

    output->appendNumber("collections", nCollections);
    output->appendNumber("views", nViews);
    output->appendNumber("objects", objects);
    output->append("avgObjSize", objects == 0 ? 0 : double(size) / double(objects));
    output->appendNumber("dataSize", size / scale);
    output->appendNumber("storageSize", storageSize / scale);
    output->appendNumber("indexes", indexes);
    output->appendNumber("indexSize", indexSize / scale);
    output->appendNumber("totalSize", (storageSize + indexSize) / scale);
    output->appendNumber("scaleFactor", scale);

    if (!opCtx->getServiceContext()->getStorageEngine()->isEphemeral()) {
        boost::filesystem::path dbpath(
            opCtx->getServiceContext()->getStorageEngine()->getFilesystemPathForDb(_name));
        boost::system::error_code ec;
        boost::filesystem::space_info spaceInfo = boost::filesystem::space(dbpath, ec);
        if (!ec) {
            output->appendNumber("fsUsedSize", (spaceInfo.capacity - spaceInfo.available) / scale);
            output->appendNumber("fsTotalSize", spaceInfo.capacity / scale);
        } else {
            output->appendNumber("fsUsedSize", -1);
            output->appendNumber("fsTotalSize", -1);
            LOGV2(20312,
                  "Failed to query filesystem disk stats (code: {ec_value}): {ec_message}",
                  "Failed to query filesystem disk stats",
                  "error"_attr = ec.message(),
                  "errorCode"_attr = ec.value());
        }
    }
}

Status DatabaseImpl::dropView(OperationContext* opCtx, NamespaceString viewName) const {
    dassert(opCtx->lockState()->isDbLockedForMode(name(), MODE_IX));
    dassert(opCtx->lockState()->isCollectionLockedForMode(viewName, MODE_IX));
    dassert(opCtx->lockState()->isCollectionLockedForMode(NamespaceString(_viewsName), MODE_X));

    auto views = ViewCatalog::get(this);
    Status status = views->dropView(opCtx, viewName);
    Top::get(opCtx->getServiceContext()).collectionDropped(viewName);
    return status;
}

Status DatabaseImpl::dropCollection(OperationContext* opCtx,
                                    NamespaceString nss,
                                    repl::OpTime dropOpTime) const {
    // Cannot drop uncommitted collections.
    invariant(!UncommittedCollections::getForTxn(opCtx, nss));

    if (!CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, nss)) {
        // Collection doesn't exist so don't bother validating if it can be dropped.
        return Status::OK();
    }

    invariant(nss.db() == _name);

    if (nss.isSystem()) {
        if (nss.isSystemDotProfile()) {
            if (_profile.load() != 0)
                return Status(ErrorCodes::IllegalOperation,
                              "turn off profiling before dropping system.profile collection");
        } else if (!(nss.isSystemDotViews() || nss.isHealthlog() ||
                     nss == NamespaceString::kLogicalSessionsNamespace ||
                     nss == NamespaceString::kSystemKeysNamespace)) {
            return Status(ErrorCodes::IllegalOperation,
                          str::stream() << "can't drop system collection " << nss);
        }
    }

    return dropCollectionEvenIfSystem(opCtx, nss, dropOpTime);
}

Status DatabaseImpl::dropCollectionEvenIfSystem(OperationContext* opCtx,
                                                NamespaceString nss,
                                                repl::OpTime dropOpTime) const {
    invariant(opCtx->lockState()->isCollectionLockedForMode(nss, MODE_X));

    LOGV2_DEBUG(20313, 1, "dropCollection: {namespace}", "dropCollection", "namespace"_attr = nss);

    // A valid 'dropOpTime' is not allowed when writes are replicated.
    if (!dropOpTime.isNull() && opCtx->writesAreReplicated()) {
        return Status(
            ErrorCodes::BadValue,
            "dropCollection() cannot accept a valid drop optime when writes are replicated.");
    }

    Collection* collection = CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, nss);

    if (!collection) {
        return Status::OK();  // Post condition already met.
    }

    auto numRecords = collection->numRecords(opCtx);

    auto uuid = collection->uuid();

    // Make sure no indexes builds are in progress.
    // Use massert() to be consistent with IndexCatalog::dropAllIndexes().
    auto numIndexesInProgress = collection->getIndexCatalog()->numIndexesInProgress(opCtx);
    massert(ErrorCodes::BackgroundOperationInProgressForNamespace,
            str::stream() << "cannot drop collection " << nss << " (" << uuid << ") when "
                          << numIndexesInProgress << " index builds in progress.",
            numIndexesInProgress == 0);

    audit::logDropCollection(&cc(), nss.toString());

    auto serviceContext = opCtx->getServiceContext();
    Top::get(serviceContext).collectionDropped(nss);

    // Drop unreplicated collections immediately.
    // If 'dropOpTime' is provided, we should proceed to rename the collection.
    auto replCoord = repl::ReplicationCoordinator::get(opCtx);
    auto opObserver = serviceContext->getOpObserver();
    auto isOplogDisabledForNamespace = replCoord->isOplogDisabledFor(opCtx, nss);
    if (dropOpTime.isNull() && isOplogDisabledForNamespace) {
        _dropCollectionIndexes(opCtx, nss, collection);
        opObserver->onDropCollection(
            opCtx, nss, uuid, numRecords, OpObserver::CollectionDropType::kOnePhase);
        return _finishDropCollection(opCtx, nss, collection);
    }

    // Replicated collections should be dropped in two phases.

    // New two-phase drop: Starting in 4.2, pending collection drops will be maintained in the
    // storage engine and will no longer be visible at the catalog layer with 3.6-style
    // <db>.system.drop.* namespaces.
    if (serviceContext->getStorageEngine()->supportsPendingDrops()) {
        _dropCollectionIndexes(opCtx, nss, collection);

        auto commitTimestamp = opCtx->recoveryUnit()->getCommitTimestamp();
        LOGV2(20314,
              "dropCollection: {namespace} ({uuid}) - storage engine will take ownership of "
              "drop-pending "
              "collection with optime {dropOpTime} and commit timestamp {commitTimestamp}",
              "dropCollection: storage engine will take ownership of drop-pending "
              "collection",
              "namespace"_attr = nss,
              "uuid"_attr = uuid,
              "dropOpTime"_attr = dropOpTime,
              "commitTimestamp"_attr = commitTimestamp);
        if (dropOpTime.isNull()) {
            // Log oplog entry for collection drop and remove the UUID.
            dropOpTime = opObserver->onDropCollection(
                opCtx, nss, uuid, numRecords, OpObserver::CollectionDropType::kOnePhase);
            invariant(!dropOpTime.isNull());
        } else {
            // If we are provided with a valid 'dropOpTime', it means we are dropping this
            // collection in the context of applying an oplog entry on a secondary.
            auto opTime = opObserver->onDropCollection(
                opCtx, nss, uuid, numRecords, OpObserver::CollectionDropType::kOnePhase);
            // OpObserver::onDropCollection should not be writing to the oplog on the secondary.
            invariant(opTime.isNull(),
                      str::stream() << "OpTime is not null. OpTime: " << opTime.toString());
        }

        return _finishDropCollection(opCtx, nss, collection);
    }

    // Old two-phase drop: Replicated collections will be renamed with a special drop-pending
    // namespace and dropped when the replica set optime reaches the drop optime.

    if (dropOpTime.isNull()) {
        // Log oplog entry for collection drop.
        dropOpTime = opObserver->onDropCollection(
            opCtx, nss, uuid, numRecords, OpObserver::CollectionDropType::kTwoPhase);
        invariant(!dropOpTime.isNull());
    } else {
        // If we are provided with a valid 'dropOpTime', it means we are dropping this
        // collection in the context of applying an oplog entry on a secondary.
        auto opTime = opObserver->onDropCollection(
            opCtx, nss, uuid, numRecords, OpObserver::CollectionDropType::kTwoPhase);
        // OpObserver::onDropCollection should not be writing to the oplog on the secondary.
        invariant(opTime.isNull());
    }

    // Rename collection using drop-pending namespace generated from drop optime.
    auto dpns = nss.makeDropPendingNamespace(dropOpTime);
    const bool stayTemp = true;
    LOGV2(20315,
          "dropCollection: {namespace} ({uuid}) - renaming to drop-pending collection: "
          "{dropPendingName} with drop "
          "optime {dropOpTime}",
          "dropCollection: renaming to drop-pending collection",
          "namespace"_attr = nss,
          "uuid"_attr = uuid,
          "dropPendingName"_attr = dpns,
          "dropOpTime"_attr = dropOpTime);
    {
        Lock::CollectionLock collLk(opCtx, dpns, MODE_X);
        fassert(40464, renameCollection(opCtx, nss, dpns, stayTemp));
    }

    // Register this drop-pending namespace with DropPendingCollectionReaper to remove when the
    // committed optime reaches the drop optime.
    repl::DropPendingCollectionReaper::get(opCtx)->addDropPendingNamespace(opCtx, dropOpTime, dpns);

    return Status::OK();
}

void DatabaseImpl::_dropCollectionIndexes(OperationContext* opCtx,
                                          const NamespaceString& nss,
                                          Collection* collection) const {
    invariant(_name == nss.db());
    LOGV2_DEBUG(
        20316, 1, "dropCollection: {namespace} - dropAllIndexes start", "namespace"_attr = nss);
    collection->getIndexCatalog()->dropAllIndexes(opCtx, true);

    invariant(DurableCatalog::get(opCtx)->getTotalIndexCount(opCtx, collection->getCatalogId()) ==
              0);
    LOGV2_DEBUG(
        20317, 1, "dropCollection: {namespace} - dropAllIndexes done", "namespace"_attr = nss);
}

Status DatabaseImpl::_finishDropCollection(OperationContext* opCtx,
                                           const NamespaceString& nss,
                                           Collection* collection) const {
    UUID uuid = collection->uuid();
    LOGV2(20318,
          "Finishing collection drop for {namespace} ({uuid}).",
          "Finishing collection drop",
          "namespace"_attr = nss,
          "uuid"_attr = uuid);

    auto status = DurableCatalog::get(opCtx)->dropCollection(opCtx, collection->getCatalogId());
    if (!status.isOK())
        return status;

    auto removedColl = CollectionCatalog::get(opCtx).deregisterCollection(uuid);
    opCtx->recoveryUnit()->registerChange(
        CollectionCatalog::get(opCtx).makeFinishDropCollectionChange(std::move(removedColl), uuid));

    return Status::OK();
}

Status DatabaseImpl::renameCollection(OperationContext* opCtx,
                                      NamespaceString fromNss,
                                      NamespaceString toNss,
                                      bool stayTemp) const {
    audit::logRenameCollection(&cc(), fromNss.ns(), toNss.ns());

    invariant(opCtx->lockState()->isCollectionLockedForMode(fromNss, MODE_X));
    invariant(opCtx->lockState()->isCollectionLockedForMode(toNss, MODE_X));

    invariant(fromNss.db() == _name);
    invariant(toNss.db() == _name);
    if (CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, toNss)) {
        return Status(ErrorCodes::NamespaceExists,
                      str::stream() << "Cannot rename '" << fromNss << "' to '" << toNss
                                    << "' because the destination namespace already exists");
    }

    Collection* collToRename =
        CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, fromNss);
    if (!collToRename) {
        return Status(ErrorCodes::NamespaceNotFound, "collection not found to rename");
    }

    // Renaming a collection under the same database name is permitted for two-phase index builds.
    if (!IndexBuildsCoordinator::supportsTwoPhaseIndexBuild()) {
        invariant(
            !collToRename->getIndexCatalog()->haveAnyIndexesInProgress(),
            str::stream() << "cannot perform operation: an index build is currently running for "
                             "collection "
                          << fromNss);

        Collection* toColl =
            CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, toNss);
        if (toColl) {
            invariant(!toColl->getIndexCatalog()->haveAnyIndexesInProgress(),
                      str::stream()
                          << "cannot perform operation: an index build is currently running for "
                             "collection "
                          << toNss);
        }
    }

    LOGV2(20319,
          "renameCollection: renaming collection {collToRename_uuid} from {fromNss} to {toNss}",
          "renameCollection",
          "uuid"_attr = collToRename->uuid(),
          "fromName"_attr = fromNss,
          "toName"_attr = toNss);

    Top::get(opCtx->getServiceContext()).collectionDropped(fromNss);

    Status status = DurableCatalog::get(opCtx)->renameCollection(
        opCtx, collToRename->getCatalogId(), toNss, stayTemp);

    // Set the namespace of 'collToRename' from within the CollectionCatalog. This is necessary
    // because the CollectionCatalog mutex synchronizes concurrent access to the collection's
    // namespace for callers that may not hold a collection lock.
    CollectionCatalog::get(opCtx).setCollectionNamespace(opCtx, collToRename, fromNss, toNss);

    opCtx->recoveryUnit()->onCommit([collToRename](auto commitTime) {
        // Ban reading from this collection on committed reads on snapshots before now.
        if (commitTime) {
            collToRename->setMinimumVisibleSnapshot(commitTime.get());
        }
    });

    return status;
}

void DatabaseImpl::_checkCanCreateCollection(OperationContext* opCtx,
                                             const NamespaceString& nss,
                                             const CollectionOptions& options) const {
    if (CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, nss) != nullptr) {
        if (options.isView()) {
            uasserted(17399,
                      str::stream()
                          << "Cannot create collection " << nss << " - collection already exists.");
        } else {
            throw WriteConflictException();
        }
    }

    if (MONGO_unlikely(throwWCEDuringTxnCollCreate.shouldFail()) &&
        opCtx->inMultiDocumentTransaction()) {
        LOGV2(4696600,
              "Throwing WriteConflictException due to failpoint 'throwWCEDuringTxnCollCreate'");
        throw WriteConflictException();
    }


    uassert(17320,
            str::stream() << "cannot do createCollection on namespace with a $ in it: " << nss,
            nss.ns().find('$') == std::string::npos);
    uassert(14037,
            "can't create user databases on a --configsvr instance",
            serverGlobalParams.clusterRole != ClusterRole::ConfigServer || nss.isOnInternalDb());

    uassert(17316,
            str::stream() << "cannot create a collection with an empty name on db: " << nss.db(),
            !nss.coll().empty());

    uassert(28838, "cannot create a non-capped oplog collection", options.capped || !nss.isOplog());
    uassert(ErrorCodes::DatabaseDropPending,
            str::stream() << "Cannot create collection " << nss
                          << " - database is in the process of being dropped.",
            !_dropPending.load());

    uassert(17381,
            str::stream() << "Fully qualified namespace is too long. Namespace: " << nss.ns()
                          << " Max: " << NamespaceString::MaxNsCollectionLen,
            !nss.isNormalCollection() || nss.size() <= NamespaceString::MaxNsCollectionLen);
    const auto& fcv = serverGlobalParams.featureCompatibility;
    if (!fcv.isVersionInitialized() ||
        fcv.getVersion() < ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
        uassert(ErrorCodes::IncompatibleServerVersion,
                str::stream() << "Fully qualified namespace is too long for FCV 4.2. Upgrade to "
                                 "FCV 4.4 to create this namespace. Namespace: "
                              << nss.ns()
                              << " FCV 4.2 Limit: " << NamespaceString::MaxNSCollectionLenFCV42,
                nss.size() <= NamespaceString::MaxNSCollectionLenFCV42);
    }
}

Status DatabaseImpl::createView(OperationContext* opCtx,
                                const NamespaceString& viewName,
                                const CollectionOptions& options) const {
    dassert(opCtx->lockState()->isDbLockedForMode(name(), MODE_IX));
    dassert(opCtx->lockState()->isCollectionLockedForMode(viewName, MODE_IX));
    dassert(opCtx->lockState()->isCollectionLockedForMode(NamespaceString(_viewsName), MODE_X));

    invariant(options.isView());

    NamespaceString viewOnNss(viewName.db(), options.viewOn);
    _checkCanCreateCollection(opCtx, viewName, options);
    audit::logCreateCollection(&cc(), viewName.toString());

    if (viewName.isOplog())
        return Status(ErrorCodes::InvalidNamespace,
                      str::stream() << "invalid namespace name for a view: " + viewName.toString());

    return ViewCatalog::get(this)->createView(
        opCtx, viewName, viewOnNss, BSONArray(options.pipeline), options.collation);
}

Collection* DatabaseImpl::createCollection(OperationContext* opCtx,
                                           const NamespaceString& nss,
                                           const CollectionOptions& options,
                                           bool createIdIndex,
                                           const BSONObj& idIndex) const {
    invariant(!options.isView());

    invariant(opCtx->lockState()->isCollectionLockedForMode(nss, MODE_IX));

    uassert(CannotImplicitlyCreateCollectionInfo(nss),
            "request doesn't allow collection to be created implicitly",
            serverGlobalParams.clusterRole != ClusterRole::ShardServer ||
                OperationShardingState::get(opCtx).allowImplicitCollectionCreation() ||
                options.temp);

    auto coordinator = repl::ReplicationCoordinator::get(opCtx);
    bool canAcceptWrites =
        (coordinator->getReplicationMode() != repl::ReplicationCoordinator::modeReplSet) ||
        coordinator->canAcceptWritesForDatabase(opCtx, nss.db()) || nss.isSystemDotProfile();


    CollectionOptions optionsWithUUID = options;
    bool generatedUUID = false;
    if (!optionsWithUUID.uuid) {
        if (!canAcceptWrites) {
            LOGV2_ERROR_OPTIONS(20329,
                                {logv2::UserAssertAfterLog(ErrorCodes::InvalidOptions)},
                                "Attempted to create a new collection without a UUID",
                                "namespace"_attr = nss);
        } else {
            optionsWithUUID.uuid.emplace(CollectionUUID::gen());
            generatedUUID = true;
        }
    }

    // Because writing the oplog entry depends on having the full spec for the _id index, which is
    // not available until the collection is actually created, we can't write the oplog entry until
    // after we have created the collection.  In order to make the storage timestamp for the
    // collection create always correct even when other operations are present in the same storage
    // transaction, we reserve an opTime before the collection creation, then pass it to the
    // opObserver.  Reserving the optime automatically sets the storage timestamp.
    // In order to ensure isolation of multi-document transactions, createCollection should only
    // reserve oplog slots here if it is run outside of a multi-document transaction. Multi-
    // document transactions reserve the appropriate oplog slots at commit time.
    OplogSlot createOplogSlot;
    if (canAcceptWrites && supportsDocLocking() && !coordinator->isOplogDisabledFor(opCtx, nss) &&
        !opCtx->inMultiDocumentTransaction()) {
        createOplogSlot = repl::getNextOpTime(opCtx);
    }

    if (MONGO_unlikely(hangAndFailAfterCreateCollectionReservesOpTime.shouldFail())) {
        hangAndFailAfterCreateCollectionReservesOpTime.pauseWhileSet(opCtx);
        uasserted(51267, "hangAndFailAfterCreateCollectionReservesOpTime fail point enabled");
    }

    _checkCanCreateCollection(opCtx, nss, optionsWithUUID);
    audit::logCreateCollection(&cc(), nss.ns());

    LOGV2(20320,
          "createCollection: {namespace} with {generatedUUID_generated_provided} UUID: "
          "{optionsWithUUID_uuid_get} and options: {options}",
          "createCollection",
          "namespace"_attr = nss,
          "uuidDisposition"_attr = (generatedUUID ? "generated" : "provided"),
          "uuid"_attr = optionsWithUUID.uuid.get(),
          "options"_attr = options);

    // Create Collection object
    auto storageEngine = opCtx->getServiceContext()->getStorageEngine();
    std::pair<RecordId, std::unique_ptr<RecordStore>> catalogIdRecordStorePair =
        uassertStatusOK(storageEngine->getCatalog()->createCollection(
            opCtx, nss, optionsWithUUID, true /*allocateDefaultSpace*/));
    auto catalogId = catalogIdRecordStorePair.first;
    std::unique_ptr<Collection> ownedCollection =
        Collection::Factory::get(opCtx)->make(opCtx,
                                              nss,
                                              catalogId,
                                              optionsWithUUID.uuid.get(),
                                              std::move(catalogIdRecordStorePair.second));
    auto collection = ownedCollection.get();
    ownedCollection->init(opCtx);
    ownedCollection->setCommitted(false);
    UncommittedCollections::addToTxn(opCtx, std::move(ownedCollection));
    openCreateCollectionWindowFp.executeIf([&](const BSONObj& data) { sleepsecs(3); },
                                           [&](const BSONObj& data) {
                                               const auto collElem = data["collectionNS"];
                                               return !collElem || nss.toString() == collElem.str();
                                           });

    BSONObj fullIdIndexSpec;

    if (createIdIndex) {
        if (collection->requiresIdIndex()) {
            if (optionsWithUUID.autoIndexId == CollectionOptions::YES ||
                optionsWithUUID.autoIndexId == CollectionOptions::DEFAULT) {
                // createCollection() may be called before the in-memory fCV parameter is
                // initialized, so use the unsafe fCV getter here.
                IndexCatalog* ic = collection->getIndexCatalog();
                fullIdIndexSpec = uassertStatusOK(ic->createIndexOnEmptyCollection(
                    opCtx, !idIndex.isEmpty() ? idIndex : ic->getDefaultIdIndexSpec()));
            } else {
                // autoIndexId: false is only allowed on unreplicated collections.
                uassert(50001,
                        str::stream() << "autoIndexId:false is not allowed for collection " << nss
                                      << " because it can be replicated",
                        !nss.isReplicated());
            }
        }
    }

    hangBeforeLoggingCreateCollection.pauseWhileSet();

    opCtx->getServiceContext()->getOpObserver()->onCreateCollection(
        opCtx, collection, nss, optionsWithUUID, fullIdIndexSpec, createOplogSlot);

    // It is necessary to create the system index *after* running the onCreateCollection so that
    // the storage timestamp for the index creation is after the storage timestamp for the
    // collection creation, and the opTimes for the corresponding oplog entries are the same as the
    // storage timestamps.  This way both primary and any secondaries will see the index created
    // after the collection is created.
    if (canAcceptWrites && createIdIndex && nss.isSystem()) {
        createSystemIndexes(opCtx, collection);
    }

    return collection;
}

StatusWith<NamespaceString> DatabaseImpl::makeUniqueCollectionNamespace(
    OperationContext* opCtx, StringData collectionNameModel) const {
    invariant(opCtx->lockState()->isDbLockedForMode(name(), MODE_IX));

    // There must be at least one percent sign in the collection name model.
    auto numPercentSign = std::count(collectionNameModel.begin(), collectionNameModel.end(), '%');
    if (numPercentSign == 0) {
        return Status(ErrorCodes::FailedToParse,
                      str::stream()
                          << "Cannot generate collection name for temporary collection: "
                             "model for collection name "
                          << collectionNameModel << " must contain at least one percent sign.");
    }

    const auto charsToChooseFrom =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"_sd;
    invariant((10U + 26U * 2) == charsToChooseFrom.size());

    stdx::lock_guard<Latch> lk(uniqueCollectionNamespaceMutex);

    auto replacePercentSign = [&, this](char c) {
        if (c != '%') {
            return c;
        }
        auto i = uniqueCollectionNamespacePseudoRandom.nextInt32(charsToChooseFrom.size());
        return charsToChooseFrom[i];
    };

    auto numGenerationAttempts = numPercentSign * charsToChooseFrom.size() * 100U;
    for (decltype(numGenerationAttempts) i = 0; i < numGenerationAttempts; ++i) {
        auto collectionName = collectionNameModel.toString();
        std::transform(collectionName.begin(),
                       collectionName.end(),
                       collectionName.begin(),
                       replacePercentSign);

        NamespaceString nss(_name, collectionName);
        if (!CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, nss)) {
            return nss;
        }
    }

    return Status(
        ErrorCodes::NamespaceExists,
        str::stream() << "Cannot generate collection name for temporary collection with model "
                      << collectionNameModel << " after " << numGenerationAttempts
                      << " attempts due to namespace conflicts with existing collections.");
}

void DatabaseImpl::checkForIdIndexesAndDropPendingCollections(OperationContext* opCtx) const {
    if (name() == "local") {
        // Collections in the local database are not replicated, so we do not need an _id index on
        // any collection. For the same reason, it is not possible for the local database to contain
        // any drop-pending collections (drops are effective immediately).
        return;
    }

    for (const auto& nss :
         CollectionCatalog::get(opCtx).getAllCollectionNamesFromDb(opCtx, _name)) {
        if (nss.isDropPendingNamespace()) {
            auto dropOpTime = fassert(40459, nss.getDropPendingNamespaceOpTime());
            LOGV2(20321,
                  "Found drop-pending namespace {namespace} with drop optime {dropOpTime}",
                  "Found drop-pending namespace",
                  "namespace"_attr = nss,
                  "dropOpTime"_attr = dropOpTime);
            repl::DropPendingCollectionReaper::get(opCtx)->addDropPendingNamespace(
                opCtx, dropOpTime, nss);
        }

        if (nss.isSystem())
            continue;

        Collection* coll = CollectionCatalog::get(opCtx).lookupCollectionByNamespace(opCtx, nss);
        if (!coll)
            continue;

        if (coll->getIndexCatalog()->findIdIndex(opCtx))
            continue;

        LOGV2_OPTIONS(
            20322,
            {logv2::LogTag::kStartupWarnings},
            "Collection lacks a unique index on _id. This index is "
            "needed for replication to function properly. To fix this, you need to create a unique "
            "index on _id. See http://dochub.mongodb.org/core/build-replica-set-indexes",
            "namespace"_attr = nss);
    }
}

Status DatabaseImpl::userCreateNS(OperationContext* opCtx,
                                  const NamespaceString& nss,
                                  CollectionOptions collectionOptions,
                                  bool createDefaultIndexes,
                                  const BSONObj& idIndex) const {
    LOGV2_DEBUG(20324,
                1,
                "create collection {namespace} {collectionOptions}",
                "namespace"_attr = nss,
                "collectionOptions"_attr = collectionOptions.toBSON());
    if (!NamespaceString::validCollectionComponent(nss.ns()))
        return Status(ErrorCodes::InvalidNamespace, str::stream() << "invalid ns: " << nss);

    // Validate the collation, if there is one.
    std::unique_ptr<CollatorInterface> collator;
    if (!collectionOptions.collation.isEmpty()) {
        auto collatorWithStatus = CollatorFactoryInterface::get(opCtx->getServiceContext())
                                      ->makeFromBSON(collectionOptions.collation);

        if (!collatorWithStatus.isOK()) {
            return collatorWithStatus.getStatus();
        }

        collator = std::move(collatorWithStatus.getValue());

        // If the collator factory returned a non-null collator, set the collation option to the
        // result of serializing the collator's spec back into BSON. We do this in order to fill in
        // all options that the user omitted.
        //
        // If the collator factory returned a null collator (representing the "simple" collation),
        // we simply unset the "collation" from the collection options. This ensures that
        // collections created on versions which do not support the collation feature have the same
        // format for representing the simple collation as collections created on this version.
        collectionOptions.collation = collator ? collator->getSpec().toBSON() : BSONObj();
    }

    if (!collectionOptions.validator.isEmpty()) {
        boost::intrusive_ptr<ExpressionContext> expCtx(
            new ExpressionContext(opCtx, std::move(collator), nss));

        // Save this to a variable to avoid reading the atomic variable multiple times.
        const auto currentFCV = serverGlobalParams.featureCompatibility.getVersion();

        // If the feature compatibility version is not 4.4, and we are validating features as
        // master, ban the use of new agg features introduced in 4.4 to prevent them from being
        // persisted in the catalog.
        if (serverGlobalParams.validateFeaturesAsMaster.load() &&
            currentFCV != ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            expCtx->maxFeatureCompatibilityVersion = currentFCV;
        }

        // The match expression parser needs to know that we're parsing an expression for a
        // validator to apply some additional checks.
        expCtx->isParsingCollectionValidator = true;

        // If the validation action is "warn" or the level is "moderate", then disallow any
        // encryption keywords. This is to prevent any plaintext data from showing up in the logs.
        auto allowedFeatures = MatchExpressionParser::kDefaultSpecialFeatures;
        if (collectionOptions.validationAction == "warn" ||
            collectionOptions.validationLevel == "moderate")
            allowedFeatures &= ~MatchExpressionParser::AllowedFeatures::kEncryptKeywords;

        auto statusWithMatcher = MatchExpressionParser::parse(collectionOptions.validator,
                                                              std::move(expCtx),
                                                              ExtensionsCallbackNoop(),
                                                              allowedFeatures);

        // We check the status of the parse to see if there are any banned features, but we don't
        // actually need the result for now.
        if (!statusWithMatcher.isOK()) {
            return statusWithMatcher.getStatus();
        }
    }

    Status status = validateStorageOptions(
        opCtx->getServiceContext(),
        collectionOptions.storageEngine,
        [](const auto& x, const auto& y) { return x->validateCollectionStorageOptions(y); });

    if (!status.isOK())
        return status;

    if (auto indexOptions = collectionOptions.indexOptionDefaults["storageEngine"]) {
        status = validateStorageOptions(
            opCtx->getServiceContext(), indexOptions.Obj(), [](const auto& x, const auto& y) {
                return x->validateIndexStorageOptions(y);
            });

        if (!status.isOK()) {
            return status;
        }
    }

    if (collectionOptions.isView()) {
        uassertStatusOK(createView(opCtx, nss, collectionOptions));
    } else {
        invariant(createCollection(opCtx, nss, collectionOptions, createDefaultIndexes, idIndex),
                  str::stream() << "Collection creation failed after validating options: " << nss
                                << ". Options: " << collectionOptions.toBSON());
    }

    return Status::OK();
}

}  // namespace mongo
