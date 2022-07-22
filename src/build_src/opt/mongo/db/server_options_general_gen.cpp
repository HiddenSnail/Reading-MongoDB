/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/server_options_general_gen.h --output build/opt/mongo/db/server_options_general_gen.cpp src/mongo/db/server_options_general.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/server_options_general_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/util/options_parser/option_section.h"
#include "mongo/util/options_parser/startup_option_init.h"
#include "mongo/util/options_parser/startup_options.h"

namespace mongo {


Status addGeneralServerOptionDefinitions(optionenvironment::OptionSection* options_ptr) {
    auto& options = *options_ptr;
    namespace moe = ::mongo::optionenvironment;

    {
        moe::OptionSection section("General options");

        section
            .addOptionChaining("help", "help,h", moe::Switch, "Show this usage information", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
        ;

        section
            .addOptionChaining("version", "version", moe::Switch, "Show version information", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
        ;

        section
            .addOptionChaining("config", "config,f", moe::String, "Configuration file specifying additional options", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
        ;

        section
            .addOptionChaining("outputConfig", "outputConfig", moe::Switch, "Display the resolved configuration and exit", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceCommandLine)
            .hidden()
        ;

        section
            .addOptionChaining("configExpand", "configExpand", moe::String, "Process expansion directives in config file (none, exec, rest)", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceCommandLine)
        ;

        section
            .addOptionChaining("configExpandTimeoutSecs", "configExpandTimeoutSecs", moe::Int, "Maximum number of seconds to wait for a single configuration expansion to resolve", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceCommandLine)
            .hidden()
            .setDefault(moe::Value(30))
        ;

        section
            .addOptionChaining("net.port", "port", moe::Int, ServerGlobalParams::getPortSettingHelpText(), {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;

        section
            .addOptionChaining("net.ipv6", "ipv6", moe::Switch, "Enable IPv6 support (disabled by default)", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;

        section
            .addOptionChaining("net.listenBacklog", "listenBacklog", moe::Int, "Set socket listen backlog size", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
            .setDefault(moe::Value(([]{ constexpr auto value = SOMAXCONN; return value; })()))
        ;

        section
            .addOptionChaining("net.maxIncomingConnections", "maxConns", moe::Int, "Max number of simultaneous connections", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
            .setDefault(moe::Value(([]{ constexpr auto value = static_cast<int>(DEFAULT_MAX_CONN); return value; })()))
        ;

        section
            .addOptionChaining("net.maxIncomingConnectionsOverride", "", moe::StringVector, "CIDR ranges that are exempt from the maxIncomingConnections limit", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceYAMLConfig)
            .hidden()
        ;

        section
            .addOptionChaining("net.reservedAdminThreads", "", moe::Int, "Number of worker threads to reserve for admin and internal connections", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceYAMLConfig)
            .hidden()
        ;

        section
            .addOptionChaining("net.transportLayer", "transportLayer", moe::String, "Sets the ingress transport layer implementation", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
            .hidden()
            .setDefault(moe::Value("asio"))
        ;

        section
            .addOptionChaining("net.serviceExecutor", "serviceExecutor", moe::String, "Sets the service executor implementation", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
            .hidden()
            .setDefault(moe::Value("synchronous"))
        ;

        section
            .addOptionChaining("processManagement.pidFilePath", "pidfilepath", moe::String, "Full path to pidfile (if not set, no pidfile is created)", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;

        section
            .addOptionChaining("processManagement.timeZoneInfo", "timeZoneInfo", moe::String, "Full path to time zone info directory, e.g. /usr/share/zoneinfo", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;

#if !defined(_WIN32)
        section
            .addOptionChaining("nounixsocket", "nounixsocket", moe::Switch, "Disable listening on unix sockets", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
        ;
#endif

#if !defined(_WIN32)
        section
            .addOptionChaining("net.unixDomainSocket.enabled", "", moe::Bool, "Disable listening on unix sockets", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceYAMLConfig)
        ;
#endif

#if !defined(_WIN32)
        section
            .addOptionChaining("net.unixDomainSocket.pathPrefix", "unixSocketPrefix", moe::String, "Alternative directory for UNIX domain sockets (defaults to /tmp)", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;
#endif

#if !defined(_WIN32)
        section
            .addOptionChaining("net.unixDomainSocket.filePermissions", "filePermissions", moe::Int, getUnixDomainSocketFilePermissionsHelpText(), {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;
#endif

#if !defined(_WIN32)
        section
            .addOptionChaining("processManagement.fork", "fork", moe::Switch, "Fork server process", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAll)
        ;
#endif

        section
            .addOptionChaining("objcheck", "objcheck", moe::Switch, "Inspect client data for validity on receipt (DEFAULT)", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
            .hidden()
            .incompatibleWith("noobjcheck")
        ;

        section
            .addOptionChaining("noobjcheck", "noobjcheck", moe::Switch, "Do NOT inspect client data for validity on receipt", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
            .hidden()
            .incompatibleWith("objcheck")
        ;

        section
            .addOptionChaining("net.wireObjectCheck", "", moe::Bool, "Inspect client data for validity on receipt (DEFAULT)", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceYAMLConfig)
            .hidden()
        ;

        section
            .addOptionChaining("enableExperimentalStorageDetailsCmd", "enableExperimentalStorageDetailsCmd", moe::Switch, "EXPERIMENTAL (UNSUPPORTED). Enable command computing aggregate statistics on storage.", {}, {}, moe::OptionSection::OptionParserUsageType::IDLAutoGeneratedCode)
            .setSources(moe::SourceAllLegacy)
            .hidden()
        ;

        auto status = options.addSection(section);
        if (!status.isOK()) {
            return status;
        }
    }

    return Status::OK();
}

}  // namespace mongo