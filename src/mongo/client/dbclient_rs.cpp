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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kNetwork

#include "mongo/platform/basic.h"

#include "mongo/client/dbclient_rs.h"

#include <memory>
#include <utility>

#include "mongo/bson/util/builder.h"
#include "mongo/client/connpool.h"
#include "mongo/client/dbclient_cursor.h"
#include "mongo/client/global_conn_pool.h"
#include "mongo/client/read_preference.h"
#include "mongo/client/replica_set_monitor.h"
#include "mongo/db/auth/sasl_command_constants.h"
#include "mongo/db/dbmessage.h"
#include "mongo/db/jsobj.h"
#include "mongo/logv2/log.h"

namespace mongo {

using std::endl;
using std::map;
using std::set;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace {

/*
 * Set of commands that can be used with $readPreference
 */
set<string> _secOkCmdList;

class PopulateReadPrefSecOkCmdList {
public:
    PopulateReadPrefSecOkCmdList() {
        _secOkCmdList.insert("aggregate");
        _secOkCmdList.insert("collStats");
        _secOkCmdList.insert("count");
        _secOkCmdList.insert("distinct");
        _secOkCmdList.insert("dbStats");
        _secOkCmdList.insert("explain");
        _secOkCmdList.insert("find");
        _secOkCmdList.insert("geoNear");
        _secOkCmdList.insert("geoSearch");
        _secOkCmdList.insert("group");
    }
} _populateReadPrefSecOkCmdList;

/**
 * Maximum number of retries to make for auto-retry logic when performing a slave ok operation.
 */
const size_t MAX_RETRY = 3;

/**
 * Extracts the read preference settings from the query document. Note that this method
 * assumes that the query is ok for secondaries so it defaults to
 * ReadPreference::SecondaryPreferred when nothing is specified. Supports the following
 * format:
 *
 * Format A (official format):
 * { query: <actual query>, $readPreference: <read pref obj> }
 *
 * Format B (unofficial internal format from mongos):
 * { <actual query>, $queryOptions: { $readPreference: <read pref obj> }}
 *
 * @param query the raw query document
 *
 * @return the read preference setting if a read preference exists, otherwise the default read
 *         preference of Primary_Only. If the tags field was not present, it will contain one
 *         empty tag document {} which matches any tag.
 *
 * @throws AssertionException if the read preference object is malformed
 */
std::unique_ptr<ReadPreferenceSetting> _extractReadPref(const BSONObj& query, int queryOptions) {
    // Default read pref is primary only or secondary preferred with slaveOK
    const auto defaultReadPref = queryOptions & QueryOption_SlaveOk
        ? ReadPreference::SecondaryPreferred
        : ReadPreference::PrimaryOnly;

    auto readPrefContainingObj = query;
    if (auto elem = query["$queryOptions"]) {
        // The readPreference is embedded in the $queryOptions field.
        readPrefContainingObj = elem.Obj();
    }
    return std::make_unique<ReadPreferenceSetting>(uassertStatusOK(
        ReadPreferenceSetting::fromContainingBSON(readPrefContainingObj, defaultReadPref)));
}

}  // namespace

// --------------------------------
// ----- DBClientReplicaSet ---------
// --------------------------------

bool DBClientReplicaSet::_authPooledSecondaryConn = true;

DBClientReplicaSet::DBClientReplicaSet(const string& name,
                                       const vector<HostAndPort>& servers,
                                       StringData applicationName,
                                       double so_timeout,
                                       MongoURI uri)
    : _setName(name),
      _applicationName(applicationName.toString()),
      _so_timeout(so_timeout),
      _uri(std::move(uri)) {
    if (_uri.isValid()) {
        _rsm = ReplicaSetMonitor::createIfNeeded(_uri);
    } else {
        _rsm = ReplicaSetMonitor::createIfNeeded(name,
                                                 set<HostAndPort>(servers.begin(), servers.end()));
    }
}

ReplicaSetMonitorPtr DBClientReplicaSet::_getMonitor() {
    // If you can't get a ReplicaSetMonitor then this connection isn't valid
    uassert(16340,
            str::stream() << "No replica set monitor active and no cached seed "
                             "found for set: "
                          << _setName,
            _rsm);
    return _rsm;
}

// This can't throw an exception because it is called in the destructor of ScopedDbConnection
string DBClientReplicaSet::getServerAddress() const {
    if (!_rsm) {
        LOGV2_WARNING(20147,
                      "Trying to get server address for DBClientReplicaSet, "
                      "but no ReplicaSetMonitor exists for {replicaSet}",
                      "Trying to get server address for DBClientReplicaSet, "
                      "but no ReplicaSetMonitor exists",
                      "replicaSet"_attr = _setName);
        return str::stream() << _setName << "/";
    }
    return _rsm->getServerAddress();
}

HostAndPort DBClientReplicaSet::getSuspectedPrimaryHostAndPort() const {
    if (!_master) {
        return HostAndPort();
    }
    return _master->getServerHostAndPort();
}

void DBClientReplicaSet::setRequestMetadataWriter(rpc::RequestMetadataWriter writer) {
    // Set the hooks in both our sub-connections and in ourselves.
    if (_master) {
        _master->setRequestMetadataWriter(writer);
    }
    if (_lastSlaveOkConn.get()) {
        _lastSlaveOkConn->setRequestMetadataWriter(writer);
    }
    DBClientBase::setRequestMetadataWriter(std::move(writer));
}

void DBClientReplicaSet::setReplyMetadataReader(rpc::ReplyMetadataReader reader) {
    // Set the hooks in both our sub-connections and in ourselves.
    if (_master) {
        _master->setReplyMetadataReader(reader);
    }
    if (_lastSlaveOkConn.get()) {
        _lastSlaveOkConn->setReplyMetadataReader(reader);
    }
    DBClientBase::setReplyMetadataReader(std::move(reader));
}

int DBClientReplicaSet::getMinWireVersion() {
    return _getMonitor()->getMinWireVersion();
}

int DBClientReplicaSet::getMaxWireVersion() {
    return _getMonitor()->getMaxWireVersion();
}

// A replica set connection is never disconnected, since it controls its own reconnection
// logic.
//
// Has the side effect of proactively clearing any cached connections which have been
// disconnected in the background.
bool DBClientReplicaSet::isStillConnected() {
    if (_master && !_master->isStillConnected()) {
        resetMaster();
        // Don't notify monitor of bg failure, since it's not clear how long ago it happened
    }

    if (_lastSlaveOkConn.get() && !_lastSlaveOkConn->isStillConnected()) {
        resetSlaveOkConn();
        // Don't notify monitor of bg failure, since it's not clear how long ago it happened
    }

    return true;
}

namespace {

bool _isSecondaryCommand(StringData commandName, const BSONObj& commandArgs) {
    if (_secOkCmdList.count(commandName.toString())) {
        return true;
    }
    if (commandName == "mapReduce" || commandName == "mapreduce") {
        if (!commandArgs.hasField("out")) {
            return false;
        }

        BSONElement outElem(commandArgs["out"]);
        if (outElem.isABSONObj() && outElem["inline"].ok()) {
            return true;
        }
    }
    return false;
}

// Internal implementation of isSecondaryQuery, takes previously-parsed read preference
bool _isSecondaryQuery(const string& ns,
                       const BSONObj& queryObj,
                       const ReadPreferenceSetting& readPref) {
    // If the read pref is primary only, this is not a secondary query
    if (readPref.pref == ReadPreference::PrimaryOnly)
        return false;

    if (ns.find(".$cmd") == string::npos) {
        return true;
    }

    // This is a command with secondary-possible read pref
    // Only certain commands are supported for secondary operation.

    BSONObj actualQueryObj;
    if (strcmp(queryObj.firstElement().fieldName(), "$query") == 0) {
        actualQueryObj = queryObj["$query"].embeddedObject();
    } else if (strcmp(queryObj.firstElement().fieldName(), "query") == 0) {
        actualQueryObj = queryObj["query"].embeddedObject();
    } else {
        actualQueryObj = queryObj;
    }

    StringData commandName = actualQueryObj.firstElementFieldName();
    return _isSecondaryCommand(commandName, actualQueryObj);
}

}  // namespace


bool DBClientReplicaSet::isSecondaryQuery(const string& ns,
                                          const BSONObj& queryObj,
                                          int queryOptions) {
    unique_ptr<ReadPreferenceSetting> readPref(_extractReadPref(queryObj, queryOptions));
    return _isSecondaryQuery(ns, queryObj, *readPref);
}

DBClientConnection* DBClientReplicaSet::checkMaster() {
    ReplicaSetMonitorPtr monitor = _getMonitor();
    HostAndPort h = monitor->getMasterOrUassert();

    if (h == _masterHost && _master) {
        // a master is selected.  let's just make sure connection didn't die
        if (!_master->isFailed())
            return _master.get();

        monitor->failedHost(_masterHost,
                            {ErrorCodes::Error(40657), "Last known master host cannot be reached"});
        h = monitor->getMasterOrUassert();  // old master failed, try again.
    }

    _masterHost = h;

    MongoURI masterUri = _uri.cloneURIForServer(_masterHost, _applicationName);

    string errmsg;
    DBClientConnection* newConn = nullptr;
    boost::optional<double> socketTimeout;
    if (_so_timeout > 0.0)
        socketTimeout = _so_timeout;

    try {
        // Needs to perform a dynamic_cast because we need to set the replSet
        // callback. We should eventually not need this after we remove the
        // callback.
        newConn = dynamic_cast<DBClientConnection*>(
            masterUri.connect(_applicationName, errmsg, socketTimeout));
    } catch (const AssertionException& ex) {
        errmsg = ex.toString();
    }

    if (newConn == nullptr || !errmsg.empty()) {
        const std::string message = str::stream()
            << "can't connect to new replica set master [" << _masterHost.toString() << "]"
            << (errmsg.empty() ? "" : ", err: ") << errmsg;
        monitor->failedHost(_masterHost, {ErrorCodes::Error(40659), message});
        uasserted(ErrorCodes::FailedToSatisfyReadPreference, message);
    }

    resetMaster();

    _masterHost = h;
    _master.reset(newConn);
    _master->setParentReplSetName(_setName);
    _master->setRequestMetadataWriter(getRequestMetadataWriter());
    _master->setReplyMetadataReader(getReplyMetadataReader());

    _authConnection(_master.get());
    return _master.get();
}

bool DBClientReplicaSet::checkLastHost(const ReadPreferenceSetting* readPref) {
    // Can't use a cached host if we don't have one.
    if (!_lastSlaveOkConn.get() || _lastSlaveOkHost.empty()) {
        return false;
    }

    // Don't pin if the readPrefs differ.
    if (!_lastReadPref || !_lastReadPref->equals(*readPref)) {
        return false;
    }

    // Make sure we don't think the host is down.
    if (_lastSlaveOkConn->isFailed() || !_getMonitor()->isHostUp(_lastSlaveOkHost)) {
        _invalidateLastSlaveOkCache(
            {ErrorCodes::Error(40660), "Last slave connection is no longer available"});
        return false;
    }

    return true;
}

void DBClientReplicaSet::_authConnection(DBClientConnection* conn) {
    if (_internalAuthRequested) {
        auto status = conn->authenticateInternalUser();
        if (!status.isOK()) {
            LOGV2_WARNING(20148,
                          "Cached auth failed for set {replicaSet}: {error}",
                          "Cached auth failed",
                          "replicaSet"_attr = _setName,
                          "error"_attr = status);
        }
        return;
    }

    for (map<string, BSONObj>::const_iterator i = _auths.begin(); i != _auths.end(); ++i) {
        try {
            conn->auth(i->second);
        } catch (const AssertionException&) {
            LOGV2_WARNING(20149,
                          "Cached auth failed for set: {replicaSet} db: {db} user: {user}",
                          "Cached auth failed",
                          "replicaSet"_attr = _setName,
                          "db"_attr = i->second[saslCommandUserDBFieldName].str(),
                          "user"_attr = i->second[saslCommandUserFieldName].str());
        }
    }
}

void DBClientReplicaSet::logoutAll(DBClientConnection* conn) {
    _internalAuthRequested = false;
    for (map<string, BSONObj>::const_iterator i = _auths.begin(); i != _auths.end(); ++i) {
        BSONObj response;
        try {
            conn->logout(i->first, response);
        } catch (const AssertionException& ex) {
            LOGV2_WARNING(20150,
                          "Failed to logout: {connString} on db: {db} with error: {error}",
                          "Failed to logout",
                          "connString"_attr = conn->getServerAddress(),
                          "db"_attr = i->first,
                          "error"_attr = redact(ex));
        }
    }
}

DBClientConnection& DBClientReplicaSet::masterConn() {
    return *checkMaster();
}

DBClientConnection& DBClientReplicaSet::slaveConn() {
    shared_ptr<ReadPreferenceSetting> readPref(
        new ReadPreferenceSetting(ReadPreference::SecondaryPreferred, TagSet()));
    DBClientConnection* conn = selectNodeUsingTags(readPref);

    uassert(16369,
            str::stream() << "No good nodes available for set: " << _getMonitor()->getName(),
            conn != nullptr);

    return *conn;
}

bool DBClientReplicaSet::connect() {
    // Returns true if there are any up hosts.
    const ReadPreferenceSetting anyUpHost(ReadPreference::Nearest, TagSet());
    return _getMonitor()->getHostOrRefresh(anyUpHost).getNoThrow().isOK();
}

template <typename Authenticate>
Status DBClientReplicaSet::_runAuthLoop(Authenticate authCb) {
    // We prefer to authenticate against a primary, but otherwise a secondary is ok too
    // Empty tag matches every secondary
    const auto readPref =
        std::make_shared<ReadPreferenceSetting>(ReadPreference::PrimaryPreferred, TagSet());

    LOGV2_DEBUG(20132,
                3,
                "dbclient_rs attempting authentication of {replicaSet}",
                "dbclient_rs attempting authentication",
                "replicaSet"_attr = _getMonitor()->getName());

    // NOTE that we retry MAX_RETRY + 1 times, since we're always primary preferred we don't
    // fallback to the primary.
    Status lastNodeStatus = Status::OK();
    for (size_t retry = 0; retry < MAX_RETRY + 1; retry++) {
        try {
            auto conn = selectNodeUsingTags(readPref);
            if (conn == nullptr) {
                break;
            }

            authCb(conn);

            // Ensure the only child connection open is the one we authenticated against - other
            // child connections may not have full authentication information.
            // NOTE: _lastSlaveOkConn may or may not be the same as _master
            dassert(_lastSlaveOkConn.get() == conn || _master.get() == conn);
            if (conn != _lastSlaveOkConn.get()) {
                resetSlaveOkConn();
            }
            if (conn != _master.get()) {
                resetMaster();
            }

            return Status::OK();
        } catch (const DBException& e) {
            auto status = e.toStatus();
            if (status == ErrorCodes::AuthenticationFailed) {
                return status;
            }

            lastNodeStatus =
                status.withContext(str::stream() << "can't authenticate against replica set node "
                                                 << _lastSlaveOkHost);
            _invalidateLastSlaveOkCache(lastNodeStatus);
        }
    }

    if (lastNodeStatus.isOK()) {
        return Status(ErrorCodes::HostNotFound,
                      str::stream() << "Failed to authenticate, no good nodes in "
                                    << _getMonitor()->getName());
    } else {
        return lastNodeStatus;
    }
}

Status DBClientReplicaSet::authenticateInternalUser() {
    if (!auth::isInternalAuthSet()) {
        return {ErrorCodes::AuthenticationFailed,
                "No authentication parameters set for internal user"};
    }

    _internalAuthRequested = true;
    return _runAuthLoop(
        [&](DBClientConnection* conn) { uassertStatusOK(conn->authenticateInternalUser()); });
}

void DBClientReplicaSet::_auth(const BSONObj& params) {
    uassertStatusOK(_runAuthLoop([&](DBClientConnection* conn) {
        conn->auth(params);
        // Cache the new auth information since we now validated it's good
        _auths[params[saslCommandUserDBFieldName].str()] = params.getOwned();
    }));
}

void DBClientReplicaSet::logout(const string& dbname, BSONObj& info) {
    DBClientConnection* priConn = checkMaster();

    priConn->logout(dbname, info);
    _auths.erase(dbname);

    /* Also logout the cached secondary connection. Note that this is only
     * needed when we actually have something cached and is last known to be
     * working.
     */
    if (_lastSlaveOkConn.get() != nullptr && !_lastSlaveOkConn->isFailed()) {
        try {
            BSONObj dummy;
            _lastSlaveOkConn->logout(dbname, dummy);
        } catch (const DBException&) {
            // Make sure we can't use this connection again.
            verify(_lastSlaveOkConn->isFailed());
        }
    }
}

// ------------- simple functions -----------------

void DBClientReplicaSet::insert(const string& ns,
                                BSONObj obj,
                                int flags,
                                boost::optional<BSONObj> writeConcernObj) {
    checkMaster()->insert(ns, obj, flags, writeConcernObj);
}

void DBClientReplicaSet::insert(const string& ns,
                                const vector<BSONObj>& v,
                                int flags,
                                boost::optional<BSONObj> writeConcernObj) {
    checkMaster()->insert(ns, v, flags, writeConcernObj);
}

void DBClientReplicaSet::remove(const string& ns,
                                Query obj,
                                int flags,
                                boost::optional<BSONObj> writeConcernObj) {
    checkMaster()->remove(ns, obj, flags, writeConcernObj);
}

void DBClientReplicaSet::update(const string& ns,
                                Query query,
                                BSONObj obj,
                                int flags,
                                boost::optional<BSONObj> writeConcernObj) {
    return checkMaster()->update(ns, query, obj, flags, writeConcernObj);
}

unique_ptr<DBClientCursor> DBClientReplicaSet::query(const NamespaceStringOrUUID& nsOrUuid,
                                                     Query query,
                                                     int nToReturn,
                                                     int nToSkip,
                                                     const BSONObj* fieldsToReturn,
                                                     int queryOptions,
                                                     int batchSize,
                                                     boost::optional<BSONObj> readConcernObj) {
    shared_ptr<ReadPreferenceSetting> readPref(_extractReadPref(query.obj, queryOptions));
    invariant(nsOrUuid.nss());
    const string ns = nsOrUuid.nss()->ns();
    if (_isSecondaryQuery(ns, query.obj, *readPref)) {
        LOGV2_DEBUG(20133,
                    3,
                    "dbclient_rs query using secondary or tagged node selection in {replicaSet}, "
                    "read pref is {readPref} "
                    "(primary : {primary}, lastTagged : {lastTagged})",
                    "dbclient_rs query using secondary or tagged node selection",
                    "replicaSet"_attr = _getMonitor()->getName(),
                    "readPref"_attr = readPref->toString(),
                    "primary"_attr =
                        (_master.get() != nullptr ? _master->getServerAddress() : "[not cached]"),
                    "lastTagged"_attr =
                        (_lastSlaveOkConn.get() != nullptr ? _lastSlaveOkConn->getServerAddress()
                                                           : "[not cached]"));

        string lastNodeErrMsg;

        for (size_t retry = 0; retry < MAX_RETRY; retry++) {
            try {
                DBClientConnection* conn = selectNodeUsingTags(readPref);

                if (conn == nullptr) {
                    break;
                }

                unique_ptr<DBClientCursor> cursor = conn->query(nsOrUuid,
                                                                query,
                                                                nToReturn,
                                                                nToSkip,
                                                                fieldsToReturn,
                                                                queryOptions,
                                                                batchSize,
                                                                readConcernObj);

                return checkSlaveQueryResult(std::move(cursor));
            } catch (const DBException& ex) {
                const Status status = ex.toStatus(str::stream() << "can't query replica set node "
                                                                << _lastSlaveOkHost);
                lastNodeErrMsg = status.reason();
                _invalidateLastSlaveOkCache(status);
            }
        }

        StringBuilder assertMsg;
        assertMsg << "Failed to do query, no good nodes in " << _getMonitor()->getName();
        if (!lastNodeErrMsg.empty()) {
            assertMsg << ", last error: " << lastNodeErrMsg;
        }

        uasserted(16370, assertMsg.str());
    }

    LOGV2_DEBUG(20134,
                3,
                "dbclient_rs query to primary node in {replicaSet}",
                "dbclient_rs query to primary node",
                "replicaSet"_attr = _getMonitor()->getName());

    return checkMaster()->query(nsOrUuid,
                                query,
                                nToReturn,
                                nToSkip,
                                fieldsToReturn,
                                queryOptions,
                                batchSize,
                                readConcernObj);
}

BSONObj DBClientReplicaSet::findOne(const string& ns,
                                    const Query& query,
                                    const BSONObj* fieldsToReturn,
                                    int queryOptions,
                                    boost::optional<BSONObj> readConcernObj) {
    shared_ptr<ReadPreferenceSetting> readPref(_extractReadPref(query.obj, queryOptions));
    if (_isSecondaryQuery(ns, query.obj, *readPref)) {
        LOGV2_DEBUG(20135,
                    3,
                    "dbclient_rs findOne using secondary or tagged node selection in {replicaSet}, "
                    "read pref is {readPref} "
                    "(primary : {primary}, lastTagged : {lastTagged})",
                    "dbclient_rs findOne using secondary or tagged node selection",
                    "replicaSet"_attr = _getMonitor()->getName(),
                    "readPref"_attr = readPref->toString(),
                    "primary"_attr =
                        (_master.get() != nullptr ? _master->getServerAddress() : "[not cached]"),
                    "secondaryHostNamme"_attr =
                        (_lastSlaveOkConn.get() != nullptr ? _lastSlaveOkConn->getServerAddress()
                                                           : "[not cached]"));

        string lastNodeErrMsg;

        for (size_t retry = 0; retry < MAX_RETRY; retry++) {
            try {
                DBClientConnection* conn = selectNodeUsingTags(readPref);

                if (conn == nullptr) {
                    break;
                }

                return conn->findOne(ns, query, fieldsToReturn, queryOptions, readConcernObj);
            } catch (const DBException& ex) {
                const Status status = ex.toStatus(str::stream() << "can't findone replica set node "
                                                                << _lastSlaveOkHost.toString());
                lastNodeErrMsg = status.reason();
                _invalidateLastSlaveOkCache(status);
            }
        }

        StringBuilder assertMsg;
        assertMsg << "Failed to call findOne, no good nodes in " << _getMonitor()->getName();
        if (!lastNodeErrMsg.empty()) {
            assertMsg << ", last error: " << lastNodeErrMsg;
        }

        uasserted(16379, assertMsg.str());
    }

    LOGV2_DEBUG(20136,
                3,
                "dbclient_rs findOne to primary node in {replicaSet}",
                "dbclient_rs findOne to primary node",
                "replicaSet"_attr = _getMonitor()->getName());

    return checkMaster()->findOne(ns, query, fieldsToReturn, queryOptions, readConcernObj);
}

void DBClientReplicaSet::killCursor(const NamespaceString& ns, long long cursorID) {
    // we should never call killCursor on a replica set connection
    // since we don't know which server it belongs to
    // can't assume master because of slave ok
    // and can have a cursor survive a master change
    verify(0);
}

void DBClientReplicaSet::isntMaster() {
    // Can't use _getMonitor because that will create a new monitor from the cached seed if the
    // monitor doesn't exist.
    _rsm->failedHost(
        _masterHost,
        {ErrorCodes::NotMaster, str::stream() << "got not master for: " << _masterHost});

    resetMaster();
}

unique_ptr<DBClientCursor> DBClientReplicaSet::checkSlaveQueryResult(
    unique_ptr<DBClientCursor> result) {
    if (result.get() == nullptr)
        return result;

    BSONObj error;
    bool isError = result->peekError(&error);
    if (!isError)
        return result;

    // We only check for "not master or secondary" errors here

    // If the error code here ever changes, we need to change this code also
    BSONElement code = error["code"];
    if (code.isNumber() && code.Int() == ErrorCodes::NotMasterOrSecondary) {
        isntSecondary();
        uasserted(14812,
                  str::stream() << "slave " << _lastSlaveOkHost.toString()
                                << " is no longer secondary");
    }

    return result;
}

void DBClientReplicaSet::isntSecondary() {
    // Failover to next slave
    _getMonitor()->failedHost(
        _lastSlaveOkHost,
        {ErrorCodes::NotMasterOrSecondary,
         str::stream() << "slave no longer has secondary status: " << _lastSlaveOkHost});

    resetSlaveOkConn();
}

DBClientConnection* DBClientReplicaSet::selectNodeUsingTags(
    shared_ptr<ReadPreferenceSetting> readPref) {
    if (checkLastHost(readPref.get())) {
        LOGV2_DEBUG(20137,
                    3,
                    "dbclient_rs selecting compatible last used node {lastTagged}",
                    "dbclient_rs selecting compatible last used node",
                    "lastTagged"_attr = _lastSlaveOkHost);

        return _lastSlaveOkConn.get();
    }

    ReplicaSetMonitorPtr monitor = _getMonitor();

    auto selectedNodeStatus = monitor->getHostOrRefresh(*readPref).getNoThrow();
    if (!selectedNodeStatus.isOK()) {
        LOGV2_DEBUG(20138,
                    3,
                    "dbclient_rs no compatible node found: {error}",
                    "dbclient_rs no compatible node found",
                    "error"_attr = redact(selectedNodeStatus.getStatus()));
        return nullptr;
    }

    const HostAndPort selectedNode = std::move(selectedNodeStatus.getValue());

    // We are now about to get a new connection from the pool, so cleanup
    // the current one and release it back to the pool.
    resetSlaveOkConn();

    _lastReadPref = readPref;
    _lastSlaveOkHost = selectedNode;

    // Primary connection is special because it is the only connection that is
    // versioned in mongos. Therefore, we have to make sure that this object
    // maintains only one connection to the primary and use that connection
    // every time we need to talk to the primary.
    if (monitor->isPrimary(selectedNode)) {
        checkMaster();

        LOGV2_DEBUG(20139,
                    3,
                    "dbclient_rs selecting primary node {connString}",
                    "dbclient_rs selecting primary node",
                    "connString"_attr = selectedNode);

        _lastSlaveOkConn = _master;

        return _master.get();
    }

    auto dtor = [host = _lastSlaveOkHost.toString()](DBClientConnection* ptr) {
        globalConnPool.release(host, ptr);
    };

    // Needs to perform a dynamic_cast because we need to set the replSet
    // callback. We should eventually not need this after we remove the
    // callback.
    DBClientConnection* newConn = dynamic_cast<DBClientConnection*>(globalConnPool.get(
        _uri.cloneURIForServer(_lastSlaveOkHost, _applicationName), _so_timeout));

    // Assert here instead of returning NULL since the contract of this method is such
    // that returning NULL means none of the nodes were good, which is not the case here.
    uassert(16532,
            str::stream() << "Failed to connect to " << _lastSlaveOkHost.toString(),
            newConn != nullptr);

    _lastSlaveOkConn = std::shared_ptr<DBClientConnection>(newConn, std::move(dtor));
    _lastSlaveOkConn->setParentReplSetName(_setName);
    _lastSlaveOkConn->setRequestMetadataWriter(getRequestMetadataWriter());
    _lastSlaveOkConn->setReplyMetadataReader(getReplyMetadataReader());

    if (_authPooledSecondaryConn) {
        if (!_lastSlaveOkConn->authenticatedDuringConnect()) {
            _authConnection(_lastSlaveOkConn.get());
        }
    } else {
        // Mongos pooled connections are authenticated through ShardingConnectionHook::onCreate()
    }

    LOGV2_DEBUG(20140,
                3,
                "dbclient_rs selecting node {connString}",
                "dbclient_rs selecting node",
                "connString"_attr = _lastSlaveOkHost);

    return _lastSlaveOkConn.get();
}

void DBClientReplicaSet::say(Message& toSend, bool isRetry, string* actualServer) {
    if (!isRetry)
        _lazyState = LazyState();

    const int lastOp = toSend.operation();

    if (lastOp == dbQuery) {
        // TODO: might be possible to do this faster by changing api
        DbMessage dm(toSend);
        QueryMessage qm(dm);

        shared_ptr<ReadPreferenceSetting> readPref(_extractReadPref(qm.query, qm.queryOptions));
        if (_isSecondaryQuery(qm.ns, qm.query, *readPref)) {
            LOGV2_DEBUG(20141,
                        3,
                        "dbclient_rs say using secondary or tagged node selection in {replicaSet}, "
                        "read pref is {readPref} "
                        "(primary : {primary}, lastTagged : {lastTagged})",
                        "dbclient_rs say using secondary or tagged node selection",
                        "replicaSet"_attr = _getMonitor()->getName(),
                        "readPref"_attr = readPref->toString(),
                        "primary"_attr = (_master.get() != nullptr ? _master->getServerAddress()
                                                                   : "[not cached]"),
                        "lastTagged"_attr = (_lastSlaveOkConn.get() != nullptr
                                                 ? _lastSlaveOkConn->getServerAddress()
                                                 : "[not cached]"));

            string lastNodeErrMsg;

            for (size_t retry = 0; retry < MAX_RETRY; retry++) {
                _lazyState._retries = retry;
                try {
                    DBClientConnection* conn = selectNodeUsingTags(readPref);

                    if (conn == nullptr) {
                        break;
                    }

                    if (actualServer != nullptr) {
                        *actualServer = conn->getServerAddress();
                    }

                    conn->say(toSend);

                    _lazyState._lastOp = lastOp;
                    _lazyState._secondaryQueryOk = true;
                    _lazyState._lastClient = conn;
                } catch (const DBException& ex) {
                    const Status status =
                        ex.toStatus(str::stream() << "can't callLazy replica set node "
                                                  << _lastSlaveOkHost.toString());
                    lastNodeErrMsg = status.reason();
                    _invalidateLastSlaveOkCache(status);

                    continue;
                }

                return;
            }

            StringBuilder assertMsg;
            assertMsg << "Failed to call say, no good nodes in " << _getMonitor()->getName();
            if (!lastNodeErrMsg.empty()) {
                assertMsg << ", last error: " << lastNodeErrMsg;
            }

            uasserted(16380, assertMsg.str());
        }
    }

    LOGV2_DEBUG(20142,
                3,
                "dbclient_rs say to primary node in {replicaSet}",
                "dbclient_rs say to primary node",
                "replicaSet"_attr = _getMonitor()->getName());

    DBClientConnection* master = checkMaster();
    if (actualServer)
        *actualServer = master->getServerAddress();

    _lazyState._lastOp = lastOp;
    _lazyState._secondaryQueryOk = false;
    // Don't retry requests to primary since there is only one host to try
    _lazyState._retries = MAX_RETRY;
    _lazyState._lastClient = master;

    master->say(toSend);
    return;
}

Status DBClientReplicaSet::recv(Message& m, int lastRequestId) {
    verify(_lazyState._lastClient);

    try {
        return _lazyState._lastClient->recv(m, lastRequestId);
    } catch (DBException& e) {
        LOGV2(20143,
              "Could not receive data from {connString}: {error}",
              "Could not receive data",
              "connString"_attr = _lazyState._lastClient->toString(),
              "error"_attr = redact(e));
        return e.toStatus();
    }
}

void DBClientReplicaSet::checkResponse(const std::vector<BSONObj>& batch,
                                       bool networkError,
                                       bool* retry,
                                       string* targetHost) {
    // For now, do exactly as we did before, so as not to break things.  In general though, we
    // should fix this so checkResponse has a more consistent contract.
    if (!retry) {
        if (_lazyState._lastClient)
            return _lazyState._lastClient->checkResponse(batch, networkError);
        else
            return checkMaster()->checkResponse(batch, networkError);
    }

    *retry = false;
    if (targetHost && _lazyState._lastClient)
        *targetHost = _lazyState._lastClient->getServerAddress();
    else if (targetHost)
        *targetHost = "";

    if (!_lazyState._lastClient)
        return;

    // nReturned == 1 means that we got one result back, which might be an error
    // networkError is a sentinel value for "no data returned" aka (usually) network problem
    // If neither, this must be a query result so our response is ok wrt the replica set
    if (batch.size() != 1 && !networkError)
        return;

    BSONObj dataObj;
    if (batch.size() == 1)
        dataObj = batch[0];

    // Check if we should retry here
    if (_lazyState._lastOp == dbQuery && _lazyState._secondaryQueryOk) {
        // query could potentially go to a secondary, so see if this is an error (or empty) and
        // retry if we're not past our retry limit.

        if (networkError ||
            (hasErrField(dataObj) && !dataObj["code"].eoo() &&
             dataObj["code"].Int() == ErrorCodes::NotMasterOrSecondary)) {
            if (_lazyState._lastClient == _lastSlaveOkConn.get()) {
                isntSecondary();
            } else if (_lazyState._lastClient == _master.get()) {
                isntMaster();
            } else {
                LOGV2_WARNING(20151,
                              "Data {dataObj} is invalid because last rs client {connString} is "
                              "not master or secondary",
                              "Data is invalid because last rs client is not master or secondary",
                              "dataObj"_attr = redact(dataObj),
                              "connString"_attr = _lazyState._lastClient->toString());
            }

            if (_lazyState._retries < static_cast<int>(MAX_RETRY)) {
                _lazyState._retries++;
                *retry = true;
            } else {
                LOGV2(20144,
                      "Too many retries ({numRetries}), could not get data from replica set",
                      "Too many retries, could not get data from replica set",
                      "numRetries"_attr = _lazyState._retries);
            }
        }
    } else if (_lazyState._lastOp == dbQuery) {
        // if query could not potentially go to a secondary, just mark the master as bad

        if (networkError ||
            (hasErrField(dataObj) && !dataObj["code"].eoo() &&
             dataObj["code"].Int() == ErrorCodes::NotMasterNoSlaveOk)) {
            if (_lazyState._lastClient == _master.get()) {
                isntMaster();
            }
        }
    }
}

DBClientBase* DBClientReplicaSet::runFireAndForgetCommand(OpMsgRequest request) {
    // Assume all fire-and-forget commands should go to the primary node. It is currently used
    // for writes which need to go to the primary and for killCursors which should be sent to a
    // specific host rather than through DBClientReplicaSet.
    return checkMaster()->runFireAndForgetCommand(std::move(request));
}

std::pair<rpc::UniqueReply, DBClientBase*> DBClientReplicaSet::runCommandWithTarget(
    OpMsgRequest request) {
    // This overload exists so we can parse out the read preference and then use server
    // selection directly without having to re-parse the raw message.

    // TODO: eventually we will want to pass the metadata before serializing it to BSON
    // so we don't have to re-parse it, however, that will come with its own set of
    // complications (e.g. some kind of base class or concept for MetadataSerializable
    // objects). For now we do it the stupid way.
    auto readPref = uassertStatusOK(ReadPreferenceSetting::fromContainingBSON(request.body));

    if (readPref.pref == ReadPreference::PrimaryOnly ||
        // If the command is not runnable on a secondary, we run it on the primary
        // regardless of the read preference.
        !_isSecondaryCommand(request.getCommandName(), request.body)) {
        auto conn = checkMaster();
        return conn->runCommandWithTarget(std::move(request));
    }

    auto rpShared = std::make_shared<ReadPreferenceSetting>(std::move(readPref));

    for (size_t retry = 0; retry < MAX_RETRY; ++retry) {
        try {
            auto* conn = selectNodeUsingTags(rpShared);
            if (conn == nullptr) {
                break;
            }
            // We can't move the request since we need it to retry.
            return conn->runCommandWithTarget(request);
        } catch (const DBException& ex) {
            _invalidateLastSlaveOkCache(ex.toStatus());
        }
    }

    uasserted(ErrorCodes::HostNotFound,
              str::stream() << "Could not satisfy $readPreference of '" << readPref.toString()
                            << "' while attempting to run command " << request.getCommandName());
}

std::pair<rpc::UniqueReply, std::shared_ptr<DBClientBase>> DBClientReplicaSet::runCommandWithTarget(
    OpMsgRequest request, std::shared_ptr<DBClientBase> me) {

    auto out = runCommandWithTarget(std::move(request));

    std::shared_ptr<DBClientBase> conn = [&] {
        if (out.second == _lastSlaveOkConn.get()) {
            return _lastSlaveOkConn;
        }

        if (out.second == _master.get()) {
            return _master;
        }

        MONGO_UNREACHABLE;
    }();

    return {std::move(out.first), std::move(conn)};
}

bool DBClientReplicaSet::call(Message& toSend,
                              Message& response,
                              bool assertOk,
                              string* actualServer) {
    const char* ns = nullptr;

    if (toSend.operation() == dbQuery) {
        // TODO: might be possible to do this faster by changing api
        DbMessage dm(toSend);
        QueryMessage qm(dm);
        ns = qm.ns;

        shared_ptr<ReadPreferenceSetting> readPref(_extractReadPref(qm.query, qm.queryOptions));
        if (_isSecondaryQuery(ns, qm.query, *readPref)) {
            LOGV2_DEBUG(
                20145,
                3,
                "dbclient_rs call using secondary or tagged node selection in {replicaSet}, "
                "read pref is {readPref} "
                "(primary : {primary}, lastTagged : {lastTagged})",
                "dbclient_rs call using secondary or tagged node selection",
                "replicaSet"_attr = _getMonitor()->getName(),
                "readPref"_attr = readPref->toString(),
                "primary"_attr =
                    (_master.get() != nullptr ? _master->getServerAddress() : "[not cached]"),
                "lastTagged"_attr =
                    (_lastSlaveOkConn.get() != nullptr ? _lastSlaveOkConn->getServerAddress()
                                                       : "[not cached]"));

            for (size_t retry = 0; retry < MAX_RETRY; retry++) {
                try {
                    DBClientConnection* conn = selectNodeUsingTags(readPref);

                    if (conn == nullptr) {
                        return false;
                    }

                    if (actualServer != nullptr) {
                        *actualServer = conn->getServerAddress();
                    }

                    return conn->call(toSend, response, assertOk, nullptr);
                } catch (const DBException& ex) {
                    if (actualServer)
                        *actualServer = "";

                    const Status status = ex.toStatus();
                    _invalidateLastSlaveOkCache(status.withContext(
                        str::stream() << "can't call replica set node " << _lastSlaveOkHost));
                }
            }

            // Was not able to successfully send after max retries
            return false;
        }
    }

    LOGV2_DEBUG(20146,
                3,
                "dbclient_rs call to primary node in {replicaSet}",
                "dbclient_rs call to primary node",
                "replicaSet"_attr = _getMonitor()->getName());

    DBClientConnection* m = checkMaster();
    if (actualServer)
        *actualServer = m->getServerAddress();

    if (!m->call(toSend, response, assertOk, nullptr))
        return false;

    if (ns) {
        QueryResult::View res = response.singleData().view2ptr();
        if (res.getNReturned() == 1) {
            BSONObj x(res.data());
            if (str::contains(ns, "$cmd")) {
                if (isNotMasterErrorString(x["errmsg"]))
                    isntMaster();
            } else {
                if (isNotMasterErrorString(getErrField(x)))
                    isntMaster();
            }
        }
    }

    return true;
}

void DBClientReplicaSet::_invalidateLastSlaveOkCache(const Status& status) {
    // This is not wrapped in with if (_lastSlaveOkConn && _lastSlaveOkConn->isFailed()) because
    // there are certain exceptions that will not make the connection be labeled as failed. For
    // example, asserts 13079, 13080, 16386
    _getMonitor()->failedHost(_lastSlaveOkHost, status);
    resetSlaveOkConn();
}

void DBClientReplicaSet::reset() {
    resetSlaveOkConn();
    _lazyState._lastClient = nullptr;
    _lastReadPref.reset();
}

void DBClientReplicaSet::setAuthPooledSecondaryConn(bool setting) {
    _authPooledSecondaryConn = setting;
}

void DBClientReplicaSet::resetMaster() {
    if (_master.get() == _lastSlaveOkConn.get()) {
        _lastSlaveOkConn.reset();
        _lastSlaveOkHost = HostAndPort();
    }

    _master.reset();
    _masterHost = HostAndPort();
}

void DBClientReplicaSet::resetSlaveOkConn() {
    if (_lastSlaveOkConn.get() == _master.get()) {
        _lastSlaveOkConn.reset();
    } else if (_lastSlaveOkConn.get() != nullptr) {
        if (_authPooledSecondaryConn) {
            logoutAll(_lastSlaveOkConn.get());
        } else {
            // Mongos pooled connections are all authenticated with the same credentials;
            // so no need to logout.
        }

        _lastSlaveOkConn.reset();
    }

    _lastSlaveOkHost = HostAndPort();
}

}  // namespace mongo
