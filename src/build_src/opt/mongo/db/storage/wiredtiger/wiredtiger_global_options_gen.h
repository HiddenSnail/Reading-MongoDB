/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/wiredtiger/wiredtiger_global_options_gen.h --output build/opt/mongo/db/storage/wiredtiger/wiredtiger_global_options_gen.cpp src/mongo/db/storage/wiredtiger/wiredtiger_global_options.idl
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
#include "mongo/db/storage/wiredtiger/wiredtiger_global_options.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/options_parser/option_description.h"

namespace mongo {

constexpr auto kStorage_wiredTiger_engineConfig_statisticsLogDelaySecsDefault = 0;

constexpr auto kStorage_wiredTiger_engineConfig_journalCompressorDefault = "snappy";

constexpr auto kStorage_wiredTiger_engineConfig_maxCacheOverflowFileSizeGBDefault = 0.0;

constexpr auto kStorage_wiredTiger_collectionConfig_blockCompressorDefault = "snappy";

constexpr auto kStorage_wiredTiger_indexConfig_prefixCompressionDefault = true;

}  // namespace mongo
