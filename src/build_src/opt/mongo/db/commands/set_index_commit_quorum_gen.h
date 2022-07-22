/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/set_index_commit_quorum_gen.h --output build/opt/mongo/db/commands/set_index_commit_quorum_gen.cpp src/mongo/db/commands/set_index_commit_quorum.idl
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
#include "mongo/db/catalog/commit_quorum_options.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/write_concern_options.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Resets the commitQuorum for an index build.
 */
class SetIndexCommitQuorum {
public:
    static constexpr auto kCommitQuorumFieldName = "commitQuorum"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kIndexNamesFieldName = "indexNames"_sd;
    static constexpr auto kCommandName = "setIndexCommitQuorum"_sd;

    explicit SetIndexCommitQuorum(const NamespaceString nss);
    SetIndexCommitQuorum(const NamespaceString nss, std::vector<std::string> indexNames, mongo::CommitQuorumOptions commitQuorum);

    static SetIndexCommitQuorum parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static SetIndexCommitQuorum parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const NamespaceString& getNamespace() const { return _nss; }

    /**
     * an array of all index names matching a single prior createIndexes request.
     */
    const std::vector<StringData> getIndexNames() const& { return transformVector(_indexNames); }
    void getIndexNames() && = delete;
    void setIndexNames(std::vector<StringData> value) & { auto _tmpValue = transformVector(value);  _indexNames = std::move(_tmpValue); _hasIndexNames = true; }

    /**
     * commitQuorum can be set to the same values as writeConcern.w and indicates how many and/or which replica set members are needed for the primary to commit the index build.
     */
    const mongo::CommitQuorumOptions& getCommitQuorum() const { return _commitQuorum; }
    void setCommitQuorum(mongo::CommitQuorumOptions value) & {  _commitQuorum = std::move(value); _hasCommitQuorum = true; }

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

    std::vector<std::string> _indexNames;
    mongo::CommitQuorumOptions _commitQuorum;
    std::string _dbName;
    bool _hasIndexNames : 1;
    bool _hasCommitQuorum : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
