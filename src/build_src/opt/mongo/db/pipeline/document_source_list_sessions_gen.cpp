/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_list_sessions_gen.h --output build/opt/mongo/db/pipeline/document_source_list_sessions_gen.cpp src/mongo/db/pipeline/document_source_list_sessions.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/pipeline/document_source_list_sessions_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ListSessionsUser::kDbFieldName;
constexpr StringData ListSessionsUser::kUserFieldName;


ListSessionsUser::ListSessionsUser() : _hasUser(false), _hasDb(false) {
    // Used for initialization only
}
ListSessionsUser::ListSessionsUser(std::string user, std::string db) : _user(std::move(user)), _db(std::move(db)), _hasUser(true), _hasDb(true) {
    // Used for initialization only
}


ListSessionsUser ListSessionsUser::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ListSessionsUser object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ListSessionsUser::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kUserBit = 0;
    const size_t kDbBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kUserFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kUserBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUserBit);

                _hasUser = true;
                _user = element.str();
            }
        }
        else if (fieldName == kDbFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbBit);

                _hasDb = true;
                _db = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kUserBit]) {
            ctxt.throwMissingField(kUserFieldName);
        }
        if (!usedFields[kDbBit]) {
            ctxt.throwMissingField(kDbFieldName);
        }
    }

}


void ListSessionsUser::serialize(BSONObjBuilder* builder) const {
    invariant(_hasUser && _hasDb);

    builder->append(kUserFieldName, _user);

    builder->append(kDbFieldName, _db);

}


BSONObj ListSessionsUser::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ListSessionsSpec::kAllUsersFieldName;
constexpr StringData ListSessionsSpec::kPredicateFieldName;
constexpr StringData ListSessionsSpec::kUsersFieldName;


ListSessionsSpec::ListSessionsSpec()  {
    // Used for initialization only
}


ListSessionsSpec ListSessionsSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ListSessionsSpec object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ListSessionsSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kAllUsersBit = 0;
    const size_t kUsersBit = 1;
    const size_t kPredicateBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAllUsersFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAllUsersBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAllUsersBit);

                _allUsers = element.boolean();
            }
        }
        else if (fieldName == kUsersFieldName) {
            if (MONGO_unlikely(usedFields[kUsersBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kUsersBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kUsersFieldName, &ctxt);
            std::vector<mongo::ListSessionsUser> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        IDLParserErrorContext tempContext(kUsersFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::ListSessionsUser::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _users = std::move(values);
        }
        else if (fieldName == kPredicateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPredicateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPredicateBit);

                _predicate = element.Obj();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kAllUsersBit]) {
            _allUsers = false;
        }
    }

}


void ListSessionsSpec::serialize(BSONObjBuilder* builder) const {
    builder->append(kAllUsersFieldName, _allUsers);

    if (_users.is_initialized()) {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kUsersFieldName));
        for (const auto& item : _users.get()) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

    if (_predicate.is_initialized()) {
        builder->append(kPredicateFieldName, _predicate.get());
    }

}


BSONObj ListSessionsSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
