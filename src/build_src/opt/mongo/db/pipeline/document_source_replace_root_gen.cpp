/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_replace_root_gen.h --output build/opt/mongo/db/pipeline/document_source_replace_root_gen.cpp src/mongo/db/pipeline/document_source_replace_root.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/pipeline/document_source_replace_root_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ReplaceRootSpec::kNewRootFieldName;


ReplaceRootSpec::ReplaceRootSpec() : _hasNewRoot(false) {
    // Used for initialization only
}
ReplaceRootSpec::ReplaceRootSpec(mongo::Value newRoot) : _newRoot(std::move(newRoot)), _hasNewRoot(true) {
    // Used for initialization only
}


ReplaceRootSpec ReplaceRootSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ReplaceRootSpec object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ReplaceRootSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kNewRootBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kNewRootFieldName) {
            if (MONGO_unlikely(usedFields[kNewRootBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kNewRootBit);

            _hasNewRoot = true;
            _newRoot = Value::deserializeForIDL(element);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kNewRootBit]) {
            ctxt.throwMissingField(kNewRootFieldName);
        }
    }

}


void ReplaceRootSpec::serialize(BSONObjBuilder* builder) const {
    invariant(_hasNewRoot);

    {
        _newRoot.serializeForIDL(kNewRootFieldName, builder);
    }

}


BSONObj ReplaceRootSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
