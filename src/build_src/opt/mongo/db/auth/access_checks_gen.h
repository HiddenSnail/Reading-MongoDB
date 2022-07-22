/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/access_checks_gen.h --output build/opt/mongo/db/auth/access_checks_gen.cpp src/mongo/db/auth/access_checks.idl
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
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/stdx/unordered_map.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {

/**
 * List of supported access checks in AuthorizationSession
 */
enum class AccessCheckEnum : std::int32_t {
    kCheckAuthorizedToListCollections,
    kCheckCursorSessionPrivilege,
    kClearImpersonatedUserData,
    kGetAuthenticatedRoleNames,
    kGetAuthenticatedUserName,
    kGetImpersonatedRoleNames,
    kGetImpersonatedUserName,
    kGetAuthenticatedUser,
    kIsAuthenticated,
    kIsAuthenticatedAsUserWithRole,
    kIsAuthorizedForAnyActionOnAnyResourceInDB,
    kIsAuthorizedForAnyActionOnResource,
    kIsAuthorizedToChangeAsUser,
    kIsAuthorizedToCreateRole,
    kIsAuthorizedToParseNamespaceElement,
    kIsCoAuthorized,
    kIsCoauthorizedWith,
    kIsCoauthorizedWithClient,
    kIsImpersonating,
    kLookupUser,
    kShouldIgnoreAuthChecks,
    kIsUsingLocalhostBypass,
};
static constexpr uint32_t kNumAccessCheckEnum = 22;

AccessCheckEnum AccessCheck_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData AccessCheck_serializer(AccessCheckEnum value);

}  // namespace mongo
