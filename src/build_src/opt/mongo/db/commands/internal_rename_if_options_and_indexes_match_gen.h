/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/internal_rename_if_options_and_indexes_match_gen.h --output build/opt/mongo/db/commands/internal_rename_if_options_and_indexes_match_gen.cpp src/mongo/db/commands/internal_rename_if_options_and_indexes_match.idl
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
 * An internal command that does a rename, but first checks to make sure the indexes and collection options on the destination match those given in the command.
 */
class InternalRenameIfOptionsAndIndexesMatch {
public:
    static constexpr auto kCollectionOptionsFieldName = "collectionOptions"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFromFieldName = "from"_sd;
    static constexpr auto kIndexesFieldName = "indexes"_sd;
    static constexpr auto kToFieldName = "to"_sd;
    static constexpr auto kCommandName = "internalRenameIfOptionsAndIndexesMatch"_sd;

    InternalRenameIfOptionsAndIndexesMatch();
    InternalRenameIfOptionsAndIndexesMatch(mongo::NamespaceString from, mongo::NamespaceString to, mongo::BSONObj collectionOptions, std::vector<mongo::BSONObj> indexes);

    static InternalRenameIfOptionsAndIndexesMatch parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static InternalRenameIfOptionsAndIndexesMatch parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    const mongo::NamespaceString& getFrom() const { return _from; }
    void setFrom(mongo::NamespaceString value) & {  _from = std::move(value); _hasFrom = true; }

    const mongo::NamespaceString& getTo() const { return _to; }
    void setTo(mongo::NamespaceString value) & {  _to = std::move(value); _hasTo = true; }

    /**
     * An object representing the options on the from collection with the same format as the options from the listCollections command.
     */
    const mongo::BSONObj& getCollectionOptions() const { return _collectionOptions; }
    void setCollectionOptions(mongo::BSONObj value) & {  _collectionOptions = std::move(value); _hasCollectionOptions = true; }

    /**
     * An object with form {indexName: {spec}, indexName: {spec}, ...}
     */
    const std::vector<mongo::BSONObj>& getIndexes() const& { return _indexes; }
    void getIndexes() && = delete;
    void setIndexes(std::vector<mongo::BSONObj> value) & {  _indexes = std::move(value); _hasIndexes = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    mongo::NamespaceString _from;
    mongo::NamespaceString _to;
    mongo::BSONObj _collectionOptions;
    std::vector<mongo::BSONObj> _indexes;
    std::string _dbName;
    bool _hasFrom : 1;
    bool _hasTo : 1;
    bool _hasCollectionOptions : 1;
    bool _hasIndexes : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
