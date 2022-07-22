/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/vote_commit_index_build_gen.h --output build/opt/mongo/db/commands/vote_commit_index_build_gen.cpp src/mongo/db/commands/vote_commit_index_build.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/vote_commit_index_build_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData VoteCommitIndexBuild::kCommandParameterFieldName;
constexpr StringData VoteCommitIndexBuild::kDbNameFieldName;
constexpr StringData VoteCommitIndexBuild::kHostAndPortFieldName;
constexpr StringData VoteCommitIndexBuild::kCommandName;

const std::vector<StringData> VoteCommitIndexBuild::_knownBSONFields {
    VoteCommitIndexBuild::kHostAndPortFieldName,
    VoteCommitIndexBuild::kCommandName,
};
const std::vector<StringData> VoteCommitIndexBuild::_knownOP_MSGFields {
    VoteCommitIndexBuild::kDbNameFieldName,
    VoteCommitIndexBuild::kHostAndPortFieldName,
    VoteCommitIndexBuild::kCommandName,
};

VoteCommitIndexBuild::VoteCommitIndexBuild(const mongo::UUID commandParameter) : _commandParameter(std::move(commandParameter)), _hasHostAndPort(false), _hasDbName(false) {
    // Used for initialization only
}
VoteCommitIndexBuild::VoteCommitIndexBuild(const mongo::UUID commandParameter, mongo::HostAndPort hostAndPort) : _commandParameter(std::move(commandParameter)), _hostAndPort(std::move(hostAndPort)), _hasHostAndPort(true), _hasDbName(true) {
    // Used for initialization only
}


VoteCommitIndexBuild VoteCommitIndexBuild::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    mongo::UUID localCmdType;
    VoteCommitIndexBuild object(localCmdType);
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void VoteCommitIndexBuild::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kHostAndPortFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasHostAndPort = true;
                _hostAndPort = HostAndPort::parseThrowing(element.valueStringData());
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kHostAndPortFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kHostAndPortFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertBinDataType(commandElement, newUUID))) {
            _commandParameter = UUID(uassertStatusOK(UUID::parse(commandElement)));
        }
    }
}

VoteCommitIndexBuild VoteCommitIndexBuild::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    mongo::UUID localCmdType;
    VoteCommitIndexBuild object(localCmdType);
    object.parseProtected(ctxt, request);
    return object;
}
void VoteCommitIndexBuild::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::set<StringData> usedFields;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            commandElement = element;
            firstFieldFound = true;
            continue;
        }
        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kHostAndPortFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasHostAndPort = true;
                _hostAndPort = HostAndPort::parseThrowing(element.valueStringData());
            }
        }
        else if (fieldName == kDbNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasDbName = true;
                _dbName = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kHostAndPortFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kHostAndPortFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kDbNameFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kDbNameFieldName);
    }

    {
        if (MONGO_likely(ctxt.checkAndAssertBinDataType(commandElement, newUUID))) {
            _commandParameter = UUID(uassertStatusOK(UUID::parse(commandElement)));
        }
    }
}

void VoteCommitIndexBuild::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasHostAndPort && _hasDbName);

    {
        ConstDataRange tempCDR = _commandParameter.toCDR();
        builder->append(kCommandParameterFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }
    {
        builder->append(kHostAndPortFieldName, _hostAndPort.toString());
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest VoteCommitIndexBuild::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasHostAndPort && _hasDbName);

        {
            ConstDataRange tempCDR = _commandParameter.toCDR();
            builder->append(kCommandParameterFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
        }
        {
            builder->append(kHostAndPortFieldName, _hostAndPort.toString());
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj VoteCommitIndexBuild::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
