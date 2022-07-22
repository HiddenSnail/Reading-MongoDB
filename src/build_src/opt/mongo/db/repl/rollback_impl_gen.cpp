/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/rollback_impl_gen.h --output build/opt/mongo/db/repl/rollback_impl_gen.cpp src/mongo/db/repl/rollback_impl.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/rollback_impl_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gCreateRollbackDataFiles{true};
AtomicWord<int> gRollbackTimeLimitSecs{60 * 60 * 24};
MONGO_SERVER_PARAMETER_REGISTER(idl_5003e181d12e034efe5a4ba5ca1bb1abebb96cda)(InitializerContext*) {
    /**
     * Control whether or not the server will write out data files containing deleted documents during rollback. This server parameter affects both rollback via refetch and rollback via recovery to stable timestamp.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("createRollbackDataFiles", gCreateRollbackDataFiles);
        return ret;
    })();

    /**
     * This amount, measured in seconds, represents the maximum allowed rollback period. It is calculated by taking the difference of the wall clock times of the oplog entries at the top of the local oplog and the first oplog entry after the common point.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("rollbackTimeLimitSecs", gRollbackTimeLimitSecs);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
