/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/generic_cursor_gen.h --output build/opt/mongo/db/generic_cursor_gen.cpp src/mongo/db/generic_cursor.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/generic_cursor_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData GenericCursor::kAwaitDataFieldName;
constexpr StringData GenericCursor::kCreatedDateFieldName;
constexpr StringData GenericCursor::kCursorIdFieldName;
constexpr StringData GenericCursor::kLastAccessDateFieldName;
constexpr StringData GenericCursor::kLastKnownCommittedOpTimeFieldName;
constexpr StringData GenericCursor::kLsidFieldName;
constexpr StringData GenericCursor::kNBatchesReturnedFieldName;
constexpr StringData GenericCursor::kNDocsReturnedFieldName;
constexpr StringData GenericCursor::kNoCursorTimeoutFieldName;
constexpr StringData GenericCursor::kNsFieldName;
constexpr StringData GenericCursor::kOperationUsingCursorIdFieldName;
constexpr StringData GenericCursor::kOriginatingCommandFieldName;
constexpr StringData GenericCursor::kPlanSummaryFieldName;
constexpr StringData GenericCursor::kTailableFieldName;


GenericCursor::GenericCursor()  {
    // Used for initialization only
}


GenericCursor GenericCursor::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    GenericCursor object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void GenericCursor::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<14> usedFields;
    const size_t kCursorIdBit = 0;
    const size_t kNsBit = 1;
    const size_t kCreatedDateBit = 2;
    const size_t kLastAccessDateBit = 3;
    const size_t kNDocsReturnedBit = 4;
    const size_t kNBatchesReturnedBit = 5;
    const size_t kNoCursorTimeoutBit = 6;
    const size_t kTailableBit = 7;
    const size_t kAwaitDataBit = 8;
    const size_t kOriginatingCommandBit = 9;
    const size_t kLsidBit = 10;
    const size_t kPlanSummaryBit = 11;
    const size_t kOperationUsingCursorIdBit = 12;
    const size_t kLastKnownCommittedOpTimeBit = 13;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kCursorIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCursorIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCursorIdBit);

                _cursorId = element._numberLong();
            }
        }
        else if (fieldName == kNsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNsBit);

                _ns = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kCreatedDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kCreatedDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCreatedDateBit);

                _createdDate = element.date();
            }
        }
        else if (fieldName == kLastAccessDateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kLastAccessDateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastAccessDateBit);

                _lastAccessDate = element.date();
            }
        }
        else if (fieldName == kNDocsReturnedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNDocsReturnedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNDocsReturnedBit);

                _nDocsReturned = element._numberLong();
            }
        }
        else if (fieldName == kNBatchesReturnedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNBatchesReturnedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNBatchesReturnedBit);

                _nBatchesReturned = element._numberLong();
            }
        }
        else if (fieldName == kNoCursorTimeoutFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kNoCursorTimeoutBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNoCursorTimeoutBit);

                _noCursorTimeout = element.boolean();
            }
        }
        else if (fieldName == kTailableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kTailableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTailableBit);

                _tailable = element.boolean();
            }
        }
        else if (fieldName == kAwaitDataFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAwaitDataBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAwaitDataBit);

                _awaitData = element.boolean();
            }
        }
        else if (fieldName == kOriginatingCommandFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kOriginatingCommandBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOriginatingCommandBit);

                _originatingCommand = element.Obj();
            }
        }
        else if (fieldName == kLsidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLsidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLsidBit);

                IDLParserErrorContext tempContext(kLsidFieldName, &ctxt);
                const auto localObject = element.Obj();
                _lsid = mongo::LogicalSessionId::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kPlanSummaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kPlanSummaryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPlanSummaryBit);

                _planSummary = element.str();
            }
        }
        else if (fieldName == kOperationUsingCursorIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kOperationUsingCursorIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOperationUsingCursorIdBit);

                _operationUsingCursorId = element._numberLong();
            }
        }
        else if (fieldName == kLastKnownCommittedOpTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLastKnownCommittedOpTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastKnownCommittedOpTimeBit);

                const BSONObj localObject = element.Obj();
                _lastKnownCommittedOpTime = repl::OpTime::parse(localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void GenericCursor::serialize(BSONObjBuilder* builder) const {
    if (_cursorId.is_initialized()) {
        builder->append(kCursorIdFieldName, _cursorId.get());
    }

    if (_ns.is_initialized()) {
        builder->append(kNsFieldName, _ns.get().toString());
    }

    if (_createdDate.is_initialized()) {
        builder->append(kCreatedDateFieldName, _createdDate.get());
    }

    if (_lastAccessDate.is_initialized()) {
        builder->append(kLastAccessDateFieldName, _lastAccessDate.get());
    }

    if (_nDocsReturned.is_initialized()) {
        builder->append(kNDocsReturnedFieldName, _nDocsReturned.get());
    }

    if (_nBatchesReturned.is_initialized()) {
        builder->append(kNBatchesReturnedFieldName, _nBatchesReturned.get());
    }

    if (_noCursorTimeout.is_initialized()) {
        builder->append(kNoCursorTimeoutFieldName, _noCursorTimeout.get());
    }

    if (_tailable.is_initialized()) {
        builder->append(kTailableFieldName, _tailable.get());
    }

    if (_awaitData.is_initialized()) {
        builder->append(kAwaitDataFieldName, _awaitData.get());
    }

    if (_originatingCommand.is_initialized()) {
        builder->append(kOriginatingCommandFieldName, _originatingCommand.get());
    }

    if (_lsid.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kLsidFieldName));
        _lsid.get().serialize(&subObjBuilder);
    }

    if (_planSummary.is_initialized()) {
        builder->append(kPlanSummaryFieldName, _planSummary.get());
    }

    if (_operationUsingCursorId.is_initialized()) {
        builder->append(kOperationUsingCursorIdFieldName, _operationUsingCursorId.get());
    }

    if (_lastKnownCommittedOpTime.is_initialized()) {
        const BSONObj localObject = _lastKnownCommittedOpTime.get().toBSON();
        builder->append(kLastKnownCommittedOpTimeFieldName, localObject);
    }

}


BSONObj GenericCursor::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
