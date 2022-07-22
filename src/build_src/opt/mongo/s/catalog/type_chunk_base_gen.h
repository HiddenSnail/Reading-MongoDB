/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/catalog/type_chunk_base_gen.h --output build/opt/mongo/s/catalog/type_chunk_base_gen.cpp src/mongo/s/catalog/type_chunk_base.idl
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
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/shard_id.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * An element of a chunk history array. The array holds the history ordered from the most recent (the index 0) to the oldest supported.
 */
class ChunkHistoryBase {
public:
    static constexpr auto kShardFieldName = "shard"_sd;
    static constexpr auto kValidAfterFieldName = "validAfter"_sd;

    ChunkHistoryBase();
    ChunkHistoryBase(mongo::Timestamp validAfter, mongo::ShardId shard);

    static ChunkHistoryBase parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The time after which this chunk is at this shard.
     */
    const mongo::Timestamp& getValidAfter() const { return _validAfter; }
    void setValidAfter(mongo::Timestamp value) & {  _validAfter = std::move(value); _hasValidAfter = true; }

    /**
     * Shard this chunk lives in, starting at the "validAfter" time.
     */
    const mongo::ShardId& getShard() const { return _shard; }
    void setShard(mongo::ShardId value) & {  _shard = std::move(value); _hasShard = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::Timestamp _validAfter;
    mongo::ShardId _shard;
    bool _hasValidAfter : 1;
    bool _hasShard : 1;
};

}  // namespace mongo
