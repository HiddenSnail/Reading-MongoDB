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

#include <vector>

#include "mongo/db/query/query_solution.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "mongo/bson/bsontypes.h"
#include "mongo/bson/mutable/document.h"
#include "mongo/bson/simple_bsonelement_comparator.h"
#include "mongo/db/field_ref.h"
#include "mongo/db/index_names.h"
#include "mongo/db/matcher/expression_geo.h"
#include "mongo/db/query/collation/collation_index_key.h"
#include "mongo/db/query/index_bounds_builder.h"
#include "mongo/db/query/planner_analysis.h"
#include "mongo/db/query/planner_wildcard_helpers.h"
#include "mongo/db/query/projection_ast_util.h"
#include "mongo/db/query/query_planner_common.h"

namespace mongo {

namespace {

namespace wcp = ::mongo::wildcard_planning;

// Create an ordred interval list which represents the bounds for all BSON elements of type String,
// Object, or Array.
OrderedIntervalList buildStringBoundsOil(const std::string& keyName) {
    OrderedIntervalList ret;
    ret.name = keyName;

    BSONObjBuilder strBob;
    strBob.appendMinForType("", BSONType::String);
    strBob.appendMaxForType("", BSONType::String);
    ret.intervals.push_back(
        IndexBoundsBuilder::makeRangeInterval(strBob.obj(), BoundInclusion::kIncludeStartKeyOnly));

    BSONObjBuilder objBob;
    objBob.appendMinForType("", BSONType::Object);
    objBob.appendMaxForType("", BSONType::Object);
    ret.intervals.push_back(
        IndexBoundsBuilder::makeRangeInterval(objBob.obj(), BoundInclusion::kIncludeStartKeyOnly));

    BSONObjBuilder arrBob;
    arrBob.appendMinForType("", BSONType::Array);
    arrBob.appendMaxForType("", BSONType::Array);
    ret.intervals.push_back(
        IndexBoundsBuilder::makeRangeInterval(arrBob.obj(), BoundInclusion::kIncludeStartKeyOnly));

    return ret;
}

bool rangeCanContainString(const BSONElement& startKey,
                           const BSONElement& endKey,
                           BoundInclusion boundInclusion) {
    OrderedIntervalList stringBoundsOil = buildStringBoundsOil("");
    OrderedIntervalList rangeOil;
    BSONObjBuilder bob;
    bob.appendAs(startKey, "");
    bob.appendAs(endKey, "");
    rangeOil.intervals.push_back(IndexBoundsBuilder::makeRangeInterval(bob.obj(), boundInclusion));

    IndexBoundsBuilder::intersectize(rangeOil, &stringBoundsOil);
    return !stringBoundsOil.intervals.empty();
}

// Helper function for IndexScanNode::computeProperties for adding additional sort orders made
// possible by point bounds on some fields of the sort pattern.
void addEqualityFieldSorts(const BSONObj& sortPattern,
                           const std::set<string>& equalityFields,
                           BSONObjSet* sortsOut) {
    invariant(sortsOut);
    // TODO: Each field in equalityFields could be dropped from the sort order since it is
    // a point interval.  The full set of sort orders is as follows:
    // For each sort in sortsOut:
    //    For each drop in powerset(equalityFields):
    //        Remove fields in 'drop' from 'sort' and add resulting sort to output.
    //
    // Since this involves a powerset, we don't generate the full set of possibilities.
    // Instead, we generate sort orders by removing possible contiguous prefixes of equality
    // predicates. For example, if the key pattern is {a: 1, b: 1, c: 1, d: 1, e: 1}
    // and and there are equality predicates on 'a', 'b', and 'c', then here we add the sort
    // orders {b: 1, c: 1, d: 1, e: 1} and {c: 1, d: 1, e: 1}. (We also end up adding
    // {d: 1, e: 1} and {d: 1}, but this is done later on.)
    BSONObjIterator it(sortPattern);
    BSONObjBuilder suffixBob;
    while (it.more()) {
        BSONElement elt = it.next();
        // TODO: string slowness.  fix when bounds are stringdata not string.
        if (equalityFields.end() == equalityFields.find(string(elt.fieldName()))) {
            suffixBob.append(elt);
            // This field isn't a point interval, can't drop.
            break;
        }

        // We add the sort obtained by dropping 'elt' and all preceding elements from the index
        // key pattern.
        BSONObjIterator droppedPrefixIt = it;
        if (!droppedPrefixIt.more()) {
            // Do not insert an empty sort order.
            break;
        }
        BSONObjBuilder droppedPrefixBob;
        while (droppedPrefixIt.more()) {
            droppedPrefixBob.append(droppedPrefixIt.next());
        }
        sortsOut->insert(droppedPrefixBob.obj());
    }

    while (it.more()) {
        suffixBob.append(it.next());
    }

    // We've found the suffix following the contiguous prefix of equality fields.
    //   Ex. For index {a: 1, b: 1, c: 1, d: 1} and query {a: 3, b: 5}, this suffix
    //   of the key pattern is {c: 1, d: 1}.
    //
    // Now we have to add all prefixes of this suffix as possible sort orders.
    //   Ex. Continuing the example from above, we have to include sort orders
    //   {c: 1} and {c: 1, d: 1}.
    BSONObj filterPointsObj = suffixBob.obj();
    for (int i = 0; i < filterPointsObj.nFields(); ++i) {
        // Make obj out of fields [0,i]
        BSONObjIterator it(filterPointsObj);
        BSONObjBuilder prefixBob;
        for (int j = 0; j <= i; ++j) {
            prefixBob.append(it.next());
        }
        sortsOut->insert(prefixBob.obj());
    }
}
}  // namespace

string QuerySolutionNode::toString() const {
    str::stream ss;
    appendToString(&ss, 0);
    return ss;
}

// static
void QuerySolutionNode::addIndent(str::stream* ss, int level) {
    for (int i = 0; i < level; ++i) {
        *ss << "---";
    }
}

void QuerySolutionNode::addCommon(str::stream* ss, int indent) const {
    addIndent(ss, indent + 1);
    *ss << "fetched = " << fetched() << '\n';
    addIndent(ss, indent + 1);
    *ss << "sortedByDiskLoc = " << sortedByDiskLoc() << '\n';
    addIndent(ss, indent + 1);
    *ss << "getSort = [";
    for (BSONObjSet::const_iterator it = getSort().begin(); it != getSort().end(); it++) {
        *ss << it->toString() << ", ";
    }
    *ss << "]" << '\n';
}

//
// TextNode
//

void TextNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "TEXT\n";
    addIndent(ss, indent + 1);
    *ss << "name = " << index.identifier.catalogName << '\n';
    addIndent(ss, indent + 1);
    *ss << "keyPattern = " << index.keyPattern.toString() << '\n';
    addIndent(ss, indent + 1);
    *ss << "query = " << ftsQuery->getQuery() << '\n';
    addIndent(ss, indent + 1);
    *ss << "language = " << ftsQuery->getLanguage() << '\n';
    addIndent(ss, indent + 1);
    *ss << "caseSensitive= " << ftsQuery->getCaseSensitive() << '\n';
    addIndent(ss, indent + 1);
    *ss << "diacriticSensitive= " << ftsQuery->getDiacriticSensitive() << '\n';
    addIndent(ss, indent + 1);
    *ss << "indexPrefix = " << indexPrefix.toString() << '\n';
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << " filter = " << filter->debugString();
    }
    addCommon(ss, indent);
}

QuerySolutionNode* TextNode::clone() const {
    TextNode* copy = new TextNode(this->index);
    cloneBaseData(copy);

    copy->_sort = this->_sort;
    copy->ftsQuery = this->ftsQuery->clone();
    copy->indexPrefix = this->indexPrefix;

    return copy;
}

//
// CollectionScanNode
//

CollectionScanNode::CollectionScanNode()
    : _sort(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), tailable(false), direction(1) {}

void CollectionScanNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "COLLSCAN\n";
    addIndent(ss, indent + 1);
    *ss << "ns = " << name << '\n';
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << "filter = " << filter->debugString();
    }
    addCommon(ss, indent);
}

QuerySolutionNode* CollectionScanNode::clone() const {
    CollectionScanNode* copy = new CollectionScanNode();
    cloneBaseData(copy);

    copy->_sort = this->_sort;
    copy->name = this->name;
    copy->tailable = this->tailable;
    copy->direction = this->direction;
    copy->shouldTrackLatestOplogTimestamp = this->shouldTrackLatestOplogTimestamp;
    copy->shouldWaitForOplogVisibility = this->shouldWaitForOplogVisibility;

    return copy;
}

//
// AndHashNode
//

AndHashNode::AndHashNode() : _sort(SimpleBSONObjComparator::kInstance.makeBSONObjSet()) {}

AndHashNode::~AndHashNode() {}

void AndHashNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "AND_HASH\n";
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << " filter = " << filter->debugString() << '\n';
    }
    addCommon(ss, indent);
    for (size_t i = 0; i < children.size(); ++i) {
        addIndent(ss, indent + 1);
        *ss << "Child " << i << ":\n";
        children[i]->appendToString(ss, indent + 1);
    }
}

bool AndHashNode::fetched() const {
    // Any WSM output from this stage came from all children stages.  If any child provides
    // fetched data, we merge that fetched data into the WSM we output.
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]->fetched()) {
            return true;
        }
    }
    return false;
}

FieldAvailability AndHashNode::getFieldAvailability(const string& field) const {
    // A field can be provided by any of the children.
    auto result = FieldAvailability::kNotProvided;
    for (size_t i = 0; i < children.size(); ++i) {
        result = std::max(result, children[i]->getFieldAvailability(field));
    }
    return result;
}

QuerySolutionNode* AndHashNode::clone() const {
    AndHashNode* copy = new AndHashNode();
    cloneBaseData(copy);

    copy->_sort = this->_sort;

    return copy;
}

//
// AndSortedNode
//

AndSortedNode::AndSortedNode() : _sort(SimpleBSONObjComparator::kInstance.makeBSONObjSet()) {}

AndSortedNode::~AndSortedNode() {}

void AndSortedNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "AND_SORTED\n";
    addCommon(ss, indent);
    for (size_t i = 0; i < children.size(); ++i) {
        addIndent(ss, indent + 1);
        *ss << "Child " << i << ":\n";
        children[i]->appendToString(ss, indent + 1);
    }
}

bool AndSortedNode::fetched() const {
    // Any WSM output from this stage came from all children stages.  If any child provides
    // fetched data, we merge that fetched data into the WSM we output.
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]->fetched()) {
            return true;
        }
    }
    return false;
}

FieldAvailability AndSortedNode::getFieldAvailability(const string& field) const {
    // A field can be provided by any of the children.
    auto result = FieldAvailability::kNotProvided;
    for (size_t i = 0; i < children.size(); ++i) {
        result = std::max(result, children[i]->getFieldAvailability(field));
    }
    return result;
}

QuerySolutionNode* AndSortedNode::clone() const {
    AndSortedNode* copy = new AndSortedNode();
    cloneBaseData(copy);

    copy->_sort = this->_sort;

    return copy;
}

//
// OrNode
//

OrNode::OrNode() : _sort(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), dedup(true) {}

OrNode::~OrNode() {}

void OrNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "OR\n";
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << " filter = " << filter->debugString() << '\n';
    }
    addCommon(ss, indent);
    for (size_t i = 0; i < children.size(); ++i) {
        addIndent(ss, indent + 1);
        *ss << "Child " << i << ":\n";
        children[i]->appendToString(ss, indent + 2);
        *ss << '\n';
    }
}

bool OrNode::fetched() const {
    // Any WSM output from this stage came exactly one child stage.  Given that we don't know
    // what child stage it came from, we require that all children provide fetched data in order
    // to guarantee that our output is fetched.
    for (size_t i = 0; i < children.size(); ++i) {
        if (!children[i]->fetched()) {
            return false;
        }
    }
    return true;
}

/**
 * Any WSM output from this stage came from exactly one child stage.  Therefore, if
 * we want to guarantee that any output has a certain field, all of our children must
 * have that field.
 */
FieldAvailability OrNode::getFieldAvailability(const string& field) const {
    auto result = FieldAvailability::kFullyProvided;
    for (size_t i = 0; i < children.size(); ++i) {
        result = std::min(result, children[i]->getFieldAvailability(field));
    }
    return result;
}

QuerySolutionNode* OrNode::clone() const {
    OrNode* copy = new OrNode();
    cloneBaseData(copy);

    copy->_sort = this->_sort;
    copy->dedup = this->dedup;

    return copy;
}

//
// MergeSortNode
//

MergeSortNode::MergeSortNode()
    : _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()), dedup(true) {}

MergeSortNode::~MergeSortNode() {}

void MergeSortNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "MERGE_SORT\n";
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << " filter = " << filter->debugString() << '\n';
    }
    addCommon(ss, indent);
    for (size_t i = 0; i < children.size(); ++i) {
        addIndent(ss, indent + 1);
        *ss << "Child " << i << ":\n";
        children[i]->appendToString(ss, indent + 2);
        *ss << '\n';
    }
}

bool MergeSortNode::fetched() const {
    // Any WSM output from this stage came exactly one child stage.  Given that we don't know
    // what child stage it came from, we require that all children provide fetched data in order
    // to guarantee that our output is fetched.
    for (size_t i = 0; i < children.size(); ++i) {
        if (!children[i]->fetched()) {
            return false;
        }
    }
    return true;
}

/**
 * Any WSM output from this stage came from exactly one child stage.  Therefore, if
 * we want to guarantee that any output has a certain field, all of our children must
 * have that field.
 */
FieldAvailability MergeSortNode::getFieldAvailability(const string& field) const {
    auto result = FieldAvailability::kFullyProvided;
    for (size_t i = 0; i < children.size(); ++i) {
        result = std::min(result, children[i]->getFieldAvailability(field));
    }
    return result;
}

QuerySolutionNode* MergeSortNode::clone() const {
    MergeSortNode* copy = new MergeSortNode();
    cloneBaseData(copy);

    copy->_sorts = this->_sorts;
    copy->dedup = this->dedup;
    copy->sort = this->sort;

    return copy;
}

//
// FetchNode
//

FetchNode::FetchNode() : _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()) {}

void FetchNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "FETCH\n";
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        StringBuilder sb;
        *ss << "filter:\n";
        filter->debugString(sb, indent + 2);
        *ss << sb.str();
    }
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* FetchNode::clone() const {
    FetchNode* copy = new FetchNode();
    cloneBaseData(copy);

    copy->_sorts = this->_sorts;

    return copy;
}

//
// IndexScanNode
//

IndexScanNode::IndexScanNode(IndexEntry index)
    : _sorts(SimpleBSONObjComparator::kInstance.makeBSONObjSet()),
      index(std::move(index)),
      direction(1),
      addKeyMetadata(false),
      shouldDedup(index.multikey),
      queryCollator(nullptr) {}

void IndexScanNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "IXSCAN\n";
    addIndent(ss, indent + 1);
    *ss << "indexName = " << index.identifier.catalogName << '\n';
    *ss << "keyPattern = " << index.keyPattern << '\n';
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << "filter = " << filter->debugString();
    }
    addIndent(ss, indent + 1);
    *ss << "direction = " << direction << '\n';
    addIndent(ss, indent + 1);
    *ss << "bounds = " << bounds.toString() << '\n';
    addCommon(ss, indent);
}

FieldAvailability IndexScanNode::getFieldAvailability(const string& field) const {
    // A $** index whose bounds overlap the object type bracket cannot provide covering, since the
    // index only contains the leaf nodes along each of the object's subpaths.
    if (index.type == IndexType::INDEX_WILDCARD && wcp::isWildcardObjectSubpathScan(this)) {
        return FieldAvailability::kNotProvided;
    }

    // The index is multikey but does not have any path-level multikeyness information. Such indexes
    // can never provide covering.
    if (index.multikey && index.multikeyPaths.empty()) {
        return FieldAvailability::kNotProvided;
    }

    // Compound hashed indexes can be covered when the projection is not on the hashed field. Other
    // custom index access methods may return non-exact key data - this function is currently used
    // for covering exact key data only.
    auto indexPluginName = IndexNames::findPluginName(index.keyPattern);
    switch (IndexNames::nameToType(indexPluginName)) {
        case IndexType::INDEX_BTREE:
        case IndexType::INDEX_HASHED:
            break;
        default:
            // All other index types provide no fields.
            return FieldAvailability::kNotProvided;
    }

    // If the index has a non-simple collation and we have collation keys inside 'field', then this
    // index scan does not provide that field (and the query cannot be covered).
    if (index.collator) {
        std::set<StringData> collatedFields = getFieldsWithStringBounds(bounds, index.keyPattern);
        if (collatedFields.find(field) != collatedFields.end()) {
            return FieldAvailability::kNotProvided;
        }
    }

    size_t keyPatternFieldIndex = 0;
    for (auto&& elt : index.keyPattern) {
        // For $** indexes, the keyPattern is prefixed by a virtual field, '$_path'. We therefore
        // skip the first keyPattern field when deciding whether we can provide the requested field.
        if (index.type == IndexType::INDEX_WILDCARD && !keyPatternFieldIndex) {
            invariant(elt.fieldNameStringData() == "$_path"_sd);
            ++keyPatternFieldIndex;
            continue;
        }
        // The index can provide this field if the requested path appears in the index key pattern,
        // and that path has no multikey components. We can't cover a field that has multikey
        // components because the index keys contain individual array elements, and we can't
        // reconstitute the array from the index keys in the right order. In order for the field to
        // be fully provided by the scan, it must be ascending (1) or descending (-1).
        if (field == elt.fieldName() &&
            (!index.multikey || index.multikeyPaths[keyPatternFieldIndex].empty())) {
            // We already know that the index is either ascending, descending or hashed. If the
            // field is hashed, we only provide hashed value.
            return elt.isNumber() ? FieldAvailability::kFullyProvided
                                  : FieldAvailability::kHashedValueProvided;
        }
        ++keyPatternFieldIndex;
    }
    return FieldAvailability::kNotProvided;
}

bool IndexScanNode::sortedByDiskLoc() const {
    // Indices use RecordId as an additional key after the actual index key.
    // Therefore, if we're only examining one index key, the output is sorted
    // by RecordId.

    // If it's a simple range query, it's easy to determine if the range is a point.
    if (bounds.isSimpleRange) {
        return 0 == bounds.startKey.woCompare(bounds.endKey, index.keyPattern);
    }

    // If it's a more complex bounds query, we make sure that each field is a point.
    for (size_t i = 0; i < bounds.fields.size(); ++i) {
        const OrderedIntervalList& oil = bounds.fields[i];
        if (1 != oil.intervals.size()) {
            return false;
        }
        const Interval& interval = oil.intervals[0];
        if (0 != interval.start.woCompare(interval.end, false)) {
            return false;
        }
    }

    return true;
}

// static
std::set<StringData> IndexScanNode::getFieldsWithStringBounds(const IndexBounds& inputBounds,
                                                              const BSONObj& indexKeyPattern) {
    // Produce a copy of the bounds which are all ascending, as we can only compute intersections
    // of ascending bounds.
    IndexBounds bounds = inputBounds.forwardize();

    BSONObjIterator keyPatternIterator(indexKeyPattern);

    if (bounds.isSimpleRange) {
        // With a simple range, the only cases we can say for sure do not contain strings
        // are those with point bounds.
        BSONObjIterator startKeyIterator(bounds.startKey);
        BSONObjIterator endKeyIterator(bounds.endKey);
        while (keyPatternIterator.more() && startKeyIterator.more() && endKeyIterator.more()) {
            BSONElement startKey = startKeyIterator.next();
            BSONElement endKey = endKeyIterator.next();
            if (SimpleBSONElementComparator::kInstance.evaluate(startKey != endKey) ||
                CollationIndexKey::isCollatableType(startKey.type())) {
                BoundInclusion boundInclusion = bounds.boundInclusion;
                if (startKeyIterator.more()) {
                    boundInclusion = BoundInclusion::kIncludeBothStartAndEndKeys;
                }
                if (!rangeCanContainString(startKey, endKey, boundInclusion)) {
                    // If the first non-point range cannot contain strings, we don't need to
                    // add it to the return set.
                    keyPatternIterator.next();
                }

                // Any remaining keys could have strings.
                std::set<StringData> ret;
                while (keyPatternIterator.more()) {
                    ret.insert(keyPatternIterator.next().fieldNameStringData());
                }
                return ret;
            }

            keyPatternIterator.next();
        }

        return std::set<StringData>{};
    }

    std::set<StringData> ret;
    invariant(bounds.fields.size() == static_cast<size_t>(indexKeyPattern.nFields()));
    for (const auto& oil : bounds.fields) {
        invariant(keyPatternIterator.more());
        BSONElement el = keyPatternIterator.next();
        OrderedIntervalList intersection = buildStringBoundsOil(el.fieldName());
        IndexBoundsBuilder::intersectize(oil, &intersection);
        if (!intersection.intervals.empty()) {
            ret.insert(el.fieldNameStringData());
        }
    }

    return ret;
}

namespace {
std::set<StringData> getMultikeyFields(const BSONObj& keyPattern,
                                       const MultikeyPaths& multikeyPaths) {
    std::set<StringData> multikeyFields;
    size_t i = 0;
    for (auto&& elem : keyPattern) {
        if (!multikeyPaths[i].empty()) {
            multikeyFields.insert(elem.fieldNameStringData());
        }
        ++i;
    }
    return multikeyFields;
}

/**
 * Returns true if the index scan described by 'multikeyFields' and 'bounds' can legally provide the
 * 'sortSpec', or false if the sort cannot be provided. A multikey index cannot provide a sort if
 * either of the following is true: 1) the sort spec includes a multikey field that has bounds other
 * than [minKey, maxKey], 2) there are bounds other than [minKey, maxKey] over a multikey field
 * which share a path prefix with a component of the sort pattern. These cases are further explained
 * in SERVER-31898.
 */
bool confirmBoundsProvideSortGivenMultikeyness(const BSONObj& sortSpec,
                                               const IndexBounds& bounds,
                                               const std::set<StringData>& multikeyFields) {
    // Forwardize the bounds to correctly apply checks to descending sorts and well as ascending
    // sorts.
    const auto ascendingBounds = bounds.forwardize();
    const auto& fields = ascendingBounds.fields;
    for (auto&& sortPatternComponent : sortSpec) {
        if (multikeyFields.count(sortPatternComponent.fieldNameStringData()) == 0) {
            // If this component of the sort pattern (which must be one of the components of
            // the index spec) is not multikey, we don't need additional checks.
            continue;
        }

        // Bail out if the bounds are specified as a simple range. As a future improvement, we could
        // extend this optimization to allow simple multikey scans to provide a sort.
        if (ascendingBounds.isSimpleRange) {
            return false;
        }

        // Checks if the current 'sortPatternComponent' has [MinKey, MaxKey].
        const auto name = sortPatternComponent.fieldNameStringData();
        for (auto&& intervalList : fields) {
            if (name == intervalList.name && !intervalList.isMinToMax()) {
                return false;
            }
        }

        // Checks if there is a shared path prefix between the bounds and the sort pattern of
        // multikey fields.
        FieldRef refName(name);
        for (const auto& intervalList : fields) {
            // Ignores the prefix if the bounds are [MinKey, MaxKey] or if the field is not
            // multikey.
            if (intervalList.isMinToMax() ||
                (multikeyFields.find(intervalList.name) == multikeyFields.end())) {
                continue;
            }

            FieldRef boundsPath(intervalList.name);
            const auto commonPrefixSize = boundsPath.commonPrefixSize(refName);
            // The interval list name and the sort pattern name will never be equal at this point.
            // This is because if they are equal and do not have [minKey, maxKey] bounds, we would
            // already have bailed out of the function. If they do have [minKey, maxKey] bounds,
            // they will be skipped in the check for [minKey, maxKey] bounds above.
            invariant(refName != boundsPath);
            // Checks if there's a common prefix between the interval list name and the sort pattern
            // name.
            if (commonPrefixSize > 0) {
                return false;
            }
        }
    }
    return true;
}


/**
 * Populates 'sortsOut' with the sort orders provided by an index scan over 'index', with the given
 * 'bounds' and 'direction'.
 *
 * The caller must ensure that the set pointed to by 'sortsOut' is empty before calling this
 * function.
 */
void computeSortsForScan(const IndexEntry& index,
                         int direction,
                         const IndexBounds& bounds,
                         const CollatorInterface* queryCollator,
                         const std::set<StringData>& multikeyFields,
                         BSONObjSet* sortsOut) {
    invariant(sortsOut->empty());

    BSONObj sortPattern = QueryPlannerAnalysis::getSortPattern(index.keyPattern);
    if (direction == -1) {
        sortPattern = QueryPlannerCommon::reverseSortObj(sortPattern);
    }

    // If 'index' is the result of expanding a wildcard index, then its key pattern should look like
    // {$_path: 1, <field>: 1}. The "$_path" prefix stores the value of the path associated with the
    // key as opposed to real user data. We shouldn't report any sort orders including "$_path". In
    // fact, $-prefixed path components are illegal in queries in most contexts, so misinterpreting
    // this as a path in user-data could trigger subsequent assertions.
    if (index.type == IndexType::INDEX_WILDCARD) {
        invariant(bounds.fields.size() == 2u);
        // No sorts are provided if the bounds for '$_path' consist of multiple intervals. This can
        // happen for existence queries. For example, {a: {$exists: true}} results in bounds
        // [["a","a"], ["a.", "a/")] for '$_path' so that keys from documents where "a" is a nested
        // object are in bounds.
        if (bounds.fields[0].intervals.size() != 1u) {
            return;
        }

        // Strip '$_path' out of 'sortPattern' and then proceed with regular sort analysis.
        BSONObjIterator it{sortPattern};
        invariant(it.more());
        auto pathElement = it.next();
        invariant(pathElement.fieldNameStringData() == "$_path"_sd);
        invariant(it.more());
        auto secondElement = it.next();
        invariant(!it.more());
        sortPattern = BSONObjBuilder{}.append(secondElement).obj();
    }

    sortsOut->insert(sortPattern);

    const int nFields = sortPattern.nFields();
    if (nFields > 1) {
        // We're sorted not only by sortPattern but also by all prefixes of it.
        for (int i = 0; i < nFields; ++i) {
            // Make obj out of fields [0,i]
            BSONObjIterator it(sortPattern);
            BSONObjBuilder prefixBob;
            for (int j = 0; j <= i; ++j) {
                prefixBob.append(it.next());
            }
            sortsOut->insert(prefixBob.obj());
        }
    }

    // If we are using the index {a:1, b:1} to answer the predicate {a: 10}, it's sorted
    // both by the index key pattern and by the pattern {b: 1}.

    // See if there are any fields with equalities for bounds.  We can drop these
    // from any sort orders created.
    std::set<string> equalityFields;
    if (!bounds.isSimpleRange) {
        // Figure out how many fields are point intervals.
        for (size_t i = 0; i < bounds.fields.size(); ++i) {
            const OrderedIntervalList& oil = bounds.fields[i];
            if (oil.intervals.size() != 1) {
                continue;
            }
            const Interval& ival = oil.intervals[0];
            if (!ival.isPoint()) {
                continue;
            }
            equalityFields.insert(oil.name);
        }
    } else {
        BSONObjIterator keyIter(index.keyPattern);
        BSONObjIterator startIter(bounds.startKey);
        BSONObjIterator endIter(bounds.endKey);
        while (keyIter.more() && startIter.more() && endIter.more()) {
            BSONElement key = keyIter.next();
            if (SimpleBSONElementComparator::kInstance.evaluate(startIter.next() ==
                                                                endIter.next())) {
                equalityFields.insert(key.fieldName());
            }
        }
    }

    if (!equalityFields.empty()) {
        addEqualityFieldSorts(sortPattern, equalityFields, sortsOut);
    }

    if (!CollatorInterface::collatorsMatch(queryCollator, index.collator)) {
        // Prune sorts containing fields that don't match the collation.
        std::set<StringData> collatedFields =
            IndexScanNode::getFieldsWithStringBounds(bounds, index.keyPattern);
        auto sortsIt = sortsOut->begin();
        while (sortsIt != sortsOut->end()) {
            bool matched = false;
            for (const BSONElement& el : *sortsIt) {
                if (collatedFields.find(el.fieldNameStringData()) != collatedFields.end()) {
                    sortsIt = sortsOut->erase(sortsIt);
                    matched = true;
                    break;
                }
            }

            if (!matched) {
                sortsIt++;
            }
        }
    }

    if (index.multikey) {
        for (auto sortsIt = sortsOut->begin(); sortsIt != sortsOut->end();) {
            // Erase this sort from 'sortsOut' if it is not compatible with multikey fields in the
            // index.
            if (confirmBoundsProvideSortGivenMultikeyness(*sortsIt, bounds, multikeyFields)) {
                ++sortsIt;
            } else {
                sortsIt = sortsOut->erase(sortsIt);
            }
        }
    }
}

/**
 * Computes sort orders for index scans, including DISTINCT_SCAN. The 'sortsOut' set gets populated
 * with all the sort orders that will be provided by the index scan, and the 'multikeyFieldsOut'
 * field gets populated with the names of all fields that the index indicates are multikey.
 */
void computeSortsAndMultikeyPathsForScan(const IndexEntry& index,
                                         int direction,
                                         const IndexBounds& bounds,
                                         const CollatorInterface* queryCollator,
                                         BSONObjSet* sortsOut,
                                         std::set<StringData>* multikeyFieldsOut) {
    sortsOut->clear();
    multikeyFieldsOut->clear();

    // If the index is multikey but does not have path-level multikey metadata, then this index
    // cannot provide any sorts and we need not populate 'multikeyFieldsOut'.
    if (index.multikey && index.multikeyPaths.empty()) {
        return;
    }

    if (index.multikey) {
        *multikeyFieldsOut = getMultikeyFields(index.keyPattern, index.multikeyPaths);
    }

    computeSortsForScan(index, direction, bounds, queryCollator, *multikeyFieldsOut, sortsOut);
}

}  // namespace

void IndexScanNode::computeProperties() {
    computeSortsAndMultikeyPathsForScan(
        index, direction, bounds, queryCollator, &_sorts, &multikeyFields);
}

QuerySolutionNode* IndexScanNode::clone() const {
    IndexScanNode* copy = new IndexScanNode(this->index);
    cloneBaseData(copy);

    copy->_sorts = this->_sorts;
    copy->direction = this->direction;
    copy->addKeyMetadata = this->addKeyMetadata;
    copy->bounds = this->bounds;
    copy->queryCollator = this->queryCollator;

    return copy;
}

namespace {

bool filtersAreEquivalent(const MatchExpression* lhs, const MatchExpression* rhs) {
    if (!lhs && !rhs) {
        return true;
    } else if (!lhs || !rhs) {
        return false;
    } else {
        return lhs->equivalent(rhs);
    }
}

}  // namespace

bool IndexScanNode::operator==(const IndexScanNode& other) const {
    return filtersAreEquivalent(filter.get(), other.filter.get()) && index == other.index &&
        direction == other.direction && addKeyMetadata == other.addKeyMetadata &&
        bounds == other.bounds;
}

//
// ReturnKeyNode
//

void ReturnKeyNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "RETURN_KEY\n";
    addIndent(ss, indent + 1);

    *ss << "sortKeyMetaFields = ["
        << boost::algorithm::join(
               sortKeyMetaFields |
                   boost::adaptors::transformed([](const auto& field) { return field.fullPath(); }),
               ", ");
    *ss << "]\n";
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* ReturnKeyNode::clone() const {
    auto copy = std::make_unique<ReturnKeyNode>(
        std::unique_ptr<QuerySolutionNode>(children[0]->clone()), std::vector(sortKeyMetaFields));
    return copy.release();
}

//
// ProjectionNode
//

void ProjectionNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "PROJ\n";
    addIndent(ss, indent + 1);
    *ss << "proj = " << projection_ast::astToDebugBSON(proj.root()).toString() << '\n';
    addIndent(ss, indent + 1);
    *ss << "type = " << projectionImplementationTypeToString() << '\n';
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

void ProjectionNode::computeProperties() {
    invariant(children.size() == 1U);
    children[0]->computeProperties();

    _sorts.clear();

    const BSONObjSet& inputSorts = children[0]->getSort();

    // Our input sort is not necessarily maintained if we project some fields that are part of the
    // sort out.
    for (auto&& sort : inputSorts) {
        bool sortCompatible = true;
        for (auto&& key : sort) {
            if (!proj.isFieldRetainedExactly(key.fieldNameStringData())) {
                sortCompatible = false;
                break;
            }
        }
        if (sortCompatible) {
            _sorts.insert(sort);
        }
    }
}

void ProjectionNode::cloneProjectionData(ProjectionNode* copy) const {
    // ProjectionNode should not populate filter. This should be a no-op.
    if (this->filter)
        copy->filter = this->filter->shallowClone();

    copy->_sorts = this->_sorts;
}

ProjectionNode* ProjectionNodeDefault::clone() const {
    auto copy = std::make_unique<ProjectionNodeDefault>(
        std::unique_ptr<QuerySolutionNode>(children[0]->clone()), fullExpression, proj);
    ProjectionNode::cloneProjectionData(copy.get());
    return copy.release();
}

ProjectionNode* ProjectionNodeCovered::clone() const {
    auto copy = std::make_unique<ProjectionNodeCovered>(
        std::unique_ptr<QuerySolutionNode>(children[0]->clone()),
        fullExpression,
        proj,
        coveredKeyObj);
    ProjectionNode::cloneProjectionData(copy.get());
    return copy.release();
}

ProjectionNode* ProjectionNodeSimple::clone() const {
    auto copy = std::make_unique<ProjectionNodeSimple>(
        std::unique_ptr<QuerySolutionNode>(children[0]->clone()), fullExpression, proj);
    ProjectionNode::cloneProjectionData(copy.get());
    return copy.release();
}


//
// SortKeyGeneratorNode
//

void SortKeyGeneratorNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "SORT_KEY_GENERATOR\n";
    addIndent(ss, indent + 1);
    *ss << "sortSpec = " << sortSpec.toString() << '\n';
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* SortKeyGeneratorNode::clone() const {
    SortKeyGeneratorNode* copy = new SortKeyGeneratorNode();
    cloneBaseData(copy);
    copy->sortSpec = this->sortSpec;
    return copy;
}

//
// SortNode
//

void SortNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "SORT\n";
    addIndent(ss, indent + 1);
    *ss << "type = " << sortImplementationTypeToString() << '\n';
    addIndent(ss, indent + 1);
    *ss << "pattern = " << pattern.toString() << '\n';
    addIndent(ss, indent + 1);
    *ss << "limit = " << limit << '\n';
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

void SortNode::cloneSortData(SortNode* copy) const {
    cloneBaseData(copy);
    copy->_sorts = this->_sorts;
    copy->pattern = this->pattern;
    copy->limit = this->limit;
    copy->addSortKeyMetadata = this->addSortKeyMetadata;
}

QuerySolutionNode* SortNodeDefault::clone() const {
    auto copy = std::make_unique<SortNodeDefault>();
    cloneSortData(copy.get());
    return copy.release();
}

QuerySolutionNode* SortNodeSimple::clone() const {
    auto copy = std::make_unique<SortNodeSimple>();
    cloneSortData(copy.get());
    return copy.release();
}

//
// LimitNode
//


void LimitNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "LIMIT\n";
    addIndent(ss, indent + 1);
    *ss << "limit = " << limit << '\n';
    addIndent(ss, indent + 1);
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* LimitNode::clone() const {
    LimitNode* copy = new LimitNode();
    cloneBaseData(copy);

    copy->limit = this->limit;

    return copy;
}

//
// SkipNode
//

void SkipNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "SKIP\n";
    addIndent(ss, indent + 1);
    *ss << "skip= " << skip << '\n';
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* SkipNode::clone() const {
    SkipNode* copy = new SkipNode();
    cloneBaseData(copy);

    copy->skip = this->skip;

    return copy;
}

//
// GeoNear2DNode
//

void GeoNear2DNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "GEO_NEAR_2D\n";
    addIndent(ss, indent + 1);
    *ss << "name = " << index.identifier.catalogName << '\n';
    addIndent(ss, indent + 1);
    *ss << "keyPattern = " << index.keyPattern.toString() << '\n';
    addCommon(ss, indent);
    *ss << "nearQuery = " << nq->toString() << '\n';
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << " filter = " << filter->debugString();
    }
}

QuerySolutionNode* GeoNear2DNode::clone() const {
    GeoNear2DNode* copy = new GeoNear2DNode(this->index);
    cloneBaseData(copy);

    copy->_sorts = this->_sorts;
    copy->nq = this->nq;
    copy->baseBounds = this->baseBounds;
    copy->addPointMeta = this->addPointMeta;
    copy->addDistMeta = this->addDistMeta;

    return copy;
}

//
// GeoNear2DSphereNode
//

void GeoNear2DSphereNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "GEO_NEAR_2DSPHERE\n";
    addIndent(ss, indent + 1);
    *ss << "name = " << index.identifier.catalogName << '\n';
    addIndent(ss, indent + 1);
    *ss << "keyPattern = " << index.keyPattern.toString() << '\n';
    addCommon(ss, indent);
    *ss << "baseBounds = " << baseBounds.toString() << '\n';
    addIndent(ss, indent + 1);
    *ss << "nearQuery = " << nq->toString() << '\n';
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        *ss << " filter = " << filter->debugString();
    }
}

QuerySolutionNode* GeoNear2DSphereNode::clone() const {
    GeoNear2DSphereNode* copy = new GeoNear2DSphereNode(this->index);
    cloneBaseData(copy);

    copy->_sorts = this->_sorts;
    copy->nq = this->nq;
    copy->baseBounds = this->baseBounds;
    copy->addPointMeta = this->addPointMeta;
    copy->addDistMeta = this->addDistMeta;

    return copy;
}

//
// ShardingFilterNode
//

void ShardingFilterNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "SHARDING_FILTER\n";
    if (nullptr != filter) {
        addIndent(ss, indent + 1);
        StringBuilder sb;
        *ss << "filter:\n";
        filter->debugString(sb, indent + 2);
        *ss << sb.str();
    }
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* ShardingFilterNode::clone() const {
    ShardingFilterNode* copy = new ShardingFilterNode();
    cloneBaseData(copy);
    return copy;
}

//
// DistinctNode
//

void DistinctNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "DISTINCT\n";
    addIndent(ss, indent + 1);
    *ss << "name = " << index.identifier.catalogName << '\n';
    addIndent(ss, indent + 1);
    *ss << "keyPattern = " << index.keyPattern << '\n';
    addIndent(ss, indent + 1);
    *ss << "direction = " << direction << '\n';
    addIndent(ss, indent + 1);
    *ss << "bounds = " << bounds.toString() << '\n';
}

QuerySolutionNode* DistinctNode::clone() const {
    DistinctNode* copy = new DistinctNode(this->index);
    cloneBaseData(copy);

    copy->sorts = this->sorts;
    copy->direction = this->direction;
    copy->bounds = this->bounds;
    copy->queryCollator = this->queryCollator;
    copy->fieldNo = this->fieldNo;

    return copy;
}

void DistinctNode::computeProperties() {
    // Note that we don't need to save the 'multikeyFields' for a DISTINCT_SCAN. They are only
    // needed for explodeForSort(), which works on IXSCAN but not DISTINCT_SCAN.
    std::set<StringData> multikeyFields;
    computeSortsAndMultikeyPathsForScan(
        index, direction, bounds, queryCollator, &sorts, &multikeyFields);
}

//
// CountScanNode
//

void CountScanNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "COUNT\n";
    addIndent(ss, indent + 1);
    *ss << "name = " << index.identifier.catalogName << '\n';
    addIndent(ss, indent + 1);
    *ss << "keyPattern = " << index.keyPattern << '\n';
    addIndent(ss, indent + 1);
    *ss << "startKey = " << startKey << '\n';
    addIndent(ss, indent + 1);
    *ss << "endKey = " << endKey << '\n';
}

QuerySolutionNode* CountScanNode::clone() const {
    CountScanNode* copy = new CountScanNode(this->index);
    cloneBaseData(copy);

    copy->sorts = this->sorts;
    copy->startKey = this->startKey;
    copy->startKeyInclusive = this->startKeyInclusive;
    copy->endKey = this->endKey;
    copy->endKeyInclusive = this->endKeyInclusive;

    return copy;
}

//
// EnsureSortedNode
//

void EnsureSortedNode::appendToString(str::stream* ss, int indent) const {
    addIndent(ss, indent);
    *ss << "ENSURE_SORTED\n";
    addIndent(ss, indent + 1);
    *ss << "pattern = " << pattern.toString() << '\n';
    addCommon(ss, indent);
    addIndent(ss, indent + 1);
    *ss << "Child:" << '\n';
    children[0]->appendToString(ss, indent + 2);
}

QuerySolutionNode* EnsureSortedNode::clone() const {
    EnsureSortedNode* copy = new EnsureSortedNode();
    cloneBaseData(copy);

    copy->pattern = this->pattern;

    return copy;
}

}  // namespace mongo
