/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/wiredtiger/oplog_stone_parameters_gen.h --output build/opt/mongo/db/storage/wiredtiger/oplog_stone_parameters_gen.cpp src/mongo/db/storage/wiredtiger/oplog_stone_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/storage/wiredtiger/oplog_stone_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

long long gMaxOplogStonesAfterStartup{100};
long long gMaxOplogStonesDuringStartup{100};
long long gMinOplogStones{10};
int gOplogStoneSizeMB{0};
AtomicWord<int> gOplogSamplingLogIntervalSeconds{10};
MONGO_SERVER_PARAMETER_REGISTER(idl_d1e6616d0623fa7d56bedf369dba264299090c32)(InitializerContext*) {
    /**
     * Maximum allowable number of oplog truncation points after startup has finished
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("maxOplogTruncationPointsAfterStartup", gMaxOplogStonesAfterStartup);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Maximum allowable number of oplog truncation points during startup
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("maxOplogTruncationPointsDuringStartup", gMaxOplogStonesDuringStartup);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Minimum allowable number of oplog truncation points
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("minOplogTruncationPoints", gMinOplogStones);
        ret->addBound<idl_server_parameter_detail::GT>(0);
        return ret;
    })();

    /**
     * Oplog truncation point size in MB used to determine the number of oplog truncation points for an oplog of a given size. The size will be rounded up to the maximum size of an internal BSON object.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("oplogTruncationPointSizeMB", gOplogStoneSizeMB);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    /**
     * The approximate interval between log messages indicating oplog sampling progress during start up. Once interval seconds have elapsed since the last log message, a progress message will be logged after the current sample is completed. A value of zero will disable this logging.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("oplogSamplingLogIntervalSeconds", gOplogSamplingLogIntervalSeconds);
        ret->addBound<idl_server_parameter_detail::GTE>(0);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
