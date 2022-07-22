/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/list_databases_gen.h --output build/opt/mongo/db/commands/list_databases_gen.cpp src/mongo/db/commands/list_databases.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/list_databases_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData ListDatabasesCommand::kAuthorizedDatabasesFieldName;
constexpr StringData ListDatabasesCommand::kDbNameFieldName;
constexpr StringData ListDatabasesCommand::kFilterFieldName;
constexpr StringData ListDatabasesCommand::kNameOnlyFieldName;
constexpr StringData ListDatabasesCommand::kCommandName;

const std::vector<StringData> ListDatabasesCommand::_knownBSONFields {
    ListDatabasesCommand::kAuthorizedDatabasesFieldName,
    ListDatabasesCommand::kFilterFieldName,
    ListDatabasesCommand::kNameOnlyFieldName,
    ListDatabasesCommand::kCommandName,
};
const std::vector<StringData> ListDatabasesCommand::_knownOP_MSGFields {
    ListDatabasesCommand::kAuthorizedDatabasesFieldName,
    ListDatabasesCommand::kDbNameFieldName,
    ListDatabasesCommand::kFilterFieldName,
    ListDatabasesCommand::kNameOnlyFieldName,
    ListDatabasesCommand::kCommandName,
};

ListDatabasesCommand::ListDatabasesCommand() : _hasDbName(false) {
    // Used for initialization only
}


ListDatabasesCommand ListDatabasesCommand::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ListDatabasesCommand object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ListDatabasesCommand::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<4> usedFields;
    const size_t kNameOnlyBit = 0;
    const size_t kAuthorizedDatabasesBit = 1;
    const size_t kFilterBit = 2;
    const size_t kDbNameBit = 3;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kNameOnlyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kNameOnlyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNameOnlyBit);

                _nameOnly = element.trueValue();
            }
        }
        else if (fieldName == kAuthorizedDatabasesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAuthorizedDatabasesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAuthorizedDatabasesBit);

                _authorizedDatabases = element.boolean();
            }
        }
        else if (fieldName == kFilterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kFilterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFilterBit);

                _filter = element.Obj();
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
        if (!usedFields[kNameOnlyBit]) {
            _nameOnly = false;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

ListDatabasesCommand ListDatabasesCommand::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    ListDatabasesCommand object;
    object.parseProtected(ctxt, request);
    return object;
}
void ListDatabasesCommand::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<4> usedFields;
    const size_t kNameOnlyBit = 0;
    const size_t kAuthorizedDatabasesBit = 1;
    const size_t kFilterBit = 2;
    const size_t kDbNameBit = 3;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kNameOnlyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {Bool, NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kNameOnlyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNameOnlyBit);

                _nameOnly = element.trueValue();
            }
        }
        else if (fieldName == kAuthorizedDatabasesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAuthorizedDatabasesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAuthorizedDatabasesBit);

                _authorizedDatabases = element.boolean();
            }
        }
        else if (fieldName == kFilterFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kFilterBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFilterBit);

                _filter = element.Obj();
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
        if (!usedFields[kNameOnlyBit]) {
            _nameOnly = false;
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void ListDatabasesCommand::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append("listDatabasesCommand"_sd, 1);
    builder->append(kNameOnlyFieldName, _nameOnly);

    if (_authorizedDatabases.is_initialized()) {
        builder->append(kAuthorizedDatabasesFieldName, _authorizedDatabases.get());
    }

    if (_filter.is_initialized()) {
        builder->append(kFilterFieldName, _filter.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ListDatabasesCommand::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append("listDatabasesCommand"_sd, 1);
        builder->append(kNameOnlyFieldName, _nameOnly);

        if (_authorizedDatabases.is_initialized()) {
            builder->append(kAuthorizedDatabasesFieldName, _authorizedDatabases.get());
        }

        if (_filter.is_initialized()) {
            builder->append(kFilterFieldName, _filter.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ListDatabasesCommand::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
