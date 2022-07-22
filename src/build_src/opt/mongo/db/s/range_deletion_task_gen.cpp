/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/range_deletion_task_gen.h --output build/opt/mongo/db/s/range_deletion_task_gen.cpp src/mongo/db/s/range_deletion_task.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/s/range_deletion_task_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * When to cleanup ranges.
 */
namespace  {
constexpr StringData kCleanWhen_kNow = "now"_sd;
constexpr StringData kCleanWhen_kDelayed = "delayed"_sd;
}  // namespace 

CleanWhenEnum CleanWhen_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kCleanWhen_kNow) {
        return CleanWhenEnum::kNow;
    }
    if (value == kCleanWhen_kDelayed) {
        return CleanWhenEnum::kDelayed;
    }
    ctxt.throwBadEnumValue(value);
}

StringData CleanWhen_serializer(CleanWhenEnum value) {
    if (value == CleanWhenEnum::kNow) {
        return kCleanWhen_kNow;
    }
    if (value == CleanWhenEnum::kDelayed) {
        return kCleanWhen_kDelayed;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData RangeDeletionTask::kCollectionUuidFieldName;
constexpr StringData RangeDeletionTask::kDonorShardIdFieldName;
constexpr StringData RangeDeletionTask::kIdFieldName;
constexpr StringData RangeDeletionTask::kNssFieldName;
constexpr StringData RangeDeletionTask::kPendingFieldName;
constexpr StringData RangeDeletionTask::kRangeFieldName;
constexpr StringData RangeDeletionTask::kWhenToCleanFieldName;


RangeDeletionTask::RangeDeletionTask() : _hasId(false), _hasNss(false), _hasCollectionUuid(false), _hasDonorShardId(false), _hasRange(false), _hasWhenToClean(false) {
    // Used for initialization only
}
RangeDeletionTask::RangeDeletionTask(mongo::UUID id, mongo::NamespaceString nss, mongo::UUID collectionUuid, mongo::ShardId donorShardId, mongo::ChunkRange range, mongo::CleanWhenEnum whenToClean) : _id(std::move(id)), _nss(std::move(nss)), _collectionUuid(std::move(collectionUuid)), _donorShardId(std::move(donorShardId)), _range(std::move(range)), _whenToClean(std::move(whenToClean)), _hasId(true), _hasNss(true), _hasCollectionUuid(true), _hasDonorShardId(true), _hasRange(true), _hasWhenToClean(true) {
    // Used for initialization only
}


RangeDeletionTask RangeDeletionTask::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RangeDeletionTask object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RangeDeletionTask::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _hasId = true;
                _id = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kNssFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasNss = true;
                _nss = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kCollectionUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _hasCollectionUuid = true;
                _collectionUuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kDonorShardIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDonorShardId = true;
                _donorShardId = element.str();
            }
        }
        else if (fieldName == kRangeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasRange = true;
                const BSONObj localObject = element.Obj();
                _range = ChunkRange::fromBSONThrowing(localObject);
            }
        }
        else if (fieldName == kPendingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _pending = element.boolean();
            }
        }
        else if (fieldName == kWhenToCleanFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasWhenToClean = true;
                IDLParserErrorContext tempContext(kWhenToCleanFieldName, &ctxt);
                _whenToClean = CleanWhen_parse(tempContext, element.valueStringData());
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kIdFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kNssFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kNssFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kCollectionUuidFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCollectionUuidFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDonorShardIdFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDonorShardIdFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kRangeFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kRangeFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kWhenToCleanFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kWhenToCleanFieldName);
    }

}


void RangeDeletionTask::serialize(BSONObjBuilder* builder) const {
    invariant(_hasId && _hasNss && _hasCollectionUuid && _hasDonorShardId && _hasRange && _hasWhenToClean);

    {
        ConstDataRange tempCDR = _id.toCDR();
        builder->append(kIdFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

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
        const BSONObj localObject = _range.toBSON();
        builder->append(kRangeFieldName, localObject);
    }

    if (_pending.is_initialized()) {
        builder->append(kPendingFieldName, _pending.get());
    }

    {
        builder->append(kWhenToCleanFieldName, CleanWhen_serializer(_whenToClean));
    }

}


BSONObj RangeDeletionTask::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
