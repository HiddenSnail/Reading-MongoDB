/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/api_parameters_gen.h --output build/opt/mongo/db/api_parameters_gen.cpp src/mongo/db/api_parameters.idl
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
#include "mongo/db/logical_time.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/tenant_id.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/basic_types_gen.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/stdx/unordered_map.h"
#include "mongo/util/uuid.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {

/**
 * Parser for pulling out VersionedAPI parameters from commands
 */
class APIParametersFromClient {
public:
    static constexpr auto kApiDeprecationErrorsFieldName = "apiDeprecationErrors"_sd;
    static constexpr auto kApiStrictFieldName = "apiStrict"_sd;
    static constexpr auto kApiVersionFieldName = "apiVersion"_sd;

    APIParametersFromClient();

    static APIParametersFromClient parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The api version specified by the command
     */
    boost::optional<StringData> getApiVersion() const { return boost::optional<StringData>{_apiVersion}; }
    void setApiVersion(boost::optional<StringData> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _apiVersion = std::move(_tmpValue);
    } else {
        _apiVersion = boost::none;
    }
      }
    /**
     * With apiVersion: 'V' and apiStrict: true, the server rejects requests to use behaviors not included in V
     */
    boost::optional<bool> getApiStrict() const { return _apiStrict; }
    void setApiStrict(boost::optional<bool> value) {  _apiStrict = std::move(value);  }
    /**
     * With apiVersion: 'V' and apiDeprecationErrors: true, the server rejects requests to use behaviors deprecated in V in the current MongoDB release
     */
    boost::optional<bool> getApiDeprecationErrors() const { return _apiDeprecationErrors; }
    void setApiDeprecationErrors(boost::optional<bool> value) {  _apiDeprecationErrors = std::move(value);  }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<std::string> _apiVersion;
    boost::optional<bool> _apiStrict;
    boost::optional<bool> _apiDeprecationErrors;
};

constexpr auto kRequireApiVersionDefault = false;

extern AtomicWord<bool> gRequireApiVersion;
constexpr auto kAcceptApiVersion2Default = false;

extern bool acceptApiVersion2;
}  // namespace mongo
