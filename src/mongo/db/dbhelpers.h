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

#include "mongo/db/namespace_string.h"
#include "mongo/db/record_id.h"

namespace mongo {

class Collection;
class Database;
class OperationContext;
class QueryRequest;

/**
 * db helpers are helper functions and classes that let us easily manipulate the local
 * database instance in-proc.
 *
 * all helpers assume locking is handled above them
 */
struct Helpers {

    /* fetch a single object from collection ns that matches query.
       set your db SavedContext first.

       @param query - the query to perform.  note this is the low level portion of query so
                      "orderby : ..." won't work.

       @param requireIndex if true, assert if no index for the query.  a way to guard against
       writing a slow query.

       @return true if object found
    */
    static bool findOne(OperationContext* opCtx,
                        Collection* collection,
                        const BSONObj& query,
                        BSONObj& result,
                        bool requireIndex = false);

    static RecordId findOne(OperationContext* opCtx,
                            Collection* collection,
                            const BSONObj& query,
                            bool requireIndex);

    static RecordId findOne(OperationContext* opCtx,
                            Collection* collection,
                            std::unique_ptr<QueryRequest> qr,
                            bool requireIndex);

    /**
     * @param foundIndex if passed in will be set to 1 if ns and index found
     * @return true if object found
     */
    static bool findById(OperationContext* opCtx,
                         Database* db,
                         StringData ns,
                         BSONObj query,
                         BSONObj& result,
                         bool* nsFound = nullptr,
                         bool* indexFound = nullptr);

    /* TODO: should this move into Collection?
     * uasserts if no _id index.
     * @return null loc if not found */
    static RecordId findById(OperationContext* opCtx, Collection* collection, const BSONObj& query);

    /**
     * Get the first object generated from a forward natural-order scan on "ns".  Callers do not
     * have to lock "ns".
     *
     * Returns true if there is such an object.  An owned copy of the object is placed into the
     * out-argument "result".
     *
     * Returns false if there is no such object.
     */
    static bool getSingleton(OperationContext* opCtx, const char* ns, BSONObj& result);

    /**
     * Same as getSingleton, but with a reverse natural-order scan on "ns".
     */
    static bool getLast(OperationContext* opCtx, const char* ns, BSONObj& result);

    /**
     * Performs an upsert of "obj" into the collection "ns", with an empty update predicate.
     * Callers must have "ns" locked.
     */
    static void putSingleton(OperationContext* opCtx, const char* ns, BSONObj obj);

    /**
     * you have to lock
     * you do not have to have Context set
     * o has to have an _id field or will assert
     */
    static void upsert(OperationContext* opCtx,
                       const std::string& ns,
                       const BSONObj& o,
                       bool fromMigrate = false);

    /**
     * Performs an upsert of 'updateMod' if we don't match the given 'filter'.
     * Callers are expected to hold the collection lock.
     * Note: Query yielding is turned off, so both read and writes are performed
     * on the same storage snapshot.
     */
    static void upsert(OperationContext* opCtx,
                       const std::string& ns,
                       const BSONObj& filter,
                       const BSONObj& updateMod,
                       bool fromMigrate = false);

    // TODO: this should be somewhere else probably
    /* Takes object o, and returns a new object with the
     * same field elements but the names stripped out.
     * Example:
     *    o = {a : 5 , b : 6} --> {"" : 5, "" : 6}
     */
    static BSONObj toKeyFormat(const BSONObj& o);

    /* Takes object o, and infers an ascending keyPattern with the same fields as o
     * Example:
     *    o = {a : 5 , b : 6} --> {a : 1 , b : 1 }
     */
    static BSONObj inferKeyPattern(const BSONObj& o);

    /**
     * Remove all documents from a collection.
     * You do not need to set the database before calling.
     * Does not oplog the operation.
     */
    static void emptyCollection(OperationContext* opCtx, const NamespaceString& nss);
};

}  // namespace mongo
