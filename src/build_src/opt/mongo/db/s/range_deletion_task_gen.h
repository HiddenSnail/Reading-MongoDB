/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/range_deletion_task_gen.h --output build/opt/mongo/db/s/range_deletion_task_gen.cpp src/mongo/db/s/range_deletion_task.idl
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
#include "mongo/crypto/sha256_block.h"
#include "mongo/db/logical_session_id_gen.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/catalog/type_chunk.h"
#include "mongo/s/shard_id.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * When to cleanup ranges.
 */
enum class CleanWhenEnum : std::int32_t {
    kNow ,
    kDelayed ,
};

CleanWhenEnum CleanWhen_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData CleanWhen_serializer(CleanWhenEnum value);

/**
 * Represents a chunk range to be deleted by the range deleter.
 */
class RangeDeletionTask {
public:
    static constexpr auto kCollectionUuidFieldName = "collectionUuid"_sd;
    static constexpr auto kDonorShardIdFieldName = "donorShardId"_sd;
    static constexpr auto kIdFieldName = "_id"_sd;
    static constexpr auto kNssFieldName = "nss"_sd;
    static constexpr auto kPendingFieldName = "pending"_sd;
    static constexpr auto kRangeFieldName = "range"_sd;
    static constexpr auto kWhenToCleanFieldName = "whenToClean"_sd;

    RangeDeletionTask();
    RangeDeletionTask(mongo::UUID id, mongo::NamespaceString nss, mongo::UUID collectionUuid, mongo::ShardId donorShardId, mongo::ChunkRange range, mongo::CleanWhenEnum whenToClean);

    static RangeDeletionTask parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * A unique identifier for the migration.
     */
    const mongo::UUID& getId() const { return _id; }
    void setId(mongo::UUID value) & {  _id = std::move(value); _hasId = true; }

    /**
     * The namespace of the collection that the chunk belongs to.
     */
    const mongo::NamespaceString& getNss() const { return _nss; }
    void setNss(mongo::NamespaceString value) & {  _nss = std::move(value); _hasNss = true; }

    /**
     * The UUID of the collection that the chunk belongs to.
     */
    const mongo::UUID& getCollectionUuid() const { return _collectionUuid; }
    void setCollectionUuid(mongo::UUID value) & {  _collectionUuid = std::move(value); _hasCollectionUuid = true; }

    /**
     * The shard from which the chunk was migrated.
     */
    const mongo::ShardId& getDonorShardId() const { return _donorShardId; }
    void setDonorShardId(mongo::ShardId value) & {  _donorShardId = std::move(value); _hasDonorShardId = true; }

    /**
     * The range to be deleted.
     */
    const mongo::ChunkRange& getRange() const { return _range; }
    void setRange(mongo::ChunkRange value) & {  _range = std::move(value); _hasRange = true; }

    /**
     * Flag that is present if the range is not yet ready for deletion
     */
    const boost::optional<bool> getPending() const& { return _pending; }
    void getPending() && = delete;
    void setPending(boost::optional<bool> value) & {  _pending = std::move(value);  }

    /**
     * Enumeration that defines when to cleanup the range.
     */
    mongo::CleanWhenEnum getWhenToClean() const { return _whenToClean; }
    void setWhenToClean(mongo::CleanWhenEnum value) & {  _whenToClean = std::move(value); _hasWhenToClean = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _id;
    mongo::NamespaceString _nss;
    mongo::UUID _collectionUuid;
    mongo::ShardId _donorShardId;
    mongo::ChunkRange _range;
    boost::optional<bool> _pending;
    mongo::CleanWhenEnum _whenToClean;
    bool _hasId : 1;
    bool _hasNss : 1;
    bool _hasCollectionUuid : 1;
    bool _hasDonorShardId : 1;
    bool _hasRange : 1;
    bool _hasWhenToClean : 1;
};

}  // namespace mongo
