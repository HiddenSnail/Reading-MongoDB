/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/scripting/mozjs/scripting_util_gen.h --output build/opt/mongo/scripting/mozjs/scripting_util_gen.cpp src/mongo/scripting/mozjs/scripting_util.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/scripting/mozjs/scripting_util_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

constexpr StringData EndSessions::kEndSessionsFieldName;


EndSessions::EndSessions() : _hasEndSessions(false) {
    // Used for initialization only
}
EndSessions::EndSessions(std::vector<mongo::BSONObj> endSessions) : _endSessions(std::move(endSessions)), _hasEndSessions(true) {
    // Used for initialization only
}


EndSessions EndSessions::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    EndSessions object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void EndSessions::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kEndSessionsBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kEndSessionsFieldName) {
            if (MONGO_unlikely(usedFields[kEndSessionsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kEndSessionsBit);

            _hasEndSessions = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kEndSessionsFieldName, &ctxt);
            std::vector<mongo::BSONObj> values;

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
                        values.emplace_back(arrayElement.Obj());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _endSessions = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kEndSessionsBit]) {
            ctxt.throwMissingField(kEndSessionsFieldName);
        }
    }

}


void EndSessions::serialize(BSONObjBuilder* builder) const {
    invariant(_hasEndSessions);

    {
        builder->append(kEndSessionsFieldName, _endSessions);
    }

}


BSONObj EndSessions::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

bool skipShellCursorFinalize{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_1bb39f92e275eb74aff8021ded075865feb7238a)(InitializerContext*) {
    /**
     * Skip finalizing cursors during garbage collection in shell
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("skipShellCursorFinalize", skipShellCursorFinalize);
        return ret;
    })();

    return Status::OK();
}
}  // namespace mongo
