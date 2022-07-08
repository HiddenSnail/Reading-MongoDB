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

#include <boost/optional.hpp>
#include <string>

#include "mongo/base/owned_pointer_vector.h"
#include "mongo/bson/ordering.h"
#include "mongo/bson/timestamp.h"
#include "mongo/db/catalog/index_catalog_entry.h"
#include "mongo/db/index/multikey_paths.h"
#include "mongo/db/matcher/expression.h"
#include "mongo/db/record_id.h"
#include "mongo/db/storage/kv/kv_prefix.h"
#include "mongo/platform/atomic_word.h"
#include "mongo/platform/mutex.h"

namespace mongo {

class CollatorInterface;
class CollectionQueryInfo;
class IndexAccessMethod;
class IndexDescriptor;
class MatchExpression;
class OperationContext;
class ExpressionContext;

class IndexCatalogEntryImpl : public IndexCatalogEntry {
    IndexCatalogEntryImpl(const IndexCatalogEntryImpl&) = delete;
    IndexCatalogEntryImpl& operator=(const IndexCatalogEntryImpl&) = delete;

public:
    IndexCatalogEntryImpl(OperationContext* opCtx,
                          const std::string& ident,
                          std::unique_ptr<IndexDescriptor> descriptor,  // ownership passes to me
                          CollectionQueryInfo* queryInfo,               // not owned, optional
                          bool isFrozen);

    ~IndexCatalogEntryImpl() final;

    const NamespaceString& ns() const final;

    void init(std::unique_ptr<IndexAccessMethod> accessMethod) final;

    const std::string& getIdent() const final {
        return _ident;
    }

    IndexDescriptor* descriptor() final {
        return _descriptor.get();
    }
    const IndexDescriptor* descriptor() const final {
        return _descriptor.get();
    }

    IndexAccessMethod* accessMethod() final {
        return _accessMethod.get();
    }
    const IndexAccessMethod* accessMethod() const final {
        return _accessMethod.get();
    }

    bool isHybridBuilding() const final {
        return _indexBuildInterceptor != nullptr;
    }

    IndexBuildInterceptor* indexBuildInterceptor() final {
        return _indexBuildInterceptor;
    }

    const IndexBuildInterceptor* indexBuildInterceptor() const final {
        return _indexBuildInterceptor;
    }

    void setIndexBuildInterceptor(IndexBuildInterceptor* interceptor) final {
        _indexBuildInterceptor = interceptor;
    }

    const Ordering& ordering() const final {
        return _ordering;
    }

    const MatchExpression* getFilterExpression() const final {
        return _filterExpression.get();
    }

    const CollatorInterface* getCollator() const final {
        return _collator.get();
    }

    /// ---------------------

    void setIsReady(bool newIsReady) final;

    void setDropped() final {
        _isDropped.store(true);
    }

    bool isDropped() const final {
        return _isDropped.load();
    }

    // --

    /**
     * Returns true if this index is multikey, and returns false otherwise.
     */
    bool isMultikey() const final;

    /**
     * Returns the path components that cause this index to be multikey if this index supports
     * path-level multikey tracking, and returns an empty vector if path-level multikey tracking
     * isn't supported.
     *
     * If this index supports path-level multikey tracking but isn't multikey, then this function
     * returns a vector with size equal to the number of elements in the index key pattern where
     * each element in the vector is an empty set.
     */
    MultikeyPaths getMultikeyPaths(OperationContext* opCtx) const final;

    /**
     * Sets this index to be multikey. Information regarding which newly detected path components
     * cause this index to be multikey can also be specified.
     *
     * If this index doesn't support path-level multikey tracking, then 'multikeyPaths' is ignored.
     *
     * If this index supports path-level multikey tracking, then 'multikeyPaths' must be a vector
     * with size equal to the number of elements in the index key pattern. Additionally, at least
     * one path component of the indexed fields must cause this index to be multikey.
     *
     * If isTrackingMultikeyPathInfo() is set on the OperationContext's MultikeyPathTracker,
     * then after we confirm that we actually need to set the index as multikey, we will save the
     * namespace, index name, and multikey paths on the OperationContext rather than set the index
     * as multikey here.
     */
    void setMultikey(OperationContext* opCtx, const MultikeyPaths& multikeyPaths) final;

    // if this ready is ready for queries
    bool isReady(OperationContext* opCtx) const final;

    bool isFrozen() const final;

    KVPrefix getPrefix() const final {
        return _prefix;
    }

    /**
     * If return value is not boost::none, reads with majority read concern using an older snapshot
     * must treat this index as unfinished.
     */
    boost::optional<Timestamp> getMinimumVisibleSnapshot() final {
        return _minVisibleSnapshot;
    }

    /**
     * Updates the minimum visible snapshot. The 'newMinimumVisibleSnapshot' is ignored if it would
     * set the minimum visible snapshot backwards in time.
     */
    void setMinimumVisibleSnapshot(Timestamp newMinimumVisibleSnapshot) final;

private:
    /**
     * Sets this index to be multikey when we are running inside a multi-document transaction.
     * Used by setMultikey() only.
     */
    Status _setMultikeyInMultiDocumentTransaction(OperationContext* opCtx,
                                                  const MultikeyPaths& multikeyPaths);

    bool _catalogIsReady(OperationContext* opCtx) const;
    bool _catalogIsPresent(OperationContext* opCtx) const;

    /**
     * Retrieves the multikey information associated with this index from '_collection',
     *
     * See CollectionCatalogEntry::isIndexMultikey() for more details.
     */
    bool _catalogIsMultikey(OperationContext* opCtx, MultikeyPaths* multikeyPaths) const;

    /**
     * Sets on-disk multikey flag for this index.
     */
    void _catalogSetMultikey(OperationContext* opCtx, const MultikeyPaths& multikeyPaths);

    KVPrefix _catalogGetPrefix(OperationContext* opCtx) const;

    // -----

    const std::string _ident;

    std::unique_ptr<IndexDescriptor> _descriptor;  // owned here

    CollectionQueryInfo* _queryInfo;  // not owned here

    std::unique_ptr<IndexAccessMethod> _accessMethod;

    IndexBuildInterceptor* _indexBuildInterceptor = nullptr;  // not owned here

    std::unique_ptr<CollatorInterface> _collator;
    std::unique_ptr<MatchExpression> _filterExpression;
    // Special ExpressionContext used to evaluate the partial filter expression.
    boost::intrusive_ptr<ExpressionContext> _expCtxForFilter;

    // cached stuff

    Ordering _ordering;  // TODO: this might be b-tree specific
    bool _isReady;       // cache of NamespaceDetails info
    bool _isFrozen;
    AtomicWord<bool> _isDropped;  // Whether the index drop is committed.

    // Set to true if this index supports path-level multikey tracking.
    // '_indexTracksPathLevelMultikeyInfo' is effectively const after IndexCatalogEntry::init() is
    // called.
    bool _indexTracksPathLevelMultikeyInfo = false;

    // Set to true if this index may contain multikey data.
    AtomicWord<bool> _isMultikeyForRead;

    // Set to true after a transaction commit successfully updates multikey on the catalog data. At
    // this point, future writers do not need to update the catalog.
    AtomicWord<bool> _isMultikeyForWrite;

    // Controls concurrent access to '_indexMultikeyPaths'. We acquire this mutex rather than the
    // RESOURCE_METADATA lock as a performance optimization so that it is cheaper to detect whether
    // there is actually any path-level multikey information to update or not.
    mutable Mutex _indexMultikeyPathsMutex =
        MONGO_MAKE_LATCH("IndexCatalogEntryImpl::_indexMultikeyPathsMutex");

    // Non-empty only if '_indexTracksPathLevelMultikeyInfo' is true.
    //
    // If non-empty, '_indexMultikeyPaths' is a vector with size equal to the number of elements
    // in the index key pattern. Each element in the vector is an ordered set of positions (starting
    // at 0) into the corresponding indexed field that represent what prefixes of the indexed field
    // causes the index to be multikey.
    MultikeyPaths _indexMultikeyPaths;

    // KVPrefix used to differentiate between index entries in different logical indexes sharing the
    // same underlying sorted data interface.
    const KVPrefix _prefix;

    // The earliest snapshot that is allowed to read this index.
    boost::optional<Timestamp> _minVisibleSnapshot;
};
}  // namespace mongo
