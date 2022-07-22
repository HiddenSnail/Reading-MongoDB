/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/watchdog/watchdog_mongod_gen.h --output build/opt/mongo/watchdog/watchdog_mongod_gen.cpp src/mongo/watchdog/watchdog_mongod.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/watchdog/watchdog_mongod_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> gWatchdogPeriodSeconds{-1};
MONGO_SERVER_PARAMETER_REGISTER(idl_4a4040f4f1d46436f017119d996cf313ca5e73ba)(InitializerContext*) {
    /**
     * Watchdog Period (seconds)
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("watchdogPeriodSeconds", gWatchdogPeriodSeconds);
        ret->setOnUpdate(onUpdateWatchdogPeriodSeconds);
        ret->addValidator(validateWatchdogPeriodSeconds);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
