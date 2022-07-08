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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kSharding

#include "mongo/platform/basic.h"

#include "mongo/s/client/shard_registry.h"

#include <memory>
#include <set>

#include "mongo/bson/bsonobj.h"
#include "mongo/bson/util/bson_extract.h"
#include "mongo/client/connection_string.h"
#include "mongo/client/replica_set_monitor.h"
#include "mongo/db/client.h"
#include "mongo/db/logical_time_metadata_hook.h"
#include "mongo/db/operation_context.h"
#include "mongo/db/server_options.h"
#include "mongo/executor/network_connection_hook.h"
#include "mongo/executor/network_interface_factory.h"
#include "mongo/executor/network_interface_thread_pool.h"
#include "mongo/executor/task_executor.h"
#include "mongo/executor/task_executor_pool.h"
#include "mongo/executor/thread_pool_task_executor.h"
#include "mongo/logv2/log.h"
#include "mongo/platform/mutex.h"
#include "mongo/rpc/metadata/egress_metadata_hook_list.h"
#include "mongo/s/catalog/sharding_catalog_client.h"
#include "mongo/s/catalog/type_shard.h"
#include "mongo/s/client/shard.h"
#include "mongo/s/client/shard_factory.h"
#include "mongo/s/grid.h"
#include "mongo/util/concurrency/with_lock.h"
#include "mongo/util/map_util.h"
#include "mongo/util/scopeguard.h"
#include "mongo/util/str.h"

namespace mongo {

using std::set;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

using executor::NetworkInterface;
using executor::NetworkInterfaceThreadPool;
using executor::TaskExecutor;
using executor::TaskExecutorPool;
using executor::ThreadPoolTaskExecutor;
using CallbackArgs = TaskExecutor::CallbackArgs;
using CallbackHandle = TaskExecutor::CallbackHandle;


namespace {
const Seconds kRefreshPeriod(30);
}  // namespace

const ShardId ShardRegistry::kConfigServerShardId = ShardId("config");

ShardRegistry::ShardRegistry(std::unique_ptr<ShardFactory> shardFactory,
                             const ConnectionString& configServerCS,
                             std::vector<ShardRemovalHook> shardRemovalHooks)
    : _shardFactory(std::move(shardFactory)),
      _initConfigServerCS(configServerCS),
      _shardRemovalHooks(std::move(shardRemovalHooks)) {}

ShardRegistry::~ShardRegistry() {
    shutdown();
}

void ShardRegistry::shutdown() {
    if (_executor && !_isShutdown) {
        LOGV2_DEBUG(22723, 1, "Shutting down task executor for reloading shard registry");
        _executor->shutdown();
        _executor->join();
        _isShutdown = true;
    }
}

ConnectionString ShardRegistry::getConfigServerConnectionString() const {
    return getConfigShard()->getConnString();
}

StatusWith<shared_ptr<Shard>> ShardRegistry::getShard(OperationContext* opCtx,
                                                      const ShardId& shardId) {
    // If we know about the shard, return it.
    auto shard = _data.findShard(shardId);
    if (shard) {
        return shard;
    }

    // If we can't find the shard, attempt to reload the ShardRegistry.
    bool didReload = reload(opCtx);
    shard = _data.findShard(shardId);

    // If we found the shard, return it.
    if (shard) {
        return shard;
    }

    // If we did not find the shard but performed the reload
    // ourselves, return, because it means the shard does not exist.
    if (didReload) {
        return {ErrorCodes::ShardNotFound, str::stream() << "Shard " << shardId << " not found"};
    }

    // If we did not perform the reload ourselves (because there was a concurrent reload), force a
    // reload again to ensure that we have seen data at least as up to date as our first reload.
    reload(opCtx);
    shard = _data.findShard(shardId);

    if (shard) {
        return shard;
    }

    return {ErrorCodes::ShardNotFound, str::stream() << "Shard " << shardId << " not found"};
}

shared_ptr<Shard> ShardRegistry::getShardNoReload(const ShardId& shardId) {
    return _data.findShard(shardId);
}

shared_ptr<Shard> ShardRegistry::getShardForHostNoReload(const HostAndPort& host) {
    return _data.findByHostAndPort(host);
}

shared_ptr<Shard> ShardRegistry::getConfigShard() const {
    auto shard = _data.getConfigShard();
    invariant(shard);
    return shard;
}

unique_ptr<Shard> ShardRegistry::createConnection(const ConnectionString& connStr) const {
    return _shardFactory->createUniqueShard(ShardId("<unnamed>"), connStr);
}

shared_ptr<Shard> ShardRegistry::lookupRSName(const string& name) const {
    return _data.findByRSName(name);
}

void ShardRegistry::getAllShardIdsNoReload(vector<ShardId>* all) const {
    std::set<ShardId> seen;
    _data.getAllShardIds(seen);
    all->assign(seen.begin(), seen.end());
}

void ShardRegistry::getAllShardIds(OperationContext* opCtx, vector<ShardId>* all) {
    getAllShardIdsNoReload(all);
    if (all->empty()) {
        bool didReload = reload(opCtx);
        getAllShardIdsNoReload(all);
        // If we didn't do the reload ourselves, we should retry to ensure
        // that the reload is actually initiated while we're executing this
        if (!didReload && all->empty()) {
            reload(opCtx);
            getAllShardIdsNoReload(all);
        }
    }
}

int ShardRegistry::getNumShards() const {
    std::set<ShardId> seen;
    _data.getAllShardIds(seen);
    return seen.size();
}

void ShardRegistry::toBSON(BSONObjBuilder* result) const {
    _data.toBSON(result);
}

void ShardRegistry::updateReplSetHosts(const ConnectionString& newConnString) {
    invariant(newConnString.type() == ConnectionString::SET ||
              newConnString.type() == ConnectionString::CUSTOM);  // For dbtests

    // to prevent update config shard connection string during init
    stdx::unique_lock<Latch> lock(_reloadMutex);
    _data.rebuildShardIfExists(newConnString, _shardFactory.get());
}

void ShardRegistry::init() {
    stdx::unique_lock<Latch> reloadLock(_reloadMutex);
    invariant(_initConfigServerCS.isValid());
    auto configShard =
        _shardFactory->createShard(ShardRegistry::kConfigServerShardId, _initConfigServerCS);
    _data.addConfigShard(configShard);
    // set to invalid so it cant be started more than once.
    _initConfigServerCS = ConnectionString();
}

void ShardRegistry::startup(OperationContext* opCtx) {
    // startup() must be called only once
    invariant(!_executor);

    auto hookList = std::make_unique<rpc::EgressMetadataHookList>();
    hookList->addHook(std::make_unique<rpc::LogicalTimeMetadataHook>(opCtx->getServiceContext()));

    // construct task executor
    auto net = executor::makeNetworkInterface("ShardRegistryUpdater", nullptr, std::move(hookList));
    auto netPtr = net.get();
    _executor = std::make_unique<ThreadPoolTaskExecutor>(
        std::make_unique<NetworkInterfaceThreadPool>(netPtr), std::move(net));
    LOGV2_DEBUG(22724, 1, "Starting up task executor for periodic reloading of ShardRegistry");
    _executor->startup();

    auto status =
        _executor->scheduleWork([this](const CallbackArgs& cbArgs) { _internalReload(cbArgs); });

    if (status.getStatus() == ErrorCodes::ShutdownInProgress) {
        LOGV2_DEBUG(22725, 1, "Cant schedule Shard Registry reload. Executor shutdown in progress");
        return;
    }

    if (!status.isOK()) {
        LOGV2_FATAL(40252,
                    "Error scheduling shard registry reload caused by {error}",
                    "Error scheduling shard registry reload",
                    "error"_attr = redact(status.getStatus()));
    }
}

void ShardRegistry::_internalReload(const CallbackArgs& cbArgs) {
    LOGV2_DEBUG(22726, 1, "Reloading shardRegistry");
    if (!cbArgs.status.isOK()) {
        LOGV2_WARNING(22734,
                      "Error reloading shard registry caused by {error}",
                      "Error reloading shard registry",
                      "error"_attr = redact(cbArgs.status));
        return;
    }

    ThreadClient tc("shard-registry-reload", getGlobalServiceContext());

    auto opCtx = tc->makeOperationContext();

    try {
        reload(opCtx.get());
    } catch (const DBException& e) {
        LOGV2(22727,
              "Error running periodic reload of shard registry caused by {error}; will retry after "
              "{shardRegistryReloadInterval}",
              "Error running periodic reload of shard registry",
              "error"_attr = redact(e),
              "shardRegistryReloadInterval"_attr = kRefreshPeriod);
    }

    // reschedule itself
    auto status =
        _executor->scheduleWorkAt(_executor->now() + kRefreshPeriod,
                                  [this](const CallbackArgs& cbArgs) { _internalReload(cbArgs); });

    if (status.getStatus() == ErrorCodes::ShutdownInProgress) {
        LOGV2_DEBUG(
            22728, 1, "Error scheduling shard registry reload. Executor shutdown in progress");
        return;
    }

    if (!status.isOK()) {
        LOGV2_FATAL(40253,
                    "Error scheduling shard registry reload caused by {error}",
                    "Error scheduling shard registry reload",
                    "error"_attr = redact(status.getStatus()));
    }
}

bool ShardRegistry::isUp() const {
    stdx::unique_lock<Latch> reloadLock(_reloadMutex);
    return _isUp;
}

bool ShardRegistry::reload(OperationContext* opCtx) {
    stdx::unique_lock<Latch> reloadLock(_reloadMutex);

    if (_reloadState == ReloadState::Reloading) {
        // Another thread is already in the process of reloading so no need to do duplicate work.
        // There is also an issue if multiple threads are allowed to call getAllShards()
        // simultaneously because there is no good way to determine which of the threads has the
        // more recent version of the data.
        try {
            opCtx->waitForConditionOrInterrupt(
                _inReloadCV, reloadLock, [&] { return _reloadState != ReloadState::Reloading; });
        } catch (const DBException& e) {
            LOGV2_DEBUG(22729,
                        1,
                        "Error reloading shard registry caused by {error}",
                        "Error reloading shard registry",
                        "error"_attr = redact(e));
            return false;
        }

        if (_reloadState == ReloadState::Idle) {
            return false;
        }
        // else proceed to reload since an error occured on the last reload attempt.
        invariant(_reloadState == ReloadState::Failed);
    }

    _reloadState = ReloadState::Reloading;
    reloadLock.unlock();

    auto nextReloadState = ReloadState::Failed;

    auto failGuard = makeGuard([&] {
        if (!reloadLock.owns_lock()) {
            reloadLock.lock();
        }
        _reloadState = nextReloadState;
        _inReloadCV.notify_all();
    });

    ShardRegistryData currData(opCtx, _shardFactory.get());
    currData.addConfigShard(_data.getConfigShard());
    _data.swapAndMerge(currData);

    // Remove RSMs that are not in the catalog any more.
    std::set<ShardId> removedShardIds;
    currData.getAllShardIds(removedShardIds);
    _data.shardIdSetDifference(removedShardIds);

    for (auto& shardId : removedShardIds) {
        auto shard = currData.findByShardId(shardId);
        invariant(shard);

        auto name = shard->getConnString().getSetName();
        ReplicaSetMonitor::remove(name);
        for (auto& callback : _shardRemovalHooks) {
            // Run callbacks asynchronously.
            ExecutorFuture<void>(Grid::get(opCtx)->getExecutorPool()->getFixedExecutor())
                .getAsync([=](const Status&) { callback(shardId); });
        }
    }

    nextReloadState = ReloadState::Idle;
    // first successful reload means that registry is up
    _isUp = true;
    return true;
}

void ShardRegistry::clearEntries() {
    ShardRegistryData empty;
    empty.addConfigShard(_data.getConfigShard());
    _data.swap(empty);
}

void ShardRegistry::updateReplicaSetOnConfigServer(ServiceContext* serviceContext,
                                                   const ConnectionString& connStr) noexcept {
    ThreadClient tc("UpdateReplicaSetOnConfigServer", serviceContext);

    auto opCtx = tc->makeOperationContext();
    auto const grid = Grid::get(opCtx.get());

    std::shared_ptr<Shard> s = grid->shardRegistry()->lookupRSName(connStr.getSetName());
    if (!s) {
        LOGV2_DEBUG(22730,
                    1,
                    "Error updating replica set on config server. Couldn't find shard for "
                    "replica set {replicaSetConnectionStr}",
                    "Error updating replica set on config servers. Couldn't find shard",
                    "replicaSetConnectionStr"_attr = connStr);
        return;
    }

    if (s->isConfig()) {
        // No need to tell the config servers their own connection string.
        return;
    }

    auto swWasUpdated = grid->catalogClient()->updateConfigDocument(
        opCtx.get(),
        ShardType::ConfigNS,
        BSON(ShardType::name(s->getId().toString())),
        BSON("$set" << BSON(ShardType::host(connStr.toString()))),
        false,
        ShardingCatalogClient::kMajorityWriteConcern);
    auto status = swWasUpdated.getStatus();
    if (!status.isOK()) {
        LOGV2_ERROR(22736,
                    "Error updating replica set {replicaSetConnectionStr} on config server caused "
                    "by {error}",
                    "Error updating replica set on config server",
                    "replicaSetConnectionStr"_attr = connStr,
                    "error"_attr = redact(status));
    }
}

////////////// ShardRegistryData //////////////////

ShardRegistryData::ShardRegistryData(OperationContext* opCtx, ShardFactory* shardFactory) {
    auto const catalogClient = Grid::get(opCtx)->catalogClient();

    auto readConcern = repl::ReadConcernLevel::kMajorityReadConcern;

    // ShardRemote requires a majority read. We can only allow a non-majority read if we are a
    // config server.
    if (serverGlobalParams.clusterRole == ClusterRole::ConfigServer &&
        !repl::ReadConcernArgs::get(opCtx).isEmpty()) {
        readConcern = repl::ReadConcernArgs::get(opCtx).getLevel();
    }

    auto shardsAndOpTime =
        uassertStatusOKWithContext(catalogClient->getAllShards(opCtx, readConcern),
                                   "could not get updated shard list from config server");

    auto shards = std::move(shardsAndOpTime.value);
    auto reloadOpTime = std::move(shardsAndOpTime.opTime);

    LOGV2_DEBUG(22731,
                1,
                "Found {shardsNumber} shards listed on config server(s) with lastVisibleOpTime: "
                "{lastVisibleOpTime}",
                "Succesfully retrieved updated shard list from config server",
                "shardsNumber"_attr = shards.size(),
                "lastVisibleOpTime"_attr = reloadOpTime);

    // Ensure targeter exists for all shards and take shard connection string from the targeter.
    // Do this before re-taking the mutex to avoid deadlock with the ReplicaSetMonitor updating
    // hosts for a given shard.
    std::vector<std::tuple<std::string, ConnectionString>> shardsInfo;
    for (const auto& shardType : shards) {
        // This validation should ideally go inside the ShardType::validate call. However, doing
        // it there would prevent us from loading previously faulty shard hosts, which might have
        // been stored (i.e., the entire getAllShards call would fail).
        auto shardHostStatus = ConnectionString::parse(shardType.getHost());
        if (!shardHostStatus.isOK()) {
            LOGV2_WARNING(22735,
                          "Error parsing shard host caused by {error}",
                          "Error parsing shard host",
                          "error"_attr = redact(shardHostStatus.getStatus()));
            continue;
        }

        shardsInfo.push_back(std::make_tuple(shardType.getName(), shardHostStatus.getValue()));
    }

    for (auto& shardInfo : shardsInfo) {
        if (std::get<0>(shardInfo) == "config") {
            continue;
        }

        auto shard = shardFactory->createShard(std::move(std::get<0>(shardInfo)),
                                               std::move(std::get<1>(shardInfo)));

        _addShard(WithLock::withoutLock(), std::move(shard), false);
    }
}

void ShardRegistryData::swap(ShardRegistryData& other) {
    stdx::lock_guard<Latch> lk(_mutex);
    _shardIdLookup.swap(other._shardIdLookup);
    _rsLookup.swap(other._rsLookup);
    _hostLookup.swap(other._hostLookup);
    _connStringLookup.swap(other._connStringLookup);
    _configShard.swap(other._configShard);
}

void ShardRegistryData::swapAndMerge(ShardRegistryData& other) {
    stdx::lock_guard<Latch> lk(_mutex);
    _rsLookup.swap(other._rsLookup);
    _configShard.swap(other._configShard);
    _shardIdLookup.swap(other._shardIdLookup);

    for (auto it = other._connStringLookup.begin(); it != other._connStringLookup.end(); ++it) {
        auto res = _connStringLookup.find(it->first);
        if (res == _connStringLookup.end()) {
            _connStringLookup[it->first] = it->second;
        }
    }

    for (auto it = other._hostLookup.begin(); it != other._hostLookup.end(); ++it) {
        auto res = _hostLookup.find(it->first);
        if (res == _hostLookup.end()) {
            _hostLookup[it->first] = it->second;
        }
    }
}

shared_ptr<Shard> ShardRegistryData::getConfigShard() const {
    stdx::lock_guard<Latch> lk(_mutex);
    return _configShard;
}

void ShardRegistryData::addConfigShard(std::shared_ptr<Shard> shard) {
    stdx::lock_guard<Latch> lk(_mutex);
    _configShard = shard;
    _addShard(lk, shard, true);
}

shared_ptr<Shard> ShardRegistryData::findByRSName(const string& name) const {
    stdx::lock_guard<Latch> lk(_mutex);
    auto i = _rsLookup.find(name);
    return (i != _rsLookup.end()) ? i->second : nullptr;
}

shared_ptr<Shard> ShardRegistryData::findByHostAndPort(const HostAndPort& hostAndPort) const {
    stdx::lock_guard<Latch> lk(_mutex);
    return _findByHostAndPort(lk, hostAndPort);
}

shared_ptr<Shard> ShardRegistryData::findByShardId(const ShardId& shardId) const {
    stdx::lock_guard<Latch> lk(_mutex);
    return _findByShardId(lk, shardId);
}

shared_ptr<Shard> ShardRegistryData::_findByConnectionString(
    WithLock, const ConnectionString& connectionString) const {
    auto i = _connStringLookup.find(connectionString);
    return (i != _connStringLookup.end()) ? i->second : nullptr;
}

shared_ptr<Shard> ShardRegistryData::_findByHostAndPort(WithLock,
                                                        const HostAndPort& hostAndPort) const {
    return mapFindWithDefault(_hostLookup, hostAndPort);
}

shared_ptr<Shard> ShardRegistryData::_findByShardId(WithLock, ShardId const& shardId) const {
    auto i = _shardIdLookup.find(shardId);
    return (i != _shardIdLookup.end()) ? i->second : nullptr;
}

shared_ptr<Shard> ShardRegistryData::findShard(ShardId const& shardId) const {
    stdx::lock_guard<Latch> lk(_mutex);
    return _findShard(lk, shardId);
}

shared_ptr<Shard> ShardRegistryData::_findShard(WithLock lk, ShardId const& shardId) const {
    auto shard = _findByShardId(lk, shardId);
    if (shard) {
        return shard;
    }

    StatusWith<ConnectionString> swConnString = ConnectionString::parse(shardId.toString());
    if (swConnString.isOK()) {
        shard = _findByConnectionString(lk, swConnString.getValue());
        if (shard) {
            return shard;
        }
    }

    StatusWith<HostAndPort> swHostAndPort = HostAndPort::parse(shardId.toString());
    if (swHostAndPort.isOK()) {
        shard = _findByHostAndPort(lk, swHostAndPort.getValue());
        if (shard) {
            return shard;
        }
    }

    return nullptr;
}

void ShardRegistryData::toBSON(BSONObjBuilder* result) const {
    // Need to copy, then sort by shardId.
    std::vector<std::pair<ShardId, std::string>> shards;
    {
        stdx::lock_guard<Latch> lk(_mutex);
        shards.reserve(_shardIdLookup.size());
        for (auto&& shard : _shardIdLookup) {
            shards.emplace_back(shard.first, shard.second->getConnString().toString());
        }
    }

    std::sort(std::begin(shards), std::end(shards));

    BSONObjBuilder mapBob(result->subobjStart("map"));
    for (auto&& shard : shards) {
        mapBob.append(shard.first, shard.second);
    }
}

void ShardRegistryData::getAllShardIds(std::set<ShardId>& seen) const {
    stdx::lock_guard<Latch> lk(_mutex);
    for (auto i = _shardIdLookup.begin(); i != _shardIdLookup.end(); ++i) {
        const auto& s = i->second;
        if (s->getId().toString() == "config") {
            continue;
        }
        seen.insert(s->getId());
    }
}

void ShardRegistryData::shardIdSetDifference(std::set<ShardId>& diff) const {
    stdx::lock_guard<Latch> lk(_mutex);
    for (auto i = _shardIdLookup.begin(); i != _shardIdLookup.end(); ++i) {
        invariant(i->second);
        auto res = diff.find(i->second->getId());
        if (res != diff.end()) {
            diff.erase(res);
        }
    }
}

void ShardRegistryData::rebuildShardIfExists(const ConnectionString& newConnString,
                                             ShardFactory* factory) {
    stdx::unique_lock<Latch> updateConnStringLock(_mutex);
    auto it = _rsLookup.find(newConnString.getSetName());
    if (it == _rsLookup.end()) {
        return;
    }

    _rebuildShard(updateConnStringLock, newConnString, factory);
}


void ShardRegistryData::_rebuildShard(WithLock lk,
                                      ConnectionString const& newConnString,
                                      ShardFactory* factory) {
    auto it = _rsLookup.find(newConnString.getSetName());
    invariant(it->second);
    auto shard = factory->createShard(it->second->getId(), newConnString);
    _addShard(lk, shard, true);
    if (shard->isConfig()) {
        _configShard = shard;
    }
}

void ShardRegistryData::_addShard(WithLock lk,
                                  std::shared_ptr<Shard> const& shard,
                                  bool useOriginalCS) {
    const ShardId shardId = shard->getId();

    const ConnectionString connString =
        useOriginalCS ? shard->originalConnString() : shard->getConnString();

    auto currentShard = _findShard(lk, shardId);
    if (currentShard) {
        auto oldConnString = currentShard->originalConnString();

        if (oldConnString.toString() != connString.toString()) {
            LOGV2(22732,
                  "Updating shard registry connection string for shard {shardId} to "
                  "{newShardConnectionString} from {oldShardConnectionString}",
                  "Updating shard connection string on shard registry",
                  "shardId"_attr = currentShard->getId(),
                  "newShardConnectionString"_attr = connString,
                  "oldShardConnectionString"_attr = oldConnString);
        }

        for (const auto& host : oldConnString.getServers()) {
            _hostLookup.erase(host);
        }
        _connStringLookup.erase(oldConnString);
    }

    _shardIdLookup[shard->getId()] = shard;

    LOGV2_DEBUG(22733,
                3,
                "Adding new shard {shardId} with connection string {shardConnectionString} to "
                "shard registry",
                "Adding new shard to shard registry",
                "shardId"_attr = shard->getId(),
                "shardConnectionString"_attr = connString);
    if (connString.type() == ConnectionString::SET) {
        _rsLookup[connString.getSetName()] = shard;
    } else if (connString.type() == ConnectionString::CUSTOM) {
        // CUSTOM connection strings (ie "$dummy:10000) become DBDirectClient connections which
        // always return "localhost" as their response to getServerAddress().  This is just for
        // making dbtest work.
        _shardIdLookup[ShardId("localhost")] = shard;
        _hostLookup[HostAndPort("localhost")] = shard;
    }

    _connStringLookup[connString] = shard;

    for (const HostAndPort& hostAndPort : connString.getServers()) {
        _hostLookup[hostAndPort] = shard;
    }
}

}  // namespace mongo
