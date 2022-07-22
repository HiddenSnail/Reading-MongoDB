/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/rpc/metadata/impersonated_user_metadata_gen.h --output build/opt/mongo/rpc/metadata/impersonated_user_metadata_gen.cpp src/mongo/rpc/metadata/impersonated_user_metadata.idl
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
#include "mongo/db/auth/role_name.h"
#include "mongo/db/auth/user_name.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace rpc {

/**
 * A struct representing the impersonated users from a mongos
 */
class ImpersonatedUserMetadata {
public:
    static constexpr auto kRolesFieldName = "$impersonatedRoles"_sd;
    static constexpr auto kUsersFieldName = "$impersonatedUsers"_sd;

    ImpersonatedUserMetadata();
    ImpersonatedUserMetadata(std::vector<mongo::UserName> users, std::vector<mongo::RoleName> roles);

    static ImpersonatedUserMetadata parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::UserName>& getUsers() const& { return _users; }
    void getUsers() && = delete;
    void setUsers(std::vector<mongo::UserName> value) & {  _users = std::move(value); _hasUsers = true; }

    const std::vector<mongo::RoleName>& getRoles() const& { return _roles; }
    void getRoles() && = delete;
    void setRoles(std::vector<mongo::RoleName> value) & {  _roles = std::move(value); _hasRoles = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::UserName> _users;
    std::vector<mongo::RoleName> _roles;
    bool _hasUsers : 1;
    bool _hasRoles : 1;
};

}  // namespace rpc
}  // namespace mongo
