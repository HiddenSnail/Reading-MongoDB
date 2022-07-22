/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/kill_operations_gen.h --output build/opt/mongo/db/commands/kill_operations_gen.cpp src/mongo/db/commands/kill_operations.idl
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
 * Interrupt a list of operations on a remote server
 */
class KillOperationsRequest {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kOperationKeysFieldName = "operationKeys"_sd;
    static constexpr auto kCommandName = "_killOperations"_sd;

    KillOperationsRequest();
    KillOperationsRequest(std::vector<mongo::UUID> operationKeys);

    static KillOperationsRequest parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static KillOperationsRequest parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * A list of (UUID) operation key
     */
    const std::vector<mongo::UUID>& getOperationKeys() const& { return _operationKeys; }
    void getOperationKeys() && = delete;
    void setOperationKeys(std::vector<mongo::UUID> value) & {  _operationKeys = std::move(value); _hasOperationKeys = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::vector<mongo::UUID> _operationKeys;
    std::string _dbName;
    bool _hasOperationKeys : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
