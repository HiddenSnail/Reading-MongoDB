/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/clear_jumbo_flag_gen.h --output build/opt/mongo/s/request_types/clear_jumbo_flag_gen.cpp src/mongo/s/request_types/clear_jumbo_flag.idl
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
 * clearJumboFlag command for mongos
 */
class ClearJumboFlag {
public:
    static constexpr auto kCommandParameterFieldName = "clearJumboFlag"_sd;
    static constexpr auto kBoundsFieldName = "bounds"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFindFieldName = "find"_sd;
    static constexpr auto kCommandName = "clearJumboFlag"_sd;

    explicit ClearJumboFlag(const mongo::NamespaceString commandParameter);

    static ClearJumboFlag parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ClearJumboFlag parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The exact boundaries for a single chunk.
     */
    const boost::optional<std::vector<mongo::BSONObj>>& getBounds() const& { return _bounds; }
    void getBounds() && = delete;
    void setBounds(boost::optional<std::vector<mongo::BSONObj>> value) & {  _bounds = std::move(value);  }

    /**
     * The shard key value that is within a chunk's boundaries. Cannot be used on collections with hashed shard keys.
     */
    const boost::optional<mongo::BSONObj>& getFind() const& { return _find; }
    void getFind() && = delete;
    void setFind(boost::optional<mongo::BSONObj> value) & {  _find = std::move(value);  }

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

    boost::optional<std::vector<mongo::BSONObj>> _bounds;
    boost::optional<mongo::BSONObj> _find;
    std::string _dbName;
    bool _hasDbName : 1;
};

/**
 * internal clearJumboFlag command for config server
 */
class ConfigsvrClearJumboFlag {
public:
    static constexpr auto kCommandParameterFieldName = "_configsvrClearJumboFlag"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kEpochFieldName = "epoch"_sd;
    static constexpr auto kMaxKeyFieldName = "maxKey"_sd;
    static constexpr auto kMinKeyFieldName = "minKey"_sd;
    static constexpr auto kCommandName = "_configsvrClearJumboFlag"_sd;

    explicit ConfigsvrClearJumboFlag(const mongo::NamespaceString commandParameter);
    explicit ConfigsvrClearJumboFlag(const mongo::NamespaceString commandParameter, mongo::OID epoch, mongo::BSONObj minKey, mongo::BSONObj maxKey);

    static ConfigsvrClearJumboFlag parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ConfigsvrClearJumboFlag parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The expected epoch of the namespace provided to clearJumboFlag.
     */
    const mongo::OID& getEpoch() const { return _epoch; }
    void setEpoch(mongo::OID value) & {  _epoch = std::move(value); _hasEpoch = true; }

    /**
     * The lower bound key value of the chunk.
     */
    const mongo::BSONObj& getMinKey() const { return _minKey; }
    void setMinKey(mongo::BSONObj value) & {  _minKey = std::move(value); _hasMinKey = true; }

    /**
     * The upper bound key value of the chunk.
     */
    const mongo::BSONObj& getMaxKey() const { return _maxKey; }
    void setMaxKey(mongo::BSONObj value) & {  _maxKey = std::move(value); _hasMaxKey = true; }

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

    mongo::OID _epoch;
    mongo::BSONObj _minKey;
    mongo::BSONObj _maxKey;
    std::string _dbName;
    bool _hasEpoch : 1;
    bool _hasMinKey : 1;
    bool _hasMaxKey : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
