/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/tailable_mode_gen.h --output build/opt/mongo/db/query/tailable_mode_gen.cpp src/mongo/db/query/tailable_mode.idl
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

namespace mongo {

/**
 * Describes the tailablility of a cursor.
 */
enum class TailableModeEnum : std::int32_t {
    kNormal ,
    kTailable ,
    kTailableAndAwaitData ,
};

TailableModeEnum TailableMode_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData TailableMode_serializer(TailableModeEnum value);

}  // namespace mongo
