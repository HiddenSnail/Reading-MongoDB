/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/txn_two_phase_commit_cmds_gen.h --output build/opt/mongo/db/commands/txn_two_phase_commit_cmds_gen.cpp src/mongo/db/commands/txn_two_phase_commit_cmds.idl
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
#include "mongo/s/shard_id.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Describes a shard participant in a transaction commit
 */
class CommitParticipant {
public:
    static constexpr auto kShardIdFieldName = "shardId"_sd;

    CommitParticipant();
    CommitParticipant(mongo::ShardId shardId);

    static CommitParticipant parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The id of the shard
     */
    const mongo::ShardId& getShardId() const { return _shardId; }
    void setShardId(mongo::ShardId value) & {  _shardId = std::move(value); _hasShardId = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::ShardId _shardId;
    bool _hasShardId : 1;
};

/**
 * Parser for the 'prepareTransaction' command.
 */
class PrepareTransaction {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "prepareTransaction"_sd;

    PrepareTransaction();

    static PrepareTransaction parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static PrepareTransaction parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::string _dbName;
    bool _hasDbName : 1;
};

/**
 * Parser for the 'coordinateCommitTransaction' command.
 */
class CoordinateCommitTransaction {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kParticipantsFieldName = "participants"_sd;
    static constexpr auto kCommandName = "coordinateCommitTransaction"_sd;

    CoordinateCommitTransaction();
    CoordinateCommitTransaction(std::vector<mongo::CommitParticipant> participants);

    static CoordinateCommitTransaction parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static CoordinateCommitTransaction parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * An array of shard participants that must be included in the commit.
     */
    const std::vector<mongo::CommitParticipant>& getParticipants() const& { return _participants; }
    void getParticipants() && = delete;
    void setParticipants(std::vector<mongo::CommitParticipant> value) & {  _participants = std::move(value); _hasParticipants = true; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::vector<mongo::CommitParticipant> _participants;
    std::string _dbName;
    bool _hasParticipants : 1;
    bool _hasDbName : 1;
};

}  // namespace mongo
