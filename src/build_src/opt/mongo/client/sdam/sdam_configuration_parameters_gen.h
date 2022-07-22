/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sdam/sdam_configuration_parameters_gen.h --output build/opt/mongo/client/sdam/sdam_configuration_parameters_gen.cpp src/mongo/client/sdam/sdam_configuration_parameters.idl
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
namespace sdam {

constexpr auto kHeartBeatFrequencyMsDefault = 10000;

extern int sdamHeartBeatFrequencyMs;
constexpr auto kLocalThresholdMsDefault = 15;

extern int sdamLocalThreshholdMs;
constexpr auto kConnectTimeoutMsDefault = 10000;

extern int sdamConnectTimeoutMs;
}  // namespace sdam
}  // namespace mongo
