/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/access_checks_gen.h --output build/opt/mongo/db/auth/access_checks_gen.cpp src/mongo/db/auth/access_checks.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/auth/access_checks_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

/**
 * List of supported access checks in AuthorizationSession
 */
namespace  {
constexpr StringData kAccessCheck_kCheckAuthorizedToListCollections = "check_authorized_to_list_collections"_sd;
constexpr StringData kAccessCheck_kCheckCursorSessionPrivilege = "check_cursor_session_privilege"_sd;
constexpr StringData kAccessCheck_kClearImpersonatedUserData = "clear_impersonated_user_data"_sd;
constexpr StringData kAccessCheck_kGetAuthenticatedRoleNames = "get_authenticated_role_names"_sd;
constexpr StringData kAccessCheck_kGetAuthenticatedUserName = "get_authenticated_user_name"_sd;
constexpr StringData kAccessCheck_kGetImpersonatedRoleNames = "get_impersonated_role_names"_sd;
constexpr StringData kAccessCheck_kGetImpersonatedUserName = "get_impersonated_user_name"_sd;
constexpr StringData kAccessCheck_kGetAuthenticatedUser = "get_authenticated_user"_sd;
constexpr StringData kAccessCheck_kIsAuthenticated = "is_authenticated"_sd;
constexpr StringData kAccessCheck_kIsAuthenticatedAsUserWithRole = "is_authenticated_as_user_with_role"_sd;
constexpr StringData kAccessCheck_kIsAuthorizedForAnyActionOnAnyResourceInDB = "is_authorized_for_any_action_on_any_resource_in_db"_sd;
constexpr StringData kAccessCheck_kIsAuthorizedForAnyActionOnResource = "is_authorized_for_any_action_on_resource"_sd;
constexpr StringData kAccessCheck_kIsAuthorizedToChangeAsUser = "is_authorized_to_change_as_user"_sd;
constexpr StringData kAccessCheck_kIsAuthorizedToCreateRole = "is_authorized_to_create_role"_sd;
constexpr StringData kAccessCheck_kIsAuthorizedToParseNamespaceElement = "is_authorized_to_parse_namespace_element"_sd;
constexpr StringData kAccessCheck_kIsCoAuthorized = "is_coauthorized"_sd;
constexpr StringData kAccessCheck_kIsCoauthorizedWith = "is_coauthorized_with"_sd;
constexpr StringData kAccessCheck_kIsCoauthorizedWithClient = "is_coauthorized_with_client"_sd;
constexpr StringData kAccessCheck_kIsImpersonating = "is_impersonating"_sd;
constexpr StringData kAccessCheck_kLookupUser = "lookup_user"_sd;
constexpr StringData kAccessCheck_kShouldIgnoreAuthChecks = "should_ignore_auth_checks"_sd;
constexpr StringData kAccessCheck_kIsUsingLocalhostBypass = "is_using_localhost_bypass"_sd;
}  // namespace 

AccessCheckEnum AccessCheck_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kAccessCheck_kCheckAuthorizedToListCollections) {
        return AccessCheckEnum::kCheckAuthorizedToListCollections;
    }
    if (value == kAccessCheck_kCheckCursorSessionPrivilege) {
        return AccessCheckEnum::kCheckCursorSessionPrivilege;
    }
    if (value == kAccessCheck_kClearImpersonatedUserData) {
        return AccessCheckEnum::kClearImpersonatedUserData;
    }
    if (value == kAccessCheck_kGetAuthenticatedRoleNames) {
        return AccessCheckEnum::kGetAuthenticatedRoleNames;
    }
    if (value == kAccessCheck_kGetAuthenticatedUserName) {
        return AccessCheckEnum::kGetAuthenticatedUserName;
    }
    if (value == kAccessCheck_kGetImpersonatedRoleNames) {
        return AccessCheckEnum::kGetImpersonatedRoleNames;
    }
    if (value == kAccessCheck_kGetImpersonatedUserName) {
        return AccessCheckEnum::kGetImpersonatedUserName;
    }
    if (value == kAccessCheck_kGetAuthenticatedUser) {
        return AccessCheckEnum::kGetAuthenticatedUser;
    }
    if (value == kAccessCheck_kIsAuthenticated) {
        return AccessCheckEnum::kIsAuthenticated;
    }
    if (value == kAccessCheck_kIsAuthenticatedAsUserWithRole) {
        return AccessCheckEnum::kIsAuthenticatedAsUserWithRole;
    }
    if (value == kAccessCheck_kIsAuthorizedForAnyActionOnAnyResourceInDB) {
        return AccessCheckEnum::kIsAuthorizedForAnyActionOnAnyResourceInDB;
    }
    if (value == kAccessCheck_kIsAuthorizedForAnyActionOnResource) {
        return AccessCheckEnum::kIsAuthorizedForAnyActionOnResource;
    }
    if (value == kAccessCheck_kIsAuthorizedToChangeAsUser) {
        return AccessCheckEnum::kIsAuthorizedToChangeAsUser;
    }
    if (value == kAccessCheck_kIsAuthorizedToCreateRole) {
        return AccessCheckEnum::kIsAuthorizedToCreateRole;
    }
    if (value == kAccessCheck_kIsAuthorizedToParseNamespaceElement) {
        return AccessCheckEnum::kIsAuthorizedToParseNamespaceElement;
    }
    if (value == kAccessCheck_kIsCoAuthorized) {
        return AccessCheckEnum::kIsCoAuthorized;
    }
    if (value == kAccessCheck_kIsCoauthorizedWith) {
        return AccessCheckEnum::kIsCoauthorizedWith;
    }
    if (value == kAccessCheck_kIsCoauthorizedWithClient) {
        return AccessCheckEnum::kIsCoauthorizedWithClient;
    }
    if (value == kAccessCheck_kIsImpersonating) {
        return AccessCheckEnum::kIsImpersonating;
    }
    if (value == kAccessCheck_kLookupUser) {
        return AccessCheckEnum::kLookupUser;
    }
    if (value == kAccessCheck_kShouldIgnoreAuthChecks) {
        return AccessCheckEnum::kShouldIgnoreAuthChecks;
    }
    if (value == kAccessCheck_kIsUsingLocalhostBypass) {
        return AccessCheckEnum::kIsUsingLocalhostBypass;
    }
    ctxt.throwBadEnumValue(value);
}

StringData AccessCheck_serializer(AccessCheckEnum value) {
    if (value == AccessCheckEnum::kCheckAuthorizedToListCollections) {
        return kAccessCheck_kCheckAuthorizedToListCollections;
    }
    if (value == AccessCheckEnum::kCheckCursorSessionPrivilege) {
        return kAccessCheck_kCheckCursorSessionPrivilege;
    }
    if (value == AccessCheckEnum::kClearImpersonatedUserData) {
        return kAccessCheck_kClearImpersonatedUserData;
    }
    if (value == AccessCheckEnum::kGetAuthenticatedRoleNames) {
        return kAccessCheck_kGetAuthenticatedRoleNames;
    }
    if (value == AccessCheckEnum::kGetAuthenticatedUserName) {
        return kAccessCheck_kGetAuthenticatedUserName;
    }
    if (value == AccessCheckEnum::kGetImpersonatedRoleNames) {
        return kAccessCheck_kGetImpersonatedRoleNames;
    }
    if (value == AccessCheckEnum::kGetImpersonatedUserName) {
        return kAccessCheck_kGetImpersonatedUserName;
    }
    if (value == AccessCheckEnum::kGetAuthenticatedUser) {
        return kAccessCheck_kGetAuthenticatedUser;
    }
    if (value == AccessCheckEnum::kIsAuthenticated) {
        return kAccessCheck_kIsAuthenticated;
    }
    if (value == AccessCheckEnum::kIsAuthenticatedAsUserWithRole) {
        return kAccessCheck_kIsAuthenticatedAsUserWithRole;
    }
    if (value == AccessCheckEnum::kIsAuthorizedForAnyActionOnAnyResourceInDB) {
        return kAccessCheck_kIsAuthorizedForAnyActionOnAnyResourceInDB;
    }
    if (value == AccessCheckEnum::kIsAuthorizedForAnyActionOnResource) {
        return kAccessCheck_kIsAuthorizedForAnyActionOnResource;
    }
    if (value == AccessCheckEnum::kIsAuthorizedToChangeAsUser) {
        return kAccessCheck_kIsAuthorizedToChangeAsUser;
    }
    if (value == AccessCheckEnum::kIsAuthorizedToCreateRole) {
        return kAccessCheck_kIsAuthorizedToCreateRole;
    }
    if (value == AccessCheckEnum::kIsAuthorizedToParseNamespaceElement) {
        return kAccessCheck_kIsAuthorizedToParseNamespaceElement;
    }
    if (value == AccessCheckEnum::kIsCoAuthorized) {
        return kAccessCheck_kIsCoAuthorized;
    }
    if (value == AccessCheckEnum::kIsCoauthorizedWith) {
        return kAccessCheck_kIsCoauthorizedWith;
    }
    if (value == AccessCheckEnum::kIsCoauthorizedWithClient) {
        return kAccessCheck_kIsCoauthorizedWithClient;
    }
    if (value == AccessCheckEnum::kIsImpersonating) {
        return kAccessCheck_kIsImpersonating;
    }
    if (value == AccessCheckEnum::kLookupUser) {
        return kAccessCheck_kLookupUser;
    }
    if (value == AccessCheckEnum::kShouldIgnoreAuthChecks) {
        return kAccessCheck_kShouldIgnoreAuthChecks;
    }
    if (value == AccessCheckEnum::kIsUsingLocalhostBypass) {
        return kAccessCheck_kIsUsingLocalhostBypass;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

}  // namespace mongo
