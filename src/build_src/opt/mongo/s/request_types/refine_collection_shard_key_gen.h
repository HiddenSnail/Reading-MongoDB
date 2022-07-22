/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/refine_collection_shard_key_gen.h --output build/opt/mongo/s/request_types/refine_collection_shard_key_gen.cpp src/mongo/s/request_types/refine_collection_shard_key.idl
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
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * The public refineCollectionShardKey command on mongos
 */
class RefineCollectionShardKey {
public:
    static constexpr auto kCommandParameterFieldName = "refineCollectionShardKey"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kKeyFieldName = "key"_sd;
    static constexpr auto kCommandName = "refineCollectionShardKey"_sd;

    explicit RefineCollectionShardKey(const mongo::NamespaceString commandParameter);
    explicit RefineCollectionShardKey(const mongo::NamespaceString commandParameter, mongo::BSONObj key);

    static RefineCollectionShardKey parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static RefineCollectionShardKey parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The index specification document to use as the new shard key.
     */
    const mongo::BSONObj& getKey() const { return _key; }
    void setKey(mongo::BSONObj value) & {  _key = std::move(value); _hasKey = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    mongo::NamespaceString _commandParameter;

    mongo::BSONObj _key;
    std::string _dbName;
    bool _hasKey : 1;
    bool _hasDbName : 1;
};

/**
 * The internal refineCollectionShardKey command on the config server
 */
class ConfigsvrRefineCollectionShardKey {
public:
    static constexpr auto kCommandParameterFieldName = "_configsvrRefineCollectionShardKey"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kEpochFieldName = "epoch"_sd;
    static constexpr auto kKeyFieldName = "key"_sd;
    static constexpr auto kCommandName = "_configsvrRefineCollectionShardKey"_sd;

    explicit ConfigsvrRefineCollectionShardKey(const mongo::NamespaceString commandParameter);
    explicit ConfigsvrRefineCollectionShardKey(const mongo::NamespaceString commandParameter, mongo::BSONObj key, mongo::OID epoch);

    static ConfigsvrRefineCollectionShardKey parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ConfigsvrRefineCollectionShardKey parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The index specification document to use as the new shard key.
     */
    const mongo::BSONObj& getKey() const { return _key; }
    void setKey(mongo::BSONObj value) & {  _key = std::move(value); _hasKey = true; }

    /**
     * The expected epoch of the namespace provided to refineCollectionShardKey.
     */
    const mongo::OID& getEpoch() const { return _epoch; }
    void setEpoch(mongo::OID value) & {  _epoch = std::move(value); _hasEpoch = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    mongo::NamespaceString _commandParameter;

    mongo::BSONObj _key;
    mongo::OID _epoch;
    std::string _dbName;
    bool _hasKey : 1;
    bool _hasEpoch : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
