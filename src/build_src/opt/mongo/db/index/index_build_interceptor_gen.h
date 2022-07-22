/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/index/index_build_interceptor_gen.h --output build/opt/mongo/db/index/index_build_interceptor_gen.cpp src/mongo/db/index/index_build_interceptor.idl
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
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

constexpr auto kMaxIndexBuildDrainBatchSizeDefault = 1000;

extern AtomicWord<int> maxIndexBuildDrainBatchSize;
constexpr auto kMaxIndexBuildDrainMemoryUsageMegabytesDefault = 16;

extern AtomicWord<int> maxIndexBuildDrainMemoryUsageMegabytes;
}  // namespace mongo
