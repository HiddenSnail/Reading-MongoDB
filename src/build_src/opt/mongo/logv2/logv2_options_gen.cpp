/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/logv2/logv2_options_gen.h --output build/opt/mongo/logv2/logv2_options_gen.cpp src/mongo/logv2/logv2_options.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/logv2/logv2_options_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

MONGO_SERVER_PARAMETER_REGISTER(idl_916b839cb88a61dbb317cfca3f791d9fea2c2ad6)(InitializerContext*) {
    /**
     * Enables redaction of encrypted fields in BSON Objects, defaults to on
     */
    [[maybe_unused]] auto* scp_0 = ([]() -> ServerParameter* {
        auto sp = makeServerParameter<RedactEncryptedFields>("redactEncryptedFields", ServerParameterType::kStartupAndRuntime);
        return sp;
    })();

}
}  // namespace mongo
