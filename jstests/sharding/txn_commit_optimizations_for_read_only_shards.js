/**
 * Tests that the appropriate commit path (single-shard, read-only, single-write-shard, two-phase
 * commit) is taken for a variety of transaction types.
 *
 * Checks that the response formats are correct across each type for several scenarios, including
 * no failures, a participant having failed over, a participant being unable to satisfy the client's
 * writeConcern, and an invalid client writeConcern.
 *
 * TODO (SERVER-48341): Remove requires_fcv_44 after backporting SERVER-48307 to 4.2.
 * @tags: [requires_fcv_44, uses_transactions, uses_multi_shard_transaction]
 */

(function() {
'use strict';

load("jstests/libs/write_concern_util.js");
load("jstests/sharding/libs/sharded_transactions_helpers.js");

// Waits for the given log to appear a number of times in the shell's rawMongoProgramOutput.
// Loops because it is not guaranteed the program output will immediately contain all lines
// logged at an earlier wall clock time.
function waitForLog(logLine, times) {
    assert.soon(function() {
        const matches = rawMongoProgramOutput().match(new RegExp(logLine, "g")) || [];
        return matches.length === times;
    }, 'Failed to find "' + logLine + '" logged ' + times + ' times');
}

const addTxnFields = function(command, lsid, txnNumber, startTransaction) {
    let txnFields = {
        lsid: lsid,
        txnNumber: NumberLong(txnNumber),
        stmtId: NumberInt(0),
        autocommit: false,
    };
    if (startTransaction) {
        txnFields.startTransaction = true;
    }
    return Object.assign({}, command, txnFields);
};

const defaultCommitCommand = {
    commitTransaction: 1,
    writeConcern: {w: "majority", wtimeout: 6000}
};
const noop = () => {};

const dbName = "test";
const collName = "foo";
const ns = dbName + "." + collName;

// TODO (SERVER-37364): Uncomment this line; otherwise, the coordinator will wait too long to
// time out waiting for votes and the test will time out.
// Lower the transaction timeout, since this test exercises cases where the coordinator should
// time out collecting prepare votes.
// TestData.transactionLifetimeLimitSeconds = 30;

let st = new ShardingTest({
    shards: 3,
    // Create shards with more than one node because we test for writeConcern majority failing.
    config: 1,
    other: {
        mongosOptions: {verbose: 3},
        rs0: {nodes: [{}, {rsConfig: {priority: 0}}]},
        rs1: {nodes: [{}, {rsConfig: {priority: 0}}]},
        rs2: {nodes: [{}, {rsConfig: {priority: 0}}]},
    },
});

assert.commandWorked(st.s.adminCommand({enableSharding: dbName}));
assert.commandWorked(st.s.adminCommand({movePrimary: dbName, to: st.shard1.shardName}));

// Create a "dummy" collection for doing noop writes to advance shard's last applied OpTimes.
assert.commandWorked(st.s.getDB(dbName).getCollection("dummy").insert({dummy: 1}));

// The test uses three shards with one chunk each in order to control which shards are targeted
// for each statement:
//
// (-inf, 0):                   shard key = txnNumber * -1
// (0, MAX_TRANSACTIONS):       shard key = txnNumber
// (MAX_TRANSACTIONS, +inf):    shard key = txnNumber + MAX_TRANSACTIONS
//
// So, if the test ever exceeds txnNumber transactions, statements that are meant to target the
// middle chunk will instead target the highest chunk. To fix this, increase MAX_TRANSACTIONS.
const MAX_TRANSACTIONS = 10000;

// Create a sharded collection with a chunk on each shard:
assert.commandWorked(st.s.adminCommand({shardCollection: ns, key: {_id: 1}}));
assert.commandWorked(st.s.adminCommand({split: ns, middle: {_id: 0}}));
assert.commandWorked(st.s.adminCommand({split: ns, middle: {_id: MAX_TRANSACTIONS}}));
assert.commandWorked(st.s.adminCommand({moveChunk: ns, find: {_id: -1}, to: st.shard0.shardName}));
assert.commandWorked(
    st.s.adminCommand({moveChunk: ns, find: {_id: MAX_TRANSACTIONS}, to: st.shard2.shardName}));

// Insert something into each chunk so that a multi-update actually results in a write on each
// shard (otherwise the shard may remain read-only). This also ensures all the routers and
// shards have fresh routing table caches, so they do not need to be refreshed separately.
assert.commandWorked(st.s.getDB(dbName).runCommand({
    insert: collName,
    documents: [{_id: -1 * MAX_TRANSACTIONS}, {_id: 0}, {_id: MAX_TRANSACTIONS}]
}));

let lsid = {id: UUID()};
let txnNumber = 1;

const readShard0 = txnNumber => {
    return {find: collName, filter: {_id: (-1 * txnNumber)}};
};

const readShard1 = txnNumber => {
    return {find: collName, filter: {_id: txnNumber}};
};

const readShard2 = txnNumber => {
    return {find: collName, filter: {_id: (MAX_TRANSACTIONS + txnNumber)}};
};

const readAllShards = () => {
    return {find: collName};
};

const writeShard0 = txnNumber => {
    return {
        update: collName,
        updates:
            [{q: {_id: (txnNumber * -1)}, u: {_id: (txnNumber * -1), updated: 1}, upsert: true}],
    };
};

const writeShard1 = txnNumber => {
    return {
        update: collName,
        updates: [{q: {_id: txnNumber}, u: {_id: txnNumber, updated: 1}, upsert: true}],
    };
};

const writeShard2 = txnNumber => {
    return {
        update: collName,
        updates: [{
            q: {_id: (txnNumber + MAX_TRANSACTIONS)},
            u: {_id: (txnNumber + MAX_TRANSACTIONS), updated: 1},
            upsert: true
        }],
    };
};

const writeAllShards = () => {
    return {
        update: collName,
        updates: [{q: {}, u: {$inc: {updated: 1}}, multi: true}],
    };
};

// For each transaction type, contains the list of statements for that type.
const transactionTypes = {
    readOnlySingleShardSingleStatementExpectSingleShardCommit: txnNumber => {
        return [readShard0(txnNumber)];
    },
    readOnlySingleShardMultiStatementExpectSingleShardCommit: txnNumber => {
        return [readShard0(txnNumber), readShard0(txnNumber)];
    },
    readOnlyMultiShardSingleStatementExpectReadOnlyCommit: txnNumber => {
        return [readAllShards(txnNumber)];
    },
    readOnlyMultiShardMultiStatementExpectReadOnlyCommit: txnNumber => {
        return [readShard0(txnNumber), readShard1(txnNumber), readShard2(txnNumber)];
    },
    writeSingleShardSingleStatementExpectSingleShardCommit: txnNumber => {
        return [writeShard0(txnNumber)];
    },
    writeSingleShardMultiStatementExpectSingleShardCommit: txnNumber => {
        return [writeShard0(txnNumber), writeShard0(txnNumber)];
    },
    writeMultiShardSingleStatementExpectTwoPhaseCommit: txnNumber => {
        return [writeAllShards(txnNumber)];
    },
    writeMultiShardMultiStatementExpectTwoPhaseCommit: txnNumber => {
        return [writeShard0(txnNumber), writeShard1(txnNumber), writeShard2(txnNumber)];
    },
    readWriteSingleShardExpectSingleShardCommit: txnNumber => {
        return [readShard0(txnNumber), writeShard0(txnNumber)];
    },
    writeReadSingleShardExpectSingleShardCommit: txnNumber => {
        return [writeShard0(txnNumber), readShard0(txnNumber)];
    },
    // TODO (SERVER-48340): Re-enable the single-write-shard transaction commit optimization.
    readOneShardWriteOtherShardExpectTwoPhaseCommit: txnNumber => {
        return [readShard0(txnNumber), writeShard1(txnNumber)];
    },
    // TODO (SERVER-48340): Re-enable the single-write-shard transaction commit optimization.
    writeOneShardReadOtherShardExpectTwoPhaseCommit: txnNumber => {
        return [writeShard0(txnNumber), readShard1(txnNumber)];
    },
    readOneShardWriteTwoOtherShardsExpectTwoPhaseCommit: txnNumber => {
        return [readShard0(txnNumber), writeShard1(txnNumber), writeShard2(txnNumber)];
    },
    writeTwoShardsReadOneOtherShardExpectTwoPhaseCommit: txnNumber => {
        return [writeShard0(txnNumber), writeShard1(txnNumber), readShard2(txnNumber)];
    },
};

const failureModes = {
    noFailures: {
        beforeStatements: noop,
        beforeCommit: noop,
        getCommitCommand: (lsid, txnNumber) => {
            return addTxnFields(defaultCommitCommand, lsid, txnNumber);
        },
        checkCommitResult: (res) => {
            // Commit should return ok without writeConcern error
            assert.commandWorked(res);
            assert.eq(null, res.errorLabels);
        },
        cleanUp: noop,
    },
    participantStepsDownBeforeClientSendsCommit: {
        beforeStatements: noop,
        beforeCommit: () => {
            // Participant primary steps down.
            let primary = st.rs0.getPrimary();
            assert.commandWorked(
                primary.adminCommand({replSetStepDown: 60 /* stepDownSecs */, force: true}));
            st.rs0.waitForState(primary, ReplSetTest.State.SECONDARY);
            assert.commandWorked(primary.adminCommand({replSetFreeze: 0}));
        },
        getCommitCommand: (lsid, txnNumber) => {
            return addTxnFields(defaultCommitCommand, lsid, txnNumber);
        },
        checkCommitResult: (res) => {
            // Commit should return NoSuchTransaction.
            assert.commandFailedWithCode(res, ErrorCodes.NoSuchTransaction);
            assert.eq(["TransientTransactionError"], res.errorLabels);
        },
        cleanUp: () => {
            st.rs0.awaitNodesAgreeOnPrimary();
        },
    },
    participantCannotMajorityCommitWritesClientSendsWriteConcernMajority: {
        beforeStatements: () => {
            // Participant cannot majority commit writes.
            stopServerReplication(st.rs0.getSecondaries());

            // Do a write on rs0 through the router outside the transaction to ensure the
            // transaction will choose a read time that has not been majority committed.
            assert.commandWorked(st.s.getDB(dbName).getCollection("dummy").insert({dummy: 1}));
        },
        beforeCommit: noop,
        getCommitCommand: (lsid, txnNumber) => {
            return addTxnFields(defaultCommitCommand, lsid, txnNumber);
        },
        checkCommitResult: (res) => {
            // Commit should return ok with a writeConcernError with wtimeout.
            assert.commandWorkedIgnoringWriteConcernErrors(res);
            checkWriteConcernTimedOut(res);
            assert.eq(null, res.errorLabels);
        },
        cleanUp: () => {
            restartServerReplication(st.rs0.getSecondaries());
        },
    },
    participantCannotMajorityCommitWritesClientSendsWriteConcern1: {
        beforeStatements: () => {
            // Participant cannot majority commit writes.
            stopServerReplication(st.rs0.getSecondaries());

            // Do a write on rs0 through the router outside the transaction to ensure the
            // transaction will choose a read time that has not been majority committed.
            assert.commandWorked(st.s.getDB(dbName).getCollection("dummy").insert({dummy: 1}));
        },
        beforeCommit: noop,
        getCommitCommand: (lsid, txnNumber) => {
            return addTxnFields({commitTransaction: 1, writeConcern: {w: 1}}, lsid, txnNumber);
        },
        checkCommitResult: (res) => {
            // Commit should return ok without writeConcern error
            assert.commandWorked(res);
            assert.eq(null, res.errorLabels);
        },
        cleanUp: () => {
            restartServerReplication(st.rs0.getSecondaries());
        },
    },
    clientSendsInvalidWriteConcernOnCommit: {
        beforeStatements: noop,
        beforeCommit: noop,
        getCommitCommand: (lsid, txnNumber) => {
            // Client sends invalid writeConcern on commit.
            return addTxnFields(
                {commitTransaction: 1, writeConcern: {w: "invalid"}}, lsid, txnNumber);
        },
        checkCommitResult: (res) => {
            // Commit should return ok with writeConcernError without wtimeout.
            assert.commandWorkedIgnoringWriteConcernErrors(res);
            assertWriteConcernError(res);
            assert.eq(ErrorCodes.UnknownReplWriteConcern, res.writeConcernError.code);
            assert(!res.writeConcernError.errInfo || !res.writeConcernError.errInfo.wtimeout);
            assert.eq(null, res.errorLabels);
        },
        cleanUp: noop,
    },
};

for (const failureModeName in failureModes) {
    for (const type in transactionTypes) {
        // TODO (SERVER-37364): Unblacklist these test cases once the coordinator returns the
        // decision as soon as the decision is made. At the moment, the coordinator makes an
        // abort decision after timing out waiting for votes, but coordinateCommitTransaction
        // hangs because it waits for the decision to be majority-ack'd by all participants,
        // which can't happen while a participant can't majority commit writes.
        if (failureModeName.includes("participantCannotMajorityCommitWrites") &&
            type.includes("ExpectTwoPhaseCommit")) {
            jsTest.log(
                `${failureModeName} with ${type} is skipped until SERVER-37364 is implemented`);
            continue;
        }

        txnNumber++;
        assert.lt(txnNumber,
                  MAX_TRANSACTIONS,
                  "Test exceeded maximum number of transactions allowable by the test's chunk" +
                      " distribution created during the test setup. Please increase" +
                      " MAX_TRANSACTIONS in the test.");

        jsTest.log(`Testing ${failureModeName} with ${type} at txnNumber ${txnNumber}`);

        const failureMode = failureModes[failureModeName];

        // Run the statements.
        failureMode.beforeStatements();
        let startTransaction = true;
        transactionTypes[type](txnNumber).forEach(command => {
            assert.commandWorked(st.s.getDB(dbName).runCommand(
                addTxnFields(command, lsid, txnNumber, startTransaction)));
            startTransaction = false;
        });

        // Run commit.
        const commitCmd = failureMode.getCommitCommand(lsid, txnNumber);
        failureMode.beforeCommit();
        const commitRes = st.s.adminCommand(commitCmd);
        failureMode.checkCommitResult(commitRes);

        // Re-running commit should return the same response.
        const commitRetryRes = st.s.adminCommand(commitCmd);
        failureMode.checkCommitResult(commitRetryRes);

        if (type.includes("ExpectSingleShardCommit")) {
            waitForLog("Committing single-shard transaction", 2);
        } else if (type.includes("ExpectReadOnlyCommit")) {
            waitForLog("Committing read-only transaction", 2);
        } else if (type.includes("ExpectSingleWriteShardCommit")) {
            waitForLog("Committing single-write-shard transaction", 2);
        } else if (type.includes("ExpectTwoPhaseCommit")) {
            waitForLog("Committing using two-phase commit", 2);
        } else {
            assert(false, `Unknown transaction type: ${type}`);
        }

        clearRawMongoProgramOutput();

        failureMode.cleanUp();
    }
}

st.stop();
})();
