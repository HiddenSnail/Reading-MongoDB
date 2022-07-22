/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/query/async_results_merger_params_gen.h --output build/opt/mongo/s/query/async_results_merger_params_gen.cpp src/mongo/s/query/async_results_merger_params.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/query/async_results_merger_params_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData RemoteCursor::kCursorResponseFieldName;
constexpr StringData RemoteCursor::kHostAndPortFieldName;
constexpr StringData RemoteCursor::kShardIdFieldName;


RemoteCursor::RemoteCursor() : _hasShardId(false), _hasHostAndPort(false), _hasCursorResponse(false) {
    // Used for initialization only
}
RemoteCursor::RemoteCursor(std::string shardId, mongo::HostAndPort hostAndPort, mongo::CursorResponse cursorResponse) : _shardId(std::move(shardId)), _hostAndPort(std::move(hostAndPort)), _cursorResponse(std::move(cursorResponse)), _hasShardId(true), _hasHostAndPort(true), _hasCursorResponse(true) {
    // Used for initialization only
}


RemoteCursor RemoteCursor::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    RemoteCursor object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void RemoteCursor::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kShardIdBit = 0;
    const size_t kHostAndPortBit = 1;
    const size_t kCursorResponseBit = 2;

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
        else if (fieldName == kHostAndPortFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kHostAndPortBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kHostAndPortBit);

                _hasHostAndPort = true;
                _hostAndPort = HostAndPort::parseThrowing(element.valueStringData());
            }
        }
        else if (fieldName == kCursorResponseFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kCursorResponseBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCursorResponseBit);

                _hasCursorResponse = true;
                const BSONObj localObject = element.Obj();
                _cursorResponse = CursorResponse::parseFromBSONThrowing(localObject);
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
        if (!usedFields[kHostAndPortBit]) {
            ctxt.throwMissingField(kHostAndPortFieldName);
        }
        if (!usedFields[kCursorResponseBit]) {
            ctxt.throwMissingField(kCursorResponseFieldName);
        }
    }

}


void RemoteCursor::serialize(BSONObjBuilder* builder) const {
    invariant(_hasShardId && _hasHostAndPort && _hasCursorResponse);

    builder->append(kShardIdFieldName, _shardId);

    {
        builder->append(kHostAndPortFieldName, _hostAndPort.toString());
    }

    {
        const BSONObj localObject = _cursorResponse.toBSONAsInitialResponse();
        builder->append(kCursorResponseFieldName, localObject);
    }

}


BSONObj RemoteCursor::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData AsyncResultsMergerParams::kOperationSessionInfoFieldName;
constexpr StringData AsyncResultsMergerParams::kAllowPartialResultsFieldName;
constexpr StringData AsyncResultsMergerParams::kAutocommitFieldName;
constexpr StringData AsyncResultsMergerParams::kBatchSizeFieldName;
constexpr StringData AsyncResultsMergerParams::kCompareWholeSortKeyFieldName;
constexpr StringData AsyncResultsMergerParams::kCoordinatorFieldName;
constexpr StringData AsyncResultsMergerParams::kNssFieldName;
constexpr StringData AsyncResultsMergerParams::kRecordRemoteOpWaitTimeFieldName;
constexpr StringData AsyncResultsMergerParams::kRemotesFieldName;
constexpr StringData AsyncResultsMergerParams::kSessionIdFieldName;
constexpr StringData AsyncResultsMergerParams::kSortFieldName;
constexpr StringData AsyncResultsMergerParams::kStartTransactionFieldName;
constexpr StringData AsyncResultsMergerParams::kTailableModeFieldName;
constexpr StringData AsyncResultsMergerParams::kTxnNumberFieldName;


AsyncResultsMergerParams::AsyncResultsMergerParams() : _hasRemotes(false), _hasNss(false) {
    // Used for initialization only
}
AsyncResultsMergerParams::AsyncResultsMergerParams(std::vector<mongo::RemoteCursor> remotes, mongo::NamespaceString nss) : _remotes(std::move(remotes)), _nss(std::move(nss)), _hasRemotes(true), _hasNss(true) {
    // Used for initialization only
}


AsyncResultsMergerParams AsyncResultsMergerParams::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    AsyncResultsMergerParams object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void AsyncResultsMergerParams::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<14> usedFields;
    const size_t kSessionIdBit = 0;
    const size_t kTxnNumberBit = 1;
    const size_t kAutocommitBit = 2;
    const size_t kStartTransactionBit = 3;
    const size_t kCoordinatorBit = 4;
    const size_t kSortBit = 5;
    const size_t kCompareWholeSortKeyBit = 6;
    const size_t kRemotesBit = 7;
    const size_t kTailableModeBit = 8;
    const size_t kBatchSizeBit = 9;
    const size_t kNssBit = 10;
    const size_t kAllowPartialResultsBit = 11;
    const size_t kRecordRemoteOpWaitTimeBit = 12;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kSessionIdFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSessionIdBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSessionIdBit);

                IDLParserErrorContext tempContext(kSessionIdFieldName, &ctxt);
                const auto localObject = element.Obj();
                _operationSessionInfo.setSessionId(mongo::LogicalSessionFromClient::parse(tempContext, localObject));
            }
        }
        else if (fieldName == kTxnNumberFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kTxnNumberBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTxnNumberBit);

                _operationSessionInfo.setTxnNumber(element._numberLong());
            }
        }
        else if (fieldName == kAutocommitFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAutocommitBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAutocommitBit);

                _operationSessionInfo.setAutocommit(element.boolean());
            }
        }
        else if (fieldName == kStartTransactionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kStartTransactionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kStartTransactionBit);

                _operationSessionInfo.setStartTransaction(element.boolean());
            }
        }
        else if (fieldName == kCoordinatorFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kCoordinatorBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCoordinatorBit);

                _operationSessionInfo.setCoordinator(element.boolean());
            }
        }
        else if (fieldName == kSortFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSortBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSortBit);

                _sort = element.Obj();
            }
        }
        else if (fieldName == kCompareWholeSortKeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kCompareWholeSortKeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCompareWholeSortKeyBit);

                _compareWholeSortKey = element.boolean();
            }
        }
        else if (fieldName == kRemotesFieldName) {
            if (MONGO_unlikely(usedFields[kRemotesBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kRemotesBit);

            _hasRemotes = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kRemotesFieldName, &ctxt);
            std::vector<mongo::RemoteCursor> values;

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
                        IDLParserErrorContext tempContext(kRemotesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::RemoteCursor::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _remotes = std::move(values);
        }
        else if (fieldName == kTailableModeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kTailableModeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kTailableModeBit);

                IDLParserErrorContext tempContext(kTailableModeFieldName, &ctxt);
                _tailableMode = TailableMode_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kBatchSizeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertTypes(element, {NumberLong, NumberInt, NumberDecimal, NumberDouble}))) {
                if (MONGO_unlikely(usedFields[kBatchSizeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBatchSizeBit);

                _batchSize = element.safeNumberLong();
            }
        }
        else if (fieldName == kNssFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kNssBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNssBit);

                _hasNss = true;
                _nss = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kAllowPartialResultsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kAllowPartialResultsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kAllowPartialResultsBit);

                _allowPartialResults = element.boolean();
            }
        }
        else if (fieldName == kRecordRemoteOpWaitTimeFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kRecordRemoteOpWaitTimeBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRecordRemoteOpWaitTimeBit);

                _recordRemoteOpWaitTime = element.boolean();
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kCompareWholeSortKeyBit]) {
            _compareWholeSortKey = false;
        }
        if (!usedFields[kRemotesBit]) {
            ctxt.throwMissingField(kRemotesFieldName);
        }
        if (!usedFields[kNssBit]) {
            ctxt.throwMissingField(kNssFieldName);
        }
        if (!usedFields[kAllowPartialResultsBit]) {
            _allowPartialResults = false;
        }
        if (!usedFields[kRecordRemoteOpWaitTimeBit]) {
            _recordRemoteOpWaitTime = false;
        }
    }

}


void AsyncResultsMergerParams::serialize(BSONObjBuilder* builder) const {
    invariant(_hasRemotes && _hasNss);

    {
        _operationSessionInfo.serialize(builder);
    }

    if (_sort.is_initialized()) {
        builder->append(kSortFieldName, _sort.get());
    }

    builder->append(kCompareWholeSortKeyFieldName, _compareWholeSortKey);

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kRemotesFieldName));
        for (const auto& item : _remotes) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

    if (_tailableMode.is_initialized()) {
        builder->append(kTailableModeFieldName, TailableMode_serializer(_tailableMode.get()));
    }

    if (_batchSize.is_initialized()) {
        builder->append(kBatchSizeFieldName, _batchSize.get());
    }

    {
        builder->append(kNssFieldName, _nss.toString());
    }

    builder->append(kAllowPartialResultsFieldName, _allowPartialResults);

    builder->append(kRecordRemoteOpWaitTimeFieldName, _recordRemoteOpWaitTime);

}


BSONObj AsyncResultsMergerParams::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
