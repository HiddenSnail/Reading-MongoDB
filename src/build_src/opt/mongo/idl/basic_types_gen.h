/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/idl/basic_types_gen.h --output build/opt/mongo/idl/basic_types_gen.cpp src/mongo/idl/basic_types.idl
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
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/stdx/unordered_map.h"
#include "mongo/util/uuid.h"

namespace mongo { class AuthorizationContract; }

namespace mongo {

/**
 * Sort order of case differences.
 */
enum class CollationCaseFirstEnum : std::int32_t {
    kUpper,
    kLower,
    kOff,
};
static constexpr uint32_t kNumCollationCaseFirstEnum = 3;

CollationCaseFirstEnum CollationCaseFirst_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData CollationCaseFirst_serializer(CollationCaseFirstEnum value);

/**
 * Controls the set of characteristics used to compare strings.
 */
enum class CollationStrength : std::int32_t {
    kPrimary = 1,
    kSecondary = 2,
    kTertiary = 3,
    kQuaternary = 4,
    kIdentical = 5,
};
static constexpr uint32_t kNumCollationStrength = 5;

CollationStrength CollationStrength_parse(const IDLParserErrorContext& ctxt, std::int32_t value);
std::int32_t CollationStrength_serializer(CollationStrength value);

/**
 * Whether collation should consider whitespace and punctuation as base characters for purposes of comparison.
 */
enum class CollationAlternateEnum : std::int32_t {
    kNonIgnorable,
    kShifted,
};
static constexpr uint32_t kNumCollationAlternateEnum = 2;

CollationAlternateEnum CollationAlternate_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData CollationAlternate_serializer(CollationAlternateEnum value);

/**
 * Up to which characters are considered ignorable when alternate is "shifted".
 */
enum class CollationMaxVariableEnum : std::int32_t {
    kPunct,
    kSpace,
};
static constexpr uint32_t kNumCollationMaxVariableEnum = 2;

CollationMaxVariableEnum CollationMaxVariable_parse(const IDLParserErrorContext& ctxt, StringData value);
StringData CollationMaxVariable_serializer(CollationMaxVariableEnum value);

/**
 * Shared by commands that reply with just {ok: 1} and no additional information
 */
class OkReply {
public:

    OkReply();

    static OkReply parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
};

/**
 * Error Reply structure shared by all commands
 */
class ErrorReply {
public:
    static constexpr auto kCodeFieldName = "code"_sd;
    static constexpr auto kCodeNameFieldName = "codeName"_sd;
    static constexpr auto kErrmsgFieldName = "errmsg"_sd;
    static constexpr auto kErrorLabelsFieldName = "errorLabels"_sd;
    static constexpr auto kOkFieldName = "ok"_sd;

    ErrorReply();
    ErrorReply(double ok, std::int32_t code, std::string codeName, std::string errmsg);

    static ErrorReply parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    double getOk() const { return _ok; }
    void setOk(double value) { validateOk(value); _ok = std::move(value); _hasOk = true; }
    std::int32_t getCode() const { return _code; }
    void setCode(std::int32_t value) {  _code = std::move(value); _hasCode = true; }
    StringData getCodeName() const { return _codeName; }
    void setCodeName(StringData value) { auto _tmpValue = value.toString();  _codeName = std::move(_tmpValue); _hasCodeName = true; }
    StringData getErrmsg() const { return _errmsg; }
    void setErrmsg(StringData value) { auto _tmpValue = value.toString();  _errmsg = std::move(_tmpValue); _hasErrmsg = true; }
    boost::optional<std::vector<StringData>> getErrorLabels() const { if (_errorLabels.is_initialized()) {
        return transformVector(_errorLabels.get());
    } else {
        return boost::none;
    }
     }
    void setErrorLabels(boost::optional<std::vector<StringData>> value) { if (value.is_initialized()) {
        auto _tmpValue = transformVector(value.get());
        
        _errorLabels = std::move(_tmpValue);
    } else {
        _errorLabels = boost::none;
    }
      }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    void validateOk(double value);
    void validateOk(IDLParserErrorContext& ctxt, double value);

private:
    double _ok;
    std::int32_t _code;
    std::string _codeName;
    std::string _errmsg;
    boost::optional<std::vector<std::string>> _errorLabels;
    bool _hasOk : 1;
    bool _hasCode : 1;
    bool _hasCodeName : 1;
    bool _hasErrmsg : 1;
};

/**
 * Parser for cursor options, for commands with minimal cursor support
 */
class SimpleCursorOptions {
public:
    static constexpr auto kBatchSizeFieldName = "batchSize"_sd;

    SimpleCursorOptions();

    static SimpleCursorOptions parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    boost::optional<std::int64_t> getBatchSize() const { return _batchSize; }
    void setBatchSize(boost::optional<std::int64_t> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get();
        validateBatchSize(_tmpValue);
        _batchSize = std::move(_tmpValue);
    } else {
        _batchSize = boost::none;
    }
      }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    void validateBatchSize(std::int64_t value);
    void validateBatchSize(IDLParserErrorContext& ctxt, std::int64_t value);

private:
    boost::optional<std::int64_t> _batchSize;
};

/**
 * Specifies the default collation for the collection or the view.
 */
class Collation {
public:
    static constexpr auto kAlternateFieldName = "alternate"_sd;
    static constexpr auto kBackwardsFieldName = "backwards"_sd;
    static constexpr auto kCaseFirstFieldName = "caseFirst"_sd;
    static constexpr auto kCaseLevelFieldName = "caseLevel"_sd;
    static constexpr auto kLocaleFieldName = "locale"_sd;
    static constexpr auto kMaxVariableFieldName = "maxVariable"_sd;
    static constexpr auto kNormalizationFieldName = "normalization"_sd;
    static constexpr auto kNumericOrderingFieldName = "numericOrdering"_sd;
    static constexpr auto kStrengthFieldName = "strength"_sd;
    static constexpr auto kVersionFieldName = "version"_sd;

    Collation();
    Collation(std::string locale);

    static Collation parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    StringData getLocale() const { return _locale; }
    void setLocale(StringData value) { auto _tmpValue = value.toString();  _locale = std::move(_tmpValue); _hasLocale = true; }
    bool getCaseLevel() const { return _caseLevel; }
    void setCaseLevel(bool value) {  _caseLevel = std::move(value);  }
    mongo::CollationCaseFirstEnum getCaseFirst() const { return _caseFirst; }
    void setCaseFirst(mongo::CollationCaseFirstEnum value) {  _caseFirst = std::move(value);  }
    std::int32_t getStrength() const { return _strength; }
    void setStrength(std::int32_t value) { validateStrength(value); _strength = std::move(value);  }
    bool getNumericOrdering() const { return _numericOrdering; }
    void setNumericOrdering(bool value) {  _numericOrdering = std::move(value);  }
    mongo::CollationAlternateEnum getAlternate() const { return _alternate; }
    void setAlternate(mongo::CollationAlternateEnum value) {  _alternate = std::move(value);  }
    mongo::CollationMaxVariableEnum getMaxVariable() const { return _maxVariable; }
    void setMaxVariable(mongo::CollationMaxVariableEnum value) {  _maxVariable = std::move(value);  }
    bool getNormalization() const { return _normalization; }
    void setNormalization(bool value) {  _normalization = std::move(value);  }
    const mongo::OptionalBool& getBackwards() const { return _backwards; }
    void setBackwards(mongo::OptionalBool value) {  _backwards = std::move(value);  }
    boost::optional<StringData> getVersion() const { return boost::optional<StringData>{_version}; }
    void setVersion(boost::optional<StringData> value) { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _version = std::move(_tmpValue);
    } else {
        _version = boost::none;
    }
      }
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    void validateStrength(std::int32_t value);
    void validateStrength(IDLParserErrorContext& ctxt, std::int32_t value);

private:
    auto _relopTuple() const {
        return std::tuple(idl::relop::Ordering{_locale}, idl::relop::Ordering{_caseLevel}, idl::relop::Ordering{_caseFirst}, idl::relop::Ordering{_strength}, idl::relop::Ordering{_numericOrdering}, idl::relop::Ordering{_alternate}, idl::relop::Ordering{_maxVariable}, idl::relop::Ordering{_normalization}, idl::relop::Ordering{_backwards}, idl::relop::Ordering{_version});
    }
    friend bool operator==(const Collation& a, const Collation& b) {
        return a._relopTuple() == b._relopTuple();
    }
    friend bool operator!=(const Collation& a, const Collation& b) {
        return a._relopTuple() != b._relopTuple();
    }
    friend bool operator<(const Collation& a, const Collation& b) {
        return a._relopTuple() < b._relopTuple();
    }
    friend bool operator>(const Collation& a, const Collation& b) {
        return a._relopTuple() > b._relopTuple();
    }
    friend bool operator<=(const Collation& a, const Collation& b) {
        return a._relopTuple() <= b._relopTuple();
    }
    friend bool operator>=(const Collation& a, const Collation& b) {
        return a._relopTuple() >= b._relopTuple();
    }

    std::string _locale;
    bool _caseLevel{false};
    mongo::CollationCaseFirstEnum _caseFirst{mongo::CollationCaseFirstEnum::kOff};
    std::int32_t _strength{static_cast<int>(CollationStrength::kTertiary)};
    bool _numericOrdering{false};
    mongo::CollationAlternateEnum _alternate{mongo::CollationAlternateEnum::kNonIgnorable};
    mongo::CollationMaxVariableEnum _maxVariable{mongo::CollationMaxVariableEnum::kPunct};
    bool _normalization{false};
    mongo::OptionalBool _backwards{mongo::OptionalBool()};
    boost::optional<std::string> _version;
    bool _hasLocale : 1;
};

}  // namespace mongo
