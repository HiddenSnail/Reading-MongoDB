/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/commands/drop_connections_gen.h --output build/opt/mongo/db/commands/drop_connections_gen.cpp src/mongo/db/commands/drop_connections.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/commands/drop_connections_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData DropConnections::kDbNameFieldName;
constexpr StringData DropConnections::kHostAndPortFieldName;
constexpr StringData DropConnections::kCommandName;

const std::vector<StringData> DropConnections::_knownBSONFields {
    DropConnections::kHostAndPortFieldName,
    DropConnections::kCommandName,
};
const std::vector<StringData> DropConnections::_knownOP_MSGFields {
    DropConnections::kDbNameFieldName,
    DropConnections::kHostAndPortFieldName,
    DropConnections::kCommandName,
};

DropConnections::DropConnections() : _hasHostAndPort(false), _hasDbName(false) {
    // Used for initialization only
}
DropConnections::DropConnections(std::vector<mongo::HostAndPort> hostAndPort) : _hostAndPort(std::move(hostAndPort)), _hasHostAndPort(true), _hasDbName(true) {
    // Used for initialization only
}


DropConnections DropConnections::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    DropConnections object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void DropConnections::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kHostAndPortBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kHostAndPortFieldName) {
            if (MONGO_unlikely(usedFields[kHostAndPortBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kHostAndPortBit);

            _hasHostAndPort = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kHostAndPortFieldName, &ctxt);
            std::vector<mongo::HostAndPort> values;

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
                        values.emplace_back(HostAndPort::parseThrowing(arrayElement.valueStringData()));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _hostAndPort = std::move(values);
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
        if (!usedFields[kHostAndPortBit]) {
            ctxt.throwMissingField(kHostAndPortFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

DropConnections DropConnections::parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    DropConnections object;
    object.parseProtected(ctxt, request);
    return object;
}
void DropConnections::parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request) {
    std::bitset<2> usedFields;
    const size_t kHostAndPortBit = 0;
    const size_t kDbNameBit = 1;
    BSONElement commandElement;
    bool firstFieldFound = false;

    for (const auto& element :request.body) {
        const auto fieldName = element.fieldNameStringData();

        if (firstFieldFound == false) {
            firstFieldFound = true;
            continue;
        }

        if (fieldName == kHostAndPortFieldName) {
            if (MONGO_unlikely(usedFields[kHostAndPortBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kHostAndPortBit);

            _hasHostAndPort = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kHostAndPortFieldName, &ctxt);
            std::vector<mongo::HostAndPort> values;

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
                        values.emplace_back(HostAndPort::parseThrowing(arrayElement.valueStringData()));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _hostAndPort = std::move(values);
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
        if (!usedFields[kHostAndPortBit]) {
            ctxt.throwMissingField(kHostAndPortFieldName);
        }
        if (!usedFields[kDbNameBit]) {
            ctxt.throwMissingField(kDbNameFieldName);
        }
    }

}

void DropConnections::serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const {
    invariant(_hasHostAndPort && _hasDbName);

    builder->append("dropConnections"_sd, 1);
    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kHostAndPortFieldName));
        for (const auto& item : _hostAndPort) {
            arrayBuilder.append(item.toString());
        }
    }

    IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownBSONFields, builder);

}

OpMsgRequest DropConnections::serialize(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder localBuilder;
    {
        BSONObjBuilder* builder = &localBuilder;
        invariant(_hasHostAndPort && _hasDbName);

        builder->append("dropConnections"_sd, 1);
        {
            BSONArrayBuilder arrayBuilder(builder->subarrayStart(kHostAndPortFieldName));
            for (const auto& item : _hostAndPort) {
                arrayBuilder.append(item.toString());
            }
        }

        builder->append(kDbNameFieldName, _dbName);

        IDLParserErrorContext::appendGenericCommandArguments(commandPassthroughFields, _knownOP_MSGFields, builder);

    }
    OpMsgRequest request;
    request.body = localBuilder.obj();
    return request;
}

BSONObj DropConnections::toBSON(const BSONObj& commandPassthroughFields) const {
    BSONObjBuilder builder;
    serialize(commandPassthroughFields, &builder);
    return builder.obj();
}

}  // namespace mongo
