/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/repl/rollback_gen.h --output build/opt/mongo/db/repl/rollback_gen.cpp src/mongo/db/repl/rollback.idl
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
#include "mongo/db/repl/optime.h"
#include "mongo/idl/basic_types.h"
#include "mongo/idl/idl_parser.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/util/uuid.h"

namespace mongo {
namespace repl {

/**
 * A document in which the server stores its Rollback ID on disk.
 */
class RollbackID {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kRollbackIdFieldName = "rollbackId"_sd;

    RollbackID();
    RollbackID(std::string _id, std::int32_t rollbackId);

    static RollbackID parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The _id of the document stored in the database
     */
    const StringData get_id() const& { return __id; }
    void get_id() && = delete;
    void set_id(StringData value) & { auto _tmpValue = value.toString();  __id = std::move(_tmpValue); _has_id = true; }

    /**
     * The Rollback ID stored in the document
     */
    std::int32_t getRollbackId() const { return _rollbackId; }
    void setRollbackId(std::int32_t value) & {  _rollbackId = std::move(value); _hasRollbackId = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string __id;
    std::int32_t _rollbackId;
    bool _has_id : 1;
    bool _hasRollbackId : 1;
};

/**
 * A document in which the server stores its rollback progress on disk.
 */
class RollbackProgress {
public:
    static constexpr auto k_idFieldName = "_id"_sd;
    static constexpr auto kApplyUntilFieldName = "applyUntil"_sd;

    RollbackProgress();
    RollbackProgress(std::string _id, mongo::repl::OpTime applyUntil);

    static RollbackProgress parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The _id of the document stored in the database.
     */
    const StringData get_id() const& { return __id; }
    void get_id() && = delete;
    void set_id(StringData value) & { auto _tmpValue = value.toString();  __id = std::move(_tmpValue); _has_id = true; }

    /**
     * The op time that rollback has to reach to complete successfully.
     */
    const mongo::repl::OpTime& getApplyUntil() const { return _applyUntil; }
    void setApplyUntil(mongo::repl::OpTime value) & {  _applyUntil = std::move(value); _hasApplyUntil = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string __id;
    mongo::repl::OpTime _applyUntil;
    bool _has_id : 1;
    bool _hasApplyUntil : 1;
};

}  // namespace repl
}  // namespace mongo
