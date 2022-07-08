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

#include <algorithm>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

#include "mongo/db/catalog/collection_catalog.h"
#include "mongo/db/catalog/collection_mock.h"
#include "mongo/db/dbmessage.h"
#include "mongo/db/json.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/pipeline/aggregation_request.h"
#include "mongo/db/query/query_request.h"
#include "mongo/db/service_context_test_fixture.h"
#include "mongo/unittest/unittest.h"

namespace mongo {
namespace {

using std::unique_ptr;
using unittest::assertGet;

static const NamespaceString testns("testdb.testcoll");

TEST(QueryRequestTest, LimitWithNToReturn) {
    QueryRequest qr(testns);
    qr.setLimit(0);
    qr.setNToReturn(0);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, BatchSizeWithNToReturn) {
    QueryRequest qr(testns);
    qr.setBatchSize(0);
    qr.setNToReturn(0);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, NegativeSkip) {
    QueryRequest qr(testns);
    qr.setSkip(-1);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ZeroSkip) {
    QueryRequest qr(testns);
    qr.setSkip(0);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, PositiveSkip) {
    QueryRequest qr(testns);
    qr.setSkip(1);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, NegativeLimit) {
    QueryRequest qr(testns);
    qr.setLimit(-1);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ZeroLimit) {
    QueryRequest qr(testns);
    qr.setLimit(0);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, PositiveLimit) {
    QueryRequest qr(testns);
    qr.setLimit(1);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, NegativeBatchSize) {
    QueryRequest qr(testns);
    qr.setBatchSize(-1);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ZeroBatchSize) {
    QueryRequest qr(testns);
    qr.setBatchSize(0);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, PositiveBatchSize) {
    QueryRequest qr(testns);
    qr.setBatchSize(1);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, NegativeNToReturn) {
    QueryRequest qr(testns);
    qr.setNToReturn(-1);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ZeroNToReturn) {
    QueryRequest qr(testns);
    qr.setNToReturn(0);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, PositiveNToReturn) {
    QueryRequest qr(testns);
    qr.setNToReturn(1);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, NegativeMaxTimeMS) {
    QueryRequest qr(testns);
    qr.setMaxTimeMS(-1);
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ZeroMaxTimeMS) {
    QueryRequest qr(testns);
    qr.setMaxTimeMS(0);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, PositiveMaxTimeMS) {
    QueryRequest qr(testns);
    qr.setMaxTimeMS(1);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, ValidSortOrder) {
    QueryRequest qr(testns);
    qr.setSort(fromjson("{a: 1}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, DoesNotErrorOnInvalidSortPattern) {
    QueryRequest qr(testns);
    qr.setSort(fromjson("{a: \"\"}"));
    // QueryRequest isn't responsible for validating the sort pattern, so it is considered valid
    // even though the sort pattern {a: ""} is not well-formed.
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, MinFieldsNotPrefixOfMax) {
    QueryRequest qr(testns);
    qr.setMin(fromjson("{a: 1}"));
    qr.setMax(fromjson("{b: 1}"));
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, MinFieldsMoreThanMax) {
    QueryRequest qr(testns);
    qr.setMin(fromjson("{a: 1, b: 1}"));
    qr.setMax(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, MinFieldsLessThanMax) {
    QueryRequest qr(testns);
    qr.setMin(fromjson("{a: 1}"));
    qr.setMax(fromjson("{a: 1, b: 1}"));
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ForbidTailableWithNonNaturalSort) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "tailable: true,"
        "sort: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ForbidTailableWithSingleBatch) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "tailable: true,"
        "singleBatch: true}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, AllowTailableWithNaturalSort) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "tailable: true,"
        "sort: {$natural: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_OK(result.getStatus());
    ASSERT_TRUE(result.getValue()->isTailable());
    ASSERT_BSONOBJ_EQ(result.getValue()->getSort(), BSON("$natural" << 1));
}

//
// Test compatibility of various projection and sort objects.
//

TEST(QueryRequestTest, ValidSortProj) {
    QueryRequest qr(testns);
    qr.setProj(fromjson("{a: 1}"));
    qr.setSort(fromjson("{a: 1}"));
    ASSERT_OK(qr.validate());

    QueryRequest metaQR(testns);
    metaQR.setProj(fromjson("{a: {$meta: \"textScore\"}}"));
    metaQR.setSort(fromjson("{a: {$meta: \"textScore\"}}"));
    ASSERT_OK(metaQR.validate());
}

TEST(QueryRequestTest, TextScoreMetaSortOnFieldDoesNotRequireMetaProjection) {
    QueryRequest qr(testns);
    qr.setProj(fromjson("{b: 1}"));
    qr.setSort(fromjson("{a: {$meta: 'textScore'}}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, TextScoreMetaProjectionDoesNotRequireTextScoreMetaSort) {
    QueryRequest qr(testns);
    qr.setProj(fromjson("{a: {$meta: \"textScore\"}}"));
    qr.setSort(fromjson("{b: 1}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, RequestResumeTokenWithHint) {
    QueryRequest qr(testns);
    qr.setRequestResumeToken(true);
    ASSERT_NOT_OK(qr.validate());
    qr.setHint(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.validate());
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, RequestResumeTokenWithSort) {
    QueryRequest qr(testns);
    qr.setRequestResumeToken(true);
    // Hint must be explicitly set for the query request to validate.
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_OK(qr.validate());
    qr.setSort(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.validate());
    qr.setSort(fromjson("{$natural: 1}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, InvalidResumeAfterWrongRecordIdType) {
    QueryRequest qr(testns);
    BSONObj resumeAfter = BSON("$recordId" << 1);
    qr.setResumeAfter(resumeAfter);
    qr.setRequestResumeToken(true);
    // Hint must be explicitly set for the query request to validate.
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_NOT_OK(qr.validate());
    resumeAfter = BSON("$recordId" << 1LL);
    qr.setResumeAfter(resumeAfter);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, InvalidResumeAfterExtraField) {
    QueryRequest qr(testns);
    BSONObj resumeAfter = BSON("$recordId" << 1LL << "$extra" << 1);
    qr.setResumeAfter(resumeAfter);
    qr.setRequestResumeToken(true);
    // Hint must be explicitly set for the query request to validate.
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_NOT_OK(qr.validate());
}

TEST(QueryRequestTest, ResumeAfterWithHint) {
    QueryRequest qr(testns);
    BSONObj resumeAfter = BSON("$recordId" << 1LL);
    qr.setResumeAfter(resumeAfter);
    qr.setRequestResumeToken(true);
    ASSERT_NOT_OK(qr.validate());
    qr.setHint(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.validate());
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, ResumeAfterWithSort) {
    QueryRequest qr(testns);
    BSONObj resumeAfter = BSON("$recordId" << 1LL);
    qr.setResumeAfter(resumeAfter);
    qr.setRequestResumeToken(true);
    // Hint must be explicitly set for the query request to validate.
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_OK(qr.validate());
    qr.setSort(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.validate());
    qr.setSort(fromjson("{$natural: 1}"));
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, ResumeNoSpecifiedRequestResumeToken) {
    QueryRequest qr(testns);
    BSONObj resumeAfter = BSON("$recordId" << 1LL);
    qr.setResumeAfter(resumeAfter);
    // Hint must be explicitly set for the query request to validate.
    qr.setHint(fromjson("{$natural: 1}"));
    ASSERT_NOT_OK(qr.validate());
    qr.setRequestResumeToken(true);
    ASSERT_OK(qr.validate());
}

TEST(QueryRequestTest, ExplicitEmptyResumeAfter) {
    QueryRequest qr(NamespaceString::kRsOplogNamespace);
    BSONObj resumeAfter = fromjson("{}");
    // Hint must be explicitly set for the query request to validate.
    qr.setHint(fromjson("{$natural: 1}"));
    qr.setResumeAfter(resumeAfter);
    ASSERT_OK(qr.validate());
    qr.setRequestResumeToken(true);
    ASSERT_OK(qr.validate());
}

//
// Text meta BSON element validation
//

bool isFirstElementTextScoreMeta(const char* sortStr) {
    BSONObj sortObj = fromjson(sortStr);
    BSONElement elt = sortObj.firstElement();
    bool result = QueryRequest::isTextScoreMeta(elt);
    return result;
}

// Check validation of $meta expressions
TEST(QueryRequestTest, IsTextScoreMeta) {
    // Valid textScore meta sort
    ASSERT(isFirstElementTextScoreMeta("{a: {$meta: \"textScore\"}}"));

    // Invalid textScore meta sorts
    ASSERT_FALSE(isFirstElementTextScoreMeta("{a: {$meta: 1}}"));
    ASSERT_FALSE(isFirstElementTextScoreMeta("{a: {$meta: \"image\"}}"));
    ASSERT_FALSE(isFirstElementTextScoreMeta("{a: {$world: \"textScore\"}}"));
    ASSERT_FALSE(isFirstElementTextScoreMeta("{a: {$meta: \"textScore\", b: 1}}"));
}

//
// Tests for parsing a query request from a command BSON object.
//

TEST(QueryRequestTest, ParseFromCommandBasic) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter: {a: 3},"
        "sort: {a: 1},"
        "projection: {_id: 0, a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandWithOptions) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter: {a: 3},"
        "sort: {a: 1},"
        "projection: {_id: 0, a: 1},"
        "showRecordId: true}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    // Make sure the values from the command BSON are reflected in the QR.
    ASSERT(qr->showRecordId());
}

TEST(QueryRequestTest, ParseFromCommandHintAsString) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "hint: 'foo_1'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    BSONObj hintObj = qr->getHint();
    ASSERT_BSONOBJ_EQ(BSON("$hint"
                           << "foo_1"),
                      hintObj);
}

TEST(QueryRequestTest, ParseFromCommandValidSortProj) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "projection: {a: 1},"
        "sort: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    ASSERT_OK(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain).getStatus());
}

TEST(QueryRequestTest, ParseFromCommandValidSortProjMeta) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "projection: {a: {$meta: 'textScore'}},"
        "sort: {a: {$meta: 'textScore'}}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    ASSERT_OK(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain).getStatus());
}

TEST(QueryRequestTest, ParseFromCommandAllFlagsTrue) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "tailable: true,"
        "oplogReplay: true,"
        "noCursorTimeout: true,"
        "awaitData: true,"
        "allowPartialResults: true,"
        "readOnce: true,"
        "_use44SortKeys: true,"
        "allowSpeculativeMajorityRead: true}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    // Test that all the flags got set to true.
    ASSERT(qr->isTailable());
    ASSERT(!qr->isSlaveOk());
    ASSERT(qr->isOplogReplay());
    ASSERT(qr->isNoCursorTimeout());
    ASSERT(qr->isTailableAndAwaitData());
    ASSERT(qr->isAllowPartialResults());
    ASSERT(qr->isReadOnce());
    ASSERT(qr->use44SortKeys());
    ASSERT(qr->allowSpeculativeMajorityRead());
}

TEST(QueryRequestTest, ParseFromCommandReadOnceDefaultsToFalse) {
    BSONObj cmdObj = fromjson("{find: 'testns'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT(!qr->isReadOnce());
}

TEST(QueryRequestTest, ParseFromCommandUse44SortKeysDefaultsToFalse) {
    BSONObj cmdObj = fromjson("{find: 'testns'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT(!qr->use44SortKeys());
}

TEST(QueryRequestTest, ParseFromCommandValidMinMax) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "comment: 'the comment',"
        "min: {a: 1},"
        "max: {a: 2}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    BSONObj expectedMin = BSON("a" << 1);
    ASSERT_EQUALS(0, expectedMin.woCompare(qr->getMin()));
    BSONObj expectedMax = BSON("a" << 2);
    ASSERT_EQUALS(0, expectedMax.woCompare(qr->getMax()));
}

TEST(QueryRequestTest, ParseFromCommandAllNonOptionFields) {
    RuntimeConstants rtc{Date_t::now(), Timestamp(1, 1)};
    BSONObj rtcObj = BSON("runtimeConstants" << rtc.toBSON());
    BSONObj cmdObj = fromjson(
                         "{find: 'testns',"
                         "filter: {a: 1},"
                         "sort: {b: 1},"
                         "projection: {c: 1},"
                         "hint: {d: 1},"
                         "readConcern: {e: 1},"
                         "$queryOptions: {$readPreference: 'secondary'},"
                         "collation: {f: 1},"
                         "limit: 3,"
                         "skip: 5,"
                         "batchSize: 90,"
                         "singleBatch: false}")
                         .addField(rtcObj["runtimeConstants"]);
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    // Check the values inside the QR.
    BSONObj expectedQuery = BSON("a" << 1);
    ASSERT_EQUALS(0, expectedQuery.woCompare(qr->getFilter()));
    BSONObj expectedSort = BSON("b" << 1);
    ASSERT_EQUALS(0, expectedSort.woCompare(qr->getSort()));
    BSONObj expectedProj = BSON("c" << 1);
    ASSERT_EQUALS(0, expectedProj.woCompare(qr->getProj()));
    BSONObj expectedHint = BSON("d" << 1);
    ASSERT_EQUALS(0, expectedHint.woCompare(qr->getHint()));
    BSONObj expectedReadConcern = BSON("e" << 1);
    ASSERT_EQUALS(0, expectedReadConcern.woCompare(qr->getReadConcern()));
    BSONObj expectedUnwrappedReadPref = BSON("$readPreference"
                                             << "secondary");
    ASSERT_EQUALS(0, expectedUnwrappedReadPref.woCompare(qr->getUnwrappedReadPref()));
    BSONObj expectedCollation = BSON("f" << 1);
    ASSERT_EQUALS(0, expectedCollation.woCompare(qr->getCollation()));
    ASSERT_EQUALS(3, *qr->getLimit());
    ASSERT_EQUALS(5, *qr->getSkip());
    ASSERT_EQUALS(90, *qr->getBatchSize());
    ASSERT(qr->getRuntimeConstants().has_value());
    ASSERT_EQUALS(qr->getRuntimeConstants()->getLocalNow(), rtc.getLocalNow());
    ASSERT_EQUALS(qr->getRuntimeConstants()->getClusterTime(), rtc.getClusterTime());
    ASSERT(qr->wantMore());
}

TEST(QueryRequestTest, ParseFromCommandLargeLimit) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter: {a: 1},"
        "limit: 8000000000}");  // 8 * 1000 * 1000 * 1000
    const NamespaceString nss("test.testns");
    const bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    ASSERT_EQUALS(8LL * 1000 * 1000 * 1000, *qr->getLimit());
}

TEST(QueryRequestTest, ParseFromCommandLargeBatchSize) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter: {a: 1},"
        "batchSize: 8000000000}");  // 8 * 1000 * 1000 * 1000
    const NamespaceString nss("test.testns");
    const bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    ASSERT_EQUALS(8LL * 1000 * 1000 * 1000, *qr->getBatchSize());
}

TEST(QueryRequestTest, ParseFromCommandLargeSkip) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter: {a: 1},"
        "skip: 8000000000}");  // 8 * 1000 * 1000 * 1000
    const NamespaceString nss("test.testns");
    const bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    ASSERT_EQUALS(8LL * 1000 * 1000 * 1000, *qr->getSkip());
}

//
// Parsing errors where a field has the wrong type.
//

TEST(QueryRequestTest, ParseFromCommandQueryWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandSortWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "sort: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandProjWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "projection: 'foo'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandSkipWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "skip: '5',"
        "projection: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}


TEST(QueryRequestTest, ParseFromCommandLimitWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "limit: '5',"
        "projection: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandSingleBatchWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "singleBatch: 'false',"
        "projection: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandUnwrappedReadPrefWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "$queryOptions: 1}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandMaxTimeMSWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "maxTimeMS: true}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandMaxWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "max: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandMinWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "min: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandReturnKeyWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "returnKey: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}


TEST(QueryRequestTest, ParseFromCommandShowRecordIdWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "showRecordId: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandTailableWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "tailable: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandSlaveOkWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "slaveOk: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandOplogReplayWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "oplogReplay: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandNoCursorTimeoutWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "noCursorTimeout: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandAwaitDataWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "tailable: true,"
        "awaitData: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandExhaustWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "exhaust: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandPartialWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "allowPartialResults: 3}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandReadConcernWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "readConcern: 'foo'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandCollationWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "filter:  {a: 1},"
        "collation: 'foo'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandReadOnceWrongType) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "readOnce: 1}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_EQ(ErrorCodes::FailedToParse, result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandRuntimeConstantsWrongType) {
    BSONObj cmdObj = BSON("find"
                          << "testns"
                          << "runtimeConstants"
                          << "shouldNotBeString");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_EQ(ErrorCodes::FailedToParse, result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandRuntimeConstantsSubfieldsWrongType) {
    BSONObj cmdObj = BSON("find"
                          << "testns"
                          << "runtimeConstants"
                          << BSON("localNow"
                                  << "shouldBeDate"
                                  << "clusterTime"
                                  << "shouldBeTimestamp"));
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    ASSERT_THROWS_CODE(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain),
                       AssertionException,
                       ErrorCodes::TypeMismatch);
}

TEST(QueryRequestTest, ParseFromCommandUse44SortKeysWrongType) {
    BSONObj cmdObj = BSON("find"
                          << "testns"
                          << "_use44SortKeys"
                          << "shouldBeBool");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

//
// Parsing errors where a field has the right type but a bad value.
//

TEST(QueryRequestTest, ParseFromCommandNegativeSkipError) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "skip: -3,"
        "filter: {a: 3}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandSkipIsZero) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "skip: 0,"
        "filter: {a: 3}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT_BSONOBJ_EQ(BSON("a" << 3), qr->getFilter());
    ASSERT_FALSE(qr->getSkip());
}

TEST(QueryRequestTest, ParseFromCommandNegativeLimitError) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "limit: -3,"
        "filter: {a: 3}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandLimitIsZero) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "limit: 0,"
        "filter: {a: 3}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT_BSONOBJ_EQ(BSON("a" << 3), qr->getFilter());
    ASSERT_FALSE(qr->getLimit());
}

TEST(QueryRequestTest, ParseFromCommandNegativeBatchSizeError) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "batchSize: -10,"
        "filter: {a: 3}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandBatchSizeZero) {
    BSONObj cmdObj = fromjson("{find: 'testns', batchSize: 0}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    ASSERT(qr->getBatchSize());
    ASSERT_EQ(0, *qr->getBatchSize());

    ASSERT(!qr->getLimit());
}

TEST(QueryRequestTest, ParseFromCommandDefaultBatchSize) {
    BSONObj cmdObj = fromjson("{find: 'testns'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    ASSERT(!qr->getBatchSize());
    ASSERT(!qr->getLimit());
}

TEST(QueryRequestTest, ParseFromCommandRequestResumeToken) {
    BSONObj cmdObj = BSON("find"
                          << "testns"
                          << "hint" << BSON("$natural" << 1) << "sort" << BSON("$natural" << 1)
                          << "$_requestResumeToken" << true);
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT(qr->getRequestResumeToken());
}

TEST(QueryRequestTest, ParseFromCommandResumeToken) {
    BSONObj cmdObj =
        BSON("find"
             << "testns"
             << "hint" << BSON("$natural" << 1) << "sort" << BSON("$natural" << 1)
             << "$_requestResumeToken" << true << "$_resumeAfter" << BSON("$recordId" << 1LL));
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT(!qr->getResumeAfter().isEmpty());
    ASSERT(qr->getRequestResumeToken());
}

TEST(QueryRequestTest, ParseFromCommandEmptyResumeToken) {
    BSONObj resumeAfter = fromjson("{}");
    BSONObj cmdObj = BSON("find"
                          << "testns"
                          << "hint" << BSON("$natural" << 1) << "sort" << BSON("$natural" << 1)
                          << "$_requestResumeToken" << true << "$_resumeAfter" << resumeAfter);
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT(qr->getRequestResumeToken());
    ASSERT(qr->getResumeAfter().isEmpty());
}

//
// Test asFindCommand ns and uuid variants.
//

TEST(QueryRequestTest, AsFindCommandAllNonOptionFields) {
    BSONObj rtcObj =
        BSON("runtimeConstants" << (RuntimeConstants{Date_t::now(), Timestamp(1, 1)}.toBSON()));
    BSONObj cmdObj = fromjson(
                         "{find: 'testns',"
                         "filter: {a: 1},"
                         "projection: {c: 1},"
                         "sort: {b: 1},"
                         "hint: {d: 1},"
                         "readConcern: {e: 1},"
                         "collation: {f: 1},"
                         "skip: 5,"
                         "limit: 3,"
                         "batchSize: 90,"
                         "singleBatch: true}")
                         .addField(rtcObj["runtimeConstants"]);
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT_BSONOBJ_EQ(cmdObj, qr->asFindCommand());
}

TEST(QueryRequestTest, AsFindCommandWithUuidAllNonOptionFields) {
    BSONObj rtcObj =
        BSON("runtimeConstants" << (RuntimeConstants{Date_t::now(), Timestamp(1, 1)}.toBSON()));
    BSONObj cmdObj =
        fromjson(
            // This binary value is UUID("01234567-89ab-cdef-edcb-a98765432101")
            "{find: { \"$binary\" : \"ASNFZ4mrze/ty6mHZUMhAQ==\", \"$type\" : \"04\" },"
            "filter: {a: 1},"
            "projection: {c: 1},"
            "sort: {b: 1},"
            "hint: {d: 1},"
            "readConcern: {e: 1},"
            "collation: {f: 1},"
            "skip: 5,"
            "limit: 3,"
            "batchSize: 90,"
            "singleBatch: true}")
            .addField(rtcObj["runtimeConstants"]);
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT_BSONOBJ_EQ(cmdObj, qr->asFindCommandWithUuid());
}

TEST(QueryRequestTest, AsFindCommandWithUuidNoAvailableNamespace) {
    BSONObj cmdObj =
        fromjson("{find: { \"$binary\" : \"ASNFZ4mrze/ty6mHZUMhAQ==\", \"$type\" : \"04\" }}");
    QueryRequest qr(NamespaceStringOrUUID(
        "test", UUID::parse("01234567-89ab-cdef-edcb-a98765432101").getValue()));
    ASSERT_BSONOBJ_EQ(cmdObj, qr.asFindCommandWithUuid());
}

TEST(QueryRequestTest, AsFindCommandWithResumeToken) {
    BSONObj cmdObj =
        BSON("find"
             << "testns"
             << "sort" << BSON("$natural" << 1) << "hint" << BSON("$natural" << 1)
             << "$_requestResumeToken" << true << "$_resumeAfter" << BSON("$recordId" << 1LL));
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT_BSONOBJ_EQ(cmdObj, qr->asFindCommand());
}

TEST(QueryRequestTest, AsFindCommandWithEmptyResumeToken) {
    BSONObj resumeAfter = fromjson("{}");
    BSONObj cmdObj = BSON("find"
                          << "testns"
                          << "hint" << BSON("$natural" << 1) << "sort" << BSON("$natural" << 1)
                          << "$_requestResumeToken" << true << "$_resumeAfter" << resumeAfter);
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));
    ASSERT(qr->asFindCommand().getField("$_resumeAfter").eoo());
}

//
//
// Errors checked in QueryRequest::validate().
//

TEST(QueryRequestTest, ParseFromCommandMinMaxDifferentFieldsError) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "min: {a: 3},"
        "max: {b: 4}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseCommandAllowNonMetaSortOnFieldWithMetaProject) {
    BSONObj cmdObj;

    cmdObj = fromjson(
        "{find: 'testns',"
        "projection: {a: {$meta: 'textScore'}},"
        "sort: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_OK(result.getStatus());

    cmdObj = fromjson(
        "{find: 'testns',"
        "projection: {a: {$meta: 'textScore'}},"
        "sort: {b: 1}}");
    ASSERT_OK(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain).getStatus());
}

TEST(QueryRequestTest, ParseCommandAllowMetaSortOnFieldWithoutMetaProject) {
    BSONObj cmdObj;

    cmdObj = fromjson(
        "{find: 'testns',"
        "projection: {a: 1},"
        "sort: {a: {$meta: 'textScore'}}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_OK(result.getStatus());

    cmdObj = fromjson(
        "{find: 'testns',"
        "projection: {b: 1},"
        "sort: {a: {$meta: 'textScore'}}}");
    result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseCommandForbidExhaust) {
    BSONObj cmdObj = fromjson("{find: 'testns', exhaust: true}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseCommandIsFromFindCommand) {
    BSONObj cmdObj = fromjson("{find: 'testns'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain)));

    ASSERT_FALSE(qr->getNToReturn());
}

TEST(QueryRequestTest, ParseCommandAwaitDataButNotTailable) {
    const NamespaceString nss("test.testns");
    BSONObj cmdObj = fromjson("{find: 'testns', awaitData: true}");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseCommandFirstFieldNotString) {
    BSONObj cmdObj = fromjson("{find: 1}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseCommandIgnoreShardVersionField) {
    BSONObj cmdObj = fromjson("{find: 'test.testns', shardVersion: 'foo'}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_OK(result.getStatus());
}

TEST(QueryRequestTest, DefaultQueryParametersCorrect) {
    BSONObj cmdObj = fromjson("{find: 'testns'}");

    const NamespaceString nss("test.testns");
    std::unique_ptr<QueryRequest> qr(
        assertGet(QueryRequest::makeFromFindCommand(nss, cmdObj, false)));

    ASSERT_FALSE(qr->getSkip());
    ASSERT_FALSE(qr->getLimit());

    ASSERT_EQUALS(true, qr->wantMore());
    ASSERT_FALSE(qr->getNToReturn());
    ASSERT_EQUALS(false, qr->isExplain());
    ASSERT_EQUALS(0, qr->getMaxTimeMS());
    ASSERT_EQUALS(false, qr->returnKey());
    ASSERT_EQUALS(false, qr->showRecordId());
    ASSERT_EQUALS(false, qr->hasReadPref());
    ASSERT_EQUALS(false, qr->isTailable());
    ASSERT_EQUALS(false, qr->isSlaveOk());
    ASSERT_EQUALS(false, qr->isOplogReplay());
    ASSERT_EQUALS(false, qr->isNoCursorTimeout());
    ASSERT_EQUALS(false, qr->isTailableAndAwaitData());
    ASSERT_EQUALS(false, qr->isExhaust());
    ASSERT_EQUALS(false, qr->isAllowPartialResults());
    ASSERT_EQUALS(false, qr->getRuntimeConstants().has_value());
    ASSERT_EQUALS(false, qr->allowDiskUse());
}

TEST(QueryRequestTest, ParseCommandAllowDiskUseTrue) {
    BSONObj cmdObj = fromjson("{find: 'testns', allowDiskUse: true}");
    const NamespaceString nss("test.testns");
    const bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);

    ASSERT_OK(result.getStatus());
    ASSERT_EQ(true, result.getValue()->allowDiskUse());
}

TEST(QueryRequestTest, ParseCommandAllowDiskUseFalse) {
    BSONObj cmdObj = fromjson("{find: 'testns', allowDiskUse: false}");
    const NamespaceString nss("test.testns");
    const bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);

    ASSERT_OK(result.getStatus());
    ASSERT_EQ(false, result.getValue()->allowDiskUse());
}

//
// Extra fields cause the parse to fail.
//

TEST(QueryRequestTest, ParseFromCommandForbidExtraField) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "foo: {a: 1}}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseFromCommandForbidExtraOption) {
    BSONObj cmdObj = fromjson(
        "{find: 'testns',"
        "foo: true}");
    const NamespaceString nss("test.testns");
    bool isExplain = false;
    auto result = QueryRequest::makeFromFindCommand(nss, cmdObj, isExplain);
    ASSERT_NOT_OK(result.getStatus());
}

TEST(QueryRequestTest, ParseMaxTimeMSStringValueFails) {
    BSONObj maxTimeObj = BSON(QueryRequest::cmdOptionMaxTimeMS << "foo");
    ASSERT_NOT_OK(QueryRequest::parseMaxTimeMS(maxTimeObj[QueryRequest::cmdOptionMaxTimeMS]));
}

TEST(QueryRequestTest, ParseMaxTimeMSNonIntegralValueFails) {
    BSONObj maxTimeObj = BSON(QueryRequest::cmdOptionMaxTimeMS << 100.3);
    ASSERT_NOT_OK(QueryRequest::parseMaxTimeMS(maxTimeObj[QueryRequest::cmdOptionMaxTimeMS]));
}

TEST(QueryRequestTest, ParseMaxTimeMSOutOfRangeDoubleFails) {
    BSONObj maxTimeObj = BSON(QueryRequest::cmdOptionMaxTimeMS << 1e200);
    ASSERT_NOT_OK(QueryRequest::parseMaxTimeMS(maxTimeObj[QueryRequest::cmdOptionMaxTimeMS]));
}

TEST(QueryRequestTest, ParseMaxTimeMSNegativeValueFails) {
    BSONObj maxTimeObj = BSON(QueryRequest::cmdOptionMaxTimeMS << -400);
    ASSERT_NOT_OK(QueryRequest::parseMaxTimeMS(maxTimeObj[QueryRequest::cmdOptionMaxTimeMS]));
}

TEST(QueryRequestTest, ParseMaxTimeMSZeroSucceeds) {
    BSONObj maxTimeObj = BSON(QueryRequest::cmdOptionMaxTimeMS << 0);
    auto maxTime = QueryRequest::parseMaxTimeMS(maxTimeObj[QueryRequest::cmdOptionMaxTimeMS]);
    ASSERT_OK(maxTime);
    ASSERT_EQ(maxTime.getValue(), 0);
}

TEST(QueryRequestTest, ParseMaxTimeMSPositiveInRangeSucceeds) {
    BSONObj maxTimeObj = BSON(QueryRequest::cmdOptionMaxTimeMS << 300);
    auto maxTime = QueryRequest::parseMaxTimeMS(maxTimeObj[QueryRequest::cmdOptionMaxTimeMS]);
    ASSERT_OK(maxTime);
    ASSERT_EQ(maxTime.getValue(), 300);
}

TEST(QueryRequestTest, ConvertToAggregationSucceeds) {
    QueryRequest qr(testns);
    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    auto ar = AggregationRequest::parseFromBSON(testns, agg.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT(!ar.getValue().getExplain());
    ASSERT(ar.getValue().getPipeline().empty());
    ASSERT_EQ(ar.getValue().getBatchSize(), AggregationRequest::kDefaultBatchSize);
    ASSERT_EQ(ar.getValue().getNamespaceString(), testns);
    ASSERT_BSONOBJ_EQ(ar.getValue().getCollation(), BSONObj());
}

TEST(QueryRequestTest, ConvertToAggregationOmitsExplain) {
    QueryRequest qr(testns);
    qr.setExplain(true);
    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    auto ar = AggregationRequest::parseFromBSON(testns, agg.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT_FALSE(ar.getValue().getExplain());
    ASSERT(ar.getValue().getPipeline().empty());
    ASSERT_EQ(ar.getValue().getNamespaceString(), testns);
    ASSERT_BSONOBJ_EQ(ar.getValue().getCollation(), BSONObj());
}

TEST(QueryRequestTest, ConvertToAggregationWithHintSucceeds) {
    QueryRequest qr(testns);
    qr.setHint(fromjson("{a_1: -1}"));
    const auto aggCmd = qr.asAggregationCommand();
    ASSERT_OK(aggCmd);

    auto ar = AggregationRequest::parseFromBSON(testns, aggCmd.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT_BSONOBJ_EQ(qr.getHint(), ar.getValue().getHint());
}

TEST(QueryRequestTest, ConvertToAggregationWithMinFails) {
    QueryRequest qr(testns);
    qr.setMin(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithMaxFails) {
    QueryRequest qr(testns);
    qr.setMax(fromjson("{a: 1}"));
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithNoWantMoreFails) {
    QueryRequest qr(testns);
    qr.setWantMore(false);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithNoWantMoreAndLimitFails) {
    QueryRequest qr(testns);
    qr.setWantMore(false);
    qr.setLimit(7);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithNoWantMoreLimitOneSucceeds) {
    QueryRequest qr(testns);
    qr.setWantMore(false);
    qr.setLimit(1);
    ASSERT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithReturnKeyFails) {
    QueryRequest qr(testns);
    qr.setReturnKey(true);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithShowRecordIdFails) {
    QueryRequest qr(testns);
    qr.setShowRecordId(true);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithTailableFails) {
    QueryRequest qr(testns);
    qr.setTailableMode(TailableModeEnum::kTailable);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithOplogReplayFails) {
    QueryRequest qr(testns);
    qr.setOplogReplay(true);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithNoCursorTimeoutFails) {
    QueryRequest qr(testns);
    qr.setNoCursorTimeout(true);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithAwaitDataFails) {
    QueryRequest qr(testns);
    qr.setTailableMode(TailableModeEnum::kTailableAndAwaitData);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithAllowPartialResultsFails) {
    QueryRequest qr(testns);
    qr.setAllowPartialResults(true);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithNToReturnFails) {
    QueryRequest qr(testns);
    qr.setNToReturn(7);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithRequestResumeTokenFails) {
    QueryRequest qr(testns);
    qr.setRequestResumeToken(true);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithResumeAfterFails) {
    QueryRequest qr(testns);
    BSONObj resumeAfter = BSON("$recordId" << 1LL);
    qr.setResumeAfter(resumeAfter);
    ASSERT_NOT_OK(qr.asAggregationCommand());
}

TEST(QueryRequestTest, ConvertToAggregationWithPipeline) {
    QueryRequest qr(testns);
    qr.setFilter(BSON("x" << 1));
    qr.setSort(BSON("y" << -1));
    qr.setLimit(3);
    qr.setSkip(7);
    qr.setProj(BSON("z" << 0));

    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    auto ar = AggregationRequest::parseFromBSON(testns, agg.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT(!ar.getValue().getExplain());
    ASSERT_EQ(ar.getValue().getBatchSize(), AggregationRequest::kDefaultBatchSize);
    ASSERT_EQ(ar.getValue().getNamespaceString(), testns);
    ASSERT_BSONOBJ_EQ(ar.getValue().getCollation(), BSONObj());

    std::vector<BSONObj> expectedPipeline{BSON("$match" << BSON("x" << 1)),
                                          BSON("$sort" << BSON("y" << -1)),
                                          BSON("$skip" << 7),
                                          BSON("$limit" << 3),
                                          BSON("$project" << BSON("z" << 0))};
    ASSERT(std::equal(expectedPipeline.begin(),
                      expectedPipeline.end(),
                      ar.getValue().getPipeline().begin(),
                      SimpleBSONObjComparator::kInstance.makeEqualTo()));
}

TEST(QueryRequestTest, ConvertToAggregationWithBatchSize) {
    QueryRequest qr(testns);
    qr.setBatchSize(4);

    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    auto ar = AggregationRequest::parseFromBSON(testns, agg.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT(!ar.getValue().getExplain());
    ASSERT_EQ(ar.getValue().getNamespaceString(), testns);
    ASSERT_EQ(ar.getValue().getBatchSize(), 4LL);
    ASSERT_BSONOBJ_EQ(ar.getValue().getCollation(), BSONObj());
}

TEST(QueryRequestTest, ConvertToAggregationWithMaxTimeMS) {
    QueryRequest qr(testns);
    qr.setMaxTimeMS(9);

    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    const BSONObj cmdObj = agg.getValue();
    ASSERT_EQ(cmdObj["maxTimeMS"].Int(), 9);

    auto ar = AggregationRequest::parseFromBSON(testns, cmdObj);
    ASSERT_OK(ar.getStatus());
    ASSERT(!ar.getValue().getExplain());
    ASSERT_EQ(ar.getValue().getBatchSize(), AggregationRequest::kDefaultBatchSize);
    ASSERT_EQ(ar.getValue().getNamespaceString(), testns);
    ASSERT_BSONOBJ_EQ(ar.getValue().getCollation(), BSONObj());
}

TEST(QueryRequestTest, ConvertToAggregationWithCollationSucceeds) {
    QueryRequest qr(testns);
    qr.setCollation(BSON("f" << 1));
    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    auto ar = AggregationRequest::parseFromBSON(testns, agg.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT(!ar.getValue().getExplain());
    ASSERT(ar.getValue().getPipeline().empty());
    ASSERT_EQ(ar.getValue().getBatchSize(), AggregationRequest::kDefaultBatchSize);
    ASSERT_EQ(ar.getValue().getNamespaceString(), testns);
    ASSERT_BSONOBJ_EQ(ar.getValue().getCollation(), BSON("f" << 1));
}

TEST(QueryRequestTest, ConvertToAggregationWithReadOnceFails) {
    QueryRequest qr(testns);
    qr.setReadOnce(true);
    const auto aggCmd = qr.asAggregationCommand();
    ASSERT_EQ(ErrorCodes::InvalidPipelineOperator, aggCmd.getStatus().code());
}

TEST(QueryRequestTest, ConvertToAggregationWithAllowSpeculativeMajorityReadFails) {
    QueryRequest qr(testns);
    qr.setAllowSpeculativeMajorityRead(true);
    const auto aggCmd = qr.asAggregationCommand();
    ASSERT_EQ(ErrorCodes::InvalidPipelineOperator, aggCmd.getStatus().code());
}

TEST(QueryRequestTest, ConvertToAggregationWithRuntimeConstantsSucceeds) {
    RuntimeConstants rtc{Date_t::now(), Timestamp(1, 1)};
    QueryRequest qr(testns);
    qr.setRuntimeConstants(rtc);
    auto agg = qr.asAggregationCommand();
    ASSERT_OK(agg);

    auto ar = AggregationRequest::parseFromBSON(testns, agg.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT(ar.getValue().getRuntimeConstants().has_value());
    ASSERT_EQ(ar.getValue().getRuntimeConstants()->getLocalNow(), rtc.getLocalNow());
    ASSERT_EQ(ar.getValue().getRuntimeConstants()->getClusterTime(), rtc.getClusterTime());
}

TEST(QueryRequestTest, ConvertToAggregationWithAllowDiskUseTrueSucceeds) {
    QueryRequest qr(testns);
    qr.setAllowDiskUse(true);
    const auto aggCmd = qr.asAggregationCommand();
    ASSERT_OK(aggCmd.getStatus());

    auto ar = AggregationRequest::parseFromBSON(testns, aggCmd.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT_EQ(true, ar.getValue().shouldAllowDiskUse());
}

TEST(QueryRequestTest, ConvertToAggregationWithAllowDiskUseFalseSucceeds) {
    QueryRequest qr(testns);
    qr.setAllowDiskUse(false);
    const auto aggCmd = qr.asAggregationCommand();
    ASSERT_OK(aggCmd.getStatus());

    auto ar = AggregationRequest::parseFromBSON(testns, aggCmd.getValue());
    ASSERT_OK(ar.getStatus());
    ASSERT_EQ(false, ar.getValue().shouldAllowDiskUse());
}

TEST(QueryRequestTest, ConvertToFindWithAllowDiskUseTrueSucceeds) {
    QueryRequest qr(testns);
    qr.setAllowDiskUse(true);
    const auto findCmd = qr.asFindCommand();

    BSONElement elem = findCmd[QueryRequest::kAllowDiskUseField];
    ASSERT_EQ(true, elem.isBoolean());
    ASSERT_EQ(true, elem.Bool());
}

TEST(QueryRequestTest, ConvertToFindWithAllowDiskUseFalseSucceeds) {
    QueryRequest qr(testns);
    qr.setAllowDiskUse(false);
    const auto findCmd = qr.asFindCommand();

    ASSERT_FALSE(findCmd[QueryRequest::kAllowDiskUseField]);
}

TEST(QueryRequestTest, ParseFromLegacyQuery) {
    const auto kSkip = 1;
    const auto kNToReturn = 2;

    BSONObj queryObj = fromjson(R"({
            query: {query: 1},
            orderby: {sort: 1},
            $hint: {hint: 1},
            $explain: false,
            $min: {x: 'min'},
            $max: {x: 'max'}
         })");
    const NamespaceString nss("test.testns");
    unique_ptr<QueryRequest> qr(assertGet(QueryRequest::fromLegacyQuery(
        nss, queryObj, BSON("proj" << 1), kSkip, kNToReturn, QueryOption_Exhaust)));

    ASSERT_EQ(qr->nss(), nss);
    ASSERT_BSONOBJ_EQ(qr->getFilter(), fromjson("{query: 1}"));
    ASSERT_BSONOBJ_EQ(qr->getProj(), fromjson("{proj: 1}"));
    ASSERT_BSONOBJ_EQ(qr->getSort(), fromjson("{sort: 1}"));
    ASSERT_BSONOBJ_EQ(qr->getHint(), fromjson("{hint: 1}"));
    ASSERT_BSONOBJ_EQ(qr->getMin(), fromjson("{x: 'min'}"));
    ASSERT_BSONOBJ_EQ(qr->getMax(), fromjson("{x: 'max'}"));
    ASSERT_EQ(qr->getSkip(), boost::optional<long long>(kSkip));
    ASSERT_EQ(qr->getNToReturn(), boost::optional<long long>(kNToReturn));
    ASSERT_EQ(qr->wantMore(), true);
    ASSERT_EQ(qr->isExplain(), false);
    ASSERT_EQ(qr->isSlaveOk(), false);
    ASSERT_EQ(qr->isOplogReplay(), false);
    ASSERT_EQ(qr->isNoCursorTimeout(), false);
    ASSERT_EQ(qr->isTailable(), false);
    ASSERT_EQ(qr->isExhaust(), true);
    ASSERT_EQ(qr->isAllowPartialResults(), false);
    ASSERT_EQ(qr->getOptions(), QueryOption_Exhaust);
}

TEST(QueryRequestTest, ParseFromLegacyQueryUnwrapped) {
    BSONObj queryObj = fromjson(R"({
            foo: 1
         })");
    const NamespaceString nss("test.testns");
    unique_ptr<QueryRequest> qr(assertGet(
        QueryRequest::fromLegacyQuery(nss, queryObj, BSONObj(), 0, 0, QueryOption_Exhaust)));

    ASSERT_EQ(qr->nss(), nss);
    ASSERT_BSONOBJ_EQ(qr->getFilter(), fromjson("{foo: 1}"));
}

TEST(QueryRequestTest, ParseFromLegacyQueryTooNegativeNToReturn) {
    BSONObj queryObj = fromjson(R"({
            foo: 1
         })");
    const NamespaceString nss("test.testns");

    ASSERT_NOT_OK(
        QueryRequest::fromLegacyQuery(
            nss, queryObj, BSONObj(), 0, std::numeric_limits<int>::min(), QueryOption_Exhaust)
            .getStatus());
}

class QueryRequestTest : public ServiceContextTest {};

TEST_F(QueryRequestTest, ParseFromUUID) {
    auto opCtx = makeOperationContext();
    // Register a UUID/Collection pair in the CollectionCatalog.
    const CollectionUUID uuid = UUID::gen();
    const NamespaceString nss("test.testns");
    std::unique_ptr<Collection> collection = std::make_unique<CollectionMock>(nss);
    CollectionCatalog& catalog = CollectionCatalog::get(opCtx.get());
    catalog.registerCollection(uuid, &collection);
    QueryRequest qr(NamespaceStringOrUUID("test", uuid));
    // Ensure a call to refreshNSS succeeds.
    qr.refreshNSS(opCtx.get());
    ASSERT_EQ(nss, qr.nss());
}

}  // namespace
}  // namespace mongo
