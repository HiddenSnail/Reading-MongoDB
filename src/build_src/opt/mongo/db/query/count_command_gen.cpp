/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/count_command_gen.h --output build/opt/mongo/db/query/count_command_gen.cpp src/mongo/db/query/count_command.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/query/count_command_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData CountCommand::kCollationFieldName;
constexpr StringData CountCommand::kCommentFieldName;
constexpr StringData CountCommand::kDbNameFieldName;
constexpr StringData CountCommand::kFieldsFieldName;
constexpr StringData CountCommand::kHintFieldName;
constexpr StringData CountCommand::kLimitFieldName;
constexpr StringData CountCommand::kMaxTimeMSFieldName;
constexpr StringData CountCommand::kQueryFieldName;
constexpr StringData CountCommand::kQueryOptionsFieldName;
constexpr StringData CountCommand::kReadConcernFieldName;
constexpr StringData CountCommand::kSkipFieldName;
constexpr StringData CountCommand::kCommandName;

const std::vector<StringData> CountCommand::_knownBSONFields {
    CountCommand::kCollationFieldName,
    CountCommand::kCommentFieldName,
    CountCommand::kFieldsFieldName,
    CountCommand::kHintFieldName,
    CountCommand::kLimitFieldName,
    CountCommand::kMaxTimeMSFieldName,
    CountCommand::kQueryFieldName,
    CountCommand::kQueryOptionsFieldName,
    CountCommand::kReadConcernFieldName,
    CountCommand::kSkipFieldName,
    CountCommand::kCommandName,
};
const std::vector<StringData> CountCommand::_knownOP_MSGFields {
    CountCommand::kCollationFieldName,
    CountCommand::kCommentFieldName,
    CountCommand::kDbNameFieldName,
    CountCommand::kFieldsFieldName,
    CountCommand::kHintFieldName,
    CountCommand::kLimitFieldName,
    CountCommand::kMaxTimeMSFieldName,
    CountCommand::kQueryFieldName,
    CountCommand::kQueryOptionsFieldName,
    CountCommand::kReadConcernFieldName,
    CountCommand::kSkipFieldName,
    CountCommand::kCommandName,
};

CountCommand::CountCommand(const NamespaceStringOrUUID nssOrUUID) : _nssOrUUID(std::move(nssOrUUID)), _dbName(nssOrUUID.uuid() ? nssOrUUID.dbname() : nssOrUUID.nss().get().db().toString()), _hasDbName(true) {
    // Used for initialization only
}


CountCommand CountCommand::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    NamespaceString localNS;
    CountCommand object(localNS);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CountCommand::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<11> usedFields;
    const size_t kQueryBit = 0;
    const size_t kLimitBit = 1;
    const size_t kSkipBit = 2;
    const size_t kHintBit = 3;
    const size_t kCollationBit = 4;
    const size_t kCommentBit = 5;
    const size_t kFieldsBit = 6;
    const size_t kReadConcernBit = 7;
    const size_t kMaxTimeMSBit = 8;
    const size_t kQueryOptionsBit = 9;
    const size_t kDbNameBit = 10;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kQueryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryBit);

                _query = element.Obj();
            }
        }
        else if (fieldName == kLimitFieldName) {
            if (MONGO_unlikely(usedFields[kLimitBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kLimitBit);

            _limit = count_request::countParseLimit(element);
        }
        else if (fieldName == kSkipFieldName) {
            if (MONGO_unlikely(usedFields[kSkipBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kSkipBit);

            _skip = count_request::countParseSkip(element);
        }
        else if (fieldName == kHintFieldName) {
            if (MONGO_unlikely(usedFields[kHintBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kHintBit);

            _hint = parseHint(element);
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
        else if (fieldName == kFieldsFieldName) {
            if (MONGO_unlikely(usedFields[kFieldsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kFieldsBit);

            // ignore field
        }
        else if (fieldName == kReadConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kReadConcernBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kReadConcernBit);

                _readConcern = element.Obj();
            }
        }
        else if (fieldName == kMaxTimeMSFieldName) {
            if (MONGO_unlikely(usedFields[kMaxTimeMSBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMaxTimeMSBit);

            _maxTimeMS = count_request::countParseMaxTime(element);
        }
        else if (fieldName == kQueryOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryOptionsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryOptionsBit);

                _queryOptions = element.Obj();
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
        if (!usedFields[kQueryBit]) {
            _query = BSONObj();
        }
        if (!usedFields[kHintBit]) {
            _hint = BSONObj();
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
    _nssOrUUID = ctxt.parseNsOrUUID(_dbName, commandElement);
}

CountCommand CountCommand::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    NamespaceString localNS;
    CountCommand object(localNS);
    object.parseProtected(ctxt, request);
    return object;
}
void CountCommand::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<11> usedFields;
    const size_t kQueryBit = 0;
    const size_t kLimitBit = 1;
    const size_t kSkipBit = 2;
    const size_t kHintBit = 3;
    const size_t kCollationBit = 4;
    const size_t kCommentBit = 5;
    const size_t kFieldsBit = 6;
    const size_t kReadConcernBit = 7;
    const size_t kMaxTimeMSBit = 8;
    const size_t kQueryOptionsBit = 9;
    const size_t kDbNameBit = 10;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kQueryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryBit);

                _query = element.Obj();
            }
        }
        else if (fieldName == kLimitFieldName) {
            if (MONGO_unlikely(usedFields[kLimitBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kLimitBit);

            _limit = count_request::countParseLimit(element);
        }
        else if (fieldName == kSkipFieldName) {
            if (MONGO_unlikely(usedFields[kSkipBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kSkipBit);

            _skip = count_request::countParseSkip(element);
        }
        else if (fieldName == kHintFieldName) {
            if (MONGO_unlikely(usedFields[kHintBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kHintBit);

            _hint = parseHint(element);
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
        else if (fieldName == kFieldsFieldName) {
            if (MONGO_unlikely(usedFields[kFieldsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kFieldsBit);

            // ignore field
        }
        else if (fieldName == kReadConcernFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kReadConcernBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kReadConcernBit);

                _readConcern = element.Obj();
            }
        }
        else if (fieldName == kMaxTimeMSFieldName) {
            if (MONGO_unlikely(usedFields[kMaxTimeMSBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kMaxTimeMSBit);

            _maxTimeMS = count_request::countParseMaxTime(element);
        }
        else if (fieldName == kQueryOptionsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kQueryOptionsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kQueryOptionsBit);

                _queryOptions = element.Obj();
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
        if (!usedFields[kQueryBit]) {
            _query = BSONObj();
        }
        if (!usedFields[kHintBit]) {
            _hint = BSONObj();
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

    invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
    _nssOrUUID = ctxt.parseNsOrUUID(_dbName, commandElement);
}

void CountCommand::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
    if( _nssOrUUID.uuid() ) {
        _nssOrUUID.uuid().get().appendToBuilder(builder, "count"_sd);
    }
    else {
        builder->append("count"_sd, _nssOrUUID.nss().get().coll());
    }

    builder->append(kQueryFieldName, _query);

    if (_limit.is_initialized()) {
        builder->append(kLimitFieldName, _limit.get());
    }

    if (_skip.is_initialized()) {
        builder->append(kSkipFieldName, _skip.get());
    }

    {
        serializeHintToBSON(_hint, kHintFieldName, builder);
    }

    if (_collation.is_initialized()) {
        builder->append(kCollationFieldName, _collation.get());
    }

    if (_comment.is_initialized()) {
        builder->append(kCommentFieldName, _comment.get());
    }

    if (_readConcern.is_initialized()) {
        builder->append(kReadConcernFieldName, _readConcern.get());
    }

    if (_maxTimeMS.is_initialized()) {
        builder->append(kMaxTimeMSFieldName, _maxTimeMS.get());
    }

    if (_queryOptions.is_initialized()) {
        builder->append(kQueryOptionsFieldName, _queryOptions.get());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest CountCommand::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        invariant(_nssOrUUID.nss() || _nssOrUUID.uuid());
        if( _nssOrUUID.uuid() ) {
            _nssOrUUID.uuid().get().appendToBuilder(builder, "count"_sd);
        }
        else {
            builder->append("count"_sd, _nssOrUUID.nss().get().coll());
        }

        builder->append(kQueryFieldName, _query);

        if (_limit.is_initialized()) {
            builder->append(kLimitFieldName, _limit.get());
        }

        if (_skip.is_initialized()) {
            builder->append(kSkipFieldName, _skip.get());
        }

        {
            serializeHintToBSON(_hint, kHintFieldName, builder);
        }

        if (_collation.is_initialized()) {
            builder->append(kCollationFieldName, _collation.get());
        }

        if (_comment.is_initialized()) {
            builder->append(kCommentFieldName, _comment.get());
        }

        if (_readConcern.is_initialized()) {
            builder->append(kReadConcernFieldName, _readConcern.get());
        }

        if (_maxTimeMS.is_initialized()) {
            builder->append(kMaxTimeMSFieldName, _maxTimeMS.get());
        }

        if (_queryOptions.is_initialized()) {
            builder->append(kQueryOptionsFieldName, _queryOptions.get());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj CountCommand::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
