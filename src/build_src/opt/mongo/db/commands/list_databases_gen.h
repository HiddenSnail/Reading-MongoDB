/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/list_databases_gen.h --output build/opt/mongo/db/commands/list_databases_gen.cpp src/mongo/db/commands/list_databases.idl
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
 * listDatabases Command
 */
class ListDatabasesCommand {
public:
    static constexpr auto kAuthorizedDatabasesFieldName = "authorizedDatabases"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFilterFieldName = "filter"_sd;
    static constexpr auto kNameOnlyFieldName = "nameOnly"_sd;
    static constexpr auto kCommandName = "listDatabasesCommand"_sd;

    ListDatabasesCommand();

    static ListDatabasesCommand parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ListDatabasesCommand parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * Return just the database name without metadata
     */
    bool getNameOnly() const { return _nameOnly; }
    void setNameOnly(bool value) & {  _nameOnly = std::move(value);  }

    /**
     * Only return those databases for which the user is authorized
     */
    const boost::optional<bool> getAuthorizedDatabases() const& { return _authorizedDatabases; }
    void getAuthorizedDatabases() && = delete;
    void setAuthorizedDatabases(boost::optional<bool> value) & {  _authorizedDatabases = std::move(value);  }

    /**
     * Filter description to limit results
     */
    const boost::optional<mongo::BSONObj>& getFilter() const& { return _filter; }
    void getFilter() && = delete;
    void setFilter(boost::optional<mongo::BSONObj> value) & {  _filter = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    bool _nameOnly{false};
    boost::optional<bool> _authorizedDatabases;
    boost::optional<mongo::BSONObj> _filter;
    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
