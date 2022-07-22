/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/apply_ops_gen.h --output build/opt/mongo/db/repl/apply_ops_gen.cpp src/mongo/db/repl/apply_ops.idl
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
namespace repl {

/**
 * A document that represents an applyOps command.
 */
class ApplyOpsCommandInfoBase {
public:
    static constexpr auto kAllowAtomicFieldName = "allowAtomic"_sd;
    static constexpr auto kAlwaysUpsertFieldName = "alwaysUpsert"_sd;
    static constexpr auto kCountFieldName = "count"_sd;
    static constexpr auto kOperationsFieldName = "applyOps"_sd;
    static constexpr auto kPartialTxnFieldName = "partialTxn"_sd;
    static constexpr auto kPreConditionFieldName = "preCondition"_sd;
    static constexpr auto kPrepareFieldName = "prepare"_sd;

    ApplyOpsCommandInfoBase();
    ApplyOpsCommandInfoBase(std::vector<mongo::BSONObj> operations);

    static ApplyOpsCommandInfoBase parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The operations to apply
     */
    const std::vector<mongo::BSONObj>& getOperations() const& { return _operations; }
    void getOperations() && = delete;
    /**
     * If 'allowAtomic' is true, applyOps() determines if all the given operations can be processed inside one WriteUnitOfWork. Set this flag to false to disable this 'atomic' mode.
     */
    bool getAllowAtomic() const { return _allowAtomic; }
    /**
     * Affects update operations. Set 'alwaysUpsert' to false in the command object to never insert a new document if no document matches the query criteria in an update operation.
     */
    bool getAlwaysUpsert() const { return _alwaysUpsert; }
    /**
     * applyOps supports checking the documents of existing collections before proceeding to execute the given operations. This flag is set to true if the 'preCondition' option is provided.
     */
    const boost::optional<std::vector<mongo::BSONObj>>& getPreCondition() const& { return _preCondition; }
    void getPreCondition() && = delete;
    /**
     * Specifies that this operation should be put into a 'prepare' state
     */
    const boost::optional<bool> getPrepare() const& { return _prepare; }
    void getPrepare() && = delete;
    /**
     * Specifies that this applyOps command is part of a multi-statement transaction that has not yet been committed or prepared.
     */
    const boost::optional<bool> getPartialTxn() const& { return _partialTxn; }
    void getPartialTxn() && = delete;
    /**
     * The number of operations contained in this multi-oplog-entry transaction.
     */
    const boost::optional<std::int64_t> getCount() const& { return _count; }
    void getCount() && = delete;
protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::vector<mongo::BSONObj> _operations;
    bool _allowAtomic{true};
    bool _alwaysUpsert{true};
    boost::optional<std::vector<mongo::BSONObj>> _preCondition;
    boost::optional<bool> _prepare;
    boost::optional<bool> _partialTxn;
    boost::optional<std::int64_t> _count;
    bool _hasOperations : 1;
};

}  // namespace repl
}  // namespace mongo
