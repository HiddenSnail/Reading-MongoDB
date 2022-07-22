/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/client/sasl_aws_client_options_gen.h --output build/opt/mongo/client/sasl_aws_client_options_gen.cpp src/mongo/client/sasl_aws_client_options.idl
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
#include "mongo/client/sasl_aws_client_options.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/options_parser/option_description.h"

namespace mongo {
namespace awsIam {

constexpr auto kAwsEC2InstanceMetadataUrlDefault = "http://169.254.169.254";

constexpr auto kAwsECSInstanceMetadataUrlDefault = "http://169.254.170.2";

}  // namespace awsIam
}  // namespace mongo
