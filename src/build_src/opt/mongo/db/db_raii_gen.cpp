/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/db_raii_gen.h --output build/opt/mongo/db/db_raii_gen.cpp src/mongo/db/db_raii.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/db_raii_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gAllowSecondaryReadsDuringBatchApplication{true};
MONGO_SERVER_PARAMETER_REGISTER(idl_6f9217d659be8dc4c35221ccd2fec12841744571)(InitializerContext*) {
    /**
     * If true, do not take PBWM lock in AutoGetCollectionForRead on secondaries during batch application.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("allowSecondaryReadsDuringBatchApplication", gAllowSecondaryReadsDuringBatchApplication);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
