/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/add_shard_cmd_gen.h --output build/opt/mongo/db/s/add_shard_cmd_gen.cpp src/mongo/db/s/add_shard_cmd.idl
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
#include "mongo/client/connection_string.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Contains all the information needed to identify a shard and lookup the shard identity document from storage
 */
class ShardIdentity {
public:
    static constexpr auto kClusterIdFieldName = "clusterId"_sd;
    static constexpr auto kConfigsvrConnectionStringFieldName = "configsvrConnectionString"_sd;
    static constexpr auto kShardNameFieldName = "shardName"_sd;

    ShardIdentity();
    ShardIdentity(std::string shardName, mongo::OID clusterId, mongo::ConnectionString configsvrConnectionString);

    static ShardIdentity parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The name of the shard
     */
    const StringData getShardName() const& { return _shardName; }
    void getShardName() && = delete;
    void setShardName(StringData value) & { auto _tmpValue = value.toString();  _shardName = std::move(_tmpValue); _hasShardName = true; }

    /**
     * The unique identifier of the cluster
     */
    const mongo::OID& getClusterId() const { return _clusterId; }
    void setClusterId(mongo::OID value) & {  _clusterId = std::move(value); _hasClusterId = true; }

    /**
     * Connection string to the config server
     */
    const mongo::ConnectionString& getConfigsvrConnectionString() const { return _configsvrConnectionString; }
    void setConfigsvrConnectionString(mongo::ConnectionString value) & {  _configsvrConnectionString = std::move(value); _hasConfigsvrConnectionString = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _shardName;
    mongo::OID _clusterId;
    mongo::ConnectionString _configsvrConnectionString;
    bool _hasShardName : 1;
    bool _hasClusterId : 1;
    bool _hasConfigsvrConnectionString : 1;
};

/**
 * _addShard Command
 */
class AddShard {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kShardIdentityFieldName = "shardIdentity"_sd;
    static constexpr auto kCommandName = "_addShard"_sd;

    AddShard();
    AddShard(mongo::ShardIdentity shardIdentity);

    static AddShard parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static AddShard parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * Identity metadata for the new shard
     */
    const mongo::ShardIdentity& getShardIdentity() const { return _shardIdentity; }
    mongo::ShardIdentity& getShardIdentity() { return _shardIdentity; }
    void setShardIdentity(mongo::ShardIdentity value) & {  _shardIdentity = std::move(value); _hasShardIdentity = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    mongo::ShardIdentity _shardIdentity;
    std::string _dbName;
    bool _hasShardIdentity : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
