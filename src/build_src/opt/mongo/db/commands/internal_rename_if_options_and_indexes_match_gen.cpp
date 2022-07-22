/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/internal_rename_if_options_and_indexes_match_gen.h --output build/opt/mongo/db/commands/internal_rename_if_options_and_indexes_match_gen.cpp src/mongo/db/commands/internal_rename_if_options_and_indexes_match.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/internal_rename_if_options_and_indexes_match_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData InternalRenameIfOptionsAndIndexesMatch::kCollectionOptionsFieldName;
constexpr StringData InternalRenameIfOptionsAndIndexesMatch::kDbNameFieldName;
constexpr StringData InternalRenameIfOptionsAndIndexesMatch::kFromFieldName;
constexpr StringData InternalRenameIfOptionsAndIndexesMatch::kIndexesFieldName;
constexpr StringData InternalRenameIfOptionsAndIndexesMatch::kToFieldName;
constexpr StringData InternalRenameIfOptionsAndIndexesMatch::kCommandName;

const std::vector<StringData> InternalRenameIfOptionsAndIndexesMatch::_knownBSONFields {
    InternalRenameIfOptionsAndIndexesMatch::kCollectionOptionsFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kFromFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kIndexesFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kToFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kCommandName,
};
const std::vector<StringData> InternalRenameIfOptionsAndIndexesMatch::_knownOP_MSGFields {
    InternalRenameIfOptionsAndIndexesMatch::kCollectionOptionsFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kDbNameFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kFromFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kIndexesFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kToFieldName,
    InternalRenameIfOptionsAndIndexesMatch::kCommandName,
};

InternalRenameIfOptionsAndIndexesMatch::InternalRenameIfOptionsAndIndexesMatch() : _hasFrom(false), _hasTo(false), _hasCollectionOptions(false), _hasIndexes(false), _hasDbName(false) {
    // Used for initialization only
}
InternalRenameIfOptionsAndIndexesMatch::InternalRenameIfOptionsAndIndexesMatch(mongo::NamespaceString from, mongo::NamespaceString to, mongo::BSONObj collectionOptions, std::vector<mongo::BSONObj> indexes) : _from(std::move(from)), _to(std::move(to)), _collectionOptions(std::move(collectionOptions)), _indexes(std::move(indexes)), _hasFrom(true), _hasTo(true), _hasCollectionOptions(true), _hasIndexes(true), _hasDbName(true) {
    // Used for initialization only
}


InternalRenameIfOptionsAndIndexesMatch InternalRenameIfOptionsAndIndexesMatch::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    InternalRenameIfOptionsAndIndexesMatch object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void InternalRenameIfOptionsAndIndexesMatch::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kFromBit = 0;
    const size_t kToBit = 1;
    const size_t kCollectionOptionsBit = 2;
    const size_t kIndexesBit = 3;
    const size_t kDbNameBit = 4;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kFromFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFromBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFromBit);

                _hasFrom = true;
                _from = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kToBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kToBit);

                _hasTo = true;
                _to = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kCollectionOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollectionOptionsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollectionOptionsBit);

                _hasCollectionOptions = true;
                _collectionOptions = element.Obj();
            }
        }
        else if (fieldName == kIndexesFieldName) {
            if (MONGO_unlikely(usedFields[kIndexesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kIndexesBit);

            _hasIndexes = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kIndexesFieldName, &ctxt);
            std::vector<mongo::BSONObj> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        values.emplace_back(arrayElement.Obj());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _indexes = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kFromBit]) {
            ctxt.throwMissingField(kFromFieldName);
        }
        if (!usedFields[kToBit]) {
            ctxt.throwMissingField(kToFieldName);
        }
        if (!usedFields[kCollectionOptionsBit]) {
            ctxt.throwMissingField(kCollectionOptionsFieldName);
        }
        if (!usedFields[kIndexesBit]) {
            ctxt.throwMissingField(kIndexesFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

InternalRenameIfOptionsAndIndexesMatch InternalRenameIfOptionsAndIndexesMatch::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    InternalRenameIfOptionsAndIndexesMatch object;
    object.parseProtected(ctxt, request);
    return object;
}
void InternalRenameIfOptionsAndIndexesMatch::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<5> usedFields;
    const size_t kFromBit = 0;
    const size_t kToBit = 1;
    const size_t kCollectionOptionsBit = 2;
    const size_t kIndexesBit = 3;
    const size_t kDbNameBit = 4;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kFromFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFromBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFromBit);

                _hasFrom = true;
                _from = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kToBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kToBit);

                _hasTo = true;
                _to = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kCollectionOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollectionOptionsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollectionOptionsBit);

                _hasCollectionOptions = true;
                _collectionOptions = element.Obj();
            }
        }
        else if (fieldName == kIndexesFieldName) {
            if (MONGO_unlikely(usedFields[kIndexesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kIndexesBit);

            _hasIndexes = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kIndexesFieldName, &ctxt);
            std::vector<mongo::BSONObj> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        values.emplace_back(arrayElement.Obj());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _indexes = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDbNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDbNameBit);

                _hasDbName = true;
                _dbName = element.str();
            }
        }
        else {
            if (!mongo::isGenericArgument(fieldName)) {
                ctxt.throwUnknownField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kFromBit]) {
            ctxt.throwMissingField(kFromFieldName);
        }
        if (!usedFields[kToBit]) {
            ctxt.throwMissingField(kToFieldName);
        }
        if (!usedFields[kCollectionOptionsBit]) {
            ctxt.throwMissingField(kCollectionOptionsFieldName);
        }
        if (!usedFields[kIndexesBit]) {
            ctxt.throwMissingField(kIndexesFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void InternalRenameIfOptionsAndIndexesMatch::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasFrom && _hasTo && _hasCollectionOptions && _hasIndexes && _hasDbName);

    builder->append("internalRenameIfOptionsAndIndexesMatch"_sd, 1);
    {
        builder->append(kFromFieldName, _from.toString());
    }

    {
        builder->append(kToFieldName, _to.toString());
    }

    builder->append(kCollectionOptionsFieldName, _collectionOptions);

    {
        builder->append(kIndexesFieldName, _indexes);
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest InternalRenameIfOptionsAndIndexesMatch::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasFrom && _hasTo && _hasCollectionOptions && _hasIndexes && _hasDbName);

        builder->append("internalRenameIfOptionsAndIndexesMatch"_sd, 1);
        {
            builder->append(kFromFieldName, _from.toString());
        }

        {
            builder->append(kToFieldName, _to.toString());
        }

        builder->append(kCollectionOptionsFieldName, _collectionOptions);

        {
            builder->append(kIndexesFieldName, _indexes);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj InternalRenameIfOptionsAndIndexesMatch::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
