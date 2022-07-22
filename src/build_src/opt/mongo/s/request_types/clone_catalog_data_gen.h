/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/clone_catalog_data_gen.h --output build/opt/mongo/s/request_types/clone_catalog_data_gen.cpp src/mongo/s/request_types/clone_catalog_data.idl
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
 * The internal cloneCatalogData command on a shard
 */
class CloneCatalogData {
public:
    static constexpr auto kCommandParameterFieldName = "cloneCatalogData"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFromFieldName = "from"_sd;
    static constexpr auto kCommandName = "cloneCatalogData"_sd;

    explicit CloneCatalogData(const mongo::NamespaceString commandParameter);
    explicit CloneCatalogData(const mongo::NamespaceString commandParameter, std::string from);

    static CloneCatalogData parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static CloneCatalogData parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The connection string of the database to clone from.
     */
    const StringData getFrom() const& { return _from; }
    void getFrom() && = delete;
    void setFrom(StringData value) & { auto _tmpValue = value.toString();  _from = std::move(_tmpValue); _hasFrom = true; }

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

    std::string _from;
    std::string _dbName;
    bool _hasFrom : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
