/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/initialize_server_global_state_gen.h --output build/opt/mongo/db/initialize_server_global_state_gen.cpp src/mongo/db/initialize_server_global_state.idl
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
#include "mongo/logger/message_event_utf8_encoder.h"
#include "mongo/logv2/constants.h"
#include "mongo/rpc/op_msg.h"

namespace mongo {

constexpr auto kMaxLogSizeKBDefault = logv2::constants::kDefaultMaxAttributeOutputSizeKB;

extern AtomicWord<int32_t> gMaxLogAttributeSizeKB;
class HonorSystemUMaskServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

class ProcessUMaskServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

}  // namespace mongo
