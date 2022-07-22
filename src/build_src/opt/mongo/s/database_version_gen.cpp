/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/database_version_gen.h --output build/opt/mongo/s/database_version_gen.cpp src/mongo/s/database_version.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/database_version_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData DatabaseVersion::kLastModFieldName;
constexpr StringData DatabaseVersion::kUuidFieldName;


DatabaseVersion::DatabaseVersion() : _lastMod(-1), _hasUuid(false), _hasLastMod(false) {
    // Used for initialization only
}
DatabaseVersion::DatabaseVersion(mongo::UUID uuid, std::int32_t lastMod) : _uuid(std::move(uuid)), _lastMod(std::move(lastMod)), _hasUuid(true), _hasLastMod(true) {
    // Used for initialization only
}


DatabaseVersion DatabaseVersion::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DatabaseVersion object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DatabaseVersion::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kUuidBit = 0;
    const size_t kLastModBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kUuidBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUuidBit);

                _hasUuid = true;
                _uuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kLastModFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kLastModBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLastModBit);

                _hasLastMod = true;
                _lastMod = element._numberInt();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kUuidBit]) {
            ctxt.throwMissingField(kUuidFieldName);
        }
        if (!usedFields[kLastModBit]) {
            ctxt.throwMissingField(kLastModFieldName);
        }
    }

}


void DatabaseVersion::serialize(BSONObjBuilder* builder) const {
    invariant(_hasUuid && _hasLastMod);

    {
        ConstDataRange tempCDR = _uuid.toCDR();
        builder->append(kUuidFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    builder->append(kLastModFieldName, _lastMod);

}


BSONObj DatabaseVersion::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
