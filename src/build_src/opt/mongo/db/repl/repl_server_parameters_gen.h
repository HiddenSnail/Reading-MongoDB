/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/repl_server_parameters_gen.h --output build/opt/mongo/db/repl/repl_server_parameters_gen.cpp src/mongo/db/repl/repl_server_parameters.idl
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
#include "mongo/client/read_preference.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace repl {

constexpr auto kInitialSyncOplogBufferDefault = "collection";

extern std::string initialSyncOplogBuffer;
constexpr auto kInitialSyncOplogBufferPeekCacheSizeDefault = 10000;

extern int initialSyncOplogBufferPeekCacheSize;
constexpr auto kNumInitialSyncConnectAttemptsDefault = 10;

extern AtomicWord<int> numInitialSyncConnectAttempts;
constexpr auto kNumInitialSyncOplogFindAttemptsDefault = 3;

extern AtomicWord<int> numInitialSyncOplogFindAttempts;
constexpr auto kInitialSyncOplogFetcherBatchSizeDefault = (16 * 1024 * 1024) / 12 * 10;

extern int initialSyncOplogFetcherBatchSize;
constexpr auto kOplogInitialFindMaxSecondsDefault = 60;

extern AtomicWord<int> oplogInitialFindMaxSeconds;
constexpr auto kOplogRetriedFindMaxSecondsDefault = 2;

extern AtomicWord<int> oplogRetriedFindMaxSeconds;
constexpr auto kOplogFetcherUsesExhaustDefault = true;

extern bool oplogFetcherUsesExhaust;
constexpr auto kBgSyncOplogFetcherBatchSizeDefault = (16 * 1024 * 1024) / 12 * 10;

extern int bgSyncOplogFetcherBatchSize;
constexpr auto kRollbackRemoteOplogQueryBatchSizeDefault = 2000;

extern AtomicWord<int> rollbackRemoteOplogQueryBatchSize;
constexpr auto kForceRollbackViaRefetchDefault = false;

extern AtomicWord<bool> forceRollbackViaRefetch;
constexpr auto kWritePeriodicNoopsDefault = true;

extern AtomicWord<bool> writePeriodicNoops;
constexpr auto kCollectionClonerUsesExhaustDefault = true;

extern bool collectionClonerUsesExhaust;
constexpr auto kCollectionBulkLoaderBatchSizeInBytesDefault = 256 * 1024;

extern int collectionBulkLoaderBatchSizeInBytes;
constexpr auto kCollectionClonerBatchSizeDefault = 0;

extern int collectionClonerBatchSize;
constexpr auto kOplogFetcherSteadyStateMaxFetcherRestartsDefault = 1;

extern AtomicWord<int> oplogFetcherSteadyStateMaxFetcherRestarts;
constexpr auto kOplogFetcherInitialSyncMaxFetcherRestartsDefault = 10;

extern AtomicWord<int> oplogFetcherInitialSyncMaxFetcherRestarts;
constexpr auto kReplWriterThreadCountDefault = 16;

extern int replWriterThreadCount;
constexpr auto kReplBatchLimitOperationsDefault = 5 * 1000;

extern AtomicWord<int> replBatchLimitOperations;
constexpr auto kReplBatchLimitBytesDefault = 100 * 1024 * 1024;

extern AtomicWord<int> replBatchLimitBytes;
constexpr auto kInitialSyncTransientErrorRetryPeriodSecondsDefault = 24 * 60 * 60;

extern AtomicWord<int> initialSyncTransientErrorRetryPeriodSeconds;
constexpr auto kOplogNetworkTimeoutBufferSecondsDefault = 5;

extern AtomicWord<int> oplogNetworkTimeoutBufferSeconds;
constexpr auto kEnableSafeReplicaSetReconfigDefault = true;

extern bool enableSafeReplicaSetReconfig;
constexpr auto kInitialSyncSourceReadPreferenceDefault = "";

extern std::string initialSyncSourceReadPreference;
constexpr auto kAssertStableTimestampEqualsAppliedThroughOnRecoveryDefault = false;

extern bool assertStableTimestampEqualsAppliedThroughOnRecovery;
}  // namespace repl
}  // namespace mongo
