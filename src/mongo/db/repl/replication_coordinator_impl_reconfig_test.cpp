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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kDefault

#include "mongo/platform/basic.h"

#include "mongo/db/jsobj.h"
#include "mongo/db/operation_context_noop.h"
#include "mongo/db/repl/repl_set_config.h"
#include "mongo/db/repl/repl_set_heartbeat_args_v1.h"
#include "mongo/db/repl/repl_set_heartbeat_response.h"
#include "mongo/db/repl/replication_coordinator.h"  // ReplSetReconfigArgs
#include "mongo/db/repl/replication_coordinator_external_state_mock.h"
#include "mongo/db/repl/replication_coordinator_impl.h"
#include "mongo/db/repl/replication_coordinator_test_fixture.h"
#include "mongo/executor/network_interface_mock.h"
#include "mongo/logv2/log.h"
#include "mongo/unittest/log_test.h"
#include "mongo/unittest/unittest.h"
#include "mongo/util/fail_point.h"

namespace mongo {
namespace repl {
namespace {

using executor::NetworkInterfaceMock;
using executor::RemoteCommandRequest;
using executor::RemoteCommandResponse;

typedef ReplicationCoordinator::ReplSetReconfigArgs ReplSetReconfigArgs;

TEST_F(ReplCoordTest, NodeReturnsNotYetInitializedWhenReconfigReceivedPriorToInitialization) {
    // start up but do not initiate
    init();
    start();
    BSONObjBuilder result;
    ReplSetReconfigArgs args;

    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::NotYetInitialized,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());
}

TEST_F(ReplCoordTest, NodeReturnsNotMasterWhenReconfigReceivedWhileSecondary) {
    // start up, become secondary, receive reconfig
    init();
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));

    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));

    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::NotMaster,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());
}

TEST_F(ReplCoordTest, NodeReturnsNotMasterWhenRunningSafeReconfigWhileInDrainMode) {
    init();

    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 1 << "members"
                            << BSON_ARRAY(BSON("_id" << 0 << "host"
                                                     << "test1:1234")
                                          << BSON("_id" << 1 << "host"
                                                        << "test2:1234"))
                            << "protocolVersion" << 1),
                       HostAndPort("test1", 1234));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    ASSERT_TRUE(getReplCoord()->getMemberState().secondary());

    const auto opCtx = makeOperationContext();
    simulateSuccessfulV1ElectionWithoutExitingDrainMode(
        getReplCoord()->getElectionTimeout_forTest(), opCtx.get());

    ASSERT_EQUALS(1, getReplCoord()->getTerm());
    ASSERT_TRUE(getReplCoord()->getMemberState().primary());

    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    auto status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result);
    ASSERT_EQUALS(ErrorCodes::NotMaster, status);
    ASSERT_STRING_CONTAINS(status.reason(), "Safe reconfig is only allowed on a writable PRIMARY.");
    ASSERT_TRUE(result.obj().isEmpty());
}

TEST_F(ReplCoordTest, NodeReturnsNotMasterWhenReconfigCmdReceivedWhileInDrainMode) {
    init();

    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 1 << "members"
                            << BSON_ARRAY(BSON("_id" << 0 << "host"
                                                     << "test1:1234")
                                          << BSON("_id" << 1 << "host"
                                                        << "test2:1234"))
                            << "protocolVersion" << 1),
                       HostAndPort("test1", 1234));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    ASSERT_TRUE(getReplCoord()->getMemberState().secondary());

    const auto opCtx = makeOperationContext();
    simulateSuccessfulV1ElectionWithoutExitingDrainMode(
        getReplCoord()->getElectionTimeout_forTest(), opCtx.get());

    ASSERT_EQUALS(1, getReplCoord()->getTerm());
    ASSERT_TRUE(getReplCoord()->getMemberState().primary());

    // Reconfig command first waits for config commitment and will get an error.
    auto status = getReplCoord()->awaitConfigCommitment(opCtx.get(), true);
    ASSERT_EQUALS(ErrorCodes::PrimarySteppedDown, status);
    ASSERT_STRING_CONTAINS(status.reason(), "should only be run on a writable PRIMARY");
}


TEST_F(ReplCoordTest, NodeReturnsInvalidReplicaSetConfigWhenReconfigReceivedWithInvalidConfig) {
    // start up, become primary, receive uninitializable config
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 2 << "protocolVersion" << 1 << "invalidlyNamedField"
                             << 3 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345"
                                                      << "arbiterOnly" << true)
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345"
                                                         << "arbiterOnly" << true)));
    const auto opCtx = makeOperationContext();
    // ErrorCodes::BadValue should be propagated from ReplSetConfig::initialize()
    ASSERT_EQUALS(ErrorCodes::InvalidReplicaSetConfig,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());
}

TEST_F(ReplCoordTest, NodeReturnsInvalidReplicaSetConfigWhenReconfigReceivedWithIncorrectSetName) {
    // start up, become primary, receive config with incorrect replset name
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "notMySet"
                             << "version" << 3 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345")));

    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::InvalidReplicaSetConfig,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());
}

TEST_F(ReplCoordTest, NodeReturnsInvalidReplicaSetConfigWhenReconfigReceivedWithIncorrectSetId) {
    // start up, become primary, receive config with incorrect replset name
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))
                            << "settings" << BSON("replicaSetId" << OID::gen())),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 3 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345"))
                             << "settings" << BSON("replicaSetId" << OID::gen()));

    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::NewReplicaSetConfigurationIncompatible,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());
}

TEST_F(ReplCoordTest,
       NodeReturnsNewReplicaSetConfigurationIncompatibleWhenANewConfigFailsToValidate) {
    // start up, become primary, validate fails
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << -3 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345")));

    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::NewReplicaSetConfigurationIncompatible,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());
}

void doReplSetInitiate(ReplicationCoordinatorImpl* replCoord,
                       Status* status,
                       OperationContext* opCtx) {
    BSONObjBuilder garbage;
    *status =
        replCoord->processReplSetInitiate(opCtx,
                                          BSON("_id"
                                               << "mySet"
                                               << "version" << 1 << "members"
                                               << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                                        << "node1:12345")
                                                             << BSON("_id" << 2 << "host"
                                                                           << "node2:12345"))),
                                          &garbage);
}

void doReplSetReconfig(ReplicationCoordinatorImpl* replCoord,
                       Status* status,
                       OperationContext* opCtx,
                       long long term = OpTime::kInitialTerm,
                       bool force = false) {
    BSONObjBuilder garbage;
    ReplSetReconfigArgs args;
    args.force = force;
    // Replica set id will be copied from existing configuration.
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 3 << "term" << term << "protocolVersion" << 1
                             << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345"
                                                         << "priority" << 3)));
    *status = replCoord->processReplSetReconfig(opCtx, args, &garbage);
}

TEST_F(ReplCoordTest, QuorumCheckFailsWhenOtherNodesHaveHigherTerm) {
    // Initiate a config without a term to avoid stepup auto-reconfig.
    auto configObj =
        configWithMembers(2, -1, BSON_ARRAY(member(1, "node1:12345") << member(2, "node2:12345")));

    OpTime opTime{Timestamp(100, 1), 1};
    assertStartSuccess(configObj, HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedAndDurableOpTime(opTime, getNet()->now());
    simulateSuccessfulV1Election();

    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    // Reconfig with version 3.
    stdx::thread reconfigThread([&] {
        doReplSetReconfig(getReplCoord(), &status, opCtx.get(), getReplCoord()->getTerm());
    });

    NetworkInterfaceMock* net = getNet();
    net->enterNetwork();
    auto noi = net->getNextReadyRequest();
    auto& request = noi->getRequest();
    repl::ReplSetHeartbeatArgsV1 hbArgs;
    ASSERT_OK(hbArgs.initialize(request.cmdObj));
    repl::ReplSetHeartbeatResponse hbResp;
    hbResp.setSetName("mySet");
    hbResp.setState(MemberState::RS_SECONDARY);
    // The config version from the remote is lower, but its term is higher.
    hbResp.setConfigVersion(2);
    hbResp.setConfigTerm(getReplCoord()->getTerm() + 1);
    hbResp.setAppliedOpTimeAndWallTime({opTime, net->now()});
    hbResp.setDurableOpTimeAndWallTime({opTime, net->now()});
    net->scheduleResponse(noi, net->now(), makeResponseStatus(hbResp.toBSON()));
    net->runReadyNetworkOperations();
    net->exitNetwork();
    reconfigThread.join();
    ASSERT_EQUALS(ErrorCodes::NewReplicaSetConfigurationIncompatible, status);
}

TEST_F(ReplCoordTest, QuorumCheckSucceedsWhenOtherNodesHaveLowerTerm) {
    // Initiate a config without a term to avoid stepup auto-reconfig.
    auto configObj =
        configWithMembers(2, -1, BSON_ARRAY(member(1, "node1:12345") << member(2, "node2:12345")));

    OpTime opTime{Timestamp(100, 1), 1};
    assertStartSuccess(configObj, HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedAndDurableOpTime(opTime, getNet()->now());
    simulateSuccessfulV1Election();

    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    // Reconfig with version 3.
    stdx::thread reconfigThread([&] {
        doReplSetReconfig(getReplCoord(), &status, opCtx.get(), getReplCoord()->getTerm());
    });

    NetworkInterfaceMock* net = getNet();
    net->enterNetwork();
    auto noi = net->getNextReadyRequest();
    auto& request = noi->getRequest();
    repl::ReplSetHeartbeatArgsV1 hbArgs;
    ASSERT_OK(hbArgs.initialize(request.cmdObj));
    repl::ReplSetHeartbeatResponse hbResp;
    hbResp.setSetName("mySet");
    hbResp.setState(MemberState::RS_SECONDARY);
    // The config version from the remote is higher, but its term is lower.
    hbResp.setConfigVersion(4);
    hbResp.setConfigTerm(getReplCoord()->getTerm() - 1);
    hbResp.setAppliedOpTimeAndWallTime({opTime, net->now()});
    hbResp.setDurableOpTimeAndWallTime({opTime, net->now()});
    net->scheduleResponse(noi, net->now(), makeResponseStatus(hbResp.toBSON()));
    net->runReadyNetworkOperations();
    net->exitNetwork();
    reconfigThread.join();
    ASSERT_OK(status);
}


TEST_F(ReplCoordTest, NodeReturnsOutOfDiskSpaceWhenSavingANewConfigFailsDuringReconfig) {
    // start up, become primary, saving the config fails
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    auto newOpTime = OpTime(Timestamp(101, 1), 1);
    replCoordSetMyLastAppliedOpTime(newOpTime, Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(newOpTime, Date_t() + Seconds(100));

    // Advance optimes of secondary so we pass the config oplog commitment check.
    ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(2, 2, newOpTime));
    ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(2, 2, newOpTime));

    Status status(ErrorCodes::InternalError, "Not Set");
    getExternalState()->setStoreLocalConfigDocumentStatus(
        Status(ErrorCodes::OutOfDiskSpace, "The test set this"));
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread([&] { doReplSetReconfig(getReplCoord(), &status, opCtx.get()); });

    replyToReceivedHeartbeatV1();
    reconfigThread.join();
    ASSERT_EQUALS(ErrorCodes::OutOfDiskSpace, status);
}

TEST_F(ReplCoordTest,
       NodeReturnsConfigurationInProgressWhenReceivingAReconfigWhileInTheMidstOfAnotherReconfig) {
    // start up, become primary, reconfig, then before that reconfig concludes, reconfig again
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    // first reconfig
    stdx::thread reconfigThread([&] { doReplSetReconfig(getReplCoord(), &status, opCtx.get()); });
    getNet()->enterNetwork();
    getNet()->blackHole(getNet()->getNextReadyRequest());
    getNet()->exitNetwork();

    // second reconfig
    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 3 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345")));
    ASSERT_EQUALS(ErrorCodes::ConfigurationInProgress,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());

    shutdown(opCtx.get());
    reconfigThread.join();
}

TEST_F(ReplCoordTest, NodeReturnsConfigurationInProgressWhenReceivingAReconfigWhileInitiating) {
    // start up, initiate, then before that initiate concludes, reconfig
    init();
    start(HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));

    // initiate
    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread initateThread([&] { doReplSetInitiate(getReplCoord(), &status, opCtx.get()); });
    getNet()->enterNetwork();
    getNet()->blackHole(getNet()->getNextReadyRequest());
    getNet()->exitNetwork();

    // reconfig
    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 3 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345")));
    ASSERT_EQUALS(ErrorCodes::ConfigurationInProgress,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    ASSERT_TRUE(result.obj().isEmpty());

    shutdown(opCtx.get());
    initateThread.join();
}

TEST_F(ReplCoordTest, PrimaryNodeAcceptsNewConfigWhenReceivingAReconfigWithACompatibleConfig) {
    // start up, become primary, reconfig successfully
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))
                            << "settings" << BSON("replicaSetId" << OID::gen())),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();

    auto newOpTime = OpTime(Timestamp(101, 1), 1);
    replCoordSetMyLastAppliedOpTime(newOpTime, Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(newOpTime, Date_t() + Seconds(100));

    // Advance optimes of secondary so we pass the config oplog commitment check.
    ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(2, 2, newOpTime));
    ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(2, 2, newOpTime));

    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread(
        [&] { doReplSetReconfig(getReplCoord(), &status, opCtx.get(), OpTime::kInitialTerm); });

    // Satisfy quorum check.
    replyToReceivedHeartbeatV1();

    // Receive heartbeat from secondary saying that it has replicated the new config (v: 3, t: 1).
    // This should allow us to finish waiting for the config majority.
    NetworkInterfaceMock* net = getNet();
    getNet()->enterNetwork();
    const NetworkInterfaceMock::NetworkOperationIterator noi = net->getNextReadyRequest();
    const RemoteCommandRequest& request = noi->getRequest();
    repl::ReplSetHeartbeatArgsV1 hbArgs;
    ASSERT_OK(hbArgs.initialize(request.cmdObj));
    repl::ReplSetHeartbeatResponse hbResp;
    hbResp.setSetName("mySet");
    hbResp.setState(MemberState::RS_SECONDARY);
    hbResp.setConfigVersion(3);
    hbResp.setConfigTerm(1);
    hbResp.setAppliedOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    hbResp.setDurableOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    BSONObjBuilder respObj;
    respObj << "ok" << 1;
    hbResp.addToBSON(&respObj);
    net->scheduleResponse(noi, net->now(), makeResponseStatus(respObj.obj()));
    net->runReadyNetworkOperations();
    getNet()->exitNetwork();
    reconfigThread.join();
    ASSERT_OK(status);
}

TEST_F(ReplCoordTest, OverrideReconfigBsonTermSoReconfigSucceeds) {
    // start up, become primary, reconfig successfully
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))
                            << "settings" << BSON("replicaSetId" << OID::gen())),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();  // Since we have simulated one election, term should be 1.

    auto newOpTime = OpTime(Timestamp(101, 1), 1);
    replCoordSetMyLastAppliedOpTime(newOpTime, Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(newOpTime, Date_t() + Seconds(100));

    // Advance optimes of secondary so we pass the config oplog commitment check.
    ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(2, 2, newOpTime));
    ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(2, 2, newOpTime));

    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    // Term is 1, but pass an invalid term, 50, to the reconfig command. The reconfig should still
    // succeed because we will override 50 with 1.
    stdx::thread reconfigThread(
        [&] { doReplSetReconfig(getReplCoord(), &status, opCtx.get(), 50 /* incorrect term */); });

    // Satisfy quorum check.
    replyToReceivedHeartbeatV1();
    // Satisfy config replication check.
    auto net = getNet();
    enterNetwork();
    const NetworkInterfaceMock::NetworkOperationIterator noi = net->getNextReadyRequest();
    const RemoteCommandRequest& request = noi->getRequest();
    repl::ReplSetHeartbeatArgsV1 hbArgs;
    ASSERT_OK(hbArgs.initialize(request.cmdObj));
    repl::ReplSetHeartbeatResponse hbResp;
    hbResp.setSetName("mySet");
    hbResp.setState(MemberState::RS_SECONDARY);
    hbResp.setConfigVersion(3);
    hbResp.setConfigTerm(1);
    BSONObjBuilder respObj;
    hbResp.setAppliedOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    hbResp.setDurableOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    respObj << "ok" << 1;
    hbResp.addToBSON(&respObj);
    net->scheduleResponse(noi, net->now(), makeResponseStatus(respObj.obj()));
    net->runReadyNetworkOperations();
    exitNetwork();

    reconfigThread.join();
    ASSERT_OK(status);

    // After the reconfig, the config term should be 1, not 50.
    const auto config = getReplCoord()->getConfig();
    ASSERT_EQUALS(config.getConfigTerm(), 1);
}

TEST_F(
    ReplCoordTest,
    NodeReturnsConfigurationInProgressWhenReceivingAReconfigWhileInTheMidstOfAHeartbeatReconfig) {
    // start up, become primary, receive reconfig via heartbeat, then a second one
    // from reconfig
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();
    ASSERT_TRUE(getReplCoord()->getMemberState().primary());

    globalFailPointRegistry().find("blockHeartbeatReconfigFinish")->setMode(FailPoint::alwaysOn);

    // hb reconfig
    NetworkInterfaceMock* net = getNet();
    net->enterNetwork();
    ReplSetHeartbeatResponse hbResp2;
    ReplSetConfig config;
    config
        .initialize(BSON("_id"
                         << "mySet"
                         << "version" << 3 << "protocolVersion" << 1 << "members"
                         << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                  << "node1:12345")
                                       << BSON("_id" << 2 << "host"
                                                     << "node2:12345"))))
        .transitional_ignore();
    hbResp2.setConfig(config);
    hbResp2.setConfigVersion(3);
    hbResp2.setSetName("mySet");
    hbResp2.setState(MemberState::RS_SECONDARY);
    hbResp2.setAppliedOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    hbResp2.setDurableOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    BSONObjBuilder respObj2;
    respObj2 << "ok" << 1;
    hbResp2.addToBSON(&respObj2);
    net->runUntil(net->now() + Seconds(10));  // run until we've sent a heartbeat request
    const NetworkInterfaceMock::NetworkOperationIterator noi2 = net->getNextReadyRequest();
    net->scheduleResponse(noi2, net->now(), makeResponseStatus(respObj2.obj()));
    net->runReadyNetworkOperations();
    getNet()->exitNetwork();

    // reconfig
    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = config.toBSON();
    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::ConfigurationInProgress,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));

    globalFailPointRegistry().find("blockHeartbeatReconfigFinish")->setMode(FailPoint::off);
}

TEST_F(ReplCoordTest, NodeDoesNotAcceptHeartbeatReconfigWhileInTheMidstOfReconfig) {
    // start up, become primary, reconfig, while reconfigging receive reconfig via heartbeat
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    simulateSuccessfulV1Election();
    ASSERT_TRUE(getReplCoord()->getMemberState().primary());

    // start reconfigThread
    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread([&] { doReplSetReconfig(getReplCoord(), &status, opCtx.get()); });

    // wait for reconfigThread to create network requests to ensure the replication coordinator
    // is in state kConfigReconfiguring
    NetworkInterfaceMock* net = getNet();
    net->enterNetwork();
    net->blackHole(net->getNextReadyRequest());

    // schedule hb reconfig
    net->runUntil(net->now() + Seconds(10));  // run until we've sent a heartbeat request
    const NetworkInterfaceMock::NetworkOperationIterator noi = net->getNextReadyRequest();
    ReplSetHeartbeatResponse hbResp;
    ReplSetConfig config;
    config
        .initialize(BSON("_id"
                         << "mySet"
                         << "version" << 4 << "members"
                         << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                  << "node1:12345")
                                       << BSON("_id" << 2 << "host"
                                                     << "node2:12345"))))
        .transitional_ignore();
    hbResp.setConfig(config);
    hbResp.setConfigVersion(4);
    hbResp.setSetName("mySet");
    hbResp.setState(MemberState::RS_SECONDARY);
    hbResp.setAppliedOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    hbResp.setDurableOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
    BSONObjBuilder respObj2;
    respObj2 << "ok" << 1;
    hbResp.addToBSON(&respObj2);
    net->scheduleResponse(noi, net->now(), makeResponseStatus(respObj2.obj()));

    auto severityGuard = unittest::MinimumLoggedSeverityGuard{logv2::LogComponent::kDefault,
                                                              logv2::LogSeverity::Debug(1)};
    startCapturingLogMessages();
    // execute hb reconfig, which should fail with a log message; confirmed at end of test
    net->runReadyNetworkOperations();
    // respond to reconfig's quorum check so that we can join that thread and exit cleanly
    net->exitNetwork();
    stopCapturingLogMessages();
    ASSERT_EQUALS(
        1,
        countTextFormatLogLinesContaining("Ignoring new configuration because we are already in "
                                          "the midst of a configuration process"));
    shutdown(opCtx.get());
    reconfigThread.join();
}

TEST_F(ReplCoordTest, NodeAcceptsConfigFromAReconfigWithForceTrueWhileNotPrimary) {
    // start up, become a secondary, receive a forced reconfig
    init();
    assertStartSuccess(BSON("_id"
                            << "mySet"
                            << "version" << 2 << "members"
                            << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                     << "node1:12345")
                                          << BSON("_id" << 2 << "host"
                                                        << "node2:12345"))),
                       HostAndPort("node1", 12345));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));
    replCoordSetMyLastDurableOpTime(OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100));

    // fail before forced
    BSONObjBuilder result;
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 3 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "node1:12345")
                                           << BSON("_id" << 2 << "host"
                                                         << "node2:12345")));
    const auto opCtx = makeOperationContext();
    ASSERT_EQUALS(ErrorCodes::NotMaster,
                  getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));

    // forced should succeed
    args.force = true;
    ASSERT_OK(getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    getReplCoord()->processReplSetGetConfig(&result);

    // ensure forced reconfig results in a random larger version
    ASSERT_GREATER_THAN(result.obj()["config"].Obj()["version"].numberInt(), 3);
}

class ReplCoordReconfigTest : public ReplCoordTest {
public:
    int counter = 0;
    std::vector<HostAndPort> initialSyncNodes;
    unittest::MinimumLoggedSeverityGuard severityGuard{logv2::LogComponent::kDefault,
                                                       logv2::LogSeverity::Debug(3)};

    void respondToHeartbeat() {
        auto net = getNet();
        auto noi = net->getNextReadyRequest();
        auto&& request = noi->getRequest();
        repl::ReplSetHeartbeatArgsV1 hbArgs;
        ASSERT_OK(hbArgs.initialize(request.cmdObj));
        repl::ReplSetHeartbeatResponse hbResp;
        hbResp.setSetName("mySet");
        hbResp.setState(MemberState::RS_SECONDARY);
        // Secondaries learn of the config version and term immediately.
        hbResp.setConfigVersion(getReplCoord()->getConfig().getConfigVersion());
        hbResp.setConfigTerm(getReplCoord()->getConfig().getConfigTerm());
        BSONObjBuilder respObj;
        hbResp.setAppliedOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
        hbResp.setDurableOpTimeAndWallTime({OpTime(Timestamp(100, 1), 0), Date_t() + Seconds(100)});
        respObj << "ok" << 1;
        hbResp.addToBSON(&respObj);
        net->scheduleResponse(noi, net->now(), makeResponseStatus(respObj.obj()));
        net->runReadyNetworkOperations();
    }

    void respondToNHeartbeats(int n) {
        enterNetwork();
        for (int i = 0; i < n; i++) {
            respondToHeartbeat();
        }
        exitNetwork();
    }

    void respondToAllHeartbeats() {
        enterNetwork();
        while (getNet()->hasReadyRequests()) {
            respondToHeartbeat();
        }
        exitNetwork();
    }

    Status doSafeReconfig(OperationContext* opCtx,
                          int configVersion,
                          BSONArray members,
                          int quorumHeartbeats) {
        ReplSetReconfigArgs args;
        BSONObjBuilder result;
        Status status(ErrorCodes::InternalError, "Not Set");
        args.force = false;
        args.newConfigObj =
            configWithMembers(configVersion, getReplCoord()->getConfig().getConfigTerm(), members);
        stdx::thread reconfigThread = stdx::thread(
            [&] { status = getReplCoord()->processReplSetReconfig(opCtx, args, &result); });
        // Satisfy quorum check with heartbeats.
        LOGV2(24257,
              "Responding to quorum check with heartbeats.",
              "heartbeats"_attr = quorumHeartbeats);
        respondToNHeartbeats(quorumHeartbeats);
        reconfigThread.join();

        // Consume any outstanding heartbeats that were scheduled after reconfig finished.
        respondToAllHeartbeats();
        return status;
    }

    void replicateOpTo(int nodeId, OpTime op) {
        auto configVersion = getReplCoord()->getConfig().getConfigVersion();
        ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(configVersion, nodeId, op));
        ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(configVersion, nodeId, op));
    }
};

TEST_F(ReplCoordReconfigTest, MustFindSelfAndBeElectableInNewConfig) {
    // Start up as a secondary and then get elected.
    init();

    // We only check for ourselves if the config contents actually change.
    auto oldConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 1 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "h1:1")
                                           << BSON("_id" << 2 << "host"
                                                         << "h2:1")
                                           << BSON("_id" << 3 << "host"
                                                         << "h3:1"))
                             << "settings" << BSON("heartbeatIntervalMillis" << 1000));
    auto newConfigObj = BSON("_id"
                             << "mySet"
                             << "version" << 2 << "protocolVersion" << 1 << "members"
                             << BSON_ARRAY(BSON("_id" << 1 << "host"
                                                      << "h1:1")
                                           << BSON("_id" << 2 << "host"
                                                         << "h2:1")
                                           << BSON("_id" << 3 << "host"
                                                         << "h3:1"
                                                         << "priority" << 0))
                             << "settings" << BSON("heartbeatIntervalMillis" << 2000));

    assertStartSuccess(oldConfigObj, HostAndPort("h1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);

    BSONObjBuilder result;
    const auto opCtx = makeOperationContext();
    ReplSetReconfigArgs args;
    args.newConfigObj = newConfigObj;

    // We must be present in the new config. Must hold for both safe and force reconfig.
    getExternalState()->clearSelfHosts();
    getExternalState()->addSelf(HostAndPort("nonself:1"));

    args.force = false;
    Status status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result);
    ASSERT_EQUALS(status.code(), ErrorCodes::NodeNotFound);

    args.force = true;
    status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result);
    ASSERT_EQUALS(status.code(), ErrorCodes::NodeNotFound);

    // We must be electable in the new config. Only required for safe reconfig.
    getExternalState()->clearSelfHosts();
    getExternalState()->addSelf(HostAndPort("h3:1"));  // h3 has priority 0 (unelectable).

    args.force = false;
    status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result);
    ASSERT_EQUALS(status.code(), ErrorCodes::NodeNotElectable);

    args.force = true;
    status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result);
    ASSERT_OK(status.code());
}

TEST_F(ReplCoordReconfigTest,
       InitialReconfigAlwaysSucceedsOnlyRegardlessOfLastCommittedOpInPrevConfig) {
    // Start up as a secondary.
    init();
    assertStartSuccess(
        configWithMembers(
            1, 0, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1"))),
        HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));

    // Simulate application of one oplog entry.
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));

    // Get elected primary.
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Advance the commit point by simulating optime reports from nodes.
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    auto configVersion = 2;
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(configVersion, 2, commitPoint));
    ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(configVersion, 2, commitPoint));
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);

    // An initial reconfig should succeed, since there is no config prior to the initial config.
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj =
        configWithMembers(2,
                          1,
                          BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1")
                                                       << member(4, "n4:1")));

    // Consume all remaining heartbeat requests.
    enterNetwork();
    while (getNet()->hasReadyRequests()) {
        respondToHeartbeat();
    }
    exitNetwork();

    BSONObjBuilder result;
    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread;
    reconfigThread = stdx::thread(
        [&] { status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result); });

    // Satisfy the quorum check.
    enterNetwork();
    respondToHeartbeat();
    respondToHeartbeat();
    exitNetwork();

    // Satisfy config replication check.
    enterNetwork();
    respondToHeartbeat();
    respondToHeartbeat();
    exitNetwork();

    // Satisfy oplog commitment wait.
    ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(configVersion, 3, commitPoint));
    ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(configVersion, 3, commitPoint));

    // The initial reconfig should succeed, since there is no config prior to the initial
    // config.
    reconfigThread.join();
    ASSERT_OK(status);
}

TEST_F(ReplCoordReconfigTest,
       ReconfigSucceedsOnlyWhenLastCommittedOpInPrevConfigIsCommittedInCurrentConfig) {
    // Start out in config version 2 to simulate case where a node that already has a non-initial
    // config.
    init();
    auto configVersion = 2;
    assertStartSuccess(
        configWithMembers(configVersion, 0, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"))),
        HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));

    // Simulate application of one oplog entry.
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));

    // Get elected primary.
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write one new oplog entry.
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    configVersion = 3;
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);

    // Do a reconfig that should fail the oplog commitment pre-condition check.
    ReplSetReconfigArgs args;
    args.force = false;
    args.newConfigObj = configWithMembers(
        configVersion, 1, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1")));

    // Consume all remaining heartbeat requests.
    enterNetwork();
    while (getNet()->hasReadyRequests()) {
        respondToHeartbeat();
    }
    exitNetwork();

    BSONObjBuilder result;
    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread;
    reconfigThread = stdx::thread(
        [&] { status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result); });

    reconfigThread.join();
    ASSERT_EQUALS(status.code(), ErrorCodes::CurrentConfigNotCommittedYet);

    // Reconfig should now succeed after advancing optime of other node.
    ASSERT_OK(getReplCoord()->setLastAppliedOptime_forTest(configVersion, 2, commitPoint));
    ASSERT_OK(getReplCoord()->setLastDurableOptime_forTest(configVersion, 2, commitPoint));

    reconfigThread = stdx::thread(
        [&] { status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result); });

    // Satisfy the quorum check.
    enterNetwork();
    respondToHeartbeat();
    exitNetwork();

    // Satisfy config replication check.
    enterNetwork();
    respondToHeartbeat();
    exitNetwork();

    reconfigThread.join();
    ASSERT_OK(status);
}

TEST_F(ReplCoordReconfigTest, WaitForConfigCommitmentTimesOutIfConfigIsNotCommitted) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    auto Ca_members = BSON_ARRAY(member(1, "n1:1"));
    auto Cb_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"));

    // Startup, simulate application of one oplog entry and get elected.
    assertStartSuccess(configWithMembers(configVersion, 0, Ca_members), HostAndPort("n1", 1));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    const auto opCtx = makeOperationContext();
    runSingleNodeElection(opCtx.get());
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write and commit one new oplog entry, and consume any heartbeats.
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);
    respondToAllHeartbeats();

    // Do a first reconfig that should succeed since the current config is committed.
    Status status(ErrorCodes::InternalError, "Not Set");
    configVersion = 3;
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 1 /* quorumHbs */));

    opCtx->setDeadlineAfterNowBy(Milliseconds(1), ErrorCodes::MaxTimeMSExpired);
    stdx::thread reconfigThread =
        stdx::thread([&] { status = getReplCoord()->awaitConfigCommitment(opCtx.get(), true); });

    // Run clock past the deadline.
    enterNetwork();
    getNet()->runUntil(getNet()->now() + Milliseconds(2));
    exitNetwork();

    reconfigThread.join();
    ASSERT_EQUALS(status.code(), ErrorCodes::MaxTimeMSExpired);
}

TEST_F(ReplCoordReconfigTest, WaitForConfigCommitmentReturnsOKIfConfigIsCommitted) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    auto Ca_members = BSON_ARRAY(member(1, "n1:1"));
    auto Cb_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"));

    // Startup, simulate application of one oplog entry and get elected.
    assertStartSuccess(configWithMembers(configVersion, 0, Ca_members), HostAndPort("n1", 1));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    const auto opCtx = makeOperationContext();
    runSingleNodeElection(opCtx.get());
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write and commit one new oplog entry, and consume any heartbeats.
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);
    respondToAllHeartbeats();

    // Do a first reconfig that should succeed since the current config is committed.
    configVersion = 3;
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 1 /* quorumHbs */));

    // Replicate op to ensure config is committed.
    replicateOpTo(2, commitPoint);
    ASSERT_OK(getReplCoord()->awaitConfigCommitment(opCtx.get(), true));
}

TEST_F(ReplCoordReconfigTest,
       ReconfigFrom1to2NodesSucceedsOnlyWhenLastCommittedOpInPrevConfigIsCommittedInCurrentConfig) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    auto Ca_members = BSON_ARRAY(member(1, "n1:1"));
    auto Cb_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"));

    // Startup, simulate application of one oplog entry and get elected.
    assertStartSuccess(configWithMembers(configVersion, 0, Ca_members), HostAndPort("n1", 1));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    const auto opCtx = makeOperationContext();
    runSingleNodeElection(opCtx.get());
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write and commit one new oplog entry, and consume any heartbeats.
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);
    respondToAllHeartbeats();

    // Do a first reconfig that should succeed since the current config is committed.
    configVersion = 3;
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 1 /* quorumHbs */));

    // Try to reconfig out of Cb which should fail.
    configVersion = 4;
    Status status = doSafeReconfig(opCtx.get(), configVersion, Cb_members, 0 /* quorumHbs */);
    ASSERT_EQUALS(status.code(), ErrorCodes::CurrentConfigNotCommittedYet);

    // Catch up node and try reconfig again.
    replicateOpTo(2, commitPoint);
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 1 /* quorumHbs */));
}

TEST_F(ReplCoordReconfigTest,
       ReconfigFrom3to4NodesSucceedsOnlyWhenLastCommittedOpInPrevConfigIsCommittedInCurrentConfig) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    auto Ca_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1"));
    auto Cb_members = BSON_ARRAY(member(1, "n1:1")
                                 << member(2, "n2:1") << member(3, "n3:1") << member(4, "n4:1"));

    // Start up, simulate application of one oplog entry and get elected.
    assertStartSuccess(configWithMembers(configVersion, 0, Ca_members), HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write and commit one new oplog entry, and consume any heartbeats.
    const auto opCtx = makeOperationContext();
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    replicateOpTo(2, commitPoint);
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);
    respondToAllHeartbeats();

    // Do a first reconfig that should succeed since the current config is committed.
    configVersion = 3;
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 2 /* quorumHbs */));

    // Try to reconfig out of Cb which should fail.
    configVersion = 4;
    Status status = doSafeReconfig(opCtx.get(), configVersion, Cb_members, 0 /* quorumHbs */);
    ASSERT_EQUALS(status.code(), ErrorCodes::CurrentConfigNotCommittedYet);

    // Catch up node and try reconfig again.
    replicateOpTo(3, commitPoint);
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 3 /* quorumHbs */));
}

TEST_F(ReplCoordReconfigTest,
       ReconfigFrom3to2NodesSucceedsOnlyWhenLastCommittedOpInPrevConfigIsCommittedInCurrentConfig) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    auto Ca_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1"));
    auto Cb_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"));

    // Start up, simulate application of one oplog entry and get elected.
    assertStartSuccess(configWithMembers(configVersion, 0, Ca_members), HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write and commit one new oplog entry, and consume any heartbeats. We replicate the op to node
    // 3, which will be removed from the config.
    const auto opCtx = makeOperationContext();
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    replicateOpTo(3, commitPoint);
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);
    respondToAllHeartbeats();

    // Do a first reconfig that should succeed since the current config is committed.
    configVersion = 3;
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 2 /* quorumHbs */));

    // Try to reconfig out of Cb which should fail.
    configVersion = 4;
    Status status = doSafeReconfig(opCtx.get(), configVersion, Cb_members, 0 /* quorumHbs */);
    ASSERT_EQUALS(status.code(), ErrorCodes::CurrentConfigNotCommittedYet);

    // Catch up node and try reconfig again.
    replicateOpTo(2, commitPoint);
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 2 /* quorumHbs */));
}

TEST_F(ReplCoordReconfigTest,
       ReconfigFrom5to4NodesSucceedsOnlyWhenLastCommittedOpInPrevConfigIsCommittedInCurrentConfig) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    auto Ca_members = BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1")
                                                   << member(4, "n4:1") << member(5, "n5:1"));
    auto Cb_members = BSON_ARRAY(member(1, "n1:1")
                                 << member(2, "n2:1") << member(3, "n3:1") << member(4, "n4:1"));

    // Start up, simulate application of one oplog entry and get elected.
    assertStartSuccess(configWithMembers(configVersion, 0, Ca_members), HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Write and commit one new oplog entry, and consume any heartbeats. We replicate the op to
    // nodes 4 and 5, one of which will be removed from the config.
    const auto opCtx = makeOperationContext();
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    replicateOpTo(4, commitPoint);
    replicateOpTo(5, commitPoint);
    ASSERT_EQ(getReplCoord()->getLastCommittedOpTime(), commitPoint);
    respondToAllHeartbeats();

    // Do a first reconfig that should succeed since the current config is committed.
    configVersion = 3;
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 3 /* quorumHbs */));

    // Try to reconfig out of Cb which should fail.
    configVersion = 4;
    Status status = doSafeReconfig(opCtx.get(), configVersion, Cb_members, 0 /* quorumHbs */);
    ASSERT_EQUALS(status.code(), ErrorCodes::CurrentConfigNotCommittedYet);

    // Catch up node and try reconfig again.
    replicateOpTo(2, commitPoint);
    ASSERT_OK(doSafeReconfig(opCtx.get(), configVersion, Cb_members, 3 /* quorumHbs */));
}

TEST_F(ReplCoordReconfigTest,
       ForceReconfigSucceedsEvenWhenLastCommittedOpInPrevConfigIsNotCommittedInCurrentConfig) {
    // Start out in config version 2 to simulate case where a node that already has a non-initial
    // config.
    init();
    auto configVersion = 2;
    assertStartSuccess(
        configWithMembers(configVersion, 0, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"))),
        HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));

    // Simulate application of one oplog entry.
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));

    // Get elected primary.
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Advance your optime.
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(2, 1), 1));

    // Do a force reconfig that should succeed even though oplog commitment pre-condition check is
    // not satisfied.
    configVersion = 3;
    ReplSetReconfigArgs args;
    args.force = true;
    args.newConfigObj = configWithMembers(
        configVersion, 1, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1")));

    BSONObjBuilder result;
    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread;
    reconfigThread = stdx::thread(
        [&] { status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result); });

    reconfigThread.join();
    ASSERT_OK(status);
}

TEST_F(ReplCoordReconfigTest, StepdownShouldInterruptConfigWrite) {
    // Start out in a non-initial config version.
    init();
    auto configVersion = 2;
    assertStartSuccess(
        configWithMembers(configVersion, 0, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1"))),
        HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));

    // Simulate application of one oplog entry.
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));

    // Get elected primary.
    simulateSuccessfulV1Election();
    ASSERT_EQ(getReplCoord()->getMemberState(), MemberState::RS_PRIMARY);
    ASSERT_EQ(getReplCoord()->getTerm(), 1);

    // Advance your optime.
    auto commitPoint = OpTime(Timestamp(2, 1), 1);
    replCoordSetMyLastAppliedAndDurableOpTime(commitPoint);
    replicateOpTo(2, commitPoint);

    // Respond to heartbeats before reconfig.
    respondToAllHeartbeats();

    // Do a reconfig that should fail due to stepdown.
    configVersion = 3;
    ReplSetReconfigArgs args;
    args.newConfigObj = configWithMembers(
        configVersion, 1, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1")));

    BSONObjBuilder result;
    Status status(ErrorCodes::InternalError, "Not Set");
    const auto opCtx = makeOperationContext();
    stdx::thread reconfigThread;
    reconfigThread = stdx::thread(
        [&] { status = getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result); });

    getNet()->enterNetwork();
    // Wait for the next heartbeat of quorum check and blackhole it.
    // The reconfig is hung during the quorum check.
    auto request = getNet()->getNextReadyRequest();
    getNet()->blackHole(request);
    getNet()->exitNetwork();

    // Step down due to a higher term.
    TopologyCoordinator::UpdateTermResult termUpdated;
    auto updateTermEvh = getReplCoord()->updateTerm_forTest(2, &termUpdated);
    ASSERT(termUpdated == TopologyCoordinator::UpdateTermResult::kTriggerStepDown);
    ASSERT(updateTermEvh.isValid());
    getReplExec()->waitForEvent(updateTermEvh);

    // Respond to quorum check to resume the reconfig.
    respondToAllHeartbeats();

    reconfigThread.join();
    ASSERT_EQ(status.code(), ErrorCodes::NotMaster);
    ASSERT_EQ(status.reason(), "Stepped down when persisting new config");
}

TEST_F(ReplCoordReconfigTest, StartElectionOnReconfigToSingleNode) {
    // Start up as a secondary.
    init();
    assertStartSuccess(
        configWithMembers(
            1, 0, BSON_ARRAY(member(1, "n1:1") << member(2, "n2:1") << member(3, "n3:1"))),
        HostAndPort("n1", 1));
    ASSERT_OK(getReplCoord()->setFollowerMode(MemberState::RS_SECONDARY));

    // Simulate application of one oplog entry.
    replCoordSetMyLastAppliedAndDurableOpTime(OpTime(Timestamp(1, 1), 0));

    // Construct the new config of single node replset.
    auto configVersion = 2;
    ReplSetReconfigArgs args;
    args.force = true;
    args.newConfigObj = configWithMembers(configVersion, 1, BSON_ARRAY(member(1, "n1:1")));

    BSONObjBuilder result;
    const auto opCtx = makeOperationContext();
    ASSERT_OK(getReplCoord()->processReplSetReconfig(opCtx.get(), args, &result));
    getNet()->enterNetwork();
    getNet()->runReadyNetworkOperations();
    getNet()->exitNetwork();

    ASSERT_EQUALS(MemberState::RS_PRIMARY, getReplCoord()->getMemberState().s);
}
}  // anonymous namespace
}  // namespace repl
}  // namespace mongo
