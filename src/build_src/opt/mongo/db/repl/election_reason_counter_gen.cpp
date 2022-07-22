/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/election_reason_counter_gen.h --output build/opt/mongo/db/repl/election_reason_counter_gen.cpp src/mongo/db/repl/election_reason_counter.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/election_reason_counter_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

constexpr StringData ElectionReasonCounterBase::kCalledFieldName;
constexpr StringData ElectionReasonCounterBase::kSuccessfulFieldName;


ElectionReasonCounterBase::ElectionReasonCounterBase()  {
    // Used for initialization only
}


ElectionReasonCounterBase ElectionReasonCounterBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ElectionReasonCounterBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ElectionReasonCounterBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kCalledBit = 0;
    const size_t kSuccessfulBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kCalledFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCalledBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCalledBit);

                _called = element._numberLong();
            }
        }
        else if (fieldName == kSuccessfulFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kSuccessfulBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSuccessfulBit);

                _successful = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kCalledBit]) {
            _called = 0;
        }
        if (!usedFields[kSuccessfulBit]) {
            _successful = 0;
        }
    }

}


void ElectionReasonCounterBase::serialize(BSONObjBuilder* builder) const {
    builder->append(kCalledFieldName, _called);

    builder->append(kSuccessfulFieldName, _successful);

}


BSONObj ElectionReasonCounterBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
