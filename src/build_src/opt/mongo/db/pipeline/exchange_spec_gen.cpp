/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/exchange_spec_gen.h --output build/opt/mongo/db/pipeline/exchange_spec_gen.cpp src/mongo/db/pipeline/exchange_spec.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/pipeline/exchange_spec_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * The type of an exchange distribution policy
 */
namespace  {
constexpr StringData kExchangePolicy_kBroadcast = "broadcast"_sd;
constexpr StringData kExchangePolicy_kRoundRobin = "roundrobin"_sd;
constexpr StringData kExchangePolicy_kKeyRange = "keyRange"_sd;
}  // namespace 

ExchangePolicyEnum ExchangePolicy_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kExchangePolicy_kBroadcast) {
        return ExchangePolicyEnum::kBroadcast;
    }
    if (value == kExchangePolicy_kRoundRobin) {
        return ExchangePolicyEnum::kRoundRobin;
    }
    if (value == kExchangePolicy_kKeyRange) {
        return ExchangePolicyEnum::kKeyRange;
    }
    ctxt.throwBadEnumValue(value);
}

StringData ExchangePolicy_serializer(ExchangePolicyEnum value) {
    if (value == ExchangePolicyEnum::kBroadcast) {
        return kExchangePolicy_kBroadcast;
    }
    if (value == ExchangePolicyEnum::kRoundRobin) {
        return kExchangePolicy_kRoundRobin;
    }
    if (value == ExchangePolicyEnum::kKeyRange) {
        return kExchangePolicy_kKeyRange;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData ExchangeSpec::kBoundariesFieldName;
constexpr StringData ExchangeSpec::kBufferSizeFieldName;
constexpr StringData ExchangeSpec::kConsumerIdsFieldName;
constexpr StringData ExchangeSpec::kConsumersFieldName;
constexpr StringData ExchangeSpec::kKeyFieldName;
constexpr StringData ExchangeSpec::kOrderPreservingFieldName;
constexpr StringData ExchangeSpec::kPolicyFieldName;


ExchangeSpec::ExchangeSpec() : _consumers(-1), _hasPolicy(false), _hasConsumers(false) {
    // Used for initialization only
}
ExchangeSpec::ExchangeSpec(mongo::ExchangePolicyEnum policy, std::int32_t consumers) : _policy(std::move(policy)), _consumers(std::move(consumers)), _hasPolicy(true), _hasConsumers(true) {
    // Used for initialization only
}


ExchangeSpec ExchangeSpec::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ExchangeSpec object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ExchangeSpec::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<7> usedFields;
    const size_t kPolicyBit = 0;
    const size_t kConsumersBit = 1;
    const size_t kOrderPreservingBit = 2;
    const size_t kBufferSizeBit = 3;
    const size_t kKeyBit = 4;
    const size_t kBoundariesBit = 5;
    const size_t kConsumerIdsBit = 6;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kPolicyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kPolicyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPolicyBit);

                _hasPolicy = true;
                IDLParserErrorContext tempContext(kPolicyFieldName, &ctxt);
                _policy = ExchangePolicy_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kConsumersFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kConsumersBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kConsumersBit);

                _hasConsumers = true;
                _consumers = element._numberInt();
            }
        }
        else if (fieldName == kOrderPreservingFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kOrderPreservingBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kOrderPreservingBit);

                _orderPreserving = element.boolean();
            }
        }
        else if (fieldName == kBufferSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kBufferSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBufferSizeBit);

                _bufferSize = element._numberInt();
            }
        }
        else if (fieldName == kKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kKeyBit);

                _key = element.Obj();
            }
        }
        else if (fieldName == kBoundariesFieldName) {
            if (MONGO_unlikely(usedFields[kBoundariesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kBoundariesBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kBoundariesFieldName, &ctxt);
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
            _boundaries = std::move(values);
        }
        else if (fieldName == kConsumerIdsFieldName) {
            if (MONGO_unlikely(usedFields[kConsumerIdsBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kConsumerIdsBit);

            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kConsumerIdsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _consumerIds = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kPolicyBit]) {
            ctxt.throwMissingField(kPolicyFieldName);
        }
        if (!usedFields[kConsumersBit]) {
            ctxt.throwMissingField(kConsumersFieldName);
        }
        if (!usedFields[kOrderPreservingBit]) {
            _orderPreserving = false;
        }
        if (!usedFields[kBufferSizeBit]) {
            _bufferSize = 16777216;
        }
        if (!usedFields[kKeyBit]) {
            _key = BSONObj();
        }
    }

}


void ExchangeSpec::serialize(BSONObjBuilder* builder) const {
    invariant(_hasPolicy && _hasConsumers);

    {
        builder->append(kPolicyFieldName, ExchangePolicy_serializer(_policy));
    }

    builder->append(kConsumersFieldName, _consumers);

    builder->append(kOrderPreservingFieldName, _orderPreserving);

    builder->append(kBufferSizeFieldName, _bufferSize);

    builder->append(kKeyFieldName, _key);

    if (_boundaries.is_initialized()) {
        builder->append(kBoundariesFieldName, _boundaries.get());
    }

    if (_consumerIds.is_initialized()) {
        builder->append(kConsumerIdsFieldName, _consumerIds.get());
    }

}


BSONObj ExchangeSpec::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
