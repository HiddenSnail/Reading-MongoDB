/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/migration_coordinator_document_gen.h --output build/opt/mongo/db/s/migration_coordinator_document_gen.cpp src/mongo/db/s/migration_coordinator_document.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/s/migration_coordinator_document_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Whether the migration committed or aborted.
 */
namespace  {
constexpr StringData kDecision_kCommitted = "committed"_sd;
constexpr StringData kDecision_kAborted = "aborted"_sd;
}  // namespace 

DecisionEnum Decision_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kDecision_kCommitted) {
        return DecisionEnum::kCommitted;
    }
    if (value == kDecision_kAborted) {
        return DecisionEnum::kAborted;
    }
    ctxt.throwBadEnumValue(value);
}

StringData Decision_serializer(DecisionEnum value) {
    if (value == DecisionEnum::kCommitted) {
        return kDecision_kCommitted;
    }
    if (value == DecisionEnum::kAborted) {
        return kDecision_kAborted;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData MigrationCoordinatorDocument::kCollectionUuidFieldName;
constexpr StringData MigrationCoordinatorDocument::kDecisionFieldName;
constexpr StringData MigrationCoordinatorDocument::kDonorShardIdFieldName;
constexpr StringData MigrationCoordinatorDocument::kIdFieldName;
constexpr StringData MigrationCoordinatorDocument::kLsidFieldName;
constexpr StringData MigrationCoordinatorDocument::kMigrationSessionIdFieldName;
constexpr StringData MigrationCoordinatorDocument::kNssFieldName;
constexpr StringData MigrationCoordinatorDocument::kPreMigrationChunkVersionFieldName;
constexpr StringData MigrationCoordinatorDocument::kRangeFieldName;
constexpr StringData MigrationCoordinatorDocument::kRecipientShardIdFieldName;
constexpr StringData MigrationCoordinatorDocument::kTxnNumberFieldName;


MigrationCoordinatorDocument::MigrationCoordinatorDocument() : _txnNumber(-1), _hasId(false), _hasMigrationSessionId(false), _hasLsid(false), _hasTxnNumber(false), _hasNss(false), _hasCollectionUuid(false), _hasDonorShardId(false), _hasRecipientShardId(false), _hasRange(false), _hasPreMigrationChunkVersion(false) {
    // Used for initialization only
}
MigrationCoordinatorDocument::MigrationCoordinatorDocument(mongo::UUID id, mongo::MigrationSessionId migrationSessionId, mongo::LogicalSessionId lsid, std::int64_t txnNumber, mongo::NamespaceString nss, mongo::UUID collectionUuid, mongo::ShardId donorShardId, mongo::ShardId recipientShardId, mongo::ChunkRange range, mongo::ChunkVersion preMigrationChunkVersion) : _id(std::move(id)), _migrationSessionId(std::move(migrationSessionId)), _lsid(std::move(lsid)), _txnNumber(std::move(txnNumber)), _nss(std::move(nss)), _collectionUuid(std::move(collectionUuid)), _donorShardId(std::move(donorShardId)), _recipientShardId(std::move(recipientShardId)), _range(std::move(range)), _preMigrationChunkVersion(std::move(preMigrationChunkVersion)), _hasId(true), _hasMigrationSessionId(true), _hasLsid(true), _hasTxnNumber(true), _hasNss(true), _hasCollectionUuid(true), _hasDonorShardId(true), _hasRecipientShardId(true), _hasRange(true), _hasPreMigrationChunkVersion(true) {
    // Used for initialization only
}


MigrationCoordinatorDocument MigrationCoordinatorDocument::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    MigrationCoordinatorDocument object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void MigrationCoordinatorDocument::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<11> usedFields;
    const size_t kIdBit = 0;
    const size_t kMigrationSessionIdBit = 1;
    const size_t kLsidBit = 2;
    const size_t kTxnNumberBit = 3;
    const size_t kNssBit = 4;
    const size_t kCollectionUuidBit = 5;
    const size_t kDonorShardIdBit = 6;
    const size_t kRecipientShardIdBit = 7;
    const size_t kRangeBit = 8;
    const size_t kPreMigrationChunkVersionBit = 9;
    const size_t kDecisionBit = 10;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdBit);

                _hasId = true;
                _id = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kMigrationSessionIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kMigrationSessionIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMigrationSessionIdBit);

                _hasMigrationSessionId = true;
                _migrationSessionId = MigrationSessionId::fromString(element.valueStringData());
            }
        }
        else if (fieldName == kLsidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLsidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLsidBit);

                _hasLsid = true;
                IDLParserErrorContext tempContext(kLsidFieldName, &ctxt);
                const auto localObject = element.Obj();
                _lsid = mongo::LogicalSessionId::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kTxnNumberFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTxnNumberBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTxnNumberBit);

                _hasTxnNumber = true;
                _txnNumber = element._numberLong();
            }
        }
        else if (fieldName == kNssFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNssBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNssBit);

                _hasNss = true;
                _nss = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kCollectionUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kCollectionUuidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollectionUuidBit);

                _hasCollectionUuid = true;
                _collectionUuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kDonorShardIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDonorShardIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDonorShardIdBit);

                _hasDonorShardId = true;
                _donorShardId = element.str();
            }
        }
        else if (fieldName == kRecipientShardIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kRecipientShardIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecipientShardIdBit);

                _hasRecipientShardId = true;
                _recipientShardId = element.str();
            }
        }
        else if (fieldName == kRangeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kRangeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRangeBit);

                _hasRange = true;
                const BSONObj localObject = element.Obj();
                _range = ChunkRange::fromBSONThrowing(localObject);
            }
        }
        else if (fieldName == kPreMigrationChunkVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPreMigrationChunkVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPreMigrationChunkVersionBit);

                _hasPreMigrationChunkVersion = true;
                const BSONObj localObject = element.Obj();
                _preMigrationChunkVersion = ChunkVersion::fromBSONThrowing(localObject);
            }
        }
        else if (fieldName == kDecisionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDecisionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDecisionBit);

                IDLParserErrorContext tempContext(kDecisionFieldName, &ctxt);
                _decision = Decision_parse(tempContext, element.valueStringData());
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kIdBit]) {
            ctxt.throwMissingField(kIdFieldName);
        }
        if (!usedFields[kMigrationSessionIdBit]) {
            ctxt.throwMissingField(kMigrationSessionIdFieldName);
        }
        if (!usedFields[kLsidBit]) {
            ctxt.throwMissingField(kLsidFieldName);
        }
        if (!usedFields[kTxnNumberBit]) {
            ctxt.throwMissingField(kTxnNumberFieldName);
        }
        if (!usedFields[kNssBit]) {
            ctxt.throwMissingField(kNssFieldName);
        }
        if (!usedFields[kCollectionUuidBit]) {
            ctxt.throwMissingField(kCollectionUuidFieldName);
        }
        if (!usedFields[kDonorShardIdBit]) {
            ctxt.throwMissingField(kDonorShardIdFieldName);
        }
        if (!usedFields[kRecipientShardIdBit]) {
            ctxt.throwMissingField(kRecipientShardIdFieldName);
        }
        if (!usedFields[kRangeBit]) {
            ctxt.throwMissingField(kRangeFieldName);
        }
        if (!usedFields[kPreMigrationChunkVersionBit]) {
            ctxt.throwMissingField(kPreMigrationChunkVersionFieldName);
        }
    }

}


void MigrationCoordinatorDocument::serialize(BSONObjBuilder* builder) const {
    invariant(_hasId && _hasMigrationSessionId && _hasLsid && _hasTxnNumber && _hasNss && _hasCollectionUuid && _hasDonorShardId && _hasRecipientShardId && _hasRange && _hasPreMigrationChunkVersion);

    {
        ConstDataRange tempCDR = _id.toCDR();
        builder->append(kIdFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        builder->append(kMigrationSessionIdFieldName, _migrationSessionId.toString());
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kLsidFieldName));
        _lsid.serialize(&subObjBuilder);
    }

    builder->append(kTxnNumberFieldName, _txnNumber);

    {
        builder->append(kNssFieldName, _nss.toString());
    }

    {
        ConstDataRange tempCDR = _collectionUuid.toCDR();
        builder->append(kCollectionUuidFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        builder->append(kDonorShardIdFieldName, _donorShardId.toString());
    }

    {
        builder->append(kRecipientShardIdFieldName, _recipientShardId.toString());
    }

    {
        const BSONObj localObject = _range.toBSON();
        builder->append(kRangeFieldName, localObject);
    }

    {
        const BSONObj localObject = _preMigrationChunkVersion.toBSON();
        builder->append(kPreMigrationChunkVersionFieldName, localObject);
    }

    if (_decision.is_initialized()) {
        builder->append(kDecisionFieldName, Decision_serializer(_decision.get()));
    }

}


BSONObj MigrationCoordinatorDocument::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
