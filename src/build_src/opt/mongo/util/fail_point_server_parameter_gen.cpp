/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/util/fail_point_server_parameter_gen.h --output build/opt/mongo/util/fail_point_server_parameter_gen.cpp src/mongo/util/fail_point_server_parameter.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/util/fail_point_server_parameter_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * failpoint.*: [internal] Failpoint ServerParameter prototype
 */
Status FailPointServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

MONGO_SERVER_PARAMETER_REGISTER(idl_0b45f128feb728345b7b4b08f1b43ce0cec1d46c)(InitializerContext*) {
    /**
     * [internal] Failpoint ServerParameter prototype
     */
    if (false) {
        MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
            return new FailPointServerParameter("failpoint.*", ServerParameterType::kStartupOnly);
        })();
    }

    return Status::OK();
}
}  // namespace mongo
