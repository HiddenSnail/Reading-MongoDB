/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/flush_routing_table_cache_updates_gen.h --output build/opt/mongo/s/request_types/flush_routing_table_cache_updates_gen.cpp src/mongo/s/request_types/flush_routing_table_cache_updates.idl
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
 * An internal command to wait for the last routing table cache refresh for a particular namespace to be persisted to disk
 */
class _flushRoutingTableCacheUpdates {
public:
    static constexpr auto kCommandParameterFieldName = "_flushRoutingTableCacheUpdates"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kSyncFromConfigFieldName = "syncFromConfig"_sd;
    static constexpr auto kCommandName = "_flushRoutingTableCacheUpdates"_sd;

    explicit _flushRoutingTableCacheUpdates(const mongo::NamespaceString commandParameter);

    static _flushRoutingTableCacheUpdates parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static _flushRoutingTableCacheUpdates parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * If true, forces a refresh of the cache entry from the config server before waiting for updates to be persist
     */
    bool getSyncFromConfig() const { return _syncFromConfig; }
    void setSyncFromConfig(bool value) & {  _syncFromConfig = std::move(value);  }

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

    bool _syncFromConfig{true};
    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
