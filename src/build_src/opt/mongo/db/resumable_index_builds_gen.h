/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/resumable_index_builds_gen.h --output build/opt/mongo/db/resumable_index_builds_gen.cpp src/mongo/db/resumable_index_builds.idl
 */

#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/data_range.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/simple_bsonobj_comparator.h"
#include "mongo/db/logical_time.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/record_id.h"
#include "mongo/db/sorter/sorter_gen.h"
#include "mongo/db/tenant_id.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/basic_types_gen.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/stdx/unordered_map.h"
#include "mongo/util/uuid.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {

/**
 * Phase of a hybrid index build
 */
enum class IndexBuildPhaseEnum : std::int32_t {
    kInitialized,
    kCollectionScan,
    kBulkLoad,
    kDrainWrites,
};
static constexpr uint32_t kNumIndexBuildPhaseEnum = 4;

IndexBuildPhaseEnum IndexBuildPhase_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData IndexBuildPhase_serializer(IndexBuildPhaseEnum value);

/**
 * One set of multikey components for the multikey paths.
 */
class MultikeyPath {
public:
    static constexpr auto kMultikeyComponentsFieldName = "multikeyComponents"_sd;

    MultikeyPath();
    MultikeyPath(std::vector<std::int32_t> multikeyComponents);

    static MultikeyPath parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The path-level components of the multikey paths.
     */
    const std::vector<std::int32_t>& getMultikeyComponents() const { return _multikeyComponents; }
    void setMultikeyComponents(std::vector<std::int32_t> value) {  _multikeyComponents = std::move(value); _hasMultikeyComponents = true; }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<std::int32_t> _multikeyComponents;
    bool _hasMultikeyComponents : 1;
};

/**
 * The state of the index build needed to resume it.
 */
class IndexStateInfo {
public:
    static constexpr auto kDuplicateKeyTrackerTableFieldName = "duplicateKeyTrackerTable"_sd;
    static constexpr auto kFileNameFieldName = "fileName"_sd;
    static constexpr auto kIsMultikeyFieldName = "isMultikey"_sd;
    static constexpr auto kMultikeyPathsFieldName = "multikeyPaths"_sd;
    static constexpr auto kNumKeysFieldName = "numKeys"_sd;
    static constexpr auto kRangesFieldName = "ranges"_sd;
    static constexpr auto kSideWritesTableFieldName = "sideWritesTable"_sd;
    static constexpr auto kSkippedRecordTrackerTableFieldName = "skippedRecordTrackerTable"_sd;
    static constexpr auto kSpecFieldName = "spec"_sd;

    IndexStateInfo();
    IndexStateInfo(std::string sideWritesTable, mongo::BSONObj spec, bool isMultikey, std::vector<mongo::MultikeyPath> multikeyPaths);

    static IndexStateInfo parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The name of the ident associated with the side writes table for this index build
     */
    StringData getSideWritesTable() const { return _sideWritesTable; }
    void setSideWritesTable(StringData value) { auto _tmpValue = value.toString();  _sideWritesTable = std::move(_tmpValue); _hasSideWritesTable = true; }
    /**
     * The name of the ident associated with the duplicate key tracker table for this index build
     */
    boost::optional<StringData> getDuplicateKeyTrackerTable() const { return boost::optional<StringData>{_duplicateKeyTrackerTable}; }
    void setDuplicateKeyTrackerTable(boost::optional<StringData> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _duplicateKeyTrackerTable = std::move(_tmpValue);
    } else {
        _duplicateKeyTrackerTable = boost::none;
    }
      }
    /**
     * The name of the ident associated with the skipped record tracker table for this index build
     */
    boost::optional<StringData> getSkippedRecordTrackerTable() const { return boost::optional<StringData>{_skippedRecordTrackerTable}; }
    void setSkippedRecordTrackerTable(boost::optional<StringData> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _skippedRecordTrackerTable = std::move(_tmpValue);
    } else {
        _skippedRecordTrackerTable = boost::none;
    }
      }
    /**
     * The name of the file that sorted data is written to
     */
    boost::optional<StringData> getFileName() const { return boost::optional<StringData>{_fileName}; }
    void setFileName(boost::optional<StringData> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _fileName = std::move(_tmpValue);
    } else {
        _fileName = boost::none;
    }
      }
    /**
     * The number of keys that were that were already sorted and spilled to disk
     */
    boost::optional<std::int64_t> getNumKeys() const { return _numKeys; }
    void setNumKeys(boost::optional<std::int64_t> value) {  _numKeys = std::move(value);  }
    /**
     * All ranges of data that were already sorted and spilled to disk
     */
    const boost::optional<std::vector<mongo::SorterRange>>& getRanges() const { return _ranges; }
    boost::optional<std::vector<mongo::SorterRange>>& getRanges() { return _ranges; }
    void setRanges(boost::optional<std::vector<mongo::SorterRange>> value) {  _ranges = std::move(value);  }
    /**
     * The index specification
     */
    const mongo::BSONObj& getSpec() const { return _spec; }
    void setSpec(mongo::BSONObj value) {  _spec = std::move(value); _hasSpec = true; }
    /**
     * Whether the index was already marked as multikey.
     */
    bool getIsMultikey() const { return _isMultikey; }
    void setIsMultikey(bool value) {  _isMultikey = std::move(value); _hasIsMultikey = true; }
    /**
     * The multikey paths for the index, if supported.
     */
    const std::vector<mongo::MultikeyPath>& getMultikeyPaths() const { return _multikeyPaths; }
    std::vector<mongo::MultikeyPath>& getMultikeyPaths() { return _multikeyPaths; }
    void setMultikeyPaths(std::vector<mongo::MultikeyPath> value) {  _multikeyPaths = std::move(value); _hasMultikeyPaths = true; }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _sideWritesTable;
    boost::optional<std::string> _duplicateKeyTrackerTable;
    boost::optional<std::string> _skippedRecordTrackerTable;
    boost::optional<std::string> _fileName;
    boost::optional<std::int64_t> _numKeys;
    boost::optional<std::vector<mongo::SorterRange>> _ranges;
    mongo::BSONObj _spec;
    bool _isMultikey;
    std::vector<mongo::MultikeyPath> _multikeyPaths;
    bool _hasSideWritesTable : 1;
    bool _hasSpec : 1;
    bool _hasIsMultikey : 1;
    bool _hasMultikeyPaths : 1;
};

/**
 * Information needed to resume index builds
 */
class ResumeIndexInfo {
public:
    static constexpr auto kBuildUUIDFieldName = "_id"_sd;
    static constexpr auto kCollectionScanPositionFieldName = "collectionScanPosition"_sd;
    static constexpr auto kCollectionUUIDFieldName = "collectionUUID"_sd;
    static constexpr auto kIndexesFieldName = "indexes"_sd;
    static constexpr auto kPhaseFieldName = "phase"_sd;

    ResumeIndexInfo();
    ResumeIndexInfo(mongo::UUID buildUUID, mongo::IndexBuildPhaseEnum phase, mongo::UUID collectionUUID, std::vector<mongo::IndexStateInfo> indexes);

    static ResumeIndexInfo parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * A UUID that uniquely identifies the index build across replica set members.
     */
    const mongo::UUID& getBuildUUID() const { return _buildUUID; }
    void setBuildUUID(mongo::UUID value) {  _buildUUID = std::move(value); _hasBuildUUID = true; }
    /**
     * The phase the index build was in when the node shut down
     */
    mongo::IndexBuildPhaseEnum getPhase() const { return _phase; }
    void setPhase(mongo::IndexBuildPhaseEnum value) {  _phase = std::move(value); _hasPhase = true; }
    /**
     * A UUID that uniquely identifies which collection the index is being built on
     */
    const mongo::UUID& getCollectionUUID() const { return _collectionUUID; }
    void setCollectionUUID(mongo::UUID value) {  _collectionUUID = std::move(value); _hasCollectionUUID = true; }
    /**
     * The last record id inserted into the sorter before shutdown
     */
    const boost::optional<mongo::RecordId>& getCollectionScanPosition() const { return _collectionScanPosition; }
    void setCollectionScanPosition(boost::optional<mongo::RecordId> value) {  _collectionScanPosition = std::move(value);  }
    /**
     * The information needed to resume each specific index in this build
     */
    const std::vector<mongo::IndexStateInfo>& getIndexes() const { return _indexes; }
    std::vector<mongo::IndexStateInfo>& getIndexes() { return _indexes; }
    void setIndexes(std::vector<mongo::IndexStateInfo> value) {  _indexes = std::move(value); _hasIndexes = true; }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _buildUUID;
    mongo::IndexBuildPhaseEnum _phase;
    mongo::UUID _collectionUUID;
    boost::optional<mongo::RecordId> _collectionScanPosition;
    std::vector<mongo::IndexStateInfo> _indexes;
    bool _hasBuildUUID : 1;
    bool _hasPhase : 1;
    bool _hasCollectionUUID : 1;
    bool _hasIndexes : 1;
};

}  // namespace mongo
