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

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/status.h"
#include "mongo/base/status_with.h"
#include "mongo/base/string_data.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/views/durable_view_catalog.h"
#include "mongo/db/views/resolved_view.h"
#include "mongo/db/views/view.h"
#include "mongo/db/views/view_graph.h"
#include "mongo/platform/mutex.h"
#include "mongo/util/concurrency/with_lock.h"
#include "mongo/util/string_map.h"

namespace mongo {
class OperationContext;
class Database;

/**
 * In-memory data structure for view definitions. This data structure is thread-safe -- this is
 * needed as concurrent updates may happen through direct writes to the views catalog collection.
 *
 * All public methods of the view catalog obtain the mutex and refresh the in-memory map with the
 * views catalog collection if necessary, throwing if the refresh fails.
 */
class ViewCatalog {
    ViewCatalog(const ViewCatalog&) = delete;
    ViewCatalog& operator=(const ViewCatalog&) = delete;

public:
    using ViewMap = StringMap<std::shared_ptr<ViewDefinition>>;
    using ViewIteratorCallback = std::function<void(const ViewDefinition& view)>;

    static ViewCatalog* get(const Database* db);
    static void set(Database* db, std::unique_ptr<ViewCatalog> catalog);

    explicit ViewCatalog(std::unique_ptr<DurableViewCatalog> durable)
        : _durable(std::move(durable)),
          _valid(false),
          _viewGraphNeedsRefresh(true),
          _ignoreExternalChange(false) {}

    /**
     * Iterates through the catalog, applying 'callback' to each view. This callback function
     * executes under the catalog's mutex, so it must not access other methods of the catalog,
     * acquire locks or run for a long time.
     */
    void iterate(OperationContext* opCtx, ViewIteratorCallback callback);

    /**
     * Create a new view 'viewName' with contents defined by running the specified aggregation
     * 'pipeline' with collation 'collation' on a collection or view 'viewOn'. This method will
     * check correctness with respect to the view catalog, but will not check for conflicts with the
     * database's catalog, so the check for an existing collection with the same name must be done
     * before calling createView.
     *
     * Must be in WriteUnitOfWork. View creation rolls back if the unit of work aborts.
     */
    Status createView(OperationContext* opCtx,
                      const NamespaceString& viewName,
                      const NamespaceString& viewOn,
                      const BSONArray& pipeline,
                      const BSONObj& collation);

    /**
     * Drop the view named 'viewName'.
     *
     * Must be in WriteUnitOfWork. The drop rolls back if the unit of work aborts.
     */
    Status dropView(OperationContext* opCtx, const NamespaceString& viewName);

    /**
     * Modify the view named 'viewName' to have the new 'viewOn' and 'pipeline'.
     *
     * Must be in WriteUnitOfWork. The modification rolls back if the unit of work aborts.
     */
    Status modifyView(OperationContext* opCtx,
                      const NamespaceString& viewName,
                      const NamespaceString& viewOn,
                      const BSONArray& pipeline);

    /**
     * Look up the 'nss' in the view catalog, returning a shared pointer to a View definition, or
     * nullptr if it doesn't exist.
     */
    std::shared_ptr<ViewDefinition> lookup(OperationContext* opCtx, StringData nss);

    /**
     * Same functionality as above, except this function skips validating durable views in the view
     * catalog.
     */
    std::shared_ptr<ViewDefinition> lookupWithoutValidatingDurableViews(OperationContext* opCtx,
                                                                        StringData nss);

    /**
     * Resolve the views on 'nss', transforming the pipeline appropriately. This function returns a
     * fully-resolved view definition containing the backing namespace, the resolved pipeline and
     * the collation to use for the operation.
     */
    StatusWith<ResolvedView> resolveView(OperationContext* opCtx, const NamespaceString& nss);

    /**
     * Reloads the in-memory state of the view catalog from the 'system.views' collection catalog.
     * If the 'lookupBehavior' is 'kValidateDurableViews', then the durable view definitions will be
     * validated. Reading stops on the first invalid entry with errors logged and returned. Performs
     * no cycle detection, etc.
     * This is implicitly called by other methods when write operations are performed on the view
     * catalog, on external changes to the 'system.views' collection and on the first opening of a
     * database.
     */
    Status reload(OperationContext* opCtx, ViewCatalogLookupBehavior lookupBehavior);

    /**
     * Clears the in-memory state of the view catalog.
     */
    void clear();

    /**
     * The view catalog needs to ignore external changes for its own modifications.
     */
    bool shouldIgnoreExternalChange(OperationContext* opCtx, const NamespaceString& name) const;

private:
    Status _createOrUpdateView(WithLock,
                               OperationContext* opCtx,
                               const NamespaceString& viewName,
                               const NamespaceString& viewOn,
                               const BSONArray& pipeline,
                               std::unique_ptr<CollatorInterface> collator);
    /**
     * Parses the view definition pipeline, attempts to upsert into the view graph, and refreshes
     * the graph if necessary. Returns an error status if the resulting graph would be invalid.
     */
    Status _upsertIntoGraph(WithLock, OperationContext* opCtx, const ViewDefinition& viewDef);

    /**
     * Returns Status::OK with the set of involved namespaces if the given pipeline is eligible to
     * act as a view definition. Otherwise, returns ErrorCodes::OptionNotSupportedOnView.
     */
    StatusWith<stdx::unordered_set<NamespaceString>> _validatePipeline(
        WithLock, OperationContext* opCtx, const ViewDefinition& viewDef) const;

    /**
     * Returns Status::OK if each view namespace in 'refs' has the same default collation as 'view'.
     * Otherwise, returns ErrorCodes::OptionNotSupportedOnView.
     */
    Status _validateCollation(WithLock,
                              OperationContext* opCtx,
                              const ViewDefinition& view,
                              const std::vector<NamespaceString>& refs);

    std::shared_ptr<ViewDefinition> _lookup(WithLock,
                                            OperationContext* opCtx,
                                            StringData ns,
                                            ViewCatalogLookupBehavior lookupBehavior);

    Status _reload(WithLock, OperationContext* opCtx, ViewCatalogLookupBehavior lookupBehavior);

    /**
     * uasserts with the InvalidViewDefinition error if the current in-memory state of the view
     * catalog is invalid. This ensures that calling into the view catalog while it is invalid
     * renders it inoperable.
     */
    void _requireValidCatalog(WithLock);

    Mutex _mutex = MONGO_MAKE_LATCH("ViewCatalog::_mutex");  // Protects all members.
    ViewMap _viewMap;
    ViewMap _viewMapBackup;
    std::unique_ptr<DurableViewCatalog> _durable;
    bool _valid;
    ViewGraph _viewGraph;
    bool _viewGraphNeedsRefresh;
    bool _ignoreExternalChange;
};
}  // namespace mongo
