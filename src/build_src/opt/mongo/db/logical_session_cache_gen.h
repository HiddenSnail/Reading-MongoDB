/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/logical_session_cache_gen.h --output build/opt/mongo/db/logical_session_cache_gen.cpp src/mongo/db/logical_session_cache.idl
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
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"

namespace mongo {

constexpr auto kLogicalSessionRefreshMillisDefault = 300000;

extern int logicalSessionRefreshMillis;
constexpr auto kMaxSessionsDefault = 1000000;

extern int maxSessions;
constexpr auto kDisableLogicalSessionCacheRefreshDefault = false;

extern bool disableLogicalSessionCacheRefresh;
constexpr auto kTransactionRecordMinimumLifetimeMinutesDefault = 30;

extern int gTransactionRecordMinimumLifetimeMinutes;
}  // namespace mongo
