/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/read_concern_mongod_gen.h --output build/opt/mongo/db/read_concern_mongod_gen.cpp src/mongo/db/read_concern_mongod.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/read_concern_mongod_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> waitForSecondaryBeforeNoopWriteMS{10};
MONGO_SERVER_PARAMETER_REGISTER(idl_0e324fb270d0dc1abee4b210883bf4af4be39278)(InitializerContext*) {
    /**
     * The length of time (in milliseconds) that a secondary must wait if the afterClusterTime is greater than the last applied time from the oplog. After the waitForSecondaryBeforeNoopWriteMS passes, if the afterClusterTime is still greater than the last applied time, the secondary makes a no-op write to advance the last applied time.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("waitForSecondaryBeforeNoopWriteMS", waitForSecondaryBeforeNoopWriteMS);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
