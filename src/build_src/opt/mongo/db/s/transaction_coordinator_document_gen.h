/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/s/transaction_coordinator_document_gen.h --output build/opt/mongo/db/s/transaction_coordinator_document_gen.cpp src/mongo/db/s/transaction_coordinator_document.idl
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
#include "mongo/crypto/sha256_block.h"
#include "mongo/db/logical_session_id_gen.h"
#include "mongo/db/namespace_string.h"
#include "mongo/db/s/transaction_coordinator_structures.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/s/shard_id.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace txn {

/**
 * An object representing the coordinator's commit decision.
 */
class CoordinatorCommitDecision {
public:
    static constexpr auto kAbortStatusFieldName = "abortStatus"_sd;
    static constexpr auto kCommitTimestampFieldName = "commitTimestamp"_sd;
    static constexpr auto kDecisionFieldName = "decision"_sd;

    CoordinatorCommitDecision();
    CoordinatorCommitDecision(txn::CommitDecision decision);

    static CoordinatorCommitDecision parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * See the values in txn::CommitDecision for details. Can only be commit or abort.
     */
    const txn::CommitDecision& getDecision() const { return _decision; }
    void setDecision(txn::CommitDecision value) & {  _decision = std::move(value); _hasDecision = true; }

    /**
     * If the decision is 'commit', contains the chosen commit timestamp, otherwise it will not be set
     */
    const boost::optional<mongo::Timestamp>& getCommitTimestamp() const& { return _commitTimestamp; }
    void getCommitTimestamp() && = delete;
    void setCommitTimestamp(boost::optional<mongo::Timestamp> value) & {  _commitTimestamp = std::move(value);  }

    /**
     * If the decision is 'abort', contains the reason the shard aborted, otherwise it will not be set
     */
    const boost::optional<::mongo::Status>& getAbortStatus() const& { return _abortStatus; }
    void getAbortStatus() && = delete;
    void setAbortStatus(boost::optional<::mongo::Status> value) & {  _abortStatus = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    txn::CommitDecision _decision;
    boost::optional<mongo::Timestamp> _commitTimestamp;
    boost::optional<::mongo::Status> _abortStatus;
    bool _hasDecision : 1;
};

/**
 * A document used for majority confirming the coordinator's state changes
 */
class TransactionCoordinatorDocument {
public:
    static constexpr auto kDecisionFieldName = "decision"_sd;
    static constexpr auto kIdFieldName = "_id"_sd;
    static constexpr auto kParticipantsFieldName = "participants"_sd;

    TransactionCoordinatorDocument();
    TransactionCoordinatorDocument(mongo::OperationSessionInfo id, std::vector<mongo::ShardId> participants);

    static TransactionCoordinatorDocument parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The sessionId and txnNumber of this transaction.
     */
    const mongo::OperationSessionInfo& getId() const { return _id; }
    mongo::OperationSessionInfo& getId() { return _id; }
    void setId(mongo::OperationSessionInfo value) & {  _id = std::move(value); _hasId = true; }

    /**
     * The list of transaction participants.
     */
    const std::vector<mongo::ShardId>& getParticipants() const& { return _participants; }
    void getParticipants() && = delete;
    void setParticipants(std::vector<mongo::ShardId> value) & {  _participants = std::move(value); _hasParticipants = true; }

    /**
     * The coordinator's decision for the transaction including the decision ('commit' or 'abort') and a commit timestamp (if the decision is 'commit'). Only set if the coordinator has made a decision.
     */
    const boost::optional<mongo::txn::CoordinatorCommitDecision>& getDecision() const& { return _decision; }
    void getDecision() && = delete;
    void setDecision(boost::optional<mongo::txn::CoordinatorCommitDecision> value) & {  _decision = std::move(value);  }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::OperationSessionInfo _id;
    std::vector<mongo::ShardId> _participants;
    boost::optional<mongo::txn::CoordinatorCommitDecision> _decision;
    bool _hasId : 1;
    bool _hasParticipants : 1;
};

}  // namespace txn
}  // namespace mongo
