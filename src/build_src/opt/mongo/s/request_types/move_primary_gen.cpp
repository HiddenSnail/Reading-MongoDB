/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/s/request_types/move_primary_gen.h --output build/opt/mongo/s/request_types/move_primary_gen.cpp src/mongo/s/request_types/move_primary.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/s/request_types/move_primary_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData MovePrimary::kMovePrimaryFieldName;
constexpr StringData MovePrimary::kMoveprimaryFieldName;
constexpr StringData MovePrimary::kToFieldName;


MovePrimary::MovePrimary() : _hasTo(false) {
    // Used for initialization only
}
MovePrimary::MovePrimary(std::string to) : _to(std::move(to)), _hasTo(true) {
    // Used for initialization only
}


MovePrimary MovePrimary::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    MovePrimary object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void MovePrimary::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kMovePrimaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _movePrimary = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kMoveprimaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _moveprimary = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasTo = true;
                _to = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kToFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kToFieldName);
    }

}


void MovePrimary::serialize(BSONObjBuilder* builder) const {
    invariant(_hasTo);

    if (_movePrimary.is_initialized()) {
        builder->append(kMovePrimaryFieldName, _movePrimary.get().toString());
    }

    if (_moveprimary.is_initialized()) {
        builder->append(kMoveprimaryFieldName, _moveprimary.get().toString());
    }

    builder->append(kToFieldName, _to);

}


BSONObj MovePrimary::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ConfigsvrMovePrimary::k_configsvrMovePrimaryFieldName;
constexpr StringData ConfigsvrMovePrimary::kToFieldName;


ConfigsvrMovePrimary::ConfigsvrMovePrimary() : _has_configsvrMovePrimary(false), _hasTo(false) {
    // Used for initialization only
}
ConfigsvrMovePrimary::ConfigsvrMovePrimary(mongo::NamespaceString _configsvrMovePrimary, std::string to) : __configsvrMovePrimary(std::move(_configsvrMovePrimary)), _to(std::move(to)), _has_configsvrMovePrimary(true), _hasTo(true) {
    // Used for initialization only
}


ConfigsvrMovePrimary ConfigsvrMovePrimary::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ConfigsvrMovePrimary object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrMovePrimary::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_configsvrMovePrimaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _has_configsvrMovePrimary = true;
                __configsvrMovePrimary = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasTo = true;
                _to = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(k_configsvrMovePrimaryFieldName) == usedFields.end())) {
        ctxt.throwMissingField(k_configsvrMovePrimaryFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kToFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kToFieldName);
    }

}


void ConfigsvrMovePrimary::serialize(BSONObjBuilder* builder) const {
    invariant(_has_configsvrMovePrimary && _hasTo);

    {
        builder->append(k_configsvrMovePrimaryFieldName, __configsvrMovePrimary.toString());
    }

    builder->append(kToFieldName, _to);

}


BSONObj ConfigsvrMovePrimary::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ShardMovePrimary::k_movePrimaryFieldName;
constexpr StringData ShardMovePrimary::k_shardsvrMovePrimaryFieldName;
constexpr StringData ShardMovePrimary::kToFieldName;


ShardMovePrimary::ShardMovePrimary() : _hasTo(false) {
    // Used for initialization only
}
ShardMovePrimary::ShardMovePrimary(std::string to) : _to(std::move(to)), _hasTo(true) {
    // Used for initialization only
}


ShardMovePrimary ShardMovePrimary::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ShardMovePrimary object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ShardMovePrimary::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_shardsvrMovePrimaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                __shardsvrMovePrimary = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == k_movePrimaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                __movePrimary = NamespaceString(element.valueStringData());
            }
        }
        else if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasTo = true;
                _to = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(kToFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kToFieldName);
    }

}


void ShardMovePrimary::serialize(BSONObjBuilder* builder) const {
    invariant(_hasTo);

    if (__shardsvrMovePrimary.is_initialized()) {
        builder->append(k_shardsvrMovePrimaryFieldName, __shardsvrMovePrimary.get().toString());
    }

    if (__movePrimary.is_initialized()) {
        builder->append(k_movePrimaryFieldName, __movePrimary.get().toString());
    }

    builder->append(kToFieldName, _to);

}


BSONObj ShardMovePrimary::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ConfigsvrCommitMovePrimary::k_configsvrCommitMovePrimaryFieldName;
constexpr StringData ConfigsvrCommitMovePrimary::kToFieldName;


ConfigsvrCommitMovePrimary::ConfigsvrCommitMovePrimary() : _has_configsvrCommitMovePrimary(false), _hasTo(false) {
    // Used for initialization only
}
ConfigsvrCommitMovePrimary::ConfigsvrCommitMovePrimary(std::string _configsvrCommitMovePrimary, std::string to) : __configsvrCommitMovePrimary(std::move(_configsvrCommitMovePrimary)), _to(std::move(to)), _has_configsvrCommitMovePrimary(true), _hasTo(true) {
    // Used for initialization only
}


ConfigsvrCommitMovePrimary ConfigsvrCommitMovePrimary::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    ConfigsvrCommitMovePrimary object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ConfigsvrCommitMovePrimary::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == k_configsvrCommitMovePrimaryFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _has_configsvrCommitMovePrimary = true;
                __configsvrCommitMovePrimary = element.str();
            }
        }
        else if (fieldName == kToFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                _hasTo = true;
                _to = element.str();
            }
        }
    }

    if (MONGO_unlikely(usedFields.find(k_configsvrCommitMovePrimaryFieldName) == usedFields.end())) {
        ctxt.throwMissingField(k_configsvrCommitMovePrimaryFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kToFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kToFieldName);
    }

}


void ConfigsvrCommitMovePrimary::serialize(BSONObjBuilder* builder) const {
    invariant(_has_configsvrCommitMovePrimary && _hasTo);

    builder->append(k_configsvrCommitMovePrimaryFieldName, __configsvrCommitMovePrimary);

    builder->append(kToFieldName, _to);

}


BSONObj ConfigsvrCommitMovePrimary::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
