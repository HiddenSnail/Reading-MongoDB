/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/rpc/metadata/impersonated_user_metadata_gen.h --output build/opt/mongo/rpc/metadata/impersonated_user_metadata_gen.cpp src/mongo/rpc/metadata/impersonated_user_metadata.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/rpc/metadata/impersonated_user_metadata_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace rpc {

constexpr StringData ImpersonatedUserMetadata::kRolesFieldName;
constexpr StringData ImpersonatedUserMetadata::kUsersFieldName;


ImpersonatedUserMetadata::ImpersonatedUserMetadata() : _hasUsers(false), _hasRoles(false) {
    // Used for initialization only
}
ImpersonatedUserMetadata::ImpersonatedUserMetadata(std::vector<mongo::UserName> users, std::vector<mongo::RoleName> roles) : _users(std::move(users)), _roles(std::move(roles)), _hasUsers(true), _hasRoles(true) {
    // Used for initialization only
}


ImpersonatedUserMetadata ImpersonatedUserMetadata::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ImpersonatedUserMetadata object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ImpersonatedUserMetadata::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kUsersBit = 0;
    const size_t kRolesBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kUsersFieldName) {
            if (MONGO_unlikely(usedFields[kUsersBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kUsersBit);

            _hasUsers = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kUsersFieldName, &ctxt);
            std::vector<mongo::UserName> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    values.emplace_back(UserName::parseFromBSON(arrayElement));
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _users = std::move(values);
        }
        else if (fieldName == kRolesFieldName) {
            if (MONGO_unlikely(usedFields[kRolesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kRolesBit);

            _hasRoles = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kRolesFieldName, &ctxt);
            std::vector<mongo::RoleName> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    values.emplace_back(RoleName::parseFromBSON(arrayElement));
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _roles = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kUsersBit]) {
            ctxt.throwMissingField(kUsersFieldName);
        }
        if (!usedFields[kRolesBit]) {
            ctxt.throwMissingField(kRolesFieldName);
        }
    }

}


void ImpersonatedUserMetadata::serialize(BSONObjBuilder* builder) const {
    invariant(_hasUsers && _hasRoles);

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kUsersFieldName));
        for (const auto& item : _users) {
            item.serializeToBSON(&arrayBuilder);
        }
    }

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kRolesFieldName));
        for (const auto& item : _roles) {
            item.serializeToBSON(&arrayBuilder);
        }
    }

}


BSONObj ImpersonatedUserMetadata::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace rpc
}  // namespace mongo
