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

#include "mongo/platform/basic.h"

#include <boost/intrusive_ptr.hpp>
#include <memory>

#include "mongo/bson/bsonelement.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/exec/document_value/document_value_test_util.h"
#include "mongo/db/pipeline/aggregation_context_fixture.h"
#include "mongo/db/pipeline/document_source_check_resume_token.h"
#include "mongo/db/pipeline/document_source_mock.h"
#include "mongo/db/pipeline/expression_context.h"
#include "mongo/db/pipeline/process_interface/stub_mongo_process_interface.h"
#include "mongo/db/pipeline/resume_token.h"
#include "mongo/db/query/collation/collator_interface_mock.h"
#include "mongo/db/service_context.h"
#include "mongo/unittest/death_test.h"
#include "mongo/unittest/unittest.h"
#include "mongo/util/uuid.h"

using boost::intrusive_ptr;
using std::deque;

namespace mongo {
namespace {
static constexpr StringData kTestNs = "test.ns"_sd;

class CheckResumeTokenTest : public AggregationContextFixture {
public:
    CheckResumeTokenTest() : _mock(DocumentSourceMock::createForTest()) {}

protected:
    /**
     * Pushes a document with a resume token corresponding to the given ResumeTokenData into the
     * mock queue.
     */
    void addDocument(ResumeTokenData tokenData) {
        _mock->push_back(Document{{"_id", ResumeToken(std::move(tokenData)).toDocument()}});
    }

    /**
     * Pushes a document with a resume token corresponding to the given timestamp, version,
     * txnOpIndex, docKey, and namespace into the mock queue.
     */
    void addDocument(
        Timestamp ts, int version, std::size_t txnOpIndex, Document docKey, UUID uuid) {
        return addDocument({ts, version, txnOpIndex, uuid, Value(docKey)});
    }

    /**
     * Pushes a document with a resume token corresponding to the given timestamp, version,
     * txnOpIndex, docKey, and namespace into the mock queue.
     */
    void addDocument(Timestamp ts, Document docKey, UUID uuid = testUuid()) {
        addDocument(ts, 0, 0, docKey, uuid);
    }
    /**
     * Pushes a document with a resume token corresponding to the given timestamp, _id string, and
     * namespace into the mock queue.
     */
    void addDocument(Timestamp ts, std::string id, UUID uuid = testUuid()) {
        addDocument(ts, 0, 0, Document{{"_id", id}}, uuid);
    }

    void addPause() {
        _mock->push_back(DocumentSource::GetNextResult::makePauseExecution());
    }

    /**
     * Convenience method to create the class under test with a given ResumeTokenData.
     */
    intrusive_ptr<DocumentSourceEnsureResumeTokenPresent> createCheckResumeToken(
        ResumeTokenData tokenData) {
        auto checkResumeToken =
            DocumentSourceEnsureResumeTokenPresent::create(getExpCtx(), std::move(tokenData));
        checkResumeToken->setSource(_mock.get());
        return checkResumeToken;
    }

    /**
     * Convenience method to create the class under test with a given timestamp, docKey, and
     * namespace.
     */
    intrusive_ptr<DocumentSourceEnsureResumeTokenPresent> createCheckResumeToken(
        Timestamp ts,
        int version,
        std::size_t txnOpIndex,
        boost::optional<Document> docKey,
        UUID uuid) {
        return createCheckResumeToken(
            {ts, version, txnOpIndex, uuid, docKey ? Value(*docKey) : Value()});
    }

    /**
     * Convenience method to create the class under test with a given timestamp, docKey, and
     * namespace.
     */
    intrusive_ptr<DocumentSourceEnsureResumeTokenPresent> createCheckResumeToken(
        Timestamp ts, boost::optional<Document> docKey, UUID uuid = testUuid()) {
        return createCheckResumeToken(ts, 0, 0, docKey, uuid);
    }

    /**
     * Convenience method to create the class under test with a given timestamp, _id string, and
     * namespace.
     */
    intrusive_ptr<DocumentSourceEnsureResumeTokenPresent> createCheckResumeToken(
        Timestamp ts, StringData id, UUID uuid = testUuid()) {
        return createCheckResumeToken(ts, 0, 0, Document{{"_id", id}}, uuid);
    }

    /**
     * This method is required to avoid a static initialization fiasco resulting from calling
     * UUID::gen() in file or class static scope.
     */
    static const UUID& testUuid() {
        static const UUID* uuid_gen = new UUID(UUID::gen());
        return *uuid_gen;
    }

    intrusive_ptr<DocumentSourceMock> _mock;
};

class ShardCheckResumabilityTest : public CheckResumeTokenTest {
protected:
    intrusive_ptr<DocumentSourceShardCheckResumability> createShardCheckResumability(
        ResumeTokenData tokenData) {
        auto shardCheckResumability =
            DocumentSourceShardCheckResumability::create(getExpCtx(), tokenData);
        shardCheckResumability->setSource(_mock.get());
        return shardCheckResumability;
    }
    intrusive_ptr<DocumentSourceShardCheckResumability> createShardCheckResumability(Timestamp ts) {
        return createShardCheckResumability(ResumeToken::makeHighWaterMarkToken(ts).getData());
    }
};

TEST_F(CheckResumeTokenTest, ShouldSucceedWithOnlyResumeToken) {
    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "1");
    addDocument(resumeTimestamp, "1");
    // We should not see the resume token.
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, ShouldSucceedWithPausesBeforeResumeToken) {
    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "1");
    addPause();
    addDocument(resumeTimestamp, "1");

    // We see the pause we inserted, but not the resume token.
    ASSERT_TRUE(checkResumeToken->getNext().isPaused());
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, ShouldSucceedWithPausesAfterResumeToken) {
    Timestamp resumeTimestamp(100, 1);
    Timestamp doc1Timestamp(100, 2);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "1");
    addDocument(resumeTimestamp, "1");
    addPause();
    addDocument(doc1Timestamp, "2");

    // Pause added explicitly.
    ASSERT_TRUE(checkResumeToken->getNext().isPaused());
    // The document after the resume token should be the first.
    auto result1 = checkResumeToken->getNext();
    ASSERT_TRUE(result1.isAdvanced());
    auto& doc1 = result1.getDocument();
    ASSERT_EQ(doc1Timestamp, ResumeToken::parse(doc1["_id"].getDocument()).getData().clusterTime);
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, ShouldSucceedWithMultipleDocumentsAfterResumeToken) {
    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "0");
    addDocument(resumeTimestamp, "0");

    Timestamp doc1Timestamp(100, 2);
    Timestamp doc2Timestamp(101, 1);
    addDocument(doc1Timestamp, "1");
    addDocument(doc2Timestamp, "2");

    auto result1 = checkResumeToken->getNext();
    ASSERT_TRUE(result1.isAdvanced());
    auto& doc1 = result1.getDocument();
    ASSERT_EQ(doc1Timestamp, ResumeToken::parse(doc1["_id"].getDocument()).getData().clusterTime);
    auto result2 = checkResumeToken->getNext();
    ASSERT_TRUE(result2.isAdvanced());
    auto& doc2 = result2.getDocument();
    ASSERT_EQ(doc2Timestamp, ResumeToken::parse(doc2["_id"].getDocument()).getData().clusterTime);
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, ShouldFailIfFirstDocHasWrongResumeToken) {
    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "1");

    Timestamp doc1Timestamp(100, 2);
    Timestamp doc2Timestamp(101, 1);
    addDocument(doc1Timestamp, "1");
    addDocument(doc2Timestamp, "2");
    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest, ShouldIgnoreChangeWithEarlierTimestamp) {
    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "1");
    addDocument(resumeTimestamp, "0");
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, ShouldFailIfTokenHasWrongNamespace) {
    Timestamp resumeTimestamp(100, 1);

    auto resumeTokenUUID = UUID::gen();
    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "1", resumeTokenUUID);
    auto otherUUID = UUID::gen();
    addDocument(resumeTimestamp, "1", otherUUID);
    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest, ShouldSucceedWithBinaryCollation) {
    CollatorInterfaceMock collatorCompareLower(CollatorInterfaceMock::MockType::kToLowerString);
    getExpCtx()->setCollator(collatorCompareLower.clone());

    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "abc");
    // We must not see the following document.
    addDocument(resumeTimestamp, "ABC");
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, UnshardedTokenSucceedsForShardedResumeOnMongosIfIdMatchesFirstDoc) {
    // Verify that a resume token whose documentKey only contains _id can be used to resume a stream
    // on a sharded collection as long as its _id matches the first document. We set 'inMongos'
    // since this behaviour is only applicable when DocumentSourceEnsureResumeTokenPresent is
    // running on mongoS.
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, Document{{"_id"_sd, 1}});

    Timestamp doc1Timestamp(100, 1);
    addDocument(doc1Timestamp, {{"x"_sd, 0}, {"_id"_sd, 1}});
    Timestamp doc2Timestamp(100, 2);
    Document doc2DocKey{{"x"_sd, 0}, {"_id"_sd, 2}};
    addDocument(doc2Timestamp, doc2DocKey);

    // We should skip doc1 since it satisfies the resume token, and retrieve doc2.
    const auto firstDocAfterResume = checkResumeToken->getNext();
    const auto tokenFromFirstDocAfterResume =
        ResumeToken::parse(firstDocAfterResume.getDocument()["_id"].getDocument()).getData();

    ASSERT_EQ(tokenFromFirstDocAfterResume.clusterTime, doc2Timestamp);
    ASSERT_DOCUMENT_EQ(tokenFromFirstDocAfterResume.documentKey.getDocument(), doc2DocKey);
}

TEST_F(CheckResumeTokenTest, UnshardedTokenFailsForShardedResumeOnMongosIfIdDoesNotMatchFirstDoc) {
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, Document{{"_id"_sd, 1}});

    addDocument(Timestamp(100, 1), {{"x"_sd, 0}, {"_id"_sd, 0}});
    addDocument(Timestamp(100, 2), {{"x"_sd, 0}, {"_id"_sd, 2}});

    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest, ShardedResumeFailsOnMongosIfTokenHasSubsetOfDocumentKeyFields) {
    // Verify that the relaxed _id check only applies if _id is the sole field present in the
    // client's resume token, even if all the fields that are present match the first doc. We set
    // 'inMongos' since this is only applicable when DocumentSourceEnsureResumeTokenPresent is
    // running on mongoS.
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    auto checkResumeToken =
        createCheckResumeToken(resumeTimestamp, Document{{"x"_sd, 0}, {"_id"_sd, 1}});

    addDocument(Timestamp(100, 1), {{"x"_sd, 0}, {"y"_sd, -1}, {"_id"_sd, 1}});
    addDocument(Timestamp(100, 2), {{"x"_sd, 0}, {"y"_sd, -1}, {"_id"_sd, 2}});

    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest, ShardedResumeFailsOnMongosIfDocumentKeyIsNonObject) {
    // Verify that a resume token whose documentKey is not a valid object will neither succeed nor
    // cause an invariant when we perform the relaxed documentKey._id check when running in a
    // sharded context.
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, boost::none);

    addDocument(Timestamp(100, 1), {{"x"_sd, 0}, {"_id"_sd, 1}});
    addDocument(Timestamp(100, 2), {{"x"_sd, 0}, {"_id"_sd, 2}});

    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest, ShardedResumeFailsOnMongosIfDocumentKeyOmitsId) {
    // Verify that a resume token whose documentKey omits the _id field will neither succeed nor
    // cause an invariant when we perform the relaxed documentKey._id, even when compared against an
    // artificial stream token whose _id is also missing.
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, Document{{"x"_sd, 0}});

    addDocument(Timestamp(100, 1), {{"x"_sd, 0}, {"y"_sd, -1}, {"_id", 1}});
    addDocument(Timestamp(100, 1), {{"x"_sd, 0}, {"y"_sd, -1}});
    addDocument(Timestamp(100, 2), {{"x"_sd, 0}, {"y"_sd, -1}});

    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest,
       ShardedResumeSucceedsOnMongosWithSameClusterTimeIfUUIDsSortBeforeResumeToken) {
    // On a sharded cluster, the documents observed by the pipeline during a resume attempt may have
    // the same clusterTime if they come from different shards. If this is a whole-db or
    // cluster-wide changeStream, then their UUIDs may legitimately differ. As long as the UUID of
    // the current document sorts before the client's resume token, we should continue to examine
    // the next document in the stream.
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    // Create an ordered array of 2 UUIDs.
    UUID uuids[2] = {UUID::gen(), UUID::gen()};

    if (uuids[0] > uuids[1]) {
        std::swap(uuids[0], uuids[1]);
    }

    // Create the resume token using the higher-sorting UUID.
    auto checkResumeToken =
        createCheckResumeToken(resumeTimestamp, Document{{"_id"_sd, 1}}, uuids[1]);

    // Add two documents which have the same clusterTime but a lower UUID. One of the documents has
    // a lower docKey than the resume token, the other has a higher docKey; this demonstrates that
    // the UUID is the discriminating factor.
    addDocument(resumeTimestamp, {{"_id"_sd, 0}}, uuids[0]);
    addDocument(resumeTimestamp, {{"_id"_sd, 2}}, uuids[0]);

    // Add a third document that matches the resume token.
    addDocument(resumeTimestamp, {{"_id"_sd, 1}}, uuids[1]);

    // Add a fourth document with the same timestamp and UUID whose docKey sorts after the token.
    auto expectedDocKey = Document{{"_id"_sd, 3}};
    addDocument(resumeTimestamp, expectedDocKey, uuids[1]);

    // We should skip the first two docs, swallow the resume token, and return the fourth doc.
    const auto firstDocAfterResume = checkResumeToken->getNext();
    const auto tokenFromFirstDocAfterResume =
        ResumeToken::parse(firstDocAfterResume.getDocument()["_id"].getDocument()).getData();

    ASSERT_EQ(tokenFromFirstDocAfterResume.clusterTime, resumeTimestamp);
    ASSERT_DOCUMENT_EQ(tokenFromFirstDocAfterResume.documentKey.getDocument(), expectedDocKey);
}

TEST_F(CheckResumeTokenTest,
       ShardedResumeFailsOnMongosWithSameClusterTimeIfUUIDsSortAfterResumeToken) {
    Timestamp resumeTimestamp(100, 1);
    getExpCtx()->inMongos = true;

    // Create an ordered array of 2 UUIDs.
    UUID uuids[2] = {UUID::gen(), UUID::gen()};

    if (uuids[0] > uuids[1]) {
        std::swap(uuids[0], uuids[1]);
    }

    // Create the resume token using the lower-sorting UUID.
    auto checkResumeToken =
        createCheckResumeToken(resumeTimestamp, Document{{"_id"_sd, 1}}, uuids[0]);

    // Add a document which has the same clusterTime and a lower docKey but a higher UUID, followed
    // by a document which matches the resume token. This is not possible in practice, but it serves
    // to demonstrate that the resume attempt fails even when the resume token is present.
    addDocument(resumeTimestamp, {{"_id"_sd, 0}}, uuids[1]);
    addDocument(resumeTimestamp, {{"_id"_sd, 1}}, uuids[0]);

    ASSERT_THROWS_CODE(
        checkResumeToken->getNext(), AssertionException, ErrorCodes::ChangeStreamFatalError);
}

TEST_F(CheckResumeTokenTest, ShouldSwallowInvalidateFromEachShardForStartAfterInvalidate) {
    Timestamp resumeTimestamp(100, 1);
    Timestamp firstEventAfter(100, 2);

    // Create an array of 2 UUIDs. The first represents the UUID of the namespace before it was
    // dropped. The second is the UUID of the collection after it is recreated.
    UUID uuids[2] = {UUID::gen(), UUID::gen()};

    // This behaviour is only relevant when DSEnsureResumeTokenPresent is running on mongoS.
    getExpCtx()->inMongos = true;

    // Create a resume token representing an 'invalidate' event, and use it to seed the stage. A
    // resume token with {fromInvalidate:true} can only be used with startAfter, to start a new
    // stream after the old stream is invalidated.
    ResumeTokenData invalidateToken;
    invalidateToken.clusterTime = resumeTimestamp;
    invalidateToken.uuid = uuids[0];
    invalidateToken.fromInvalidate = ResumeTokenData::kFromInvalidate;
    auto checkResumeToken = createCheckResumeToken(invalidateToken);

    // Add three documents which each have the invalidate resume token. We expect to see this in the
    // event that we are starting after an invalidate and the invalidating event occurred on several
    // shards at the same clusterTime.
    addDocument(invalidateToken);
    addDocument(invalidateToken);
    addDocument(invalidateToken);

    // Add a document representing an insert which recreated the collection after it was dropped.
    auto expectedDocKey = Document{{"_id"_sd, 1}};
    addDocument(Timestamp{100, 2}, expectedDocKey, uuids[1]);

    // DSEnsureResumeTokenPresent should confirm that the invalidate event is present, swallow it
    // and the next two invalidates, and return the insert event after the collection drop.
    const auto firstDocAfterResume = checkResumeToken->getNext();
    const auto tokenFromFirstDocAfterResume =
        ResumeToken::parse(firstDocAfterResume.getDocument()["_id"].getDocument()).getData();

    ASSERT_EQ(tokenFromFirstDocAfterResume.clusterTime, firstEventAfter);
    ASSERT_DOCUMENT_EQ(tokenFromFirstDocAfterResume.documentKey.getDocument(), expectedDocKey);
}

TEST_F(CheckResumeTokenTest, ShouldNotSwallowUnrelatedInvalidateForStartAfterInvalidate) {
    Timestamp resumeTimestamp(100, 1);

    // This behaviour is only relevant when DSEnsureResumeTokenPresent is running on mongoS.
    getExpCtx()->inMongos = true;

    // Create an ordered array of of 2 UUIDs.
    std::vector<UUID> uuids = {UUID::gen(), UUID::gen()};
    std::sort(uuids.begin(), uuids.end());

    // Create a resume token representing an 'invalidate' event, and use it to seed the stage. A
    // resume token with {fromInvalidate:true} can only be used with startAfter, to start a new
    // stream after the old stream is invalidated.
    ResumeTokenData invalidateToken;
    invalidateToken.clusterTime = resumeTimestamp;
    invalidateToken.uuid = uuids[0];
    invalidateToken.fromInvalidate = ResumeTokenData::kFromInvalidate;
    auto checkResumeToken = createCheckResumeToken(invalidateToken);

    // Create a second invalidate token with the same clusterTime but a different UUID.
    auto unrelatedInvalidateToken = invalidateToken;
    unrelatedInvalidateToken.uuid = uuids[1];

    // Add three documents which each have the invalidate resume token. We expect to see this in the
    // event that we are starting after an invalidate and the invalidating event occurred on several
    // shards at the same clusterTime.
    addDocument(invalidateToken);
    addDocument(invalidateToken);
    addDocument(invalidateToken);

    // Add a fourth document which has the unrelated invalidate at the same clusterTime.
    addDocument(unrelatedInvalidateToken);

    // DSEnsureResumeTokenPresent should confirm that the invalidate event is present, swallow it
    // and the next two invalidates, but decline to swallow the unrelated invalidate.
    const auto firstDocAfterResume = checkResumeToken->getNext();
    const auto tokenFromFirstDocAfterResume =
        ResumeToken::parse(firstDocAfterResume.getDocument()["_id"].getDocument()).getData();

    ASSERT_EQ(tokenFromFirstDocAfterResume, unrelatedInvalidateToken);
}

TEST_F(CheckResumeTokenTest, ShouldSwallowOnlyFirstInvalidateForStartAfterInvalidateInReplSet) {
    Timestamp resumeTimestamp(100, 1);

    // We only swallow multiple invalidates when DSEnsureResumeTokenPresent is running on mongoS.
    // Set {inMongos:false} to verify that we do not swallow additional invalidates on a replica
    // set, since this should never occur.
    getExpCtx()->inMongos = false;

    // Create a resume token representing an 'invalidate' event, and use it to seed the stage. A
    // resume token with {fromInvalidate:true} can only be used with startAfter, to start a new
    // stream after the old stream is invalidated.
    ResumeTokenData invalidateToken;
    invalidateToken.clusterTime = resumeTimestamp;
    invalidateToken.uuid = testUuid();
    invalidateToken.fromInvalidate = ResumeTokenData::kFromInvalidate;
    auto checkResumeToken = createCheckResumeToken(invalidateToken);

    // Add three documents which each have the invalidate resume token.
    addDocument(invalidateToken);
    addDocument(invalidateToken);
    addDocument(invalidateToken);

    // DSEnsureResumeTokenPresent should confirm that the invalidate event is present and swallow
    // it. However, it should not swallow the subsequent two invalidates.
    for (size_t i = 0; i < 2; ++i) {
        const auto nextDocAfterResume = checkResumeToken->getNext();
        const auto tokenFromNextDocAfterResume =
            ResumeToken::parse(nextDocAfterResume.getDocument()["_id"].getDocument()).getData();
        ASSERT_EQ(tokenFromNextDocAfterResume, invalidateToken);
    }
    ASSERT(checkResumeToken->getNext().isEOF());
}

TEST_F(CheckResumeTokenTest, ShouldSkipResumeTokensWithEarlierTxnOpIndex) {
    Timestamp resumeTimestamp(100, 1);

    // Create an ordered array of 3 UUIDs.
    std::vector<UUID> uuids = {UUID::gen(), UUID::gen(), UUID::gen()};

    std::sort(uuids.begin(), uuids.end());

    auto checkResumeToken =
        createCheckResumeToken(resumeTimestamp, 0, 2, Document{{"_id"_sd, 1}}, uuids[1]);

    // Add two documents which have the same clusterTime and version but a lower applyOps index. One
    // of the documents has a lower uuid than the resume token, the other has a higher uuid; this
    // demonstrates that the applyOps index is the discriminating factor.
    addDocument(resumeTimestamp, 0, 0, {{"_id"_sd, 0}}, uuids[0]);
    addDocument(resumeTimestamp, 0, 1, {{"_id"_sd, 2}}, uuids[2]);

    // Add a third document that matches the resume token.
    addDocument(resumeTimestamp, 0, 2, {{"_id"_sd, 1}}, uuids[1]);

    // Add a fourth document with the same timestamp and version whose applyOps sorts after the
    // resume token.
    auto expectedDocKey = Document{{"_id"_sd, 3}};
    addDocument(resumeTimestamp, 0, 3, expectedDocKey, uuids[1]);

    // We should skip the first two docs, swallow the resume token, and return the fourth doc.
    const auto firstDocAfterResume = checkResumeToken->getNext();
    const auto tokenFromFirstDocAfterResume =
        ResumeToken::parse(firstDocAfterResume.getDocument()["_id"].getDocument()).getData();

    ASSERT_EQ(tokenFromFirstDocAfterResume.clusterTime, resumeTimestamp);
    ASSERT_DOCUMENT_EQ(tokenFromFirstDocAfterResume.documentKey.getDocument(), expectedDocKey);
}

/**
 * We should _error_ on the no-document case, because that means the resume token was not found.
 */
TEST_F(CheckResumeTokenTest, ShouldSucceedWithNoDocuments) {
    Timestamp resumeTimestamp(100, 1);

    auto checkResumeToken = createCheckResumeToken(resumeTimestamp, "0");
    ASSERT_TRUE(checkResumeToken->getNext().isEOF());
}

/**
 * A mock MongoProcessInterface which allows mocking a foreign pipeline.
 */
class MockMongoInterface final : public StubMongoProcessInterface {
public:
    MockMongoInterface(deque<DocumentSource::GetNextResult> mockResults)
        : _mockResults(std::move(mockResults)) {}

    bool isSharded(OperationContext* opCtx, const NamespaceString& nss) final {
        return false;
    }

    std::unique_ptr<Pipeline, PipelineDeleter> attachCursorSourceToPipeline(
        Pipeline* ownedPipeline, bool localCursorOnly) final {
        std::unique_ptr<Pipeline, PipelineDeleter> pipeline(
            ownedPipeline, PipelineDeleter(ownedPipeline->getContext()->opCtx));
        pipeline->addInitialSource(DocumentSourceMock::createForTest(_mockResults));
        return pipeline;
    }

private:
    deque<DocumentSource::GetNextResult> _mockResults;
};

TEST_F(ShardCheckResumabilityTest,
       ShouldSucceedIfResumeTokenIsPresentAndEarliestOplogEntryBeforeToken) {
    Timestamp oplogTimestamp(100, 1);
    Timestamp resumeTimestamp(100, 2);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    addDocument(resumeTimestamp, "ID");
    // We should see the resume token.
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    auto& doc = result.getDocument();
    ASSERT_EQ(resumeTimestamp, ResumeToken::parse(doc["_id"].getDocument()).getData().clusterTime);
}

TEST_F(ShardCheckResumabilityTest,
       ShouldSucceedIfResumeTokenIsPresentAndEarliestOplogEntryAfterToken) {
    Timestamp resumeTimestamp(100, 1);
    Timestamp oplogTimestamp(100, 2);

    ResumeTokenData tokenData(
        resumeTimestamp, 0, 0, testUuid(), Value(Document{{"_id"_sd, "ID"_sd}}));
    auto shardCheckResumability = createShardCheckResumability(tokenData);
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    addDocument(resumeTimestamp, "ID");
    // We should see the resume token.
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    auto& doc = result.getDocument();
    ASSERT_EQ(resumeTimestamp, ResumeToken::parse(doc["_id"].getDocument()).getData().clusterTime);
}

TEST_F(ShardCheckResumabilityTest, ShouldSucceedIfResumeTokenIsPresentAndOplogIsEmpty) {
    Timestamp resumeTimestamp(100, 1);

    ResumeTokenData token(resumeTimestamp, 0, 0, testUuid(), Value(Document{{"_id"_sd, "ID"_sd}}));
    auto shardCheckResumability = createShardCheckResumability(token);
    deque<DocumentSource::GetNextResult> mockOplog;
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    addDocument(resumeTimestamp, "ID");
    // We should see the resume token.
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    auto& doc = result.getDocument();
    ASSERT_EQ(resumeTimestamp, ResumeToken::parse(doc["_id"].getDocument()).getData().clusterTime);
}

TEST_F(ShardCheckResumabilityTest,
       ShouldSucceedWithNoDocumentsInPipelineAndEarliestOplogEntryBeforeToken) {
    Timestamp oplogTimestamp(100, 1);
    Timestamp resumeTimestamp(100, 2);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isEOF());
}

TEST_F(ShardCheckResumabilityTest,
       ShouldFailWithNoDocumentsInPipelineAndEarliestOplogEntryAfterToken) {
    Timestamp resumeTimestamp(100, 1);
    Timestamp oplogTimestamp(100, 2);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    ASSERT_THROWS_CODE(
        shardCheckResumability->getNext(), AssertionException, ErrorCodes::ChangeStreamHistoryLost);
}

TEST_F(ShardCheckResumabilityTest, ShouldSucceedWithNoDocumentsInPipelineAndOplogIsEmpty) {
    Timestamp resumeTimestamp(100, 2);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    deque<DocumentSource::GetNextResult> mockOplog;
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isEOF());
}

TEST_F(ShardCheckResumabilityTest,
       ShouldSucceedWithLaterDocumentsInPipelineAndEarliestOplogEntryBeforeToken) {
    Timestamp oplogTimestamp(100, 1);
    Timestamp resumeTimestamp(100, 2);
    Timestamp docTimestamp(100, 3);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    addDocument(docTimestamp, "ID");
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    auto& doc = result.getDocument();
    ASSERT_EQ(docTimestamp, ResumeToken::parse(doc["_id"].getDocument()).getData().clusterTime);
}

TEST_F(ShardCheckResumabilityTest,
       ShouldFailWithLaterDocumentsInPipelineAndEarliestOplogEntryAfterToken) {
    Timestamp resumeTimestamp(100, 1);
    Timestamp oplogTimestamp(100, 2);
    Timestamp docTimestamp(100, 3);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    addDocument(docTimestamp, "ID");
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    ASSERT_THROWS_CODE(
        shardCheckResumability->getNext(), AssertionException, ErrorCodes::ChangeStreamHistoryLost);
}

TEST_F(ShardCheckResumabilityTest, ShouldIgnoreOplogAfterFirstDoc) {
    Timestamp oplogTimestamp(100, 1);
    Timestamp resumeTimestamp(100, 2);
    Timestamp oplogFutureTimestamp(100, 3);
    Timestamp docTimestamp(100, 4);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    addDocument(docTimestamp, "ID");
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result1 = shardCheckResumability->getNext();
    ASSERT_TRUE(result1.isAdvanced());
    auto& doc1 = result1.getDocument();
    ASSERT_EQ(docTimestamp, ResumeToken::parse(doc1["_id"].getDocument()).getData().clusterTime);

    mockOplog = {Document{{"ts", oplogFutureTimestamp}}};
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result2 = shardCheckResumability->getNext();
    ASSERT_TRUE(result2.isEOF());
}

TEST_F(ShardCheckResumabilityTest, ShouldSucceedWhenOplogEntriesExistBeforeAndAfterResumeToken) {
    Timestamp oplogTimestamp(100, 1);
    Timestamp resumeTimestamp(100, 2);
    Timestamp oplogFutureTimestamp(100, 3);
    Timestamp docTimestamp(100, 4);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    addDocument(docTimestamp, "ID");
    deque<DocumentSource::GetNextResult> mockOplog(
        {{Document{{"ts", oplogTimestamp}}}, {Document{{"ts", oplogFutureTimestamp}}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result1 = shardCheckResumability->getNext();
    ASSERT_TRUE(result1.isAdvanced());
    auto& doc1 = result1.getDocument();
    ASSERT_EQ(docTimestamp, ResumeToken::parse(doc1["_id"].getDocument()).getData().clusterTime);
    auto result2 = shardCheckResumability->getNext();
    ASSERT_TRUE(result2.isEOF());
}

TEST_F(ShardCheckResumabilityTest, ShouldIgnoreOplogAfterFirstEOF) {
    Timestamp oplogTimestamp(100, 1);
    Timestamp resumeTimestamp(100, 2);
    Timestamp oplogFutureTimestamp(100, 3);

    auto shardCheckResumability = createShardCheckResumability(resumeTimestamp);
    deque<DocumentSource::GetNextResult> mockOplog({Document{{"ts", oplogTimestamp}}});
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result1 = shardCheckResumability->getNext();
    ASSERT_TRUE(result1.isEOF());

    mockOplog = {Document{{"ts", oplogFutureTimestamp}}};
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);
    auto result2 = shardCheckResumability->getNext();
    ASSERT_TRUE(result2.isEOF());
}

TEST_F(ShardCheckResumabilityTest, ShouldSwallowAllEventsAtSameClusterTimeUpToResumeToken) {
    Timestamp resumeTimestamp(100, 2);

    // Set up the DocumentSourceShardCheckResumability to check for an exact event ResumeToken.
    ResumeTokenData token(resumeTimestamp, 0, 0, testUuid(), Value(Document{{"_id"_sd, "3"_sd}}));
    auto shardCheckResumability = createShardCheckResumability(token);
    deque<DocumentSource::GetNextResult> mockOplog;
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);

    // Add 2 events at the same clusterTime as the resume token but whose docKey sort before it.
    addDocument(resumeTimestamp, "1");
    addDocument(resumeTimestamp, "2");
    // Add the resume token, plus one further event whose docKey sorts after the token.
    addDocument(resumeTimestamp, "3");
    addDocument(resumeTimestamp, "4");

    // The first event we see should be the resume token...
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    auto doc = result.getDocument();
    ASSERT_EQ(token, ResumeToken::parse(doc["_id"].getDocument()).getData());
    // ... then the post-token event, and then finally EOF.
    result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    Document postResumeTokenDoc{
        {"_id",
         ResumeToken({resumeTimestamp, 0, 0, testUuid(), Value(Document{{"_id"_sd, "4"_sd}})})
             .toDocument()}};
    ASSERT_DOCUMENT_EQ(result.getDocument(), postResumeTokenDoc);
    ASSERT_TRUE(shardCheckResumability->getNext().isEOF());
}

TEST_F(ShardCheckResumabilityTest, ShouldSwallowAllEventsAtSameClusterTimePriorToResumeToken) {
    Timestamp resumeTimestamp(100, 2);

    // Set up the DocumentSourceShardCheckResumability to check for an exact event ResumeToken.
    ResumeTokenData token(resumeTimestamp, 0, 0, testUuid(), Value(Document{{"_id"_sd, "3"_sd}}));
    auto shardCheckResumability = createShardCheckResumability(token);
    deque<DocumentSource::GetNextResult> mockOplog;
    getExpCtx()->mongoProcessInterface = std::make_shared<MockMongoInterface>(mockOplog);

    // Add 2 events at the same clusterTime as the resume token but whose docKey sort before it.
    addDocument(resumeTimestamp, "1");
    addDocument(resumeTimestamp, "2");
    // Add one further event whose docKey sorts after the token.
    addDocument(resumeTimestamp, "4");

    // The first event we see should be the post-token event, followed by EOF.
    auto result = shardCheckResumability->getNext();
    ASSERT_TRUE(result.isAdvanced());
    Document postResumeTokenDoc{
        {"_id",
         ResumeToken({resumeTimestamp, 0, 0, testUuid(), Value(Document{{"_id"_sd, "4"_sd}})})
             .toDocument()}};
    ASSERT_DOCUMENT_EQ(result.getDocument(), postResumeTokenDoc);
    ASSERT_TRUE(shardCheckResumability->getNext().isEOF());
}

}  // namespace
}  // namespace mongo
