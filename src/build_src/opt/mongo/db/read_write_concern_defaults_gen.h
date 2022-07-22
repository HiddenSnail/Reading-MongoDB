/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/read_write_concern_defaults_gen.h --output build/opt/mongo/db/read_write_concern_defaults_gen.cpp src/mongo/db/read_write_concern_defaults.idl
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
#include "mongo/db/repl/read_concern_args.h"
#include "mongo/db/write_concern_options.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Represents a set of read/write concern defaults, and associated metadata
 */
class RWConcernDefault {
public:
    static constexpr auto kDefaultReadConcernFieldName = "defaultReadConcern"_sd;
    static constexpr auto kDefaultWriteConcernFieldName = "defaultWriteConcern"_sd;
    static constexpr auto kUpdateOpTimeFieldName = "updateOpTime"_sd;
    static constexpr auto kUpdateWallClockTimeFieldName = "updateWallClockTime"_sd;

    RWConcernDefault();

    static RWConcernDefault parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The default read concern
     */
    const boost::optional<mongo::repl::ReadConcernArgs>& getDefaultReadConcern() const& { return _defaultReadConcern; }
    void getDefaultReadConcern() && = delete;
    void setDefaultReadConcern(boost::optional<mongo::repl::ReadConcernArgs> value) & {  _defaultReadConcern = std::move(value);  }

    /**
     * The default write concern
     */
    const boost::optional<mongo::WriteConcernOptions>& getDefaultWriteConcern() const& { return _defaultWriteConcern; }
    void getDefaultWriteConcern() && = delete;
    void setDefaultWriteConcern(boost::optional<mongo::WriteConcernOptions> value) & {  _defaultWriteConcern = std::move(value);  }

    /**
     * The optime of when the default read or write concern was last set. On replica sets it advances with the primary's optime and on clusters it advances with the config server primary's optime.
     */
    const boost::optional<mongo::Timestamp>& getUpdateOpTime() const& { return _updateOpTime; }
    void getUpdateOpTime() && = delete;
    void setUpdateOpTime(boost::optional<mongo::Timestamp> value) & {  _updateOpTime = std::move(value);  }

    /**
     * The wallclock time when the default read or write concern was last set by an administrator. This value is only informational and must not be used for any recency comparisons.
     */
    const boost::optional<mongo::Date_t>& getUpdateWallClockTime() const& { return _updateWallClockTime; }
    void getUpdateWallClockTime() && = delete;
    void setUpdateWallClockTime(boost::optional<mongo::Date_t> value) & {  _updateWallClockTime = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::repl::ReadConcernArgs> _defaultReadConcern;
    boost::optional<mongo::WriteConcernOptions> _defaultWriteConcern;
    boost::optional<mongo::Timestamp> _updateOpTime;
    boost::optional<mongo::Date_t> _updateWallClockTime;
};

}  // namespace mongo
