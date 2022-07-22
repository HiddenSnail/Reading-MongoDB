/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/vote_commit_index_build_gen.h --output build/opt/mongo/db/commands/vote_commit_index_build_gen.cpp src/mongo/db/commands/vote_commit_index_build.idl
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
 * An internal mongod command pertaining to cross replica set index builds
 */
class VoteCommitIndexBuild {
public:
    static constexpr auto kCommandParameterFieldName = "voteCommitIndexBuild"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kHostAndPortFieldName = "hostAndPort"_sd;
    static constexpr auto kCommandName = "voteCommitIndexBuild"_sd;

    explicit VoteCommitIndexBuild(const mongo::UUID commandParameter);
    explicit VoteCommitIndexBuild(const mongo::UUID commandParameter, mongo::HostAndPort hostAndPort);

    static VoteCommitIndexBuild parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static VoteCommitIndexBuild parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::UUID& getCommandParameter() const { return _commandParameter; }

    /**
     * Replica set member identification
     */
    const mongo::HostAndPort& getHostAndPort() const { return _hostAndPort; }
    void setHostAndPort(mongo::HostAndPort value) & {  _hostAndPort = std::move(value); _hasHostAndPort = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    mongo::UUID _commandParameter;

    mongo::HostAndPort _hostAndPort;
    std::string _dbName;
    bool _hasHostAndPort : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
