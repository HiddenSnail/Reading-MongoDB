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

#pragma once

#include "mongo/db/curop.h"
#include "mongo/db/jsobj.h"
#include "mongo/db/logical_session_id.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/ops/write_ops_parsers.h"
#include "mongo/db/pipeline/runtime_constants_gen.h"
#include "mongo/db/query/explain.h"
#include "mongo/util/str.h"

namespace mongo {

class FieldRef;

class UpdateRequest {
public:
    enum ReturnDocOption {
        // Return no document.
        RETURN_NONE,

        // Return the document as it was before the update. If the update results in an insert,
        // no document will be returned.
        RETURN_OLD,

        // Return the document as it is after the update.
        RETURN_NEW
    };

    inline UpdateRequest(const NamespaceString& nsString) : _nsString(nsString) {}

    const NamespaceString& getNamespaceString() const {
        return _nsString;
    }

    inline void setQuery(const BSONObj& query) {
        _query = query;
    }

    inline const BSONObj& getQuery() const {
        return _query;
    }

    inline void setProj(const BSONObj& proj) {
        _proj = proj;
    }

    inline const BSONObj& getProj() const {
        return _proj;
    }

    inline void setSort(const BSONObj& sort) {
        _sort = sort;
    }

    inline const BSONObj& getSort() const {
        return _sort;
    }

    inline void setCollation(const BSONObj& collation) {
        _collation = collation;
    }

    inline const BSONObj& getCollation() const {
        return _collation;
    }

    inline void setUpdateModification(const write_ops::UpdateModification& updateMod) {
        _updateMod = updateMod;
    }

    inline const write_ops::UpdateModification& getUpdateModification() const {
        return _updateMod;
    }

    inline void setUpdateConstants(const boost::optional<BSONObj>& updateConstants) {
        _updateConstants = updateConstants;
    }

    inline const boost::optional<BSONObj>& getUpdateConstants() const {
        return _updateConstants;
    }

    inline void setRuntimeConstants(RuntimeConstants runtimeConstants) {
        _runtimeConstants = std::move(runtimeConstants);
    }

    inline const boost::optional<RuntimeConstants>& getRuntimeConstants() const {
        return _runtimeConstants;
    }

    inline void setArrayFilters(const std::vector<BSONObj>& arrayFilters) {
        _arrayFilters = arrayFilters;
    }

    inline const std::vector<BSONObj>& getArrayFilters() const {
        return _arrayFilters;
    }

    // Please see documentation on the private members matching these names for
    // explanations of the following fields.

    inline void setGod(bool value = true) {
        _god = value;
    }

    bool isGod() const {
        return _god;
    }

    inline void setUpsert(bool value = true) {
        _upsert = value;
    }

    bool isUpsert() const {
        return _upsert;
    }

    inline void setUpsertSuppliedDocument(bool value = true) {
        _upsertSuppliedDocument = value;
    }

    bool shouldUpsertSuppliedDocument() const {
        return _upsertSuppliedDocument;
    }

    inline void setMulti(bool value = true) {
        _multi = value;
    }

    bool isMulti() const {
        return _multi;
    }

    inline void setFromMigration(bool value = true) {
        _fromMigration = value;
    }

    bool isFromMigration() const {
        return _fromMigration;
    }

    inline void setFromOplogApplication(bool value = true) {
        _fromOplogApplication = value;
    }

    bool isFromOplogApplication() const {
        return _fromOplogApplication;
    }

    inline void setExplain(bool value = true) {
        _isExplain = value;
    }

    inline bool isExplain() const {
        return _isExplain;
    }

    inline void setReturnDocs(ReturnDocOption value) {
        _returnDocs = value;
    }

    void setHint(const BSONObj& hint) {
        _hint = hint;
    }

    BSONObj getHint() const {
        return _hint;
    }

    inline bool shouldReturnOldDocs() const {
        return _returnDocs == ReturnDocOption::RETURN_OLD;
    }

    inline bool shouldReturnNewDocs() const {
        return _returnDocs == ReturnDocOption::RETURN_NEW;
    }

    inline bool shouldReturnAnyDocs() const {
        return shouldReturnOldDocs() || shouldReturnNewDocs();
    }

    inline void setYieldPolicy(PlanExecutor::YieldPolicy yieldPolicy) {
        _yieldPolicy = yieldPolicy;
    }

    inline PlanExecutor::YieldPolicy getYieldPolicy() const {
        return _yieldPolicy;
    }

    inline void setStmtId(StmtId stmtId) {
        _stmtId = std::move(stmtId);
    }

    inline StmtId getStmtId() const {
        return _stmtId;
    }

    const std::string toString() const {
        StringBuilder builder;
        builder << " query: " << _query;
        builder << " projection: " << _proj;
        builder << " sort: " << _sort;
        builder << " collation: " << _collation;
        builder << " updateModification: " << _updateMod.toString();
        builder << " stmtId: " << _stmtId;

        builder << " arrayFilters: [";
        bool first = true;
        for (auto arrayFilter : _arrayFilters) {
            if (!first) {
                builder << ", ";
            }
            first = false;
            builder << arrayFilter;
        }
        builder << "]";

        if (_updateConstants) {
            builder << " updateConstants: " << *_updateConstants;
        }

        if (_runtimeConstants) {
            builder << " runtimeConstants: " << _runtimeConstants->toBSON().toString();
        }

        builder << " god: " << _god;
        builder << " upsert: " << _upsert;
        builder << " multi: " << _multi;
        builder << " fromMigration: " << _fromMigration;
        builder << " fromOplogApplication: " << _fromOplogApplication;
        builder << " isExplain: " << _isExplain;
        return builder.str();
    }

private:
    const NamespaceString& _nsString;

    // The hint provided, if any.  If the hint was by index key pattern, the value of '_hint' is
    // the key pattern hinted.  If the hint was by index name, the value of '_hint' is
    // {$hint: <String>}, where <String> is the index name hinted.
    BSONObj _hint;

    // Contains the query that selects documents to update.
    BSONObj _query;

    // Contains the projection information.
    BSONObj _proj;

    // Contains the sort order information.
    BSONObj _sort;

    // Contains the collation information.
    BSONObj _collation;

    // Contains the modifiers to apply to matched objects, or a replacement document.
    write_ops::UpdateModification _updateMod;

    // User-defined constant values to be used with a pipeline-style update. Those are different
    // from the '_runtimeConstants' as they can be specified by the user for each individual
    // element of the 'updates' array in the 'update' command. The '_runtimeConstants' contains
    // runtime system constant values which remain unchanged for all update statements in the
    // 'update' command.
    boost::optional<BSONObj> _updateConstants;

    // System-defined constant values which may be required by the query or update operation.
    boost::optional<RuntimeConstants> _runtimeConstants;

    // Filters to specify which array elements should be updated.
    std::vector<BSONObj> _arrayFilters;

    // The statement id of this request.
    StmtId _stmtId = kUninitializedStmtId;

    // Flags controlling the update.

    // God bypasses _id checking and index generation. It is only used on behalf of system
    // updates, never user updates.
    bool _god = false;

    // True if this should insert if no matching document is found.
    bool _upsert = false;

    // True if this upsert operation should insert the document supplied as 'c.new' if the query
    // does not match any documents.
    bool _upsertSuppliedDocument = false;

    // True if this update is allowed to affect more than one document.
    bool _multi = false;

    // True if this update is on behalf of a chunk migration.
    bool _fromMigration = false;

    // True if this update was triggered by the application of an oplog entry.
    bool _fromOplogApplication = false;

    // Whether or not we are requesting an explained update. Explained updates are read-only.
    bool _isExplain = false;

    // Specifies which version of the documents to return, if any.
    //
    //   RETURN_NONE (default): Never return any documents, old or new.
    //   RETURN_OLD: Return ADVANCED when a matching document is encountered, and the value of
    //               the document before it was updated. If there were no matches, return
    //               IS_EOF instead (even in case of an upsert).
    //   RETURN_NEW: Return ADVANCED when a matching document is encountered, and the value of
    //               the document after being updated. If an upsert was specified and it
    //               resulted in an insert, return the inserted document.
    //
    // This allows findAndModify to execute an update and retrieve the resulting document
    // without another query before or after the update.
    ReturnDocOption _returnDocs = ReturnDocOption::RETURN_NONE;

    // Whether or not the update should yield. Defaults to NO_YIELD.
    PlanExecutor::YieldPolicy _yieldPolicy = PlanExecutor::NO_YIELD;
};

}  // namespace mongo
