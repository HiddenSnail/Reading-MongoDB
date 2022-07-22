/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_change_stream_gen.h --output build/opt/mongo/db/pipeline/document_source_change_stream_gen.cpp src/mongo/db/pipeline/document_source_change_stream.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/pipeline/document_source_change_stream_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Possible modes for the 'fullDocumentBeforeChange' parameter of the $changeStream stage.
 */
namespace  {
constexpr StringData kFullDocumentBeforeChangeMode_kOff = "off"_sd;
constexpr StringData kFullDocumentBeforeChangeMode_kWhenAvailable = "whenAvailable"_sd;
constexpr StringData kFullDocumentBeforeChangeMode_kRequired = "required"_sd;
}  // namespace 

FullDocumentBeforeChangeModeEnum FullDocumentBeforeChangeMode_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kFullDocumentBeforeChangeMode_kOff) {
        return FullDocumentBeforeChangeModeEnum::kOff;
    }
    if (value == kFullDocumentBeforeChangeMode_kWhenAvailable) {
        return FullDocumentBeforeChangeModeEnum::kWhenAvailable;
    }
    if (value == kFullDocumentBeforeChangeMode_kRequired) {
        return FullDocumentBeforeChangeModeEnum::kRequired;
    }
    ctxt.throwBadEnumValue(value);
}

StringData FullDocumentBeforeChangeMode_serializer(FullDocumentBeforeChangeModeEnum value) {
    if (value == FullDocumentBeforeChangeModeEnum::kOff) {
        return kFullDocumentBeforeChangeMode_kOff;
    }
    if (value == FullDocumentBeforeChangeModeEnum::kWhenAvailable) {
        return kFullDocumentBeforeChangeMode_kWhenAvailable;
    }
    if (value == FullDocumentBeforeChangeModeEnum::kRequired) {
        return kFullDocumentBeforeChangeMode_kRequired;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData ResumeTokenClusterTime::kTimestampFieldName;


ResumeTokenClusterTime::ResumeTokenClusterTime() : _hasTimestamp(false) {
    // Used for initialization only
}
ResumeTokenClusterTime::ResumeTokenClusterTime(mongo::Timestamp timestamp) : _timestamp(std::move(timestamp)), _hasTimestamp(true) {
    // Used for initialization only
}


ResumeTokenClusterTime ResumeTokenClusterTime::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ResumeTokenClusterTime object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ResumeTokenClusterTime::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kTimestampBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kTimestampBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTimestampBit);

                _hasTimestamp = true;
                _timestamp = element.timestamp();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kTimestampBit]) {
            ctxt.throwMissingField(kTimestampFieldName);
        }
    }

}


void ResumeTokenClusterTime::serialize(BSONObjBuilder* builder) const {
    invariant(_hasTimestamp);

    builder->append(kTimestampFieldName, _timestamp);

}


BSONObj ResumeTokenClusterTime::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData DocumentSourceChangeStreamSpec::kAllChangesForClusterFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kAllowToRunOnConfigDBFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kFullDocumentFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kFullDocumentBeforeChangeFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kResumeAfterFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kShowMigrationEventsFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kStartAfterFieldName;
constexpr StringData DocumentSourceChangeStreamSpec::kStartAtOperationTimeFieldName;


DocumentSourceChangeStreamSpec::DocumentSourceChangeStreamSpec()  {
    // Used for initialization only
}


DocumentSourceChangeStreamSpec DocumentSourceChangeStreamSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DocumentSourceChangeStreamSpec object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DocumentSourceChangeStreamSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<8> usedFields;
    const size_t kResumeAfterBit = 0;
    const size_t kStartAfterBit = 1;
    const size_t kStartAtOperationTimeBit = 2;
    const size_t kFullDocumentBit = 3;
    const size_t kFullDocumentBeforeChangeBit = 4;
    const size_t kAllChangesForClusterBit = 5;
    const size_t kShowMigrationEventsBit = 6;
    const size_t kAllowToRunOnConfigDBBit = 7;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kResumeAfterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kResumeAfterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kResumeAfterBit);

                const BSONObj localObject = element.Obj();
                _resumeAfter = ResumeToken::parse(localObject);
            }
        }
        else if (fieldName == kStartAfterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kStartAfterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStartAfterBit);

                const BSONObj localObject = element.Obj();
                _startAfter = ResumeToken::parse(localObject);
            }
        }
        else if (fieldName == kStartAtOperationTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kStartAtOperationTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStartAtOperationTimeBit);

                _startAtOperationTime = element.timestamp();
            }
        }
        else if (fieldName == kFullDocumentFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFullDocumentBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFullDocumentBit);

                _fullDocument = element.str();
            }
        }
        else if (fieldName == kFullDocumentBeforeChangeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFullDocumentBeforeChangeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFullDocumentBeforeChangeBit);

                IDLParserErrorContext tempContext(kFullDocumentBeforeChangeFieldName, &ctxt);
                _fullDocumentBeforeChange = FullDocumentBeforeChangeMode_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kAllChangesForClusterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAllChangesForClusterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAllChangesForClusterBit);

                _allChangesForCluster = element.boolean();
            }
        }
        else if (fieldName == kShowMigrationEventsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kShowMigrationEventsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kShowMigrationEventsBit);

                _showMigrationEvents = element.boolean();
            }
        }
        else if (fieldName == kAllowToRunOnConfigDBFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAllowToRunOnConfigDBBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAllowToRunOnConfigDBBit);

                _allowToRunOnConfigDB = element.boolean();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kFullDocumentBit]) {
            _fullDocument = "default";
        }
        if (!usedFields[kFullDocumentBeforeChangeBit]) {
            _fullDocumentBeforeChange = mongo::FullDocumentBeforeChangeModeEnum::kOff;
        }
        if (!usedFields[kAllChangesForClusterBit]) {
            _allChangesForCluster = false;
        }
        if (!usedFields[kShowMigrationEventsBit]) {
            _showMigrationEvents = false;
        }
        if (!usedFields[kAllowToRunOnConfigDBBit]) {
            _allowToRunOnConfigDB = false;
        }
    }

}


void DocumentSourceChangeStreamSpec::serialize(BSONObjBuilder* builder) const {
    if (_resumeAfter.is_initialized()) {
        const BSONObj localObject = _resumeAfter.get().toBSON_do_not_use();
        builder->append(kResumeAfterFieldName, localObject);
    }

    if (_startAfter.is_initialized()) {
        const BSONObj localObject = _startAfter.get().toBSON_do_not_use();
        builder->append(kStartAfterFieldName, localObject);
    }

    if (_startAtOperationTime.is_initialized()) {
        builder->append(kStartAtOperationTimeFieldName, _startAtOperationTime.get());
    }

    builder->append(kFullDocumentFieldName, _fullDocument);

    {
        builder->append(kFullDocumentBeforeChangeFieldName, FullDocumentBeforeChangeMode_serializer(_fullDocumentBeforeChange));
    }

    builder->append(kAllChangesForClusterFieldName, _allChangesForCluster);

    builder->append(kShowMigrationEventsFieldName, _showMigrationEvents);

    builder->append(kAllowToRunOnConfigDBFieldName, _allowToRunOnConfigDB);

}


BSONObj DocumentSourceChangeStreamSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
