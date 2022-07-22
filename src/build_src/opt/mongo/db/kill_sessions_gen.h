/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/kill_sessions_gen.h --output build/opt/mongo/db/kill_sessions_gen.cpp src/mongo/db/kill_sessions.idl
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
 * A struct representing a killSessions command from a client
 */
class KillSessionsCmdFromClient {
public:
    static constexpr auto kKillSessionsFieldName = "killSessions"_sd;

    KillSessionsCmdFromClient();
    KillSessionsCmdFromClient(std::vector<mongo::LogicalSessionFromClient> killSessions);

    static KillSessionsCmdFromClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::LogicalSessionFromClient>& getKillSessions() const& { return _killSessions; }
    void getKillSessions() && = delete;
    void setKillSessions(std::vector<mongo::LogicalSessionFromClient> value) & {  _killSessions = std::move(value); _hasKillSessions = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::LogicalSessionFromClient> _killSessions;
    bool _hasKillSessions : 1;
};

/**
 * A struct representing a killAllSessions User
 */
class KillAllSessionsUser {
public:
    static constexpr auto kDbFieldName = "db"_sd;
    static constexpr auto kUserFieldName = "user"_sd;

    KillAllSessionsUser();
    KillAllSessionsUser(std::string user, std::string db);

    static KillAllSessionsUser parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getUser() const& { return _user; }
    void getUser() && = delete;
    void setUser(StringData value) & { auto _tmpValue = value.toString();  _user = std::move(_tmpValue); _hasUser = true; }

    const StringData getDb() const& { return _db; }
    void getDb() && = delete;
    void setDb(StringData value) & { auto _tmpValue = value.toString();  _db = std::move(_tmpValue); _hasDb = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _user;
    std::string _db;
    bool _hasUser : 1;
    bool _hasDb : 1;
};

/**
 * A struct representing a killAllSessions Role
 */
class KillAllSessionsRole {
public:
    static constexpr auto kDbFieldName = "db"_sd;
    static constexpr auto kRoleFieldName = "role"_sd;

    KillAllSessionsRole();
    KillAllSessionsRole(std::string role, std::string db);

    static KillAllSessionsRole parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const StringData getRole() const& { return _role; }
    void getRole() && = delete;
    void setRole(StringData value) & { auto _tmpValue = value.toString();  _role = std::move(_tmpValue); _hasRole = true; }

    const StringData getDb() const& { return _db; }
    void getDb() && = delete;
    void setDb(StringData value) & { auto _tmpValue = value.toString();  _db = std::move(_tmpValue); _hasDb = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _role;
    std::string _db;
    bool _hasRole : 1;
    bool _hasDb : 1;
};

/**
 * A struct representing a killAllSessions command
 */
class KillAllSessionsCmd {
public:
    static constexpr auto kKillAllSessionsFieldName = "killAllSessions"_sd;

    KillAllSessionsCmd();
    KillAllSessionsCmd(std::vector<mongo::KillAllSessionsUser> killAllSessions);

    static KillAllSessionsCmd parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::KillAllSessionsUser>& getKillAllSessions() const& { return _killAllSessions; }
    void getKillAllSessions() && = delete;
    void setKillAllSessions(std::vector<mongo::KillAllSessionsUser> value) & {  _killAllSessions = std::move(value); _hasKillAllSessions = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::KillAllSessionsUser> _killAllSessions;
    bool _hasKillAllSessions : 1;
};

/**
 * A struct representing a killAllSessionsByPatternCmd kill pattern
 */
class KillAllSessionsByPattern {
public:
    static constexpr auto kLsidFieldName = "lsid"_sd;
    static constexpr auto kRolesFieldName = "roles"_sd;
    static constexpr auto kUidFieldName = "uid"_sd;
    static constexpr auto kUsersFieldName = "users"_sd;

    KillAllSessionsByPattern();

    static KillAllSessionsByPattern parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const boost::optional<mongo::LogicalSessionId>& getLsid() const& { return _lsid; }
    void getLsid() && = delete;
    void setLsid(boost::optional<mongo::LogicalSessionId> value) & {  _lsid = std::move(value);  }

    const boost::optional<mongo::SHA256Block>& getUid() const& { return _uid; }
    void getUid() && = delete;
    void setUid(boost::optional<mongo::SHA256Block> value) & {  _uid = std::move(value);  }

    /**
     * logged in users for impersonate
     */
    const boost::optional<std::vector<mongo::KillAllSessionsUser>>& getUsers() const& { return _users; }
    void getUsers() && = delete;
    void setUsers(boost::optional<std::vector<mongo::KillAllSessionsUser>> value) & {  _users = std::move(value);  }

    /**
     * logged in roles for impersonate
     */
    const boost::optional<std::vector<mongo::KillAllSessionsRole>>& getRoles() const& { return _roles; }
    void getRoles() && = delete;
    void setRoles(boost::optional<std::vector<mongo::KillAllSessionsRole>> value) & {  _roles = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::LogicalSessionId> _lsid;
    boost::optional<mongo::SHA256Block> _uid;
    boost::optional<std::vector<mongo::KillAllSessionsUser>> _users;
    boost::optional<std::vector<mongo::KillAllSessionsRole>> _roles;
};

/**
 * A struct representing a killAllSessionsByPattern command
 */
class KillAllSessionsByPatternCmd {
public:
    static constexpr auto kKillAllSessionsByPatternFieldName = "killAllSessionsByPattern"_sd;

    KillAllSessionsByPatternCmd();
    KillAllSessionsByPatternCmd(std::vector<mongo::KillAllSessionsByPattern> killAllSessionsByPattern);

    static KillAllSessionsByPatternCmd parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::KillAllSessionsByPattern>& getKillAllSessionsByPattern() const& { return _killAllSessionsByPattern; }
    void getKillAllSessionsByPattern() && = delete;
    void setKillAllSessionsByPattern(std::vector<mongo::KillAllSessionsByPattern> value) & {  _killAllSessionsByPattern = std::move(value); _hasKillAllSessionsByPattern = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::KillAllSessionsByPattern> _killAllSessionsByPattern;
    bool _hasKillAllSessionsByPattern : 1;
};

/**
 * A struct representing a killSessions command to a server
 */
class KillSessionsCmdToServer {
public:
    static constexpr auto kKillSessionsFieldName = "killSessions"_sd;

    KillSessionsCmdToServer();
    KillSessionsCmdToServer(std::vector<mongo::LogicalSessionId> killSessions);

    static KillSessionsCmdToServer parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::LogicalSessionId>& getKillSessions() const& { return _killSessions; }
    void getKillSessions() && = delete;
    void setKillSessions(std::vector<mongo::LogicalSessionId> value) & {  _killSessions = std::move(value); _hasKillSessions = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::LogicalSessionId> _killSessions;
    bool _hasKillSessions : 1;
};

}  // namespace mongo
