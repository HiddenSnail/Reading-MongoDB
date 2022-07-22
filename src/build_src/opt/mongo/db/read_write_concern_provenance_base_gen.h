/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/read_write_concern_provenance_base_gen.h --output build/opt/mongo/db/read_write_concern_provenance_base_gen.cpp src/mongo/db/read_write_concern_provenance_base.idl
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
 * Provenance sources
 */
enum class ReadWriteConcernProvenanceSourceEnum : std::int32_t {
    clientSupplied ,
    implicitDefault ,
    customDefault ,
    getLastErrorDefaults ,
};

ReadWriteConcernProvenanceSourceEnum ReadWriteConcernProvenanceSource_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData ReadWriteConcernProvenanceSource_serializer(ReadWriteConcernProvenanceSourceEnum value);

/**
 * Represents the 'provenance' (ie. original source) of a read or write concern.
 */
class ReadWriteConcernProvenanceBase {
public:
    static constexpr auto kSourceFieldName = "provenance"_sd;

    ReadWriteConcernProvenanceBase();

    static ReadWriteConcernProvenanceBase parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The source for this provenance
     */
    const boost::optional<mongo::ReadWriteConcernProvenanceSourceEnum> getSource() const& { return _source; }
    void getSource() && = delete;
    void setSource(boost::optional<mongo::ReadWriteConcernProvenanceSourceEnum> value) & {  _source = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::ReadWriteConcernProvenanceSourceEnum> _source;
};

}  // namespace mongo
