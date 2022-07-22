/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/catalog/index_build_entry_gen.h --output build/opt/mongo/db/catalog/index_build_entry_gen.cpp src/mongo/db/catalog/index_build_entry.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/catalog/index_build_entry_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

constexpr StringData IndexBuildEntry::kBuildUUIDFieldName;
constexpr StringData IndexBuildEntry::kCollectionUUIDFieldName;
constexpr StringData IndexBuildEntry::kCommitQuorumFieldName;
constexpr StringData IndexBuildEntry::kCommitReadyMembersFieldName;
constexpr StringData IndexBuildEntry::kIndexNamesFieldName;


IndexBuildEntry::IndexBuildEntry() : _hasBuildUUID(false), _hasCollectionUUID(false), _hasCommitQuorum(false), _hasIndexNames(false) {
    // Used for initialization only
}
IndexBuildEntry::IndexBuildEntry(mongo::UUID buildUUID, mongo::UUID collectionUUID, mongo::CommitQuorumOptions commitQuorum, std::vector<std::string> indexNames) : _buildUUID(std::move(buildUUID)), _collectionUUID(std::move(collectionUUID)), _commitQuorum(std::move(commitQuorum)), _indexNames(std::move(indexNames)), _hasBuildUUID(true), _hasCollectionUUID(true), _hasCommitQuorum(true), _hasIndexNames(true) {
    // Used for initialization only
}


IndexBuildEntry IndexBuildEntry::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    IndexBuildEntry object;
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void IndexBuildEntry::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::set<StringData> usedFields;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();

        auto push_result = usedFields.insert(fieldName);
        if (MONGO_unlikely(push_result.second == false)) {
            ctxt.throwDuplicateField(fieldName);
        }

        if (fieldName == kBuildUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _hasBuildUUID = true;
                _buildUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kCollectionUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                _hasCollectionUUID = true;
                _collectionUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kCommitQuorumFieldName) {
            _hasCommitQuorum = true;
            _commitQuorum = CommitQuorumOptions::deserializerForIDL(element);
        }
        else if (fieldName == kIndexNamesFieldName) {
            _hasIndexNames = true;
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kIndexNamesFieldName, &ctxt);
            std::vector<std::string> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, String)) {
                        values.emplace_back(arrayElement.str());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _indexNames = std::move(values);
        }
        else if (fieldName == kCommitReadyMembersFieldName) {
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kCommitReadyMembersFieldName, &ctxt);
            std::vector<mongo::HostAndPort> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, String)) {
                        values.emplace_back(HostAndPort::parseThrowing(arrayElement.valueStringData()));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _commitReadyMembers = std::move(values);
        }
    }

    if (MONGO_unlikely(usedFields.find(kBuildUUIDFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kBuildUUIDFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kCollectionUUIDFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCollectionUUIDFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kCommitQuorumFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kCommitQuorumFieldName);
    }
    if (MONGO_unlikely(usedFields.find(kIndexNamesFieldName) == usedFields.end())) {
        ctxt.throwMissingField(kIndexNamesFieldName);
    }

}


void IndexBuildEntry::serialize(BSONObjBuilder* builder) const {
    invariant(_hasBuildUUID && _hasCollectionUUID && _hasCommitQuorum && _hasIndexNames);

    {
        ConstDataRange tempCDR = _buildUUID.toCDR();
        builder->append(kBuildUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        ConstDataRange tempCDR = _collectionUUID.toCDR();
        builder->append(kCollectionUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        _commitQuorum.appendToBuilder(kCommitQuorumFieldName, builder);
    }

    {
        builder->append(kIndexNamesFieldName, _indexNames);
    }

    if (_commitReadyMembers.is_initialized()) {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kCommitReadyMembersFieldName));
        for (const auto& item : _commitReadyMembers.get()) {
            arrayBuilder.append(item.toString());
        }
    }

}


BSONObj IndexBuildEntry::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
