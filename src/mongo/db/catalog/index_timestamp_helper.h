/**
 *    Copyright (C) 2019-present MongoDB, Inc.
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
#include "mongo/db/operation_context.h"

namespace mongo {

namespace IndexTimestampHelper {

/**
 * Returns true if writes to the catalog entry for the input namespace require being
 * timestamped. A ghost write is when the operation is not committed with an oplog entry and
 * implies the caller will look at the logical clock to choose a time to use.
 */
bool requiresGhostCommitTimestampForCatalogWrite(OperationContext* opCtx, NamespaceString nss);

/**
 * If required, sets a timestamp on an active WriteUnitOfWork. A ghost write is when the
 * operation is not committed with an oplog entry, which may be necessary for certain index
 * build operations not associated with a unique optime. This implementation uses the
 * lastApplied OpTime.
 *
 * May throw a WriteConflictException if the timestamp chosen is too old.
 */
void setGhostCommitTimestampForWrite(OperationContext* opCtx, const NamespaceString& nss);

/**
 * If required, sets a timestamp on an active WriteUnitOfWork for a catalog write. A ghost write
 * is when the operation is not committed with an oplog entry, which may be necessary for
 * certain index catalog operations not associated with a unique optime. This implementation
 * uses the LogicalClock to timestamp operations.
 * Returns true if a ghost timestamp was set, false if no timestamp was required to be set.  Can
 * also throw WriteConflictException.
 */
bool setGhostCommitTimestampForCatalogWrite(OperationContext* opCtx, const NamespaceString& nss);
};  // namespace IndexTimestampHelper

}  // namespace mongo
