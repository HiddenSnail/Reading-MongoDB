/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/exchange_spec_gen.h --output build/opt/mongo/db/pipeline/exchange_spec_gen.cpp src/mongo/db/pipeline/exchange_spec.idl
 */

#pragma once

#include <algorithm>
#include <boost/optional.hpp>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "mongo/base/data_range.h"
#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/bson/simple_bsonobj_comparator.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * The type of an exchange distribution policy
 */
enum class ExchangePolicyEnum : std::int32_t {
    kBroadcast ,
    kRoundRobin ,
    kKeyRange ,
};

ExchangePolicyEnum ExchangePolicy_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData ExchangePolicy_serializer(ExchangePolicyEnum value);

/**
 * exchange aggregation request specification
 */
class ExchangeSpec {
public:
    static constexpr auto kBoundariesFieldName = "boundaries"_sd;
    static constexpr auto kBufferSizeFieldName = "bufferSize"_sd;
    static constexpr auto kConsumerIdsFieldName = "consumerIds"_sd;
    static constexpr auto kConsumersFieldName = "consumers"_sd;
    static constexpr auto kKeyFieldName = "key"_sd;
    static constexpr auto kOrderPreservingFieldName = "orderPreserving"_sd;
    static constexpr auto kPolicyFieldName = "policy"_sd;

    ExchangeSpec();
    ExchangeSpec(mongo::ExchangePolicyEnum policy, std::int32_t consumers);

    static ExchangeSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * A string indicating a policy of how documents are distributed to consumers.
     */
    mongo::ExchangePolicyEnum getPolicy() const { return _policy; }
    void setPolicy(mongo::ExchangePolicyEnum value) & {  _policy = std::move(value); _hasPolicy = true; }

    /**
     * Number of consumers.
     */
    std::int32_t getConsumers() const { return _consumers; }
    void setConsumers(std::int32_t value) & {  _consumers = std::move(value); _hasConsumers = true; }

    /**
     * A flag indicating documents are merged while preserving the order.
     */
    bool getOrderPreserving() const { return _orderPreserving; }
    void setOrderPreserving(bool value) & {  _orderPreserving = std::move(value);  }

    /**
     * The size of exchange buffers.
     */
    std::int32_t getBufferSize() const { return _bufferSize; }
    void setBufferSize(std::int32_t value) & {  _bufferSize = std::move(value);  }

    /**
     * A key used for document distribution to consumers. The same description as sorting/sharding. If the document entering the exchange does not have every field listed here, or if any prefix of any path is multikey (i.e. an array is encountered while traversing a path listed here), then it is by definition sent to consumer 0.
     */
    const mongo::BSONObj& getKey() const { return _key; }
    void setKey(mongo::BSONObj value) & {  _key = std::move(value);  }

    /**
     * Range/hash split points.
     */
    const boost::optional<std::vector<mongo::BSONObj>>& getBoundaries() const& { return _boundaries; }
    void getBoundaries() && = delete;
    void setBoundaries(boost::optional<std::vector<mongo::BSONObj>> value) & {  _boundaries = std::move(value);  }

    /**
     * Mapping from a range index to a consumer id.
     */
    const boost::optional<std::vector<std::int32_t>>& getConsumerIds() const& { return _consumerIds; }
    void getConsumerIds() && = delete;
    void setConsumerIds(boost::optional<std::vector<std::int32_t>> value) & {  _consumerIds = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::ExchangePolicyEnum _policy;
    std::int32_t _consumers;
    bool _orderPreserving{false};
    std::int32_t _bufferSize{16777216};
    mongo::BSONObj _key{BSONObj()};
    boost::optional<std::vector<mongo::BSONObj>> _boundaries;
    boost::optional<std::vector<std::int32_t>> _consumerIds;
    bool _hasPolicy : 1;
    bool _hasConsumers : 1;
};

}  // namespace mongo
