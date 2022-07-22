/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/balancer_collection_status_gen.h --output build/opt/mongo/s/request_types/balancer_collection_status_gen.cpp src/mongo/s/request_types/balancer_collection_status.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/balancer_collection_status_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData BalancerCollectionStatusResponse::kBalancerCompliantFieldName;
constexpr StringData BalancerCollectionStatusResponse::kFirstComplianceViolationFieldName;


BalancerCollectionStatusResponse::BalancerCollectionStatusResponse() : _balancerCompliant(false), _hasBalancerCompliant(false) {
    // Used for initialization only
}
BalancerCollectionStatusResponse::BalancerCollectionStatusResponse(bool balancerCompliant) : _balancerCompliant(std::move(balancerCompliant)), _hasBalancerCompliant(true) {
    // Used for initialization only
}


BalancerCollectionStatusResponse BalancerCollectionStatusResponse::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    BalancerCollectionStatusResponse object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void BalancerCollectionStatusResponse::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kBalancerCompliantFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                _hasBalancerCompliant = true;
                _balancerCompliant = element.boolean();
            }
        }
        else if (fieldName == kFirstComplianceViolationFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _firstComplianceViolation = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kBalancerCompliantFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kBalancerCompliantFieldName);
    }

}


void BalancerCollectionStatusResponse::serialize(BSONObjBuilder* builder) const {
    invariant(_hasBalancerCompliant);

    builder->append(kBalancerCompliantFieldName, _balancerCompliant);

    if (_firstComplianceViolation.is_initialized()) {
        builder->append(kFirstComplianceViolationFieldName, _firstComplianceViolation.get());
    }

}


BSONObj BalancerCollectionStatusResponse::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData BalancerCollectionStatus::kCommandParameterFieldName;
constexpr StringData BalancerCollectionStatus::kDbNameFieldName;
constexpr StringData BalancerCollectionStatus::kCommandName;

const std::vector<StringData> BalancerCollectionStatus::_knownBSONFields {
    BalancerCollectionStatus::kCommandName,
};
const std::vector<StringData> BalancerCollectionStatus::_knownOP_MSGFields {
    BalancerCollectionStatus::kDbNameFieldName,
    BalancerCollectionStatus::kCommandName,
};

BalancerCollectionStatus::BalancerCollectionStatus(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}


BalancerCollectionStatus BalancerCollectionStatus::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    BalancerCollectionStatus object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void BalancerCollectionStatus::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
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

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

BalancerCollectionStatus BalancerCollectionStatus::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    BalancerCollectionStatus object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void BalancerCollectionStatus::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
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

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

void BalancerCollectionStatus::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest BalancerCollectionStatus::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj BalancerCollectionStatus::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

constexpr StringData ConfigsvrBalancerCollectionStatus::kCommandParameterFieldName;
constexpr StringData ConfigsvrBalancerCollectionStatus::kDbNameFieldName;
constexpr StringData ConfigsvrBalancerCollectionStatus::kCommandName;

const std::vector<StringData> ConfigsvrBalancerCollectionStatus::_knownBSONFields {
    ConfigsvrBalancerCollectionStatus::kCommandName,
};
const std::vector<StringData> ConfigsvrBalancerCollectionStatus::_knownOP_MSGFields {
    ConfigsvrBalancerCollectionStatus::kDbNameFieldName,
    ConfigsvrBalancerCollectionStatus::kCommandName,
};

ConfigsvrBalancerCollectionStatus::ConfigsvrBalancerCollectionStatus(const mongo::NamespaceString commandParameter) : _commandParameter(std::move(commandParameter)), _hasDbName(false) {
    // Used for initialization only
}


ConfigsvrBalancerCollectionStatus ConfigsvrBalancerCollectionStatus::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::NamespaceString localCmdType;
    ConfigsvrBalancerCollectionStatus object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrBalancerCollectionStatus::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
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

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

ConfigsvrBalancerCollectionStatus ConfigsvrBalancerCollectionStatus::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::NamespaceString localCmdType;
    ConfigsvrBalancerCollectionStatus object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void ConfigsvrBalancerCollectionStatus::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<1> usedFields;
    const size_t kDbNameBit = 0;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
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

    {
        if (MONGO_likely(ctxt.checkAndAssertType(commandElement, String))) {
            _commandParameter = NamespaceString(commandElement.valueStringData());
        }
    }
}

void ConfigsvrBalancerCollectionStatus::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasDbName);

    {
        builder->append(kCommandParameterFieldName, _commandParameter.toString());
    }
    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest ConfigsvrBalancerCollectionStatus::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasDbName);

        {
            builder->append(kCommandParameterFieldName, _commandParameter.toString());
        }
        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj ConfigsvrBalancerCollectionStatus::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
