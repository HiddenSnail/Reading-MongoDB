/**
 * Test that the validate command properly limits the index entry inconsistencies reported when
 * there is corruption on an index with a long name.
 *
 * @tags: [requires_wiredtiger, requires_persistence]
 */
(function() {
"use strict";

load("jstests/disk/libs/wt_file_helper.js");

// 64 * 1024 * 1024 = 64MB worth of index names ensures that we test against the maximum BSONObj
// size lmit.
const kNumDocs = 64;
const kIndexNameLength = 1024 * 1024;

const baseName = "validate_with_long_index_name";
const dbpath = MongoRunner.dataPath + baseName + "/";
const indexName = "a".repeat(kIndexNameLength);
let conn = MongoRunner.runMongod({dbpath: dbpath});
let coll = conn.getDB("test").getCollection("corrupt");

function insertDocsAndBuildIndex() {
    const bulk = coll.initializeUnorderedBulkOp();
    for (let i = 0; i < kNumDocs; i++) {
        bulk.insert({_id: i});
    }
    bulk.execute();
    coll.createIndex({a: 1}, {name: indexName});
}

function truncateAndRestartMongod(uri) {
    MongoRunner.stopMongod(conn, null, {skipValidation: true});
    runWiredTigerTool("-h", conn.dbpath, "truncate", uri);
    conn = startMongodOnExistingPath(dbpath, {});
    coll = conn.getDB("test").getCollection("corrupt");
}

insertDocsAndBuildIndex();
let uri = getUriForIndex(coll, indexName);
truncateAndRestartMongod(uri);

const missingIndexEntries = "Detected " + kNumDocs + " missing index entries.";
const missingSizeLimitations =
    "Not all missing index entry inconsistencies are listed due to size limitations.";

let res = coll.validate();
assert.commandWorked(res);
assert(!res.valid);
assert.contains(missingIndexEntries, res.warnings);
assert.contains(missingSizeLimitations, res.errors);

coll.drop();
insertDocsAndBuildIndex();
uri = getUriForColl(coll);
truncateAndRestartMongod(uri);

const extraIndexEntries = "Detected " + 2 * kNumDocs + " extra index entries.";
const extraSizeLimitations =
    "Not all extra index entry inconsistencies are listed due to size limitations.";

res = coll.validate();
assert.commandWorked(res);
assert(!res.valid);
assert.contains(extraIndexEntries, res.warnings);
assert.contains(extraSizeLimitations, res.errors);

MongoRunner.stopMongod(conn, null, {skipValidation: true});
})();