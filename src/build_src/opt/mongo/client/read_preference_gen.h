/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/read_preference_gen.h --output build/opt/mongo/client/read_preference_gen.cpp src/mongo/client/read_preference.idl
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
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Enumeration representing Read Preference Modes
 */
enum class ReadPreferenceEnum : std::int32_t {
    PrimaryOnly ,
    PrimaryPreferred ,
    SecondaryOnly ,
    SecondaryPreferred ,
    Nearest ,
};

ReadPreferenceEnum ReadPreference_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData ReadPreference_serializer(ReadPreferenceEnum value);

}  // namespace mongo
