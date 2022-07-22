/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/transaction_participant_gen.h --output build/opt/mongo/db/transaction_participant_gen.cpp src/mongo/db/transaction_participant.idl
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
#include "mongo/db/transaction_participant.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"

namespace mongo {

constexpr auto kMaxTransactionLockRequestTimeoutMillisDefault = 5;

extern AtomicWord<int> gMaxTransactionLockRequestTimeoutMillis;
constexpr auto kTransactionLifetimeLimitSecondsDefault = 60;

extern AtomicWord<decltype(TransactionParticipant::observeTransactionLifetimeLimitSeconds)::Argument> gTransactionLifetimeLimitSeconds;
constexpr auto kMaxNumberOfTransactionOperationsInSingleOplogEntryDefault = 2147483647;

extern int gMaxNumberOfTransactionOperationsInSingleOplogEntry;
constexpr auto kTransactionSizeLimitBytesDefault = std::numeric_limits<long long>::max();

extern AtomicWord<long long> gTransactionSizeLimitBytes;
}  // namespace mongo
