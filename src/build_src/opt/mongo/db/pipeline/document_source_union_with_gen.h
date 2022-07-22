/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_union_with_gen.h --output build/opt/mongo/db/pipeline/document_source_union_with_gen.cpp src/mongo/db/pipeline/document_source_union_with.idl
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
 * Specification for a $unionWith stage given in the full, explicit format.
 */
class UnionWithSpec {
public:
    static constexpr auto kCollFieldName = "coll"_sd;
    static constexpr auto kPipelineFieldName = "pipeline"_sd;

    UnionWithSpec();
    UnionWithSpec(std::string coll);

    static UnionWithSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The collection to union with.
     */
    const StringData getColl() const& { return _coll; }
    void getColl() && = delete;
    void setColl(StringData value) & { auto _tmpValue = value.toString();  _coll = std::move(_tmpValue); _hasColl = true; }

    /**
     * An optional pipeline to apply to the collection being unioned.
     */
    const boost::optional<std::vector<mongo::BSONObj>>& getPipeline() const& { return _pipeline; }
    void getPipeline() && = delete;
    void setPipeline(boost::optional<std::vector<mongo::BSONObj>> value) & {  _pipeline = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string _coll;
    boost::optional<std::vector<mongo::BSONObj>> _pipeline;
    bool _hasColl : 1;
};

}  // namespace mongo
