/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/storage/flow_control_parameters_gen.h --output build/opt/mongo/db/storage/flow_control_parameters_gen.cpp src/mongo/db/storage/flow_control_parameters.idl
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

constexpr auto kEnableFlowControlDefault = true;

extern AtomicWord<bool> gFlowControlEnabled;
constexpr auto kFlowControlTargetLagSecondsDefault = 10;

extern AtomicWord<std::int32_t> gFlowControlTargetLagSeconds;
constexpr auto kFlowControlThresholdLagPercentageDefault = 0.5;

extern AtomicWord<double> gFlowControlThresholdLagPercentage;
constexpr auto kFlowControlMaxSamplesDefault = 1000000;

extern int gFlowControlMaxSamples;
constexpr auto kFlowControlSamplePeriodDefault = 1000;

extern AtomicWord<int> gFlowControlSamplePeriod;
constexpr auto kFlowControlMinTicketsPerSecondDefault = 100;

extern AtomicWord<int> gFlowControlMinTicketsPerSecond;
constexpr auto kFlowControlDecayConstantDefault = 0.5;

extern AtomicWord<double> gFlowControlDecayConstant;
constexpr auto kFlowControlFudgeFactorDefault = 0.95;

extern AtomicWord<double> gFlowControlFudgeFactor;
constexpr auto kFlowControlTicketAdderConstantDefault = 1000;

extern AtomicWord<int> gFlowControlTicketAdderConstant;
constexpr auto kFlowControlTicketMultiplierConstantDefault = 1.05;

extern AtomicWord<double> gFlowControlTicketMultiplierConstant;
constexpr auto kFlowControlWarnThresholdSecondsDefault = 10;

extern AtomicWord<int> gFlowControlWarnThresholdSeconds;
}  // namespace mongo
