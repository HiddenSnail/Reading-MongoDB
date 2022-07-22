/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/clustered_collection_options_gen.h --output build/opt/mongo/db/catalog/clustered_collection_options_gen.cpp src/mongo/db/catalog/clustered_collection_options.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/catalog/clustered_collection_options_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

constexpr StringData ClusteredIndexSpec::kKeyFieldName;
constexpr StringData ClusteredIndexSpec::kNameFieldName;
constexpr StringData ClusteredIndexSpec::kUniqueFieldName;
constexpr StringData ClusteredIndexSpec::kVFieldName;


ClusteredIndexSpec::ClusteredIndexSpec() : _key(mongo::idl::preparsedValue<decltype(_key)>()), _unique(mongo::idl::preparsedValue<decltype(_unique)>()), _hasKey(false), _hasUnique(false) {
    // Used for initialization only
}
ClusteredIndexSpec::ClusteredIndexSpec(mongo::BSONObj key, bool unique) : _key(std::move(key)), _unique(std::move(unique)), _hasKey(true), _hasUnique(true) {
    // Used for initialization only
}


ClusteredIndexSpec ClusteredIndexSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<ClusteredIndexSpec>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ClusteredIndexSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kVBit = 0;
    const size_t kKeyBit = 1;
    const size_t kNameBit = 2;
    const size_t kUniqueBit = 3;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kVFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kVBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kVBit);

                _v = element.safeNumberInt();
            }
        }
        else if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyBit);

                _hasKey = true;
                const BSONObj localObject = element.Obj();
                _key = BSONObj::getOwned(localObject);
            }
        }
        else if (fieldName == kNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNameBit);

                _name = element.str();
            }
        }
        else if (fieldName == kUniqueFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kUniqueBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kUniqueBit);

                _hasUnique = true;
                _unique = element.trueValue();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kKeyBit]) {
            ctxt.throwMissingField(kKeyFieldName);
        }
        if (!usedFields[kUniqueBit]) {
            ctxt.throwMissingField(kUniqueFieldName);
        }
    }

}


void ClusteredIndexSpec::serialize(BSONObjBuilder* builder) const {
    invariant(_hasKey && _hasUnique);

    builder->append(kVFieldName, _v);

    builder->append(kKeyFieldName, _key);

    if (_name.is_initialized()) {
        builder->append(kNameFieldName, _name.get());
    }

    builder->append(kUniqueFieldName, _unique);

}


BSONObj ClusteredIndexSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ClusteredCollectionInfo::kIndexSpecFieldName;
constexpr StringData ClusteredCollectionInfo::kLegacyFormatFieldName;


ClusteredCollectionInfo::ClusteredCollectionInfo() : _indexSpec(mongo::idl::preparsedValue<decltype(_indexSpec)>()), _legacyFormat(mongo::idl::preparsedValue<decltype(_legacyFormat)>()), _hasIndexSpec(false), _hasLegacyFormat(false) {
    // Used for initialization only
}
ClusteredCollectionInfo::ClusteredCollectionInfo(mongo::ClusteredIndexSpec indexSpec, bool legacyFormat) : _indexSpec(std::move(indexSpec)), _legacyFormat(std::move(legacyFormat)), _hasIndexSpec(true), _hasLegacyFormat(true) {
    // Used for initialization only
}


ClusteredCollectionInfo ClusteredCollectionInfo::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<ClusteredCollectionInfo>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ClusteredCollectionInfo::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kIndexSpecBit = 0;
    const size_t kLegacyFormatBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kIndexSpecFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kIndexSpecBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIndexSpecBit);

                _hasIndexSpec = true;
                IDLParserErrorContext tempContext(kIndexSpecFieldName, &ctxt);
                const auto localObject = element.Obj();
                _indexSpec = mongo::ClusteredIndexSpec::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kLegacyFormatFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kLegacyFormatBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kLegacyFormatBit);

                _hasLegacyFormat = true;
                _legacyFormat = element.trueValue();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kIndexSpecBit]) {
            ctxt.throwMissingField(kIndexSpecFieldName);
        }
        if (!usedFields[kLegacyFormatBit]) {
            ctxt.throwMissingField(kLegacyFormatFieldName);
        }
    }

}


void ClusteredCollectionInfo::serialize(BSONObjBuilder* builder) const {
    invariant(_hasIndexSpec && _hasLegacyFormat);

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kIndexSpecFieldName));
        _indexSpec.serialize(&subObjBuilder);
    }

    builder->append(kLegacyFormatFieldName, _legacyFormat);

}


BSONObj ClusteredCollectionInfo::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

bool gSupportArbitraryClusterKeyIndex{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_d01d5c6b87eff01e7c98c7695994afe1c38ba038)(InitializerContext*) {
    /**
     * Enables arbitrary cluster index key support.
     */
    auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("supportArbitraryClusterKeyIndex", gSupportArbitraryClusterKeyIndex);
        return ret;
    })();
    scp_0->setTestOnly();

}
}  // namespace mongo
