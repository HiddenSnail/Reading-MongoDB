/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/wiredtiger/wiredtiger_parameters_gen.h --output build/opt/mongo/db/storage/wiredtiger/wiredtiger_parameters_gen.cpp src/mongo/db/storage/wiredtiger/wiredtiger_parameters.idl
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
#include "mongo/db/storage/wiredtiger/wiredtiger_kv_engine.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/concurrency/ticketholder.h"
#include "mongo/util/debug_util.h"

namespace mongo {

class OpenWriteTransactionParam : public ServerParameter {
public:
    OpenWriteTransactionParam(StringData name, ServerParameterType spt);

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;

    TicketHolder* _data;
};

class OpenReadTransactionParam : public ServerParameter {
public:
    OpenReadTransactionParam(StringData name, ServerParameterType spt);

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;

    TicketHolder* _data;
};

class WiredTigerEngineRuntimeConfigParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;

    std::pair<std::string, WiredTigerKVEngine*> _data;
};

constexpr auto kWiredTigerSessionCloseIdleTimeSecsDefault = kDebugBuild ? 5 : 300;

extern AtomicWord<std::int32_t> gWiredTigerSessionCloseIdleTimeSecs;
constexpr auto kWiredTigerCursorCacheSizeDefault = -100;

extern AtomicWord<std::int32_t> gWiredTigerCursorCacheSize;
constexpr auto kWiredTigerMaxCacheOverflowSizeGBDefault = 0;

extern AtomicWord<double> gWiredTigerMaxCacheOverflowSizeGBDeprecated;
constexpr auto kWiredTigerEvictionDebugModeDefault = false;

extern bool gWiredTigerEvictionDebugMode;
constexpr auto kWiredTigerFileHandleCloseIdleTimeDefault = 100000;

extern std::int32_t gWiredTigerFileHandleCloseIdleTime;
constexpr auto kWiredTigerFileHandleCloseMinimumDefault = 250;

extern std::int32_t gWiredTigerFileHandleCloseMinimum;
constexpr auto kWiredTigerFileHandleCloseScanIntervalDefault = 10;

extern std::int32_t gWiredTigerFileHandleCloseScanInterval;
}  // namespace mongo
