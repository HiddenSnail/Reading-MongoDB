/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_merge_gen.h --output build/opt/mongo/db/pipeline/document_source_merge_gen.cpp src/mongo/db/pipeline/document_source_merge.idl
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
#include "mongo/db/pipeline/document_source_merge_spec.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/chunk_version.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A document used to specify a namespace.
 */
class NamespaceSpec {
public:
    static constexpr auto kCollFieldName = "coll"_sd;
    static constexpr auto kDbFieldName = "db"_sd;

    NamespaceSpec();

    static NamespaceSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The database field for this namespace.
     */
    const boost::optional<StringData> getDb() const& { return boost::optional<StringData>{_db}; }
    void getDb() && = delete;
    void setDb(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _db = std::move(_tmpValue);
    } else {
        _db = boost::none;
    }
      }

    /**
     * The collection field for this namespace.
     */
    const boost::optional<StringData> getColl() const& { return boost::optional<StringData>{_coll}; }
    void getColl() && = delete;
    void setColl(boost::optional<StringData> value) & { if (value.is_initialized()) {
        auto _tmpValue = value.get().toString();
        
        _coll = std::move(_tmpValue);
    } else {
        _coll = boost::none;
    }
      }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<std::string> _db;
    boost::optional<std::string> _coll;
};

/**
 * A document used to specify the $merge stage of an aggregation pipeline.
 */
class DocumentSourceMergeSpec {
public:
    static constexpr auto kLetFieldName = "let"_sd;
    static constexpr auto kOnFieldName = "on"_sd;
    static constexpr auto kTargetCollectionVersionFieldName = "targetCollectionVersion"_sd;
    static constexpr auto kTargetNssFieldName = "into"_sd;
    static constexpr auto kWhenMatchedFieldName = "whenMatched"_sd;
    static constexpr auto kWhenNotMatchedFieldName = "whenNotMatched"_sd;

    DocumentSourceMergeSpec();
    DocumentSourceMergeSpec(mongo::NamespaceString targetNss);

    static DocumentSourceMergeSpec parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Target namespace to merge documents from an aggregation pipeline into.
     */
    const mongo::NamespaceString& getTargetNss() const { return _targetNss; }
    void setTargetNss(mongo::NamespaceString value) & {  _targetNss = std::move(value); _hasTargetNss = true; }

    /**
     * A single field or array of fields that uniquely identify a document.
     */
    const boost::optional<std::vector<std::string>>& getOn() const& { return _on; }
    void getOn() && = delete;
    void setOn(boost::optional<std::vector<std::string>> value) & {  _on = std::move(value);  }

    /**
     * Specifies variables to use in the update pipeline defined in MergeWhenMatchedPolicy when the 'whenMatched' mode is a custom pipeline.
     */
    const boost::optional<mongo::BSONObj>& getLet() const& { return _let; }
    void getLet() && = delete;
    void setLet(boost::optional<mongo::BSONObj> value) & {  _let = std::move(value);  }

    /**
     * The merge mode for the merge operation when source and target elements match.
     */
    const boost::optional<::mongo::MergeWhenMatchedPolicy>& getWhenMatched() const& { return _whenMatched; }
    void getWhenMatched() && = delete;
    void setWhenMatched(boost::optional<::mongo::MergeWhenMatchedPolicy> value) & {  _whenMatched = std::move(value);  }

    /**
     * The merge mode for the merge operation when source and target elements do not match.
     */
    const boost::optional<mongo::MergeWhenNotMatchedModeEnum> getWhenNotMatched() const& { return _whenNotMatched; }
    void getWhenNotMatched() && = delete;
    void setWhenNotMatched(boost::optional<mongo::MergeWhenNotMatchedModeEnum> value) & {  _whenNotMatched = std::move(value);  }

    /**
     * If set, the collection's ChunkVersion found when parsed on mongos. Can be used to check if a collection has since been dropped and re-created, in which case the shard key may have changed, or had its shard key refined. This also can be used to detect if the collection has gone from unsharded to sharded, and thus now has a shard key.
     */
    const boost::optional<mongo::ChunkVersion>& getTargetCollectionVersion() const& { return _targetCollectionVersion; }
    void getTargetCollectionVersion() && = delete;
    void setTargetCollectionVersion(boost::optional<mongo::ChunkVersion> value) & {  _targetCollectionVersion = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::NamespaceString _targetNss;
    boost::optional<std::vector<std::string>> _on;
    boost::optional<mongo::BSONObj> _let;
    boost::optional<::mongo::MergeWhenMatchedPolicy> _whenMatched;
    boost::optional<mongo::MergeWhenNotMatchedModeEnum> _whenNotMatched;
    boost::optional<mongo::ChunkVersion> _targetCollectionVersion;
    bool _hasTargetNss : 1;
};

}  // namespace mongo
