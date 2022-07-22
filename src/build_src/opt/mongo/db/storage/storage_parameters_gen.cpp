/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/storage_parameters_gen.h --output build/opt/mongo/db/storage/storage_parameters_gen.cpp src/mongo/db/storage/storage_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/storage/storage_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

bool gTakeUnstableCheckpointOnShutdown{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_00f530d5d6eec2fd73d1b0b17c85c58438bf160b)(InitializerContext*) {
    /**
     * Whether all queries must use indexes. If true, MongoDB will not execute queries that require a table scan and will return an error.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("notablescan", storageGlobalParams.noTableScan);
        return ret;
    })();

    /**
     * Interval in seconds between fsync operations where mongod flushes its working memory to disk. By default, mongod flushes memory to disk every 60 seconds.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("syncdelay", storageGlobalParams.syncdelay);
        ret->addBound<idl_server_parameter_detail::LTE>(([]{ constexpr auto value = StorageGlobalParams::kMaxSyncdelaySecs; return value; })());
        ret->addBound<idl_server_parameter_detail::GTE>(0.0);
        return ret;
    })();

    /**
     * Number of milliseconds between journal commits
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("journalCommitInterval", storageGlobalParams.journalCommitIntervalMs);
        ret->addBound<idl_server_parameter_detail::LTE>(([]{ constexpr auto value = StorageGlobalParams::kMaxJournalCommitIntervalMs; return value; })());
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Take unstable checkpoint on shutdown
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("takeUnstableCheckpointOnShutdown", gTakeUnstableCheckpointOnShutdown);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
