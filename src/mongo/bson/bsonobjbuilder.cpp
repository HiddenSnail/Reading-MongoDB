/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kDefault

#include "mongo/db/jsobj.h"

#include <boost/lexical_cast.hpp>

#include "mongo/bson/timestamp.h"
#include "mongo/logv2/log.h"

namespace mongo {

using std::string;

BSONObjBuilder& BSONObjBuilder::appendMinForType(StringData fieldName, int t) {
    switch (t) {
        // Shared canonical types
        case NumberInt:
        case NumberDouble:
        case NumberLong:
        case NumberDecimal:
            append(fieldName, std::numeric_limits<double>::quiet_NaN());
            return *this;
        case Symbol:
        case String:
            append(fieldName, "");
            return *this;
        case Date:
            appendDate(fieldName, Date_t::min());
            return *this;
        case bsonTimestamp:
            appendTimestamp(fieldName, 0);
            return *this;
        case Undefined:  // shared with EOO
            appendUndefined(fieldName);
            return *this;

        // Separate canonical types
        case MinKey:
            appendMinKey(fieldName);
            return *this;
        case MaxKey:
            appendMaxKey(fieldName);
            return *this;
        case jstOID: {
            OID o;
            appendOID(fieldName, &o);
            return *this;
        }
        case Bool:
            appendBool(fieldName, false);
            return *this;
        case jstNULL:
            appendNull(fieldName);
            return *this;
        case Object:
            append(fieldName, BSONObj());
            return *this;
        case Array:
            appendArray(fieldName, BSONObj());
            return *this;
        case BinData:
            appendBinData(fieldName, 0, BinDataGeneral, (const char*)nullptr);
            return *this;
        case RegEx:
            appendRegex(fieldName, "");
            return *this;
        case DBRef: {
            OID o;
            appendDBRef(fieldName, "", o);
            return *this;
        }
        case Code:
            appendCode(fieldName, "");
            return *this;
        case CodeWScope:
            appendCodeWScope(fieldName, "", BSONObj());
            return *this;
    };
    LOGV2(20101, "type not supported for appendMinElementForType: {t}", "t"_attr = t);
    uassert(10061, "type not supported for appendMinElementForType", false);
}

BSONObjBuilder& BSONObjBuilder::appendMaxForType(StringData fieldName, int t) {
    switch (t) {
        // Shared canonical types
        case NumberInt:
        case NumberDouble:
        case NumberLong:
        case NumberDecimal:
            append(fieldName, std::numeric_limits<double>::infinity());
            return *this;
        case Symbol:
        case String:
            appendMinForType(fieldName, Object);
            return *this;
        case Date:
            appendDate(fieldName, Date_t::max());
            return *this;
        case bsonTimestamp:
            append(fieldName, Timestamp::max());
            return *this;
        case Undefined:  // shared with EOO
            appendUndefined(fieldName);
            return *this;

        // Separate canonical types
        case MinKey:
            appendMinKey(fieldName);
            return *this;
        case MaxKey:
            appendMaxKey(fieldName);
            return *this;
        case jstOID: {
            OID o = OID::max();
            appendOID(fieldName, &o);
            return *this;
        }
        case Bool:
            appendBool(fieldName, true);
            return *this;
        case jstNULL:
            appendNull(fieldName);
            return *this;
        case Object:
            appendMinForType(fieldName, Array);
            return *this;
        case Array:
            appendMinForType(fieldName, BinData);
            return *this;
        case BinData:
            appendMinForType(fieldName, jstOID);
            return *this;
        case RegEx:
            appendMinForType(fieldName, DBRef);
            return *this;
        case DBRef:
            appendMinForType(fieldName, Code);
            return *this;
        case Code:
            appendMinForType(fieldName, CodeWScope);
            return *this;
        case CodeWScope:
            // This upper bound may change if a new bson type is added.
            appendMinForType(fieldName, MaxKey);
            return *this;
    }
    LOGV2(20102, "type not supported for appendMaxElementForType: {t}", "t"_attr = t);
    uassert(14853, "type not supported for appendMaxElementForType", false);
}

BSONObjBuilder& BSONObjBuilder::appendDate(StringData fieldName, Date_t dt) {
    _b.appendNum((char)Date);
    _b.appendStr(fieldName);
    _b.appendNum(dt.toMillisSinceEpoch());
    return *this;
}

/* add all the fields from the object specified to this object */
BSONObjBuilder& BSONObjBuilder::appendElements(const BSONObj& x) {
    if (!x.isEmpty())
        _b.appendBuf(x.objdata() + 4,   // skip over leading length
                     x.objsize() - 5);  // ignore leading length and trailing \0
    return *this;
}

/* add all the fields from the object specified to this object if they don't exist */
BSONObjBuilder& BSONObjBuilder::appendElementsUnique(const BSONObj& x) {
    std::set<std::string> have;
    {
        BSONObjIterator i = iterator();
        while (i.more())
            have.insert(i.next().fieldName());
    }

    BSONObjIterator it(x);
    while (it.more()) {
        BSONElement e = it.next();
        if (have.count(e.fieldName()))
            continue;
        append(e);
    }
    return *this;
}

BSONObjIterator BSONObjBuilder::iterator() const {
    const char* s = _b.buf() + _offset;
    const char* e = _b.buf() + _b.len();
    return BSONObjIterator(s, e);
}

bool BSONObjBuilder::hasField(StringData name) const {
    BSONObjIterator i = iterator();
    while (i.more())
        if (name == i.next().fieldName())
            return true;
    return false;
}

BSONObjBuilder::~BSONObjBuilder() {
    // If 'done' has not already been called, and we have a reference to an owning
    // BufBuilder but do not own it ourselves, then we must call _done to write in the
    // length. Otherwise, we own this memory and its lifetime ends with us, therefore
    // we can elide the write.
    if (!_doneCalled && _b.buf() && _buf.getSize() == 0) {
        _done();
    }
}

template class BasicBufBuilder<SharedBufferAllocator>;
template class BasicBufBuilder<StackAllocator<StackSizeDefault>>;
template class StringBuilderImpl<SharedBufferAllocator>;
template class StringBuilderImpl<StackAllocator<StackSizeDefault>>;

}  // namespace mongo
