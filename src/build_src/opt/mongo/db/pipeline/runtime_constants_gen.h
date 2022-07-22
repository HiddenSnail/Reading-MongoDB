/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/runtime_constants_gen.h --output build/opt/mongo/db/pipeline/runtime_constants_gen.cpp src/mongo/db/pipeline/runtime_constants.idl
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
 * A collection of values that do not change once computed. It is shipped as part of an aggregation request so all shards participating in a query see the same values.
 */
class RuntimeConstants {
public:
    static constexpr auto kClusterTimeFieldName = "clusterTime"_sd;
    static constexpr auto kIsMapReduceFieldName = "isMapReduce"_sd;
    static constexpr auto kJsScopeFieldName = "jsScope"_sd;
    static constexpr auto kLocalNowFieldName = "localNow"_sd;

    RuntimeConstants();
    RuntimeConstants(mongo::Date_t localNow, mongo::Timestamp clusterTime);

    static RuntimeConstants parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * A value of the $$NOW variable.
     */
    const mongo::Date_t& getLocalNow() const { return _localNow; }
    void setLocalNow(mongo::Date_t value) & {  _localNow = std::move(value); _hasLocalNow = true; }

    /**
     * A value of the $$CLUSTER_TIME variable.
     */
    const mongo::Timestamp& getClusterTime() const { return _clusterTime; }
    void setClusterTime(mongo::Timestamp value) & {  _clusterTime = std::move(value); _hasClusterTime = true; }

    /**
     * Optional scope variables accessible from internal javacsript expressions.
     */
    const boost::optional<mongo::BSONObj>& getJsScope() const& { return _jsScope; }
    void getJsScope() && = delete;
    void setJsScope(boost::optional<mongo::BSONObj> value) & {  _jsScope = std::move(value);  }

    /**
     * When specified, signals that the aggregation pipeline originated from a mapReduce command.
     */
    const boost::optional<bool> getIsMapReduce() const& { return _isMapReduce; }
    void getIsMapReduce() && = delete;
    void setIsMapReduce(boost::optional<bool> value) & {  _isMapReduce = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::Date_t _localNow;
    mongo::Timestamp _clusterTime;
    boost::optional<mongo::BSONObj> _jsScope;
    boost::optional<bool> _isMapReduce;
    bool _hasLocalNow : 1;
    bool _hasClusterTime : 1;
};

}  // namespace mongo
