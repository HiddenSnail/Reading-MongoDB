/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/create_gen.h --output build/opt/mongo/db/commands/create_gen.cpp src/mongo/db/commands/create.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/create_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData CreateCommand::kAutoIndexIdFieldName;
constexpr StringData CreateCommand::kCappedFieldName;
constexpr StringData CreateCommand::kCollationFieldName;
constexpr StringData CreateCommand::kDbNameFieldName;
constexpr StringData CreateCommand::kFlagsFieldName;
constexpr StringData CreateCommand::kIdIndexFieldName;
constexpr StringData CreateCommand::kIndexOptionDefaultsFieldName;
constexpr StringData CreateCommand::kMaxFieldName;
constexpr StringData CreateCommand::kPipelineFieldName;
constexpr StringData CreateCommand::kRecordPreImagesFieldName;
constexpr StringData CreateCommand::kSizeFieldName;
constexpr StringData CreateCommand::kStorageEngineFieldName;
constexpr StringData CreateCommand::kTempFieldName;
constexpr StringData CreateCommand::kValidationActionFieldName;
constexpr StringData CreateCommand::kValidationLevelFieldName;
constexpr StringData CreateCommand::kValidatorFieldName;
constexpr StringData CreateCommand::kViewOnFieldName;
constexpr StringData CreateCommand::kWriteConcernFieldName;
constexpr StringData CreateCommand::kCommandName;

const std::vector<StringData> CreateCommand::_knownBSONFields {
    CreateCommand::kAutoIndexIdFieldName,
    CreateCommand::kCappedFieldName,
    CreateCommand::kCollationFieldName,
    CreateCommand::kFlagsFieldName,
    CreateCommand::kIdIndexFieldName,
    CreateCommand::kIndexOptionDefaultsFieldName,
    CreateCommand::kMaxFieldName,
    CreateCommand::kPipelineFieldName,
    CreateCommand::kRecordPreImagesFieldName,
    CreateCommand::kSizeFieldName,
    CreateCommand::kStorageEngineFieldName,
    CreateCommand::kTempFieldName,
    CreateCommand::kValidationActionFieldName,
    CreateCommand::kValidationLevelFieldName,
    CreateCommand::kValidatorFieldName,
    CreateCommand::kViewOnFieldName,
    CreateCommand::kWriteConcernFieldName,
    CreateCommand::kCommandName,
};
const std::vector<StringData> CreateCommand::_knownOP_MSGFields {
    CreateCommand::kAutoIndexIdFieldName,
    CreateCommand::kCappedFieldName,
    CreateCommand::kCollationFieldName,
    CreateCommand::kDbNameFieldName,
    CreateCommand::kFlagsFieldName,
    CreateCommand::kIdIndexFieldName,
    CreateCommand::kIndexOptionDefaultsFieldName,
    CreateCommand::kMaxFieldName,
    CreateCommand::kPipelineFieldName,
    CreateCommand::kRecordPreImagesFieldName,
    CreateCommand::kSizeFieldName,
    CreateCommand::kStorageEngineFieldName,
    CreateCommand::kTempFieldName,
    CreateCommand::kValidationActionFieldName,
    CreateCommand::kValidationLevelFieldName,
    CreateCommand::kValidatorFieldName,
    CreateCommand::kViewOnFieldName,
    CreateCommand::kWriteConcernFieldName,
    CreateCommand::kCommandName,
};

CreateCommand::CreateCommand(const NamespaceString nss) : _nss(std::move(nss)), _dbName(nss.db().toString()), _hasDbName(true) {
    // Used for initialization only
}


CreateCommand CreateCommand::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    CreateCommand object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CreateCommand::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<18> usedFields;
    const size_t kCappedBit = 0;
    const size_t kAutoIndexIdBit = 1;
    const size_t kIdIndexBit = 2;
    const size_t kSizeBit = 3;
    const size_t kMaxBit = 4;
    const size_t kStorageEngineBit = 5;
    const size_t kValidatorBit = 6;
    const size_t kValidationLevelBit = 7;
    const size_t kValidationActionBit = 8;
    const size_t kIndexOptionDefaultsBit = 9;
    const size_t kViewOnBit = 10;
    const size_t kPipelineBit = 11;
    const size_t kCollationBit = 12;
    const size_t kWriteConcernBit = 13;
    const size_t kRecordPreImagesBit = 14;
    const size_t kTempBit = 15;
    const size_t kFlagsBit = 16;
    const size_t kDbNameBit = 17;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kCappedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kCappedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCappedBit);

                _capped = element.trueValue();
            }
        }
        else if (fieldName == kAutoIndexIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kAutoIndexIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAutoIndexIdBit);

                _autoIndexId = element.trueValue();
            }
        }
        else if (fieldName == kIdIndexFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kIdIndexBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdIndexBit);

                _idIndex = element.Obj();
            }
        }
        else if (fieldName == kSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSizeBit);

                _size = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxBit);

                _max = element.safeNumberLong();
            }
        }
        else if (fieldName == kStorageEngineFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kStorageEngineBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStorageEngineBit);

                _storageEngine = element.Obj();
            }
        }
        else if (fieldName == kValidatorFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kValidatorBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidatorBit);

                _validator = element.Obj();
            }
        }
        else if (fieldName == kValidationLevelFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kValidationLevelBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidationLevelBit);

                _validationLevel = element.str();
            }
        }
        else if (fieldName == kValidationActionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kValidationActionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidationActionBit);

                _validationAction = element.str();
            }
        }
        else if (fieldName == kIndexOptionDefaultsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kIndexOptionDefaultsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIndexOptionDefaultsBit);

                _indexOptionDefaults = element.Obj();
            }
        }
        else if (fieldName == kViewOnFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kViewOnBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kViewOnBit);

                _viewOn = element.str();
            }
        }
        else if (fieldName == kPipelineFieldName) {
            if (MONGO_unlikely(usedFields[kPipelineBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kPipelineBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kPipelineFieldName, &ctxt);
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
            _pipeline = std::move(values);
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
        else if (fieldName == kWriteConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kWriteConcernBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWriteConcernBit);

                _writeConcern = element.Obj();
            }
        }
        else if (fieldName == kRecordPreImagesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kRecordPreImagesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecordPreImagesBit);

                _recordPreImages = element.trueValue();
            }
        }
        else if (fieldName == kTempFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kTempBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTempBit);

                _temp = element.trueValue();
            }
        }
        else if (fieldName == kFlagsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kFlagsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFlagsBit);

                _flags = element.safeNumberLong();
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
        if (!usedFields[kCappedBit]) {
            _capped = false;
        }
        if (!usedFields[kValidationLevelBit]) {
            _validationLevel = "strict";
        }
        if (!usedFields[kValidationActionBit]) {
            _validationAction = "error";
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

CreateCommand CreateCommand::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    CreateCommand object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void CreateCommand::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<18> usedFields;
    const size_t kCappedBit = 0;
    const size_t kAutoIndexIdBit = 1;
    const size_t kIdIndexBit = 2;
    const size_t kSizeBit = 3;
    const size_t kMaxBit = 4;
    const size_t kStorageEngineBit = 5;
    const size_t kValidatorBit = 6;
    const size_t kValidationLevelBit = 7;
    const size_t kValidationActionBit = 8;
    const size_t kIndexOptionDefaultsBit = 9;
    const size_t kViewOnBit = 10;
    const size_t kPipelineBit = 11;
    const size_t kCollationBit = 12;
    const size_t kWriteConcernBit = 13;
    const size_t kRecordPreImagesBit = 14;
    const size_t kTempBit = 15;
    const size_t kFlagsBit = 16;
    const size_t kDbNameBit = 17;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kCappedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kCappedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCappedBit);

                _capped = element.trueValue();
            }
        }
        else if (fieldName == kAutoIndexIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kAutoIndexIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAutoIndexIdBit);

                _autoIndexId = element.trueValue();
            }
        }
        else if (fieldName == kIdIndexFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kIdIndexBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdIndexBit);

                _idIndex = element.Obj();
            }
        }
        else if (fieldName == kSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSizeBit);

                _size = element.safeNumberLong();
            }
        }
        else if (fieldName == kMaxFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kMaxBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxBit);

                _max = element.safeNumberLong();
            }
        }
        else if (fieldName == kStorageEngineFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kStorageEngineBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStorageEngineBit);

                _storageEngine = element.Obj();
            }
        }
        else if (fieldName == kValidatorFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kValidatorBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidatorBit);

                _validator = element.Obj();
            }
        }
        else if (fieldName == kValidationLevelFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kValidationLevelBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidationLevelBit);

                _validationLevel = element.str();
            }
        }
        else if (fieldName == kValidationActionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kValidationActionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kValidationActionBit);

                _validationAction = element.str();
            }
        }
        else if (fieldName == kIndexOptionDefaultsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kIndexOptionDefaultsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIndexOptionDefaultsBit);

                _indexOptionDefaults = element.Obj();
            }
        }
        else if (fieldName == kViewOnFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kViewOnBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kViewOnBit);

                _viewOn = element.str();
            }
        }
        else if (fieldName == kPipelineFieldName) {
            if (MONGO_unlikely(usedFields[kPipelineBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kPipelineBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kPipelineFieldName, &ctxt);
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
            _pipeline = std::move(values);
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
        else if (fieldName == kWriteConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kWriteConcernBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWriteConcernBit);

                _writeConcern = element.Obj();
            }
        }
        else if (fieldName == kRecordPreImagesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kRecordPreImagesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecordPreImagesBit);

                _recordPreImages = element.trueValue();
            }
        }
        else if (fieldName == kTempFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kTempBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTempBit);

                _temp = element.trueValue();
            }
        }
        else if (fieldName == kFlagsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kFlagsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFlagsBit);

                _flags = element.safeNumberLong();
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
        if (!usedFields[kCappedBit]) {
            _capped = false;
        }
        if (!usedFields[kValidationLevelBit]) {
            _validationLevel = "strict";
        }
        if (!usedFields[kValidationActionBit]) {
            _validationAction = "error";
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nss.isEmpty());
    _nss = ctxt.parseNSCollectionRequired(_dbName, commandElement);
}

void CreateCommand::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    invariant(!_nss.isEmpty());
    builder->append("create"_sd, _nss.coll());

    builder->append(kCappedFieldName, _capped);

    if (_autoIndexId.is_initialized()) {
        builder->append(kAutoIndexIdFieldName, _autoIndexId.get());
    }

    if (_idIndex.is_initialized()) {
        builder->append(kIdIndexFieldName, _idIndex.get());
    }

    if (_size.is_initialized()) {
        builder->append(kSizeFieldName, _size.get());
    }

    if (_max.is_initialized()) {
        builder->append(kMaxFieldName, _max.get());
    }

    if (_storageEngine.is_initialized()) {
        builder->append(kStorageEngineFieldName, _storageEngine.get());
    }

    if (_validator.is_initialized()) {
        builder->append(kValidatorFieldName, _validator.get());
    }

    builder->append(kValidationLevelFieldName, _validationLevel);

    builder->append(kValidationActionFieldName, _validationAction);

    if (_indexOptionDefaults.is_initialized()) {
        builder->append(kIndexOptionDefaultsFieldName, _indexOptionDefaults.get());
    }

    if (_viewOn.is_initialized()) {
        builder->append(kViewOnFieldName, _viewOn.get());
    }

    if (_pipeline.is_initialized()) {
        builder->append(kPipelineFieldName, _pipeline.get());
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

    if (_writeConcern.is_initialized()) {
        builder->append(kWriteConcernFieldName, _writeConcern.get());
    }

    if (_recordPreImages.is_initialized()) {
        builder->append(kRecordPreImagesFieldName, _recordPreImages.get());
    }

    if (_temp.is_initialized()) {
        builder->append(kTempFieldName, _temp.get());
    }

    if (_flags.is_initialized()) {
        builder->append(kFlagsFieldName, _flags.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest CreateCommand::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        invariant(!_nss.isEmpty());
        builder->append("create"_sd, _nss.coll());

        builder->append(kCappedFieldName, _capped);

        if (_autoIndexId.is_initialized()) {
            builder->append(kAutoIndexIdFieldName, _autoIndexId.get());
        }

        if (_idIndex.is_initialized()) {
            builder->append(kIdIndexFieldName, _idIndex.get());
        }

        if (_size.is_initialized()) {
            builder->append(kSizeFieldName, _size.get());
        }

        if (_max.is_initialized()) {
            builder->append(kMaxFieldName, _max.get());
        }

        if (_storageEngine.is_initialized()) {
            builder->append(kStorageEngineFieldName, _storageEngine.get());
        }

        if (_validator.is_initialized()) {
            builder->append(kValidatorFieldName, _validator.get());
        }

        builder->append(kValidationLevelFieldName, _validationLevel);

        builder->append(kValidationActionFieldName, _validationAction);

        if (_indexOptionDefaults.is_initialized()) {
            builder->append(kIndexOptionDefaultsFieldName, _indexOptionDefaults.get());
        }

        if (_viewOn.is_initialized()) {
            builder->append(kViewOnFieldName, _viewOn.get());
        }

        if (_pipeline.is_initialized()) {
            builder->append(kPipelineFieldName, _pipeline.get());
        }

        if (_collation.is_initialized()) {
            builder->append(kCollationFieldName, _collation.get());
        }

        if (_writeConcern.is_initialized()) {
            builder->append(kWriteConcernFieldName, _writeConcern.get());
        }

        if (_recordPreImages.is_initialized()) {
            builder->append(kRecordPreImagesFieldName, _recordPreImages.get());
        }

        if (_temp.is_initialized()) {
            builder->append(kTempFieldName, _temp.get());
        }

        if (_flags.is_initialized()) {
            builder->append(kFlagsFieldName, _flags.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj CreateCommand::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
