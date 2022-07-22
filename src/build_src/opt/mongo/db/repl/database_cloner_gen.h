/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/database_cloner_gen.h --output build/opt/mongo/db/repl/database_cloner_gen.cpp src/mongo/db/repl/database_cloner.idl
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
#include "mongo/db/catalog/collection_options.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/repl/database_cloner.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace repl {

/**
 * A struct representing the 'info' field in the listCollections output.
 */
class CollectionInfo {
public:
    static constexpr auto kReadOnlyFieldName = "readOnly"_sd;
    static constexpr auto kUuidFieldName = "uuid"_sd;

    CollectionInfo();
    CollectionInfo(mongo::UUID uuid);

    static CollectionInfo parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const boost::optional<bool> getReadOnly() const& { return _readOnly; }
    void getReadOnly() && = delete;
    void setReadOnly(boost::optional<bool> value) & {  _readOnly = std::move(value);  }

    const mongo::UUID& getUuid() const { return _uuid; }
    void setUuid(mongo::UUID value) & {  _uuid = std::move(value); _hasUuid = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<bool> _readOnly;
    mongo::UUID _uuid;
    bool _hasUuid : 1;
};

/**
 * A struct representing the output of the listCollections command
 */
class ListCollectionResult {
public:
    static constexpr auto kInfoFieldName = "info"_sd;
    static constexpr auto kNameFieldName = "name"_sd;
    static constexpr auto kOptionsFieldName = "options"_sd;
    static constexpr auto kTypeFieldName = "type"_sd;

    ListCollectionResult();
    ListCollectionResult(std::string name, std::string type, mongo::CollectionOptions options, mongo::repl::CollectionInfo info);

    static ListCollectionResult parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The collection or view name.
     */
    const StringData getName() const& { return _name; }
    void getName() && = delete;
    void setName(StringData value) & { auto _tmpValue = value.toString();  _name = std::move(_tmpValue); _hasName = true; }

    /**
     * Either 'collection' or 'view'
     */
    const StringData getType() const& { return _type; }
    void getType() && = delete;
    void setType(StringData value) & { auto _tmpValue = value.toString();  _type = std::move(_tmpValue); _hasType = true; }

    /**
     * Collection options as passed to db.createCollection()
     */
    const mongo::CollectionOptions& getOptions() const { return _options; }
    mongo::CollectionOptions& getOptions() { return _options; }
    void setOptions(mongo::CollectionOptions value) & {  _options = std::move(value); _hasOptions = true; }

    const mongo::repl::CollectionInfo& getInfo() const { return _info; }
    mongo::repl::CollectionInfo& getInfo() { return _info; }
    void setInfo(mongo::repl::CollectionInfo value) & {  _info = std::move(value); _hasInfo = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _name;
    std::string _type;
    mongo::CollectionOptions _options;
    mongo::repl::CollectionInfo _info;
    bool _hasName : 1;
    bool _hasType : 1;
    bool _hasOptions : 1;
    bool _hasInfo : 1;
};

}  // namespace repl
}  // namespace mongo
