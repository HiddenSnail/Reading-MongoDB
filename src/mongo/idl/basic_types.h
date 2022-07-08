/**
 *    Copyright (C) 2019 MongoDB, Inc.
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

#pragma once

#include <boost/optional.hpp>

#include "mongo/base/string_data.h"
#include "mongo/bson/bsonobjbuilder.h"

namespace mongo {

/**
 * Wraps a boost::optional<bool> to provide consistent semantics. A standard boost::optional<bool>
 * can introduce ambiguity because its 'operator bool' will resolve to 'true' if the optional is
 * populated, even if is populated with boolean 'false'. By contrast, an instance of this class
 * always resolves to the populated value, or false if not yet populated. This class will also
 * serialize to BSON via the IDL only if the value has been explicitly set.
 */
class OptionalBool {
public:
    static OptionalBool parseFromBSON(BSONElement element) {
        uassert(ErrorCodes::BadValue,
                str::stream() << "Field '" << element.fieldNameStringData()
                              << "' should be a boolean value, but found: " << element.type(),
                !element || element.type() == BSONType::Bool);
        return element ? OptionalBool{element.boolean()} : OptionalBool{};
    }

    OptionalBool(boost::optional<bool> value) : _value(std::move(value)) {}
    OptionalBool(bool value) : _value(value) {}

    OptionalBool() : OptionalBool(boost::none) {}

    /**
     * Returns true only if _value is populated with a value of true.
     */
    operator bool() const {
        return _value.value_or(false);
    }

    /**
     * Returns true if the value has been populated, false otherwise.
     */
    bool has_value() const {
        return _value.has_value();
    }

    /**
     * Serialize this object as a field in a document. If _value is empty, omit the field.
     */
    void serializeToBSON(StringData fieldName, BSONObjBuilder* builder) const {
        if (_value) {
            builder->appendBool(fieldName, *_value);
        }
    }

    /**
     * Serialize this object as an element of a BSON array. If _value is empty, omit the entry.
     */
    void serializeToBSON(BSONArrayBuilder* builder) const {
        if (_value) {
            builder->append(*_value);
        }
    }

    /**
     * Returns a string representation of the current value.
     */
    std::string toString() const {
        return *this ? "1" : "0";
    }
    operator std::string() const {
        return toString();
    }

private:
    boost::optional<bool> _value;
};

}  // namespace mongo
