/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/catalog/type_chunk_base_gen.h --output build/opt/mongo/s/catalog/type_chunk_base_gen.cpp src/mongo/s/catalog/type_chunk_base.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/catalog/type_chunk_base_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ChunkHistoryBase::kShardFieldName;
constexpr StringData ChunkHistoryBase::kValidAfterFieldName;


ChunkHistoryBase::ChunkHistoryBase() : _hasValidAfter(false), _hasShard(false) {
    // Used for initialization only
}
ChunkHistoryBase::ChunkHistoryBase(mongo::Timestamp validAfter, mongo::ShardId shard) : _validAfter(std::move(validAfter)), _shard(std::move(shard)), _hasValidAfter(true), _hasShard(true) {
    // Used for initialization only
}


ChunkHistoryBase ChunkHistoryBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ChunkHistoryBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ChunkHistoryBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kValidAfterBit = 0;
    const size_t kShardBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kValidAfterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kValidAfterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidAfterBit);

                _hasValidAfter = true;
                _validAfter = element.timestamp();
            }
        }
        else if (fieldName == kShardFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kShardBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kShardBit);

                _hasShard = true;
                _shard = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kValidAfterBit]) {
            ctxt.throwMissingField(kValidAfterFieldName);
        }
        if (!usedFields[kShardBit]) {
            ctxt.throwMissingField(kShardFieldName);
        }
    }

}


void ChunkHistoryBase::serialize(BSONObjBuilder* builder) const {
    invariant(_hasValidAfter && _hasShard);

    builder->append(kValidAfterFieldName, _validAfter);

    {
        builder->append(kShardFieldName, _shard.toString());
    }

}


BSONObj ChunkHistoryBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
