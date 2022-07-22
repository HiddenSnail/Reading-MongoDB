/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/stats/read_concern_stats_gen.h --output build/opt/mongo/db/stats/read_concern_stats_gen.cpp src/mongo/db/stats/read_concern_stats.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/stats/read_concern_stats_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ReadConcernStats::kAvailableFieldName;
constexpr StringData ReadConcernStats::kLinearizableFieldName;
constexpr StringData ReadConcernStats::kLocalFieldName;
constexpr StringData ReadConcernStats::kMajorityFieldName;
constexpr StringData ReadConcernStats::kNoneFieldName;
constexpr StringData ReadConcernStats::kSnapshotFieldName;


ReadConcernStats::ReadConcernStats()  {
    // Used for initialization only
}


ReadConcernStats ReadConcernStats::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ReadConcernStats object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ReadConcernStats::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kAvailableBit = 0;
    const size_t kLinearizableBit = 1;
    const size_t kLocalBit = 2;
    const size_t kMajorityBit = 3;
    const size_t kSnapshotBit = 4;
    const size_t kNoneBit = 5;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAvailableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kAvailableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAvailableBit);

                _available = element._numberLong();
            }
        }
        else if (fieldName == kLinearizableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kLinearizableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLinearizableBit);

                _linearizable = element._numberLong();
            }
        }
        else if (fieldName == kLocalFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kLocalBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLocalBit);

                _local = element._numberLong();
            }
        }
        else if (fieldName == kMajorityFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kMajorityBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMajorityBit);

                _majority = element._numberLong();
            }
        }
        else if (fieldName == kSnapshotFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kSnapshotBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSnapshotBit);

                _snapshot = element._numberLong();
            }
        }
        else if (fieldName == kNoneFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNoneBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNoneBit);

                _none = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kAvailableBit]) {
            _available = 0;
        }
        if (!usedFields[kLinearizableBit]) {
            _linearizable = 0;
        }
        if (!usedFields[kLocalBit]) {
            _local = 0;
        }
        if (!usedFields[kMajorityBit]) {
            _majority = 0;
        }
        if (!usedFields[kSnapshotBit]) {
            _snapshot = 0;
        }
        if (!usedFields[kNoneBit]) {
            _none = 0;
        }
    }

}


void ReadConcernStats::serialize(BSONObjBuilder* builder) const {
    builder->append(kAvailableFieldName, _available);

    builder->append(kLinearizableFieldName, _linearizable);

    builder->append(kLocalFieldName, _local);

    builder->append(kMajorityFieldName, _majority);

    builder->append(kSnapshotFieldName, _snapshot);

    builder->append(kNoneFieldName, _none);

}


BSONObj ReadConcernStats::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
