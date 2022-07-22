/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/auth/address_restriction_gen.h --output build/opt/mongo/db/auth/address_restriction_gen.cpp src/mongo/db/auth/address_restriction.idl
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
#include "mongo/db/auth/address_restriction.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * clientSource/serverAddress restriction pair
 */
class Address_restriction {
public:
    static constexpr auto kClientSourceFieldName = "clientSource"_sd;
    static constexpr auto kServerAddressFieldName = "serverAddress"_sd;

    Address_restriction();

    static Address_restriction parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const boost::optional<std::vector<StringData>> getClientSource() const& { if (_clientSource.is_initialized()) {
        return transformVector(_clientSource.get());
    } else {
        return boost::none;
    }
     }
    void getClientSource() && = delete;
    void setClientSource(boost::optional<std::vector<StringData>> value) & { if (value.is_initialized()) {
        auto _tmpValue = transformVector(value.get());
        
        _clientSource = std::move(_tmpValue);
    } else {
        _clientSource = boost::none;
    }
      }

    const boost::optional<std::vector<StringData>> getServerAddress() const& { if (_serverAddress.is_initialized()) {
        return transformVector(_serverAddress.get());
    } else {
        return boost::none;
    }
     }
    void getServerAddress() && = delete;
    void setServerAddress(boost::optional<std::vector<StringData>> value) & { if (value.is_initialized()) {
        auto _tmpValue = transformVector(value.get());
        
        _serverAddress = std::move(_tmpValue);
    } else {
        _serverAddress = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<std::vector<std::string>> _clientSource;
    boost::optional<std::vector<std::string>> _serverAddress;
};

}  // namespace mongo
