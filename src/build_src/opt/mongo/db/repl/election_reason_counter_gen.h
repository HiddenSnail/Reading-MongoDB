/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/election_reason_counter_gen.h --output build/opt/mongo/db/repl/election_reason_counter_gen.cpp src/mongo/db/repl/election_reason_counter.idl
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
namespace repl {

/**
 * Stores the total number of elections a node has called for a given reason and the number of those elections that were successful
 */
class ElectionReasonCounterBase {
public:
    static constexpr auto kCalledFieldName = "called"_sd;
    static constexpr auto kSuccessfulFieldName = "successful"_sd;

    ElectionReasonCounterBase();

    static ElectionReasonCounterBase parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Number of elections this node has called for the given reason
     */
    std::int64_t getCalled() const { return _called; }
    void setCalled(std::int64_t value) & {  _called = std::move(value);  }

    /**
     * Number of successful elections for the given reason
     */
    std::int64_t getSuccessful() const { return _successful; }
    void setSuccessful(std::int64_t value) & {  _successful = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _called{0};
    std::int64_t _successful{0};
};

}  // namespace repl
}  // namespace mongo
