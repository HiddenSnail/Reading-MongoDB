/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/drop_connections_gen.h --output build/opt/mongo/db/commands/drop_connections_gen.cpp src/mongo/db/commands/drop_connections.idl
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
#include "mongo/util/net/hostandport.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * An administrative command which takes a list of host and ports and drops pooled connections to them
 */
class DropConnections {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kHostAndPortFieldName = "hostAndPort"_sd;
    static constexpr auto kCommandName = "dropConnections"_sd;

    DropConnections();
    DropConnections(std::vector<mongo::HostAndPort> hostAndPort);

    static DropConnections parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static DropConnections parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * List of host and ports
     */
    const std::vector<mongo::HostAndPort>& getHostAndPort() const& { return _hostAndPort; }
    void getHostAndPort() && = delete;
    void setHostAndPort(std::vector<mongo::HostAndPort> value) & {  _hostAndPort = std::move(value); _hasHostAndPort = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::vector<mongo::HostAndPort> _hostAndPort;
    std::string _dbName;
    bool _hasHostAndPort : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
