/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/transport/message_compressor_options_server_gen.h --output build/opt/mongo/transport/message_compressor_options_server_gen.cpp src/mongo/transport/message_compressor_options_server.idl
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
#include "mongo/rpc/op_msg.h"
#include "mongo/util/options_parser/option_description.h"

namespace mongo {

constexpr auto kNet_compression_compressorsDefault = "snappy,zstd,zlib";

}  // namespace mongo
