/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/replication_coordinator_impl_gen.h --output build/opt/mongo/db/repl/replication_coordinator_impl_gen.cpp src/mongo/db/repl/replication_coordinator_impl.idl
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
namespace repl {

constexpr auto kNumInitialSyncAttemptsDefault = 10;

extern AtomicWord<int> numInitialSyncAttempts;
constexpr auto kEnableElectionHandoffDefault = true;

extern AtomicWord<bool> enableElectionHandoff;
constexpr auto kWaitForStepDownOnNonCommandShutdownDefault = true;

extern AtomicWord<bool> waitForStepDownOnNonCommandShutdown;
constexpr auto kPeriodicNoopIntervalSecsDefault = 10;

extern AtomicWord<int> periodicNoopIntervalSecs;
extern bool gTestingSnapshotBehaviorInIsolation;
}  // namespace repl
}  // namespace mongo
