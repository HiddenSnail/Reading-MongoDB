/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/rpc/topology_version_gen.h --output build/opt/mongo/rpc/topology_version_gen.cpp src/mongo/rpc/topology_version.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/rpc/topology_version_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData TopologyVersion::kCounterFieldName;
constexpr StringData TopologyVersion::kProcessIdFieldName;


TopologyVersion::TopologyVersion() : _counter(-1), _hasProcessId(false), _hasCounter(false) {
    // Used for initialization only
}
TopologyVersion::TopologyVersion(mongo::OID processId, std::int64_t counter) : _processId(std::move(processId)), _counter(std::move(counter)), _hasProcessId(true), _hasCounter(true) {
    // Used for initialization only
}


TopologyVersion TopologyVersion::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TopologyVersion object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TopologyVersion::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kProcessIdBit = 0;
    const size_t kCounterBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kProcessIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, jstOID))) {
                if (MONGO_unlikely(usedFields[kProcessIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kProcessIdBit);

                _hasProcessId = true;
                _processId = element.OID();
            }
        }
        else if (fieldName == kCounterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCounterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCounterBit);

                _hasCounter = true;
                _counter = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kProcessIdBit]) {
            ctxt.throwMissingField(kProcessIdFieldName);
        }
        if (!usedFields[kCounterBit]) {
            ctxt.throwMissingField(kCounterFieldName);
        }
    }

}


void TopologyVersion::serialize(BSONObjBuilder* builder) const {
    invariant(_hasProcessId && _hasCounter);

    builder->append(kProcessIdFieldName, _processId);

    builder->append(kCounterFieldName, _counter);

}


BSONObj TopologyVersion::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
