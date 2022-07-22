/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/expression_index_knobs_gen.h --output build/opt/mongo/db/query/expression_index_knobs_gen.cpp src/mongo/db/query/expression_index_knobs.idl
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

constexpr auto kInternalGeoPredicateQuery2DMaxCoveringCellsDefault = 16;

extern AtomicWord<int> gInternalGeoPredicateQuery2DMaxCoveringCells;
constexpr auto kInternalGeoNearQuery2DMaxCoveringCellsDefault = 16;

extern AtomicWord<int> gInternalGeoNearQuery2DMaxCoveringCells;
constexpr auto kInternalQueryS2GeoFinestLevelDefault = 23;

extern AtomicWord<int> gInternalQueryS2GeoFinestLevel;
constexpr auto kInternalQueryS2GeoCoarsestLevelDefault = 0;

extern AtomicWord<int> gInternalQueryS2GeoCoarsestLevel;
constexpr auto kInternalQueryS2GeoMaxCellsDefault = 20;

extern AtomicWord<int> gInternalQueryS2GeoMaxCells;
}  // namespace mongo
