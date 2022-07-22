/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/sharding_runtime_d_params_gen.h --output build/opt/mongo/db/s/sharding_runtime_d_params_gen.cpp src/mongo/db/s/sharding_runtime_d_params.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/s/sharding_runtime_d_params_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> rangeDeleterBatchSize{0};
AtomicWord<int> rangeDeleterBatchDelayMS{20};
AtomicWord<int> migrateCloneInsertionBatchSize{0};
AtomicWord<int> migrateCloneInsertionBatchDelayMS{0};
AtomicWord<int> migrationLockAcquisitionMaxWaitMS{500};
AtomicWord<int> orphanCleanupDelaySecs{900};
AtomicWord<bool> disableResumableRangeDeleter{false};
AtomicWord<bool> enableShardedIndexConsistencyCheck{true};
int shardedIndexConsistencyCheckIntervalMS{600000};
AtomicWord<int32_t> minNumChunksForSessionsCollection{1024};
MONGO_SERVER_PARAMETER_REGISTER(idl_0f4ff67704659a8aecb5278d23668d0561d2d29b)(InitializerContext*) {
    /**
     * The maximum number of documents in each batch to delete during the cleanup stage of chunk migration (or the cleanupOrphaned command). The default value of 0 indicates that the system chooses an appropriate value, generally 128 documents.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("rangeDeleterBatchSize", rangeDeleterBatchSize);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * The amount of time in milliseconds to wait before the next batch of deletion during the cleanup stage of chunk migration (or the cleanupOrphaned command).
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("rangeDeleterBatchDelayMS", rangeDeleterBatchDelayMS);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * The maximum number of documents to insert in a single batch during the cloning step of the migration process. The default value of 0 indicates no maximum number of documents per batch. However, in practice, this results in batches that contain up to 16 MB of documents. The value 1 corresponds to MongoDB 3.4.16 (and earlier) behavior.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("migrateCloneInsertionBatchSize", migrateCloneInsertionBatchSize);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Time in milliseconds to wait between batches of insertions during cloning step of the migration process. This wait is in addition to the secondaryThrottle. The default value of 0 indicates no additional wait.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("migrateCloneInsertionBatchDelayMS", migrateCloneInsertionBatchDelayMS);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * How long to wait to acquire collection lock for migration related operations.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("migrationLockAcquisitionMaxWaitMS", migrationLockAcquisitionMaxWaitMS);
        return ret;
    })();

    /**
     * How long to wait before starting cleanup of an emigrated chunk range.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("orphanCleanupDelaySecs", orphanCleanupDelaySecs);
        return ret;
    })();

    /**
     * Disable the resumable range deleter. Ranges will not be submitted for deletion, and if an incoming migration sees orphans in the incoming range, the migration will fail. The resumable range deleter replaced the old non-durable range deleter as of FCV 4.4.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("disableResumableRangeDeleter", disableResumableRangeDeleter);
        return ret;
    })();

    /**
     * Enable the periodic sharded index consistency check on the config server's primary. The count of sharded collections with inconsistent indexes is exposed via the 'shardedIndexConsistency' section in the serverStatus output.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("enableShardedIndexConsistencyCheck", enableShardedIndexConsistencyCheck);
        return ret;
    })();

    /**
     * Time interval in milliseconds between subsequent index checks.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_8 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("shardedIndexConsistencyCheckIntervalMS", shardedIndexConsistencyCheckIntervalMS);
        return ret;
    })();

    /**
     * The minimum number of chunks for config.system.sessions collection
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_9 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("minNumChunksForSessionsCollection", minNumChunksForSessionsCollection);
        ret->addBound<idl_server_parameter_detail::LTE>(1000000);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
