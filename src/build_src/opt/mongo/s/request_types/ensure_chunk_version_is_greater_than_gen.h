/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/ensure_chunk_version_is_greater_than_gen.h --output build/opt/mongo/s/request_types/ensure_chunk_version_is_greater_than_gen.cpp src/mongo/s/request_types/ensure_chunk_version_is_greater_than.idl
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
#include "mongo/s/chunk_version.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * If a chunk matching 'requestedChunk' exists, bumps the chunk's version to one greater than the current collection version.
 */
class ConfigsvrEnsureChunkVersionIsGreaterThan {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kMaxKeyFieldName = "maxKey"_sd;
    static constexpr auto kMinKeyFieldName = "minKey"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;
    static constexpr auto kCommandName = "_configsvrEnsureChunkVersionIsGreaterThan"_sd;

    ConfigsvrEnsureChunkVersionIsGreaterThan();
    ConfigsvrEnsureChunkVersionIsGreaterThan(mongo::BSONObj minKey, mongo::BSONObj maxKey, mongo::ChunkVersion version);

    static ConfigsvrEnsureChunkVersionIsGreaterThan parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ConfigsvrEnsureChunkVersionIsGreaterThan parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * The lower bound of the chunk.
     */
    const mongo::BSONObj& getMinKey() const { return _minKey; }
    void setMinKey(mongo::BSONObj value) & {  _minKey = std::move(value); _hasMinKey = true; }

    /**
     * The upper bound of the chunk.
     */
    const mongo::BSONObj& getMaxKey() const { return _maxKey; }
    void setMaxKey(mongo::BSONObj value) & {  _maxKey = std::move(value); _hasMaxKey = true; }

    /**
     * The version of the chunk, including major version, minor version, and epoch.
     */
    const mongo::ChunkVersion& getVersion() const { return _version; }
    void setVersion(mongo::ChunkVersion value) & {  _version = std::move(value); _hasVersion = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    mongo::BSONObj _minKey;
    mongo::BSONObj _maxKey;
    mongo::ChunkVersion _version;
    std::string _dbName;
    bool _hasMinKey : 1;
    bool _hasMaxKey : 1;
    bool _hasVersion : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
