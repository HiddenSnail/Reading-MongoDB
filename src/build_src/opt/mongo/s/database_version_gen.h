/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/database_version_gen.h --output build/opt/mongo/s/database_version_gen.cpp src/mongo/s/database_version.idl
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
 * A (UUID, int) pair that uniquely identifies an incarnation of a database in a cluster and reflects changes to the database's primary shard
 */
class DatabaseVersion {
public:
    static constexpr auto kLastModFieldName = "lastMod"_sd;
    static constexpr auto kUuidFieldName = "uuid"_sd;

    DatabaseVersion();
    DatabaseVersion(mongo::UUID uuid, std::int32_t lastMod);

    static DatabaseVersion parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * a unique identifier to distinguish different incarnations of this database
     */
    const mongo::UUID& getUuid() const { return _uuid; }
    void setUuid(mongo::UUID value) & {  _uuid = std::move(value); _hasUuid = true; }

    /**
     * an integer which is bumped whenever the database's primary shard changes
     */
    std::int32_t getLastMod() const { return _lastMod; }
    void setLastMod(std::int32_t value) & {  _lastMod = std::move(value); _hasLastMod = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _uuid;
    std::int32_t _lastMod;
    bool _hasUuid : 1;
    bool _hasLastMod : 1;
};

}  // namespace mongo
