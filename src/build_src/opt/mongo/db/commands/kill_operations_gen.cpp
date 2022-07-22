/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/kill_operations_gen.h --output build/opt/mongo/db/commands/kill_operations_gen.cpp src/mongo/db/commands/kill_operations.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/kill_operations_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData KillOperationsRequest::kDbNameFieldName;
constexpr StringData KillOperationsRequest::kOperationKeysFieldName;
constexpr StringData KillOperationsRequest::kCommandName;

const std::vector<StringData> KillOperationsRequest::_knownBSONFields {
    KillOperationsRequest::kOperationKeysFieldName,
    KillOperationsRequest::kCommandName,
};
const std::vector<StringData> KillOperationsRequest::_knownOP_MSGFields {
    KillOperationsRequest::kDbNameFieldName,
    KillOperationsRequest::kOperationKeysFieldName,
    KillOperationsRequest::kCommandName,
};

KillOperationsRequest::KillOperationsRequest() : _hasOperationKeys(false), _hasDbName(false) {
    // Used for initialization only
}
KillOperationsRequest::KillOperationsRequest(std::vector<mongo::UUID> operationKeys) : _operationKeys(std::move(operationKeys)), _hasOperationKeys(true), _hasDbName(true) {
    // Used for initialization only
}


KillOperationsRequest KillOperationsRequest::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    KillOperationsRequest object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void KillOperationsRequest::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kOperationKeysBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kOperationKeysFieldName) {
            if (MONGO_unlikely(usedFields[kOperationKeysBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kOperationKeysBit);

            _hasOperationKeys = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kOperationKeysFieldName, &ctxt);
            std::vector<mongo::UUID> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertBinDataType(arrayElement, newUUID)) {
                        values.emplace_back(UUID(uassertStatusOK(UUID::parse(arrayElement))));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _operationKeys = std::move(values);
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
        if (!usedFields[kOperationKeysBit]) {
            ctxt.throwMissingField(kOperationKeysFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

KillOperationsRequest KillOperationsRequest::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    KillOperationsRequest object;
    object.parseProtected(ctxt, request);
    return object;
}
void KillOperationsRequest::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<2> usedFields;
    const size_t kOperationKeysBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kOperationKeysFieldName) {
            if (MONGO_unlikely(usedFields[kOperationKeysBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kOperationKeysBit);

            _hasOperationKeys = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kOperationKeysFieldName, &ctxt);
            std::vector<mongo::UUID> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertBinDataType(arrayElement, newUUID)) {
                        values.emplace_back(UUID(uassertStatusOK(UUID::parse(arrayElement))));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _operationKeys = std::move(values);
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
        if (!usedFields[kOperationKeysBit]) {
            ctxt.throwMissingField(kOperationKeysFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void KillOperationsRequest::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasOperationKeys && _hasDbName);

    builder->append("_killOperations"_sd, 1);
    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kOperationKeysFieldName));
        for (const auto& item : _operationKeys) {
            ConstDataRange tempCDR = item.toCDR();
            arrayBuilder.append(BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
        }
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest KillOperationsRequest::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasOperationKeys && _hasDbName);

        builder->append("_killOperations"_sd, 1);
        {
            BSONArrayBuilder arrayBuilder(builder->subarrayStart(kOperationKeysFieldName));
            for (const auto& item : _operationKeys) {
                ConstDataRange tempCDR = item.toCDR();
                arrayBuilder.append(BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
            }
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj KillOperationsRequest::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
