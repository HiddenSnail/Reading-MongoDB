/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_merge_gen.h --output build/opt/mongo/db/pipeline/document_source_merge_gen.cpp src/mongo/db/pipeline/document_source_merge.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/pipeline/document_source_merge_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData NamespaceSpec::kCollFieldName;
constexpr StringData NamespaceSpec::kDbFieldName;


NamespaceSpec::NamespaceSpec()  {
    // Used for initialization only
}


NamespaceSpec NamespaceSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceSpec object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void NamespaceSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kDbBit = 0;
    const size_t kCollBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kDbFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbBit);

                _db = element.str();
            }
        }
        else if (fieldName == kCollFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kCollBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollBit);

                _coll = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void NamespaceSpec::serialize(BSONObjBuilder* builder) const {
    if (_db.is_initialized()) {
        builder->append(kDbFieldName, _db.get());
    }

    if (_coll.is_initialized()) {
        builder->append(kCollFieldName, _coll.get());
    }

}


BSONObj NamespaceSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData DocumentSourceMergeSpec::kLetFieldName;
constexpr StringData DocumentSourceMergeSpec::kOnFieldName;
constexpr StringData DocumentSourceMergeSpec::kTargetCollectionVersionFieldName;
constexpr StringData DocumentSourceMergeSpec::kTargetNssFieldName;
constexpr StringData DocumentSourceMergeSpec::kWhenMatchedFieldName;
constexpr StringData DocumentSourceMergeSpec::kWhenNotMatchedFieldName;


DocumentSourceMergeSpec::DocumentSourceMergeSpec() : _hasTargetNss(false) {
    // Used for initialization only
}
DocumentSourceMergeSpec::DocumentSourceMergeSpec(mongo::NamespaceString targetNss) : _targetNss(std::move(targetNss)), _hasTargetNss(true) {
    // Used for initialization only
}


DocumentSourceMergeSpec DocumentSourceMergeSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DocumentSourceMergeSpec object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DocumentSourceMergeSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<6> usedFields;
    const size_t kTargetNssBit = 0;
    const size_t kOnBit = 1;
    const size_t kLetBit = 2;
    const size_t kWhenMatchedBit = 3;
    const size_t kWhenNotMatchedBit = 4;
    const size_t kTargetCollectionVersionBit = 5;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kTargetNssFieldName) {
            if (MONGO_unlikely(usedFields[kTargetNssBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kTargetNssBit);

            _hasTargetNss = true;
            _targetNss = mergeTargetNssParseFromBSON(element);
        }
        else if (fieldName == kOnFieldName) {
            if (MONGO_unlikely(usedFields[kOnBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kOnBit);

            _on = mergeOnFieldsParseFromBSON(element);
        }
        else if (fieldName == kLetFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kLetBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLetBit);

                _let = element.Obj();
            }
        }
        else if (fieldName == kWhenMatchedFieldName) {
            if (MONGO_unlikely(usedFields[kWhenMatchedBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kWhenMatchedBit);

            _whenMatched = mergeWhenMatchedParseFromBSON(element);
        }
        else if (fieldName == kWhenNotMatchedFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kWhenNotMatchedBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kWhenNotMatchedBit);

                IDLParserErrorContext tempContext(kWhenNotMatchedFieldName, &ctxt);
                _whenNotMatched = MergeWhenNotMatchedMode_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kTargetCollectionVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kTargetCollectionVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTargetCollectionVersionBit);

                const BSONObj localObject = element.Obj();
                _targetCollectionVersion = ChunkVersion::fromBSONThrowing(localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kTargetNssBit]) {
            ctxt.throwMissingField(kTargetNssFieldName);
        }
    }

}


void DocumentSourceMergeSpec::serialize(BSONObjBuilder* builder) const {
    invariant(_hasTargetNss);

    {
        mergeTargetNssSerializeToBSON(_targetNss, kTargetNssFieldName, builder);
    }

    if (_on.is_initialized()) {
        mergeOnFieldsSerializeToBSON(_on.get(), kOnFieldName, builder);
    }

    if (_let.is_initialized()) {
        builder->append(kLetFieldName, _let.get());
    }

    if (_whenMatched.is_initialized()) {
        mergeWhenMatchedSerializeToBSON(_whenMatched.get(), kWhenMatchedFieldName, builder);
    }

    if (_whenNotMatched.is_initialized()) {
        builder->append(kWhenNotMatchedFieldName, MergeWhenNotMatchedMode_serializer(_whenNotMatched.get()));
    }

    if (_targetCollectionVersion.is_initialized()) {
        const BSONObj localObject = _targetCollectionVersion.get().toBSON();
        builder->append(kTargetCollectionVersionFieldName, localObject);
    }

}


BSONObj DocumentSourceMergeSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
