/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/migration_coordinator_document_gen.h --output build/opt/mongo/db/s/migration_coordinator_document_gen.cpp src/mongo/db/s/migration_coordinator_document.idl
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
#include "mongo/db/s/migration_session_id.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/catalog/type_chunk.h"
#include "mongo/s/chunk_version.h"
#include "mongo/s/shard_id.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Whether the migration committed or aborted.
 */
enum class DecisionEnum : std::int32_t {
    kCommitted ,
    kAborted ,
};

DecisionEnum Decision_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData Decision_serializer(DecisionEnum value);

/**
 * Represents an in-progress migration on the migration donor.
 */
class MigrationCoordinatorDocument {
public:
    static constexpr auto kCollectionUuidFieldName = "collectionUuid"_sd;
    static constexpr auto kDecisionFieldName = "decision"_sd;
    static constexpr auto kDonorShardIdFieldName = "donorShardId"_sd;
    static constexpr auto kIdFieldName = "_id"_sd;
    static constexpr auto kLsidFieldName = "lsid"_sd;
    static constexpr auto kMigrationSessionIdFieldName = "migrationSessionId"_sd;
    static constexpr auto kNssFieldName = "nss"_sd;
    static constexpr auto kPreMigrationChunkVersionFieldName = "preMigrationChunkVersion"_sd;
    static constexpr auto kRangeFieldName = "range"_sd;
    static constexpr auto kRecipientShardIdFieldName = "recipientShardId"_sd;
    static constexpr auto kTxnNumberFieldName = "txnNumber"_sd;

    MigrationCoordinatorDocument();
    MigrationCoordinatorDocument(mongo::UUID id, mongo::MigrationSessionId migrationSessionId, mongo::LogicalSessionId lsid, std::int64_t txnNumber, mongo::NamespaceString nss, mongo::UUID collectionUuid, mongo::ShardId donorShardId, mongo::ShardId recipientShardId, mongo::ChunkRange range, mongo::ChunkVersion preMigrationChunkVersion);

    static MigrationCoordinatorDocument parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * A unique identifier for the migration.
     */
    const mongo::UUID& getId() const { return _id; }
    void setId(mongo::UUID value) & {  _id = std::move(value); _hasId = true; }

    /**
     * A legacy unique identifier for the migration session.
     */
    const mongo::MigrationSessionId& getMigrationSessionId() const { return _migrationSessionId; }
    void setMigrationSessionId(mongo::MigrationSessionId value) & {  _migrationSessionId = std::move(value); _hasMigrationSessionId = true; }

    /**
     * The sessionId to use to communicate with the recipient
     */
    const mongo::LogicalSessionId& getLsid() const { return _lsid; }
    mongo::LogicalSessionId& getLsid() { return _lsid; }
    void setLsid(mongo::LogicalSessionId value) & {  _lsid = std::move(value); _hasLsid = true; }

    /**
     * The last txnNumber used to communicate with the recipient
     */
    std::int64_t getTxnNumber() const { return _txnNumber; }
    void setTxnNumber(std::int64_t value) & {  _txnNumber = std::move(value); _hasTxnNumber = true; }

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
     * The shard from which the chunk is being migrated.
     */
    const mongo::ShardId& getDonorShardId() const { return _donorShardId; }
    void setDonorShardId(mongo::ShardId value) & {  _donorShardId = std::move(value); _hasDonorShardId = true; }

    /**
     * The shard to which the chunk is being migrated.
     */
    const mongo::ShardId& getRecipientShardId() const { return _recipientShardId; }
    void setRecipientShardId(mongo::ShardId value) & {  _recipientShardId = std::move(value); _hasRecipientShardId = true; }

    /**
     * The range being migrated.
     */
    const mongo::ChunkRange& getRange() const { return _range; }
    void setRange(mongo::ChunkRange value) & {  _range = std::move(value); _hasRange = true; }

    /**
     * The version, at the start of the migration, of the chunk being moved.
     */
    const mongo::ChunkVersion& getPreMigrationChunkVersion() const { return _preMigrationChunkVersion; }
    void setPreMigrationChunkVersion(mongo::ChunkVersion value) & {  _preMigrationChunkVersion = std::move(value); _hasPreMigrationChunkVersion = true; }

    /**
     * Enumeration that defines whether the migration committed or aborted.
     */
    const boost::optional<mongo::DecisionEnum> getDecision() const& { return _decision; }
    void getDecision() && = delete;
    void setDecision(boost::optional<mongo::DecisionEnum> value) & {  _decision = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _id;
    mongo::MigrationSessionId _migrationSessionId;
    mongo::LogicalSessionId _lsid;
    std::int64_t _txnNumber;
    mongo::NamespaceString _nss;
    mongo::UUID _collectionUuid;
    mongo::ShardId _donorShardId;
    mongo::ShardId _recipientShardId;
    mongo::ChunkRange _range;
    mongo::ChunkVersion _preMigrationChunkVersion;
    boost::optional<mongo::DecisionEnum> _decision;
    bool _hasId : 1;
    bool _hasMigrationSessionId : 1;
    bool _hasLsid : 1;
    bool _hasTxnNumber : 1;
    bool _hasNss : 1;
    bool _hasCollectionUuid : 1;
    bool _hasDonorShardId : 1;
    bool _hasRecipientShardId : 1;
    bool _hasRange : 1;
    bool _hasPreMigrationChunkVersion : 1;
};

}  // namespace mongo
