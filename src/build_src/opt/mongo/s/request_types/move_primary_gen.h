/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/move_primary_gen.h --output build/opt/mongo/s/request_types/move_primary_gen.cpp src/mongo/s/request_types/move_primary.idl
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
 * The public movePrimary command on mongos
 */
class MovePrimary {
public:
    static constexpr auto kMovePrimaryFieldName = "movePrimary"_sd;
    static constexpr auto kMoveprimaryFieldName = "moveprimary"_sd;
    static constexpr auto kToFieldName = "to"_sd;

    MovePrimary();
    MovePrimary(std::string to);

    static MovePrimary parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The namespace of the database whose primary shard is to be reassigned.
     */
    const boost::optional<mongo::NamespaceString>& getMovePrimary() const& { return _movePrimary; }
    void getMovePrimary() && = delete;
    void setMovePrimary(boost::optional<mongo::NamespaceString> value) & {  _movePrimary = std::move(value);  }

    /**
     * The deprecated version of this command's name.
     */
    const boost::optional<mongo::NamespaceString>& getMoveprimary() const& { return _moveprimary; }
    void getMoveprimary() && = delete;
    void setMoveprimary(boost::optional<mongo::NamespaceString> value) & {  _moveprimary = std::move(value);  }

    /**
     * The shard serving as the destination for un-sharded collections.
     */
    const StringData getTo() const& { return _to; }
    void getTo() && = delete;
    void setTo(StringData value) & { auto _tmpValue = value.toString();  _to = std::move(_tmpValue); _hasTo = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::NamespaceString> _movePrimary;
    boost::optional<mongo::NamespaceString> _moveprimary;
    std::string _to;
    bool _hasTo : 1;
};

/**
 * The internal movePrimary command on the config server
 */
class ConfigsvrMovePrimary {
public:
    static constexpr auto k_configsvrMovePrimaryFieldName = "_configsvrMovePrimary"_sd;
    static constexpr auto kToFieldName = "to"_sd;

    ConfigsvrMovePrimary();
    ConfigsvrMovePrimary(mongo::NamespaceString _configsvrMovePrimary, std::string to);

    static ConfigsvrMovePrimary parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The namespace of the database whose primary shard is to be reassigned.
     */
    const mongo::NamespaceString& get_configsvrMovePrimary() const { return __configsvrMovePrimary; }
    void set_configsvrMovePrimary(mongo::NamespaceString value) & {  __configsvrMovePrimary = std::move(value); _has_configsvrMovePrimary = true; }

    /**
     * The shard serving as the destination for un-sharded collections.
     */
    const StringData getTo() const& { return _to; }
    void getTo() && = delete;
    void setTo(StringData value) & { auto _tmpValue = value.toString();  _to = std::move(_tmpValue); _hasTo = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    mongo::NamespaceString __configsvrMovePrimary;
    std::string _to;
    bool _has_configsvrMovePrimary : 1;
    bool _hasTo : 1;
};

/**
 * The internal movePrimary command on a primary shard
 */
class ShardMovePrimary {
public:
    static constexpr auto k_movePrimaryFieldName = "_movePrimary"_sd;
    static constexpr auto k_shardsvrMovePrimaryFieldName = "_shardsvrMovePrimary"_sd;
    static constexpr auto kToFieldName = "to"_sd;

    ShardMovePrimary();
    ShardMovePrimary(std::string to);

    static ShardMovePrimary parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The namespace of the database whose primary shard is to be reassigned.
     */
    const boost::optional<mongo::NamespaceString>& get_shardsvrMovePrimary() const& { return __shardsvrMovePrimary; }
    void get_shardsvrMovePrimary() && = delete;
    void set_shardsvrMovePrimary(boost::optional<mongo::NamespaceString> value) & {  __shardsvrMovePrimary = std::move(value);  }

    /**
     * The deprecated version of this command's name.
     */
    const boost::optional<mongo::NamespaceString>& get_movePrimary() const& { return __movePrimary; }
    void get_movePrimary() && = delete;
    void set_movePrimary(boost::optional<mongo::NamespaceString> value) & {  __movePrimary = std::move(value);  }

    /**
     * The shard serving as the destination for un-sharded collections.
     */
    const StringData getTo() const& { return _to; }
    void getTo() && = delete;
    void setTo(StringData value) & { auto _tmpValue = value.toString();  _to = std::move(_tmpValue); _hasTo = true; }


    friend bool operator==(const ShardMovePrimary& left, const ShardMovePrimary& right) {
        return left.__shardsvrMovePrimary == right.__shardsvrMovePrimary && left.__movePrimary == right.__movePrimary && left._to == right._to;
    }

    friend bool operator!=(const ShardMovePrimary& left, const ShardMovePrimary& right) {
        return left.__shardsvrMovePrimary != right.__shardsvrMovePrimary || left.__movePrimary != right.__movePrimary || left._to != right._to;
    }
    friend bool operator<(const ShardMovePrimary& left, const ShardMovePrimary& right) {
        return left.__shardsvrMovePrimary < right.__shardsvrMovePrimary || (!(right.__shardsvrMovePrimary < left.__shardsvrMovePrimary) && (left.__movePrimary < right.__movePrimary || (!(right.__movePrimary < left.__movePrimary) && (left._to < right._to))));
    }
    friend bool operator>(const ShardMovePrimary& left, const ShardMovePrimary& right) {
        return right < left;
    }
    friend bool operator<=(const ShardMovePrimary& left, const ShardMovePrimary& right) {
        return !(right < left);
    }
    friend bool operator>=(const ShardMovePrimary& left, const ShardMovePrimary& right) {
        return !(left < right);
    }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    boost::optional<mongo::NamespaceString> __shardsvrMovePrimary;
    boost::optional<mongo::NamespaceString> __movePrimary;
    std::string _to;
    bool _hasTo : 1;
};

/**
 * The internal commitMovePrimary command on the config server
 */
class ConfigsvrCommitMovePrimary {
public:
    static constexpr auto k_configsvrCommitMovePrimaryFieldName = "_configsvrCommitMovePrimary"_sd;
    static constexpr auto kToFieldName = "to"_sd;

    ConfigsvrCommitMovePrimary();
    ConfigsvrCommitMovePrimary(std::string _configsvrCommitMovePrimary, std::string to);

    static ConfigsvrCommitMovePrimary parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);
    void serialize(BSONObjBuilder* builder) const;
    BSONObj toBSON() const;

    /**
     * The namespace of the database whose primary shard is to be reassigned.
     */
    const StringData get_configsvrCommitMovePrimary() const& { return __configsvrCommitMovePrimary; }
    void get_configsvrCommitMovePrimary() && = delete;
    void set_configsvrCommitMovePrimary(StringData value) & { auto _tmpValue = value.toString();  __configsvrCommitMovePrimary = std::move(_tmpValue); _has_configsvrCommitMovePrimary = true; }

    /**
     * The shard serving as the destination for un-sharded collections.
     */
    const StringData getTo() const& { return _to; }
    void getTo() && = delete;
    void setTo(StringData value) & { auto _tmpValue = value.toString();  _to = std::move(_tmpValue); _hasTo = true; }

protected:
    void parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject);

private:
    std::string __configsvrCommitMovePrimary;
    std::string _to;
    bool _has_configsvrCommitMovePrimary : 1;
    bool _hasTo : 1;
};

}  // namespace mongo
