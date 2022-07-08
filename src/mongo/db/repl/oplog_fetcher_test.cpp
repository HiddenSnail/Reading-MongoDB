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

#include <memory>

#include "mongo/db/logical_clock.h"
#include "mongo/db/repl/data_replicator_external_state_mock.h"
#include "mongo/db/repl/oplog_fetcher.h"
#include "mongo/db/repl/repl_server_parameters_gen.h"
#include "mongo/db/repl/task_executor_mock.h"
#include "mongo/db/service_context_test_fixture.h"
#include "mongo/dbtests/mock/mock_dbclient_connection.h"
#include "mongo/executor/thread_pool_task_executor_test_fixture.h"
#include "mongo/rpc/metadata.h"
#include "mongo/rpc/metadata/logical_time_metadata.h"
#include "mongo/rpc/metadata/oplog_query_metadata.h"
#include "mongo/rpc/metadata/repl_set_metadata.h"
#include "mongo/unittest/death_test.h"
#include "mongo/unittest/task_executor_proxy.h"
#include "mongo/unittest/unittest.h"
#include "mongo/util/fail_point.h"
#include "mongo/util/scopeguard.h"

namespace {

using namespace mongo;
using namespace mongo::repl;
using namespace unittest;

HostAndPort source("localhost:12345");
NamespaceString nss("local.oplog.rs");

ReplSetConfig _createConfig() {
    BSONObjBuilder bob;
    bob.append("_id", "myset");
    bob.append("version", 1);
    bob.append("protocolVersion", 1);
    {
        BSONArrayBuilder membersBob(bob.subarrayStart("members"));
        BSONObjBuilder(membersBob.subobjStart())
            .appendElements(BSON("_id" << 0 << "host" << source.toString()));
    }
    {
        BSONObjBuilder settingsBob(bob.subobjStart("settings"));
        settingsBob.append("electionTimeoutMillis", 10000);
    }
    auto configObj = bob.obj();

    ReplSetConfig config;
    ASSERT_OK(config.initialize(configObj));
    return config;
}

BSONObj concatenate(BSONObj a, const BSONObj& b) {
    auto bob = BSONObjBuilder(std::move(a));
    bob.appendElements(b);
    return bob.obj();
}

BSONObj makeNoopOplogEntry(OpTime opTime) {
    auto oplogEntry =
        repl::OplogEntry(opTime,                           // optime
                         boost::none,                      // hash
                         OpTypeEnum ::kNoop,               // opType
                         NamespaceString("test.t"),        // namespace
                         boost::none,                      // uuid
                         boost::none,                      // fromMigrate
                         repl::OplogEntry::kOplogVersion,  // version
                         BSONObj(),                        // o
                         boost::none,                      // o2
                         {},                               // sessionInfo
                         boost::none,                      // upsert
                         Date_t(),                         // wall clock time
                         boost::none,                      // statement id
                         boost::none,   // optime of previous write within same transaction
                         boost::none,   // pre-image optime
                         boost::none);  // post-image optime
    return oplogEntry.toBSON();
}

BSONObj makeNoopOplogEntry(Seconds seconds) {
    return makeNoopOplogEntry({{seconds, 0}, 1LL});
}

BSONObj makeOplogBatchMetadata(boost::optional<const rpc::ReplSetMetadata&> replMetadata,
                               boost::optional<const rpc::OplogQueryMetadata&> oqMetadata) {
    BSONObjBuilder bob;
    if (replMetadata) {
        ASSERT_OK(replMetadata->writeToMetadata(&bob));
    }
    if (oqMetadata) {
        ASSERT_OK(oqMetadata->writeToMetadata(&bob));
    }
    return bob.obj();
}

Message makeFirstBatch(CursorId cursorId,
                       const OplogFetcher::Documents& oplogEntries,
                       const BSONObj& metadata) {
    return MockDBClientConnection::mockFindResponse(
        NamespaceString::kRsOplogNamespace, cursorId, oplogEntries, metadata);
}

Message makeSubsequentBatch(CursorId cursorId,
                            const OplogFetcher::Documents& oplogEntries,
                            const BSONObj& metadata,
                            bool moreToCome) {
    return MockDBClientConnection::mockGetMoreResponse(
        NamespaceString::kRsOplogNamespace, cursorId, oplogEntries, metadata, moreToCome);
}

bool blockedOnNetworkSoon(MockDBClientConnection* conn) {
    // Wait up to 10 seconds.
    for (auto i = 0; i < 100; i++) {
        if (conn->isBlockedOnNetwork()) {
            return true;
        }
        mongo::sleepmillis(100);
    }
    return false;
}

void validateMetadataRequest(OpMsg msg) {
    ASSERT_EQUALS(1, msg.body.getIntField("$replData"));
    ASSERT_EQUALS(1, msg.body.getIntField("$oplogQueryData"));
    ASSERT_BSONOBJ_EQ(BSON("mode"
                           << "secondaryPreferred"),
                      msg.body.getObjectField("$readPreference"));
}

void validateFindCommand(Message m, OpTime lastFetched, int findTimeout) {
    auto msg = mongo::OpMsg::parse(m);
    ASSERT_EQ(mongo::StringData(msg.body.firstElement().fieldName()), "find");
    ASSERT_TRUE(msg.body.getBoolField("tailable"));
    ASSERT_TRUE(msg.body.getBoolField("oplogReplay"));
    ASSERT_TRUE(msg.body.getBoolField("awaitData"));
    ASSERT_EQUALS(findTimeout, msg.body.getIntField("maxTimeMS"));
    ASSERT_BSONOBJ_EQ(BSON("ts" << BSON("$gte" << lastFetched.getTimestamp())),
                      msg.body.getObjectField("filter"));
    ASSERT_EQUALS(lastFetched.getTerm(), msg.body.getIntField("term"));
    ASSERT_BSONOBJ_EQ(BSON("level"
                           << "local"
                           << "afterClusterTime" << Timestamp(0, 1)),
                      msg.body.getObjectField("readConcern"));

    validateMetadataRequest(msg);
}

void validateGetMoreCommand(Message m,
                            int cursorId,
                            int timeout,
                            OpTimeWithTerm lastCommittedWithCurrentTerm,
                            bool exhaustSupported = true) {
    auto msg = mongo::OpMsg::parse(m);
    ASSERT_EQ(mongo::StringData(msg.body.firstElement().fieldName()), "getMore");
    ASSERT_EQ(cursorId, msg.body.getIntField("getMore"));
    ASSERT_EQUALS(timeout, msg.body.getIntField("maxTimeMS"));

    // In unittests, lastCommittedWithCurrentTerm should always be default to valid and non-null.
    // The case when currentTerm is kUninitializedTerm is tested separately in
    // GetMoreQueryDoesNotContainTermIfGetCurrentTermAndLastCommittedOpTimeReturnsUninitializedTerm.
    invariant(lastCommittedWithCurrentTerm.value != OpTime::kUninitializedTerm);
    invariant(!lastCommittedWithCurrentTerm.opTime.isNull());
    ASSERT_EQUALS(lastCommittedWithCurrentTerm.value, msg.body["term"].numberLong());
    ASSERT_EQUALS(lastCommittedWithCurrentTerm.opTime.getTimestamp(),
                  msg.body["lastKnownCommittedOpTime"]["ts"].timestamp());
    ASSERT_EQUALS(lastCommittedWithCurrentTerm.opTime.getTerm(),
                  msg.body["lastKnownCommittedOpTime"]["t"].numberLong());

    if (exhaustSupported) {
        ASSERT_TRUE(OpMsg::isFlagSet(m, OpMsg::kExhaustSupported));
    } else {
        ASSERT_FALSE(OpMsg::isFlagSet(m, OpMsg::kExhaustSupported));
    }

    validateMetadataRequest(msg);
}

// Simulate a response to a single outgoing client request and return the client request. Use this
// function to simulate responses to client find/getMore requests.
Message processSingleRequestResponse(DBClientConnection* conn,
                                     const StatusWith<Message> response,
                                     bool expectReadyNetworkOperationsAfterProcessing = false) {
    auto* mockConn = dynamic_cast<MockDBClientConnection*>(conn);
    ASSERT_TRUE(blockedOnNetworkSoon(mockConn));
    auto request = mockConn->getLastSentMessage();
    mockConn->setCallResponses({response});
    if (expectReadyNetworkOperationsAfterProcessing) {
        ASSERT_TRUE(blockedOnNetworkSoon(mockConn));
    }
    return request;
}

// Simulate a response to a single network recv() call. Use this function to simulate responses to
// exhaust stream where a client expects to receive responses without sending out new requests.
void processSingleExhaustResponse(DBClientConnection* conn,
                                  const StatusWith<Message> response,
                                  bool expectReadyNetworkOperationsAfterProcessing = false) {
    auto* mockConn = dynamic_cast<MockDBClientConnection*>(conn);
    ASSERT_TRUE(blockedOnNetworkSoon(mockConn));
    mockConn->setRecvResponses({response});
    if (expectReadyNetworkOperationsAfterProcessing) {
        ASSERT_TRUE(blockedOnNetworkSoon(mockConn));
    }
}

void simulateNetworkDisconnect(DBClientConnection* conn) {
    auto* mockConn = dynamic_cast<MockDBClientConnection*>(conn);
    ASSERT_TRUE(blockedOnNetworkSoon(mockConn));
    mockConn->shutdown();
}

class ShutdownState {
    ShutdownState(const ShutdownState&) = delete;
    ShutdownState& operator=(const ShutdownState&) = delete;

public:
    ShutdownState();

    /**
     * Returns the status at shutdown.
     */
    Status getStatus() const;

    /**
     * Use this for oplog fetcher shutdown callback.
     */
    void operator()(const Status& status);

private:
    Status _status = executor::TaskExecutorTest::getDetectableErrorStatus();
};

ShutdownState::ShutdownState() = default;

Status ShutdownState::getStatus() const {
    return _status;
}

void ShutdownState::operator()(const Status& status) {
    _status = status;
}

class OplogFetcherTest : public executor::ThreadPoolExecutorTest,
                         public ScopedGlobalServiceContextForTest {
protected:
    static const OpTime remoteNewerOpTime;
    static const OpTime staleOpTime;
    static const Date_t staleWallTime;
    static const int rbid = 2;
    static const int primaryIndex = 2;
    static const int syncSourceIndex = 2;
    static const rpc::OplogQueryMetadata oqMetadata;
    static const rpc::OplogQueryMetadata staleOqMetadata;
    static const rpc::ReplSetMetadata replSetMetadata;

    // 16MB max batch size / 12 byte min doc size * 10 (for good measure) = defaultBatchSize to use.
    const int defaultBatchSize = (16 * 1024 * 1024) / 12 * 10;

    void setUp() override;

    std::unique_ptr<OplogFetcher> makeOplogFetcher();
    std::unique_ptr<OplogFetcher> makeOplogFetcherWithDifferentExecutor(
        executor::TaskExecutor* executor,
        OplogFetcher::OnShutdownCallbackFn fn,
        int numRestarts = 0,
        bool requireFresherSyncSource = true,
        OplogFetcher::StartingPoint startingPoint = OplogFetcher::StartingPoint::kSkipFirstDoc);
    std::unique_ptr<OplogFetcher> getOplogFetcherAfterConnectionCreated(
        OplogFetcher::OnShutdownCallbackFn fn,
        int numRestarts = 0,
        bool requireFresherSyncSource = true,
        OplogFetcher::StartingPoint startingPoint = OplogFetcher::StartingPoint::kSkipFirstDoc);

    std::unique_ptr<ShutdownState> processSingleBatch(const Message& response,
                                                      bool shouldShutdown = false,
                                                      bool requireFresherSyncSource = true,
                                                      bool lastFetchedShouldAdvance = false);

    /**
     * Tests checkSyncSource result handling.
     */
    void testSyncSourceChecking(const rpc::ReplSetMetadata& replMetadata,
                                const rpc::OplogQueryMetadata& oqMetadata);

    void validateLastBatch(bool skipFirstDoc, OplogFetcher::Documents docs, OpTime lastFetched);

    std::unique_ptr<DataReplicatorExternalStateMock> dataReplicatorExternalState;

    OplogFetcher::Documents lastEnqueuedDocuments;
    OplogFetcher::DocumentsInfo lastEnqueuedDocumentsInfo;
    OplogFetcher::EnqueueDocumentsFn enqueueDocumentsFn;

    // The last OpTime fetched by the oplog fetcher.
    OpTime lastFetched;

    std::unique_ptr<MockRemoteDBServer> _mockServer;
};

const int OplogFetcherTest::rbid;
const OpTime OplogFetcherTest::remoteNewerOpTime = OpTime(Timestamp(1000, 1), 2);
const rpc::OplogQueryMetadata OplogFetcherTest::oqMetadata = rpc::OplogQueryMetadata(
    {staleOpTime, staleWallTime}, remoteNewerOpTime, rbid, primaryIndex, syncSourceIndex);

const OpTime OplogFetcherTest::staleOpTime = OpTime(Timestamp(1, 1), 0);
const Date_t OplogFetcherTest::staleWallTime = Date_t() + Seconds(staleOpTime.getSecs());
const rpc::OplogQueryMetadata OplogFetcherTest::staleOqMetadata = rpc::OplogQueryMetadata(
    {staleOpTime, staleWallTime}, staleOpTime, rbid, primaryIndex, syncSourceIndex);

const rpc::ReplSetMetadata OplogFetcherTest::replSetMetadata = rpc::ReplSetMetadata(
    1, OpTimeAndWallTime(), OpTime(), 1, 0, OID(), primaryIndex, syncSourceIndex, false);

void OplogFetcherTest::setUp() {
    executor::ThreadPoolExecutorTest::setUp();
    launchExecutorThread();

    lastFetched = {{123, 0}, 1};

    dataReplicatorExternalState = std::make_unique<DataReplicatorExternalStateMock>();
    dataReplicatorExternalState->currentTerm = lastFetched.getTerm();
    dataReplicatorExternalState->lastCommittedOpTime = {{9999, 0}, lastFetched.getTerm()};

    enqueueDocumentsFn = [this](OplogFetcher::Documents::const_iterator begin,
                                OplogFetcher::Documents::const_iterator end,
                                const OplogFetcher::DocumentsInfo& info) -> Status {
        lastEnqueuedDocuments = {begin, end};
        lastEnqueuedDocumentsInfo = info;
        return Status::OK();
    };

    auto target = HostAndPort{"localhost:12346"};
    _mockServer = std::make_unique<MockRemoteDBServer>(target.toString());

    // Always enable oplogFetcherUsesExhaust at the beginning of each unittest in case some
    // unittests disable it in the test.
    oplogFetcherUsesExhaust = true;

    // Set up a logical clock.
    auto service = getGlobalServiceContext();
    auto logicalClock = std::make_unique<LogicalClock>(service);
    LogicalClock::set(service, std::move(logicalClock));
}

std::unique_ptr<OplogFetcher> OplogFetcherTest::makeOplogFetcher() {
    return makeOplogFetcherWithDifferentExecutor(&getExecutor(), [](Status) {});
}

std::unique_ptr<OplogFetcher> OplogFetcherTest::getOplogFetcherAfterConnectionCreated(
    OplogFetcher::OnShutdownCallbackFn fn,
    int numRestarts,
    bool requireFresherSyncSource,
    OplogFetcher::StartingPoint startingPoint) {
    auto oplogFetcher = makeOplogFetcherWithDifferentExecutor(
        &getExecutor(), fn, numRestarts, requireFresherSyncSource, startingPoint);

    auto waitForConnCreatedFailPoint =
        globalFailPointRegistry().find("hangAfterOplogFetcherCallbackScheduled");
    auto timesEnteredFailPoint = waitForConnCreatedFailPoint->setMode(FailPoint::alwaysOn);

    ASSERT_FALSE(oplogFetcher->isActive());
    ASSERT_OK(oplogFetcher->startup());
    ASSERT_TRUE(oplogFetcher->isActive());

    // Ensure that the MockDBClientConnection was created before proceeding.
    waitForConnCreatedFailPoint->waitForTimesEntered(timesEnteredFailPoint + 1);
    waitForConnCreatedFailPoint->setMode(FailPoint::off);

    return oplogFetcher;
}

std::unique_ptr<OplogFetcher> OplogFetcherTest::makeOplogFetcherWithDifferentExecutor(
    executor::TaskExecutor* executor,
    OplogFetcher::OnShutdownCallbackFn fn,
    int numRestarts,
    bool requireFresherSyncSource,
    OplogFetcher::StartingPoint startingPoint) {
    auto oplogFetcher = std::make_unique<OplogFetcher>(
        executor,
        lastFetched,
        source,
        _createConfig(),
        std::make_unique<OplogFetcher::OplogFetcherRestartDecisionDefault>(numRestarts),
        rbid,
        requireFresherSyncSource,
        dataReplicatorExternalState.get(),
        enqueueDocumentsFn,
        fn,
        defaultBatchSize,
        startingPoint);
    oplogFetcher->setCreateClientFn_forTest([this]() {
        const auto autoReconnect = true;
        return std::unique_ptr<DBClientConnection>(
            new MockDBClientConnection(_mockServer.get(), autoReconnect));
    });
    return oplogFetcher;
}

std::unique_ptr<ShutdownState> OplogFetcherTest::processSingleBatch(const Message& response,
                                                                    bool shouldShutdown,
                                                                    bool requireFresherSyncSource,
                                                                    bool lastFetchedShouldAdvance) {
    auto shutdownState = std::make_unique<ShutdownState>();

    // Create an oplog fetcher with no retries.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(
        std::ref(*shutdownState), 0, requireFresherSyncSource);

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // We should only be blocked on the network after the response if we need to shut down the oplog
    // fetcher after this test.
    auto m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(), response, shouldShutdown);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    if (shouldShutdown) {
        oplogFetcher->shutdown();
    }
    oplogFetcher->join();

    if (!lastFetchedShouldAdvance) {
        ASSERT_EQUALS(lastFetched, oplogFetcher->getLastOpTimeFetched_forTest());
    }

    return shutdownState;
}

void OplogFetcherTest::testSyncSourceChecking(const rpc::ReplSetMetadata& replMetadata,
                                              const rpc::OplogQueryMetadata& oqMetadata) {
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto thirdEntry = makeNoopOplogEntry({{Seconds(789), 0}, lastFetched.getTerm()});

    auto metadataObj = makeOplogBatchMetadata(replMetadata, oqMetadata);

    dataReplicatorExternalState->shouldStopFetchingResult = true;

    auto shutdownState =
        processSingleBatch(makeFirstBatch(0, {firstEntry, secondEntry, thirdEntry}, metadataObj));

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource, shutdownState->getStatus());
}

void OplogFetcherTest::validateLastBatch(bool skipFirstDoc,
                                         OplogFetcher::Documents docs,
                                         OpTime lastFetched) {
    auto docs_iter = docs.begin();
    auto enqueue_iter = lastEnqueuedDocuments.begin();

    if (skipFirstDoc) {
        ASSERT_EQ(docs.size() - 1, lastEnqueuedDocuments.size());
        docs_iter++;
    } else {
        ASSERT_EQ(docs.size(), lastEnqueuedDocuments.size());
    }

    while (docs_iter != docs.end()) {
        ASSERT_BSONOBJ_EQ(*docs_iter++, *enqueue_iter++);
    }

    ASSERT_EQUALS(docs.back()["ts"].timestamp(), lastFetched.getTimestamp());
}

TEST_F(OplogFetcherTest, ShuttingExecutorDownShouldPreventOplogFetcherFromStarting) {
    getExecutor().shutdown();

    auto oplogFetcher = makeOplogFetcher();

    // Last optime fetched should match values passed to constructor.
    ASSERT_EQUALS(lastFetched, oplogFetcher->getLastOpTimeFetched_forTest());

    ASSERT_FALSE(oplogFetcher->isActive());
    ASSERT_EQUALS(ErrorCodes::ShutdownInProgress, oplogFetcher->startup());
    ASSERT_FALSE(oplogFetcher->isActive());

    // Last optime fetched should not change.
    ASSERT_EQUALS(lastFetched, oplogFetcher->getLastOpTimeFetched_forTest());
}

TEST_F(OplogFetcherTest, OplogFetcherReturnsOperationFailedIfExecutorFailsToScheduleRunQuery) {
    TaskExecutorMock taskExecutorMock(&getExecutor());
    taskExecutorMock.shouldFailScheduleWorkRequest = []() { return true; };

    // The onShutdownFn should not be called because the oplog fetcher should fail during startup.
    auto oplogFetcher =
        makeOplogFetcherWithDifferentExecutor(&taskExecutorMock, [](Status) { MONGO_UNREACHABLE; });

    // Last optime fetched should match values passed to constructor.
    ASSERT_EQUALS(lastFetched, oplogFetcher->getLastOpTimeFetched_forTest());

    ASSERT_FALSE(oplogFetcher->isActive());
    ASSERT_EQUALS(ErrorCodes::OperationFailed, oplogFetcher->startup());
    ASSERT_FALSE(oplogFetcher->isActive());

    // Last optime fetched should not change.
    ASSERT_EQUALS(lastFetched, oplogFetcher->getLastOpTimeFetched_forTest());
}

TEST_F(OplogFetcherTest, ShuttingExecutorDownAfterStartupButBeforeRunQueryScheduled) {
    ShutdownState shutdownState;

    // Defer scheduling work so that the executor's shutdown happens before startup's work is
    // scheduled.
    TaskExecutorMock taskExecutorMock(&getExecutor());
    taskExecutorMock.shouldDeferScheduleWorkRequestByOneSecond = []() { return true; };

    auto oplogFetcher =
        makeOplogFetcherWithDifferentExecutor(&taskExecutorMock, std::ref(shutdownState));

    ASSERT_FALSE(oplogFetcher->isActive());
    ASSERT_OK(oplogFetcher->startup());
    ASSERT_TRUE(oplogFetcher->isActive());

    getExecutor().shutdown();

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherReturnsCallbackCanceledIfShutdownBeforeRunQueryScheduled) {
    ShutdownState shutdownState;

    // Defer scheduling work so that the oplog fetcher's shutdown happens before startup's work is
    // scheduled.
    TaskExecutorMock taskExecutorMock(&getExecutor());
    taskExecutorMock.shouldDeferScheduleWorkRequestByOneSecond = []() { return true; };

    auto oplogFetcher =
        makeOplogFetcherWithDifferentExecutor(&taskExecutorMock, std::ref(shutdownState));

    ASSERT_FALSE(oplogFetcher->isActive());
    ASSERT_OK(oplogFetcher->startup());
    ASSERT_TRUE(oplogFetcher->isActive());

    oplogFetcher->shutdown();

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherReturnsCallbackCanceledIfShutdownAfterRunQueryScheduled) {
    // Tests shutting down after _runQuery is scheduled (but not while blocked on the network).

    ShutdownState shutdownState;

    auto waitForCallbackScheduledFailPoint =
        globalFailPointRegistry().find("hangAfterOplogFetcherCallbackScheduled");
    auto timesEnteredFailPoint = waitForCallbackScheduledFailPoint->setMode(FailPoint::alwaysOn, 0);

    // This will also ensure that _runQuery was scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    waitForCallbackScheduledFailPoint->waitForTimesEntered(timesEnteredFailPoint + 1);

    // Only call shutdown once we have confirmed that the callback is paused at the fail point.
    oplogFetcher->shutdown();

    // Unpause the oplog fetcher.
    waitForCallbackScheduledFailPoint->setMode(FailPoint::off);

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest,
       OplogFetcherReturnsHostUnreachableIfShutdownAfterRunQueryScheduledWhileBlockedOnCall) {
    // Tests that shutting down while the connection is blocked on call successfully shuts down the
    // connection as well.

    ShutdownState shutdownState;

    // This will also ensure that _runQuery was scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    // Make sure that we are blocked on the network before shutting down to make sure that shutting
    // down properly interrupts and shuts down the DBClientConnection.
    auto* mockConn =
        dynamic_cast<MockDBClientConnection*>(oplogFetcher->getDBClientConnection_forTest());
    ASSERT_TRUE(blockedOnNetworkSoon(mockConn));

    oplogFetcher->shutdown();

    oplogFetcher->join();

    // This is the error that the connection throws if shutdown while blocked on the network.
    ASSERT_EQUALS(ErrorCodes::HostUnreachable, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest,
       OplogFetcherReturnsCallbackCanceledIfShutdownAfterGettingBatchBeforeProcessing) {
    // Tests shutting down after getting the first batch, but before enqueuing it.

    ShutdownState shutdownState;

    // This will also ensure that _runQuery was scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    auto waitForFailPoint = globalFailPointRegistry().find("hangBeforeProcessingSuccessfulBatch");
    auto timesEnteredFailPoint = waitForFailPoint->setMode(FailPoint::alwaysOn, 0);

    // Successfully create a cursor and get the first batch.
    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(boost::none, oqMetadata);

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Creating the cursor will succeed.
    auto m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeFirstBatch(cursorId, {firstEntry, secondEntry}, metadataObj));

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Ensure that the oplog fetcher is paused before processing the successful batch.
    waitForFailPoint->waitForTimesEntered(timesEnteredFailPoint + 1);

    oplogFetcher->shutdown();

    // Unpause the oplog fetcher.
    waitForFailPoint->setMode(FailPoint::off);

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

bool sharedCallbackStateDestroyed = false;
bool sharedCallbackStateDestroyedSoon() {
    // Wait up to 10 seconds.
    for (auto i = 0; i < 100; i++) {
        if (sharedCallbackStateDestroyed) {
            return true;
        }
        mongo::sleepmillis(100);
    }
    return false;
}

class SharedCallbackState {
    SharedCallbackState(const SharedCallbackState&) = delete;
    SharedCallbackState& operator=(const SharedCallbackState&) = delete;

public:
    SharedCallbackState() {}
    ~SharedCallbackState() {
        sharedCallbackStateDestroyed = true;
    }
};

TEST_F(OplogFetcherTest, OplogFetcherResetsOnShutdownCallbackFnOnCompletion) {
    auto sharedCallbackData = std::make_shared<SharedCallbackState>();
    auto callbackInvoked = false;
    auto status = getDetectableErrorStatus();

    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(
        [&callbackInvoked, sharedCallbackData, &status](const Status& shutdownStatus) {
            status = shutdownStatus, callbackInvoked = true;
        });

    sharedCallbackData.reset();
    ASSERT_FALSE(sharedCallbackStateDestroyed);

    // This will cause the initial attempt to create a cursor to fail.
    processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                 Status{ErrorCodes::OperationFailed, "oplog tailing query failed"});

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::OperationFailed, status);

    // Oplog fetcher should reset 'OplogFetcher::_onShutdownCallbackFn' after running callback
    // function before becoming inactive.
    // This ensures that we release resources associated with
    // 'OplogFetcher::_onShutdownCallbackFn'.
    ASSERT_TRUE(callbackInvoked);

    // We need to check sharedCallbackStateDestroyed in a loop because SharedCallbackState's
    // desctructor is run after the oplog fetcher transitions to complete and outside of the oplog
    // fetcher's mutex, which means that it does not necessarily run before the oplog fetcher is
    // joined.
    ASSERT_TRUE(sharedCallbackStateDestroyedSoon());
}

TEST_F(OplogFetcherTest,
       FindQueryContainsTermIfGetCurrentTermAndLastCommittedOpTimeReturnsValidTerm) {
    // Test that the correct maxTimeMS is set if this is the initial 'find' query.
    auto oplogFetcher = makeOplogFetcher();
    auto findTimeout = durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest());
    auto queryObj = oplogFetcher->getFindQuery_forTest(findTimeout);
    ASSERT_EQUALS(60000, queryObj.getIntField("$maxTimeMS"));

    ASSERT_EQUALS(mongo::BSONType::Object, queryObj["query"].type());
    ASSERT_BSONOBJ_EQ(BSON("ts" << BSON("$gte" << lastFetched.getTimestamp())),
                      queryObj["query"].Obj());
    ASSERT_EQUALS(mongo::BSONType::Object, queryObj["readConcern"].type());
    ASSERT_BSONOBJ_EQ(BSON("level"
                           << "local"
                           << "afterClusterTime" << Timestamp(0, 1)),
                      queryObj["readConcern"].Obj());
    ASSERT_EQUALS(dataReplicatorExternalState->currentTerm, queryObj["term"].numberLong());
}

TEST_F(OplogFetcherTest,
       FindQueryDoesNotContainTermIfGetCurrentTermAndLastCommittedOpTimeReturnsUninitializedTerm) {
    dataReplicatorExternalState->currentTerm = OpTime::kUninitializedTerm;
    auto oplogFetcher = makeOplogFetcher();

    // Test that the correct maxTimeMS is set if we are retrying the 'find' query.
    auto findTimeout = durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest());
    auto queryObj = oplogFetcher->getFindQuery_forTest(findTimeout);
    ASSERT_EQUALS(2000, queryObj.getIntField("$maxTimeMS"));

    ASSERT_EQUALS(mongo::BSONType::Object, queryObj["query"].type());
    ASSERT_BSONOBJ_EQ(BSON("ts" << BSON("$gte" << lastFetched.getTimestamp())),
                      queryObj["query"].Obj());
    ASSERT_EQUALS(mongo::BSONType::Object, queryObj["readConcern"].type());
    ASSERT_BSONOBJ_EQ(BSON("level"
                           << "local"
                           << "afterClusterTime" << Timestamp(0, 1)),
                      queryObj["readConcern"].Obj());
    ASSERT_FALSE(queryObj.hasField("term"));
}

TEST_F(
    OplogFetcherTest,
    GetMoreQueryDoesNotContainTermIfGetCurrentTermAndLastCommittedOpTimeReturnsUninitializedTerm) {
    dataReplicatorExternalState->currentTerm = OpTime::kUninitializedTerm;

    ShutdownState shutdownState;

    // Create an oplog fetcher without any retries.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    auto conn = oplogFetcher->getDBClientConnection_forTest();

    // First batch for the initial find command.
    processSingleRequestResponse(conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);

    // Second and terminating batch (with cursorId 0) for the getMore command.
    auto secondEntry = makeNoopOplogEntry({{Seconds(124), 0}, lastFetched.getTerm()});
    auto m = processSingleRequestResponse(
        conn, makeSubsequentBatch(0LL, {secondEntry}, metadataObj, false /* moreToCome */), false);

    auto msg = mongo::OpMsg::parse(m);
    ASSERT_EQ(mongo::StringData(msg.body.firstElement().fieldName()), "getMore");
    // Test that the getMore query does not contain the term or the lastKnownCommittedOpTime field.
    ASSERT_FALSE(msg.body.hasField("term"));
    ASSERT_FALSE(msg.body.hasField("lastKnownCommittedOpTime"));

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, AwaitDataTimeoutShouldEqualHalfElectionTimeout) {
    auto config = _createConfig();
    auto timeout = makeOplogFetcher()->getAwaitDataTimeout_forTest();
    ASSERT_EQUALS(config.getElectionTimeoutPeriod() / 2, timeout);
}

TEST_F(OplogFetcherTest, AwaitDataTimeoutSmallerWhenFailPointSet) {
    auto failPoint = globalFailPointRegistry().find("setSmallOplogGetMoreMaxTimeMS");
    failPoint->setMode(FailPoint::alwaysOn);
    auto timeout = makeOplogFetcher()->getAwaitDataTimeout_forTest();
    ASSERT_EQUALS(Milliseconds(50), timeout);
    failPoint->setMode(FailPoint::off);
}

TEST_F(OplogFetcherTest, InvalidReplSetMetadataInResponseStopsTheOplogFetcher) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(lastFetched);
    auto metadataObj =
        BSON(rpc::kReplSetMetadataFieldName << BSON("invalid_repl_metadata_field" << 1));
    ASSERT_EQUALS(ErrorCodes::NoSuchKey,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());
}

TEST_F(OplogFetcherTest, InvalidOplogQueryMetadataInResponseStopsTheOplogFetcher) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(lastFetched);
    auto metadataObj =
        BSON(rpc::kOplogQueryMetadataFieldName << BSON("invalid_oq_metadata_field" << 1));
    ASSERT_EQUALS(ErrorCodes::NoSuchKey,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());
}

TEST_F(OplogFetcherTest, ValidMetadataInResponseWithoutOplogMetadataStopsTheOplogFetcher) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, boost::none);
    ASSERT_EQUALS(ErrorCodes::NoSuchKey,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());
}

TEST_F(OplogFetcherTest, ResponseWithoutReplicaSetMetadataStopsTheOplogFetcher) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(boost::none, oqMetadata);
    ASSERT_EQUALS(ErrorCodes::NoSuchKey,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());
}

TEST_F(OplogFetcherTest, ValidMetadataWithInResponseShouldBeForwardedToProcessMetadataFn) {
    CursorId cursorId = 0LL;
    auto entry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    ASSERT_OK(processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());
    ASSERT_TRUE(dataReplicatorExternalState->metadataWasProcessed);
    ASSERT_EQUALS(replSetMetadata.getPrimaryIndex(),
                  dataReplicatorExternalState->replMetadataProcessed.getPrimaryIndex());
    ASSERT_EQUALS(oqMetadata.getPrimaryIndex(),
                  dataReplicatorExternalState->oqMetadataProcessed.getPrimaryIndex());
}

TEST_F(OplogFetcherTest, MetadataAndBatchAreNotProcessedWhenSyncSourceRollsBack) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(lastFetched);

    rpc::OplogQueryMetadata oplogQueryMetadata(
        {staleOpTime, staleWallTime}, remoteNewerOpTime, rbid + 1, primaryIndex, syncSourceIndex);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oplogQueryMetadata);

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());

    ASSERT_FALSE(dataReplicatorExternalState->metadataWasProcessed);
    ASSERT(lastEnqueuedDocuments.empty());
}

TEST_F(OplogFetcherTest, MetadataAndBatchAreNotProcessedWhenSyncSourceIsBehind) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(staleOpTime);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, staleOqMetadata);

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());

    ASSERT_FALSE(dataReplicatorExternalState->metadataWasProcessed);
    ASSERT(lastEnqueuedDocuments.empty());
}

TEST_F(OplogFetcherTest, MetadataAndBatchAreNotProcessedWhenSyncSourceIsNotAhead) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(lastFetched);

    rpc::OplogQueryMetadata oplogQueryMetadata(
        {staleOpTime, staleWallTime}, lastFetched, rbid, primaryIndex, syncSourceIndex);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oplogQueryMetadata);

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());

    ASSERT_FALSE(dataReplicatorExternalState->metadataWasProcessed);
    ASSERT(lastEnqueuedDocuments.empty());
}

TEST_F(OplogFetcherTest,
       MetadataAndBatchAreNotProcessedWhenSyncSourceIsBehindWithoutRequiringFresherSyncSource) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry(staleOpTime);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, staleOqMetadata);

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj),
                                     false /* shouldShutdown */,
                                     false /* requireFresherSyncSource */)
                      ->getStatus());

    ASSERT_FALSE(dataReplicatorExternalState->metadataWasProcessed);
    ASSERT(lastEnqueuedDocuments.empty());
}

TEST_F(OplogFetcherTest, MetadataAndBatchAreProcessedWhenSyncSourceIsCurrentButMetadataIsStale) {
    // This tests the case where the sync source metadata is behind us but we get a document which
    // is equal to us.  Since that means the metadata is stale and can be ignored, we should accept
    // this sync source.
    CursorId cursorId = 0LL;
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, staleOqMetadata);
    auto entry = makeNoopOplogEntry(lastFetched);

    ASSERT_OK(processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj),
                                 false /* shouldShutdown */,
                                 false /* requireFresherSyncSource */)
                  ->getStatus());

    ASSERT(dataReplicatorExternalState->metadataWasProcessed);
}

TEST_F(OplogFetcherTest,
       MetadataAndBatchAreProcessedWhenSyncSourceIsNotAheadWithoutRequiringFresherSyncSource) {
    CursorId cursorId = 0LL;
    rpc::OplogQueryMetadata oplogQueryMetadata(
        {staleOpTime, staleWallTime}, lastFetched, rbid, 2, 2);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oplogQueryMetadata);
    auto entry = makeNoopOplogEntry(lastFetched);

    ASSERT_OK(processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj),
                                 false /* shouldShutdown */,
                                 false /* requireFresherSyncSource */)
                  ->getStatus());
    ASSERT(dataReplicatorExternalState->metadataWasProcessed);
}

TEST_F(OplogFetcherTest, MetadataIsNotProcessedOnBatchThatTriggersRollback) {
    CursorId cursorId = 22LL;
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto entry = makeNoopOplogEntry(Seconds(456));

    ASSERT_EQUALS(
        ErrorCodes::OplogStartMissing,
        processSingleBatch(makeFirstBatch(cursorId, {entry}, {metadataObj}))->getStatus());
    ASSERT_FALSE(dataReplicatorExternalState->metadataWasProcessed);
}

TEST_F(OplogFetcherTest, FailingInitialCreateNewCursorNoRetriesShutsDownOplogFetcher) {
    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource, processSingleBatch(Message())->getStatus());
}

TEST_F(OplogFetcherTest, FailingInitialCreateNewCursorWithRetriesShutsDownOplogFetcher) {
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    // An empty message will cause the initial attempt to create a cursor to fail.
    processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(), Message(), true);

    // An empty message will cause the attempt to recreate a cursor to fail.
    processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(), Message());

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest,
       NetworkExceptionDuringInitialCreateNewCursorWithRetriesShutsDownOplogFetcher) {
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    // This will cause the initial attempt to create a cursor to fail.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"},
        true);

    // This will cause the attempt to recreate a cursor to fail.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"});

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::NetworkTimeout, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, DontRecreateNewCursorAfterFailedBatchNoRetries) {
    ShutdownState shutdownState;

    // Create an oplog fetcher without any retries.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto firstBatch = {firstEntry, secondEntry};

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Creating the cursor will succeed.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, firstBatch, metadataObj),
                                          true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Check that the first batch was successfully processed.
    validateLastBatch(
        true /* skipFirstDoc */, firstBatch, oplogFetcher->getLastOpTimeFetched_forTest());

    // This will cause the oplog fetcher to fail while getting the next batch. Since it doesn't have
    // any retries, the oplog fetcher will shut down.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"});

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::NetworkTimeout, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, FailCreateNewCursorAfterFailedBatchRetriesShutsDownOplogFetcher) {
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto firstBatch = {firstEntry, secondEntry};

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Creating the cursor will succeed.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, firstBatch, metadataObj),
                                          true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Check that the first batch was successfully processed.
    validateLastBatch(
        true /* skipFirstDoc */, firstBatch, oplogFetcher->getLastOpTimeFetched_forTest());

    // This will cause us to fail getting the next batch, meaning a new cursor needs to be created.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"},
        true);

    // An empty message will cause the attempt to create a cursor to fail.
    processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(), Message());

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::InvalidSyncSource, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, SuccessfullyRecreateCursorAfterFailedBatch) {
    // This tests that the oplog fetcher successfully can recreate a cursor after it failed to get
    // a batch and makes sure the recreated cursor behaves like an exhaust cursor. This will also
    // check that the socket timeouts are set as expected. The steps are:
    // 1. Start the oplog fetcher.
    // 2. Create the initial cursor successfully.
    // 3. Fail getting the next batch, causing us to create a new cursor.
    // 4. Succeed creating a new cursor.
    // 5. Successfully get the next batch (from the first getMore command).
    // 6. Successfully get the next batch (this is the first exhaust batch).
    // 7. Shut down while the connection is blocked on the network.

    ShutdownState shutdownState;

    // -- Step 1 --
    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    // Make sure we are blocked on the network before checking that the socket timeout is properly
    // set.
    auto conn = oplogFetcher->getDBClientConnection_forTest();
    auto* mockConn = dynamic_cast<MockDBClientConnection*>(conn);
    ASSERT_TRUE(blockedOnNetworkSoon(mockConn));

    auto initialMaxFindTimeDouble =
        durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()) / 1000.0;
    auto retriedMaxFindTimeDouble =
        durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()) / 1000.0;
    auto awaitDataTimeoutDouble =
        durationCount<Milliseconds>(makeOplogFetcher()->getAwaitDataTimeout_forTest()) / 1000.0;

    // Check the socket timeout is equal to the initial find max time plus the network buffer.
    ASSERT_EQUALS(initialMaxFindTimeDouble + oplogNetworkTimeoutBufferSeconds.load(),
                  conn->getSoTimeout());

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(124), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto firstBatch = {firstEntry, secondEntry};

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // -- Step 2 --
    // Creating the cursor will succeed. After this, the cursor will be blocked on call() for the
    // getMore command.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, firstBatch, metadataObj),
                                          true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Check the socket timeout is equal to the awaitData timeout plus the network buffer.
    ASSERT_EQUALS(awaitDataTimeoutDouble + oplogNetworkTimeoutBufferSeconds.load(),
                  conn->getSoTimeout());

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the first batch was successfully processed.
    validateLastBatch(true /* skipFirstDoc */, firstBatch, lastFetched);

    // -- Step 3 --
    // This will cause us to fail getting the next batch, meaning a new cursor needs to be created.
    // After this, the new cursor will be blocked on call() while trying to initialize.
    m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"},
        true);

    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime());

    // Check the socket timeout is equal to the retried find max time plus the network buffer.
    ASSERT_EQUALS(retriedMaxFindTimeDouble + oplogNetworkTimeoutBufferSeconds.load(),
                  conn->getSoTimeout());

    cursorId = 23LL;
    auto thirdEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto secondBatch = {secondEntry, thirdEntry};

    // -- Step 4 --
    // This will cause the attempt to create a cursor to succeed. After this, the cursor will be
    // blocked on call() for the getMore command.
    m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                     makeFirstBatch(cursorId, secondBatch, metadataObj),
                                     true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    // Check the socket timeout is equal to the awaitData timeout plus the network buffer.
    ASSERT_EQUALS(awaitDataTimeoutDouble + oplogNetworkTimeoutBufferSeconds.load(),
                  conn->getSoTimeout());

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the first batch with the new cursor was successfully processed.
    validateLastBatch(true /* skipFirstDoc */, secondBatch, lastFetched);

    auto fourthEntry = makeNoopOplogEntry({{Seconds(457), 0}, lastFetched.getTerm()});
    auto fifthEntry = makeNoopOplogEntry({{Seconds(458), 0}, lastFetched.getTerm()});
    auto thirdBatch = {fourthEntry, fifthEntry};

    // -- Step 5 --
    // This will be the first getMore. After this, the cursor will be blocked on recv() for the next
    // batch.
    m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeSubsequentBatch(cursorId, thirdBatch, metadataObj, true /* moreToCome */),
        true);

    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime());

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the next batch was successfully processed.
    validateLastBatch(false /* skipFirstDoc */, thirdBatch, lastFetched);

    auto sixthEntry = makeNoopOplogEntry({{Seconds(789), 0}, lastFetched.getTerm()});
    auto seventhEntry = makeNoopOplogEntry({{Seconds(790), 0}, lastFetched.getTerm()});
    auto fourthBatch = {sixthEntry, seventhEntry};

    // -- Step 6 --
    // Getting this batch will mean the cursor was successfully recreated as an exhaust cursor. The
    // moreToCome flag is set to false so that _connectionHasPendingReplies on the cursor will be
    // false when cleaning up the cursor (otherwise we'd need to use a side connection to clean up
    // the cursor).
    processSingleExhaustResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeSubsequentBatch(cursorId, fourthBatch, metadataObj, false /* moreToCome */),
        true);

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the next batch was successfully processed.
    validateLastBatch(false /* skipFirstDoc */, fourthBatch, lastFetched);

    // -- Step 7 --
    oplogFetcher->shutdown();
    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, SuccessfulBatchResetsNumRestarts) {
    // This tests that the OplogFetcherRestartDecision resets its counter when the oplog fetcher
    // successfully gets the next batch. The steps are:
    // 1. Start the oplog fetcher.
    // 2. Fail to create the initial cursor. This will increment the number of failed restarts.
    // 3. Create the cursor successfully. This should reset the count of failed restarts.
    // 4. Fail getting the next batch, causing us to create a new cursor.
    // 5. Succeed creating a new cursor.
    // 6. Shut down while the connection is blocked on the network.

    ShutdownState shutdownState;

    // -- Step 1 --
    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    // -- Step 2 --
    // This will cause the first attempt to create a cursor to fail. After this, the new cursor will
    // be blocked on call() while trying to initialize.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"},
        true);

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto firstBatch = {firstEntry, secondEntry};

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // -- Step 3 --
    // Creating the cursor will succeed. After this, the cursor will be blocked on call() for the
    // getMore command.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, firstBatch, metadataObj),
                                          true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the first batch was successfully processed.
    validateLastBatch(true /* skipFirstDoc */, firstBatch, lastFetched);

    // -- Step 4 --
    // This will cause an error when getting the next batch, which will cause us to recreate the
    // cursor. If the number of retries was not reset, this will fail because the new cursor won't
    // be blocked on the call while recreating the cursor.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        mongo::Status{mongo::ErrorCodes::NetworkTimeout, "Fake socket timeout"},
        true);

    cursorId = 23LL;
    auto thirdEntry = makeNoopOplogEntry({{Seconds(789), 0}, lastFetched.getTerm()});
    auto secondBatch = {secondEntry, thirdEntry};

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // -- Step 5 --
    // Make sure that we can finish recreating the cursor successfully.
    m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                     makeFirstBatch(cursorId, secondBatch, metadataObj),
                                     true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the first batch from the new cursor was successfully processed.
    validateLastBatch(true /* skipFirstDoc */, secondBatch, lastFetched);

    oplogFetcher->shutdown();
    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherWorksWithoutExhaust) {
    // Test that the oplog fetcher works if the 'oplogFetcherUsesExhaust' server parameter is set to
    // false.

    ShutdownState shutdownState;

    oplogFetcherUsesExhaust = false;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto firstBatch = {firstEntry, secondEntry};

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Creating the cursor will succeed. After this, the cursor will be blocked on call() for the
    // getMore command.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, firstBatch, metadataObj),
                                          true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the first batch was successfully processed.
    validateLastBatch(true /* skipFirstDoc */, firstBatch, lastFetched);

    auto thirdEntry = makeNoopOplogEntry({{Seconds(457), 0}, lastFetched.getTerm()});
    auto fourthEntry = makeNoopOplogEntry({{Seconds(458), 0}, lastFetched.getTerm()});
    auto secondBatch = {thirdEntry, fourthEntry};

    // moreToCome would be set to false if oplogFetcherUsesExhaust was set to false. After this,
    // the cursor will be blocked on call() for the next getMore command.
    m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeSubsequentBatch(cursorId, secondBatch, metadataObj, false /* moreToCome */),
        true);

    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime(),
                           false /* exhaustSupported */);

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the next batch was successfully processed.
    validateLastBatch(false /* skipFirstDoc */, secondBatch, lastFetched);

    auto fifthEntry = makeNoopOplogEntry({{Seconds(789), 0}, lastFetched.getTerm()});
    auto sixthEntry = makeNoopOplogEntry({{Seconds(790), 0}, lastFetched.getTerm()});
    auto thirdBatch = {fifthEntry, sixthEntry};

    m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                     makeSubsequentBatch(cursorId, thirdBatch, metadataObj, false),
                                     true);

    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime(),
                           false /* exhaustSupported */);

    // Update lastFetched since it should have been updated after getting the last batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Check that the next batch was successfully processed.
    validateLastBatch(false /* skipFirstDoc */, thirdBatch, lastFetched);

    oplogFetcher->shutdown();
    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, CursorIsDeadShutsDownOplogFetcherWithSuccessfulStatus) {
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    CursorId cursorId = 0LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(124), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    auto firstBatch = {firstEntry, secondEntry};

    // Creating the cursor will succeed, but the oplog fetcher will shut down after receiving this
    // batch because the cursor id is 0.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, firstBatch, metadataObj));

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Check that the oplog fetcher has shut down to make sure it has processed the next batch
    // before verifying the batch's contents.
    oplogFetcher->join();

    // Check that the next batch was successfully processed.
    validateLastBatch(
        true /* skipFirstDoc */, firstBatch, oplogFetcher->getLastOpTimeFetched_forTest());

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, EmptyFirstBatchStopsOplogFetcherWithOplogStartMissingError) {
    CursorId cursorId = 22LL;
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    ASSERT_EQUALS(ErrorCodes::OplogStartMissing,
                  processSingleBatch(makeFirstBatch(cursorId, {}, {metadataObj}))->getStatus());
}

TEST_F(OplogFetcherTest, MissingOpTimeInFirstDocumentCausesOplogFetcherToStopWithInvalidBSONError) {
    CursorId cursorId = 22LL;
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    ASSERT_EQUALS(
        ErrorCodes::InvalidBSON,
        processSingleBatch(makeFirstBatch(cursorId, {BSONObj()}, metadataObj))->getStatus());
}

TEST_F(
    OplogFetcherTest,
    LastOpTimeFetchedDoesNotMatchFirstDocumentCausesOplogFetcherToStopWithOplogStartMissingError) {
    CursorId cursorId = 22LL;
    auto entry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    ASSERT_EQUALS(ErrorCodes::OplogStartMissing,
                  processSingleBatch(makeFirstBatch(cursorId, {entry}, metadataObj))->getStatus());
}

TEST_F(OplogFetcherTest,
       MissingOpTimeInSecondDocumentOfFirstBatchCausesOplogFetcherToStopWithNoSuchKey) {
    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    ASSERT_EQUALS(
        ErrorCodes::NoSuchKey,
        processSingleBatch(makeFirstBatch(cursorId,
                                          {firstEntry,
                                           BSON("o" << BSON("msg"
                                                            << "oplog entry without optime"))},
                                          metadataObj))
            ->getStatus());
}

TEST_F(OplogFetcherTest, TimestampsNotAdvancingInBatchCausesOplogFetcherStopWithOplogOutOfOrder) {
    CursorId cursorId = 22LL;
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);
    ASSERT_EQUALS(ErrorCodes::OplogOutOfOrder,
                  processSingleBatch(makeFirstBatch(cursorId,
                                                    {makeNoopOplogEntry(lastFetched),
                                                     makeNoopOplogEntry(Seconds(1000)),
                                                     makeNoopOplogEntry(Seconds(2000)),
                                                     makeNoopOplogEntry(Seconds(1500))},
                                                    metadataObj))
                      ->getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherShouldExcludeFirstDocumentInFirstBatchWhenEnqueuingDocuments) {
    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto thirdEntry = makeNoopOplogEntry({{Seconds(789), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    auto shutdownState = processSingleBatch(
        makeFirstBatch(cursorId, {firstEntry, secondEntry, thirdEntry}, metadataObj),
        true /* shouldShutdown */,
        true /* requireFresherSyncSource */,
        true /* lastFetchedShouldAdvance */);

    ASSERT_EQUALS(2U, lastEnqueuedDocuments.size());
    ASSERT_BSONOBJ_EQ(secondEntry, lastEnqueuedDocuments[0]);
    ASSERT_BSONOBJ_EQ(thirdEntry, lastEnqueuedDocuments[1]);

    ASSERT_EQUALS(3U, lastEnqueuedDocumentsInfo.networkDocumentCount);
    ASSERT_EQUALS(size_t(firstEntry.objsize() + secondEntry.objsize() + thirdEntry.objsize()),
                  lastEnqueuedDocumentsInfo.networkDocumentBytes);

    ASSERT_EQUALS(2U, lastEnqueuedDocumentsInfo.toApplyDocumentCount);
    ASSERT_EQUALS(size_t(secondEntry.objsize() + thirdEntry.objsize()),
                  lastEnqueuedDocumentsInfo.toApplyDocumentBytes);

    ASSERT_EQUALS(unittest::assertGet(OpTime::parseFromOplogEntry(thirdEntry)),
                  lastEnqueuedDocumentsInfo.lastDocument);

    ASSERT_EQUALS(ErrorCodes::HostUnreachable, shutdownState->getStatus());
}

TEST_F(OplogFetcherTest,
       OplogFetcherShouldNotDuplicateFirstDocWithEnqueueFirstDocOnErrorAfterFirstDoc) {

    // This function verifies that every oplog entry is only enqueued once.
    OpTime lastEnqueuedOpTime = OpTime();
    enqueueDocumentsFn = [&lastEnqueuedOpTime](OplogFetcher::Documents::const_iterator begin,
                                               OplogFetcher::Documents::const_iterator end,
                                               const OplogFetcher::DocumentsInfo&) -> Status {
        auto count = 0;
        auto toEnqueueOpTime = OpTime();

        for (auto i = begin; i != end; ++i) {
            count++;

            toEnqueueOpTime = OplogEntry(*i).getOpTime();
            ASSERT_GREATER_THAN(toEnqueueOpTime, lastEnqueuedOpTime);
            lastEnqueuedOpTime = toEnqueueOpTime;
        }

        ASSERT_EQ(1, count);
        return Status::OK();
    };

    auto shutdownState = std::make_unique<ShutdownState>();

    // Create an oplog fetcher with one retry.
    auto oplogFetcher =
        getOplogFetcherAfterConnectionCreated(std::ref(*shutdownState),
                                              1,
                                              true /* requireFresherSyncSource */,
                                              OplogFetcher::StartingPoint::kEnqueueFirstDoc);

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry({{Seconds(123), 0}, lastFetched.getTerm()});
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Creating the cursor will succeed. Only send over the first entry. Save the second for the
    // getMore request.
    auto m = processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                          makeFirstBatch(cursorId, {firstEntry}, metadataObj),
                                          true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Simulate an error right before receiving the second entry. This will cause an error when
    // getting the next batch, which will cause the oplog fetcher to recreate the cursor.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        Status{ErrorCodes::QueryPlanKilled, "Simulating failure for test."},
        true);

    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Resend all data for the retry. The enqueueDocumentsFn will check to make sure that
    // the first entry was not enqueued twice.
    m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeFirstBatch(cursorId, {firstEntry, secondEntry}, metadataObj),
        true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    oplogFetcher->shutdown();
    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState->getStatus());
}

TEST_F(OplogFetcherTest,
       OplogFetcherShouldNotDuplicateFirstDocWithEnqueueFirstDocOnErrorAfterSecondDoc) {

    // This function verifies that every oplog entry is only enqueued once.
    OpTime lastEnqueuedOpTime = OpTime();
    enqueueDocumentsFn = [&lastEnqueuedOpTime](OplogFetcher::Documents::const_iterator begin,
                                               OplogFetcher::Documents::const_iterator end,
                                               const OplogFetcher::DocumentsInfo&) -> Status {
        auto count = 0;
        auto toEnqueueOpTime = OpTime();

        for (auto i = begin; i != end; ++i) {
            count++;

            toEnqueueOpTime = OplogEntry(*i).getOpTime();
            ASSERT_GREATER_THAN(toEnqueueOpTime, lastEnqueuedOpTime);
            lastEnqueuedOpTime = toEnqueueOpTime;
        }

        ASSERT_NOT_GREATER_THAN(count, 2);
        return Status::OK();
    };

    auto shutdownState = std::make_unique<ShutdownState>();

    // Create an oplog fetcher with one retry.
    auto oplogFetcher =
        getOplogFetcherAfterConnectionCreated(std::ref(*shutdownState),
                                              1,
                                              true /* requireFresherSyncSource */,
                                              OplogFetcher::StartingPoint::kEnqueueFirstDoc);

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry({{Seconds(123), 0}, lastFetched.getTerm()});
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto thirdEntry = makeNoopOplogEntry({{Seconds(789), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    // Update lastFetched before it is updated by getting the next batch.
    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Creating the cursor will succeed.
    auto m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeFirstBatch(cursorId, {firstEntry, secondEntry}, metadataObj),
        true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getInitialFindMaxTime_forTest()));

    // Simulate an error. This will cause an error when getting the next batch, which will cause the
    // oplog fetcher to recreate the cursor.
    processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        Status{ErrorCodes::QueryPlanKilled, "Simulating failure for test."},
        true);

    lastFetched = oplogFetcher->getLastOpTimeFetched_forTest();

    // Resend the second entry for the retry. The enqueueDocumentsFn will check to make sure that
    // the second entry was not enqueued twice.
    m = processSingleRequestResponse(
        oplogFetcher->getDBClientConnection_forTest(),
        makeFirstBatch(cursorId, {secondEntry, thirdEntry}, metadataObj),
        true);

    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    oplogFetcher->shutdown();
    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState->getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherShouldReportErrorsThrownFromEnqueueDocumentsFn) {
    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(456), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    enqueueDocumentsFn = [](OplogFetcher::Documents::const_iterator,
                            OplogFetcher::Documents::const_iterator,
                            const OplogFetcher::DocumentsInfo&) -> Status {
        return Status(ErrorCodes::InternalError, "my custom error");
    };

    auto shutdownState =
        processSingleBatch(makeFirstBatch(cursorId, {firstEntry, secondEntry}, metadataObj));
    ASSERT_EQ(Status(ErrorCodes::InternalError, "my custom error"), shutdownState->getStatus());
}

TEST_F(OplogFetcherTest, FailedSyncSourceCheckWithBothMetadatasStopsTheOplogFetcher) {
    testSyncSourceChecking(replSetMetadata, oqMetadata);

    // Sync source optime and "hasSyncSource" can be set if the respone contains metadata.
    ASSERT_EQUALS(source, dataReplicatorExternalState->lastSyncSourceChecked);
    ASSERT_EQUALS(oqMetadata.getLastOpApplied(), dataReplicatorExternalState->syncSourceLastOpTime);
    ASSERT_TRUE(dataReplicatorExternalState->syncSourceHasSyncSource);
}

TEST_F(OplogFetcherTest,
       FailedSyncSourceCheckWithSyncSourceHavingNoSyncSourceStopsTheOplogFetcher) {
    rpc::OplogQueryMetadata oplogQueryMetadata(
        {staleOpTime, staleWallTime}, remoteNewerOpTime, rbid, primaryIndex, -1);
    testSyncSourceChecking(replSetMetadata, oplogQueryMetadata);

    // Sync source "hasSyncSource" is derived from metadata.
    ASSERT_EQUALS(source, dataReplicatorExternalState->lastSyncSourceChecked);
    ASSERT_EQUALS(oplogQueryMetadata.getLastOpApplied(),
                  dataReplicatorExternalState->syncSourceLastOpTime);
    ASSERT_FALSE(dataReplicatorExternalState->syncSourceHasSyncSource);
}

TEST_F(OplogFetcherTest, ValidateDocumentsReturnsNoSuchKeyIfTimestampIsNotFoundInAnyDocument) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));
    auto secondEntry = BSON("o" << BSON("msg"
                                        << "oplog entry without optime"));

    ASSERT_EQUALS(ErrorCodes::NoSuchKey,
                  OplogFetcher::validateDocuments(
                      {firstEntry, secondEntry},
                      true,
                      unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp())
                      .getStatus());
}

TEST_F(
    OplogFetcherTest,
    ValidateDocumentsReturnsOutOfOrderIfTimestampInFirstEntryIsEqualToLastTimestampAndNotProcessingFirstBatch) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));
    auto secondEntry = makeNoopOplogEntry(Seconds(456));

    ASSERT_EQUALS(ErrorCodes::OplogOutOfOrder,
                  OplogFetcher::validateDocuments(
                      {firstEntry, secondEntry},
                      false,
                      unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp())
                      .getStatus());
}

TEST_F(OplogFetcherTest, ValidateDocumentsReturnsOutOfOrderIfTimestampInSecondEntryIsBeforeFirst) {
    auto firstEntry = makeNoopOplogEntry(Seconds(456));
    auto secondEntry = makeNoopOplogEntry(Seconds(123));

    ASSERT_EQUALS(ErrorCodes::OplogOutOfOrder,
                  OplogFetcher::validateDocuments(
                      {firstEntry, secondEntry},
                      true,
                      unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp())
                      .getStatus());
}

TEST_F(OplogFetcherTest, ValidateDocumentsReturnsOutOfOrderIfTimestampInThirdEntryIsBeforeSecond) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));
    auto secondEntry = makeNoopOplogEntry(Seconds(789));
    auto thirdEntry = makeNoopOplogEntry(Seconds(456));

    ASSERT_EQUALS(ErrorCodes::OplogOutOfOrder,
                  OplogFetcher::validateDocuments(
                      {firstEntry, secondEntry, thirdEntry},
                      true,
                      unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp())
                      .getStatus());
}

TEST_F(
    OplogFetcherTest,
    ValidateDocumentsExcludesFirstDocumentInApplyCountAndBytesIfProcessingFirstBatchAndSkipFirstDoc) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));
    auto secondEntry = makeNoopOplogEntry(Seconds(456));
    auto thirdEntry = makeNoopOplogEntry(Seconds(789));

    auto info = unittest::assertGet(OplogFetcher::validateDocuments(
        {firstEntry, secondEntry, thirdEntry},
        true,
        unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp(),
        mongo::repl::OplogFetcher::StartingPoint::kSkipFirstDoc));

    ASSERT_EQUALS(3U, info.networkDocumentCount);
    ASSERT_EQUALS(2U, info.toApplyDocumentCount);
    ASSERT_EQUALS(size_t(firstEntry.objsize() + secondEntry.objsize() + thirdEntry.objsize()),
                  info.networkDocumentBytes);
    ASSERT_EQUALS(size_t(secondEntry.objsize() + thirdEntry.objsize()), info.toApplyDocumentBytes);

    ASSERT_EQUALS(unittest::assertGet(OpTime::parseFromOplogEntry(thirdEntry)), info.lastDocument);
}

TEST_F(
    OplogFetcherTest,
    ValidateDocumentsIncludesFirstDocumentInApplyCountAndBytesIfProcessingFirstBatchAndEnqueueFirstDoc) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));
    auto secondEntry = makeNoopOplogEntry(Seconds(456));
    auto thirdEntry = makeNoopOplogEntry(Seconds(789));

    auto info = unittest::assertGet(OplogFetcher::validateDocuments(
        {firstEntry, secondEntry, thirdEntry},
        true,
        unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp(),
        mongo::repl::OplogFetcher::StartingPoint::kEnqueueFirstDoc));

    ASSERT_EQUALS(3U, info.networkDocumentCount);
    ASSERT_EQUALS(3U, info.toApplyDocumentCount);
    ASSERT_EQUALS(size_t(firstEntry.objsize() + secondEntry.objsize() + thirdEntry.objsize()),
                  info.networkDocumentBytes);
    ASSERT_EQUALS(size_t(firstEntry.objsize() + secondEntry.objsize() + thirdEntry.objsize()),
                  info.toApplyDocumentBytes);

    ASSERT_EQUALS(unittest::assertGet(OpTime::parseFromOplogEntry(thirdEntry)), info.lastDocument);
}

TEST_F(OplogFetcherTest,
       ValidateDocumentsIncludesFirstDocumentInApplyCountAndBytesIfNotProcessingFirstBatch) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));
    auto secondEntry = makeNoopOplogEntry(Seconds(456));
    auto thirdEntry = makeNoopOplogEntry(Seconds(789));

    auto info = unittest::assertGet(OplogFetcher::validateDocuments(
        {firstEntry, secondEntry, thirdEntry}, false, Timestamp(Seconds(100), 0)));

    ASSERT_EQUALS(3U, info.networkDocumentCount);
    ASSERT_EQUALS(size_t(firstEntry.objsize() + secondEntry.objsize() + thirdEntry.objsize()),
                  info.networkDocumentBytes);

    ASSERT_EQUALS(info.networkDocumentCount, info.toApplyDocumentCount);
    ASSERT_EQUALS(info.networkDocumentBytes, info.toApplyDocumentBytes);

    ASSERT_EQUALS(unittest::assertGet(OpTime::parseFromOplogEntry(thirdEntry)), info.lastDocument);
}

TEST_F(OplogFetcherTest,
       ValidateDocumentsReturnsDefaultLastDocumentOpTimeWhenThereAreNoDocumentsToApply) {
    auto firstEntry = makeNoopOplogEntry(Seconds(123));

    auto info = unittest::assertGet(OplogFetcher::validateDocuments(
        {firstEntry},
        true,
        unittest::assertGet(OpTime::parseFromOplogEntry(firstEntry)).getTimestamp()));

    ASSERT_EQUALS(1U, info.networkDocumentCount);
    ASSERT_EQUALS(size_t(firstEntry.objsize()), info.networkDocumentBytes);

    ASSERT_EQUALS(0U, info.toApplyDocumentCount);
    ASSERT_EQUALS(0U, info.toApplyDocumentBytes);

    ASSERT_EQUALS(OpTime(), info.lastDocument);
}

TEST_F(OplogFetcherTest,
       ValidateDocumentsReturnsOplogStartMissingWhenThereAreNoDocumentsWhenProcessingFirstBatch) {
    ASSERT_EQUALS(
        ErrorCodes::OplogStartMissing,
        OplogFetcher::validateDocuments({}, true, Timestamp(Seconds(100), 0)).getStatus());
}

TEST_F(OplogFetcherTest,
       ValidateDocumentsReturnsDefaultInfoWhenThereAreNoDocumentsWhenNotProcessingFirstBatch) {
    auto info =
        unittest::assertGet(OplogFetcher::validateDocuments({}, false, Timestamp(Seconds(100), 0)));

    ASSERT_EQUALS(0U, info.networkDocumentCount);
    ASSERT_EQUALS(0U, info.networkDocumentBytes);

    ASSERT_EQUALS(0U, info.toApplyDocumentCount);
    ASSERT_EQUALS(0U, info.toApplyDocumentBytes);

    ASSERT_EQUALS(OpTime(), info.lastDocument);
}

TEST_F(OplogFetcherTest, OplogFetcherReturnsHostUnreachableOnConnectionFailures) {
    // Test that OplogFetcher fails to establish initial connection, retrying HostUnreachable.
    ShutdownState shutdownState;

    // Shutdown the mock remote server before the OplogFetcher tries to connect.
    _mockServer->shutdown();

    // This will also ensure that _runQuery was scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    oplogFetcher->join();

    // This is the error code for connection failures.
    ASSERT_EQUALS(ErrorCodes::HostUnreachable, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherRetriesConnectionButFails) {
    // Test that OplogFetcher tries but fails after failing the initial connection, retrying
    // HostUnreachable.
    ShutdownState shutdownState;

    // Shutdown the mock remote server before the OplogFetcher tries to connect.
    _mockServer->shutdown();

    // Create an OplogFetcher with 1 retry attempt. This will also ensure that _runQuery was
    // scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    oplogFetcher->join();

    // This is the error code for connection failures.
    ASSERT_EQUALS(ErrorCodes::HostUnreachable, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherReturnsCallbackCanceledIfShutdownBeforeReconnect) {
    // Test that OplogFetcher returns CallbackCanceled error if it is shut down after failing the
    // initial connection but before it retries the connection.
    ShutdownState shutdownState;

    // Shutdown the mock remote server before the OplogFetcher tries to connect.
    _mockServer->shutdown();

    // Hang OplogFetcher before it retries the connection.
    auto beforeRetryingConnection = globalFailPointRegistry().find("hangBeforeOplogFetcherRetries");
    auto timesEntered = beforeRetryingConnection->setMode(FailPoint::alwaysOn);

    // Create an OplogFetcher with 1 retry attempt. This will also ensure that _runQuery was
    // scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    // Wait until the first connect attempt fails but before it retries.
    beforeRetryingConnection->waitForTimesEntered(timesEntered + 1);

    // Shut down the OplogFetcher.
    oplogFetcher->shutdown();

    // Disable the failpoint to allow reconnection.
    beforeRetryingConnection->setMode(FailPoint::off);

    oplogFetcher->join();

    ASSERT_EQUALS(ErrorCodes::CallbackCanceled, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherResetsNumRestartsOnSuccessfulConnection) {
    // Test that OplogFetcher resets the number of restarts after a successful connection on a
    // retry.
    ShutdownState shutdownState;

    // Shutdown the mock remote server before the OplogFetcher tries to connect.
    _mockServer->shutdown();

    // Hang OplogFetcher before it retries the connection.
    auto beforeRetryingConnection = globalFailPointRegistry().find("hangBeforeOplogFetcherRetries");
    auto timesEntered = beforeRetryingConnection->setMode(FailPoint::alwaysOn);

    // Create an OplogFetcher with 1 retry attempt. This will also ensure that _runQuery was
    // scheduled before returning.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    // Wait until the first connect attempt fails but before it retries.
    beforeRetryingConnection->waitForTimesEntered(timesEntered + 1);

    // Bring up the mock server so that the connection retry can succeed.
    _mockServer->reboot();

    // Disable the failpoint to allow reconnection.
    beforeRetryingConnection->setMode(FailPoint::off);

    // After the connection succeeded, the number of restarts should be reset back to 0 so that the
    // OplogFetcher can tolerate another failure before failing. This will cause the first attempt
    // to create a cursor to fail. After this, the new cursor will be blocked on call() while
    // retrying to initialize. This also makes sure the OplogFetcher reconnects correctly.
    simulateNetworkDisconnect(oplogFetcher->getDBClientConnection_forTest());

    CursorId cursorId = 0LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    // Allow the cursor re-initialization to succeed. But the OplogFetcher will shut down with an OK
    // status after receiving this batch because the cursor id is 0.
    processSingleRequestResponse(oplogFetcher->getDBClientConnection_forTest(),
                                 makeFirstBatch(cursorId, {firstEntry}, metadataObj));

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherCanAutoReconnect) {
    // Test that the OplogFetcher can autoreconnect after a broken connection.
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    auto conn = oplogFetcher->getDBClientConnection_forTest();
    // Simulate a disconnect for the first find command.
    simulateNetworkDisconnect(conn);

    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(124), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    // Simulate closing the cursor and the OplogFetcher should exit with an OK status.
    processSingleRequestResponse(conn, makeFirstBatch(0LL, {firstEntry}, metadataObj));

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, OplogFetcherAutoReconnectsButFails) {
    // Test that the OplogFetcher fails an autoreconnect after a broken connection.
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    auto conn = oplogFetcher->getDBClientConnection_forTest();

    auto beforeRecreatingCursor = globalFailPointRegistry().find("hangBeforeOplogFetcherRetries");
    auto timesEntered = beforeRecreatingCursor->setMode(FailPoint::alwaysOn);

    // Simulate a disconnect for the first find command. And the OplogFetcher should retry with
    // AutoReconnect.
    simulateNetworkDisconnect(conn);

    // Shut down the mock server before OplogFetcher reconnects.
    beforeRecreatingCursor->waitForTimesEntered(timesEntered + 1);
    _mockServer->shutdown();

    // Allow retry and autoreconnect.
    beforeRecreatingCursor->setMode(FailPoint::off);

    oplogFetcher->join();

    // AutoReconnect should also fail because the server is shut down.
    ASSERT_EQUALS(ErrorCodes::HostUnreachable, shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, DisconnectsOnErrorsDuringExhaustStream) {
    // Test that the connection disconnects if we get errors after successfully receiving a batch
    // from the exhaust stream.
    ShutdownState shutdownState;

    // Create an oplog fetcher with one retry.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState), 1);

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto secondEntry = makeNoopOplogEntry({{Seconds(124), 0}, lastFetched.getTerm()});
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    auto conn = oplogFetcher->getDBClientConnection_forTest();
    // First batch for the initial find command.
    processSingleRequestResponse(conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);

    auto beforeRecreatingCursor = globalFailPointRegistry().find("hangBeforeOplogFetcherRetries");
    auto timesEntered = beforeRecreatingCursor->setMode(FailPoint::alwaysOn);

    // Temporarily override the metatdata reader to introduce failure after successfully receiving a
    // batch from the first getMore. And the exhaust stream is now established.
    conn->setReplyMetadataReader(
        [&](OperationContext* opCtx, const BSONObj& metadataObj, StringData target) {
            return Status(ErrorCodes::FailedToParse, "Fake error");
        });
    processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {secondEntry}, metadataObj, true /* moreToCome */));

    beforeRecreatingCursor->waitForTimesEntered(timesEntered + 1);

    // Test that the connection is disconnected because we cannot use the same connection to
    // recreate cursor as more data is on the way from the server for the exhaust stream.
    ASSERT_TRUE(conn->isFailed());

    // Unset the metatdata reader.
    conn->setReplyMetadataReader(rpc::ReplyMetadataReader());

    // Allow retry and autoreconnect.
    beforeRecreatingCursor->setMode(FailPoint::off);

    // Simulate closing the cursor and the OplogFetcher should exit with an OK status.
    processSingleRequestResponse(conn, makeFirstBatch(0LL, {firstEntry}, metadataObj));

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, GetMoreEmptyBatch) {
    ShutdownState shutdownState;

    // Create an oplog fetcher without any retries.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    auto conn = oplogFetcher->getDBClientConnection_forTest();

    // Creating the cursor will succeed.
    auto m = processSingleRequestResponse(
        conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);

    // Empty batch from first getMore.
    processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, true /* moreToCome */), true);

    // Terminating empty batch from exhaust stream with cursorId 0.
    processSingleExhaustResponse(oplogFetcher->getDBClientConnection_forTest(),
                                 makeSubsequentBatch(0LL, {}, metadataObj, false /* moreToCome */),
                                 false);

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, HandleLogicalTimeMetaDataAndAdvanceClusterTime) {
    auto firstEntry = makeNoopOplogEntry(lastFetched);

    auto oldClusterTime = LogicalClock::get(getGlobalServiceContext())->getClusterTime();

    auto logicalTime = LogicalTime(Timestamp(123456, 78));
    auto logicalTimeMetadata =
        rpc::LogicalTimeMetadata(SignedLogicalTime(logicalTime, TimeProofService::TimeProof(), 0));

    BSONObjBuilder bob;
    ASSERT_OK(replSetMetadata.writeToMetadata(&bob));
    ASSERT_OK(oqMetadata.writeToMetadata(&bob));
    logicalTimeMetadata.writeToMetadata(&bob);
    auto metadataObj = bob.obj();

    // Process one batch with the logical time metadata.
    ASSERT_OK(processSingleBatch(makeFirstBatch(0LL, {firstEntry}, metadataObj))->getStatus());

    // Test that the cluster time is updated to the cluster time in the metadata.
    auto currentClusterTime = LogicalClock::get(getGlobalServiceContext())->getClusterTime();
    ASSERT_EQ(currentClusterTime, logicalTime);
    ASSERT_NE(oldClusterTime, logicalTime);
}

TEST_F(OplogFetcherTest, DowngradeFrom44To42) {
    ShutdownState shutdownState;

    // Start with FCV 4.4.
    serverGlobalParams.featureCompatibility.setVersion(
        ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44);

    // Create an oplog fetcher without any retries, retrying FCV changes does not count towards the
    // max retries.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    auto conn = oplogFetcher->getDBClientConnection_forTest();

    // Creating the cursor will succeed.
    processSingleRequestResponse(conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);

    // Empty batch from first getMore.
    auto m = processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, true /* moreToCome */), true);
    // Test that an exhaust cursor is used in FCV 4.4.
    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime(),
                           true /* exhaustSupported */);

    // Skip cleaning up the cursor (otherwise we'd need to use a side connection to clean up the
    // cursor and this is not supported in unit tests).
    auto skipKillingCursor = globalFailPointRegistry().find("skipKillingExhaustCursorsOnErrors");
    skipKillingCursor->setMode(FailPoint::alwaysOn);

    auto beforeRecreatingCursor = globalFailPointRegistry().find("hangBeforeOplogFetcherRetries");
    auto timesEntered = beforeRecreatingCursor->setMode(FailPoint::alwaysOn);

    // Downgrade to FCV 4.2.
    serverGlobalParams.featureCompatibility.setVersion(
        ServerGlobalParams::FeatureCompatibility::Version::kFullyDowngradedTo42);

    ON_BLOCK_EXIT([&] {
        serverGlobalParams.featureCompatibility.setVersion(
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44);
        skipKillingCursor->setMode(FailPoint::off);
        beforeRecreatingCursor->setMode(FailPoint::off);
    });

    // Simulate an empty batch for the exhaust getMore and this will trigger a retry as FCV has
    // changed to 4.2.
    processSingleExhaustResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, true /* moreToCome */));

    beforeRecreatingCursor->waitForTimesEntered(timesEntered + 1);
    // Test that the connection is disconnected because we cannot use the same connection to
    // recreate cursor as more data is on the way from the server for the exhaust stream.
    ASSERT_TRUE(conn->isFailed());
    // Allow retry and autoreconnect.
    beforeRecreatingCursor->setMode(FailPoint::off);

    // Simulate a response for the find command from the retry.
    m = processSingleRequestResponse(
        conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);
    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    // Simulate a response for the first getMore command after the retry.
    m = processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, false /* moreToCome */), true);
    // Test that a regular cursor is used in FCV 4.2.
    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime(),
                           false /* exhaustSupported */);

    // Terminating empty batch with cursorId 0.
    processSingleRequestResponse(
        conn, makeSubsequentBatch(0LL, {}, metadataObj, false /* moreToCome */), false);

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}

TEST_F(OplogFetcherTest, UpgradeFrom42To44) {
    ShutdownState shutdownState;

    // Start with FCV 4.2.
    serverGlobalParams.featureCompatibility.setVersion(
        ServerGlobalParams::FeatureCompatibility::Version::kDowngradingTo42);
    ON_BLOCK_EXIT([&] {
        serverGlobalParams.featureCompatibility.setVersion(
            ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44);
    });

    // Create an oplog fetcher without any retries, retrying FCV changes does not count towards the
    // max retries.
    auto oplogFetcher = getOplogFetcherAfterConnectionCreated(std::ref(shutdownState));

    CursorId cursorId = 22LL;
    auto firstEntry = makeNoopOplogEntry(lastFetched);
    auto metadataObj = makeOplogBatchMetadata(replSetMetadata, oqMetadata);

    auto conn = oplogFetcher->getDBClientConnection_forTest();

    // Creating the cursor will succeed.
    processSingleRequestResponse(conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);

    // Empty batch from first getMore.
    auto m = processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, false /* moreToCome */), true);
    // Test that a regular cursor is used in FCV 4.2.
    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime(),
                           false /* exhaustSupported */);

    // Upgrade to FCV 4.4.
    serverGlobalParams.featureCompatibility.setVersion(
        ServerGlobalParams::FeatureCompatibility::Version::kFullyUpgradedTo44);

    // Simulate an empty batch for the second getMore and this will trigger a retry as FCV has
    // changed to 4.4.
    processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, false /* moreToCome */), true);

    // Simulate a response for the find command from the retry.
    m = processSingleRequestResponse(
        conn, makeFirstBatch(cursorId, {firstEntry}, metadataObj), true);
    validateFindCommand(
        m, lastFetched, durationCount<Milliseconds>(oplogFetcher->getRetriedFindMaxTime_forTest()));

    // Simulate a response for the first getMore command after the retry.
    m = processSingleRequestResponse(
        conn, makeSubsequentBatch(cursorId, {}, metadataObj, true /* moreToCome */), true);
    // Test that an exhaust cursor is used in FCV 4.4.
    validateGetMoreCommand(m,
                           cursorId,
                           durationCount<Milliseconds>(oplogFetcher->getAwaitDataTimeout_forTest()),
                           dataReplicatorExternalState->getCurrentTermAndLastCommittedOpTime(),
                           true /* exhaustSupported */);

    // Terminating empty batch from exhaust stream with cursorId 0.
    processSingleExhaustResponse(
        conn, makeSubsequentBatch(0LL, {}, metadataObj, false /* moreToCome */), false);

    oplogFetcher->join();

    ASSERT_OK(shutdownState.getStatus());
}
}  // namespace
