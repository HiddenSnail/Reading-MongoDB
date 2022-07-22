/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/transaction_coordinator_document_gen.h --output build/opt/mongo/db/s/transaction_coordinator_document_gen.cpp src/mongo/db/s/transaction_coordinator_document.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/s/transaction_coordinator_document_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {
namespace txn {

constexpr StringData CoordinatorCommitDecision::kAbortStatusFieldName;
constexpr StringData CoordinatorCommitDecision::kCommitTimestampFieldName;
constexpr StringData CoordinatorCommitDecision::kDecisionFieldName;


CoordinatorCommitDecision::CoordinatorCommitDecision() : _hasDecision(false) {
    // Used for initialization only
}
CoordinatorCommitDecision::CoordinatorCommitDecision(txn::CommitDecision decision) : _decision(std::move(decision)), _hasDecision(true) {
    // Used for initialization only
}


CoordinatorCommitDecision CoordinatorCommitDecision::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    CoordinatorCommitDecision object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void CoordinatorCommitDecision::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kDecisionBit = 0;
    const size_t kCommitTimestampBit = 1;
    const size_t kAbortStatusBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kDecisionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDecisionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDecisionBit);

                _hasDecision = true;
                _decision = txn::readCommitDecisionEnumProperty(element.valueStringData());
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
        else if (fieldName == kAbortStatusFieldName) {
            if (MONGO_unlikely(usedFields[kAbortStatusBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kAbortStatusBit);

            _abortStatus = txn::readStatusProperty(element);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kDecisionBit]) {
            ctxt.throwMissingField(kDecisionFieldName);
        }
    }

}


void CoordinatorCommitDecision::serialize(BSONObjBuilder* builder) const {
    invariant(_hasDecision);

    {
        builder->append(kDecisionFieldName, writeCommitDecisionEnumProperty(_decision));
    }

    if (_commitTimestamp.is_initialized()) {
        builder->append(kCommitTimestampFieldName, _commitTimestamp.get());
    }

    if (_abortStatus.is_initialized()) {
        writeStatusProperty(_abortStatus.get(), kAbortStatusFieldName, builder);
    }

}


BSONObj CoordinatorCommitDecision::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData TransactionCoordinatorDocument::kDecisionFieldName;
constexpr StringData TransactionCoordinatorDocument::kIdFieldName;
constexpr StringData TransactionCoordinatorDocument::kParticipantsFieldName;


TransactionCoordinatorDocument::TransactionCoordinatorDocument() : _hasId(false), _hasParticipants(false) {
    // Used for initialization only
}
TransactionCoordinatorDocument::TransactionCoordinatorDocument(mongo::OperationSessionInfo id, std::vector<mongo::ShardId> participants) : _id(std::move(id)), _participants(std::move(participants)), _hasId(true), _hasParticipants(true) {
    // Used for initialization only
}


TransactionCoordinatorDocument TransactionCoordinatorDocument::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    TransactionCoordinatorDocument object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void TransactionCoordinatorDocument::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kIdBit = 0;
    const size_t kParticipantsBit = 1;
    const size_t kDecisionBit = 2;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIdBit);

                _hasId = true;
                IDLParserErrorContext tempContext(kIdFieldName, &ctxt);
                const auto localObject = element.Obj();
                _id = mongo::OperationSessionInfo::parse(tempContext, localObject);
            }
        }
        else if (fieldName == kParticipantsFieldName) {
            if (MONGO_unlikely(usedFields[kParticipantsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kParticipantsBit);

            _hasParticipants = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kParticipantsFieldName, &ctxt);
            std::vector<mongo::ShardId> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, String)) {
                        values.emplace_back(arrayElement.str());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _participants = std::move(values);
        }
        else if (fieldName == kDecisionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kDecisionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDecisionBit);

                IDLParserErrorContext tempContext(kDecisionFieldName, &ctxt);
                const auto localObject = element.Obj();
                _decision = mongo::txn::CoordinatorCommitDecision::parse(tempContext, localObject);
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kIdBit]) {
            ctxt.throwMissingField(kIdFieldName);
        }
        if (!usedFields[kParticipantsBit]) {
            ctxt.throwMissingField(kParticipantsFieldName);
        }
    }

}


void TransactionCoordinatorDocument::serialize(BSONObjBuilder* builder) const {
    invariant(_hasId && _hasParticipants);

    {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kIdFieldName));
        _id.serialize(&subObjBuilder);
    }

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kParticipantsFieldName));
        for (const auto& item : _participants) {
            arrayBuilder.append(item.toString());
        }
    }

    if (_decision.is_initialized()) {
        BSONObjBuilder subObjBuilder(builder->subobjStart(kDecisionFieldName));
        _decision.get().serialize(&subObjBuilder);
    }

}


BSONObj TransactionCoordinatorDocument::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace txn
}  // namespace mongo
