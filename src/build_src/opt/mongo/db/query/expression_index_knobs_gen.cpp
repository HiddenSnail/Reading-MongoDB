/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/expression_index_knobs_gen.h --output build/opt/mongo/db/query/expression_index_knobs_gen.cpp src/mongo/db/query/expression_index_knobs.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/query/expression_index_knobs_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

AtomicWord<int> gInternalGeoPredicateQuery2DMaxCoveringCells{16};
AtomicWord<int> gInternalGeoNearQuery2DMaxCoveringCells{16};
AtomicWord<int> gInternalQueryS2GeoFinestLevel{23};
AtomicWord<int> gInternalQueryS2GeoCoarsestLevel{0};
AtomicWord<int> gInternalQueryS2GeoMaxCells{20};
MONGO_SERVER_PARAMETER_REGISTER(idl_5de512b2c698a0618a7ffc69efde27e3b4e5ae8f)(InitializerContext*) {
    /**
     * Maximum number of cells to use for 2D geo query covering for predicate queries
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalGeoPredicateQuery2DMaxCoveringCells", gInternalGeoPredicateQuery2DMaxCoveringCells);
        return ret;
    })();

    /**
     * Maximum number of cells to use for 2D geo query covering for near queries
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalGeoNearQuery2DMaxCoveringCells", gInternalGeoNearQuery2DMaxCoveringCells);
        return ret;
    })();

    /**
     * Finest level we will cover a queried region or geoNear annulus
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryS2GeoFinestLevel", gInternalQueryS2GeoFinestLevel);
        return ret;
    })();

    /**
     * Coarsest level we will cover a queried region or geoNear annulus
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryS2GeoCoarsestLevel", gInternalQueryS2GeoCoarsestLevel);
        return ret;
    })();

    /**
     * Maximum cell count that we want? (advisory, not a hard threshold)
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("internalQueryS2GeoMaxCells", gInternalQueryS2GeoMaxCells);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
