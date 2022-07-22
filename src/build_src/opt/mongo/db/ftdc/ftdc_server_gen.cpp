/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/ftdc/ftdc_server_gen.h --output build/opt/mongo/db/ftdc/ftdc_server_gen.cpp src/mongo/db/ftdc/ftdc_server.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/ftdc/ftdc_server_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

/**
 * diagnosticDataCollectionDirectoryPath: Specify the directory for the diagnostic data directory.
 */
Status DiagnosticDataCollectionDirectoryPathServerParameter::set(const BSONElement& newValueElement) {
    auto swValue = coerceToString(newValueElement, false);
    if (!swValue.isOK()) {
        return swValue.getStatus();
    }
    return setFromString(swValue.getValue());
}

AtomicWord<bool> gDiagnosticDataCollectionEnableLatencyHistograms;
MONGO_SERVER_PARAMETER_REGISTER(idl_d65e863dd5fd6712a543d4563d77f8c55602d6b9)(InitializerContext*) {
    /**
     * Determines whether to enable the collecting and logging of data for diagnostic purposes
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionEnabled", ftdcStartupParams.enabled);
        ret->setOnUpdate(onUpdateFTDCEnabled);
        return ret;
    })();

    /**
     * Specifies the interval, in milliseconds, at which to collect diagnostic data.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionPeriodMillis", ftdcStartupParams.periodMillis);
        ret->setOnUpdate(onUpdateFTDCPeriod);
        ret->addBound<idl_server_parameter_detail::GTE>(100);
        return ret;
    })();

    /**
     * Specifies the maximum size, in megabytes, of the diagnostic.data directory
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_2 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionDirectorySizeMB", ftdcStartupParams.maxDirectorySizeMB);
        ret->setOnUpdate(onUpdateFTDCDirectorySize);
        ret->addBound<idl_server_parameter_detail::GTE>(10);
        return ret;
    })();

    /**
     * Specifies the maximum size, in megabytes, of each diagnostic file"
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_3 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionFileSizeMB", ftdcStartupParams.maxFileSizeMB);
        ret->setOnUpdate(onUpdateFTDCFileSize);
        ret->addBound<idl_server_parameter_detail::GTE>(1);
        return ret;
    })();

    /**
     * Internal, Specifies the number of samples per diagnostic archive chunk
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_4 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionSamplesPerChunk", ftdcStartupParams.maxSamplesPerArchiveMetricChunk);
        ret->setOnUpdate(onUpdateFTDCSamplesPerChunk);
        ret->addBound<idl_server_parameter_detail::GTE>(2);
        return ret;
    })();

    /**
     * Internal, Specifies the number of samples per diagnostic interim update
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_5 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionSamplesPerInterimUpdate", ftdcStartupParams.maxSamplesPerInterimMetricChunk);
        ret->setOnUpdate(onUpdateFTDCPerInterimUpdate);
        ret->addBound<idl_server_parameter_detail::GTE>(2);
        return ret;
    })();

    /**
     * Specify the directory for the diagnostic data directory.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_6 = ([]() -> ServerParameter* {
        return new DiagnosticDataCollectionDirectoryPathServerParameter("diagnosticDataCollectionDirectoryPath", ServerParameterType::kStartupAndRuntime);
    })();

    /**
     * Enable the capture of opLatencies: { histograms: true } } in FTDC.
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_7 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("diagnosticDataCollectionEnableLatencyHistograms", gDiagnosticDataCollectionEnableLatencyHistograms);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
