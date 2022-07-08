/**
 * Test to make sure that the createIndex command gets sent to all shards if the mongos
 * version is last-stable, and to shards that own chunks only if the mongos version is
 * latest.
 */
(function() {
'use strict';

var st = new ShardingTest({shards: 2});
assert.commandWorked(st.s.adminCommand({enablesharding: 'test'}));
st.ensurePrimaryShard('test', st.shard1.shardName);

var testDB = st.s.getDB('test');
assert.commandWorked(testDB.adminCommand({shardcollection: 'test.user', key: {_id: 1}}));

// Move only chunk out of primary shard.
assert.commandWorked(
    testDB.adminCommand({movechunk: 'test.user', find: {_id: 0}, to: st.shard0.shardName}));

assert.commandWorked(testDB.user.insert({_id: 0}));

var res = testDB.user.ensureIndex({i: 1});
assert.commandWorked(res);

// TODO (SERVER-45017): Remove this check when v4.4 becomes last-stable.
const isLastStableMongos = (jsTestOptions().mongosBinVersion === "last-stable");

var indexes = testDB.user.getIndexes();
assert.eq(2, indexes.length);

indexes = st.rs0.getPrimary().getDB('test').user.getIndexes();
assert.eq(2, indexes.length);

indexes = st.rs1.getPrimary().getDB('test').user.getIndexes();
assert.eq(isLastStableMongos ? 2 : 1, indexes.length);

st.stop();
})();
