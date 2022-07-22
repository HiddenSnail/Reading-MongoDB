/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/server_options_general_gen.h --output build/opt/mongo/db/server_options_general_gen.cpp src/mongo/db/server_options_general.idl
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
#include "mongo/db/server_options.h"
#include "mongo/db/server_options_base.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/options_parser/environment.h"
#include "mongo/util/options_parser/option_description.h"
#include "mongo/util/options_parser/option_section.h"

namespace mongo {

constexpr auto kConfigExpandTimeoutSecsDefault = 30;

constexpr auto kNet_listenBacklogDefault = SOMAXCONN;

constexpr auto kNet_maxIncomingConnectionsDefault = static_cast<int>(DEFAULT_MAX_CONN);

constexpr auto kNet_transportLayerDefault = "asio";

constexpr auto kNet_serviceExecutorDefault = "synchronous";

Status addGeneralServerOptionDefinitions(optionenvironment::OptionSection*);

}  // namespace mongo
