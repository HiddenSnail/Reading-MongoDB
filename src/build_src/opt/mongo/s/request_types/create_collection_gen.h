/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/create_collection_gen.h --output build/opt/mongo/s/request_types/create_collection_gen.cpp src/mongo/s/request_types/create_collection.idl
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
 * The internal createCollection command on the config server
 */
class ConfigsvrCreateCollection {
public:
    static constexpr auto kCommandParameterFieldName = "_configsvrCreateCollection"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kOptionsFieldName = "options"_sd;
    static constexpr auto kCommandName = "_configsvrCreateCollection"_sd;

    explicit ConfigsvrCreateCollection(const mongo::NamespaceString commandParameter);

    static ConfigsvrCreateCollection parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ConfigsvrCreateCollection parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * collection creation options
     */
    const boost::optional<mongo::BSONObj>& getOptions() const& { return _options; }
    void getOptions() && = delete;
    void setOptions(boost::optional<mongo::BSONObj> value) & {  _options = std::move(value);  }

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

    boost::optional<mongo::BSONObj> _options;
    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
