/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/oplog_entry_gen.h --output build/opt/mongo/db/repl/oplog_entry_gen.cpp src/mongo/db/repl/oplog_entry.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/oplog_entry_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

/**
 * The type of an operation in the oplog
 */
namespace  {
constexpr StringData kOpType_kCommand = "c"_sd;
constexpr StringData kOpType_kInsert = "i"_sd;
constexpr StringData kOpType_kUpdate = "u"_sd;
constexpr StringData kOpType_kDelete = "d"_sd;
constexpr StringData kOpType_kNoop = "n"_sd;
}  // namespace 

OpTypeEnum OpType_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kOpType_kCommand) {
        return OpTypeEnum::kCommand;
    }
    if (value == kOpType_kInsert) {
        return OpTypeEnum::kInsert;
    }
    if (value == kOpType_kUpdate) {
        return OpTypeEnum::kUpdate;
    }
    if (value == kOpType_kDelete) {
        return OpTypeEnum::kDelete;
    }
    if (value == kOpType_kNoop) {
        return OpTypeEnum::kNoop;
    }
    ctxt.throwBadEnumValue(value);
}

StringData OpType_serializer(OpTypeEnum value) {
    if (value == OpTypeEnum::kCommand) {
        return kOpType_kCommand;
    }
    if (value == OpTypeEnum::kInsert) {
        return kOpType_kInsert;
    }
    if (value == OpTypeEnum::kUpdate) {
        return kOpType_kUpdate;
    }
    if (value == OpTypeEnum::kDelete) {
        return kOpType_kDelete;
    }
    if (value == OpTypeEnum::kNoop) {
        return kOpType_kNoop;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData DurableReplOperation::kNssFieldName;
constexpr StringData DurableReplOperation::kObjectFieldName;
constexpr StringData DurableReplOperation::kObject2FieldName;
constexpr StringData DurableReplOperation::kOpTypeFieldName;
constexpr StringData DurableReplOperation::kPreImageOpTimeFieldName;
constexpr StringData DurableReplOperation::kUpsertFieldName;
constexpr StringData DurableReplOperation::kUuidFieldName;


DurableReplOperation::DurableReplOperation() : _hasOpType(false), _hasNss(false), _hasObject(false) {
    // Used for initialization only
}
DurableReplOperation::DurableReplOperation(mongo::repl::OpTypeEnum opType, mongo::NamespaceString nss, mongo::BSONObj object) : _opType(std::move(opType)), _nss(std::move(nss)), _object(std::move(object)), _hasOpType(true), _hasNss(true), _hasObject(true) {
    // Used for initialization only
}


DurableReplOperation DurableReplOperation::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DurableReplOperation object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DurableReplOperation::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<7> usedFields;
    const size_t kOpTypeBit = 0;
    const size_t kNssBit = 1;
    const size_t kUuidBit = 2;
    const size_t kObjectBit = 3;
    const size_t kObject2Bit = 4;
    const size_t kUpsertBit = 5;
    const size_t kPreImageOpTimeBit = 6;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kOpTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kOpTypeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOpTypeBit);

                _hasOpType = true;
                IDLParserErrorContext tempContext(kOpTypeFieldName, &ctxt);
                _opType = repl::OpType_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kNssFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNssBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNssBit);

                _hasNss = true;
                _nss = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kUuidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUuidBit);

                _uuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kObjectFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kObjectBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kObjectBit);

                _hasObject = true;
                _object = element.Obj();
            }
        }
        else if (fieldName == kObject2FieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kObject2Bit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kObject2Bit);

                _object2 = element.Obj();
            }
        }
        else if (fieldName == kUpsertFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kUpsertBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUpsertBit);

                _upsert = element.boolean();
            }
        }
        else if (fieldName == kPreImageOpTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPreImageOpTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPreImageOpTimeBit);

                const BSONObj localObject = element.Obj();
                _preImageOpTime = repl::OpTime::parse(localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kOpTypeBit]) {
            ctxt.throwMissingField(kOpTypeFieldName);
        }
        if (!usedFields[kNssBit]) {
            ctxt.throwMissingField(kNssFieldName);
        }
        if (!usedFields[kObjectBit]) {
            ctxt.throwMissingField(kObjectFieldName);
        }
    }

}


void DurableReplOperation::serialize(BSONObjBuilder* builder) const {
    invariant(_hasOpType && _hasNss && _hasObject);

    {
        builder->append(kOpTypeFieldName, OpType_serializer(_opType));
    }

    {
        builder->append(kNssFieldName, _nss.toString());
    }

    if (_uuid.is_initialized()) {
        ConstDataRange tempCDR = _uuid.get().toCDR();
        builder->append(kUuidFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    builder->append(kObjectFieldName, _object);

    if (_object2.is_initialized()) {
        builder->append(kObject2FieldName, _object2.get());
    }

    if (_upsert.is_initialized()) {
        builder->append(kUpsertFieldName, _upsert.get());
    }

    if (_preImageOpTime.is_initialized()) {
        const BSONObj localObject = _preImageOpTime.get().toBSON();
        builder->append(kPreImageOpTimeFieldName, localObject);
    }

}


BSONObj DurableReplOperation::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData OplogEntryBase::kDurableReplOperationFieldName;
constexpr StringData OplogEntryBase::kOpTimeAndWallTimeBaseFieldName;
constexpr StringData OplogEntryBase::kOperationSessionInfoFieldName;
constexpr StringData OplogEntryBase::k_idFieldName;
constexpr StringData OplogEntryBase::kFromMigrateFieldName;
constexpr StringData OplogEntryBase::kHashFieldName;
constexpr StringData OplogEntryBase::kNssFieldName;
constexpr StringData OplogEntryBase::kObjectFieldName;
constexpr StringData OplogEntryBase::kObject2FieldName;
constexpr StringData OplogEntryBase::kOpTypeFieldName;
constexpr StringData OplogEntryBase::kPostImageOpTimeFieldName;
constexpr StringData OplogEntryBase::kPreImageOpTimeFieldName;
constexpr StringData OplogEntryBase::kPrevWriteOpTimeInTransactionFieldName;
constexpr StringData OplogEntryBase::kSessionIdFieldName;
constexpr StringData OplogEntryBase::kStatementIdFieldName;
constexpr StringData OplogEntryBase::kTermFieldName;
constexpr StringData OplogEntryBase::kTimestampFieldName;
constexpr StringData OplogEntryBase::kTxnNumberFieldName;
constexpr StringData OplogEntryBase::kUpsertFieldName;
constexpr StringData OplogEntryBase::kUuidFieldName;
constexpr StringData OplogEntryBase::kVersionFieldName;
constexpr StringData OplogEntryBase::kWallClockTimeFieldName;


OplogEntryBase::OplogEntryBase()  {
    // Used for initialization only
}


OplogEntryBase OplogEntryBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    OplogEntryBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void OplogEntryBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<22> usedFields;
    const size_t kSessionIdBit = 0;
    const size_t kTxnNumberBit = 1;
    const size_t kOpTypeBit = 2;
    const size_t kNssBit = 3;
    const size_t kUuidBit = 4;
    const size_t kObjectBit = 5;
    const size_t kObject2Bit = 6;
    const size_t kUpsertBit = 7;
    const size_t kPreImageOpTimeBit = 8;
    const size_t kTimestampBit = 9;
    const size_t kTermBit = 10;
    const size_t kWallClockTimeBit = 11;
    const size_t kHashBit = 12;
    const size_t kVersionBit = 13;
    const size_t kFromMigrateBit = 14;
    const size_t k_idBit = 15;
    const size_t kStatementIdBit = 16;
    const size_t kPrevWriteOpTimeInTransactionBit = 17;
    const size_t kPostImageOpTimeBit = 18;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kSessionIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSessionIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSessionIdBit);

                IDLParserErrorContext tempContext(kSessionIdFieldName, &ctxt);
                const auto localObject = element.Obj();
                _operationSessionInfo.setSessionId(mongo::LogicalSessionId::parse(tempContext, localObject));
            }
        }
        else if (fieldName == kTxnNumberFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTxnNumberBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTxnNumberBit);

                _operationSessionInfo.setTxnNumber(element._numberLong());
            }
        }
        else if (fieldName == kOpTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kOpTypeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOpTypeBit);

                IDLParserErrorContext tempContext(kOpTypeFieldName, &ctxt);
                _durableReplOperation.setOpType(repl::OpType_parse(tempContext, element.valueStringData()));
            }
        }
        else if (fieldName == kNssFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNssBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNssBit);

                _durableReplOperation.setNss(NamespaceString(element.valueStringData()));
            }
        }
        else if (fieldName == kUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kUuidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUuidBit);

                _durableReplOperation.setUuid(UUID(uassertStatusOK(UUID::parse(element))));
            }
        }
        else if (fieldName == kObjectFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kObjectBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kObjectBit);

                _durableReplOperation.setObject(element.Obj());
            }
        }
        else if (fieldName == kObject2FieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kObject2Bit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kObject2Bit);

                _durableReplOperation.setObject2(element.Obj());
            }
        }
        else if (fieldName == kUpsertFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kUpsertBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUpsertBit);

                _durableReplOperation.setUpsert(element.boolean());
            }
        }
        else if (fieldName == kPreImageOpTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPreImageOpTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPreImageOpTimeBit);

                const BSONObj localObject = element.Obj();
                _durableReplOperation.setPreImageOpTime(repl::OpTime::parse(localObject));
            }
        }
        else if (fieldName == kTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kTimestampBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTimestampBit);

                _opTimeAndWallTimeBase.setTimestamp(element.timestamp());
            }
        }
        else if (fieldName == kTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTermBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTermBit);

                _opTimeAndWallTimeBase.setTerm(element._numberLong());
            }
        }
        else if (fieldName == kWallClockTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kWallClockTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWallClockTimeBit);

                _opTimeAndWallTimeBase.setWallClockTime(element.date());
            }
        }
        else if (fieldName == kHashFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kHashBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kHashBit);

                _hash = element._numberLong();
            }
        }
        else if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                _version = element.safeNumberLong();
            }
        }
        else if (fieldName == kFromMigrateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kFromMigrateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFromMigrateBit);

                _fromMigrate = element.boolean();
            }
        }
        else if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                if (MONGO_unlikely(usedFields[k_idBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(k_idBit);

                __id = element.OID();
            }
        }
        else if (fieldName == kStatementIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStatementIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStatementIdBit);

                _statementId = element._numberInt();
            }
        }
        else if (fieldName == kPrevWriteOpTimeInTransactionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPrevWriteOpTimeInTransactionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPrevWriteOpTimeInTransactionBit);

                const BSONObj localObject = element.Obj();
                _prevWriteOpTimeInTransaction = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kPostImageOpTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kPostImageOpTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPostImageOpTimeBit);

                const BSONObj localObject = element.Obj();
                _postImageOpTime = repl::OpTime::parse(localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kOpTypeBit]) {
            ctxt.throwMissingField(kOpTypeFieldName);
        }
        if (!usedFields[kNssBit]) {
            ctxt.throwMissingField(kNssFieldName);
        }
        if (!usedFields[kObjectBit]) {
            ctxt.throwMissingField(kObjectFieldName);
        }
        if (!usedFields[kTimestampBit]) {
            ctxt.throwMissingField(kTimestampFieldName);
        }
        if (!usedFields[kWallClockTimeBit]) {
            ctxt.throwMissingField(kWallClockTimeFieldName);
        }
        if (!usedFields[kVersionBit]) {
            _version = 2;
        }
    }

}


void OplogEntryBase::serialize(BSONObjBuilder* builder) const {
    {
        _operationSessionInfo.serialize(builder);
    }

    {
        _durableReplOperation.serialize(builder);
    }

    {
        _opTimeAndWallTimeBase.serialize(builder);
    }

    if (_hash.is_initialized()) {
        builder->append(kHashFieldName, _hash.get());
    }

    builder->append(kVersionFieldName, _version);

    if (_fromMigrate.is_initialized()) {
        builder->append(kFromMigrateFieldName, _fromMigrate.get());
    }

    if (__id.is_initialized()) {
        builder->append(k_idFieldName, __id.get());
    }

    if (_statementId.is_initialized()) {
        builder->append(kStatementIdFieldName, _statementId.get());
    }

    if (_prevWriteOpTimeInTransaction.is_initialized()) {
        const BSONObj localObject = _prevWriteOpTimeInTransaction.get().toBSON();
        builder->append(kPrevWriteOpTimeInTransactionFieldName, localObject);
    }

    if (_postImageOpTime.is_initialized()) {
        const BSONObj localObject = _postImageOpTime.get().toBSON();
        builder->append(kPostImageOpTimeFieldName, localObject);
    }

}


BSONObj OplogEntryBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
