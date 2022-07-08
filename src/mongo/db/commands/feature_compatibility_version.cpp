/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kCommand

#include "mongo/platform/basic.h"

#include "mongo/db/commands/feature_compatibility_version.h"

#include "mongo/base/status.h"
#include "mongo/db/catalog_raii.h"
#include "mongo/db/commands/feature_compatibility_version_documentation.h"
#include "mongo/db/commands/feature_compatibility_version_gen.h"
#include "mongo/db/commands/feature_compatibility_version_parser.h"
#include "mongo/db/dbdirectclient.h"
#include "mongo/db/kill_sessions_local.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/operation_context.h"
#include "mongo/db/repl/optime.h"
#include "mongo/db/repl/replication_coordinator.h"
#include "mongo/db/repl/storage_interface.h"
#include "mongo/db/s/collection_sharding_state.h"
#include "mongo/db/s/sharding_state.h"
#include "mongo/db/service_context.h"
#include "mongo/db/storage/storage_engine.h"
#include "mongo/db/wire_version.h"
#include "mongo/db/write_concern_options.h"
#include "mongo/executor/egress_tag_closer_manager.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/get_status_from_command_result.h"
#include "mongo/s/catalog_cache.h"
#include "mongo/s/grid.h"
#include "mongo/transport/service_entry_point.h"

namespace mongo {

using repl::UnreplicatedWritesBlock;

Lock::ResourceMutex FeatureCompatibilityVersion::fcvLock("featureCompatibilityVersionLock");

MONGO_FAIL_POINT_DEFINE(hangBeforeAbortingRunningTransactionsOnFCVDowngrade);

void FeatureCompatibilityVersion::setTargetUpgrade(OperationContext* opCtx) {
    // Sets both 'version' and 'targetVersion' fields.
    _runUpdateCommand(opCtx, [](auto updateMods) {
        updateMods.append(FeatureCompatibilityVersionParser::kVersionField,
                          FeatureCompatibilityVersionParser::kVersion42);
        updateMods.append(FeatureCompatibilityVersionParser::kTargetVersionField,
                          FeatureCompatibilityVersionParser::kVersion44);
    });
}

void FeatureCompatibilityVersion::setTargetDowngrade(OperationContext* opCtx) {
    // Sets both 'version' and 'targetVersion' fields.
    _runUpdateCommand(opCtx, [](auto updateMods) {
        updateMods.append(FeatureCompatibilityVersionParser::kVersionField,
                          FeatureCompatibilityVersionParser::kVersion42);
        updateMods.append(FeatureCompatibilityVersionParser::kTargetVersionField,
                          FeatureCompatibilityVersionParser::kVersion42);
    });
}

void FeatureCompatibilityVersion::unsetTargetUpgradeOrDowngrade(OperationContext* opCtx,
                                                                StringData version) {
    _validateVersion(version);

    // Updates 'version' field, while also unsetting the 'targetVersion' field.
    _runUpdateCommand(opCtx, [version](auto updateMods) {
        updateMods.append(FeatureCompatibilityVersionParser::kVersionField, version);
    });
}

void FeatureCompatibilityVersion::setIfCleanStartup(OperationContext* opCtx,
                                                    repl::StorageInterface* storageInterface) {
    if (!isCleanStartUp())
        return;

    // If the server was not started with --shardsvr, the default featureCompatibilityVersion on
    // clean startup is the upgrade version. If it was started with --shardsvr, the default
    // featureCompatibilityVersion is the downgrade version, so that it can be safely added to a
    // downgrade version cluster. The config server will run setFeatureCompatibilityVersion as part
    // of addShard.
    const bool storeUpgradeVersion = serverGlobalParams.clusterRole != ClusterRole::ShardServer;

    UnreplicatedWritesBlock unreplicatedWritesBlock(opCtx);
    NamespaceString nss(NamespaceString::kServerConfigurationNamespace);

    {
        CollectionOptions options;
        options.uuid = CollectionUUID::gen();
        uassertStatusOK(storageInterface->createCollection(opCtx, nss, options));
    }

    // We then insert the featureCompatibilityVersion document into the server configuration
    // collection. The server parameter will be updated on commit by the op observer.
    uassertStatusOK(storageInterface->insertDocument(
        opCtx,
        nss,
        repl::TimestampedBSONObj{
            BSON("_id" << FeatureCompatibilityVersionParser::kParameterName
                       << FeatureCompatibilityVersionParser::kVersionField
                       << (storeUpgradeVersion ? FeatureCompatibilityVersionParser::kVersion44
                                               : FeatureCompatibilityVersionParser::kVersion42)),
            Timestamp()},
        repl::OpTime::kUninitializedTerm));  // No timestamp or term because this write is not
                                             // replicated.
}

bool FeatureCompatibilityVersion::isCleanStartUp() {
    StorageEngine* storageEngine = getGlobalServiceContext()->getStorageEngine();
    std::vector<std::string> dbNames = storageEngine->listDatabases();

    for (auto&& dbName : dbNames) {
        if (dbName != "local") {
            return false;
        }
    }
    return true;
}

void FeatureCompatibilityVersion::onInsertOrUpdate(OperationContext* opCtx, const BSONObj& doc) {
    auto idElement = doc["_id"];
    if (idElement.type() != BSONType::String ||
        idElement.String() != FeatureCompatibilityVersionParser::kParameterName) {
        return;
    }
    auto newVersion = uassertStatusOK(FeatureCompatibilityVersionParser::parse(doc));

    // To avoid extra log messages when the targetVersion is set/unset, only log when the version
    // changes.
    logv2::DynamicAttributes attrs;
    bool isDifferent = true;
    if (serverGlobalParams.featureCompatibility.isVersionInitialized()) {
        const auto currentVersion = serverGlobalParams.featureCompatibility.getVersion();
        attrs.add("currentVersion", FeatureCompatibilityVersionParser::toString(currentVersion));
        isDifferent = currentVersion != newVersion;
    }

    if (isDifferent) {
        attrs.add("newVersion", FeatureCompatibilityVersionParser::toString(newVersion));
        LOGV2(20459, "Setting featureCompatibilityVersion", attrs);
    }

    opCtx->recoveryUnit()->onCommit(
        [opCtx, newVersion](boost::optional<Timestamp>) { _setVersion(opCtx, newVersion); });
}

void FeatureCompatibilityVersion::updateMinWireVersion() {
    WireSpec& spec = WireSpec::instance();

    switch (serverGlobalParams.featureCompatibility.getVersion()) {
        case ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44:
        case ServerGlobalParams::FeatureCompatibility::Version::kUpgradingTo44:
        case ServerGlobalParams::FeatureCompatibility::Version::kDowngradingTo42:
            spec.incomingInternalClient.minWireVersion = LATEST_WIRE_VERSION;
            spec.outgoing.minWireVersion = LATEST_WIRE_VERSION;
            return;
        case ServerGlobalParams::FeatureCompatibility::Version::kFullyDowngradedTo42:
            spec.incomingInternalClient.minWireVersion = LATEST_WIRE_VERSION - 1;
            spec.outgoing.minWireVersion = LATEST_WIRE_VERSION - 1;
            return;
        case ServerGlobalParams::FeatureCompatibility::Version::kUnsetDefault42Behavior:
            // getVersion() does not return this value.
            MONGO_UNREACHABLE;
    }
}

void FeatureCompatibilityVersion::_setVersion(
    OperationContext* opCtx, ServerGlobalParams::FeatureCompatibility::Version newVersion) {
    serverGlobalParams.featureCompatibility.setVersion(newVersion);
    updateMinWireVersion();

    if (newVersion != ServerGlobalParams::FeatureCompatibility::Version::kFullyDowngradedTo42) {
        // Close all incoming connections from internal clients with binary versions lower than
        // ours.
        opCtx->getServiceContext()->getServiceEntryPoint()->endAllSessions(
            transport::Session::kLatestVersionInternalClientKeepOpen |
            transport::Session::kExternalClientKeepOpen);
        // Close all outgoing connections to servers with binary versions lower than ours.
        executor::EgressTagCloserManager::get(opCtx->getServiceContext())
            .dropConnections(transport::Session::kKeepOpen);
    }

    if (newVersion != ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44) {
        if (MONGO_unlikely(hangBeforeAbortingRunningTransactionsOnFCVDowngrade.shouldFail())) {
            LOGV2(20460,
                  "FeatureCompatibilityVersion - "
                  "hangBeforeAbortingRunningTransactionsOnFCVDowngrade fail point enabled, "
                  "blocking until fail point is disabled");
            hangBeforeAbortingRunningTransactionsOnFCVDowngrade.pauseWhileSet();
        }
        // Abort all open transactions when downgrading the featureCompatibilityVersion.
        SessionKiller::Matcher matcherAllSessions(
            KillAllSessionsByPatternSet{makeKillAllSessionsByPattern(opCtx)});
        killSessionsAbortUnpreparedTransactions(opCtx, matcherAllSessions);
    }
    const auto replCoordinator = repl::ReplicationCoordinator::get(opCtx);
    const bool isReplSet =
        replCoordinator->getReplicationMode() == repl::ReplicationCoordinator::modeReplSet;
    // We only want to increment the server TopologyVersion when the minWireVersion has changed.
    // This can only happen in two scenarios:
    // 1. Setting featureCompatibilityVersion from downgrading to fullyDowngraded.
    // 2. Setting featureCompatibilityVersion from fullyDowngraded to upgrading.
    const auto shouldIncrementTopologyVersion =
        newVersion == ServerGlobalParams::FeatureCompatibility::Version::kFullyDowngradedTo42 ||
        newVersion == ServerGlobalParams::FeatureCompatibility::Version::kUpgradingTo44;
    if (isReplSet && shouldIncrementTopologyVersion) {
        replCoordinator->incrementTopologyVersion();
    }
}

void FeatureCompatibilityVersion::onReplicationRollback(OperationContext* opCtx) {
    const auto query = BSON("_id" << FeatureCompatibilityVersionParser::kParameterName);
    const auto swFcv = repl::StorageInterface::get(opCtx)->findById(
        opCtx, NamespaceString::kServerConfigurationNamespace, query["_id"]);
    if (swFcv.isOK()) {
        const auto featureCompatibilityVersion = swFcv.getValue();
        auto swVersion = FeatureCompatibilityVersionParser::parse(featureCompatibilityVersion);
        const auto memoryFcv = serverGlobalParams.featureCompatibility.getVersion();
        if (swVersion.isOK() && (swVersion.getValue() != memoryFcv)) {
            auto diskFcv = swVersion.getValue();
            LOGV2(4675801,
                  "Setting featureCompatibilityVersion as part of rollback",
                  "newVersion"_attr = FeatureCompatibilityVersionParser::toString(diskFcv),
                  "oldVersion"_attr = FeatureCompatibilityVersionParser::toString(memoryFcv));
            _setVersion(opCtx, diskFcv);
        }
    }
}

void FeatureCompatibilityVersion::_validateVersion(StringData version) {
    uassert(40284,
            str::stream() << "featureCompatibilityVersion must be '"
                          << FeatureCompatibilityVersionParser::kVersion44 << "' or '"
                          << FeatureCompatibilityVersionParser::kVersion42 << "'. See "
                          << feature_compatibility_version_documentation::kCompatibilityLink << ".",
            version == FeatureCompatibilityVersionParser::kVersion44 ||
                version == FeatureCompatibilityVersionParser::kVersion42);
}

void FeatureCompatibilityVersion::_runUpdateCommand(OperationContext* opCtx,
                                                    UpdateBuilder builder) {
    DBDirectClient client(opCtx);
    NamespaceString nss(NamespaceString::kServerConfigurationNamespace);

    BSONObjBuilder updateCmd;
    updateCmd.append("update", nss.coll());
    {
        BSONArrayBuilder updates(updateCmd.subarrayStart("updates"));
        {
            BSONObjBuilder updateSpec(updates.subobjStart());
            {
                BSONObjBuilder queryFilter(updateSpec.subobjStart("q"));
                queryFilter.append("_id", FeatureCompatibilityVersionParser::kParameterName);
            }
            {
                BSONObjBuilder updateMods(updateSpec.subobjStart("u"));
                builder(std::move(updateMods));
            }
            updateSpec.appendBool("upsert", true);
        }
    }
    auto timeout = opCtx->getWriteConcern().usedDefault ? WriteConcernOptions::kNoTimeout
                                                        : opCtx->getWriteConcern().wTimeout;
    auto newWC = WriteConcernOptions(
        WriteConcernOptions::kMajority, WriteConcernOptions::SyncMode::UNSET, timeout);
    updateCmd.append(WriteConcernOptions::kWriteConcernField, newWC.toBSON());

    // Update the featureCompatibilityVersion document stored in the server configuration
    // collection.
    BSONObj updateResult;
    client.runCommand(nss.db().toString(), updateCmd.obj(), updateResult);
    uassertStatusOK(getStatusFromWriteCommandReply(updateResult));
}

/**
 * Read-only server parameter for featureCompatibilityVersion.
 */
// No ability to specify 'none' as set_at type,
// so use 'startup' in the IDL file, then override to none here.
FeatureCompatibilityVersionParameter::FeatureCompatibilityVersionParameter(StringData name,
                                                                           ServerParameterType)
    : ServerParameter(ServerParameterSet::getGlobal(), name, false, false) {}

void FeatureCompatibilityVersionParameter::append(OperationContext* opCtx,
                                                  BSONObjBuilder& b,
                                                  const std::string& name) {
    uassert(ErrorCodes::UnknownFeatureCompatibilityVersion,
            str::stream() << name << " is not yet known.",
            serverGlobalParams.featureCompatibility.isVersionInitialized());

    BSONObjBuilder featureCompatibilityVersionBuilder(b.subobjStart(name));
    switch (serverGlobalParams.featureCompatibility.getVersion()) {
        case ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44:
            featureCompatibilityVersionBuilder.append(
                FeatureCompatibilityVersionParser::kVersionField,
                FeatureCompatibilityVersionParser::kVersion44);
            return;
        case ServerGlobalParams::FeatureCompatibility::Version::kUpgradingTo44:
            featureCompatibilityVersionBuilder.append(
                FeatureCompatibilityVersionParser::kVersionField,
                FeatureCompatibilityVersionParser::kVersion42);
            featureCompatibilityVersionBuilder.append(
                FeatureCompatibilityVersionParser::kTargetVersionField,
                FeatureCompatibilityVersionParser::kVersion44);
            return;
        case ServerGlobalParams::FeatureCompatibility::Version::kDowngradingTo42:
            featureCompatibilityVersionBuilder.append(
                FeatureCompatibilityVersionParser::kVersionField,
                FeatureCompatibilityVersionParser::kVersion42);
            featureCompatibilityVersionBuilder.append(
                FeatureCompatibilityVersionParser::kTargetVersionField,
                FeatureCompatibilityVersionParser::kVersion42);
            return;
        case ServerGlobalParams::FeatureCompatibility::Version::kFullyDowngradedTo42:
            featureCompatibilityVersionBuilder.append(
                FeatureCompatibilityVersionParser::kVersionField,
                FeatureCompatibilityVersionParser::kVersion42);
            return;
        case ServerGlobalParams::FeatureCompatibility::Version::kUnsetDefault42Behavior:
            // getVersion() does not return this value.
            MONGO_UNREACHABLE;
    }
}

Status FeatureCompatibilityVersionParameter::setFromString(const std::string&) {
    return {ErrorCodes::IllegalOperation,
            str::stream() << name() << " cannot be set via setParameter. See "
                          << feature_compatibility_version_documentation::kCompatibilityLink
                          << "."};
}

}  // namespace mongo
