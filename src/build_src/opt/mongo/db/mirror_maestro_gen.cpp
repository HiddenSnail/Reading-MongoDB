/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/mirror_maestro_gen.h --output build/opt/mongo/db/mirror_maestro_gen.cpp src/mongo/db/mirror_maestro.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/mirror_maestro_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"

namespace mongo {

constexpr StringData MirroredReadsParameters::kMaxTimeMSFieldName;
constexpr StringData MirroredReadsParameters::kSamplingRateFieldName;


MirroredReadsParameters::MirroredReadsParameters()  {
    // Used for initialization only
}

void MirroredReadsParameters::validateSamplingRate(IDLParserErrorContext& ctxt, const double value)
{
    if (!(value >= 0.0)) {
        throwComparisonError<double>(ctxt, "samplingRate", ">="_sd, value, 0.0);
    }
    if (!(value <= 1.0)) {
        throwComparisonError<double>(ctxt, "samplingRate", "<="_sd, value, 1.0);
    }
}

void MirroredReadsParameters::validateSamplingRate(const double value)
{
    if (!(value >= 0.0)) {
        throwComparisonError<double>("samplingRate", ">="_sd, value, 0.0);
    }
    if (!(value <= 1.0)) {
        throwComparisonError<double>("samplingRate", "<="_sd, value, 1.0);
    }
}

void MirroredReadsParameters::validateMaxTimeMS(IDLParserErrorContext& ctxt, const std::int32_t value)
{
    if (!(value > 0)) {
        throwComparisonError<std::int32_t>(ctxt, "maxTimeMS", ">"_sd, value, 0);
    }
}

void MirroredReadsParameters::validateMaxTimeMS(const std::int32_t value)
{
    if (!(value > 0)) {
        throwComparisonError<std::int32_t>("maxTimeMS", ">"_sd, value, 0);
    }
}


MirroredReadsParameters MirroredReadsParameters::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    MirroredReadsParameters object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void MirroredReadsParameters::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<2> usedFields;
    const size_t kSamplingRateBit = 0;
    const size_t kMaxTimeMSBit = 1;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kSamplingRateFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberDouble))) {
                if (MONGO_unlikely(usedFields[kSamplingRateBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSamplingRateBit);

                {
                    auto value = element._numberDouble();
                    validateSamplingRate(value);
                    _samplingRate = std::move(value);
                }
            }
        }
        else if (fieldName == kMaxTimeMSFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberInt))) {
                if (MONGO_unlikely(usedFields[kMaxTimeMSBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMaxTimeMSBit);

                {
                    auto value = element._numberInt();
                    validateMaxTimeMS(value);
                    _maxTimeMS = std::move(value);
                }
            }
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kSamplingRateBit]) {
            _samplingRate = 0.01;
        }
        if (!usedFields[kMaxTimeMSBit]) {
            _maxTimeMS = 1000;
        }
    }

}


void MirroredReadsParameters::serialize(BSONObjBuilder* builder) const {
    builder->append(kSamplingRateFieldName, _samplingRate);

    builder->append(kMaxTimeMSFieldName, _maxTimeMS);

}


BSONObj MirroredReadsParameters::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

MONGO_SERVER_PARAMETER_REGISTER(idl_2bd42c5c414b5a082839adafa629416d2fa69be8)(InitializerContext*) {
    /**
     * How to mirror read command requests
     */
    MONGO_COMPILER_VARIABLE_UNUSED auto* scp_0 = ([]() -> ServerParameter* {
        return new MirroredReadsServerParameter("mirrorReads", ServerParameterType::kStartupAndRuntime);
    })();

    return Status::OK();
}
}  // namespace mongo
