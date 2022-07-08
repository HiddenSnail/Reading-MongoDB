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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kSharding

#include "mongo/platform/basic.h"

#include "mongo/db/s/config/sharding_catalog_manager.h"

#include "mongo/base/status_with.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/util/bson_extract.h"
#include "mongo/client/connection_string.h"
#include "mongo/client/read_preference.h"
#include "mongo/db/catalog_raii.h"
#include "mongo/db/dbdirectclient.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/operation_context.h"
#include "mongo/db/repl/repl_client_info.h"
#include "mongo/db/s/sharding_logging.h"
#include "mongo/db/server_options.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/get_status_from_command_result.h"
#include "mongo/s/catalog/sharding_catalog_client.h"
#include "mongo/s/catalog/type_chunk.h"
#include "mongo/s/client/shard.h"
#include "mongo/s/client/shard_registry.h"
#include "mongo/s/grid.h"
#include "mongo/s/shard_key_pattern.h"
#include "mongo/util/fail_point.h"
#include "mongo/util/str.h"

namespace mongo {
namespace {

MONGO_FAIL_POINT_DEFINE(migrationCommitVersionError);
MONGO_FAIL_POINT_DEFINE(skipExpiringOldChunkHistory);

const WriteConcernOptions kNoWaitWriteConcern(1, WriteConcernOptions::SyncMode::UNSET, Seconds(0));

bool isUpgradingOrDowngradingFCV() {
    return (serverGlobalParams.featureCompatibility.getVersion() ==
            ServerGlobalParams::FeatureCompatibility::Version::kUpgradingTo44) ||
        (serverGlobalParams.featureCompatibility.getVersion() ==
         ServerGlobalParams::FeatureCompatibility::Version::kDowngradingTo42);
}

/**
 * Append min, max and version information from chunk to the buffer for logChange purposes.
 */
void appendShortVersion(BufBuilder* out, const ChunkType& chunk) {
    BSONObjBuilder bb(*out);
    bb.append(ChunkType::min(), chunk.getMin());
    bb.append(ChunkType::max(), chunk.getMax());
    if (chunk.isVersionSet()) {
        chunk.getVersion().appendLegacyWithField(&bb, ChunkType::lastmod());
    }
    bb.done();
}

BSONArray buildMergeChunksTransactionUpdates(const std::vector<ChunkType>& chunksToMerge,
                                             const ChunkVersion& mergeVersion,
                                             const boost::optional<Timestamp>& validAfter) {
    invariant(!isUpgradingOrDowngradingFCV());

    BSONArrayBuilder updates;

    // Build an update operation to expand the first chunk into the newly merged chunk
    {
        BSONObjBuilder op;
        op.append("op", "u");
        op.appendBool("b", false);  // no upsert
        op.append("ns", ChunkType::ConfigNS.ns());

        // expand first chunk into newly merged chunk
        ChunkType mergedChunk(chunksToMerge.front());
        mergedChunk.setMax(chunksToMerge.back().getMax());

        // fill in additional details for sending through transaction
        mergedChunk.setVersion(mergeVersion);

        invariant(validAfter);
        mergedChunk.setHistory({ChunkHistory(validAfter.get(), mergedChunk.getShard())});

        // add the new chunk information as the update object
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            op.append("o", mergedChunk.toConfigBSON());

            // query object
            op.append("o2", BSON(ChunkType::name(mergedChunk.getName())));
        } else {
            op.append("o", mergedChunk.toConfigBSONLegacyID());

            // query object
            op.append("o2", BSON(ChunkType::legacyName(mergedChunk.getLegacyName())));
        }

        updates.append(op.obj());
    }

    // Build update operations to delete the rest of the chunks to be merged. Remember not
    // to delete the first chunk we're expanding
    for (size_t i = 1; i < chunksToMerge.size(); ++i) {
        BSONObjBuilder op;
        op.append("op", "d");
        op.append("ns", ChunkType::ConfigNS.ns());

        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            op.append("o", BSON(ChunkType::name(chunksToMerge[i].getName())));
        } else {
            op.append("o", BSON(ChunkType::legacyName(chunksToMerge[i].getLegacyName())));
        }

        updates.append(op.obj());
    }

    return updates.arr();
}

BSONArray buildMergeChunksTransactionPrecond(const std::vector<ChunkType>& chunksToMerge,
                                             const ChunkVersion& collVersion) {
    BSONArrayBuilder preCond;

    for (auto chunk : chunksToMerge) {
        BSONObjBuilder b;
        b.append("ns", ChunkType::ConfigNS.ns());
        b.append("q",
                 BSON("query" << BSON(ChunkType::ns(chunk.getNS().ns())
                                      << ChunkType::min(chunk.getMin())
                                      << ChunkType::max(chunk.getMax()))
                              << "orderby" << BSON(ChunkType::lastmod() << -1)));
        b.append("res",
                 BSON(ChunkType::epoch(collVersion.epoch())
                      << ChunkType::shard(chunk.getShard().toString())));
        preCond.append(b.obj());
    }
    return preCond.arr();
}

Status checkChunkMatchesRequest(OperationContext* opCtx,
                                const NamespaceString& nss,
                                const ChunkType& requestedChunk,
                                const ShardId& shard) {
    BSONObj chunkQuery =
        BSON(ChunkType::ns() << nss.ns() << ChunkType::min() << requestedChunk.getMin()
                             << ChunkType::max() << requestedChunk.getMax() << ChunkType::shard()
                             << shard);

    // Must use local read concern because we're going to perform subsequent writes.
    auto findResponseWith =
        Grid::get(opCtx)->shardRegistry()->getConfigShard()->exhaustiveFindOnConfig(
            opCtx,
            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
            repl::ReadConcernLevel::kLocalReadConcern,
            ChunkType::ConfigNS,
            chunkQuery,
            BSONObj(),
            1);
    if (!findResponseWith.isOK()) {
        return findResponseWith.getStatus();
    }

    if (findResponseWith.getValue().docs.empty()) {
        return {ErrorCodes::Error(40165),
                str::stream()
                    << "Could not find the chunk (" << chunkQuery.toString()
                    << ") on the shard. Cannot execute the migration commit with invalid chunks."};
    }

    const auto currentChunk =
        uassertStatusOK(ChunkType::fromConfigBSON(findResponseWith.getValue().docs.front()));

    // In the FCV 4.4 protocol, additionally check that the chunk's version matches what's in the
    // request.
    if (serverGlobalParams.featureCompatibility.getVersion() ==
        ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
        uassert(ErrorCodes::ConflictingOperationInProgress,
                "Config server rejecting commitChunkMigration request that does not have a "
                "ChunkVersion because config server is in feature compatibility version 4.4",
                requestedChunk.isVersionSet() && requestedChunk.getVersion().isSet() &&
                    requestedChunk.getVersion().epoch().isSet());

        if (requestedChunk.getVersion().epoch() != currentChunk.getVersion().epoch() ||
            requestedChunk.getVersion().isOlderThan(currentChunk.getVersion())) {
            return {ErrorCodes::ConflictingOperationInProgress,
                    str::stream()
                        << "Rejecting migration request because the version of the requested chunk "
                        << requestedChunk.toConfigBSON()
                        << " is older than the version of the current chunk "
                        << currentChunk.toConfigBSON()};
        }
    }

    return Status::OK();
}

BSONObj makeCommitChunkTransactionCommand(const NamespaceString& nss,
                                          const ChunkType& migratedChunk,
                                          const boost::optional<ChunkType>& controlChunk,
                                          StringData fromShard,
                                          StringData toShard) {
    // The _id format for chunk documents changed in 4.4, so during an upgrade or downgrade it is
    // not known which format the chunks are currently in. Moving a chunk requires updating the
    // associated chunk document by its _id, so migrations are disabled until the upgrade or
    // downgrade completes.
    uassert(ErrorCodes::ConflictingOperationInProgress,
            "Chunks cannot be migrated while a feature compatibility version upgrade or downgrade "
            "is in progress",
            !isUpgradingOrDowngradingFCV());

    // Update migratedChunk's version and shard.
    BSONArrayBuilder updates;
    {
        BSONObjBuilder op;
        op.append("op", "u");
        op.appendBool("b", false);  // No upserting
        op.append("ns", ChunkType::ConfigNS.ns());

        BSONObjBuilder n(op.subobjStart("o"));
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            n.append(ChunkType::name(), migratedChunk.getName());
        } else {
            n.append(ChunkType::legacyName(), ChunkType::genLegacyID(nss, migratedChunk.getMin()));
        }
        migratedChunk.getVersion().appendLegacyWithField(&n, ChunkType::lastmod());
        n.append(ChunkType::ns(), nss.ns());
        n.append(ChunkType::min(), migratedChunk.getMin());
        n.append(ChunkType::max(), migratedChunk.getMax());
        n.append(ChunkType::shard(), toShard);
        migratedChunk.addHistoryToBSON(n);
        n.done();

        BSONObjBuilder q(op.subobjStart("o2"));
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            q.append(ChunkType::name(), migratedChunk.getName());
        } else {
            n.append(ChunkType::legacyName(), ChunkType::genLegacyID(nss, migratedChunk.getMin()));
        }
        q.done();

        updates.append(op.obj());
    }

    // If we have a controlChunk, update its chunk version.
    if (controlChunk) {
        BSONObjBuilder op;
        op.append("op", "u");
        op.appendBool("b", false);
        op.append("ns", ChunkType::ConfigNS.ns());

        BSONObjBuilder n(op.subobjStart("o"));
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            n.append(ChunkType::name(), controlChunk->getName());
        } else {
            n.append(ChunkType::legacyName(), ChunkType::genLegacyID(nss, controlChunk->getMin()));
        }
        controlChunk->getVersion().appendLegacyWithField(&n, ChunkType::lastmod());
        n.append(ChunkType::ns(), nss.ns());
        n.append(ChunkType::min(), controlChunk->getMin());
        n.append(ChunkType::max(), controlChunk->getMax());
        n.append(ChunkType::shard(), fromShard);
        n.append(ChunkType::jumbo(), controlChunk->getJumbo());
        controlChunk->addHistoryToBSON(n);
        n.done();

        BSONObjBuilder q(op.subobjStart("o2"));
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            q.append(ChunkType::name(), controlChunk->getName());
        } else {
            q.append(ChunkType::legacyName(), controlChunk->getLegacyName());
        }
        q.done();

        updates.append(op.obj());
    }

    // Do not give applyOps a write concern. If applyOps tries to wait for replication, it will fail
    // because of the GlobalWrite lock CommitChunkMigration already holds. Replication will not be
    // able to take the lock it requires.
    return BSON("applyOps" << updates.arr());
}

/**
 * Returns a chunk different from the one being migrated or 'none' if one doesn't exist.
 */
boost::optional<ChunkType> getControlChunkForMigrate(OperationContext* opCtx,
                                                     const NamespaceString& nss,
                                                     const ChunkType& migratedChunk,
                                                     const ShardId& fromShard) {
    auto const configShard = Grid::get(opCtx)->shardRegistry()->getConfigShard();

    BSONObjBuilder queryBuilder;
    queryBuilder << ChunkType::ns(nss.ns());
    queryBuilder << ChunkType::shard(fromShard.toString());
    queryBuilder << ChunkType::min(BSON("$ne" << migratedChunk.getMin()));

    auto status =
        configShard->exhaustiveFindOnConfig(opCtx,
                                            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
                                            repl::ReadConcernLevel::kLocalReadConcern,
                                            ChunkType::ConfigNS,
                                            queryBuilder.obj(),
                                            {},
                                            1);
    auto response = uassertStatusOK(status);
    if (response.docs.empty()) {
        return boost::none;
    }

    return uassertStatusOK(ChunkType::fromConfigBSON(response.docs.front()));
}

// Helper function to find collection version and shard version.
StatusWith<ChunkVersion> getMaxChunkVersionFromQueryResponse(
    const NamespaceString& nss, const StatusWith<Shard::QueryResponse>& queryResponse) {

    if (!queryResponse.isOK()) {
        return queryResponse.getStatus();
    }

    const auto& chunksVector = queryResponse.getValue().docs;
    if (chunksVector.empty()) {
        return {ErrorCodes::IllegalOperation,
                str::stream() << "Collection '" << nss.ns()
                              << "' no longer either exists, is sharded, or has chunks"};
    }

    return ChunkVersion::parseLegacyWithField(chunksVector.front(), ChunkType::lastmod());
}

}  // namespace

Status ShardingCatalogManager::commitChunkSplit(OperationContext* opCtx,
                                                const NamespaceString& nss,
                                                const OID& requestEpoch,
                                                const ChunkRange& range,
                                                const std::vector<BSONObj>& splitPoints,
                                                const std::string& shardName) {
    // Take _kChunkOpLock in exclusive mode to prevent concurrent chunk splits, merges, and
    // migrations
    // TODO(SERVER-25359): Replace with a collection-specific lock map to allow splits/merges/
    // move chunks on different collections to proceed in parallel
    Lock::ExclusiveLock lk(opCtx->lockState(), _kChunkOpLock);

    // The _id format for chunk documents changed in 4.4, so during an upgrade or downgrade it is
    // not known which format the chunks are currently in. Splitting a chunk requires knowing the
    // _id of the chunk being split, so to avoid confusing failures, splitting is disabled.
    uassert(ErrorCodes::ConflictingOperationInProgress,
            "Chunks cannot be split while a feature compatibility version upgrade or downgrade is "
            "in progress",
            !isUpgradingOrDowngradingFCV());

    // Get the max chunk version for this namespace.
    auto swCollVersion = getMaxChunkVersionFromQueryResponse(
        nss,
        Grid::get(opCtx)->shardRegistry()->getConfigShard()->exhaustiveFindOnConfig(
            opCtx,
            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
            repl::ReadConcernLevel::kLocalReadConcern,
            ChunkType::ConfigNS,
            BSON("ns" << nss.ns()),          // Query all chunks for this namespace.
            BSON(ChunkType::lastmod << -1),  // Sort by version.
            1));                             // Limit 1.

    if (!swCollVersion.isOK()) {
        return swCollVersion.getStatus().withContext(
            str::stream() << "splitChunk cannot split chunk " << range.toString() << ".");
    }

    auto collVersion = swCollVersion.getValue();

    // Return an error if collection epoch does not match epoch of request.
    if (collVersion.epoch() != requestEpoch) {
        return {ErrorCodes::StaleEpoch,
                str::stream() << "splitChunk cannot split chunk " << range.toString()
                              << ". Epoch of collection '" << nss.ns() << "' has changed."
                              << " Current epoch: " << collVersion.epoch()
                              << ", cmd epoch: " << requestEpoch};
    }

    // Get the shard version (max chunk version) for the shard requesting the split.
    auto swShardVersion = getMaxChunkVersionFromQueryResponse(
        nss,
        Grid::get(opCtx)->shardRegistry()->getConfigShard()->exhaustiveFindOnConfig(
            opCtx,
            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
            repl::ReadConcernLevel::kLocalReadConcern,
            ChunkType::ConfigNS,
            BSON("ns" << nss.ns() << "shard"
                      << shardName),         // Query all chunks for this namespace and shard.
            BSON(ChunkType::lastmod << -1),  // Sort by version.
            1));                             // Limit 1.

    if (!swShardVersion.isOK()) {
        return swShardVersion.getStatus().withContext(
            str::stream() << "splitChunk cannot split chunk " << range.toString() << ".");
    }

    auto shardVersion = swShardVersion.getValue();

    // Find the chunk history.
    const auto origChunk = _findChunkOnConfig(opCtx, nss, range.getMin());
    if (!origChunk.isOK()) {
        return origChunk.getStatus();
    }

    std::vector<ChunkType> newChunks;

    ChunkVersion currentMaxVersion = collVersion;
    // Increment the major version only if the shard that owns the chunk being split has version ==
    // collection version. See SERVER-41480 for details.
    if (shardVersion == collVersion) {
        currentMaxVersion.incMajor();
    }

    auto startKey = range.getMin();
    auto newChunkBounds(splitPoints);
    newChunkBounds.push_back(range.getMax());

    auto shouldTakeOriginalChunkID = true;
    OID chunkID;
    std::string legacyChunkID;

    BSONArrayBuilder updates;

    for (const auto& endKey : newChunkBounds) {
        // Verify the split points are all within the chunk
        if (endKey.woCompare(range.getMax()) != 0 && !range.containsKey(endKey)) {
            return {ErrorCodes::InvalidOptions,
                    str::stream() << "Split key " << endKey << " not contained within chunk "
                                  << range.toString()};
        }

        // Verify the split points came in increasing order
        if (endKey.woCompare(startKey) < 0) {
            return {
                ErrorCodes::InvalidOptions,
                str::stream() << "Split keys must be specified in strictly increasing order. Key "
                              << endKey << " was specified after " << startKey << "."};
        }

        // Verify that splitPoints are not repeated
        if (endKey.woCompare(startKey) == 0) {
            return {ErrorCodes::InvalidOptions,
                    str::stream() << "Split on lower bound of chunk "
                                  << ChunkRange(startKey, endKey).toString() << "is not allowed"};
        }

        // verify that splits don't use disallowed BSON object format
        Status shardKeyStorageStatus =
            ShardKeyPattern::checkShardKeyIsValidForMetadataStorage(endKey);
        if (!shardKeyStorageStatus.isOK()) {
            return shardKeyStorageStatus;
        }

        // splits only update the 'minor' portion of version
        currentMaxVersion.incMinor();

        // First chunk takes ID of the original chunk and all other chunks get new IDs. This occurs
        // because we perform an update operation below (with upsert true). Keeping the original ID
        // ensures we overwrite the old chunk (before the split) without having to perform a delete.
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            chunkID = shouldTakeOriginalChunkID ? origChunk.getValue().getName() : OID::gen();
        } else {
            legacyChunkID = shouldTakeOriginalChunkID ? origChunk.getValue().getLegacyName()
                                                      : ChunkType::genLegacyID(nss, startKey);
        }
        shouldTakeOriginalChunkID = false;

        // build an update operation against the chunks collection of the config database
        // with upsert true
        BSONObjBuilder op;
        op.append("op", "u");
        op.appendBool("b", true);
        op.append("ns", ChunkType::ConfigNS.ns());

        // add the modified (new) chunk information as the update object
        BSONObjBuilder n(op.subobjStart("o"));
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            n.append(ChunkType::name(), chunkID);
        } else {
            n.append(ChunkType::legacyName(), legacyChunkID);
        }
        currentMaxVersion.appendLegacyWithField(&n, ChunkType::lastmod());
        n.append(ChunkType::ns(), nss.ns());
        n.append(ChunkType::min(), startKey);
        n.append(ChunkType::max(), endKey);
        n.append(ChunkType::shard(), shardName);

        origChunk.getValue().addHistoryToBSON(n);

        n.done();

        // add the chunk's _id as the query part of the update statement
        BSONObjBuilder q(op.subobjStart("o2"));
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            q.append(ChunkType::name(), chunkID);
        } else {
            q.append(ChunkType::legacyName(), legacyChunkID);
        }
        q.done();

        updates.append(op.obj());

        // remember this chunk info for logging later
        ChunkType chunk;
        chunk.setMin(startKey);
        chunk.setMax(endKey);
        chunk.setVersion(currentMaxVersion);

        newChunks.push_back(std::move(chunk));

        startKey = endKey;
    }

    BSONArrayBuilder preCond;
    {
        BSONObjBuilder b;
        b.append("ns", ChunkType::ConfigNS.ns());
        b.append("q",
                 BSON("query" << BSON(ChunkType::ns(nss.ns()) << ChunkType::min() << range.getMin()
                                                              << ChunkType::max() << range.getMax())
                              << "orderby" << BSON(ChunkType::lastmod() << -1)));
        {
            BSONObjBuilder bb(b.subobjStart("res"));
            bb.append(ChunkType::epoch(), requestEpoch);
            bb.append(ChunkType::shard(), shardName);
        }
        preCond.append(b.obj());
    }

    // apply the batch of updates to local metadata.
    const Status applyOpsStatus = Grid::get(opCtx)->catalogClient()->applyChunkOpsDeprecated(
        opCtx,
        updates.arr(),
        preCond.arr(),
        nss,
        currentMaxVersion,
        WriteConcernOptions(),
        repl::ReadConcernLevel::kLocalReadConcern);
    if (!applyOpsStatus.isOK()) {
        return applyOpsStatus;
    }

    // log changes
    BSONObjBuilder logDetail;
    {
        BSONObjBuilder b(logDetail.subobjStart("before"));
        b.append(ChunkType::min(), range.getMin());
        b.append(ChunkType::max(), range.getMax());
        collVersion.appendLegacyWithField(&b, ChunkType::lastmod());
    }

    if (newChunks.size() == 2) {
        appendShortVersion(&logDetail.subobjStart("left"), newChunks[0]);
        appendShortVersion(&logDetail.subobjStart("right"), newChunks[1]);

        ShardingLogging::get(opCtx)->logChange(
            opCtx, "split", nss.ns(), logDetail.obj(), WriteConcernOptions());
    } else {
        BSONObj beforeDetailObj = logDetail.obj();
        BSONObj firstDetailObj = beforeDetailObj.getOwned();
        const int newChunksSize = newChunks.size();

        for (int i = 0; i < newChunksSize; i++) {
            BSONObjBuilder chunkDetail;
            chunkDetail.appendElements(beforeDetailObj);
            chunkDetail.append("number", i + 1);
            chunkDetail.append("of", newChunksSize);
            appendShortVersion(&chunkDetail.subobjStart("chunk"), newChunks[i]);

            ShardingLogging::get(opCtx)->logChange(
                opCtx, "multi-split", nss.ns(), chunkDetail.obj(), WriteConcernOptions());
        }
    }

    return Status::OK();
}

Status ShardingCatalogManager::commitChunkMerge(OperationContext* opCtx,
                                                const NamespaceString& nss,
                                                const OID& requestEpoch,
                                                const std::vector<BSONObj>& chunkBoundaries,
                                                const std::string& shardName,
                                                const boost::optional<Timestamp>& validAfter) {
    // This method must never be called with empty chunks to merge
    invariant(!chunkBoundaries.empty());

    // Take _kChunkOpLock in exclusive mode to prevent concurrent chunk splits, merges, and
    // migrations
    // TODO(SERVER-25359): Replace with a collection-specific lock map to allow splits/merges/
    // move chunks on different collections to proceed in parallel
    Lock::ExclusiveLock lk(opCtx->lockState(), _kChunkOpLock);

    // The _id format for chunk documents changed in 4.4, so during an upgrade or downgrade it is
    // not known which format the chunks are currently in. Merging a chunk requires knowing the
    // _id of the chunks being merged, so to avoid confusing failures, merging is disabled.
    uassert(ErrorCodes::ConflictingOperationInProgress,
            "Chunks cannot be merged while a feature compatibility version upgrade or downgrade is "
            "in progress",
            !isUpgradingOrDowngradingFCV());

    if (!validAfter) {
        return {ErrorCodes::IllegalOperation, "chunk operation requires validAfter timestamp"};
    }

    // Get the max chunk version for this namespace.
    auto swCollVersion = getMaxChunkVersionFromQueryResponse(
        nss,
        Grid::get(opCtx)->shardRegistry()->getConfigShard()->exhaustiveFindOnConfig(
            opCtx,
            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
            repl::ReadConcernLevel::kLocalReadConcern,
            ChunkType::ConfigNS,
            BSON("ns" << nss.ns()),          // Query all chunks for this namespace.
            BSON(ChunkType::lastmod << -1),  // Sort by version.
            1));                             // Limit 1.

    if (!swCollVersion.isOK()) {
        return swCollVersion.getStatus().withContext(str::stream()
                                                     << "mergeChunk cannot merge chunks.");
    }

    auto collVersion = swCollVersion.getValue();

    // Return an error if epoch of chunk does not match epoch of request
    if (collVersion.epoch() != requestEpoch) {
        return {ErrorCodes::StaleEpoch,
                str::stream() << "Epoch of chunk does not match epoch of request. Chunk epoch: "
                              << collVersion.epoch() << ", request epoch: " << requestEpoch};
    }

    // Check if the chunk(s) have already been merged. If so, return success.
    auto minChunkOnDisk = uassertStatusOK(_findChunkOnConfig(opCtx, nss, chunkBoundaries.front()));
    if (minChunkOnDisk.getMax().woCompare(chunkBoundaries.back()) == 0) {
        return Status::OK();
    }

    // Build chunks to be merged
    std::vector<ChunkType> chunksToMerge;

    ChunkType itChunk;
    itChunk.setMax(chunkBoundaries.front());
    itChunk.setNS(nss);
    itChunk.setShard(shardName);

    // Do not use the first chunk boundary as a max bound while building chunks
    for (size_t i = 1; i < chunkBoundaries.size(); ++i) {
        itChunk.setMin(itChunk.getMax());

        // Read the original chunk from disk to lookup that chunk's '_id' field.
        auto itOrigChunk = _findChunkOnConfig(opCtx, nss, itChunk.getMin());
        if (!itOrigChunk.isOK()) {
            return itOrigChunk.getStatus();
        }

        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            itChunk.setName(itOrigChunk.getValue().getName());
        }

        // Ensure the chunk boundaries are strictly increasing
        if (chunkBoundaries[i].woCompare(itChunk.getMin()) <= 0) {
            return {
                ErrorCodes::InvalidOptions,
                str::stream()
                    << "Chunk boundaries must be specified in strictly increasing order. Boundary "
                    << chunkBoundaries[i] << " was specified after " << itChunk.getMin() << "."};
        }

        itChunk.setMax(chunkBoundaries[i]);
        chunksToMerge.push_back(itChunk);
    }

    ChunkVersion mergeVersion = collVersion;
    mergeVersion.incMinor();

    auto updates = buildMergeChunksTransactionUpdates(chunksToMerge, mergeVersion, validAfter);
    auto preCond = buildMergeChunksTransactionPrecond(chunksToMerge, collVersion);

    // apply the batch of updates to local metadata
    const Status applyOpsStatus = Grid::get(opCtx)->catalogClient()->applyChunkOpsDeprecated(
        opCtx,
        updates,
        preCond,
        nss,
        mergeVersion,
        WriteConcernOptions(),
        repl::ReadConcernLevel::kLocalReadConcern);
    if (!applyOpsStatus.isOK()) {
        return applyOpsStatus;
    }

    // log changes
    BSONObjBuilder logDetail;
    {
        BSONArrayBuilder b(logDetail.subarrayStart("merged"));
        for (auto chunkToMerge : chunksToMerge) {
            b.append(chunkToMerge.toConfigBSON());
        }
    }
    collVersion.appendLegacyWithField(&logDetail, "prevShardVersion");
    mergeVersion.appendLegacyWithField(&logDetail, "mergedVersion");

    ShardingLogging::get(opCtx)->logChange(
        opCtx, "merge", nss.ns(), logDetail.obj(), WriteConcernOptions());

    return Status::OK();
}

StatusWith<BSONObj> ShardingCatalogManager::commitChunkMigration(
    OperationContext* opCtx,
    const NamespaceString& nss,
    const ChunkType& migratedChunk,
    const OID& collectionEpoch,
    const ShardId& fromShard,
    const ShardId& toShard,
    const boost::optional<Timestamp>& validAfter) {

    auto const configShard = Grid::get(opCtx)->shardRegistry()->getConfigShard();

    // Must hold the shard lock until the entire commit finishes to serialize with removeShard.
    Lock::SharedLock shardLock(opCtx->lockState(), _kShardMembershipLock);
    auto shardResult = uassertStatusOK(
        configShard->exhaustiveFindOnConfig(opCtx,
                                            ReadPreferenceSetting(ReadPreference::PrimaryOnly),
                                            repl::ReadConcernLevel::kLocalReadConcern,
                                            ShardType::ConfigNS,
                                            BSON(ShardType::name(toShard.toString())),
                                            {},
                                            boost::none));

    uassert(ErrorCodes::ShardNotFound,
            str::stream() << "shard " << toShard << " does not exist",
            !shardResult.docs.empty());

    auto shard = uassertStatusOK(ShardType::fromBSON(shardResult.docs.front()));
    uassert(ErrorCodes::ShardNotFound,
            str::stream() << toShard << " is draining",
            !shard.getDraining());

    // Take _kChunkOpLock in exclusive mode to prevent concurrent chunk splits, merges, and
    // migrations.
    //
    // ConfigSvrCommitChunkMigration commands must be run serially because the new ChunkVersions
    // for migrated chunks are generated within the command and must be committed to the database
    // before another chunk commit generates new ChunkVersions in the same manner.
    //
    // TODO(SERVER-25359): Replace with a collection-specific lock map to allow splits/merges/
    // move chunks on different collections to proceed in parallel.
    // (Note: This is not needed while we have a global lock, taken here only for consistency.)
    Lock::ExclusiveLock lk(opCtx->lockState(), _kChunkOpLock);

    // The _id format for chunk documents changed in 4.4, so during an upgrade or downgrade it is
    // not known which format the chunks are currently in. Moving a chunk requires knowing the
    // _id of the chunks being moved, so to avoid confusing failures, migrations are disabled.
    uassert(ErrorCodes::ConflictingOperationInProgress,
            "Chunks cannot be migrated while a feature compatibility version upgrade or downgrade "
            "is in progress",
            !isUpgradingOrDowngradingFCV());

    if (!validAfter) {
        return {ErrorCodes::IllegalOperation, "chunk operation requires validAfter timestamp"};
    }

    // Must use local read concern because we will perform subsequent writes.
    auto findResponse =
        configShard->exhaustiveFindOnConfig(opCtx,
                                            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
                                            repl::ReadConcernLevel::kLocalReadConcern,
                                            ChunkType::ConfigNS,
                                            BSON("ns" << nss.ns()),
                                            BSON(ChunkType::lastmod << -1),
                                            1);
    if (!findResponse.isOK()) {
        return findResponse.getStatus();
    }

    if (MONGO_unlikely(migrationCommitVersionError.shouldFail())) {
        uassert(ErrorCodes::StaleEpoch,
                "failpoint 'migrationCommitVersionError' generated error",
                false);
    }

    const auto chunksVector = std::move(findResponse.getValue().docs);
    if (chunksVector.empty()) {
        return {ErrorCodes::IncompatibleShardingMetadata,
                str::stream() << "Tried to find max chunk version for collection '" << nss.ns()
                              << ", but found no chunks"};
    }

    const auto swChunk = ChunkType::fromConfigBSON(chunksVector.front());
    if (!swChunk.isOK()) {
        return swChunk.getStatus();
    }

    const auto currentCollectionVersion = swChunk.getValue().getVersion();

    // It is possible for a migration to end up running partly without the protection of the
    // distributed lock if the config primary stepped down since the start of the migration and
    // failed to recover the migration. Check that the collection has not been dropped and recreated
    // or had its shard key refined since the migration began, unbeknown to the shard when the
    // command was sent.
    if (currentCollectionVersion.epoch() != collectionEpoch) {
        return {ErrorCodes::StaleEpoch,
                str::stream() << "The epoch of collection '" << nss.ns()
                              << "' has changed since the migration began. The config server's "
                                 "collection version epoch is now '"
                              << currentCollectionVersion.epoch().toString()
                              << "', but the shard's is " << collectionEpoch.toString()
                              << "'. Aborting migration commit for chunk ("
                              << migratedChunk.getRange().toString() << ")."};
    }

    // Check that migratedChunk has not been modified since the migration started.
    auto migratedOnShard = checkChunkMatchesRequest(opCtx, nss, migratedChunk, fromShard);
    if (!migratedOnShard.isOK()) {
        return migratedOnShard;
    }

    auto controlChunk = getControlChunkForMigrate(opCtx, nss, migratedChunk, fromShard);

    // Find the chunk history.
    const auto origChunk = _findChunkOnConfig(opCtx, nss, migratedChunk.getMin());
    if (!origChunk.isOK()) {
        return origChunk.getStatus();
    }

    // Generate the new versions of migratedChunk and controlChunk. Migrating chunk's minor version
    // will be 0.
    ChunkType newMigratedChunk = migratedChunk;
    if (serverGlobalParams.featureCompatibility.getVersion() >=
        ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
        newMigratedChunk.setName(origChunk.getValue().getName());
    }
    newMigratedChunk.setShard(toShard);
    newMigratedChunk.setVersion(ChunkVersion(
        currentCollectionVersion.majorVersion() + 1, 0, currentCollectionVersion.epoch()));

    // Copy the complete history.
    auto newHistory = origChunk.getValue().getHistory();
    const int kHistorySecs = 10;

    invariant(validAfter);

    // Update the history of the migrated chunk.
    // Drop the history that is too old (10 seconds of history for now).
    // TODO SERVER-33831 to update the old history removal policy.
    if (!MONGO_unlikely(skipExpiringOldChunkHistory.shouldFail())) {
        while (!newHistory.empty() &&
               newHistory.back().getValidAfter().getSecs() + kHistorySecs <
                   validAfter.get().getSecs()) {
            newHistory.pop_back();
        }
    }

    if (!newHistory.empty() && newHistory.front().getValidAfter() >= validAfter.get()) {
        return {ErrorCodes::IncompatibleShardingMetadata,
                str::stream() << "The chunk history for chunk with namespace " << nss.ns()
                              << " and min key " << migratedChunk.getMin()
                              << " is corrupted. The last validAfter "
                              << newHistory.back().getValidAfter().toString()
                              << " is greater or equal to the new validAfter "
                              << validAfter.get().toString()};
    }
    newHistory.emplace(newHistory.begin(), ChunkHistory(validAfter.get(), toShard));
    newMigratedChunk.setHistory(std::move(newHistory));

    // Control chunk's minor version will be 1 (if control chunk is present).
    boost::optional<ChunkType> newControlChunk = boost::none;
    if (controlChunk) {
        // Find the chunk history.
        const auto origControlChunk = _findChunkOnConfig(opCtx, nss, controlChunk->getMin());
        if (!origControlChunk.isOK()) {
            return origControlChunk.getStatus();
        }

        newControlChunk = origControlChunk.getValue();
        if (serverGlobalParams.featureCompatibility.getVersion() >=
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
            newControlChunk->setName(origControlChunk.getValue().getName());
        }
        newControlChunk->setVersion(ChunkVersion(
            currentCollectionVersion.majorVersion() + 1, 1, currentCollectionVersion.epoch()));
    }

    auto command = makeCommitChunkTransactionCommand(
        nss, newMigratedChunk, newControlChunk, fromShard.toString(), toShard.toString());

    StatusWith<Shard::CommandResponse> applyOpsCommandResponse =
        configShard->runCommandWithFixedRetryAttempts(
            opCtx,
            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
            nss.db().toString(),
            command,
            Shard::RetryPolicy::kIdempotent);

    if (!applyOpsCommandResponse.isOK()) {
        return applyOpsCommandResponse.getStatus();
    }

    if (!applyOpsCommandResponse.getValue().commandStatus.isOK()) {
        return applyOpsCommandResponse.getValue().commandStatus;
    }

    BSONObjBuilder result;
    newMigratedChunk.getVersion().appendWithField(&result, "migratedChunkVersion");
    if (controlChunk) {
        newControlChunk->getVersion().appendWithField(&result, "controlChunkVersion");
    }

    return result.obj();
}

StatusWith<ChunkType> ShardingCatalogManager::_findChunkOnConfig(OperationContext* opCtx,
                                                                 const NamespaceString& nss,
                                                                 const BSONObj& key) {
    auto const configShard = Grid::get(opCtx)->shardRegistry()->getConfigShard();

    auto findResponse =
        configShard->exhaustiveFindOnConfig(opCtx,
                                            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
                                            repl::ReadConcernLevel::kLocalReadConcern,
                                            ChunkType::ConfigNS,
                                            BSON(ChunkType::ns(nss.ns()) << ChunkType::min(key)),
                                            BSONObj(),
                                            1);

    if (!findResponse.isOK()) {
        return findResponse.getStatus();
    }

    const auto origChunks = std::move(findResponse.getValue().docs);
    if (origChunks.size() != 1) {
        return {ErrorCodes::IncompatibleShardingMetadata,
                str::stream() << "Tried to find the chunk for namespace " << nss.ns()
                              << " and min key " << key.toString() << ", but found no chunks"};
    }

    return ChunkType::fromConfigBSON(origChunks.front());
}

void ShardingCatalogManager::clearJumboFlag(OperationContext* opCtx,
                                            const NamespaceString& nss,
                                            const OID& collectionEpoch,
                                            const ChunkRange& chunk) {
    auto const configShard = Grid::get(opCtx)->shardRegistry()->getConfigShard();

    // Take _kChunkOpLock in exclusive mode to prevent concurrent chunk splits, merges, and
    // migrations.
    //
    // ConfigSvrClearJumboFlag commands must be run serially because the new ChunkVersions
    // for the modified chunks are generated within the command and must be committed to the
    // database before another chunk operation generates new ChunkVersions in the same manner.
    //
    // TODO(SERVER-25359): Replace with a collection-specific lock map to allow splits/merges/
    // move chunks on different collections to proceed in parallel.
    // (Note: This is not needed while we have a global lock, taken here only for consistency.)
    Lock::ExclusiveLock lk(opCtx->lockState(), _kChunkOpLock);

    auto targetChunkResult = uassertStatusOK(configShard->exhaustiveFindOnConfig(
        opCtx,
        ReadPreferenceSetting{ReadPreference::PrimaryOnly},
        repl::ReadConcernLevel::kLocalReadConcern,
        ChunkType::ConfigNS,
        BSON(ChunkType::ns(nss.ns())
             << ChunkType::min(chunk.getMin()) << ChunkType::max(chunk.getMax())),
        {},
        1));

    const auto targetChunkVector = std::move(targetChunkResult.docs);
    uassert(51262,
            str::stream() << "Unable to locate chunk " << chunk.toString()
                          << " from ns: " << nss.ns(),
            !targetChunkVector.empty());

    const auto targetChunk = uassertStatusOK(ChunkType::fromConfigBSON(targetChunkVector.front()));

    if (!targetChunk.getJumbo()) {
        return;
    }

    // Must use local read concern because we will perform subsequent writes.
    auto findResponse = uassertStatusOK(
        configShard->exhaustiveFindOnConfig(opCtx,
                                            ReadPreferenceSetting{ReadPreference::PrimaryOnly},
                                            repl::ReadConcernLevel::kLocalReadConcern,
                                            ChunkType::ConfigNS,
                                            BSON(ChunkType::ns(nss.ns())),
                                            BSON(ChunkType::lastmod << -1),
                                            1));

    const auto chunksVector = std::move(findResponse.docs);
    uassert(ErrorCodes::IncompatibleShardingMetadata,
            str::stream() << "Tried to find max chunk version for collection '" << nss.ns()
                          << ", but found no chunks",
            !chunksVector.empty());

    const auto highestVersionChunk =
        uassertStatusOK(ChunkType::fromConfigBSON(chunksVector.front()));
    const auto currentCollectionVersion = highestVersionChunk.getVersion();

    // It is possible for a migration to end up running partly without the protection of the
    // distributed lock if the config primary stepped down since the start of the migration and
    // failed to recover the migration. Check that the collection has not been dropped and recreated
    // or had its shard key refined since the migration began, unbeknown to the shard when the
    // command was sent.
    uassert(ErrorCodes::StaleEpoch,
            str::stream() << "The epoch of collection '" << nss.ns()
                          << "' has changed since the migration began. The config server's "
                             "collection version epoch is now '"
                          << currentCollectionVersion.epoch().toString() << "', but the shard's is "
                          << collectionEpoch.toString() << "'. Aborting clear jumbo on chunk ("
                          << chunk.toString() << ").",
            currentCollectionVersion.epoch() == collectionEpoch);

    ChunkVersion newVersion(
        currentCollectionVersion.majorVersion() + 1, 0, currentCollectionVersion.epoch());

    BSONObj chunkQuery(BSON(ChunkType::ns(nss.ns())
                            << ChunkType::epoch(collectionEpoch) << ChunkType::min(chunk.getMin())
                            << ChunkType::max(chunk.getMax())));

    BSONObjBuilder updateBuilder;
    updateBuilder.append("$unset", BSON(ChunkType::jumbo() << ""));

    BSONObjBuilder updateVersionClause(updateBuilder.subobjStart("$set"));
    newVersion.appendLegacyWithField(&updateVersionClause, ChunkType::lastmod());
    updateVersionClause.doneFast();

    auto chunkUpdate = updateBuilder.obj();

    auto didUpdate = uassertStatusOK(
        Grid::get(opCtx)->catalogClient()->updateConfigDocument(opCtx,
                                                                ChunkType::ConfigNS,
                                                                chunkQuery,
                                                                chunkUpdate,
                                                                false /* upsert */,
                                                                kNoWaitWriteConcern));

    uassert(51263,
            str::stream() << "failed to clear jumbo flag due to " << chunkQuery
                          << " not matching any existing chunks",
            didUpdate);
}

void ShardingCatalogManager::ensureChunkVersionIsGreaterThan(OperationContext* opCtx,
                                                             const BSONObj& minKey,
                                                             const BSONObj& maxKey,
                                                             const ChunkVersion& version) {
    auto earlyReturnBeforeDoingWriteGuard = makeGuard([&] {
        // Ensure waiting for writeConcern of the data read.
        repl::ReplClientInfo::forClient(opCtx->getClient()).setLastOpToSystemLastOpTime(opCtx);
    });

    // Take _kChunkOpLock in exclusive mode to prevent concurrent chunk operations.
    // TODO (SERVER-25359): Replace with a collection-specific lock map to allow splits/merges/
    // move chunks on different collections to proceed in parallel.
    Lock::ExclusiveLock lk(opCtx->lockState(), _kChunkOpLock);

    const auto requestedChunkQuery =
        BSON(ChunkType::min(minKey) << ChunkType::max(maxKey) << ChunkType::epoch(version.epoch()));
    const auto configShard = Grid::get(opCtx)->shardRegistry()->getConfigShard();

    // Get the chunk matching the requested chunk.
    const auto matchingChunksVector =
        uassertStatusOK(
            configShard->exhaustiveFindOnConfig(opCtx,
                                                ReadPreferenceSetting{ReadPreference::PrimaryOnly},
                                                repl::ReadConcernLevel::kLocalReadConcern,
                                                ChunkType::ConfigNS,
                                                requestedChunkQuery,
                                                BSONObj() /* sort */,
                                                1 /* limit */))
            .docs;
    if (matchingChunksVector.empty()) {
        // This can happen in a number of cases, such as that the collection has been dropped, its
        // shard key has been refined, the chunk has been split, or the chunk has been merged.
        LOGV2(23884,
              "ensureChunkVersionIsGreaterThan did not find any chunks with minKey {minKey}, "
              "maxKey {maxKey}, and epoch {epoch}. Returning success.",
              "ensureChunkVersionIsGreaterThan did not find any matching chunks; returning success",
              "minKey"_attr = minKey,
              "maxKey"_attr = maxKey,
              "epoch"_attr = version.epoch());
        return;
    }

    const auto currentChunk =
        uassertStatusOK(ChunkType::fromConfigBSON(matchingChunksVector.front()));

    if (version.isOlderThan(currentChunk.getVersion())) {
        LOGV2(23885,
              "ensureChunkVersionIsGreaterThan found that the chunk with minKey {minKey}, maxKey "
              "{maxKey}, and epoch {epoch} already has a higher version than {version}. Current "
              "chunk is {currentChunk}. Returning success.",
              "ensureChunkVersionIsGreaterThan found that the chunk already has a higher version; "
              "returning success",
              "minKey"_attr = minKey,
              "maxKey"_attr = maxKey,
              "epoch"_attr = version.epoch(),
              "version"_attr = version,
              "currentChunk"_attr = currentChunk.toConfigBSON());
        return;
    }

    // Get the chunk with the current collectionVersion for this epoch.
    const auto highestChunksVector =
        uassertStatusOK(
            configShard->exhaustiveFindOnConfig(opCtx,
                                                ReadPreferenceSetting{ReadPreference::PrimaryOnly},
                                                repl::ReadConcernLevel::kLocalReadConcern,
                                                ChunkType::ConfigNS,
                                                BSON(ChunkType::epoch(version.epoch())) /* query */,
                                                BSON(ChunkType::lastmod << -1) /* sort */,
                                                1 /* limit */))
            .docs;
    if (highestChunksVector.empty()) {
        LOGV2(23886,
              "ensureChunkVersionIsGreaterThan did not find any chunks with epoch {epoch} when "
              "attempting to find the collectionVersion. The collection must have been dropped "
              "concurrently or had its shard key refined. Returning success.",
              "ensureChunkVersionIsGreaterThan did not find any chunks with a matching epoch when "
              "attempting to find the collectionVersion. The collection must have been dropped "
              "concurrently or had its shard key refined. Returning success.",
              "epoch"_attr = version.epoch());
        return;
    }
    const auto highestChunk =
        uassertStatusOK(ChunkType::fromConfigBSON(highestChunksVector.front()));

    // Generate a new version for the chunk by incrementing the collectionVersion's major version.
    auto newChunk = currentChunk;
    newChunk.setVersion(
        ChunkVersion(highestChunk.getVersion().majorVersion() + 1, 0, version.epoch()));

    // Update the chunk, if it still exists, to have the bumped version.
    earlyReturnBeforeDoingWriteGuard.dismiss();
    auto didUpdate = uassertStatusOK(
        Grid::get(opCtx)->catalogClient()->updateConfigDocument(opCtx,
                                                                ChunkType::ConfigNS,
                                                                requestedChunkQuery,
                                                                newChunk.toConfigBSON(),
                                                                false /* upsert */,
                                                                kNoWaitWriteConcern));
    if (didUpdate) {
        LOGV2(
            23887,
            "ensureChunkVersionIsGreaterThan bumped the version of the chunk with minKey {minKey}, "
            "maxKey {maxKey}, and epoch {epoch}. Chunk is now {newChunk}",
            "ensureChunkVersionIsGreaterThan bumped the the chunk version",
            "minKey"_attr = minKey,
            "maxKey"_attr = maxKey,
            "epoch"_attr = version.epoch(),
            "newChunk"_attr = newChunk.toConfigBSON());
    } else {
        LOGV2(
            23888,
            "ensureChunkVersionIsGreaterThan did not find a chunk matching minKey {minKey}, "
            "maxKey {maxKey}, and epoch {epoch} when trying to bump its version. The collection "
            "must have been dropped concurrently or had its shard key refined. Returning success.",
            "ensureChunkVersionIsGreaterThan did not find a matching chunk when trying to bump its "
            "version. The collection must have been dropped concurrently or had its shard key "
            "refined. Returning success.",
            "minKey"_attr = minKey,
            "maxKey"_attr = maxKey,
            "epoch"_attr = version.epoch());
    }
}

}  // namespace mongo
