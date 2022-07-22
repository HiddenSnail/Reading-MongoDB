/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/rpc/topology_version_gen.h --output build/opt/mongo/rpc/topology_version_gen.cpp src/mongo/rpc/topology_version.idl
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
 * Clients and servers exchange topologyVersions to determine the relative freshness of topology information in concurrent messages.
 */
class TopologyVersion {
public:
    static constexpr auto kCounterFieldName = "counter"_sd;
    static constexpr auto kProcessIdFieldName = "processId"_sd;

    TopologyVersion();
    TopologyVersion(mongo::OID processId, std::int64_t counter);

    static TopologyVersion parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * An id maintained in memory by the server. It is reinitialized by the server using the standard ObjectId logic each time this server process starts.
     */
    const mongo::OID& getProcessId() const { return _processId; }
    void setProcessId(mongo::OID value) & {  _processId = std::move(value); _hasProcessId = true; }

    /**
     * State change counter, maintained in memory by the server. It begins at 0 when the server starts, and it is incremented whenever there is a significant topology change.
     */
    std::int64_t getCounter() const { return _counter; }
    void setCounter(std::int64_t value) & {  _counter = std::move(value); _hasCounter = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::OID _processId;
    std::int64_t _counter;
    bool _hasProcessId : 1;
    bool _hasCounter : 1;
};

}  // namespace mongo
