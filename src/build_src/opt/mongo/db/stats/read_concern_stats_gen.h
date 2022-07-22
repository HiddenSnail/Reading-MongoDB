/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/stats/read_concern_stats_gen.h --output build/opt/mongo/db/stats/read_concern_stats_gen.cpp src/mongo/db/stats/read_concern_stats.idl
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
 * A struct representing the section of the server status command with information about readConcern levels used by operations
 */
class ReadConcernStats {
public:
    static constexpr auto kAvailableFieldName = "available"_sd;
    static constexpr auto kLinearizableFieldName = "linearizable"_sd;
    static constexpr auto kLocalFieldName = "local"_sd;
    static constexpr auto kMajorityFieldName = "majority"_sd;
    static constexpr auto kNoneFieldName = "none"_sd;
    static constexpr auto kSnapshotFieldName = "snapshot"_sd;

    ReadConcernStats();

    static ReadConcernStats parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getAvailable() const { return _available; }
    void setAvailable(std::int64_t value) & {  _available = std::move(value);  }

    std::int64_t getLinearizable() const { return _linearizable; }
    void setLinearizable(std::int64_t value) & {  _linearizable = std::move(value);  }

    std::int64_t getLocal() const { return _local; }
    void setLocal(std::int64_t value) & {  _local = std::move(value);  }

    std::int64_t getMajority() const { return _majority; }
    void setMajority(std::int64_t value) & {  _majority = std::move(value);  }

    std::int64_t getSnapshot() const { return _snapshot; }
    void setSnapshot(std::int64_t value) & {  _snapshot = std::move(value);  }

    std::int64_t getNone() const { return _none; }
    void setNone(std::int64_t value) & {  _none = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _available{0};
    std::int64_t _linearizable{0};
    std::int64_t _local{0};
    std::int64_t _majority{0};
    std::int64_t _snapshot{0};
    std::int64_t _none{0};
};

}  // namespace mongo
