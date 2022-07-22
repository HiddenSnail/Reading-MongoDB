/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/repl_server_parameters_gen.h --output build/opt/mongo/db/repl/repl_server_parameters_gen.cpp src/mongo/db/repl/repl_server_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/repl_server_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {
namespace repl {

std::string initialSyncOplogBuffer{"collection"};
int initialSyncOplogBufferPeekCacheSize{10000};
AtomicWord<int> numInitialSyncConnectAttempts{10};
AtomicWord<int> numInitialSyncOplogFindAttempts{3};
int initialSyncOplogFetcherBatchSize{(16 * 1024 * 1024) / 12 * 10};
AtomicWord<int> oplogInitialFindMaxSeconds{60};
AtomicWord<int> oplogRetriedFindMaxSeconds{2};
bool oplogFetcherUsesExhaust{true};
int bgSyncOplogFetcherBatchSize{(16 * 1024 * 1024) / 12 * 10};
AtomicWord<int> rollbackRemoteOplogQueryBatchSize{2000};
AtomicWord<bool> forceRollbackViaRefetch{false};
AtomicWord<bool> writePeriodicNoops{true};
bool collectionClonerUsesExhaust{true};
int collectionBulkLoaderBatchSizeInBytes{256 * 1024};
int collectionClonerBatchSize{0};
AtomicWord<int> oplogFetcherSteadyStateMaxFetcherRestarts{1};
AtomicWord<int> oplogFetcherInitialSyncMaxFetcherRestarts{10};
int replWriterThreadCount{16};
AtomicWord<int> replBatchLimitOperations{5 * 1000};
AtomicWord<int> replBatchLimitBytes{100 * 1024 * 1024};
AtomicWord<int> initialSyncTransientErrorRetryPeriodSeconds{24 * 60 * 60};
AtomicWord<int> oplogNetworkTimeoutBufferSeconds{5};
bool enableSafeReplicaSetReconfig{true};
std::string initialSyncSourceReadPreference{""};
bool assertStableTimestampEqualsAppliedThroughOnRecovery{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_8ddc250dbb1c12b1430c218ab90a19f202a22596)(InitializerContext*) {
    /**
     * Set this to specify whether to use a collection to buffer the oplog on the destination server during initial sync to prevent rolling over the oplog.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("initialSyncOplogBuffer", initialSyncOplogBuffer);
        return ret;
    })();

    /**
     * Set this to specify size of read ahead buffer in the OplogBufferCollection.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("initialSyncOplogBufferPeekCacheSize", initialSyncOplogBufferPeekCacheSize);
        return ret;
    })();

    /**
     * The number of attempts to connect to a sync source
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("numInitialSyncConnectAttempts", numInitialSyncConnectAttempts);
        return ret;
    })();

    /**
     * The number of attempts to call find on the remote oplog
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("numInitialSyncOplogFindAttempts", numInitialSyncOplogFindAttempts);
        return ret;
    })();

    /**
     * The batchSize to use for the find/getMore queries called by the OplogFetcher
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("initialSyncOplogFetcherBatchSize", initialSyncOplogFetcherBatchSize);
        return ret;
    })();

    /**
     * Number of seconds for the `maxTimeMS` on the initial `find` command. For the initial 'find' request, we provide a generous timeout, to account for the potentially slow process of a sync source finding the lastApplied optime provided in a node's query in its oplog.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("oplogInitialFindMaxSeconds", oplogInitialFindMaxSeconds);
        return ret;
    })();

    /**
     * Number of seconds for the `maxTimeMS` on any retried `find` commands
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("oplogRetriedFindMaxSeconds", oplogRetriedFindMaxSeconds);
        return ret;
    })();

    /**
     * Whether to use the "exhaust cursor" feature when fetching oplog entries from the sync source.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("oplogFetcherUsesExhaust", oplogFetcherUsesExhaust);
        return ret;
    })();

    /**
     * The batchSize to use for the find/getMore queries called by the OplogFetcher
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_8 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("bgSyncOplogFetcherBatchSize", bgSyncOplogFetcherBatchSize);
        return ret;
    })();

    /**
     * The batchSize to use for the find/getMore queries called by the rollback common point resolver. A batchSize of 0 means that the 'find' and 'getMore' commands will be given no batchSize. We set the default to 2000 to prevent the sync source from having to read too much data at once, and reduce the chance of a socket timeout.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_9 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("rollbackRemoteOplogQueryBatchSize", rollbackRemoteOplogQueryBatchSize);
        return ret;
    })();

    /**
     * If 'forceRollbackViaRefetch' is true, always perform rollbacks via the refetch algorithm, even if the storage engine supports rollback via recover to timestamp.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_10 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("forceRollbackViaRefetch", forceRollbackViaRefetch);
        return ret;
    })();

    /**
     * Sets whether to write periodic noops or not
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_11 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("writePeriodicNoops", writePeriodicNoops);
        return ret;
    })();

    /**
     * Whether to use the "exhaust cursor" feature when retrieving collection data.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_12 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("collectionClonerUsesExhaust", collectionClonerUsesExhaust);
        return ret;
    })();

    /**
     * Limit for the number of bytes of data inserted per storage transaction (WriteUnitOfWork) by collectionBulkLoader during initial sync collection cloning
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_13 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("collectionBulkLoaderBatchSizeInBytes", collectionBulkLoaderBatchSizeInBytes);
        return ret;
    })();

    /**
     * The batch size (number of documents) to use for the queries in the CollectionCloner. Default of '0' means the limit is the number of documents that will fit in a single BSON object.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_14 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("collectionClonerBatchSize", collectionClonerBatchSize);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Set this to specify the maximum number of times the oplog fetcher will consecutively restart the oplog tailing query on non-cancellation errors during steady state replication.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_15 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("oplogFetcherSteadyStateMaxFetcherRestarts", oplogFetcherSteadyStateMaxFetcherRestarts);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Set this to specify the maximum number of times the oplog fetcher will consecutively restart the oplog tailing query on non-cancellation errors during initial sync. By default we provide a generous amount of restarts to avoid potentially restarting an entire initial sync from scratch.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_16 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("oplogFetcherInitialSyncMaxFetcherRestarts", oplogFetcherInitialSyncMaxFetcherRestarts);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * The number of threads in the thread pool used to apply the oplog
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_17 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("replWriterThreadCount", replWriterThreadCount);
        ret->addBound<idl_server_parameter_detail::LTE>(256);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * The maximum number of operations to apply in a single batch
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_18 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("replBatchLimitOperations", replBatchLimitOperations);
        ret->addBound<idl_server_parameter_detail::LTE>(([]{ constexpr auto value = 1000 * 1000; return value; })());
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * The maximum oplog application batch size in bytes
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_19 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("replBatchLimitBytes", replBatchLimitBytes);
        ret->addBound<idl_server_parameter_detail::LTE>(([]{ constexpr auto value = 100 * 1024 * 1024; return value; })());
        ret->addBound<idl_server_parameter_detail::GTE>(([]{ constexpr auto value = 16 * 1024 * 1024; return value; })());
        return ret;
    })();

    /**
     * The amount of time to continue retrying transient errors during initial sync before declaring the attempt failed.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_20 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("initialSyncTransientErrorRetryPeriodSeconds", initialSyncTransientErrorRetryPeriodSeconds);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Number of seconds to add to the `find` and `getMore` timeouts for oplog fetching to calculate the network timeout for the requests.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_21 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("oplogNetworkTimeoutBufferSeconds", oplogNetworkTimeoutBufferSeconds);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Enables safe replica set reconfig.
     */
    auto* scp_22 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("enableSafeReplicaSetReconfig", enableSafeReplicaSetReconfig);
        return ret;
    })();
    scp_22->setTestOnly();

    /**
     * Set this to specify how the sync source for initial sync is determined. Valid options are: nearest, primary, primaryPreferred, secondary, and secondaryPreferred.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_23 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("initialSyncSourceReadPreference", initialSyncSourceReadPreference);
        ret->addValidator(validateReadPreferenceMode);
        return ret;
    })();

    /**
     * Enables invariant to check stable timestamp equals appliedThrough on recovery.
     */
    auto* scp_24 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("assertStableTimestampEqualsAppliedThroughOnRecovery", assertStableTimestampEqualsAppliedThroughOnRecovery);
        return ret;
    })();
    scp_24->setTestOnly();

    return Status::OK();
}
}  // namespace repl
}  // namespace mongo
