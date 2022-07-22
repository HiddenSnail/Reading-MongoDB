/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/catalog/type_shard_collection_gen.h --output build/opt/mongo/s/catalog/type_shard_collection_gen.cpp src/mongo/s/catalog/type_shard_collection.idl
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
#include "mongo/db/keypattern.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/chunk_version.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Represents the layout and contents of documents contained in the shard server's config.collections collection.
 */
class ShardCollectionTypeBase {
public:
    static constexpr auto kDefaultCollationFieldName = "defaultCollation"_sd;
    static constexpr auto kEnterCriticalSectionCounterFieldName = "enterCriticalSectionCounter"_sd;
    static constexpr auto kEpochFieldName = "epoch"_sd;
    static constexpr auto kKeyPatternFieldName = "key"_sd;
    static constexpr auto kLastRefreshedCollectionVersionFieldName = "lastRefreshedCollectionVersion"_sd;
    static constexpr auto kNssFieldName = "_id"_sd;
    static constexpr auto kRefreshingFieldName = "refreshing"_sd;
    static constexpr auto kUniqueFieldName = "unique"_sd;
    static constexpr auto kUuidFieldName = "uuid"_sd;

    ShardCollectionTypeBase();
    ShardCollectionTypeBase(mongo::NamespaceString nss, mongo::OID epoch, mongo::KeyPattern keyPattern, bool unique);

    static ShardCollectionTypeBase parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The full namespace (with the database prefix).
     */
    const mongo::NamespaceString& getNss() const { return _nss; }
    void setNss(mongo::NamespaceString value) & {  _nss = std::move(value); _hasNss = true; }

    /**
     * The UUID of the collection, if known.
     */
    const boost::optional<mongo::UUID>& getUuid() const& { return _uuid; }
    void getUuid() && = delete;
    void setUuid(boost::optional<mongo::UUID> value) & {  _uuid = std::move(value);  }

    /**
     * Uniquely identifies this instance of the collection, in case of drop/create or shard key refine.
     */
    const mongo::OID& getEpoch() const { return _epoch; }
    void setEpoch(mongo::OID value) & {  _epoch = std::move(value); _hasEpoch = true; }

    /**
     * Sharding key. If collection is dropped, this is no longer required.
     */
    const mongo::KeyPattern& getKeyPattern() const { return _keyPattern; }
    void setKeyPattern(mongo::KeyPattern value) & {  _keyPattern = std::move(value); _hasKeyPattern = true; }

    /**
     * Optional collection default collation. If empty, implies simple collation.
     */
    const mongo::BSONObj& getDefaultCollation() const { return _defaultCollation; }
    void setDefaultCollation(mongo::BSONObj value) & {  _defaultCollation = std::move(value);  }

    /**
     * Uniqueness of the sharding key.
     */
    bool getUnique() const { return _unique; }
    void setUnique(bool value) & {  _unique = std::move(value); _hasUnique = true; }

    /**
     * Set by primaries and used by shard secondaries to safely refresh chunk metadata. Indicates whether the chunks collection is current being updated, which means read results won't provide a complete view of the chunk metadata.
     */
    const boost::optional<bool> getRefreshing() const& { return _refreshing; }
    void getRefreshing() && = delete;
    void setRefreshing(boost::optional<bool> value) & {  _refreshing = std::move(value);  }

    /**
     * Set by primaries and used by shard secondaries to safely refresh chunk metadata. Indicates the collection version of the last complete chunk metadata refresh, and is used to indicate if a refresh occurred if the value is different than when the caller last checked -- because 'refreshing' will be false both before and after a refresh occurs.
     */
    const boost::optional<mongo::ChunkVersion>& getLastRefreshedCollectionVersion() const& { return _lastRefreshedCollectionVersion; }
    void getLastRefreshedCollectionVersion() && = delete;
    void setLastRefreshedCollectionVersion(boost::optional<mongo::ChunkVersion> value) & {  _lastRefreshedCollectionVersion = std::move(value);  }

    /**
     * Currently just an OpObserver signal, thus otherwise ignored.
     */
    const boost::optional<std::int32_t> getEnterCriticalSectionCounter() const& { return _enterCriticalSectionCounter; }
    void getEnterCriticalSectionCounter() && = delete;
    void setEnterCriticalSectionCounter(boost::optional<std::int32_t> value) & {  _enterCriticalSectionCounter = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::NamespaceString _nss;
    boost::optional<mongo::UUID> _uuid;
    mongo::OID _epoch;
    mongo::KeyPattern _keyPattern;
    mongo::BSONObj _defaultCollation{BSONObj()};
    bool _unique;
    boost::optional<bool> _refreshing;
    boost::optional<mongo::ChunkVersion> _lastRefreshedCollectionVersion;
    boost::optional<std::int32_t> _enterCriticalSectionCounter;
    bool _hasNss : 1;
    bool _hasEpoch : 1;
    bool _hasKeyPattern : 1;
    bool _hasUnique : 1;
};

}  // namespace mongo
