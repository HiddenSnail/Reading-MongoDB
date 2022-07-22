/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/apply_ops_gen.h --output build/opt/mongo/db/repl/apply_ops_gen.cpp src/mongo/db/repl/apply_ops.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/apply_ops_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

constexpr StringData ApplyOpsCommandInfoBase::kAllowAtomicFieldName;
constexpr StringData ApplyOpsCommandInfoBase::kAlwaysUpsertFieldName;
constexpr StringData ApplyOpsCommandInfoBase::kCountFieldName;
constexpr StringData ApplyOpsCommandInfoBase::kOperationsFieldName;
constexpr StringData ApplyOpsCommandInfoBase::kPartialTxnFieldName;
constexpr StringData ApplyOpsCommandInfoBase::kPreConditionFieldName;
constexpr StringData ApplyOpsCommandInfoBase::kPrepareFieldName;


ApplyOpsCommandInfoBase::ApplyOpsCommandInfoBase() : _hasOperations(false) {
    // Used for initialization only
}
ApplyOpsCommandInfoBase::ApplyOpsCommandInfoBase(std::vector<mongo::BSONObj> operations) : _operations(std::move(operations)), _hasOperations(true) {
    // Used for initialization only
}


ApplyOpsCommandInfoBase ApplyOpsCommandInfoBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ApplyOpsCommandInfoBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ApplyOpsCommandInfoBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kOperationsFieldName) {
            _hasOperations = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kOperationsFieldName, &ctxt);
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
            _operations = std::move(values);
        }
        else if (fieldName == kAllowAtomicFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _allowAtomic = element.boolean();
            }
        }
        else if (fieldName == kAlwaysUpsertFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _alwaysUpsert = element.boolean();
            }
        }
        else if (fieldName == kPreConditionFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kPreConditionFieldName, &ctxt);
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
            _preCondition = std::move(values);
        }
        else if (fieldName == kPrepareFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _prepare = element.boolean();
            }
        }
        else if (fieldName == kPartialTxnFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _partialTxn = element.boolean();
            }
        }
        else if (fieldName == kCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _count = element.safeNumberLong();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kOperationsFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kOperationsFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kAllowAtomicFieldName) == usedFields.end())) {
        _allowAtomic = true;
    }
    if (MONGO_unlikely(usedFields.find(kAlwaysUpsertFieldName) == usedFields.end())) {
        _alwaysUpsert = true;
    }

}


void ApplyOpsCommandInfoBase::serialize(BSONObjBuilder* builder) const {
    invariant(_hasOperations);

    {
        builder->append(kOperationsFieldName, _operations);
    }

    builder->append(kAllowAtomicFieldName, _allowAtomic);

    builder->append(kAlwaysUpsertFieldName, _alwaysUpsert);

    if (_preCondition.is_initialized()) {
        builder->append(kPreConditionFieldName, _preCondition.get());
    }

    if (_prepare.is_initialized()) {
        builder->append(kPrepareFieldName, _prepare.get());
    }

    if (_partialTxn.is_initialized()) {
        builder->append(kPartialTxnFieldName, _partialTxn.get());
    }

    if (_count.is_initialized()) {
        builder->append(kCountFieldName, _count.get());
    }

}


BSONObj ApplyOpsCommandInfoBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
