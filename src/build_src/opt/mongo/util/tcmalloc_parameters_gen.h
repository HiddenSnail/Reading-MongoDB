/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/util/tcmalloc_parameters_gen.h --output build/opt/mongo/util/tcmalloc_parameters_gen.cpp src/mongo/util/tcmalloc_parameters.idl
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
#include "mongo/config.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"

namespace mongo {

class TCMallocMaxTotalThreadCacheBytesServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

class TCMallocAggressiveMemoryDecommitServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
constexpr auto kHeapProfilingEnabledDefault = false;
#endif

#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
extern bool HeapProfilingEnabled;
#endif
#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
constexpr auto kHeapProfilingSampleIntervalBytesDefault = 262144;
#endif

#if defined(_POSIX_VERSION) && (defined(MONGO_CONFIG_HAVE_EXECINFO_BACKTRACE) || defined(MONGO_CONFIG_USE_LIBUNWIND))
extern long long HeapProfilingSampleIntervalBytes;
#endif
class TCMallocEnableMarkThreadTemporarilyIdle : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

class TCMallocReleaseRateServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;
};

}  // namespace mongo
