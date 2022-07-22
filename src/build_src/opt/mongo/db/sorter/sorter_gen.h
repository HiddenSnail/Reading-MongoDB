/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/sorter/sorter_gen.h --output build/opt/mongo/db/sorter/sorter_gen.cpp src/mongo/db/sorter/sorter.idl
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
#include "mongo/db/logical_time.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/tenant_id.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/basic_types_gen.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/stdx/unordered_map.h"
#include "mongo/util/uuid.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {

/**
 * The range of data that was sorted and spilled to disk.
 */
class SorterRange {
public:
    static constexpr auto kChecksumFieldName = "checksum"_sd;
    static constexpr auto kEndOffsetFieldName = "endOffset"_sd;
    static constexpr auto kStartOffsetFieldName = "startOffset"_sd;

    SorterRange();
    SorterRange(std::int64_t startOffset, std::int64_t endOffset, std::int64_t checksum);

    static SorterRange parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Tracks where in the file we started writing this data range.
     */
    std::int64_t getStartOffset() const { return _startOffset; }
    void setStartOffset(std::int64_t value) {  _startOffset = std::move(value); _hasStartOffset = true; }
    /**
     * Tracks where in the file we finished writing this data range.
     */
    std::int64_t getEndOffset() const { return _endOffset; }
    void setEndOffset(std::int64_t value) {  _endOffset = std::move(value); _hasEndOffset = true; }
    /**
     * Tracks the hash of all data objects spilled to disk.
     */
    std::int64_t getChecksum() const { return _checksum; }
    void setChecksum(std::int64_t value) { validateChecksum(value); _checksum = std::move(value); _hasChecksum = true; }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    void validateChecksum(std::int64_t value);
    void validateChecksum(IDLParserErrorContext& ctxt, std::int64_t value);

private:
    std::int64_t _startOffset;
    std::int64_t _endOffset;
    std::int64_t _checksum;
    bool _hasStartOffset : 1;
    bool _hasEndOffset : 1;
    bool _hasChecksum : 1;
};

}  // namespace mongo
