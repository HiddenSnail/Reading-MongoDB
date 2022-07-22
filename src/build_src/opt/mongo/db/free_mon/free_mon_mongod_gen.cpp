/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/free_mon/free_mon_mongod_gen.h --output build/opt/mongo/db/free_mon/free_mon_mongod_gen.cpp src/mongo/db/free_mon/free_mon_mongod.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/free_mon/free_mon_mongod_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

std::string FreeMonEndpointURL{"https://cloud.mongodb.com/freemonitoring/mongo"};
MONGO_SERVER_PARAMETER_REGISTER(idl_10de0d47878949f4ef424237b9c0a37247c4113e)(InitializerContext*) {
    /**
     * Suppress logging of warnings when non-SSL connections are accepted in preferSSL mode
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("cloudFreeMonitoringEndpointURL", FreeMonEndpointURL);
        ret->addValidator(onValidateFreeMonEndpointURL);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
