/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/query/count_command_gen.h --output build/opt/mongo/db/query/count_command_gen.cpp src/mongo/db/query/count_command.idl
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
#include "mongo/db/query/count_request.h"
#include "mongo/db/query/hint_parser.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Parser for the 'count' command.
 */
class CountCommand {
public:
    static constexpr auto kCollationFieldName = "collation"_sd;
    static constexpr auto kCommentFieldName = "comment"_sd;
    static constexpr auto kDbNameFieldName = "$db"_sd;
    static constexpr auto kFieldsFieldName = "fields"_sd;
    static constexpr auto kHintFieldName = "hint"_sd;
    static constexpr auto kLimitFieldName = "limit"_sd;
    static constexpr auto kMaxTimeMSFieldName = "maxTimeMS"_sd;
    static constexpr auto kQueryFieldName = "query"_sd;
    static constexpr auto kQueryOptionsFieldName = "$queryOptions"_sd;
    static constexpr auto kReadConcernFieldName = "readConcern"_sd;
    static constexpr auto kSkipFieldName = "skip"_sd;
    static constexpr auto kCommandName = "count"_sd;

    explicit CountCommand(const NamespaceStringOrUUID nssOrUUID);

    static CountCommand parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    static CountCommand parse(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);
    void serialize(const BSONObj& commandPassthroughFields, BSONObjBuilder* builder) const;
    OpMsgRequest serialize(const BSONObj& commandPassthroughFields) const;
    BSONObj toBSON(const BSONObj& commandPassthroughFields) const;

    const NamespaceStringOrUUID& getNamespaceOrUUID() const { return _nssOrUUID; }

    /**
     * A query that selects which documents to count in the collection or view.
     */
    const mongo::BSONObj& getQuery() const { return _query; }
    void setQuery(mongo::BSONObj value) & {  _query = std::move(value);  }

    /**
     * The maximum number of matching documents to count.
     */
    const boost::optional<std::int64_t> getLimit() const& { return _limit; }
    void getLimit() && = delete;
    void setLimit(boost::optional<std::int64_t> value) & {  _limit = std::move(value);  }

    /**
     * The number of matching documents to skip before counting subsequent results.
     */
    const boost::optional<std::int64_t> getSkip() const& { return _skip; }
    void getSkip() && = delete;
    void setSkip(boost::optional<std::int64_t> value) & {  _skip = std::move(value);  }

    /**
     * The index name to use or the index specification document.
     */
    const mongo::BSONObj& getHint() const { return _hint; }
    void setHint(mongo::BSONObj value) & {  _hint = std::move(value);  }

    /**
     * The collation to use in the count command.
     */
    const boost::optional<mongo::BSONObj>& getCollation() const& { return _collation; }
    void getCollation() && = delete;
    void setCollation(boost::optional<mongo::BSONObj> value) & {  _collation = std::move(value);  }

    /**
     * A comment.
     */
    const boost::optional<StringData> getComment() const& { return boost::optional<StringData>{_comment}; }
    void getComment() && = delete;
    void setComment(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _comment = std::move(_tmpValue);
    } else {
        _comment = boost::none;
    }
      }

    /**
     * A BSONObj read concern.
     */
    const boost::optional<mongo::BSONObj>& getReadConcern() const& { return _readConcern; }
    void getReadConcern() && = delete;
    void setReadConcern(boost::optional<mongo::BSONObj> value) & {  _readConcern = std::move(value);  }

    const boost::optional<std::int64_t> getMaxTimeMS() const& { return _maxTimeMS; }
    void getMaxTimeMS() && = delete;
    void setMaxTimeMS(boost::optional<std::int64_t> value) & {  _maxTimeMS = std::move(value);  }

    /**
     * Unwrapped read preference.
     */
    const boost::optional<mongo::BSONObj>& getQueryOptions() const& { return _queryOptions; }
    void getQueryOptions() && = delete;
    void setQueryOptions(boost::optional<mongo::BSONObj> value) & {  _queryOptions = std::move(value);  }

    const StringData getDbName() const& { return _dbName; }
    void getDbName() && = delete;
    void setDbName(StringData value) & { auto _tmpValue = value.toString();  _dbName = std::move(_tmpValue); _hasDbName = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void parseProtected(const IDLParserErrorContext& ctxt, const OpMsgRequest& request);

private:
    static const std::vector<StringData> _knownBSONFields;
    static const std::vector<StringData> _knownOP_MSGFields;


    NamespaceStringOrUUID _nssOrUUID;

    mongo::BSONObj _query{BSONObj()};
    boost::optional<std::int64_t> _limit;
    boost::optional<std::int64_t> _skip;
    mongo::BSONObj _hint{BSONObj()};
    boost::optional<mongo::BSONObj> _collation;
    boost::optional<std::string> _comment;
    boost::optional<mongo::BSONObj> _readConcern;
    boost::optional<std::int64_t> _maxTimeMS;
    boost::optional<mongo::BSONObj> _queryOptions;
    std::string _dbName;
    bool _hasDbName : 1;
};

}  // namespace mongo
