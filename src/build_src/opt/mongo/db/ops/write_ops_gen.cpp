/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/ops/write_ops_gen.h --output build/opt/mongo/db/ops/write_ops_gen.cpp src/mongo/db/ops/write_ops.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/ops/write_ops_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace write_ops {

constexpr StringData WriteCommandBase::kBypassDocumentValidationFieldName;
constexpr StringData WriteCommandBase::kOrderedFieldName;
constexpr StringData WriteCommandBase::kStmtIdFieldName;
constexpr StringData WriteCommandBase::kStmtIdsFieldName;


WriteCommandBase::WriteCommandBase()  {
    // Used for initialization only
}


WriteCommandBase WriteCommandBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    WriteCommandBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void WriteCommandBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                _bypassDocumentValidation = element.trueValue();
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _ordered = element.boolean();
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                _stmtId = element._numberInt();
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _stmtIds = std::move(values);
        }
    }

    if (MONGO_unlikely(usedFields.find(kBypassDocumentValidationFieldName) == usedFields.end())) {
        _bypassDocumentValidation = false;
    }
    if (MONGO_unlikely(usedFields.find(kOrderedFieldName) == usedFields.end())) {
        _ordered = true;
    }

}


void WriteCommandBase::serialize(BSONObjBuilder* builder) const {
    builder->append(kBypassDocumentValidationFieldName, _bypassDocumentValidation);

    builder->append(kOrderedFieldName, _ordered);

    if (_stmtId.is_initialized()) {
        builder->append(kStmtIdFieldName, _stmtId.get());
    }

    if (_stmtIds.is_initialized()) {
        builder->append(kStmtIdsFieldName, _stmtIds.get());
    }

}


BSONObj WriteCommandBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData UpdateOpEntry::kArrayFiltersFieldName;
constexpr StringData UpdateOpEntry::kCFieldName;
constexpr StringData UpdateOpEntry::kCollationFieldName;
constexpr StringData UpdateOpEntry::kHintFieldName;
constexpr StringData UpdateOpEntry::kMultiFieldName;
constexpr StringData UpdateOpEntry::kQFieldName;
constexpr StringData UpdateOpEntry::kUFieldName;
constexpr StringData UpdateOpEntry::kUpsertFieldName;
constexpr StringData UpdateOpEntry::kUpsertSuppliedFieldName;


UpdateOpEntry::UpdateOpEntry() : _hasQ(false), _hasU(false) {
    // Used for initialization only
}
UpdateOpEntry::UpdateOpEntry(mongo::BSONObj q, mongo::write_ops::UpdateModification u) : _q(std::move(q)), _u(std::move(u)), _hasQ(true), _hasU(true) {
    // Used for initialization only
}


UpdateOpEntry UpdateOpEntry::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    UpdateOpEntry object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void UpdateOpEntry::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<9> usedFields;
    const size_t kQBit = 0;
    const size_t kUBit = 1;
    const size_t kCBit = 2;
    const size_t kArrayFiltersBit = 3;
    const size_t kHintBit = 4;
    const size_t kMultiBit = 5;
    const size_t kUpsertBit = 6;
    const size_t kUpsertSuppliedBit = 7;
    const size_t kCollationBit = 8;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kQFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQBit);

                _hasQ = true;
                _q = element.Obj();
            }
        }
        else if (fieldName == kUFieldName) {
            if (MONGO_unlikely(usedFields[kUBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kUBit);

            _hasU = true;
            _u = write_ops::UpdateModification::parseFromBSON(element);
        }
        else if (fieldName == kCFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCBit);

                _c = element.Obj();
            }
        }
        else if (fieldName == kArrayFiltersFieldName) {
            if (MONGO_unlikely(usedFields[kArrayFiltersBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kArrayFiltersBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kArrayFiltersFieldName, &ctxt);
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
            _arrayFilters = std::move(values);
        }
        else if (fieldName == kHintFieldName) {
            if (MONGO_unlikely(usedFields[kHintBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kHintBit);

            _hint = parseHint(element);
        }
        else if (fieldName == kMultiFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kMultiBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMultiBit);

                _multi = element.boolean();
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
        else if (fieldName == kUpsertSuppliedFieldName) {
            if (MONGO_unlikely(usedFields[kUpsertSuppliedBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kUpsertSuppliedBit);

            _upsertSupplied = OptionalBool::parseFromBSON(element);
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollationBit);

                _collation = element.Obj();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kQBit]) {
            ctxt.throwMissingField(kQFieldName);
        }
        if (!usedFields[kUBit]) {
            ctxt.throwMissingField(kUFieldName);
        }
        if (!usedFields[kHintBit]) {
            _hint = mongo::BSONObj();
        }
        if (!usedFields[kMultiBit]) {
            _multi = false;
        }
        if (!usedFields[kUpsertBit]) {
            _upsert = false;
        }
        if (!usedFields[kUpsertSuppliedBit]) {
            _upsertSupplied = mongo::OptionalBool();
        }
    }

}


void UpdateOpEntry::serialize(BSONObjBuilder* builder) const {
    invariant(_hasQ && _hasU);

    builder->append(kQFieldName, _q);

    {
        _u.serializeToBSON(kUFieldName, builder);
    }

    if (_c.is_initialized()) {
        builder->append(kCFieldName, _c.get());
    }

    if (_arrayFilters.is_initialized()) {
        builder->append(kArrayFiltersFieldName, _arrayFilters.get());
    }

    {
        serializeHintToBSON(_hint, kHintFieldName, builder);
    }

    builder->append(kMultiFieldName, _multi);

    builder->append(kUpsertFieldName, _upsert);

    {
        _upsertSupplied.serializeToBSON(kUpsertSuppliedFieldName, builder);
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

}


BSONObj UpdateOpEntry::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData DeleteOpEntry::kCollationFieldName;
constexpr StringData DeleteOpEntry::kHintFieldName;
constexpr StringData DeleteOpEntry::kMultiFieldName;
constexpr StringData DeleteOpEntry::kQFieldName;


DeleteOpEntry::DeleteOpEntry() : _multi(false), _hasQ(false), _hasMulti(false) {
    // Used for initialization only
}
DeleteOpEntry::DeleteOpEntry(mongo::BSONObj q, bool multi) : _q(std::move(q)), _multi(std::move(multi)), _hasQ(true), _hasMulti(true) {
    // Used for initialization only
}


DeleteOpEntry DeleteOpEntry::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DeleteOpEntry object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DeleteOpEntry::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kQBit = 0;
    const size_t kMultiBit = 1;
    const size_t kHintBit = 2;
    const size_t kCollationBit = 3;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kQFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQBit);

                _hasQ = true;
                _q = element.Obj();
            }
        }
        else if (fieldName == kMultiFieldName) {
            if (MONGO_unlikely(usedFields[kMultiBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMultiBit);

            _hasMulti = true;
            _multi = write_ops::readMultiDeleteProperty(element);
        }
        else if (fieldName == kHintFieldName) {
            if (MONGO_unlikely(usedFields[kHintBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kHintBit);

            _hint = parseHint(element);
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollationBit);

                _collation = element.Obj();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kQBit]) {
            ctxt.throwMissingField(kQFieldName);
        }
        if (!usedFields[kMultiBit]) {
            ctxt.throwMissingField(kMultiFieldName);
        }
        if (!usedFields[kHintBit]) {
            _hint = mongo::BSONObj();
        }
    }

}


void DeleteOpEntry::serialize(BSONObjBuilder* builder) const {
    invariant(_hasQ && _hasMulti);

    builder->append(kQFieldName, _q);

    {
        writeMultiDeleteProperty(_multi, kMultiFieldName, builder);
    }

    {
        serializeHintToBSON(_hint, kHintFieldName, builder);
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

}


BSONObj DeleteOpEntry::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData Insert::kBypassDocumentValidationFieldName;
constexpr StringData Insert::kDbNameFieldName;
constexpr StringData Insert::kDocumentsFieldName;
constexpr StringData Insert::kOrderedFieldName;
constexpr StringData Insert::kStmtIdFieldName;
constexpr StringData Insert::kStmtIdsFieldName;
constexpr StringData Insert::kWriteCommandBaseFieldName;
constexpr StringData Insert::kCommandName;

const std::vector<StringData> Insert::_knownBSONFields {
    Insert::kBypassDocumentValidationFieldName,
    Insert::kDocumentsFieldName,
    Insert::kOrderedFieldName,
    Insert::kStmtIdFieldName,
    Insert::kStmtIdsFieldName,
    Insert::kWriteCommandBaseFieldName,
    Insert::kCommandName,
};
const std::vector<StringData> Insert::_knownOP_MSGFields {
    Insert::kBypassDocumentValidationFieldName,
    Insert::kDbNameFieldName,
    Insert::kDocumentsFieldName,
    Insert::kOrderedFieldName,
    Insert::kStmtIdFieldName,
    Insert::kStmtIdsFieldName,
    Insert::kWriteCommandBaseFieldName,
    Insert::kCommandName,
};

Insert::Insert(const NamespaceString nss) : _nss(std::move(nss)), _dbName(nss.db().toString()), _hasDocuments(false), _hasDbName(true) {
    // Used for initialization only
}
Insert::Insert(const NamespaceString nss, std::vector<mongo::BSONObj> documents) : _nss(std::move(nss)), _documents(std::move(documents)), _dbName(nss.db().toString()), _hasDocuments(true), _hasDbName(true) {
    // Used for initialization only
}


Insert Insert::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    Insert object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void Insert::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<7> usedFields;
    const size_t kBypassDocumentValidationBit = 0;
    const size_t kOrderedBit = 1;
    const size_t kStmtIdBit = 2;
    const size_t kStmtIdsBit = 3;
    const size_t kDocumentsBit = 4;
    const size_t kDbNameBit = 5;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _writeCommandBase.setBypassDocumentValidation(element.trueValue());
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderedBit);

                _writeCommandBase.setOrdered(element.boolean());
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStmtIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStmtIdBit);

                _writeCommandBase.setStmtId(element._numberInt());
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            if (MONGO_unlikely(usedFields[kStmtIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStmtIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _writeCommandBase.setStmtIds(std::move(values));
        }
        else if (fieldName == kDocumentsFieldName) {
            if (MONGO_unlikely(usedFields[kDocumentsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kDocumentsBit);

            _hasDocuments = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kDocumentsFieldName, &ctxt);
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
            _documents = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBypassDocumentValidationBit]) {
            _writeCommandBase.setBypassDocumentValidation(false);
        }
        if (!usedFields[kOrderedBit]) {
            _writeCommandBase.setOrdered(true);
        }
        if (!usedFields[kDocumentsBit]) {
            ctxt.throwMissingField(kDocumentsFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

Insert Insert::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    Insert object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void Insert::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<7> usedFields;
    const size_t kBypassDocumentValidationBit = 0;
    const size_t kOrderedBit = 1;
    const size_t kStmtIdBit = 2;
    const size_t kStmtIdsBit = 3;
    const size_t kDocumentsBit = 4;
    const size_t kDbNameBit = 5;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _writeCommandBase.setBypassDocumentValidation(element.trueValue());
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderedBit);

                _writeCommandBase.setOrdered(element.boolean());
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStmtIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStmtIdBit);

                _writeCommandBase.setStmtId(element._numberInt());
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            if (MONGO_unlikely(usedFields[kStmtIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStmtIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _writeCommandBase.setStmtIds(std::move(values));
        }
        else if (fieldName == kDocumentsFieldName) {
            if (MONGO_unlikely(usedFields[kDocumentsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kDocumentsBit);

            _hasDocuments = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kDocumentsFieldName, &ctxt);
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
            _documents = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    for (auto&& sequence : request.sequences) {

        if (sequence.name == kDocumentsFieldName) {
            if (MONGO_unlikely(usedFields[kDocumentsBit])) {
                ctxt.throwDuplicateField(sequence.name);
            }

            usedFields.set(kDocumentsBit);

            _hasDocuments = true;
            std::vector<mongo::BSONObj> values;

            for (const BSONObj& sequenceObject : sequence.objs) {
                values.emplace_back(sequenceObject);
            }
            _documents = std::move(values);
        }
        else {
            ctxt.throwUnknownField(sequence.name);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBypassDocumentValidationBit]) {
            _writeCommandBase.setBypassDocumentValidation(false);
        }
        if (!usedFields[kOrderedBit]) {
            _writeCommandBase.setOrdered(true);
        }
        if (!usedFields[kDocumentsBit]) {
            ctxt.throwMissingField(kDocumentsFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

void Insert::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDocuments && _hasDbName);

    invariant(!_nss.isEmpty());
    builder->append("insert"_sd, _nss.coll());

    {
        _writeCommandBase.serialize(builder);
    }

    {
        builder->append(kDocumentsFieldName, _documents);
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest Insert::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDocuments && _hasDbName);

        invariant(!_nss.isEmpty());
        builder->append("insert"_sd, _nss.coll());

        {
            _writeCommandBase.serialize(builder);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    {
        OpMsg::DocumentSequence documentSequence;
        documentSequence.name = kDocumentsFieldName.toString();
        for (const auto& item : _documents) {
            documentSequence.objs.push_back(item);
        }
        request.sequences.emplace_back(documentSequence);
    }

    return request;
}

BSONObj Insert::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData Update::kBypassDocumentValidationFieldName;
constexpr StringData Update::kDbNameFieldName;
constexpr StringData Update::kOrderedFieldName;
constexpr StringData Update::kRuntimeConstantsFieldName;
constexpr StringData Update::kStmtIdFieldName;
constexpr StringData Update::kStmtIdsFieldName;
constexpr StringData Update::kUpdatesFieldName;
constexpr StringData Update::kWriteCommandBaseFieldName;
constexpr StringData Update::kCommandName;

const std::vector<StringData> Update::_knownBSONFields {
    Update::kBypassDocumentValidationFieldName,
    Update::kOrderedFieldName,
    Update::kRuntimeConstantsFieldName,
    Update::kStmtIdFieldName,
    Update::kStmtIdsFieldName,
    Update::kUpdatesFieldName,
    Update::kWriteCommandBaseFieldName,
    Update::kCommandName,
};
const std::vector<StringData> Update::_knownOP_MSGFields {
    Update::kBypassDocumentValidationFieldName,
    Update::kDbNameFieldName,
    Update::kOrderedFieldName,
    Update::kRuntimeConstantsFieldName,
    Update::kStmtIdFieldName,
    Update::kStmtIdsFieldName,
    Update::kUpdatesFieldName,
    Update::kWriteCommandBaseFieldName,
    Update::kCommandName,
};

Update::Update(const NamespaceString nss) : _nss(std::move(nss)), _dbName(nss.db().toString()), _hasUpdates(false), _hasDbName(true) {
    // Used for initialization only
}
Update::Update(const NamespaceString nss, std::vector<mongo::write_ops::UpdateOpEntry> updates) : _nss(std::move(nss)), _updates(std::move(updates)), _dbName(nss.db().toString()), _hasUpdates(true), _hasDbName(true) {
    // Used for initialization only
}


Update Update::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    Update object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void Update::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<8> usedFields;
    const size_t kBypassDocumentValidationBit = 0;
    const size_t kOrderedBit = 1;
    const size_t kStmtIdBit = 2;
    const size_t kStmtIdsBit = 3;
    const size_t kUpdatesBit = 4;
    const size_t kRuntimeConstantsBit = 5;
    const size_t kDbNameBit = 6;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _writeCommandBase.setBypassDocumentValidation(element.trueValue());
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderedBit);

                _writeCommandBase.setOrdered(element.boolean());
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStmtIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStmtIdBit);

                _writeCommandBase.setStmtId(element._numberInt());
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            if (MONGO_unlikely(usedFields[kStmtIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStmtIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _writeCommandBase.setStmtIds(std::move(values));
        }
        else if (fieldName == kUpdatesFieldName) {
            if (MONGO_unlikely(usedFields[kUpdatesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kUpdatesBit);

            _hasUpdates = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kUpdatesFieldName, &ctxt);
            std::vector<mongo::write_ops::UpdateOpEntry> values;

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
                        IDLParserErrorContext tempContext(kUpdatesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::write_ops::UpdateOpEntry::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _updates = std::move(values);
        }
        else if (fieldName == kRuntimeConstantsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kRuntimeConstantsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRuntimeConstantsBit);

                IDLParserErrorContext tempContext(kRuntimeConstantsFieldName, &ctxt);
                const auto localObject = element.Obj();
                _runtimeConstants = mongo::RuntimeConstants::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBypassDocumentValidationBit]) {
            _writeCommandBase.setBypassDocumentValidation(false);
        }
        if (!usedFields[kOrderedBit]) {
            _writeCommandBase.setOrdered(true);
        }
        if (!usedFields[kUpdatesBit]) {
            ctxt.throwMissingField(kUpdatesFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

Update Update::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    Update object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void Update::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<8> usedFields;
    const size_t kBypassDocumentValidationBit = 0;
    const size_t kOrderedBit = 1;
    const size_t kStmtIdBit = 2;
    const size_t kStmtIdsBit = 3;
    const size_t kUpdatesBit = 4;
    const size_t kRuntimeConstantsBit = 5;
    const size_t kDbNameBit = 6;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _writeCommandBase.setBypassDocumentValidation(element.trueValue());
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderedBit);

                _writeCommandBase.setOrdered(element.boolean());
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStmtIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStmtIdBit);

                _writeCommandBase.setStmtId(element._numberInt());
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            if (MONGO_unlikely(usedFields[kStmtIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStmtIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _writeCommandBase.setStmtIds(std::move(values));
        }
        else if (fieldName == kUpdatesFieldName) {
            if (MONGO_unlikely(usedFields[kUpdatesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kUpdatesBit);

            _hasUpdates = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kUpdatesFieldName, &ctxt);
            std::vector<mongo::write_ops::UpdateOpEntry> values;

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
                        IDLParserErrorContext tempContext(kUpdatesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::write_ops::UpdateOpEntry::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _updates = std::move(values);
        }
        else if (fieldName == kRuntimeConstantsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kRuntimeConstantsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRuntimeConstantsBit);

                IDLParserErrorContext tempContext(kRuntimeConstantsFieldName, &ctxt);
                const auto localObject = element.Obj();
                _runtimeConstants = mongo::RuntimeConstants::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    for (auto&& sequence : request.sequences) {

        if (sequence.name == kUpdatesFieldName) {
            if (MONGO_unlikely(usedFields[kUpdatesBit])) {
                ctxt.throwDuplicateField(sequence.name);
            }

            usedFields.set(kUpdatesBit);

            _hasUpdates = true;
            std::vector<mongo::write_ops::UpdateOpEntry> values;

            for (const BSONObj& sequenceObject : sequence.objs) {
                IDLParserErrorContext tempContext(kUpdatesFieldName, &ctxt);
                values.emplace_back(mongo::write_ops::UpdateOpEntry::parse(tempContext, sequenceObject));
            }
            _updates = std::move(values);
        }
        else {
            ctxt.throwUnknownField(sequence.name);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBypassDocumentValidationBit]) {
            _writeCommandBase.setBypassDocumentValidation(false);
        }
        if (!usedFields[kOrderedBit]) {
            _writeCommandBase.setOrdered(true);
        }
        if (!usedFields[kUpdatesBit]) {
            ctxt.throwMissingField(kUpdatesFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

void Update::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasUpdates && _hasDbName);

    invariant(!_nss.isEmpty());
    builder->append("update"_sd, _nss.coll());

    {
        _writeCommandBase.serialize(builder);
    }

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kUpdatesFieldName));
        for (const auto& item : _updates) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

    if (_runtimeConstants.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kRuntimeConstantsFieldName));
        _runtimeConstants.get().serialize(&subObjBuilder);
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest Update::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasUpdates && _hasDbName);

        invariant(!_nss.isEmpty());
        builder->append("update"_sd, _nss.coll());

        {
            _writeCommandBase.serialize(builder);
        }

        if (_runtimeConstants.is_initialized()) {
            BSONObjBuilder subObjBuilder(builder->subobjStart(kRuntimeConstantsFieldName));
            _runtimeConstants.get().serialize(&subObjBuilder);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    {
        OpMsg::DocumentSequence documentSequence;
        documentSequence.name = kUpdatesFieldName.toString();
        for (const auto& item : _updates) {
            BSONObjBuilder builder;
            item.serialize(&builder);
            documentSequence.objs.push_back(builder.obj());
        }
        request.sequences.emplace_back(documentSequence);
    }

    return request;
}

BSONObj Update::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData Delete::kBypassDocumentValidationFieldName;
constexpr StringData Delete::kDbNameFieldName;
constexpr StringData Delete::kDeletesFieldName;
constexpr StringData Delete::kOrderedFieldName;
constexpr StringData Delete::kStmtIdFieldName;
constexpr StringData Delete::kStmtIdsFieldName;
constexpr StringData Delete::kWriteCommandBaseFieldName;
constexpr StringData Delete::kCommandName;

const std::vector<StringData> Delete::_knownBSONFields {
    Delete::kBypassDocumentValidationFieldName,
    Delete::kDeletesFieldName,
    Delete::kOrderedFieldName,
    Delete::kStmtIdFieldName,
    Delete::kStmtIdsFieldName,
    Delete::kWriteCommandBaseFieldName,
    Delete::kCommandName,
};
const std::vector<StringData> Delete::_knownOP_MSGFields {
    Delete::kBypassDocumentValidationFieldName,
    Delete::kDbNameFieldName,
    Delete::kDeletesFieldName,
    Delete::kOrderedFieldName,
    Delete::kStmtIdFieldName,
    Delete::kStmtIdsFieldName,
    Delete::kWriteCommandBaseFieldName,
    Delete::kCommandName,
};

Delete::Delete(const NamespaceString nss) : _nss(std::move(nss)), _dbName(nss.db().toString()), _hasDeletes(false), _hasDbName(true) {
    // Used for initialization only
}
Delete::Delete(const NamespaceString nss, std::vector<mongo::write_ops::DeleteOpEntry> deletes) : _nss(std::move(nss)), _deletes(std::move(deletes)), _dbName(nss.db().toString()), _hasDeletes(true), _hasDbName(true) {
    // Used for initialization only
}


Delete Delete::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    Delete object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void Delete::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<7> usedFields;
    const size_t kBypassDocumentValidationBit = 0;
    const size_t kOrderedBit = 1;
    const size_t kStmtIdBit = 2;
    const size_t kStmtIdsBit = 3;
    const size_t kDeletesBit = 4;
    const size_t kDbNameBit = 5;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _writeCommandBase.setBypassDocumentValidation(element.trueValue());
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderedBit);

                _writeCommandBase.setOrdered(element.boolean());
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStmtIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStmtIdBit);

                _writeCommandBase.setStmtId(element._numberInt());
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            if (MONGO_unlikely(usedFields[kStmtIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStmtIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _writeCommandBase.setStmtIds(std::move(values));
        }
        else if (fieldName == kDeletesFieldName) {
            if (MONGO_unlikely(usedFields[kDeletesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kDeletesBit);

            _hasDeletes = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kDeletesFieldName, &ctxt);
            std::vector<mongo::write_ops::DeleteOpEntry> values;

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
                        IDLParserErrorContext tempContext(kDeletesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::write_ops::DeleteOpEntry::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _deletes = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBypassDocumentValidationBit]) {
            _writeCommandBase.setBypassDocumentValidation(false);
        }
        if (!usedFields[kOrderedBit]) {
            _writeCommandBase.setOrdered(true);
        }
        if (!usedFields[kDeletesBit]) {
            ctxt.throwMissingField(kDeletesFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

Delete Delete::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    Delete object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void Delete::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<7> usedFields;
    const size_t kBypassDocumentValidationBit = 0;
    const size_t kOrderedBit = 1;
    const size_t kStmtIdBit = 2;
    const size_t kStmtIdsBit = 3;
    const size_t kDeletesBit = 4;
    const size_t kDbNameBit = 5;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kBypassDocumentValidationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBypassDocumentValidationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBypassDocumentValidationBit);

                _writeCommandBase.setBypassDocumentValidation(element.trueValue());
            }
        }
        else if (fieldName == kOrderedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderedBit);

                _writeCommandBase.setOrdered(element.boolean());
            }
        }
        else if (fieldName == kStmtIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kStmtIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStmtIdBit);

                _writeCommandBase.setStmtId(element._numberInt());
            }
        }
        else if (fieldName == kStmtIdsFieldName) {
            if (MONGO_unlikely(usedFields[kStmtIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kStmtIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kStmtIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _writeCommandBase.setStmtIds(std::move(values));
        }
        else if (fieldName == kDeletesFieldName) {
            if (MONGO_unlikely(usedFields[kDeletesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kDeletesBit);

            _hasDeletes = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kDeletesFieldName, &ctxt);
            std::vector<mongo::write_ops::DeleteOpEntry> values;

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
                        IDLParserErrorContext tempContext(kDeletesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::write_ops::DeleteOpEntry::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _deletes = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    for (auto&& sequence : request.sequences) {

        if (sequence.name == kDeletesFieldName) {
            if (MONGO_unlikely(usedFields[kDeletesBit])) {
                ctxt.throwDuplicateField(sequence.name);
            }

            usedFields.set(kDeletesBit);

            _hasDeletes = true;
            std::vector<mongo::write_ops::DeleteOpEntry> values;

            for (const BSONObj& sequenceObject : sequence.objs) {
                IDLParserErrorContext tempContext(kDeletesFieldName, &ctxt);
                values.emplace_back(mongo::write_ops::DeleteOpEntry::parse(tempContext, sequenceObject));
            }
            _deletes = std::move(values);
        }
        else {
            ctxt.throwUnknownField(sequence.name);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBypassDocumentValidationBit]) {
            _writeCommandBase.setBypassDocumentValidation(false);
        }
        if (!usedFields[kOrderedBit]) {
            _writeCommandBase.setOrdered(true);
        }
        if (!usedFields[kDeletesBit]) {
            ctxt.throwMissingField(kDeletesFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

void Delete::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDeletes && _hasDbName);

    invariant(!_nss.isEmpty());
    builder->append("delete"_sd, _nss.coll());

    {
        _writeCommandBase.serialize(builder);
    }

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kDeletesFieldName));
        for (const auto& item : _deletes) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest Delete::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDeletes && _hasDbName);

        invariant(!_nss.isEmpty());
        builder->append("delete"_sd, _nss.coll());

        {
            _writeCommandBase.serialize(builder);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    {
        OpMsg::DocumentSequence documentSequence;
        documentSequence.name = kDeletesFieldName.toString();
        for (const auto& item : _deletes) {
            BSONObjBuilder builder;
            item.serialize(&builder);
            documentSequence.objs.push_back(builder.obj());
        }
        request.sequences.emplace_back(documentSequence);
    }

    return request;
}

BSONObj Delete::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace write_ops
}  // namespace mongo
