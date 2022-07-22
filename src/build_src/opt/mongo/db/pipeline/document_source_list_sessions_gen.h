/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_list_sessions_gen.h --output build/opt/mongo/db/pipeline/document_source_list_sessions_gen.cpp src/mongo/db/pipeline/document_source_list_sessions.idl
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
 * A struct representing a $listSessions/$listLocalSessions User
 */
class ListSessionsUser {
public:
    static constexpr auto kDbFieldName = "db"_sd;
    static constexpr auto kUserFieldName = "user"_sd;

    ListSessionsUser();
    ListSessionsUser(std::string user, std::string db);

    static ListSessionsUser parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
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
 * $listSessions and $listLocalSessions pipeline spec
 */
class ListSessionsSpec {
public:
    static constexpr auto kAllUsersFieldName = "allUsers"_sd;
    static constexpr auto kPredicateFieldName = "$_internalPredicate"_sd;
    static constexpr auto kUsersFieldName = "users"_sd;

    ListSessionsSpec();

    static ListSessionsSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    bool getAllUsers() const { return _allUsers; }
    void setAllUsers(bool value) & {  _allUsers = std::move(value);  }

    const boost::optional<std::vector<mongo::ListSessionsUser>>& getUsers() const& { return _users; }
    void getUsers() && = delete;
    void setUsers(boost::optional<std::vector<mongo::ListSessionsUser>> value) & {  _users = std::move(value);  }

    const boost::optional<mongo::BSONObj>& getPredicate() const& { return _predicate; }
    void getPredicate() && = delete;
    void setPredicate(boost::optional<mongo::BSONObj> value) & {  _predicate = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    bool _allUsers{false};
    boost::optional<std::vector<mongo::ListSessionsUser>> _users;
    boost::optional<mongo::BSONObj> _predicate;
};

}  // namespace mongo
