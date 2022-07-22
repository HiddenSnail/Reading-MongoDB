/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/traffic_recorder_gen.h --output build/opt/mongo/db/traffic_recorder_gen.cpp src/mongo/db/traffic_recorder.idl
 */

#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/data_range.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/simple_bsonobj_comparator.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/traffic_recorder_validators.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing the trafficRecording server status section
 */
class TrafficRecorderStats {
public:
    static constexpr auto kBufferSizeFieldName = "bufferSize"_sd;
    static constexpr auto kBufferedBytesFieldName = "bufferedBytes"_sd;
    static constexpr auto kCurrentFileSizeFieldName = "currentFileSize"_sd;
    static constexpr auto kMaxFileSizeFieldName = "maxFileSize"_sd;
    static constexpr auto kRecordingFileFieldName = "recordingFile"_sd;
    static constexpr auto kRunningFieldName = "running"_sd;

    TrafficRecorderStats();
    TrafficRecorderStats(bool running, std::int64_t bufferSize, std::int64_t bufferedBytes, std::string recordingFile, std::int64_t maxFileSize, std::int64_t currentFileSize);

    static TrafficRecorderStats parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    bool getRunning() const { return _running; }
    void setRunning(bool value) & {  _running = std::move(value); _hasRunning = true; }

    std::int64_t getBufferSize() const { return _bufferSize; }
    void setBufferSize(std::int64_t value) & {  _bufferSize = std::move(value); _hasBufferSize = true; }

    std::int64_t getBufferedBytes() const { return _bufferedBytes; }
    void setBufferedBytes(std::int64_t value) & {  _bufferedBytes = std::move(value); _hasBufferedBytes = true; }

    const StringData getRecordingFile() const& { return _recordingFile; }
    void getRecordingFile() && = delete;
    void setRecordingFile(StringData value) & { auto _tmpValue = value.toString();  _recordingFile = std::move(_tmpValue); _hasRecordingFile = true; }

    std::int64_t getMaxFileSize() const { return _maxFileSize; }
    void setMaxFileSize(std::int64_t value) & {  _maxFileSize = std::move(value); _hasMaxFileSize = true; }

    std::int64_t getCurrentFileSize() const { return _currentFileSize; }
    void setCurrentFileSize(std::int64_t value) & {  _currentFileSize = std::move(value); _hasCurrentFileSize = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    bool _running;
    std::int64_t _bufferSize;
    std::int64_t _bufferedBytes;
    std::string _recordingFile;
    std::int64_t _maxFileSize;
    std::int64_t _currentFileSize;
    bool _hasRunning : 1;
    bool _hasBufferSize : 1;
    bool _hasBufferedBytes : 1;
    bool _hasRecordingFile : 1;
    bool _hasMaxFileSize : 1;
    bool _hasCurrentFileSize : 1;
};

/**
 * start recording Command
 */
class StartRecordingTraffic {
public:
    static constexpr auto kBufferSizeFieldName = "bufferSize"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFilenameFieldName = "filename"_sd;
    static constexpr auto kMaxFileSizeFieldName = "maxFileSize"_sd;
    static constexpr auto kCommandName = "startRecordingTraffic"_sd;

    StartRecordingTraffic();
    StartRecordingTraffic(std::string filename);

    static StartRecordingTraffic parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static StartRecordingTraffic parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    /**
     * output file name
     */
    const StringData getFilename() const& { return _filename; }
    void getFilename() && = delete;
    void setFilename(StringData value) & { auto _tmpValue = value.toString();  _filename = std::move(_tmpValue); _hasFilename = true; }

    /**
     * size of buffer
     */
    std::int64_t getBufferSize() const { return _bufferSize; }
    void setBufferSize(std::int64_t value) & {  _bufferSize = std::move(value);  }

    /**
     * size of log file
     */
    std::int64_t getMaxFileSize() const { return _maxFileSize; }
    void setMaxFileSize(std::int64_t value) & {  _maxFileSize = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::string _filename;
    std::int64_t _bufferSize{134217728};
    std::int64_t _maxFileSize{6294967296};
    std::string _dbName;
    bool _hasFilename : 1;
    bool _hasDbName : 1;
};

/**
 * stop recording Command
 */
class StopRecordingTraffic {
public:
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "stopRecordingTraffic"_sd;

    StopRecordingTraffic();

    static StopRecordingTraffic parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static StopRecordingTraffic parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;


    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;



    std::string _dbName;
    bool _hasDbName : 1;
};

extern std::string gTrafficRecordingDirectory;
extern std::string gAlwaysRecordTraffic;
}  // namespace mongo
