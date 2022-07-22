/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/clustered_collection_options_gen.h --output build/opt/mongo/db/catalog/clustered_collection_options_gen.cpp src/mongo/db/catalog/clustered_collection_options.idl
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
#include "mongo/db/tenant_id.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/basic_types_gen.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/stdx/unordered_map.h"
#include "mongo/util/uuid.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {

/**
 * The specifications for a given clusteredIndex
 */
class ClusteredIndexSpec {
public:
    static constexpr auto kKeyFieldName = "key"_sd;
    static constexpr auto kNameFieldName = "name"_sd;
    static constexpr auto kUniqueFieldName = "unique"_sd;
    static constexpr auto kVFieldName = "v"_sd;

    ClusteredIndexSpec();
    ClusteredIndexSpec(mongo::BSONObj key, bool unique);

    static ClusteredIndexSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Index spec version
     */
    std::int32_t getV() const { return _v; }
    void setV(std::int32_t value) {  _v = std::move(value);  }
    /**
     * Key to index on
     */
    const mongo::BSONObj& getKey() const { return _key; }
    void setKey(mongo::BSONObj value) {  _key = std::move(value); _hasKey = true; }
    /**
     * Descriptive name for the index
     */
    boost::optional<StringData> getName() const { return boost::optional<StringData>{_name}; }
    void setName(boost::optional<StringData> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _name = std::move(_tmpValue);
    } else {
        _name = boost::none;
    }
      }
    bool getUnique() const { return _unique; }
    void setUnique(bool value) {  _unique = std::move(value); _hasUnique = true; }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int32_t _v{2};
    mongo::BSONObj _key;
    boost::optional<std::string> _name;
    bool _unique;
    bool _hasKey : 1;
    bool _hasUnique : 1;
};

/**
 * Information on how a collection is clustered. For internal use only
 */
class ClusteredCollectionInfo {
public:
    static constexpr auto kIndexSpecFieldName = "indexSpec"_sd;
    static constexpr auto kLegacyFormatFieldName = "legacyFormat"_sd;

    ClusteredCollectionInfo();
    ClusteredCollectionInfo(mongo::ClusteredIndexSpec indexSpec, bool legacyFormat);

    static ClusteredCollectionInfo parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::ClusteredIndexSpec& getIndexSpec() const { return _indexSpec; }
    mongo::ClusteredIndexSpec& getIndexSpec() { return _indexSpec; }
    void setIndexSpec(mongo::ClusteredIndexSpec value) {  _indexSpec = std::move(value); _hasIndexSpec = true; }
    /**
     * Legacy format means the clustered information was specified as {clusteredIndex: true} and the cluster key defaults to _id. This should only be supported for certain internal collections (e.g: time-series buckets collections)
     */
    bool getLegacyFormat() const { return _legacyFormat; }
    void setLegacyFormat(bool value) {  _legacyFormat = std::move(value); _hasLegacyFormat = true; }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::ClusteredIndexSpec _indexSpec;
    bool _legacyFormat;
    bool _hasIndexSpec : 1;
    bool _hasLegacyFormat : 1;
};

constexpr auto kSupportArbitraryClusterKeyIndexDefault = false;

extern bool gSupportArbitraryClusterKeyIndex;
}  // namespace mongo
