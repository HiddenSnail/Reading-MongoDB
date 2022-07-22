/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/read_write_concern_defaults_gen.h --output build/opt/mongo/db/read_write_concern_defaults_gen.cpp src/mongo/db/read_write_concern_defaults.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/read_write_concern_defaults_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData RWConcernDefault::kDefaultReadConcernFieldName;
constexpr StringData RWConcernDefault::kDefaultWriteConcernFieldName;
constexpr StringData RWConcernDefault::kUpdateOpTimeFieldName;
constexpr StringData RWConcernDefault::kUpdateWallClockTimeFieldName;


RWConcernDefault::RWConcernDefault()  {
    // Used for initialization only
}


RWConcernDefault RWConcernDefault::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RWConcernDefault object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RWConcernDefault::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kDefaultReadConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                const BSONObj localObject = element.Obj();
                _defaultReadConcern = repl::ReadConcernArgs::fromBSONThrows(localObject);
            }
        }
        else if (fieldName == kDefaultWriteConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                const BSONObj localObject = element.Obj();
                _defaultWriteConcern = WriteConcernOptions::deserializerForIDL(localObject);
            }
        }
        else if (fieldName == kUpdateOpTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                _updateOpTime = element.timestamp();
            }
        }
        else if (fieldName == kUpdateWallClockTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                _updateWallClockTime = element.date();
            }
        }
    }


}


void RWConcernDefault::serialize(BSONObjBuilder* builder) const {
    if (_defaultReadConcern.is_initialized()) {
        const BSONObj localObject = _defaultReadConcern.get().toBSONInner();
        builder->append(kDefaultReadConcernFieldName, localObject);
    }

    if (_defaultWriteConcern.is_initialized()) {
        const BSONObj localObject = _defaultWriteConcern.get().toBSON();
        builder->append(kDefaultWriteConcernFieldName, localObject);
    }

    if (_updateOpTime.is_initialized()) {
        builder->append(kUpdateOpTimeFieldName, _updateOpTime.get());
    }

    if (_updateWallClockTime.is_initialized()) {
        builder->append(kUpdateWallClockTimeFieldName, _updateWallClockTime.get());
    }

}


BSONObj RWConcernDefault::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
