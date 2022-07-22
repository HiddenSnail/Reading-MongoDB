/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/clone_collection_options_from_primary_shard_gen.h --output build/opt/mongo/s/request_types/clone_collection_options_from_primary_shard_gen.cpp src/mongo/s/request_types/clone_collection_options_from_primary_shard.idl
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
 * Internal command to create a collection on a non-primary shard with the collection options from the primary shard.
 */
class CloneCollectionOptionsFromPrimaryShard {
public:
    static constexpr auto kCommandParameterFieldName = "_cloneCollectionOptionsFromPrimaryShard"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kPrimaryShardFieldName = "primaryShard"_sd;
    static constexpr auto kCommandName = "_cloneCollectionOptionsFromPrimaryShard"_sd;

    explicit CloneCollectionOptionsFromPrimaryShard(const mongo::NamespaceString commandParameter);
    explicit CloneCollectionOptionsFromPrimaryShard(const mongo::NamespaceString commandParameter, std::string primaryShard);

    static CloneCollectionOptionsFromPrimaryShard parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static CloneCollectionOptionsFromPrimaryShard parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The primary shard to get the options from.
     */
    const StringData getPrimaryShard() const& { return _primaryShard; }
    void getPrimaryShard() && = delete;
    void setPrimaryShard(StringData value) & { auto _tmpValue = value.toString();  _primaryShard = std::move(_tmpValue); _hasPrimaryShard = true; }

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

    std::string _primaryShard;
    std::string _dbName;
    bool _hasPrimaryShard : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
