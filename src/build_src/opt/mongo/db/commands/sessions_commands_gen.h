/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/sessions_commands_gen.h --output build/opt/mongo/db/commands/sessions_commands_gen.cpp src/mongo/db/commands/sessions_commands.idl
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
#include "mongo/crypto/sha256_block.h"
#include "mongo/db/logical_session_id_gen.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing an endSessions command from a client
 */
class EndSessionsCmdFromClient {
public:
    static constexpr auto kSessionsFieldName = "endSessions"_sd;

    EndSessionsCmdFromClient();
    EndSessionsCmdFromClient(std::vector<mongo::LogicalSessionFromClient> sessions);

    static EndSessionsCmdFromClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::LogicalSessionFromClient>& getSessions() const& { return _sessions; }
    void getSessions() && = delete;
    void setSessions(std::vector<mongo::LogicalSessionFromClient> value) & {  _sessions = std::move(value); _hasSessions = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::LogicalSessionFromClient> _sessions;
    bool _hasSessions : 1;
};

/**
 * A struct representing a refreshSessions command from a client
 */
class RefreshSessionsCmdFromClient {
public:
    static constexpr auto kSessionsFieldName = "refreshSessions"_sd;

    RefreshSessionsCmdFromClient();
    RefreshSessionsCmdFromClient(std::vector<mongo::LogicalSessionFromClient> sessions);

    static RefreshSessionsCmdFromClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::LogicalSessionFromClient>& getSessions() const& { return _sessions; }
    void getSessions() && = delete;
    void setSessions(std::vector<mongo::LogicalSessionFromClient> value) & {  _sessions = std::move(value); _hasSessions = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::LogicalSessionFromClient> _sessions;
    bool _hasSessions : 1;
};

}  // namespace mongo
