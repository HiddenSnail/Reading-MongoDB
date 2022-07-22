/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/max_validate_mb_per_sec_gen.h --output build/opt/mongo/db/catalog/max_validate_mb_per_sec_gen.cpp src/mongo/db/catalog/max_validate_mb_per_sec.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/catalog/max_validate_mb_per_sec_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> gMaxValidateMBperSec{0};
MONGO_SERVER_PARAMETER_REGISTER(idl_dace9dc014df744013e94bb85aa8354358cc5390)(InitializerContext*) {
    /**
     * Max MB per second that a single validate command running with { background: true } will process in order to limit I/O and CPU usage. Defaults to 0, which turns off throttling.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("maxValidateMBperSec", gMaxValidateMBperSec);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
