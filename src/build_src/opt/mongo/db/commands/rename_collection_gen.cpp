/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/rename_collection_gen.h --output build/opt/mongo/db/commands/rename_collection_gen.cpp src/mongo/db/commands/rename_collection.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/rename_collection_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData RenameCollectionCommand::kCommandParameterFieldName;
constexpr StringData RenameCollectionCommand::kDbNameFieldName;
constexpr StringData RenameCollectionCommand::kDropTargetFieldName;
constexpr StringData RenameCollectionCommand::kStayTempFieldName;
constexpr StringData RenameCollectionCommand::kToFieldName;
constexpr StringData RenameCollectionCommand::kCommandName;

const std::vector<StringData> RenameCollectionCommand::_knownBSONFields {
    RenameCollectionCommand::kDropTargetFieldName,
    RenameCollectionCommand::kStayTempFieldName,
    RenameCollectionCommand::kToFieldName,
    RenameCollectionCommand::kCommandName,
};
const std::vector<StringData> RenameCollectionCommand::_knownOP_MSGFields {
    RenameCollectionCommand::kDbNameFieldName,
    RenameCollectionCommand::kDropTargetFieldName,
    RenameCollectionCommand::kStayTempFieldName,
    RenameCollectionCommand::kToFieldName,
    RenameCollectionCommand::kCommandName,
};

RenameCollectionCommand::RenameCollectionCommand(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasTo(false), _hasDbName(false) {
    // Used for initialization only
}
RenameCollectionCommand::RenameCollectionCommand(const mongo::NamespaceString commandParameter, mongo::NamespaceString to) : _commandParameter(std::move(commandParameter)), _to(std::move(to)), _hasTo(true), _hasDbName(true) {
    // Used for initialization only
}


RenameCollectionCommand RenameCollectionCommand::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    RenameCollectionCommand object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RenameCollectionCommand::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kToBit = 0;
    const size_t kDropTargetBit = 1;
    const size_t kStayTempBit = 2;
    const size_t kDbNameBit = 3;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kToBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kToBit);

                _hasTo = true;
                _to = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kDropTargetFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kDropTargetBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDropTargetBit);

                _dropTarget = element.boolean();
            }
        }
        else if (fieldName == kStayTempFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kStayTempBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStayTempBit);

                _stayTemp = element.boolean();
            }
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
        if (!usedFields[kToBit]) {
            ctxt.throwMissingField(kToFieldName);
        }
        if (!usedFields[kDropTargetBit]) {
            _dropTarget = false;
        }
        if (!usedFields[kStayTempBit]) {
            _stayTemp = false;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

RenameCollectionCommand RenameCollectionCommand::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    RenameCollectionCommand object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void RenameCollectionCommand::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<4> usedFields;
    const size_t kToBit = 0;
    const size_t kDropTargetBit = 1;
    const size_t kStayTempBit = 2;
    const size_t kDbNameBit = 3;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kToBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kToBit);

                _hasTo = true;
                _to = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kDropTargetFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kDropTargetBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDropTargetBit);

                _dropTarget = element.boolean();
            }
        }
        else if (fieldName == kStayTempFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kStayTempBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStayTempBit);

                _stayTemp = element.boolean();
            }
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
        if (!usedFields[kToBit]) {
            ctxt.throwMissingField(kToFieldName);
        }
        if (!usedFields[kDropTargetBit]) {
            _dropTarget = false;
        }
        if (!usedFields[kStayTempBit]) {
            _stayTemp = false;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

void RenameCollectionCommand::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasTo && _hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    {
        builder->append(kToFieldName, _to.toString());
    }

    builder->append(kDropTargetFieldName, _dropTarget);

    builder->append(kStayTempFieldName, _stayTemp);

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest RenameCollectionCommand::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasTo && _hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        {
            builder->append(kToFieldName, _to.toString());
        }

        builder->append(kDropTargetFieldName, _dropTarget);

        builder->append(kStayTempFieldName, _stayTemp);

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj RenameCollectionCommand::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
