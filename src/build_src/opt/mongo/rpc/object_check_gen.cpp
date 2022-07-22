/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/rpc/object_check_gen.h --output build/opt/mongo/rpc/object_check_gen.cpp src/mongo/rpc/object_check.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/rpc/object_check_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

MONGO_SERVER_PARAMETER_REGISTER(idl_48dc1e3752e5ce279f0f3578b4368a98e687172a)(InitializerContext*) {
    /**
     * Maximum nesting depth of BSON objects
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("maxBSONDepth", BSONDepth::maxAllowableDepth);
        ret->addBound<idl_server_parameter_detail::LTE>(([]{ constexpr auto value = BSONDepth::kBSONDepthParameterCeiling; return value; })());
        ret->addBound<idl_server_parameter_detail::GTE>(([]{ constexpr auto value = BSONDepth::kBSONDepthParameterFloor; return value; })());
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
