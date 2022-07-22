/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/sharding_task_executor_pool_gen.h --output build/opt/mongo/s/sharding_task_executor_pool_gen.cpp src/mongo/s/sharding_task_executor_pool.idl
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
#include "mongo/s/sharding_task_executor_pool_controller.h"

namespace mongo {

constexpr auto kShardingTaskExecutorPoolMinSizeDefault = 1;

constexpr auto kShardingTaskExecutorPoolMaxSizeDefault = 32767;

constexpr auto kShardingTaskExecutorPoolMaxConnectingDefault = 2;

constexpr auto kShardingTaskExecutorPoolHostTimeoutMSDefault = 300000;

constexpr auto kShardingTaskExecutorPoolRefreshRequirementMSDefault = 60000;

constexpr auto kShardingTaskExecutorPoolRefreshTimeoutMSDefault = 20000;

constexpr auto kShardingTaskExecutorPoolReplicaSetMatchingDefault = "matchPrimaryNode";

}  // namespace mongo
