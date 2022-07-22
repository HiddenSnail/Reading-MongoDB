/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/replication_consistency_markers_gen.h --output build/opt/mongo/db/repl/replication_consistency_markers_gen.cpp src/mongo/db/repl/replication_consistency_markers.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/replication_consistency_markers_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

constexpr StringData MinValidDocument::k_idFieldName;
constexpr StringData MinValidDocument::kAppliedThroughFieldName;
constexpr StringData MinValidDocument::kInitialSyncFlagFieldName;
constexpr StringData MinValidDocument::kMinValidTermFieldName;
constexpr StringData MinValidDocument::kMinValidTimestampFieldName;


MinValidDocument::MinValidDocument() : _minValidTerm(-1), _hasMinValidTimestamp(false), _hasMinValidTerm(false) {
    // Used for initialization only
}
MinValidDocument::MinValidDocument(mongo::Timestamp minValidTimestamp, std::int64_t minValidTerm) : _minValidTimestamp(std::move(minValidTimestamp)), _minValidTerm(std::move(minValidTerm)), _hasMinValidTimestamp(true), _hasMinValidTerm(true) {
    // Used for initialization only
}


MinValidDocument MinValidDocument::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    MinValidDocument object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void MinValidDocument::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kMinValidTimestampBit = 0;
    const size_t kMinValidTermBit = 1;
    const size_t kAppliedThroughBit = 2;
    const size_t kInitialSyncFlagBit = 3;
    const size_t k_idBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kMinValidTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kMinValidTimestampBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMinValidTimestampBit);

                _hasMinValidTimestamp = true;
                _minValidTimestamp = element.timestamp();
            }
        }
        else if (fieldName == kMinValidTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMinValidTermBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMinValidTermBit);

                _hasMinValidTerm = true;
                _minValidTerm = element.safeNumberLong();
            }
        }
        else if (fieldName == kAppliedThroughFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kAppliedThroughBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAppliedThroughBit);

                const BSONObj localObject = element.Obj();
                _appliedThrough = repl::OpTime::parse(localObject);
            }
        }
        else if (fieldName == kInitialSyncFlagFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kInitialSyncFlagBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kInitialSyncFlagBit);

                _initialSyncFlag = element.boolean();
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
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kMinValidTimestampBit]) {
            ctxt.throwMissingField(kMinValidTimestampFieldName);
        }
        if (!usedFields[kMinValidTermBit]) {
            ctxt.throwMissingField(kMinValidTermFieldName);
        }
    }

}


void MinValidDocument::serialize(BSONObjBuilder* builder) const {
    invariant(_hasMinValidTimestamp && _hasMinValidTerm);

    builder->append(kMinValidTimestampFieldName, _minValidTimestamp);

    builder->append(kMinValidTermFieldName, _minValidTerm);

    if (_appliedThrough.is_initialized()) {
        const BSONObj localObject = _appliedThrough.get().toBSON();
        builder->append(kAppliedThroughFieldName, localObject);
    }

    if (_initialSyncFlag.is_initialized()) {
        builder->append(kInitialSyncFlagFieldName, _initialSyncFlag.get());
    }

    if (__id.is_initialized()) {
        builder->append(k_idFieldName, __id.get());
    }

}


BSONObj MinValidDocument::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData OplogTruncateAfterPointDocument::k_idFieldName;
constexpr StringData OplogTruncateAfterPointDocument::kOplogTruncateAfterPointFieldName;


OplogTruncateAfterPointDocument::OplogTruncateAfterPointDocument() : _hasOplogTruncateAfterPoint(false), _has_id(false) {
    // Used for initialization only
}
OplogTruncateAfterPointDocument::OplogTruncateAfterPointDocument(mongo::Timestamp oplogTruncateAfterPoint, std::string _id) : _oplogTruncateAfterPoint(std::move(oplogTruncateAfterPoint)), __id(std::move(_id)), _hasOplogTruncateAfterPoint(true), _has_id(true) {
    // Used for initialization only
}


OplogTruncateAfterPointDocument OplogTruncateAfterPointDocument::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    OplogTruncateAfterPointDocument object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void OplogTruncateAfterPointDocument::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kOplogTruncateAfterPointBit = 0;
    const size_t k_idBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kOplogTruncateAfterPointFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kOplogTruncateAfterPointBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOplogTruncateAfterPointBit);

                _hasOplogTruncateAfterPoint = true;
                _oplogTruncateAfterPoint = element.timestamp();
            }
        }
        else if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[k_idBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(k_idBit);

                _has_id = true;
                __id = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kOplogTruncateAfterPointBit]) {
            ctxt.throwMissingField(kOplogTruncateAfterPointFieldName);
        }
        if (!usedFields[k_idBit]) {
            ctxt.throwMissingField(k_idFieldName);
        }
    }

}


void OplogTruncateAfterPointDocument::serialize(BSONObjBuilder* builder) const {
    invariant(_hasOplogTruncateAfterPoint && _has_id);

    builder->append(kOplogTruncateAfterPointFieldName, _oplogTruncateAfterPoint);

    builder->append(k_idFieldName, __id);

}


BSONObj OplogTruncateAfterPointDocument::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData InitialSyncIdDocument::k_idFieldName;
constexpr StringData InitialSyncIdDocument::kWallTimeFieldName;


InitialSyncIdDocument::InitialSyncIdDocument() : _has_id(false) {
    // Used for initialization only
}
InitialSyncIdDocument::InitialSyncIdDocument(mongo::UUID _id) : __id(std::move(_id)), _has_id(true) {
    // Used for initialization only
}


InitialSyncIdDocument InitialSyncIdDocument::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    InitialSyncIdDocument object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void InitialSyncIdDocument::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t k_idBit = 0;
    const size_t kWallTimeBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == k_idFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[k_idBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(k_idBit);

                _has_id = true;
                __id = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kWallTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                if (MONGO_unlikely(usedFields[kWallTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWallTimeBit);

                _wallTime = element.date();
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
    }

}


void InitialSyncIdDocument::serialize(BSONObjBuilder* builder) const {
    invariant(_has_id);

    {
        ConstDataRange tempCDR = __id.toCDR();
        builder->append(k_idFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    if (_wallTime.is_initialized()) {
        builder->append(kWallTimeFieldName, _wallTime.get());
    }

}


BSONObj InitialSyncIdDocument::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
