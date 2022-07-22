/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/transaction_participant_gen.h --output build/opt/mongo/db/transaction_participant_gen.cpp src/mongo/db/transaction_participant.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/transaction_participant_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> gMaxTransactionLockRequestTimeoutMillis{5};
AtomicWord<decltype(TransactionParticipant::observeTransactionLifetimeLimitSeconds)::Argument> gTransactionLifetimeLimitSeconds{60};
int gMaxNumberOfTransactionOperationsInSingleOplogEntry{2147483647};
AtomicWord<long long> gTransactionSizeLimitBytes{std::numeric_limits<long long>::max()};
MONGO_SERVER_PARAMETER_REGISTER(idl_2d027c0f06ff60334b8c977697f76a27b3b6872d)(InitializerContext*) {
    /**
     * Max number of milliseconds that any transaction lock request will wait for lock acquisition. If an operation provides a greater timeout in a lock request, maxTransactionLockRequestTimeoutMillis will override it. If this is set to a negative value, it is inactive and nothing will be overridden.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxTransactionLockRequestTimeoutMillis", gMaxTransactionLockRequestTimeoutMillis);
        return ret;
    })();

    /**
     * Lifetime given to each transaction. Transactions must eventually expire to preempt storage cache pressure immobilizing the system.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("transactionLifetimeLimitSeconds", gTransactionLifetimeLimitSeconds);
        ret->setOnUpdate(std::ref(TransactionParticipant::observeTransactionLifetimeLimitSeconds));
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Maximum number of operations to pack into a single oplog entry, when multi-oplog format for transactions is in use.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("maxNumberOfTransactionOperationsInSingleOplogEntry", gMaxNumberOfTransactionOperationsInSingleOplogEntry);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Maximum total size of operations in a multi-document transaction.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("transactionSizeLimitBytes", gTransactionSizeLimitBytes);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
