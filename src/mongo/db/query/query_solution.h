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

#include <memory>

#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj_comparator_interface.h"
#include "mongo/db/fts/fts_query.h"
#include "mongo/db/jsobj.h"
#include "mongo/db/matcher/expression.h"
#include "mongo/db/query/index_bounds.h"
#include "mongo/db/query/plan_cache.h"
#include "mongo/db/query/stage_types.h"

namespace mongo {

class GeoNearExpression;

/**
 * Represents the granularity at which a field is available in a query solution node. Note that the
 * order of the fields represents increasing availability.
 */
enum class FieldAvailability {
    // The field is not provided.
    kNotProvided,

    // The field is provided as a hash of raw data instead of the raw data itself. For example, this
    // can happen when the field is a hashed field in an index.
    kHashedValueProvided,

    // The field is completely provided.
    kFullyProvided,
};

/**
 * This is an abstract representation of a query plan.  It can be transcribed into a tree of
 * PlanStages, which can then be handed to a PlanRunner for execution.
 */
struct QuerySolutionNode {
    QuerySolutionNode() {}

    /**
     * Constructs a QuerySolutionNode with a single child.
     */
    QuerySolutionNode(std::unique_ptr<QuerySolutionNode> child) : children{child.release()} {}

    virtual ~QuerySolutionNode() {
        for (size_t i = 0; i < children.size(); ++i) {
            delete children[i];
        }
    }

    /**
     * Return a std::string representation of this node and any children.
     */
    std::string toString() const;

    /**
     * What stage should this be transcribed to?  See stage_types.h.
     */
    virtual StageType getType() const = 0;

    /**
     * Internal function called by toString()
     *
     * TODO: Consider outputting into a BSONObj or builder thereof.
     */
    virtual void appendToString(str::stream* ss, int indent) const = 0;

    //
    // Computed properties
    //

    /**
     * Must be called before any properties are examined.
     */
    virtual void computeProperties() {
        for (size_t i = 0; i < children.size(); ++i) {
            children[i]->computeProperties();
        }
    }

    /**
     * If true, one of these are true:
     *          1. All outputs are already fetched, or
     *          2. There is a projection in place and a fetch is not required.
     *
     * If false, a fetch needs to be placed above the root in order to provide results.
     *
     * Usage: To determine if every possible result that might reach the root
     * will be fully-fetched or not.  We don't want any surplus fetches.
     */
    virtual bool fetched() const = 0;

    /**
     * Returns the granularity at which the tree rooted at this node provides data with the field
     * name 'field'. This data can come from any of the types of the WSM.
     *
     * Usage: If an index-only plan has all the fields we're interested in, we don't
     * have to fetch to show results with those fields.
     */
    virtual FieldAvailability getFieldAvailability(const std::string& field) const = 0;

    /**
     * Syntatic sugar on top of getFieldAvailability(). Returns true if the 'field' is fully
     * provided and false otherwise.
     */
    bool hasField(const std::string& field) const {
        return getFieldAvailability(field) == FieldAvailability::kFullyProvided;
    }

    /**
     * Returns true if the tree rooted at this node provides data that is sorted by its location on
     * disk.
     *
     * Usage: If all the children of an STAGE_AND_HASH have this property, we can compute the AND
     * faster by replacing the STAGE_AND_HASH with STAGE_AND_SORTED.
     */
    virtual bool sortedByDiskLoc() const = 0;

    /**
     * Return a BSONObjSet representing the possible sort orders of the data stream from this node.
     * If the data is not sorted in any particular fashion, returns an empty set.
     *
     * Usage:
     * 1. If our plan gives us a sort order, we don't have to add a sort stage.
     * 2. If all the children of an OR have the same sort order, we can maintain that
     *    sort order with a STAGE_SORT_MERGE instead of STAGE_OR.
     */
    virtual const BSONObjSet& getSort() const = 0;

    /**
     * Make a deep copy.
     */
    virtual QuerySolutionNode* clone() const = 0;

    /**
     * Copy base query solution data from 'this' to 'other'.
     */
    void cloneBaseData(QuerySolutionNode* other) const {
        for (size_t i = 0; i < this->children.size(); i++) {
            other->children.push_back(this->children[i]->clone());
        }
        if (nullptr != this->filter) {
            other->filter = this->filter->shallowClone();
        }
    }

    /**
     * Adds a vector of query solution nodes to the list of children of this node.
     *
     * TODO SERVER-35512: Once 'children' are held by unique_ptr, this method should no longer be
     * necessary.
     */
    void addChildren(std::vector<std::unique_ptr<QuerySolutionNode>> newChildren) {
        children.reserve(children.size() + newChildren.size());
        std::transform(newChildren.begin(),
                       newChildren.end(),
                       std::back_inserter(children),
                       [](auto& child) { return child.release(); });
    }

    // These are owned here.
    //
    // TODO SERVER-35512: Make this a vector of unique_ptr.
    std::vector<QuerySolutionNode*> children;

    // If a stage has a non-NULL filter all values outputted from that stage must pass that
    // filter.
    std::unique_ptr<MatchExpression> filter;

protected:
    /**
     * Formatting helper used by toString().
     */
    static void addIndent(str::stream* ss, int level);

    /**
     * Every solution node has properties and this adds the debug info for the
     * properties.
     */
    void addCommon(str::stream* ss, int indent) const;

private:
    QuerySolutionNode(const QuerySolutionNode&) = delete;
    QuerySolutionNode& operator=(const QuerySolutionNode&) = delete;
};

/**
 * A QuerySolution must be entirely self-contained and own everything inside of it.
 *
 * A tree of stages may be built from a QuerySolution.  The QuerySolution must outlive the tree
 * of stages.
 */
struct QuerySolution {
    QuerySolution() : hasBlockingStage(false), indexFilterApplied(false) {}

    // Owned here.
    std::unique_ptr<QuerySolutionNode> root;

    // Any filters in root or below point into this object.  Must be owned.
    BSONObj filterData;

    // There are two known scenarios in which a query solution might potentially block:
    //
    // Sort stage:
    // If the solution has a sort stage, the sort wasn't provided by an index, so we might want
    // to scan an index to provide that sort in a non-blocking fashion.
    //
    // Hashed AND stage:
    // The hashed AND stage buffers data from multiple index scans and could block. In that case,
    // we would want to fall back on an alternate non-blocking solution.
    bool hasBlockingStage;

    // Runner executing this solution might be interested in knowing
    // if the planning process for this solution was based on filtered indices.
    bool indexFilterApplied;

    // Owned here. Used by the plan cache.
    std::unique_ptr<SolutionCacheData> cacheData;

    /**
     * Output a human-readable std::string representing the plan.
     */
    std::string toString() {
        if (nullptr == root) {
            return "empty query solution";
        }

        str::stream ss;
        root->appendToString(&ss, 0);
        return ss;
    }

private:
    QuerySolution(const QuerySolution&) = delete;
    QuerySolution& operator=(const QuerySolution&) = delete;
};

struct TextNode : public QuerySolutionNode {
    TextNode(IndexEntry index)
        : _sort(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), index(std::move(index)) {}

    virtual ~TextNode() {}

    virtual StageType getType() const {
        return STAGE_TEXT;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    // Text's return is LOC_AND_OBJ so it's fetched and has all fields.
    bool fetched() const {
        return true;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return _sort;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sort;

    IndexEntry index;
    std::unique_ptr<fts::FTSQuery> ftsQuery;

    // The number of fields in the prefix of the text index. For example, if the key pattern is
    //
    //   { a: 1, b: 1, _fts: "text", _ftsx: 1, c: 1 }
    //
    // then the number of prefix fields is 2, because of "a" and "b".
    size_t numPrefixFields = 0u;

    // "Prefix" fields of a text index can handle equality predicates.  We group them with the
    // text node while creating the text leaf node and convert them into a BSONObj index prefix
    // when we finish the text leaf node.
    BSONObj indexPrefix;
};

struct CollectionScanNode : public QuerySolutionNode {
    CollectionScanNode();
    virtual ~CollectionScanNode() {}

    virtual StageType getType() const {
        return STAGE_COLLSCAN;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return true;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return _sort;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sort;

    // Name of the namespace.
    std::string name;

    // If present, the collection scan will seek directly to the RecordId of an oplog entry as
    // close to 'minTs' as possible without going higher. Should only be set on forward oplog scans.
    // This field cannot be used in conjunction with 'resumeAfterRecordId'.
    boost::optional<Timestamp> minTs;

    // If present the collection scan will stop and return EOF the first time it sees a document
    // that does not pass the filter and has 'ts' greater than 'maxTs'. Should only be set on
    // forward oplog scans.
    // This field cannot be used in conjunction with 'resumeAfterRecordId'.
    boost::optional<Timestamp> maxTs;

    // If true, the collection scan will return a token that can be used to resume the scan.
    bool requestResumeToken = false;

    // If present, the collection scan will seek to the exact RecordId, or return KeyNotFound if it
    // does not exist. Must only be set on forward collection scans.
    // This field cannot be used in conjunction with 'minTs' or 'maxTs'.
    boost::optional<RecordId> resumeAfterRecordId;

    // Should we make a tailable cursor?
    bool tailable;

    // Should we keep track of the timestamp of the latest oplog entry we've seen? This information
    // is needed to merge cursors from the oplog in order of operation time when reading the oplog
    // across a sharded cluster.
    bool shouldTrackLatestOplogTimestamp = false;

    int direction{1};

    // Whether or not to wait for oplog visibility on oplog collection scans.
    bool shouldWaitForOplogVisibility = false;

    // Once the first matching document is found, assume that all documents after it must match.
    bool stopApplyingFilterAfterFirstMatch = false;
};

struct AndHashNode : public QuerySolutionNode {
    AndHashNode();
    virtual ~AndHashNode();

    virtual StageType getType() const {
        return STAGE_AND_HASH;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const;
    FieldAvailability getFieldAvailability(const std::string& field) const;
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return children.back()->getSort();
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sort;
};

struct AndSortedNode : public QuerySolutionNode {
    AndSortedNode();
    virtual ~AndSortedNode();

    virtual StageType getType() const {
        return STAGE_AND_SORTED;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const;
    FieldAvailability getFieldAvailability(const std::string& field) const;
    bool sortedByDiskLoc() const {
        return true;
    }
    const BSONObjSet& getSort() const {
        return _sort;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sort;
};

struct OrNode : public QuerySolutionNode {
    OrNode();
    virtual ~OrNode();

    virtual StageType getType() const {
        return STAGE_OR;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const;
    FieldAvailability getFieldAvailability(const std::string& field) const;
    bool sortedByDiskLoc() const {
        // Even if our children are sorted by their diskloc or other fields, we don't maintain
        // any order on the output.
        return false;
    }
    const BSONObjSet& getSort() const {
        return _sort;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sort;

    bool dedup;
};

struct MergeSortNode : public QuerySolutionNode {
    MergeSortNode();
    virtual ~MergeSortNode();

    virtual StageType getType() const {
        return STAGE_SORT_MERGE;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const;
    FieldAvailability getFieldAvailability(const std::string& field) const;
    bool sortedByDiskLoc() const {
        return false;
    }

    const BSONObjSet& getSort() const {
        return _sorts;
    }

    QuerySolutionNode* clone() const;

    virtual void computeProperties() {
        for (size_t i = 0; i < children.size(); ++i) {
            children[i]->computeProperties();
        }
        _sorts.clear();
        _sorts.insert(sort);
    }

    BSONObjSet _sorts;

    BSONObj sort;
    bool dedup;
};

struct FetchNode : public QuerySolutionNode {
    FetchNode();
    virtual ~FetchNode() {}

    virtual StageType getType() const {
        return STAGE_FETCH;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return true;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return children[0]->sortedByDiskLoc();
    }
    const BSONObjSet& getSort() const {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sorts;
};

struct IndexScanNode : public QuerySolutionNode {
    IndexScanNode(IndexEntry index);
    virtual ~IndexScanNode() {}

    virtual void computeProperties();

    virtual StageType getType() const {
        return STAGE_IXSCAN;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return false;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const;
    bool sortedByDiskLoc() const;
    const BSONObjSet& getSort() const {
        return _sorts;
    }

    QuerySolutionNode* clone() const;

    bool operator==(const IndexScanNode& other) const;

    /**
     * This function extracts a list of field names from 'indexKeyPattern' whose corresponding index
     * bounds in 'bounds' can contain strings.  This is the case if there are intervals containing
     * String, Object, or Array values.
     */
    static std::set<StringData> getFieldsWithStringBounds(const IndexBounds& bounds,
                                                          const BSONObj& indexKeyPattern);

    BSONObjSet _sorts;

    IndexEntry index;

    int direction;

    // If there's a 'returnKey' projection we add key metadata.
    bool addKeyMetadata;

    bool shouldDedup = false;

    IndexBounds bounds;

    const CollatorInterface* queryCollator;

    // The set of paths in the index key pattern which have at least one multikey path component, or
    // empty if the index either is not multikey or does not have path-level multikeyness metadata.
    //
    // The correct set of paths is computed and stored here by computeProperties().
    std::set<StringData> multikeyFields;
};

struct ReturnKeyNode : public QuerySolutionNode {
    ReturnKeyNode(std::unique_ptr<QuerySolutionNode> child,
                  std::vector<FieldPath> sortKeyMetaFields)
        : QuerySolutionNode(std::move(child)), sortKeyMetaFields(std::move(sortKeyMetaFields)) {}

    StageType getType() const final {
        return STAGE_RETURN_KEY;
    }

    void appendToString(str::stream* ss, int indent) const final;

    bool fetched() const final {
        return children[0]->fetched();
    }
    FieldAvailability getFieldAvailability(const std::string& field) const final {
        return FieldAvailability::kNotProvided;
    }
    bool sortedByDiskLoc() const final {
        return children[0]->sortedByDiskLoc();
    }
    const BSONObjSet& getSort() const final {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const final;

    std::vector<FieldPath> sortKeyMetaFields;
};

/**
 * We have a few implementations of the projection functionality. They are chosen by constructing
 * a type derived from this abstract struct. The most general implementation 'ProjectionNodeDefault'
 * is much slower than the fast-path implementations. We only really have all the information
 * available to choose a projection implementation at planning time.
 */
struct ProjectionNode : QuerySolutionNode {
    ProjectionNode(std::unique_ptr<QuerySolutionNode> child,
                   const MatchExpression& fullExpression,
                   projection_ast::Projection proj)
        : QuerySolutionNode(std::move(child)),
          _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()),
          fullExpression(fullExpression),
          proj(std::move(proj)) {}

    void computeProperties() final;

    void appendToString(str::stream* ss, int indent) const final;

    /**
     * Data from the projection node is considered fetch iff the child provides fetched data.
     */
    bool fetched() const {
        return children[0]->fetched();
    }

    FieldAvailability getFieldAvailability(const std::string& field) const {
        // If we were to construct a plan where the input to the project stage was a hashed value,
        // and that field was retained exactly, then we would mistakenly return 'kFullyProvided'.
        // The important point here is that we are careful to construct plans where we fetch before
        // projecting if there is hashed data, collation keys, etc. So this situation does not
        // arise.
        return proj.isFieldRetainedExactly(StringData{field}) ? FieldAvailability::kFullyProvided
                                                              : FieldAvailability::kNotProvided;
    }

    bool sortedByDiskLoc() const {
        // Projections destroy the RecordId.  By returning true here, this kind of implies that a
        // fetch could still be done upstream.
        //
        // Perhaps this should be false to not imply that there *is* a RecordId?  Kind of a
        // corner case.
        return children[0]->sortedByDiskLoc();
    }

    const BSONObjSet& getSort() const {
        return _sorts;
    }

protected:
    void cloneProjectionData(ProjectionNode* copy) const;

public:
    /**
     * Identify projectionImplementation type as a string.
     */
    virtual StringData projectionImplementationTypeToString() const = 0;

    BSONObjSet _sorts;

    // The full query tree.  Needed when we have positional operators.
    // Owned in the CanonicalQuery, not here.
    const MatchExpression& fullExpression;

    projection_ast::Projection proj;
};

/**
 * This is the most general implementation of the projection functionality. It handles every case.
 */
struct ProjectionNodeDefault final : ProjectionNode {
    using ProjectionNode::ProjectionNode;

    StageType getType() const final {
        return STAGE_PROJECTION_DEFAULT;
    }

    ProjectionNode* clone() const final;

    StringData projectionImplementationTypeToString() const final {
        return "DEFAULT"_sd;
    }
};

/**
 * This is a fast-path for when the projection is fully covered by one index.
 */
struct ProjectionNodeCovered final : ProjectionNode {
    ProjectionNodeCovered(std::unique_ptr<QuerySolutionNode> child,
                          const MatchExpression& fullExpression,
                          projection_ast::Projection proj,
                          BSONObj coveredKeyObj)
        : ProjectionNode(std::move(child), fullExpression, std::move(proj)),
          coveredKeyObj(std::move(coveredKeyObj)) {}

    StageType getType() const final {
        return STAGE_PROJECTION_COVERED;
    }

    ProjectionNode* clone() const final;

    StringData projectionImplementationTypeToString() const final {
        return "COVERED_ONE_INDEX"_sd;
    }

    // This is the key pattern of the index supplying our covered data. We can pre-compute which
    // fields to include and cache that data for later if we know we only have one index.
    BSONObj coveredKeyObj;
};

/**
 * This is a fast-path for when the projection only has inclusions on non-dotted fields.
 */
struct ProjectionNodeSimple final : ProjectionNode {
    using ProjectionNode::ProjectionNode;

    StageType getType() const final {
        return STAGE_PROJECTION_SIMPLE;
    }

    ProjectionNode* clone() const final;

    StringData projectionImplementationTypeToString() const final {
        return "SIMPLE_DOC"_sd;
    }
};

struct SortKeyGeneratorNode : public QuerySolutionNode {
    StageType getType() const final {
        return STAGE_SORT_KEY_GENERATOR;
    }

    bool fetched() const final {
        return children[0]->fetched();
    }

    FieldAvailability getFieldAvailability(const std::string& field) const final {
        return children[0]->getFieldAvailability(field);
    }

    bool sortedByDiskLoc() const final {
        return children[0]->sortedByDiskLoc();
    }

    const BSONObjSet& getSort() const final {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const final;

    void appendToString(str::stream* ss, int indent) const final;

    // The user-supplied sort pattern.
    BSONObj sortSpec;
};

struct SortNode : public QuerySolutionNode {
    SortNode() : _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), limit(0) {}

    virtual ~SortNode() {}

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return children[0]->fetched();
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return children[0]->getFieldAvailability(field);
    }
    bool sortedByDiskLoc() const {
        return false;
    }

    const BSONObjSet& getSort() const {
        return _sorts;
    }

    virtual void computeProperties() {
        for (size_t i = 0; i < children.size(); ++i) {
            children[i]->computeProperties();
        }
        _sorts.clear();
        _sorts.insert(pattern);
    }

    BSONObjSet _sorts;

    BSONObj pattern;

    // Sum of both limit and skip count in the parsed query.
    size_t limit;

    bool addSortKeyMetadata = false;

protected:
    void cloneSortData(SortNode* copy) const;

private:
    virtual StringData sortImplementationTypeToString() const = 0;
};

/**
 * Represents sort algorithm that can handle any kind of input data.
 */
struct SortNodeDefault final : public SortNode {
    virtual StageType getType() const override {
        return STAGE_SORT_DEFAULT;
    }

    QuerySolutionNode* clone() const override;

    StringData sortImplementationTypeToString() const override {
        return "DEFAULT"_sd;
    }
};

/**
 * Represents a special, optimized sort algorithm that is only correct if:
 *  - The input data is fetched.
 *  - The input data has no metadata attached.
 *  - The record id can be discarded.
 */
struct SortNodeSimple final : public SortNode {
    virtual StageType getType() const {
        return STAGE_SORT_SIMPLE;
    }

    QuerySolutionNode* clone() const override;

    StringData sortImplementationTypeToString() const override {
        return "SIMPLE"_sd;
    }
};

struct LimitNode : public QuerySolutionNode {
    LimitNode() {}
    virtual ~LimitNode() {}

    virtual StageType getType() const {
        return STAGE_LIMIT;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return children[0]->fetched();
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return children[0]->getFieldAvailability(field);
    }
    bool sortedByDiskLoc() const {
        return children[0]->sortedByDiskLoc();
    }
    const BSONObjSet& getSort() const {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const;

    long long limit;
};

struct SkipNode : public QuerySolutionNode {
    SkipNode() {}
    virtual ~SkipNode() {}

    virtual StageType getType() const {
        return STAGE_SKIP;
    }
    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return children[0]->fetched();
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return children[0]->getFieldAvailability(field);
    }
    bool sortedByDiskLoc() const {
        return children[0]->sortedByDiskLoc();
    }
    const BSONObjSet& getSort() const {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const;

    long long skip;
};

// This is a standalone stage.
struct GeoNear2DNode : public QuerySolutionNode {
    GeoNear2DNode(IndexEntry index)
        : _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()),
          index(std::move(index)),
          addPointMeta(false),
          addDistMeta(false) {}

    virtual ~GeoNear2DNode() {}

    virtual StageType getType() const {
        return STAGE_GEO_NEAR_2D;
    }
    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return true;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return _sorts;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sorts;

    // Not owned here
    const GeoNearExpression* nq;
    IndexBounds baseBounds;

    IndexEntry index;
    bool addPointMeta;
    bool addDistMeta;
};

// This is actually its own standalone stage.
struct GeoNear2DSphereNode : public QuerySolutionNode {
    GeoNear2DSphereNode(IndexEntry index)
        : _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()),
          index(std::move(index)),
          addPointMeta(false),
          addDistMeta(false) {}

    virtual ~GeoNear2DSphereNode() {}

    virtual StageType getType() const {
        return STAGE_GEO_NEAR_2DSPHERE;
    }
    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return true;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return _sorts;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet _sorts;

    // Not owned here
    const GeoNearExpression* nq;
    IndexBounds baseBounds;

    IndexEntry index;
    bool addPointMeta;
    bool addDistMeta;
};

//
// Internal nodes used to provide functionality
//

/**
 * If we're answering a query on a sharded cluster, docs must be checked against the shard key
 * to ensure that we don't return data that shouldn't be there.  This must be done prior to
 * projection, and in fact should be done as early as possible to avoid propagating stale data
 * through the pipeline.
 */
struct ShardingFilterNode : public QuerySolutionNode {
    ShardingFilterNode() {}
    virtual ~ShardingFilterNode() {}

    virtual StageType getType() const {
        return STAGE_SHARDING_FILTER;
    }
    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return children[0]->fetched();
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return children[0]->getFieldAvailability(field);
    }
    bool sortedByDiskLoc() const {
        return children[0]->sortedByDiskLoc();
    }
    const BSONObjSet& getSort() const {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const;
};

/**
 * Distinct queries only want one value for a given field.  We run an index scan but
 * *always* skip over the current key to the next key.
 */
struct DistinctNode : public QuerySolutionNode {
    DistinctNode(IndexEntry index)
        : sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), index(std::move(index)) {}

    virtual ~DistinctNode() {}

    virtual StageType getType() const {
        return STAGE_DISTINCT_SCAN;
    }
    virtual void appendToString(str::stream* ss, int indent) const;

    // This stage is created "on top" of normal planning and as such the properties
    // below don't really matter.
    bool fetched() const {
        return false;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        // The distinct scan can return collation keys, but we can still consider the field fully
        // provided. This is because the logic around when the index bounds might incorporate
        // collation keys does not rely on 'getFieldAvailability()'. As a future improvement, we
        // could look into using 'getFieldAvailabilty()' for collation covering analysis.
        return index.keyPattern[field].eoo() ? FieldAvailability::kNotProvided
                                             : FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return sorts;
    }

    QuerySolutionNode* clone() const;

    virtual void computeProperties();

    BSONObjSet sorts;

    IndexEntry index;
    IndexBounds bounds;

    const CollatorInterface* queryCollator;

    // We are distinct-ing over the 'fieldNo'-th field of 'index.keyPattern'.
    int fieldNo{0};
    int direction{1};
};

/**
 * Some count queries reduce to counting how many keys are between two entries in a
 * Btree.
 */
struct CountScanNode : public QuerySolutionNode {
    CountScanNode(IndexEntry index)
        : sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), index(std::move(index)) {}

    virtual ~CountScanNode() {}

    virtual StageType getType() const {
        return STAGE_COUNT_SCAN;
    }
    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return false;
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return FieldAvailability::kFullyProvided;
    }
    bool sortedByDiskLoc() const {
        return false;
    }
    const BSONObjSet& getSort() const {
        return sorts;
    }

    QuerySolutionNode* clone() const;

    BSONObjSet sorts;

    IndexEntry index;

    BSONObj startKey;
    bool startKeyInclusive;

    BSONObj endKey;
    bool endKeyInclusive;
};

/**
 * This stage drops results that are out of sorted order.
 */
struct EnsureSortedNode : public QuerySolutionNode {
    EnsureSortedNode() {}
    virtual ~EnsureSortedNode() {}

    virtual StageType getType() const {
        return STAGE_ENSURE_SORTED;
    }

    virtual void appendToString(str::stream* ss, int indent) const;

    bool fetched() const {
        return children[0]->fetched();
    }
    FieldAvailability getFieldAvailability(const std::string& field) const {
        return children[0]->getFieldAvailability(field);
    }
    bool sortedByDiskLoc() const {
        return children[0]->sortedByDiskLoc();
    }
    const BSONObjSet& getSort() const {
        return children[0]->getSort();
    }

    QuerySolutionNode* clone() const;

    // The pattern that the results should be sorted by.
    BSONObj pattern;
};

}  // namespace mongo
