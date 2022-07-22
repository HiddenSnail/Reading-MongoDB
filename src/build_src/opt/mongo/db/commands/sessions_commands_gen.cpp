/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/sessions_commands_gen.h --output build/opt/mongo/db/commands/sessions_commands_gen.cpp src/mongo/db/commands/sessions_commands.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/sessions_commands_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData EndSessionsCmdFromClient::kSessionsFieldName;


EndSessionsCmdFromClient::EndSessionsCmdFromClient() : _hasSessions(false) {
    // Used for initialization only
}
EndSessionsCmdFromClient::EndSessionsCmdFromClient(std::vector<mongo::LogicalSessionFromClient> sessions) : _sessions(std::move(sessions)), _hasSessions(true) {
    // Used for initialization only
}


EndSessionsCmdFromClient EndSessionsCmdFromClient::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    EndSessionsCmdFromClient object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void EndSessionsCmdFromClient::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kSessionsFieldName) {
            _hasSessions = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kSessionsFieldName, &ctxt);
            std::vector<mongo::LogicalSessionFromClient> values;

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
                        IDLParserErrorContext tempContext(kSessionsFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::LogicalSessionFromClient::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _sessions = std::move(values);
        }
    }

    if (MONGO_unlikely(usedFields.find(kSessionsFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kSessionsFieldName);
    }

}


void EndSessionsCmdFromClient::serialize(BSONObjBuilder* builder) const {
    invariant(_hasSessions);

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kSessionsFieldName));
        for (const auto& item : _sessions) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

}


BSONObj EndSessionsCmdFromClient::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData RefreshSessionsCmdFromClient::kSessionsFieldName;


RefreshSessionsCmdFromClient::RefreshSessionsCmdFromClient() : _hasSessions(false) {
    // Used for initialization only
}
RefreshSessionsCmdFromClient::RefreshSessionsCmdFromClient(std::vector<mongo::LogicalSessionFromClient> sessions) : _sessions(std::move(sessions)), _hasSessions(true) {
    // Used for initialization only
}


RefreshSessionsCmdFromClient RefreshSessionsCmdFromClient::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RefreshSessionsCmdFromClient object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RefreshSessionsCmdFromClient::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kSessionsFieldName) {
            _hasSessions = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kSessionsFieldName, &ctxt);
            std::vector<mongo::LogicalSessionFromClient> values;

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
                        IDLParserErrorContext tempContext(kSessionsFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::LogicalSessionFromClient::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _sessions = std::move(values);
        }
    }

    if (MONGO_unlikely(usedFields.find(kSessionsFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kSessionsFieldName);
    }

}


void RefreshSessionsCmdFromClient::serialize(BSONObjBuilder* builder) const {
    invariant(_hasSessions);

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kSessionsFieldName));
        for (const auto& item : _sessions) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

}


BSONObj RefreshSessionsCmdFromClient::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
