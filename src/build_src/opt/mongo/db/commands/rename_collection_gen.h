/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/rename_collection_gen.h --output build/opt/mongo/db/commands/rename_collection_gen.cpp src/mongo/db/commands/rename_collection.idl
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
 * Parser for the 'renameCollection' command.
 */
class RenameCollectionCommand {
public:
    static constexpr auto kCommandParameterFieldName = "renameCollection"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kDropTargetFieldName = "dropTarget"_sd;
    static constexpr auto kStayTempFieldName = "stayTemp"_sd;
    static constexpr auto kToFieldName = "to"_sd;
    static constexpr auto kCommandName = "renameCollection"_sd;

    explicit RenameCollectionCommand(const mongo::NamespaceString commandParameter);
    explicit RenameCollectionCommand(const mongo::NamespaceString commandParameter, mongo::NamespaceString to);

    static RenameCollectionCommand parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static RenameCollectionCommand parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    /**
     * The new namespace for the collection being renamed.
     */
    const mongo::NamespaceString& getTo() const { return _to; }
    void setTo(mongo::NamespaceString value) & {  _to = std::move(value); _hasTo = true; }

    /**
     * If true, mongod will drop the target of renameCollection prior to renaming the collection.
     */
    bool getDropTarget() const { return _dropTarget; }
    void setDropTarget(bool value) & {  _dropTarget = std::move(value);  }

    /**
     * If true, the original collection will remain temp if it was temp before the rename.
     */
    bool getStayTemp() const { return _stayTemp; }
    void setStayTemp(bool value) & {  _stayTemp = std::move(value);  }

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

    mongo::NamespaceString _to;
    bool _dropTarget{false};
    bool _stayTemp{false};
    std::string _dbName;
    bool _hasTo : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
