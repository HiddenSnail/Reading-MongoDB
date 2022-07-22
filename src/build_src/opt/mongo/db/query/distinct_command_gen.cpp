/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/distinct_command_gen.h --output build/opt/mongo/db/query/distinct_command_gen.cpp src/mongo/db/query/distinct_command.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/query/distinct_command_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData DistinctCommand::kCollationFieldName;
constexpr StringData DistinctCommand::kCommentFieldName;
constexpr StringData DistinctCommand::kDbNameFieldName;
constexpr StringData DistinctCommand::kKeyFieldName;
constexpr StringData DistinctCommand::kQueryFieldName;
constexpr StringData DistinctCommand::kCommandName;

const std::vector<StringData> DistinctCommand::_knownBSONFields {
    DistinctCommand::kCollationFieldName,
    DistinctCommand::kCommentFieldName,
    DistinctCommand::kKeyFieldName,
    DistinctCommand::kQueryFieldName,
    DistinctCommand::kCommandName,
};
const std::vector<StringData> DistinctCommand::_knownOP_MSGFields {
    DistinctCommand::kCollationFieldName,
    DistinctCommand::kCommentFieldName,
    DistinctCommand::kDbNameFieldName,
    DistinctCommand::kKeyFieldName,
    DistinctCommand::kQueryFieldName,
    DistinctCommand::kCommandName,
};

DistinctCommand::DistinctCommand(const NamespaceStringOrUUID nssOrUUID) : _nssOrUUID(std::move(nssOrUUID)), _dbName(nssOrUUID.uuid() ? nssOrUUID.dbname() : nssOrUUID.nss().get().db().toString()), _hasKey(false), _hasDbName(true) {
    // Used for initialization only
}
DistinctCommand::DistinctCommand(const NamespaceStringOrUUID nssOrUUID, std::string key) : _nssOrUUID(std::move(nssOrUUID)), _key(std::move(key)), _dbName(nssOrUUID.uuid() ? nssOrUUID.dbname() : nssOrUUID.nss().get().db().toString()), _hasKey(true), _hasDbName(true) {
    // Used for initialization only
}


DistinctCommand DistinctCommand::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    DistinctCommand object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DistinctCommand::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kKeyBit = 0;
    const size_t kQueryBit = 1;
    const size_t kCollationBit = 2;
    const size_t kCommentBit = 3;
    const size_t kDbNameBit = 4;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyBit);

                _hasKey = true;
                _key = element.str();
            }
        }
        else if (fieldName == kQueryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryBit);

                _query = element.Obj();
            }
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollationBit);

                _collation = element.Obj();
            }
        }
        else if (fieldName == kCommentFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kCommentBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommentBit);

                _comment = element.str();
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
        if (!usedFields[kKeyBit]) {
            ctxt.throwMissingField(kKeyFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
    _nssOrUUID = ctxt.parseNsOrUUID(_dbName, commandElement);
}

DistinctCommand DistinctCommand::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    DistinctCommand object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void DistinctCommand::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<5> usedFields;
    const size_t kKeyBit = 0;
    const size_t kQueryBit = 1;
    const size_t kCollationBit = 2;
    const size_t kCommentBit = 3;
    const size_t kDbNameBit = 4;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyBit);

                _hasKey = true;
                _key = element.str();
            }
        }
        else if (fieldName == kQueryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryBit);

                _query = element.Obj();
            }
        }
        else if (fieldName == kCollationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCollationBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollationBit);

                _collation = element.Obj();
            }
        }
        else if (fieldName == kCommentFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kCommentBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommentBit);

                _comment = element.str();
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
        if (!usedFields[kKeyBit]) {
            ctxt.throwMissingField(kKeyFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
    _nssOrUUID = ctxt.parseNsOrUUID(_dbName, commandElement);
}

void DistinctCommand::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasKey && _hasDbName);

    invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
    if( _nssOrUUID.uuid() ) {
        _nssOrUUID.uuid().get().appendToBuilder(builder, "distinct"_sd);
    }
    else {
        builder->append("distinct"_sd, _nssOrUUID.nss().get().coll());
    }

    builder->append(kKeyFieldName, _key);

    if (_query.is_initialized()) {
        builder->append(kQueryFieldName, _query.get());
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

    if (_comment.is_initialized()) {
        builder->append(kCommentFieldName, _comment.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest DistinctCommand::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasKey && _hasDbName);

        invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
        if( _nssOrUUID.uuid() ) {
            _nssOrUUID.uuid().get().appendToBuilder(builder, "distinct"_sd);
        }
        else {
            builder->append("distinct"_sd, _nssOrUUID.nss().get().coll());
        }

        builder->append(kKeyFieldName, _key);

        if (_query.is_initialized()) {
            builder->append(kQueryFieldName, _query.get());
        }

        if (_collation.is_initialized()) {
            builder->append(kCollationFieldName, _collation.get());
        }

        if (_comment.is_initialized()) {
            builder->append(kCommentFieldName, _comment.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj DistinctCommand::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
