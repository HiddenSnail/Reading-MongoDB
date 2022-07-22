/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/index_build_entry_gen.h --output build/opt/mongo/db/catalog/index_build_entry_gen.cpp src/mongo/db/catalog/index_build_entry.idl
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
#include "mongo/db/catalog/commit_quorum_options.h"
#include "mongo/db/namespace_string.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/net/hostandport.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * A document that tracks the progress of an index build across replica set members.
 */
class IndexBuildEntry {
public:
    static constexpr auto kBuildUUIDFieldName = "_id"_sd;
    static constexpr auto kCollectionUUIDFieldName = "collectionUUID"_sd;
    static constexpr auto kCommitQuorumFieldName = "commitQuorum"_sd;
    static constexpr auto kCommitReadyMembersFieldName = "commitReadyMembers"_sd;
    static constexpr auto kIndexNamesFieldName = "indexNames"_sd;

    IndexBuildEntry();
    IndexBuildEntry(mongo::UUID buildUUID, mongo::UUID collectionUUID, mongo::CommitQuorumOptions commitQuorum, std::vector<std::string> indexNames);

    static IndexBuildEntry parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * Unique identifier of an index build across replica set members.
     */
    const mongo::UUID& getBuildUUID() const { return _buildUUID; }
    void setBuildUUID(mongo::UUID value) & {  _buildUUID = std::move(value); _hasBuildUUID = true; }

    /**
     * Identifies the collection upon which the index is being built.
     */
    const mongo::UUID& getCollectionUUID() const { return _collectionUUID; }
    void setCollectionUUID(mongo::UUID value) & {  _collectionUUID = std::move(value); _hasCollectionUUID = true; }

    /**
     * commitQuorum can be set to the same values as writeConcern.w and indicates how many and/or which replica set members are needed for the primary to commit the index build.
     */
    const mongo::CommitQuorumOptions& getCommitQuorum() const { return _commitQuorum; }
    void setCommitQuorum(mongo::CommitQuorumOptions value) & {  _commitQuorum = std::move(value); _hasCommitQuorum = true; }

    /**
     * An array of index names associated with this particular cross replica set index build.
     */
    const std::vector<StringData> getIndexNames() const& { return transformVector(_indexNames); }
    void getIndexNames() && = delete;
    void setIndexNames(std::vector<StringData> value) & { auto _tmpValue = transformVector(value);  _indexNames = std::move(_tmpValue); _hasIndexNames = true; }

    /**
     * List of replica set members, by their host and port, that are in the commit phase waiting for the primary to write the commitIndexBuild oplog entry.
     */
    const boost::optional<std::vector<mongo::HostAndPort>>& getCommitReadyMembers() const& { return _commitReadyMembers; }
    void getCommitReadyMembers() && = delete;
    void setCommitReadyMembers(boost::optional<std::vector<mongo::HostAndPort>> value) & {  _commitReadyMembers = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::UUID _buildUUID;
    mongo::UUID _collectionUUID;
    mongo::CommitQuorumOptions _commitQuorum;
    std::vector<std::string> _indexNames;
    boost::optional<std::vector<mongo::HostAndPort>> _commitReadyMembers;
    bool _hasBuildUUID : 1;
    bool _hasCollectionUUID : 1;
    bool _hasCommitQuorum : 1;
    bool _hasIndexNames : 1;
};

}  // namespace mongo
