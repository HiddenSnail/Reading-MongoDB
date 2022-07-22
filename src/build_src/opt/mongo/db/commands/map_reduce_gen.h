/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/map_reduce_gen.h --output build/opt/mongo/db/commands/map_reduce_gen.cpp src/mongo/db/commands/map_reduce.idl
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
#include "mongo/db/commands/map_reduce_global_variable_scope.h"
#include "mongo/db/commands/map_reduce_javascript_code.h"
#include "mongo/db/commands/map_reduce_out_options.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/write_concern_options.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * The MapReduce command.
 */
class MapReduce {
public:
    static constexpr auto kBypassDocumentValidationFieldName = "bypassDocumentValidation"_sd;
    static constexpr auto kCollationFieldName = "collation"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFinalizeFieldName = "finalize"_sd;
    static constexpr auto kJsModeFieldName = "jsMode"_sd;
    static constexpr auto kLimitFieldName = "limit"_sd;
    static constexpr auto kMapFieldName = "map"_sd;
    static constexpr auto kOutOptionsFieldName = "out"_sd;
    static constexpr auto kQueryFieldName = "query"_sd;
    static constexpr auto kReduceFieldName = "reduce"_sd;
    static constexpr auto kScopeFieldName = "scope"_sd;
    static constexpr auto kSortFieldName = "sort"_sd;
    static constexpr auto kVerboseFieldName = "verbose"_sd;
    static constexpr auto kCommandName = "MapReduce"_sd;

    explicit MapReduce(const NamespaceString nss);
    MapReduce(const NamespaceString nss, mongo::MapReduceJavascriptCode map, mongo::MapReduceJavascriptCode reduce, mongo::MapReduceOutOptions outOptions);

    static MapReduce parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static MapReduce parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const NamespaceString& getNamespace() const { return _nss; }

    /**
     * Javascript code to run as the map operation which associates a value with a key and emits the key and value pair.
     */
    const mongo::MapReduceJavascriptCode& getMap() const { return _map; }
    void setMap(mongo::MapReduceJavascriptCode value) & {  _map = std::move(value); _hasMap = true; }

    /**
     * Javascript code to run as the map operation which reduces all the values associated with a particular key to a single value.
     */
    const mongo::MapReduceJavascriptCode& getReduce() const { return _reduce; }
    void setReduce(mongo::MapReduceJavascriptCode value) & {  _reduce = std::move(value); _hasReduce = true; }

    /**
     * Out Options sub-object or string representing a collection.
     */
    const mongo::MapReduceOutOptions& getOutOptions() const { return _outOptions; }
    void setOutOptions(mongo::MapReduceOutOptions value) & {  _outOptions = std::move(value); _hasOutOptions = true; }

    /**
     * Query object in match language to use as a filter applied before the map step.
     */
    const boost::optional<mongo::BSONObj>& getQuery() const& { return _query; }
    void getQuery() && = delete;
    void setQuery(boost::optional<mongo::BSONObj> value) & {  _query = std::move(value);  }

    /**
     * Sort specification to apply before the map step.
     */
    const boost::optional<mongo::BSONObj>& getSort() const& { return _sort; }
    void getSort() && = delete;
    void setSort(boost::optional<mongo::BSONObj> value) & {  _sort = std::move(value);  }

    /**
     * collation specification which takes effect for 'query' and 'sort'.
     */
    const boost::optional<mongo::BSONObj>& getCollation() const& { return _collation; }
    void getCollation() && = delete;
    void setCollation(boost::optional<mongo::BSONObj> value) & {  _collation = std::move(value);  }

    /**
     * Document count limit to apply before the map step.
     */
    const boost::optional<std::int64_t> getLimit() const& { return _limit; }
    void getLimit() && = delete;
    void setLimit(boost::optional<std::int64_t> value) & {  _limit = std::move(value);  }

    /**
     * Javascript code to run after the reduce operation.
     */
    const boost::optional<mongo::MapReduceJavascriptCodeOrNull>& getFinalize() const& { return _finalize; }
    void getFinalize() && = delete;
    void setFinalize(boost::optional<mongo::MapReduceJavascriptCodeOrNull> value) & {  _finalize = std::move(value);  }

    /**
     * Javascript global variable mapping for map, reduce and finalize.
     */
    const boost::optional<mongo::MapReduceGlobalVariableScope>& getScope() const& { return _scope; }
    void getScope() && = delete;
    void setScope(boost::optional<mongo::MapReduceGlobalVariableScope> value) & {  _scope = std::move(value);  }

    /**
     * Specifies whether to include the timing information in the result information.
     */
    const boost::optional<bool> getVerbose() const& { return _verbose; }
    void getVerbose() && = delete;
    void setVerbose(boost::optional<bool> value) & {  _verbose = std::move(value);  }

    /**
     * Causes the out portion of the operation to ignore the output collection's document validation.
     */
    const boost::optional<bool> getBypassDocumentValidation() const& { return _bypassDocumentValidation; }
    void getBypassDocumentValidation() && = delete;
    void setBypassDocumentValidation(boost::optional<bool> value) & {  _bypassDocumentValidation = std::move(value);  }

    /**
     * Deprecated non-functional option which is ignored.
     */
    const boost::optional<bool> getJsMode() const& { return _jsMode; }
    void getJsMode() && = delete;
    void setJsMode(boost::optional<bool> value) & {  _jsMode = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    NamespaceString _nss;

    mongo::MapReduceJavascriptCode _map;
    mongo::MapReduceJavascriptCode _reduce;
    mongo::MapReduceOutOptions _outOptions;
    boost::optional<mongo::BSONObj> _query;
    boost::optional<mongo::BSONObj> _sort;
    boost::optional<mongo::BSONObj> _collation;
    boost::optional<std::int64_t> _limit;
    boost::optional<mongo::MapReduceJavascriptCodeOrNull> _finalize;
    boost::optional<mongo::MapReduceGlobalVariableScope> _scope;
    boost::optional<bool> _verbose;
    boost::optional<bool> _bypassDocumentValidation;
    boost::optional<bool> _jsMode;
    std::string _dbName;
    bool _hasMap : 1;
    bool _hasReduce : 1;
    bool _hasOutOptions : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
