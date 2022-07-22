/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/idl/basic_types_gen.h --output build/opt/mongo/idl/basic_types_gen.cpp src/mongo/idl/basic_types.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/idl/basic_types_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

/**
 * Sort order of case differences.
 */
namespace  {
constexpr StringData kCollationCaseFirst_kUpper = "upper"_sd;
constexpr StringData kCollationCaseFirst_kLower = "lower"_sd;
constexpr StringData kCollationCaseFirst_kOff = "off"_sd;
}  // namespace 

CollationCaseFirstEnum CollationCaseFirst_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kCollationCaseFirst_kUpper) {
        return CollationCaseFirstEnum::kUpper;
    }
    if (value == kCollationCaseFirst_kLower) {
        return CollationCaseFirstEnum::kLower;
    }
    if (value == kCollationCaseFirst_kOff) {
        return CollationCaseFirstEnum::kOff;
    }
    ctxt.throwBadEnumValue(value);
}

StringData CollationCaseFirst_serializer(CollationCaseFirstEnum value) {
    if (value == CollationCaseFirstEnum::kUpper) {
        return kCollationCaseFirst_kUpper;
    }
    if (value == CollationCaseFirstEnum::kLower) {
        return kCollationCaseFirst_kLower;
    }
    if (value == CollationCaseFirstEnum::kOff) {
        return kCollationCaseFirst_kOff;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

/**
 * Controls the set of characteristics used to compare strings.
 */
CollationStrength CollationStrength_parse(const IDLParserErrorContext& ctxt, std::int32_t value) {
    
    if (!(value >= static_cast<std::underlying_type<CollationStrength>::type>(
        CollationStrength::kPrimary) &&
        value <= static_cast<std::underlying_type<CollationStrength>::type>(
            CollationStrength::kIdentical))) {
        ctxt.throwBadEnumValue(value);
    } else {
        return static_cast<CollationStrength>(value);
    }
}

std::int32_t CollationStrength_serializer(CollationStrength value) {
    return static_cast<std::int32_t>(value);
}

/**
 * Whether collation should consider whitespace and punctuation as base characters for purposes of comparison.
 */
namespace  {
constexpr StringData kCollationAlternate_kNonIgnorable = "non-ignorable"_sd;
constexpr StringData kCollationAlternate_kShifted = "shifted"_sd;
}  // namespace 

CollationAlternateEnum CollationAlternate_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kCollationAlternate_kNonIgnorable) {
        return CollationAlternateEnum::kNonIgnorable;
    }
    if (value == kCollationAlternate_kShifted) {
        return CollationAlternateEnum::kShifted;
    }
    ctxt.throwBadEnumValue(value);
}

StringData CollationAlternate_serializer(CollationAlternateEnum value) {
    if (value == CollationAlternateEnum::kNonIgnorable) {
        return kCollationAlternate_kNonIgnorable;
    }
    if (value == CollationAlternateEnum::kShifted) {
        return kCollationAlternate_kShifted;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

/**
 * Up to which characters are considered ignorable when alternate is "shifted".
 */
namespace  {
constexpr StringData kCollationMaxVariable_kPunct = "punct"_sd;
constexpr StringData kCollationMaxVariable_kSpace = "space"_sd;
}  // namespace 

CollationMaxVariableEnum CollationMaxVariable_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kCollationMaxVariable_kPunct) {
        return CollationMaxVariableEnum::kPunct;
    }
    if (value == kCollationMaxVariable_kSpace) {
        return CollationMaxVariableEnum::kSpace;
    }
    ctxt.throwBadEnumValue(value);
}

StringData CollationMaxVariable_serializer(CollationMaxVariableEnum value) {
    if (value == CollationMaxVariableEnum::kPunct) {
        return kCollationMaxVariable_kPunct;
    }
    if (value == CollationMaxVariableEnum::kSpace) {
        return kCollationMaxVariable_kSpace;
    }
    MONGO_UNREACHABLE;
    return StringData();
}



OkReply::OkReply()  {
    // Used for initialization only
}


OkReply OkReply::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<OkReply>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void OkReply::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
}


void OkReply::serialize(BSONObjBuilder* builder) const {
}


BSONObj OkReply::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ErrorReply::kCodeFieldName;
constexpr StringData ErrorReply::kCodeNameFieldName;
constexpr StringData ErrorReply::kErrmsgFieldName;
constexpr StringData ErrorReply::kErrorLabelsFieldName;
constexpr StringData ErrorReply::kOkFieldName;


ErrorReply::ErrorReply() : _ok(mongo::idl::preparsedValue<decltype(_ok)>()), _code(mongo::idl::preparsedValue<decltype(_code)>()), _codeName(mongo::idl::preparsedValue<decltype(_codeName)>()), _errmsg(mongo::idl::preparsedValue<decltype(_errmsg)>()), _hasOk(false), _hasCode(false), _hasCodeName(false), _hasErrmsg(false) {
    // Used for initialization only
}
ErrorReply::ErrorReply(double ok, std::int32_t code, std::string codeName, std::string errmsg) : _ok(std::move(ok)), _code(std::move(code)), _codeName(std::move(codeName)), _errmsg(std::move(errmsg)), _hasOk(true), _hasCode(true), _hasCodeName(true), _hasErrmsg(true) {
    // Used for initialization only
}

void ErrorReply::validateOk(IDLParserErrorContext& ctxt, const double value)
{
    if (!(value >= 0.0)) {
        throwComparisonError<double>(ctxt, "ok", ">="_sd, value, 0.0);
    }
    if (!(value <= 0.0)) {
        throwComparisonError<double>(ctxt, "ok", "<="_sd, value, 0.0);
    }
}

void ErrorReply::validateOk(const double value)
{
    if (!(value >= 0.0)) {
        throwComparisonError<double>("ok", ">="_sd, value, 0.0);
    }
    if (!(value <= 0.0)) {
        throwComparisonError<double>("ok", "<="_sd, value, 0.0);
    }
}


ErrorReply ErrorReply::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<ErrorReply>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ErrorReply::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kOkBit = 0;
    const size_t kCodeBit = 1;
    const size_t kCodeNameBit = 2;
    const size_t kErrmsgBit = 3;
    const size_t kErrorLabelsBit = 4;
    std::set<StringData> usedFieldSet;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kOkFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kOkBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOkBit);

                _hasOk = true;
                {
                    auto value = element.safeNumberDouble();
                    validateOk(value);
                    _ok = std::move(value);
                }
            }
        }
        else if (fieldName == kCodeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kCodeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCodeBit);

                _hasCode = true;
                _code = element._numberInt();
            }
        }
        else if (fieldName == kCodeNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kCodeNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCodeNameBit);

                _hasCodeName = true;
                _codeName = element.str();
            }
        }
        else if (fieldName == kErrmsgFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kErrmsgBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kErrmsgBit);

                _hasErrmsg = true;
                _errmsg = element.str();
            }
        }
        else if (fieldName == kErrorLabelsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Array))) {
                if (MONGO_unlikely(usedFields[kErrorLabelsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kErrorLabelsBit);

            }
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kErrorLabelsFieldName, &ctxt);
            std::vector<std::string> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, String)) {
                        values.emplace_back(arrayElement.str());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _errorLabels = std::move(values);
        }
        else {
            auto push_result = usedFieldSet.insert(fieldName);
            if (MONGO_unlikely(push_result.second == false)) {
                ctxt.throwDuplicateField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kOkBit]) {
            ctxt.throwMissingField(kOkFieldName);
        }
        if (!usedFields[kCodeBit]) {
            ctxt.throwMissingField(kCodeFieldName);
        }
        if (!usedFields[kCodeNameBit]) {
            ctxt.throwMissingField(kCodeNameFieldName);
        }
        if (!usedFields[kErrmsgBit]) {
            ctxt.throwMissingField(kErrmsgFieldName);
        }
    }

}


void ErrorReply::serialize(BSONObjBuilder* builder) const {
    invariant(_hasOk && _hasCode && _hasCodeName && _hasErrmsg);

    builder->append(kOkFieldName, _ok);

    builder->append(kCodeFieldName, _code);

    builder->append(kCodeNameFieldName, _codeName);

    builder->append(kErrmsgFieldName, _errmsg);

    if (_errorLabels.is_initialized()) {
        builder->append(kErrorLabelsFieldName, _errorLabels.get());
    }

}


BSONObj ErrorReply::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData SimpleCursorOptions::kBatchSizeFieldName;


SimpleCursorOptions::SimpleCursorOptions()  {
    // Used for initialization only
}

void SimpleCursorOptions::validateBatchSize(IDLParserErrorContext& ctxt, const std::int64_t value)
{
    if (!(value >= 0)) {
        throwComparisonError<std::int64_t>(ctxt, "batchSize", ">="_sd, value, 0);
    }
}

void SimpleCursorOptions::validateBatchSize(const std::int64_t value)
{
    if (!(value >= 0)) {
        throwComparisonError<std::int64_t>("batchSize", ">="_sd, value, 0);
    }
}


SimpleCursorOptions SimpleCursorOptions::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<SimpleCursorOptions>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void SimpleCursorOptions::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kBatchSizeBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kBatchSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBatchSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBatchSizeBit);

                {
                    auto value = element.safeNumberLong();
                    validateBatchSize(value);
                    _batchSize = std::move(value);
                }
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void SimpleCursorOptions::serialize(BSONObjBuilder* builder) const {
    if (_batchSize.is_initialized()) {
        builder->append(kBatchSizeFieldName, _batchSize.get());
    }

}


BSONObj SimpleCursorOptions::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData Collation::kAlternateFieldName;
constexpr StringData Collation::kBackwardsFieldName;
constexpr StringData Collation::kCaseFirstFieldName;
constexpr StringData Collation::kCaseLevelFieldName;
constexpr StringData Collation::kLocaleFieldName;
constexpr StringData Collation::kMaxVariableFieldName;
constexpr StringData Collation::kNormalizationFieldName;
constexpr StringData Collation::kNumericOrderingFieldName;
constexpr StringData Collation::kStrengthFieldName;
constexpr StringData Collation::kVersionFieldName;


Collation::Collation() : _locale(mongo::idl::preparsedValue<decltype(_locale)>()), _hasLocale(false) {
    // Used for initialization only
}
Collation::Collation(std::string locale) : _locale(std::move(locale)), _hasLocale(true) {
    // Used for initialization only
}

void Collation::validateStrength(IDLParserErrorContext& ctxt, const std::int32_t value)
{
    if (!(value >= 0)) {
        throwComparisonError<std::int32_t>(ctxt, "strength", ">="_sd, value, 0);
    }
    if (!(value <= 5)) {
        throwComparisonError<std::int32_t>(ctxt, "strength", "<="_sd, value, 5);
    }
}

void Collation::validateStrength(const std::int32_t value)
{
    if (!(value >= 0)) {
        throwComparisonError<std::int32_t>("strength", ">="_sd, value, 0);
    }
    if (!(value <= 5)) {
        throwComparisonError<std::int32_t>("strength", "<="_sd, value, 5);
    }
}


Collation Collation::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<Collation>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void Collation::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<10> usedFields;
    const size_t kLocaleBit = 0;
    const size_t kCaseLevelBit = 1;
    const size_t kCaseFirstBit = 2;
    const size_t kStrengthBit = 3;
    const size_t kNumericOrderingBit = 4;
    const size_t kAlternateBit = 5;
    const size_t kMaxVariableBit = 6;
    const size_t kNormalizationBit = 7;
    const size_t kBackwardsBit = 8;
    const size_t kVersionBit = 9;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kLocaleFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kLocaleBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLocaleBit);

                _hasLocale = true;
                _locale = element.str();
            }
        }
        else if (fieldName == kCaseLevelFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kCaseLevelBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCaseLevelBit);

                _caseLevel = element.boolean();
            }
        }
        else if (fieldName == kCaseFirstFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kCaseFirstBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCaseFirstBit);

                IDLParserErrorContext tempContext(kCaseFirstFieldName, &ctxt);
                _caseFirst = CollationCaseFirst_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kStrengthFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kStrengthBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStrengthBit);

                {
                    auto value = element.safeNumberInt();
                    validateStrength(value);
                    _strength = std::move(value);
                }
            }
        }
        else if (fieldName == kNumericOrderingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kNumericOrderingBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumericOrderingBit);

                _numericOrdering = element.boolean();
            }
        }
        else if (fieldName == kAlternateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kAlternateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAlternateBit);

                IDLParserErrorContext tempContext(kAlternateFieldName, &ctxt);
                _alternate = CollationAlternate_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kMaxVariableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kMaxVariableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxVariableBit);

                IDLParserErrorContext tempContext(kMaxVariableFieldName, &ctxt);
                _maxVariable = CollationMaxVariable_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kNormalizationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kNormalizationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNormalizationBit);

                _normalization = element.boolean();
            }
        }
        else if (fieldName == kBackwardsFieldName) {
            if (MONGO_unlikely(usedFields[kBackwardsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kBackwardsBit);

            _backwards = OptionalBool::parseFromBSON(element);
        }
        else if (fieldName == kVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVersionBit);

                ctxt.throwAPIStrictErrorIfApplicable(element);

                _version = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kLocaleBit]) {
            ctxt.throwMissingField(kLocaleFieldName);
        }
    }

}


void Collation::serialize(BSONObjBuilder* builder) const {
    invariant(_hasLocale);

    builder->append(kLocaleFieldName, _locale);

    builder->append(kCaseLevelFieldName, _caseLevel);

    {
        builder->append(kCaseFirstFieldName, ::mongo::CollationCaseFirst_serializer(_caseFirst));
    }

    builder->append(kStrengthFieldName, _strength);

    builder->append(kNumericOrderingFieldName, _numericOrdering);

    {
        builder->append(kAlternateFieldName, ::mongo::CollationAlternate_serializer(_alternate));
    }

    {
        builder->append(kMaxVariableFieldName, ::mongo::CollationMaxVariable_serializer(_maxVariable));
    }

    builder->append(kNormalizationFieldName, _normalization);

    {
        _backwards.serializeToBSON(kBackwardsFieldName, builder);
    }

    if (_version.is_initialized()) {
        builder->append(kVersionFieldName, _version.get());
    }

}


BSONObj Collation::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
