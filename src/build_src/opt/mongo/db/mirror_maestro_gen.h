/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/mirror_maestro_gen.h --output build/opt/mongo/db/mirror_maestro_gen.cpp src/mongo/db/mirror_maestro.idl
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
#include "mongo/db/mirror_maestro.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A struct representing how to mirror read command requests
 */
class MirroredReadsParameters {
public:
    static constexpr auto kMaxTimeMSFieldName = "maxTimeMS"_sd;
    static constexpr auto kSamplingRateFieldName = "samplingRate"_sd;

    MirroredReadsParameters();

    static MirroredReadsParameters parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * A floating-point value within [0.0,1.0] specifying what fraction of reads to sample
     */
    double getSamplingRate() const { return _samplingRate; }
    void setSamplingRate(double value) & { validateSamplingRate(value); _samplingRate = std::move(value);  }

    /**
     * The maxTimeMS to apply to mirrored reads
     */
    std::int32_t getMaxTimeMS() const { return _maxTimeMS; }
    void setMaxTimeMS(std::int32_t value) & { validateMaxTimeMS(value); _maxTimeMS = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    void validateSamplingRate(const double value);
    void validateSamplingRate(IDLParserErrorContext& ctxt, const double value);

    void validateMaxTimeMS(const std::int32_t value);
    void validateMaxTimeMS(IDLParserErrorContext& ctxt, const std::int32_t value);

private:
    double _samplingRate{0.01};
    std::int32_t _maxTimeMS{1000};
};

class MirroredReadsServerParameter : public ServerParameter {
public:
    using ServerParameter::ServerParameter;

    void append(OperationContext*, BSONObjBuilder&, const std::string&) final;
    Status set(const BSONElement&) final;
    Status setFromString(const std::string&) final;

    synchronized_value<MirroredReadsParameters> _data;
};

}  // namespace mongo
