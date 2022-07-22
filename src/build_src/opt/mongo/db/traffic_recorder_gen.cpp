/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/traffic_recorder_gen.h --output build/opt/mongo/db/traffic_recorder_gen.cpp src/mongo/db/traffic_recorder.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/traffic_recorder_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

constexpr StringData TrafficRecorderStats::kBufferSizeFieldName;
constexpr StringData TrafficRecorderStats::kBufferedBytesFieldName;
constexpr StringData TrafficRecorderStats::kCurrentFileSizeFieldName;
constexpr StringData TrafficRecorderStats::kMaxFileSizeFieldName;
constexpr StringData TrafficRecorderStats::kRecordingFileFieldName;
constexpr StringData TrafficRecorderStats::kRunningFieldName;


TrafficRecorderStats::TrafficRecorderStats() : _running(false), _bufferSize(-1), _bufferedBytes(-1), _maxFileSize(-1), _currentFileSize(-1), _hasRunning(false), _hasBufferSize(false), _hasBufferedBytes(false), _hasRecordingFile(false), _hasMaxFileSize(false), _hasCurrentFileSize(false) {
    // Used for initialization only
}
TrafficRecorderStats::TrafficRecorderStats(bool running, std::int64_t bufferSize, std::int64_t bufferedBytes, std::string recordingFile, std::int64_t maxFileSize, std::int64_t currentFileSize) : _running(std::move(running)), _bufferSize(std::move(bufferSize)), _bufferedBytes(std::move(bufferedBytes)), _recordingFile(std::move(recordingFile)), _maxFileSize(std::move(maxFileSize)), _currentFileSize(std::move(currentFileSize)), _hasRunning(true), _hasBufferSize(true), _hasBufferedBytes(true), _hasRecordingFile(true), _hasMaxFileSize(true), _hasCurrentFileSize(true) {
    // Used for initialization only
}


TrafficRecorderStats TrafficRecorderStats::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TrafficRecorderStats object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TrafficRecorderStats::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kRunningBit = 0;
    const size_t kBufferSizeBit = 1;
    const size_t kBufferedBytesBit = 2;
    const size_t kRecordingFileBit = 3;
    const size_t kMaxFileSizeBit = 4;
    const size_t kCurrentFileSizeBit = 5;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kRunningFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kRunningBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRunningBit);

                _hasRunning = true;
                _running = element.boolean();
            }
        }
        else if (fieldName == kBufferSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kBufferSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBufferSizeBit);

                _hasBufferSize = true;
                _bufferSize = element._numberLong();
            }
        }
        else if (fieldName == kBufferedBytesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kBufferedBytesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBufferedBytesBit);

                _hasBufferedBytes = true;
                _bufferedBytes = element._numberLong();
            }
        }
        else if (fieldName == kRecordingFileFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kRecordingFileBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecordingFileBit);

                _hasRecordingFile = true;
                _recordingFile = element.str();
            }
        }
        else if (fieldName == kMaxFileSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kMaxFileSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxFileSizeBit);

                _hasMaxFileSize = true;
                _maxFileSize = element._numberLong();
            }
        }
        else if (fieldName == kCurrentFileSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCurrentFileSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCurrentFileSizeBit);

                _hasCurrentFileSize = true;
                _currentFileSize = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kRunningBit]) {
            ctxt.throwMissingField(kRunningFieldName);
        }
        if (!usedFields[kBufferSizeBit]) {
            ctxt.throwMissingField(kBufferSizeFieldName);
        }
        if (!usedFields[kBufferedBytesBit]) {
            ctxt.throwMissingField(kBufferedBytesFieldName);
        }
        if (!usedFields[kRecordingFileBit]) {
            ctxt.throwMissingField(kRecordingFileFieldName);
        }
        if (!usedFields[kMaxFileSizeBit]) {
            ctxt.throwMissingField(kMaxFileSizeFieldName);
        }
        if (!usedFields[kCurrentFileSizeBit]) {
            ctxt.throwMissingField(kCurrentFileSizeFieldName);
        }
    }

}


void TrafficRecorderStats::serialize(BSONObjBuilder* builder) const {
    invariant(_hasRunning && _hasBufferSize && _hasBufferedBytes && _hasRecordingFile && _hasMaxFileSize && _hasCurrentFileSize);

    builder->append(kRunningFieldName, _running);

    builder->append(kBufferSizeFieldName, _bufferSize);

    builder->append(kBufferedBytesFieldName, _bufferedBytes);

    builder->append(kRecordingFileFieldName, _recordingFile);

    builder->append(kMaxFileSizeFieldName, _maxFileSize);

    builder->append(kCurrentFileSizeFieldName, _currentFileSize);

}


BSONObj TrafficRecorderStats::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData StartRecordingTraffic::kBufferSizeFieldName;
constexpr StringData StartRecordingTraffic::kDbNameFieldName;
constexpr StringData StartRecordingTraffic::kFilenameFieldName;
constexpr StringData StartRecordingTraffic::kMaxFileSizeFieldName;
constexpr StringData StartRecordingTraffic::kCommandName;

const std::vector<StringData> StartRecordingTraffic::_knownBSONFields {
    StartRecordingTraffic::kBufferSizeFieldName,
    StartRecordingTraffic::kFilenameFieldName,
    StartRecordingTraffic::kMaxFileSizeFieldName,
    StartRecordingTraffic::kCommandName,
};
const std::vector<StringData> StartRecordingTraffic::_knownOP_MSGFields {
    StartRecordingTraffic::kBufferSizeFieldName,
    StartRecordingTraffic::kDbNameFieldName,
    StartRecordingTraffic::kFilenameFieldName,
    StartRecordingTraffic::kMaxFileSizeFieldName,
    StartRecordingTraffic::kCommandName,
};

StartRecordingTraffic::StartRecordingTraffic() : _hasFilename(false), _hasDbName(false) {
    // Used for initialization only
}
StartRecordingTraffic::StartRecordingTraffic(std::string filename) : _filename(std::move(filename)), _hasFilename(true), _hasDbName(true) {
    // Used for initialization only
}


StartRecordingTraffic StartRecordingTraffic::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    StartRecordingTraffic object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void StartRecordingTraffic::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kFilenameBit = 0;
    const size_t kBufferSizeBit = 1;
    const size_t kMaxFileSizeBit = 2;
    const size_t kDbNameBit = 3;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kFilenameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFilenameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFilenameBit);

                _hasFilename = true;
                _filename = element.str();
            }
        }
        else if (fieldName == kBufferSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kBufferSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBufferSizeBit);

                _bufferSize = element._numberLong();
            }
        }
        else if (fieldName == kMaxFileSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kMaxFileSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxFileSizeBit);

                _maxFileSize = element._numberLong();
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
        if (!usedFields[kFilenameBit]) {
            ctxt.throwMissingField(kFilenameFieldName);
        }
        if (!usedFields[kBufferSizeBit]) {
            _bufferSize = 134217728;
        }
        if (!usedFields[kMaxFileSizeBit]) {
            _maxFileSize = 6294967296;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

StartRecordingTraffic StartRecordingTraffic::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    StartRecordingTraffic object;
    object.parseProtected(ctxt, request);
    return object;
}
void StartRecordingTraffic::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<4> usedFields;
    const size_t kFilenameBit = 0;
    const size_t kBufferSizeBit = 1;
    const size_t kMaxFileSizeBit = 2;
    const size_t kDbNameBit = 3;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kFilenameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFilenameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFilenameBit);

                _hasFilename = true;
                _filename = element.str();
            }
        }
        else if (fieldName == kBufferSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kBufferSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBufferSizeBit);

                _bufferSize = element._numberLong();
            }
        }
        else if (fieldName == kMaxFileSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kMaxFileSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxFileSizeBit);

                _maxFileSize = element._numberLong();
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
        if (!usedFields[kFilenameBit]) {
            ctxt.throwMissingField(kFilenameFieldName);
        }
        if (!usedFields[kBufferSizeBit]) {
            _bufferSize = 134217728;
        }
        if (!usedFields[kMaxFileSizeBit]) {
            _maxFileSize = 6294967296;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void StartRecordingTraffic::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasFilename && _hasDbName);

    builder->append("startRecordingTraffic"_sd, 1);
    builder->append(kFilenameFieldName, _filename);

    builder->append(kBufferSizeFieldName, _bufferSize);

    builder->append(kMaxFileSizeFieldName, _maxFileSize);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest StartRecordingTraffic::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasFilename && _hasDbName);

        builder->append("startRecordingTraffic"_sd, 1);
        builder->append(kFilenameFieldName, _filename);

        builder->append(kBufferSizeFieldName, _bufferSize);

        builder->append(kMaxFileSizeFieldName, _maxFileSize);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj StartRecordingTraffic::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData StopRecordingTraffic::kDbNameFieldName;
constexpr StringData StopRecordingTraffic::kCommandName;

const std::vector<StringData> StopRecordingTraffic::_knownBSONFields {
    StopRecordingTraffic::kCommandName,
};
const std::vector<StringData> StopRecordingTraffic::_knownOP_MSGFields {
    StopRecordingTraffic::kDbNameFieldName,
    StopRecordingTraffic::kCommandName,
};

StopRecordingTraffic::StopRecordingTraffic() : _hasDbName(false) {
    // Used for initialization only
}


StopRecordingTraffic StopRecordingTraffic::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    StopRecordingTraffic object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void StopRecordingTraffic::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kDbNameFieldName) {
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
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

StopRecordingTraffic StopRecordingTraffic::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    StopRecordingTraffic object;
    object.parseProtected(ctxt, request);
    return object;
}
void StopRecordingTraffic::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kDbNameFieldName) {
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
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void StopRecordingTraffic::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append("stopRecordingTraffic"_sd, 1);
    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest StopRecordingTraffic::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append("stopRecordingTraffic"_sd, 1);
        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj StopRecordingTraffic::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

std::string gTrafficRecordingDirectory;
std::string gAlwaysRecordTraffic;
MONGO_SERVER_PARAMETER_REGISTER(idl_1688f48a0e9ad541fed4cfdbedbcc46c200daaec)(InitializerContext*) {
    /**
     * Path to directory where traffic recordings will be saved
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("trafficRecordingDirectory", gTrafficRecordingDirectory);
        ret->addValidator(validateTrafficRecordDestination);
        return ret;
    })();

    /**
     * Start server with traffic recording enabled, and ensure all records are flushed. Test only.
     */
    auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("AlwaysRecordTraffic", gAlwaysRecordTraffic);
        return ret;
    })();
    scp_1->setTestOnly();

    return Status::OK();
}
}  // namespace mongo
