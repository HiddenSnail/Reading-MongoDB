/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/scripting/mozjs/scripting_util_gen.h --output build/opt/mongo/scripting/mozjs/scripting_util_gen.cpp src/mongo/scripting/mozjs/scripting_util.idl
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
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing an endSessions command
 */
class EndSessions {
public:
    static constexpr auto kEndSessionsFieldName = "endSessions"_sd;

    EndSessions();
    EndSessions(std::vector<mongo::BSONObj> endSessions);

    static EndSessions parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const std::vector<mongo::BSONObj>& getEndSessions() const& { return _endSessions; }
    void getEndSessions() && = delete;
    void setEndSessions(std::vector<mongo::BSONObj> value) & {  _endSessions = std::move(value); _hasEndSessions = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::BSONObj> _endSessions;
    bool _hasEndSessions : 1;
};

constexpr auto kSkipShellCursorFinalizeDefault = false;

extern bool skipShellCursorFinalize;
}  // namespace mongo
