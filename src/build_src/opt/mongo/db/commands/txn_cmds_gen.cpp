/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/txn_cmds_gen.h --output build/opt/mongo/db/commands/txn_cmds_gen.cpp src/mongo/db/commands/txn_cmds.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/txn_cmds_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData TxnResponseMetadata::kReadOnlyFieldName;


TxnResponseMetadata::TxnResponseMetadata() : _readOnly(false), _hasReadOnly(false) {
    // Used for initialization only
}
TxnResponseMetadata::TxnResponseMetadata(bool readOnly) : _readOnly(std::move(readOnly)), _hasReadOnly(true) {
    // Used for initialization only
}


TxnResponseMetadata TxnResponseMetadata::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TxnResponseMetadata object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TxnResponseMetadata::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kReadOnlyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _hasReadOnly = true;
                _readOnly = element.boolean();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kReadOnlyFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kReadOnlyFieldName);
    }

}


void TxnResponseMetadata::serialize(BSONObjBuilder* builder) const {
    invariant(_hasReadOnly);

    builder->append(kReadOnlyFieldName, _readOnly);

}


BSONObj TxnResponseMetadata::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData TxnRecoveryToken::kRecoveryShardIdFieldName;


TxnRecoveryToken::TxnRecoveryToken()  {
    // Used for initialization only
}


TxnRecoveryToken TxnRecoveryToken::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TxnRecoveryToken object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TxnRecoveryToken::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kRecoveryShardIdBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kRecoveryShardIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kRecoveryShardIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecoveryShardIdBit);

                _recoveryShardId = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void TxnRecoveryToken::serialize(BSONObjBuilder* builder) const {
    if (_recoveryShardId.is_initialized()) {
        builder->append(kRecoveryShardIdFieldName, _recoveryShardId.get().toString());
    }

}


BSONObj TxnRecoveryToken::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData CommitTransactionOplogObject::kCommitTimestampFieldName;
constexpr StringData CommitTransactionOplogObject::kCommitTransactionFieldName;
constexpr StringData CommitTransactionOplogObject::kCountFieldName;


CommitTransactionOplogObject::CommitTransactionOplogObject()  {
    // Used for initialization only
}


CommitTransactionOplogObject CommitTransactionOplogObject::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CommitTransactionOplogObject object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CommitTransactionOplogObject::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kCommitTransactionBit = 0;
    const size_t kCommitTimestampBit = 1;
    const size_t kCountBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kCommitTransactionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kCommitTransactionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommitTransactionBit);

                _commitTransaction = element._numberInt();
            }
        }
        else if (fieldName == kCommitTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kCommitTimestampBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommitTimestampBit);

                _commitTimestamp = element.timestamp();
            }
        }
        else if (fieldName == kCountFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kCountBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCountBit);

                _count = element._numberLong();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kCommitTransactionBit]) {
            _commitTransaction = 1;
        }
    }

}


void CommitTransactionOplogObject::serialize(BSONObjBuilder* builder) const {
    builder->append(kCommitTransactionFieldName, _commitTransaction);

    if (_commitTimestamp.is_initialized()) {
        builder->append(kCommitTimestampFieldName, _commitTimestamp.get());
    }

    if (_count.is_initialized()) {
        builder->append(kCountFieldName, _count.get());
    }

}


BSONObj CommitTransactionOplogObject::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AbortTransactionOplogObject::kAbortTransactionFieldName;


AbortTransactionOplogObject::AbortTransactionOplogObject()  {
    // Used for initialization only
}


AbortTransactionOplogObject AbortTransactionOplogObject::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AbortTransactionOplogObject object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AbortTransactionOplogObject::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kAbortTransactionBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kAbortTransactionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kAbortTransactionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAbortTransactionBit);

                _abortTransaction = element._numberInt();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kAbortTransactionBit]) {
            _abortTransaction = 1;
        }
    }

}


void AbortTransactionOplogObject::serialize(BSONObjBuilder* builder) const {
    builder->append(kAbortTransactionFieldName, _abortTransaction);

}


BSONObj AbortTransactionOplogObject::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData CommitTransaction::kCommitTimestampFieldName;
constexpr StringData CommitTransaction::kDbNameFieldName;
constexpr StringData CommitTransaction::kRecoveryTokenFieldName;
constexpr StringData CommitTransaction::kCommandName;

const std::vector<StringData> CommitTransaction::_knownBSONFields {
    CommitTransaction::kCommitTimestampFieldName,
    CommitTransaction::kRecoveryTokenFieldName,
    CommitTransaction::kCommandName,
};
const std::vector<StringData> CommitTransaction::_knownOP_MSGFields {
    CommitTransaction::kCommitTimestampFieldName,
    CommitTransaction::kDbNameFieldName,
    CommitTransaction::kRecoveryTokenFieldName,
    CommitTransaction::kCommandName,
};

CommitTransaction::CommitTransaction() : _hasDbName(false) {
    // Used for initialization only
}


CommitTransaction CommitTransaction::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CommitTransaction object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CommitTransaction::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kCommitTimestampBit = 0;
    const size_t kRecoveryTokenBit = 1;
    const size_t kDbNameBit = 2;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kCommitTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kCommitTimestampBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommitTimestampBit);

                _commitTimestamp = element.timestamp();
            }
        }
        else if (fieldName == kRecoveryTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kRecoveryTokenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecoveryTokenBit);

                IDLParserErrorContext tempContext(kRecoveryTokenFieldName, &ctxt);
                const auto localObject = element.Obj();
                _recoveryToken = mongo::TxnRecoveryToken::parse(tempContext, localObject);
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
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

CommitTransaction CommitTransaction::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    CommitTransaction object;
    object.parseProtected(ctxt, request);
    return object;
}
void CommitTransaction::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<3> usedFields;
    const size_t kCommitTimestampBit = 0;
    const size_t kRecoveryTokenBit = 1;
    const size_t kDbNameBit = 2;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kCommitTimestampFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, bsonTimestamp))) {
                if (MONGO_unlikely(usedFields[kCommitTimestampBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCommitTimestampBit);

                _commitTimestamp = element.timestamp();
            }
        }
        else if (fieldName == kRecoveryTokenFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kRecoveryTokenBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecoveryTokenBit);

                IDLParserErrorContext tempContext(kRecoveryTokenFieldName, &ctxt);
                const auto localObject = element.Obj();
                _recoveryToken = mongo::TxnRecoveryToken::parse(tempContext, localObject);
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
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void CommitTransaction::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append("commitTransaction"_sd, 1);
    if (_commitTimestamp.is_initialized()) {
        builder->append(kCommitTimestampFieldName, _commitTimestamp.get());
    }

    if (_recoveryToken.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kRecoveryTokenFieldName));
        _recoveryToken.get().serialize(&subObjBuilder);
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest CommitTransaction::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append("commitTransaction"_sd, 1);
        if (_commitTimestamp.is_initialized()) {
            builder->append(kCommitTimestampFieldName, _commitTimestamp.get());
        }

        if (_recoveryToken.is_initialized()) {
            BSONObjBuilder subObjBuilder(builder->subobjStart(kRecoveryTokenFieldName));
            _recoveryToken.get().serialize(&subObjBuilder);
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj CommitTransaction::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData AbortTransaction::kDbNameFieldName;
constexpr StringData AbortTransaction::kCommandName;

const std::vector<StringData> AbortTransaction::_knownBSONFields {
    AbortTransaction::kCommandName,
};
const std::vector<StringData> AbortTransaction::_knownOP_MSGFields {
    AbortTransaction::kDbNameFieldName,
    AbortTransaction::kCommandName,
};

AbortTransaction::AbortTransaction() : _hasDbName(false) {
    // Used for initialization only
}


AbortTransaction AbortTransaction::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AbortTransaction object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AbortTransaction::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kDbNameFieldName) {
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
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

AbortTransaction AbortTransaction::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    AbortTransaction object;
    object.parseProtected(ctxt, request);
    return object;
}
void AbortTransaction::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kDbNameFieldName) {
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
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void AbortTransaction::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append("abortTransaction"_sd, 1);
    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest AbortTransaction::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append("abortTransaction"_sd, 1);
        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj AbortTransaction::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
