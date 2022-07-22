/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/database_cloner_gen.h --output build/opt/mongo/db/repl/database_cloner_gen.cpp src/mongo/db/repl/database_cloner.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/repl/database_cloner_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace repl {

constexpr StringData CollectionInfo::kReadOnlyFieldName;
constexpr StringData CollectionInfo::kUuidFieldName;


CollectionInfo::CollectionInfo() : _hasUuid(false) {
    // Used for initialization only
}
CollectionInfo::CollectionInfo(mongo::UUID uuid) : _uuid(std::move(uuid)), _hasUuid(true) {
    // Used for initialization only
}


CollectionInfo CollectionInfo::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CollectionInfo object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CollectionInfo::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kReadOnlyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _readOnly = element.boolean();
            }
        }
        else if (fieldName == kUuidFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _hasUuid = true;
                _uuid = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kUuidFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kUuidFieldName);
    }

}


void CollectionInfo::serialize(BSONObjBuilder* builder) const {
    invariant(_hasUuid);

    if (_readOnly.is_initialized()) {
        builder->append(kReadOnlyFieldName, _readOnly.get());
    }

    {
        ConstDataRange tempCDR = _uuid.toCDR();
        builder->append(kUuidFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

}


BSONObj CollectionInfo::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ListCollectionResult::kInfoFieldName;
constexpr StringData ListCollectionResult::kNameFieldName;
constexpr StringData ListCollectionResult::kOptionsFieldName;
constexpr StringData ListCollectionResult::kTypeFieldName;


ListCollectionResult::ListCollectionResult() : _hasName(false), _hasType(false), _hasOptions(false), _hasInfo(false) {
    // Used for initialization only
}
ListCollectionResult::ListCollectionResult(std::string name, std::string type, mongo::CollectionOptions options, mongo::repl::CollectionInfo info) : _name(std::move(name)), _type(std::move(type)), _options(std::move(options)), _info(std::move(info)), _hasName(true), _hasType(true), _hasOptions(true), _hasInfo(true) {
    // Used for initialization only
}


ListCollectionResult ListCollectionResult::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ListCollectionResult object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ListCollectionResult::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasName = true;
                _name = element.str();
            }
        }
        else if (fieldName == kTypeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasType = true;
                _type = element.str();
            }
        }
        else if (fieldName == kOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasOptions = true;
                const BSONObj localObject = element.Obj();
                _options = repl::DatabaseCloner::parseCollectionOptions(localObject);
            }
        }
        else if (fieldName == kInfoFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                _hasInfo = true;
                IDLParserErrorContext tempContext(kInfoFieldName, &ctxt);
                const auto localObject = element.Obj();
                _info = mongo::repl::CollectionInfo::parse(tempContext, localObject);
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kNameFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kTypeFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kTypeFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kOptionsFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kOptionsFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kInfoFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kInfoFieldName);
    }

}


void ListCollectionResult::serialize(BSONObjBuilder* builder) const {
    invariant(_hasName && _hasType && _hasOptions && _hasInfo);

    builder->append(kNameFieldName, _name);

    builder->append(kTypeFieldName, _type);

    {
        const BSONObj localObject = _options.toBSON();
        builder->append(kOptionsFieldName, localObject);
    }

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kInfoFieldName));
        _info.serialize(&subObjBuilder);
    }

}


BSONObj ListCollectionResult::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace repl
}  // namespace mongo
