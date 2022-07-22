/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/sharding_runtime_d_params_gen.h --output build/opt/mongo/db/s/sharding_runtime_d_params_gen.cpp src/mongo/db/s/sharding_runtime_d_params.idl
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
#include "mongo/rpc/op_msg.h"

namespace mongo {

constexpr auto kRangeDeleterBatchSizeDefault = 0;

extern AtomicWord<int> rangeDeleterBatchSize;
constexpr auto kRangeDeleterBatchDelayMSDefault = 20;

extern AtomicWord<int> rangeDeleterBatchDelayMS;
constexpr auto kMigrateCloneInsertionBatchSizeDefault = 0;

extern AtomicWord<int> migrateCloneInsertionBatchSize;
constexpr auto kMigrateCloneInsertionBatchDelayMSDefault = 0;

extern AtomicWord<int> migrateCloneInsertionBatchDelayMS;
constexpr auto kMigrationLockAcquisitionMaxWaitMSDefault = 500;

extern AtomicWord<int> migrationLockAcquisitionMaxWaitMS;
constexpr auto kOrphanCleanupDelaySecsDefault = 900;

extern AtomicWord<int> orphanCleanupDelaySecs;
constexpr auto kDisableResumableRangeDeleterDefault = false;

extern AtomicWord<bool> disableResumableRangeDeleter;
constexpr auto kEnableShardedIndexConsistencyCheckDefault = true;

extern AtomicWord<bool> enableShardedIndexConsistencyCheck;
constexpr auto kShardedIndexConsistencyCheckIntervalMSDefault = 600000;

extern int shardedIndexConsistencyCheckIntervalMS;
constexpr auto kMinNumChunksForSessionsCollectionDefault = 1024;

extern AtomicWord<int32_t> minNumChunksForSessionsCollection;
}  // namespace mongo
