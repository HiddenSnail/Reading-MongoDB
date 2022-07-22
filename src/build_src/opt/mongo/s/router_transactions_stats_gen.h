/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/router_transactions_stats_gen.h --output build/opt/mongo/s/router_transactions_stats_gen.cpp src/mongo/s/router_transactions_stats.idl
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
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {

/**
 * Contains metrics for a type of commit
 */
class CommitTypeStats {
public:
    static constexpr auto kInitiatedFieldName = "initiated"_sd;
    static constexpr auto kSuccessfulFieldName = "successful"_sd;
    static constexpr auto kSuccessfulDurationMicrosFieldName = "successfulDurationMicros"_sd;

    CommitTypeStats();

    static CommitTypeStats parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getInitiated() const { return _initiated; }
    void setInitiated(std::int64_t value) & {  _initiated = std::move(value);  }

    std::int64_t getSuccessful() const { return _successful; }
    void setSuccessful(std::int64_t value) & {  _successful = std::move(value);  }

    std::int64_t getSuccessfulDurationMicros() const { return _successfulDurationMicros; }
    void setSuccessfulDurationMicros(std::int64_t value) & {  _successfulDurationMicros = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _initiated{0};
    std::int64_t _successful{0};
    std::int64_t _successfulDurationMicros{0};
};

/**
 * Contains metrics for each type of commit
 */
class CommitTypes {
public:
    static constexpr auto kNoShardsFieldName = "noShards"_sd;
    static constexpr auto kReadOnlyFieldName = "readOnly"_sd;
    static constexpr auto kRecoverWithTokenFieldName = "recoverWithToken"_sd;
    static constexpr auto kSingleShardFieldName = "singleShard"_sd;
    static constexpr auto kSingleWriteShardFieldName = "singleWriteShard"_sd;
    static constexpr auto kTwoPhaseCommitFieldName = "twoPhaseCommit"_sd;

    CommitTypes();
    CommitTypes(mongo::CommitTypeStats noShards, mongo::CommitTypeStats singleShard, mongo::CommitTypeStats singleWriteShard, mongo::CommitTypeStats readOnly, mongo::CommitTypeStats twoPhaseCommit, mongo::CommitTypeStats recoverWithToken);

    static CommitTypes parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    const mongo::CommitTypeStats& getNoShards() const { return _noShards; }
    mongo::CommitTypeStats& getNoShards() { return _noShards; }
    void setNoShards(mongo::CommitTypeStats value) & {  _noShards = std::move(value); _hasNoShards = true; }

    const mongo::CommitTypeStats& getSingleShard() const { return _singleShard; }
    mongo::CommitTypeStats& getSingleShard() { return _singleShard; }
    void setSingleShard(mongo::CommitTypeStats value) & {  _singleShard = std::move(value); _hasSingleShard = true; }

    const mongo::CommitTypeStats& getSingleWriteShard() const { return _singleWriteShard; }
    mongo::CommitTypeStats& getSingleWriteShard() { return _singleWriteShard; }
    void setSingleWriteShard(mongo::CommitTypeStats value) & {  _singleWriteShard = std::move(value); _hasSingleWriteShard = true; }

    const mongo::CommitTypeStats& getReadOnly() const { return _readOnly; }
    mongo::CommitTypeStats& getReadOnly() { return _readOnly; }
    void setReadOnly(mongo::CommitTypeStats value) & {  _readOnly = std::move(value); _hasReadOnly = true; }

    const mongo::CommitTypeStats& getTwoPhaseCommit() const { return _twoPhaseCommit; }
    mongo::CommitTypeStats& getTwoPhaseCommit() { return _twoPhaseCommit; }
    void setTwoPhaseCommit(mongo::CommitTypeStats value) & {  _twoPhaseCommit = std::move(value); _hasTwoPhaseCommit = true; }

    const mongo::CommitTypeStats& getRecoverWithToken() const { return _recoverWithToken; }
    mongo::CommitTypeStats& getRecoverWithToken() { return _recoverWithToken; }
    void setRecoverWithToken(mongo::CommitTypeStats value) & {  _recoverWithToken = std::move(value); _hasRecoverWithToken = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::CommitTypeStats _noShards;
    mongo::CommitTypeStats _singleShard;
    mongo::CommitTypeStats _singleWriteShard;
    mongo::CommitTypeStats _readOnly;
    mongo::CommitTypeStats _twoPhaseCommit;
    mongo::CommitTypeStats _recoverWithToken;
    bool _hasNoShards : 1;
    bool _hasSingleShard : 1;
    bool _hasSingleWriteShard : 1;
    bool _hasReadOnly : 1;
    bool _hasTwoPhaseCommit : 1;
    bool _hasRecoverWithToken : 1;
};

/**
 * A struct representing the section of the server status command on mongos with information about sharded transactions
 */
class RouterTransactionsStats {
public:
    static constexpr auto kAbortCauseFieldName = "abortCause"_sd;
    static constexpr auto kCommitTypesFieldName = "commitTypes"_sd;
    static constexpr auto kCurrentActiveFieldName = "currentActive"_sd;
    static constexpr auto kCurrentInactiveFieldName = "currentInactive"_sd;
    static constexpr auto kCurrentOpenFieldName = "currentOpen"_sd;
    static constexpr auto kTotalAbortedFieldName = "totalAborted"_sd;
    static constexpr auto kTotalCommittedFieldName = "totalCommitted"_sd;
    static constexpr auto kTotalContactedParticipantsFieldName = "totalContactedParticipants"_sd;
    static constexpr auto kTotalParticipantsAtCommitFieldName = "totalParticipantsAtCommit"_sd;
    static constexpr auto kTotalRequestsTargetedFieldName = "totalRequestsTargeted"_sd;
    static constexpr auto kTotalStartedFieldName = "totalStarted"_sd;

    RouterTransactionsStats();
    RouterTransactionsStats(mongo::BSONObj abortCause, mongo::CommitTypes commitTypes);

    static RouterTransactionsStats parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    std::int64_t getCurrentOpen() const { return _currentOpen; }
    void setCurrentOpen(std::int64_t value) & {  _currentOpen = std::move(value);  }

    std::int64_t getCurrentActive() const { return _currentActive; }
    void setCurrentActive(std::int64_t value) & {  _currentActive = std::move(value);  }

    std::int64_t getCurrentInactive() const { return _currentInactive; }
    void setCurrentInactive(std::int64_t value) & {  _currentInactive = std::move(value);  }

    std::int64_t getTotalStarted() const { return _totalStarted; }
    void setTotalStarted(std::int64_t value) & {  _totalStarted = std::move(value);  }

    std::int64_t getTotalCommitted() const { return _totalCommitted; }
    void setTotalCommitted(std::int64_t value) & {  _totalCommitted = std::move(value);  }

    std::int64_t getTotalAborted() const { return _totalAborted; }
    void setTotalAborted(std::int64_t value) & {  _totalAborted = std::move(value);  }

    const mongo::BSONObj& getAbortCause() const { return _abortCause; }
    void setAbortCause(mongo::BSONObj value) & {  _abortCause = std::move(value); _hasAbortCause = true; }

    std::int64_t getTotalContactedParticipants() const { return _totalContactedParticipants; }
    void setTotalContactedParticipants(std::int64_t value) & {  _totalContactedParticipants = std::move(value);  }

    std::int64_t getTotalParticipantsAtCommit() const { return _totalParticipantsAtCommit; }
    void setTotalParticipantsAtCommit(std::int64_t value) & {  _totalParticipantsAtCommit = std::move(value);  }

    std::int64_t getTotalRequestsTargeted() const { return _totalRequestsTargeted; }
    void setTotalRequestsTargeted(std::int64_t value) & {  _totalRequestsTargeted = std::move(value);  }

    const mongo::CommitTypes& getCommitTypes() const { return _commitTypes; }
    mongo::CommitTypes& getCommitTypes() { return _commitTypes; }
    void setCommitTypes(mongo::CommitTypes value) & {  _commitTypes = std::move(value); _hasCommitTypes = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::int64_t _currentOpen{0};
    std::int64_t _currentActive{0};
    std::int64_t _currentInactive{0};
    std::int64_t _totalStarted{0};
    std::int64_t _totalCommitted{0};
    std::int64_t _totalAborted{0};
    mongo::BSONObj _abortCause;
    std::int64_t _totalContactedParticipants{0};
    std::int64_t _totalParticipantsAtCommit{0};
    std::int64_t _totalRequestsTargeted{0};
    mongo::CommitTypes _commitTypes;
    bool _hasAbortCause : 1;
    bool _hasCommitTypes : 1;
};

}  // namespace mongo
