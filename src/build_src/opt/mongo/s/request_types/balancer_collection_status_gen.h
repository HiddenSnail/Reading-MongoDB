/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/balancer_collection_status_gen.h --output build/opt/mongo/s/request_types/balancer_collection_status_gen.cpp src/mongo/s/request_types/balancer_collection_status.idl
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
 * Response of the config server command
 */
class BalancerCollectionStatusResponse {
public:
    static constexpr auto kBalancerCompliantFieldName = "balancerCompliant"_sd;
    static constexpr auto kFirstComplianceViolationFieldName = "firstComplianceViolation"_sd;

    BalancerCollectionStatusResponse();
    BalancerCollectionStatusResponse(bool balancerCompliant);

    static BalancerCollectionStatusResponse parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * true if there are no actions needed, if false, then firstComplianceViolation will contain the violation with the highest priority which will be addressed next
     */
    bool getBalancerCompliant() const { return _balancerCompliant; }
    void setBalancerCompliant(bool value) & {  _balancerCompliant = std::move(value); _hasBalancerCompliant = true; }

    /**
     * One of the following: draining, zoneViolation or chunksImbalance
     */
    const boost::optional<StringData> getFirstComplianceViolation() const& { return boost::optional<StringData>{_firstComplianceViolation}; }
    void getFirstComplianceViolation() && = delete;
    void setFirstComplianceViolation(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _firstComplianceViolation = std::move(_tmpValue);
    } else {
        _firstComplianceViolation = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    bool _balancerCompliant;
    boost::optional<std::string> _firstComplianceViolation;
    bool _hasBalancerCompliant : 1;
};

/**
 * Public balancerCollectionStatus command on mongos
 */
class BalancerCollectionStatus {
public:
    static constexpr auto kCommandParameterFieldName = "balancerCollectionStatus"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "balancerCollectionStatus"_sd;

    explicit BalancerCollectionStatus(const mongo::NamespaceString commandParameter);

    static BalancerCollectionStatus parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static BalancerCollectionStatus parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    mongo::NamespaceString _commandParameter;

    std::string _dbName;
    bool _hasDbName : 1;
};

/**
 * Internal balancerCollectionStatus command on the config server
 */
class ConfigsvrBalancerCollectionStatus {
public:
    static constexpr auto kCommandParameterFieldName = "_configsvrBalancerCollectionStatus"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kCommandName = "_configsvrBalancerCollectionStatus"_sd;

    explicit ConfigsvrBalancerCollectionStatus(const mongo::NamespaceString commandParameter);

    static ConfigsvrBalancerCollectionStatus parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static ConfigsvrBalancerCollectionStatus parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const mongo::NamespaceString& getCommandParameter() const { return _commandParameter; }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    mongo::NamespaceString _commandParameter;

    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
