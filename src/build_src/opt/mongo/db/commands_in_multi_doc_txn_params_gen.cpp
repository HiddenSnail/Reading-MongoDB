/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands_in_multi_doc_txn_params_gen.h --output build/opt/mongo/db/commands_in_multi_doc_txn_params_gen.cpp src/mongo/db/commands_in_multi_doc_txn_params.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands_in_multi_doc_txn_params_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<bool> gShouldMultiDocTxnCreateCollectionAndIndexes{true};
MONGO_SERVER_PARAMETER_REGISTER(idl_70e7ef79bce8f179890362378794b93b103dbdf8)(InitializerContext*) {
    /**
     * Enable ability to create new collections and indexes inside multi-document transactions. Default is true.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("shouldMultiDocTxnCreateCollectionAndIndexes", gShouldMultiDocTxnCreateCollectionAndIndexes);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
