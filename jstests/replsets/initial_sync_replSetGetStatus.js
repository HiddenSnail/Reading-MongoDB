/**
 * This test tests that replSetGetStatus returns initial sync stats while initial sync is in
 * progress.
 */

(function() {
"use strict";

load("jstests/libs/fail_point_util.js");

var name = 'initial_sync_replSetGetStatus';
var replSet = new ReplSetTest({
    name: name,
    nodes: 1,
});

replSet.startSet();
replSet.initiate();
var primary = replSet.getPrimary();

var coll = primary.getDB('test').foo;
assert.commandWorked(coll.insert({a: 1}));
assert.commandWorked(coll.insert({a: 2}));

// Add a secondary node but make it hang before copying databases.
var secondary = replSet.add({rsConfig: {votes: 0, priority: 0}});
secondary.setSlaveOk();

var failPointBeforeCopying = configureFailPoint(secondary, 'initialSyncHangBeforeCopyingDatabases');
var failPointBeforeFinish = configureFailPoint(secondary, 'initialSyncHangBeforeFinish');
replSet.reInitiate();

// Wait for initial sync to pause before it copies the databases.
failPointBeforeCopying.wait();

// Test that replSetGetStatus returns the correct results while initial sync is in progress.
var res = assert.commandWorked(secondary.adminCommand({replSetGetStatus: 1}));
assert(res.initialSyncStatus,
       () => "Response should have an 'initialSyncStatus' field: " + tojson(res));

res = assert.commandWorked(secondary.adminCommand({replSetGetStatus: 1, initialSync: 0}));
assert(!res.initialSyncStatus,
       () => "Response should not have an 'initialSyncStatus' field: " + tojson(res));

assert.commandFailedWithCode(secondary.adminCommand({replSetGetStatus: 1, initialSync: "t"}),
                             ErrorCodes.TypeMismatch);

assert.commandWorked(coll.insert({a: 3}));
assert.commandWorked(coll.insert({a: 4}));

// Let initial sync continue working.
failPointBeforeCopying.off();

// Wait for initial sync to pause right before it finishes.
failPointBeforeFinish.wait();

// Test that replSetGetStatus returns the correct results when initial sync is at the very end.
res = assert.commandWorked(secondary.adminCommand({replSetGetStatus: 1}));
assert(res.initialSyncStatus,
       () => "Response should have an 'initialSyncStatus' field: " + tojson(res));

// It is possible that we update the config document after going through a reconfig. So make sure
// we account for this.
assert.gte(res.initialSyncStatus.appliedOps, 3);

assert.eq(res.initialSyncStatus.failedInitialSyncAttempts, 0);
assert.eq(res.initialSyncStatus.maxFailedInitialSyncAttempts, 10);
assert.eq(res.initialSyncStatus.databases.databasesCloned, 3);
assert.eq(res.initialSyncStatus.databases.test.collections, 1);
assert.eq(res.initialSyncStatus.databases.test.clonedCollections, 1);
assert.eq(res.initialSyncStatus.databases.test["test.foo"].documentsToCopy, 4);
assert.eq(res.initialSyncStatus.databases.test["test.foo"].documentsCopied, 4);
assert.eq(res.initialSyncStatus.databases.test["test.foo"].indexes, 1);
assert.eq(res.initialSyncStatus.databases.test["test.foo"].fetchedBatches, 1);

// Let initial sync finish and get into secondary state.
failPointBeforeFinish.off();
replSet.awaitSecondaryNodes(60 * 1000);

// Test that replSetGetStatus returns the correct results after initial sync is finished.
res = assert.commandWorked(secondary.adminCommand({replSetGetStatus: 1}));
assert(!res.initialSyncStatus,
       () => "Response should not have an 'initialSyncStatus' field: " + tojson(res));

assert.commandFailedWithCode(secondary.adminCommand({replSetGetStatus: 1, initialSync: "m"}),
                             ErrorCodes.TypeMismatch);
assert.eq(0,
          secondary.getDB('local')['temp_oplog_buffer'].find().itcount(),
          "Oplog buffer was not dropped after initial sync");

replSet.stopSet();
})();
