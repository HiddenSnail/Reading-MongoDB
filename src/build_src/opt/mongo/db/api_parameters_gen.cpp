/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/api_parameters_gen.h --output build/opt/mongo/db/api_parameters_gen.cpp src/mongo/db/api_parameters.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/api_parameters_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/idl/server_parameter.h"
#include "mongo/idl/server_parameter_with_storage.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

constexpr StringData APIParametersFromClient::kApiDeprecationErrorsFieldName;
constexpr StringData APIParametersFromClient::kApiStrictFieldName;
constexpr StringData APIParametersFromClient::kApiVersionFieldName;


APIParametersFromClient::APIParametersFromClient()  {
    // Used for initialization only
}


APIParametersFromClient APIParametersFromClient::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<APIParametersFromClient>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void APIParametersFromClient::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<3> usedFields;
    const size_t kApiVersionBit = 0;
    const size_t kApiStrictBit = 1;
    const size_t kApiDeprecationErrorsBit = 2;
    std::set<StringData> usedFieldSet;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kApiVersionFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kApiVersionBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kApiVersionBit);

                _apiVersion = element.str();
            }
        }
        else if (fieldName == kApiStrictFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kApiStrictBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kApiStrictBit);

                _apiStrict = element.boolean();
            }
        }
        else if (fieldName == kApiDeprecationErrorsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kApiDeprecationErrorsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kApiDeprecationErrorsBit);

                _apiDeprecationErrors = element.boolean();
            }
        }
        else {
            auto push_result = usedFieldSet.insert(fieldName);
            if (MONGO_unlikely(push_result.second == false)) {
                ctxt.throwDuplicateField(fieldName);
            }
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
    }

}


void APIParametersFromClient::serialize(BSONObjBuilder* builder) const {
    if (_apiVersion.is_initialized()) {
        builder->append(kApiVersionFieldName, _apiVersion.get());
    }

    if (_apiStrict.is_initialized()) {
        builder->append(kApiStrictFieldName, _apiStrict.get());
    }

    if (_apiDeprecationErrors.is_initialized()) {
        builder->append(kApiDeprecationErrorsFieldName, _apiDeprecationErrors.get());
    }

}


BSONObj APIParametersFromClient::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

AtomicWord<bool> gRequireApiVersion{false};
bool acceptApiVersion2{false};
MONGO_SERVER_PARAMETER_REGISTER(idl_39f18cfb42523261138617a51cdf30a17f374861)(InitializerContext*) {
    /**
     * Require clients to pass the 'apiVersion' parameter with all commands
     */
    auto* scp_0 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupAndRuntime>("requireApiVersion", gRequireApiVersion);
        return ret;
    })();
    scp_0->setTestOnly();

    /**
     * Allow users to issue commands that include apiVersion: "2". Test only.
     */
    auto* scp_1 = ([]() -> ServerParameter* {
        auto* ret = makeIDLServerParameterWithStorage<ServerParameterType::kStartupOnly>("acceptApiVersion2", acceptApiVersion2);
        return ret;
    })();
    scp_1->setTestOnly();

}
}  // namespace mongo
