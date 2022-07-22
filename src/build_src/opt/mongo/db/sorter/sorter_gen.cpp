/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/sorter/sorter_gen.h --output build/opt/mongo/db/sorter/sorter_gen.cpp src/mongo/db/sorter/sorter.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/sorter/sorter_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

constexpr StringData SorterRange::kChecksumFieldName;
constexpr StringData SorterRange::kEndOffsetFieldName;
constexpr StringData SorterRange::kStartOffsetFieldName;


SorterRange::SorterRange() : _startOffset(mongo::idl::preparsedValue<decltype(_startOffset)>()), _endOffset(mongo::idl::preparsedValue<decltype(_endOffset)>()), _checksum(mongo::idl::preparsedValue<decltype(_checksum)>()), _hasStartOffset(false), _hasEndOffset(false), _hasChecksum(false) {
    // Used for initialization only
}
SorterRange::SorterRange(std::int64_t startOffset, std::int64_t endOffset, std::int64_t checksum) : _startOffset(std::move(startOffset)), _endOffset(std::move(endOffset)), _checksum(std::move(checksum)), _hasStartOffset(true), _hasEndOffset(true), _hasChecksum(true) {
    // Used for initialization only
}

void SorterRange::validateChecksum(IDLParserErrorContext& ctxt, const std::int64_t value)
{
    if (!(value >= 0)) {
        throwComparisonError<std::int64_t>(ctxt, "checksum", ">="_sd, value, 0);
    }
}

void SorterRange::validateChecksum(const std::int64_t value)
{
    if (!(value >= 0)) {
        throwComparisonError<std::int64_t>("checksum", ">="_sd, value, 0);
    }
}


SorterRange SorterRange::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<SorterRange>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void SorterRange::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kStartOffsetBit = 0;
    const size_t kEndOffsetBit = 1;
    const size_t kChecksumBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kStartOffsetFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kStartOffsetBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStartOffsetBit);

                _hasStartOffset = true;
                _startOffset = element._numberLong();
            }
        }
        else if (fieldName == kEndOffsetFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kEndOffsetBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kEndOffsetBit);

                _hasEndOffset = true;
                _endOffset = element._numberLong();
            }
        }
        else if (fieldName == kChecksumFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kChecksumBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kChecksumBit);

                _hasChecksum = true;
                {
                    auto value = element._numberLong();
                    validateChecksum(value);
                    _checksum = std::move(value);
                }
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kStartOffsetBit]) {
            ctxt.throwMissingField(kStartOffsetFieldName);
        }
        if (!usedFields[kEndOffsetBit]) {
            ctxt.throwMissingField(kEndOffsetFieldName);
        }
        if (!usedFields[kChecksumBit]) {
            ctxt.throwMissingField(kChecksumFieldName);
        }
    }

}


void SorterRange::serialize(BSONObjBuilder* builder) const {
    invariant(_hasStartOffset && _hasEndOffset && _hasChecksum);

    builder->append(kStartOffsetFieldName, _startOffset);

    builder->append(kEndOffsetFieldName, _endOffset);

    builder->append(kChecksumFieldName, _checksum);

}


BSONObj SorterRange::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
