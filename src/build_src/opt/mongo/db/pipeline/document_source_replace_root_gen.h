/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_replace_root_gen.h --output build/opt/mongo/db/pipeline/document_source_replace_root_gen.cpp src/mongo/db/pipeline/document_source_replace_root.idl
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
#include "mongo/db/exec/document_value/value.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"

namespace mongo {

/**
 * $replaceRoot aggregation stage
 */
class ReplaceRootSpec {
public:
    static constexpr auto kNewRootFieldName = "newRoot"_sd;

    ReplaceRootSpec();
    ReplaceRootSpec(mongo::Value newRoot);

    static ReplaceRootSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The expression to use to compute the new root of this stage, e.g. '$subObject'
     */
    const mongo::Value& getNewRoot() const { return _newRoot; }
    void setNewRoot(mongo::Value value) & {  _newRoot = std::move(value); _hasNewRoot = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::Value _newRoot;
    bool _hasNewRoot : 1;
};

}  // namespace mongo
