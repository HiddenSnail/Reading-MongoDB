/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/explain_verbosity_gen.h --output build/opt/mongo/db/query/explain_verbosity_gen.cpp src/mongo/db/query/explain_verbosity.idl
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
#include "mongo/stdx/unordered_map.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {
namespace explain {

/**
 * The various supported verbosity levels for explain. The order is significant: the enum values are assigned in order of increasing verbosity
 */
enum class VerbosityEnum : std::int32_t {
    kQueryPlanner,
    kExecStats,
    kExecAllPlans,
};
static constexpr uint32_t kNumVerbosityEnum = 3;

VerbosityEnum Verbosity_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData Verbosity_serializer(VerbosityEnum value);

}  // namespace explain
}  // namespace mongo
