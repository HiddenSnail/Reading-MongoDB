/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/rollback_gen.h --output build/opt/mongo/db/repl/rollback_gen.cpp src/mongo/db/repl/rollback.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/rollback_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

constexpr StringData RollbackID::k_idFieldName;
constexpr StringData RollbackID::kRollbackIdFieldName;


RollbackID::RollbackID() : _rollbackId(-1), _has_id(false), _hasRollbackId(false) {
    // Used for initialization only
}
RollbackID::RollbackID(std::string _id, std::int32_t rollbackId) : __id(std::move(_id)), _rollbackId(std::move(rollbackId)), _has_id(true), _hasRollbackId(true) {
    // Used for initialization only
}


RollbackID RollbackID::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RollbackID object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RollbackID::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t k_idBit = 0;
    const size_t kRollbackIdBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[k_idBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(k_idBit);

                _has_id = true;
                __id = element.str();
            }
        }
        else if (fieldName == kRollbackIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kRollbackIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRollbackIdBit);

                _hasRollbackId = true;
                _rollbackId = element._numberInt();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[k_idBit]) {
            ctxt.throwMissingField(k_idFieldName);
        }
        if (!usedFields[kRollbackIdBit]) {
            ctxt.throwMissingField(kRollbackIdFieldName);
        }
    }

}


void RollbackID::serialize(BSONObjBuilder* builder) const {
    invariant(_has_id && _hasRollbackId);

    builder->append(k_idFieldName, __id);

    builder->append(kRollbackIdFieldName, _rollbackId);

}


BSONObj RollbackID::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData RollbackProgress::k_idFieldName;
constexpr StringData RollbackProgress::kApplyUntilFieldName;


RollbackProgress::RollbackProgress() : _has_id(false), _hasApplyUntil(false) {
    // Used for initialization only
}
RollbackProgress::RollbackProgress(std::string _id, mongo::repl::OpTime applyUntil) : __id(std::move(_id)), _applyUntil(std::move(applyUntil)), _has_id(true), _hasApplyUntil(true) {
    // Used for initialization only
}


RollbackProgress RollbackProgress::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RollbackProgress object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RollbackProgress::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t k_idBit = 0;
    const size_t kApplyUntilBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[k_idBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(k_idBit);

                _has_id = true;
                __id = element.str();
            }
        }
        else if (fieldName == kApplyUntilFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kApplyUntilBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kApplyUntilBit);

                _hasApplyUntil = true;
                const BSONObj localObject = element.Obj();
                _applyUntil = repl::OpTime::parse(localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[k_idBit]) {
            ctxt.throwMissingField(k_idFieldName);
        }
        if (!usedFields[kApplyUntilBit]) {
            ctxt.throwMissingField(kApplyUntilFieldName);
        }
    }

}


void RollbackProgress::serialize(BSONObjBuilder* builder) const {
    invariant(_has_id && _hasApplyUntil);

    builder->append(k_idFieldName, __id);

    {
        const BSONObj localObject = _applyUntil.toBSON();
        builder->append(kApplyUntilFieldName, localObject);
    }

}


BSONObj RollbackProgress::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
