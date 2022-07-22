/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/optime_and_wall_time_base_gen.h --output build/opt/mongo/db/repl/optime_and_wall_time_base_gen.cpp src/mongo/db/repl/optime_and_wall_time_base.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/optime_and_wall_time_base_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

constexpr StringData OpTimeAndWallTimeBase::kTermFieldName;
constexpr StringData OpTimeAndWallTimeBase::kTimestampFieldName;
constexpr StringData OpTimeAndWallTimeBase::kWallClockTimeFieldName;


OpTimeAndWallTimeBase::OpTimeAndWallTimeBase() : _hasTimestamp(false), _hasWallClockTime(false) {
    // Used for initialization only
}
OpTimeAndWallTimeBase::OpTimeAndWallTimeBase(mongo::Timestamp timestamp, mongo::Date_t wallClockTime) : _timestamp(std::move(timestamp)), _wallClockTime(std::move(wallClockTime)), _hasTimestamp(true), _hasWallClockTime(true) {
    // Used for initialization only
}


OpTimeAndWallTimeBase OpTimeAndWallTimeBase::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    OpTimeAndWallTimeBase object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void OpTimeAndWallTimeBase::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                _hasTimestamp = true;
                _timestamp = element.timestamp();
            }
        }
        else if (fieldName == kTermFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                _term = element._numberLong();
            }
        }
        else if (fieldName == kWallClockTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Date))) {
                _hasWallClockTime = true;
                _wallClockTime = element.date();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kTimestampFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTimestampFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kWallClockTimeFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kWallClockTimeFieldName);
    }

}


void OpTimeAndWallTimeBase::serialize(BSONObjBuilder* builder) const {
    invariant(_hasTimestamp && _hasWallClockTime);

    builder->append(kTimestampFieldName, _timestamp);

    if (_term.is_initialized()) {
        builder->append(kTermFieldName, _term.get());
    }

    builder->append(kWallClockTimeFieldName, _wallClockTime);

}


BSONObj OpTimeAndWallTimeBase::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
