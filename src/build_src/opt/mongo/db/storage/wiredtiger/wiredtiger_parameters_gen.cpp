/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/wiredtiger/wiredtiger_parameters_gen.h --output build/opt/mongo/db/storage/wiredtiger/wiredtiger_parameters_gen.cpp src/mongo/db/storage/wiredtiger/wiredtiger_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/storage/wiredtiger/wiredtiger_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * wiredTigerConcurrentWriteTransactions: WiredTiger Concurrent Write Transactions
 */
Status OpenWriteTransactionParam::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

/**
 * wiredTigerConcurrentReadTransactions: WiredTiger Concurrent Read Transactions
 */
Status OpenReadTransactionParam::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

/**
 * wiredTigerEngineRuntimeConfig: WiredTiger Configuration
 */
Status WiredTigerEngineRuntimeConfigParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

AtomicWord<std::int32_t> gWiredTigerSessionCloseIdleTimeSecs{kDebugBuild ? 5 : 300};
AtomicWord<std::int32_t> gWiredTigerCursorCacheSize{-100};
AtomicWord<double> gWiredTigerMaxCacheOverflowSizeGBDeprecated{0};
bool gWiredTigerEvictionDebugMode{false};
std::int32_t gWiredTigerFileHandleCloseIdleTime{100000};
std::int32_t gWiredTigerFileHandleCloseMinimum{250};
std::int32_t gWiredTigerFileHandleCloseScanInterval{10};
MONGO_SERVER_PARAMETER_REGISTER(idl_37c24b9a7239f7fd2d08c68708438c4221821321)(InitializerContext*) {
    /**
     * WiredTiger Concurrent Write Transactions
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new OpenWriteTransactionParam("wiredTigerConcurrentWriteTransactions", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * WiredTiger Concurrent Read Transactions
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        return new OpenReadTransactionParam("wiredTigerConcurrentReadTransactions", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * WiredTiger Configuration
     */
    if (false) {
        MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
            return new WiredTigerEngineRuntimeConfigParameter("wiredTigerEngineRuntimeConfig", ServerParameterType::kRuntimeOnly);
        })();
    }

    /**
     * Close idle wiredtiger sessions in the session cache after this many seconds
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("wiredTigerSessionCloseIdleTimeSecs", gWiredTigerSessionCloseIdleTimeSecs);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * Wired tiger cursor cache size
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("wiredTigerCursorCacheSize", gWiredTigerCursorCacheSize);
        return ret;
    })();

    /**
     * Maximum amount of disk space to use for cache overflow; Defaults to 0 (unbounded) Deprecated.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kRuntimeOnly>("wiredTigerMaxCacheOverflowSizeGB", gWiredTigerMaxCacheOverflowSizeGBDeprecated);
        return ret;
    })();

    /**
     * If true, modify internal WiredTiger algorithms to force lookaside eviction to happen more aggressively.
     */
    if constexpr (kDebugBuild) {
        MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
            auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("wiredTigerEvictionDebugMode", gWiredTigerEvictionDebugMode);
            return ret;
        })();
    }

    /**
     * The amount of time in seconds a file handle in WiredTiger needs to be idle before attempting to close it.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("wiredTigerFileHandleCloseIdleTime", gWiredTigerFileHandleCloseIdleTime);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * The number of handles open before the WiredTiger file manager will look for handles to close.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_8 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("wiredTigerFileHandleCloseMinimum", gWiredTigerFileHandleCloseMinimum);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * The interval in seconds at which WiredTiger checks for inactive files and closes them.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_9 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("wiredTigerFileHandleCloseScanInterval", gWiredTigerFileHandleCloseScanInterval);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
