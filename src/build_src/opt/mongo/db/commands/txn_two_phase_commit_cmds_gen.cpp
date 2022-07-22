/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/txn_two_phase_commit_cmds_gen.h --output build/opt/mongo/db/commands/txn_two_phase_commit_cmds_gen.cpp src/mongo/db/commands/txn_two_phase_commit_cmds.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/txn_two_phase_commit_cmds_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData CommitParticipant::kShardIdFieldName;


CommitParticipant::CommitParticipant() : _hasShardId(false) {
    // Used for initialization only
}
CommitParticipant::CommitParticipant(mongo::ShardId shardId) : _shardId(std::move(shardId)), _hasShardId(true) {
    // Used for initialization only
}


CommitParticipant CommitParticipant::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CommitParticipant object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CommitParticipant::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kShardIdBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kShardIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kShardIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kShardIdBit);

                _hasShardId = true;
                _shardId = element.str();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kShardIdBit]) {
            ctxt.throwMissingField(kShardIdFieldName);
        }
    }

}


void CommitParticipant::serialize(BSONObjBuilder* builder) const {
    invariant(_hasShardId);

    {
        builder->append(kShardIdFieldName, _shardId.toString());
    }

}


BSONObj CommitParticipant::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData PrepareTransaction::kDbNameFieldName;
constexpr StringData PrepareTransaction::kCommandName;

const std::vector<StringData> PrepareTransaction::_knownBSONFields {
    PrepareTransaction::kCommandName,
};
const std::vector<StringData> PrepareTransaction::_knownOP_MSGFields {
    PrepareTransaction::kDbNameFieldName,
    PrepareTransaction::kCommandName,
};

PrepareTransaction::PrepareTransaction() : _hasDbName(false) {
    // Used for initialization only
}


PrepareTransaction PrepareTransaction::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    PrepareTransaction object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void PrepareTransaction::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
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

PrepareTransaction PrepareTransaction::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    PrepareTransaction object;
    object.parseProtected(ctxt, request);
    return object;
}
void PrepareTransaction::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
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

void PrepareTransaction::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    builder->append("prepareTransaction"_sd, 1);
    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest PrepareTransaction::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        builder->append("prepareTransaction"_sd, 1);
        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj PrepareTransaction::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData CoordinateCommitTransaction::kDbNameFieldName;
constexpr StringData CoordinateCommitTransaction::kParticipantsFieldName;
constexpr StringData CoordinateCommitTransaction::kCommandName;

const std::vector<StringData> CoordinateCommitTransaction::_knownBSONFields {
    CoordinateCommitTransaction::kParticipantsFieldName,
    CoordinateCommitTransaction::kCommandName,
};
const std::vector<StringData> CoordinateCommitTransaction::_knownOP_MSGFields {
    CoordinateCommitTransaction::kDbNameFieldName,
    CoordinateCommitTransaction::kParticipantsFieldName,
    CoordinateCommitTransaction::kCommandName,
};

CoordinateCommitTransaction::CoordinateCommitTransaction() : _hasParticipants(false), _hasDbName(false) {
    // Used for initialization only
}
CoordinateCommitTransaction::CoordinateCommitTransaction(std::vector<mongo::CommitParticipant> participants) : _participants(std::move(participants)), _hasParticipants(true), _hasDbName(true) {
    // Used for initialization only
}


CoordinateCommitTransaction CoordinateCommitTransaction::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CoordinateCommitTransaction object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CoordinateCommitTransaction::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kParticipantsFieldName) {
            _hasParticipants = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kParticipantsFieldName, &ctxt);
            std::vector<mongo::CommitParticipant> values;

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
                        IDLParserErrorContext tempContext(kParticipantsFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::CommitParticipant::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _participants = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kParticipantsFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kParticipantsFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

}

CoordinateCommitTransaction CoordinateCommitTransaction::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    CoordinateCommitTransaction object;
    object.parseProtected(ctxt, request);
    return object;
}
void CoordinateCommitTransaction::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kParticipantsFieldName) {
            _hasParticipants = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kParticipantsFieldName, &ctxt);
            std::vector<mongo::CommitParticipant> values;

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
                        IDLParserErrorContext tempContext(kParticipantsFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::CommitParticipant::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _participants = std::move(values);
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kParticipantsFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kParticipantsFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

}

void CoordinateCommitTransaction::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasParticipants && _hasDbName);

    builder->append("coordinateCommitTransaction"_sd, 1);
    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kParticipantsFieldName));
        for (const auto& item : _participants) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest CoordinateCommitTransaction::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasParticipants && _hasDbName);

        builder->append("coordinateCommitTransaction"_sd, 1);
        {
            BSONArrayBuilder arrayBuilder(builder->subarrayStart(kParticipantsFieldName));
            for (const auto& item : _participants) {
                BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
                item.serialize(&subObjBuilder);
            }
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj CoordinateCommitTransaction::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
