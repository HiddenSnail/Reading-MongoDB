/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/read_write_concern_provenance_base_gen.h --output build/opt/mongo/db/read_write_concern_provenance_base_gen.cpp src/mongo/db/read_write_concern_provenance_base.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/read_write_concern_provenance_base_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Provenance sources
 */
namespace  {
constexpr StringData kReadWriteConcernProvenanceSource_clientSupplied = "clientSupplied"_sd;
constexpr StringData kReadWriteConcernProvenanceSource_implicitDefault = "implicitDefault"_sd;
constexpr StringData kReadWriteConcernProvenanceSource_customDefault = "customDefault"_sd;
constexpr StringData kReadWriteConcernProvenanceSource_getLastErrorDefaults = "getLastErrorDefaults"_sd;
}  // namespace 

ReadWriteConcernProvenanceSourceEnum ReadWriteConcernProvenanceSource_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kReadWriteConcernProvenanceSource_clientSupplied) {
        return ReadWriteConcernProvenanceSourceEnum::clientSupplied;
    }
    if (value == kReadWriteConcernProvenanceSource_implicitDefault) {
        return ReadWriteConcernProvenanceSourceEnum::implicitDefault;
    }
    if (value == kReadWriteConcernProvenanceSource_customDefault) {
        return ReadWriteConcernProvenanceSourceEnum::customDefault;
    }
    if (value == kReadWriteConcernProvenanceSource_getLastErrorDefaults) {
        return ReadWriteConcernProvenanceSourceEnum::getLastErrorDefaults;
    }
    ctxt.throwBadEnumValue(value);
}

StringData ReadWriteConcernProvenanceSource_serializer(ReadWriteConcernProvenanceSourceEnum value) {
    if (value == ReadWriteConcernProvenanceSourceEnum::clientSupplied) {
        return kReadWriteConcernProvenanceSource_clientSupplied;
    }
    if (value == ReadWriteConcernProvenanceSourceEnum::implicitDefault) {
        return kReadWriteConcernProvenanceSource_implicitDefault;
    }
    if (value == ReadWriteConcernProvenanceSourceEnum::customDefault) {
        return kReadWriteConcernProvenanceSource_customDefault;
    }
    if (value == ReadWriteConcernProvenanceSourceEnum::getLastErrorDefaults) {
        return kReadWriteConcernProvenanceSource_getLastErrorDefaults;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData ReadWriteConcernProvenanceBase::kSourceFieldName;


ReadWriteConcernProvenanceBase::ReadWriteConcernProvenanceBase()  {
    // Used for initialization only
}


ReadWriteConcernProvenanceBase ReadWriteConcernProvenanceBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ReadWriteConcernProvenanceBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ReadWriteConcernProvenanceBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kSourceFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                IDLParserErrorContext tempContext(kSourceFieldName, &ctxt);
                _source = ReadWriteConcernProvenanceSource_parse(tempContext, element.valueStringData());
            }
        }
    }


}


void ReadWriteConcernProvenanceBase::serialize(BSONObjBuilder* builder) const {
    if (_source.is_initialized()) {
        builder->append(kSourceFieldName, ReadWriteConcernProvenanceSource_serializer(_source.get()));
    }

}


BSONObj ReadWriteConcernProvenanceBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
