/**
 * This test exercises an unrecoverable rollback using rollback_test.js, expecting it to terminate
 * cleanly early instead of failing.
 * An unrecoverable rollback can happen with EMRC:false, as it is possible for rollback via refetch
 * to set a minValid based on oplog entries that the sync source may have failed to recover after
 * an unclean shutdown. The rollback node will need to sync and apply oplog entries up to minValid
 * to be consistent, but if those oplog entries no longer exist, then it will be stuck in sync
 * source selection and unable to complete recovery.
 * This test reproduces this scenario in a simpler form by modifying the minValid on the rollback
 * node very far forward, so that we do not have to simulate anything happening to the sync source.
 */

(function() {
"use strict";

load("jstests/libs/fail_point_util.js");
load("jstests/replsets/libs/rollback_test.js");

function tsToDate(ts) {
    return new Date(ts.getTime() * 1000);
}

const testName = jsTestName();

const rst = new ReplSetTest({
    name: testName,
    nodes: [{}, {}, {rsConfig: {priority: 0}}],
    useBridge: true,
    settings: {chainingAllowed: false},
    nodeOptions: {enableMajorityReadConcern: "false"}
});
rst.startSet();
rst.initiateWithHighElectionTimeout();

const rollbackTest = new RollbackTest(testName, rst);
const rollbackNode = rollbackTest.transitionToRollbackOperations();

rollbackTest.transitionToSyncSourceOperationsBeforeRollback();

const failpoint = configureFailPoint(rollbackNode, "rollbackHangBeforeFinish");

rollbackTest.transitionToSyncSourceOperationsDuringRollback();

assert.soonNoExcept(function() {
    // Rollback will cause the node to close connections.
    failpoint.wait();
    return true;
});

const farFutureTS = new Timestamp(
    Math.floor(new Date().getTime() / 1000) + (60 * 60 * 24 * 5 /* in five days*/), 0);

jsTest.log("future TS: " + tojson(farFutureTS) + ", date:" + tsToDate(farFutureTS));

const mMinvalid = rollbackNode.getDB("local").getCollection("replset.minvalid");

const minValidUpdate = {
    $set: {ts: farFutureTS}
};
jsTestLog("Current minValid is " + tojson(mMinvalid.findOne()));
jsTestLog("Updating minValid to: " + tojson(minValidUpdate));
printjson(assert.commandWorked(mMinvalid.update({}, minValidUpdate)));

failpoint.off();

rollbackTest.setAwaitSecondaryNodesForRollbackTimeout(5 * 1000);

// We will detect an unrecoverable rollback here.
// This will cause an assert.soon() in ReplSetTest to fail. This normally triggers the hang
// analyzer, but since we do not want to run it on expected timeouts, we temporarily disable it.
MongoRunner.runHangAnalyzer.disable();
try {
    rollbackTest.transitionToSteadyStateOperations();
} finally {
    MongoRunner.runHangAnalyzer.enable();
}

rollbackTest.stop();
})();