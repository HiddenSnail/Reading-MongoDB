/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/resumable_index_builds_gen.h --output build/opt/mongo/db/resumable_index_builds_gen.cpp src/mongo/db/resumable_index_builds.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/resumable_index_builds_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/auth/authorization_contract.h"
#include "mongo/db/commands.h"
#include "mongo/idl/command_generic_argument.h"
#include "mongo/util/visit_helper.h"

namespace mongo {

/**
 * Phase of a hybrid index build
 */
namespace  {
constexpr StringData kIndexBuildPhase_kInitialized = "initialized"_sd;
constexpr StringData kIndexBuildPhase_kCollectionScan = "collection scan"_sd;
constexpr StringData kIndexBuildPhase_kBulkLoad = "bulk load"_sd;
constexpr StringData kIndexBuildPhase_kDrainWrites = "drain writes"_sd;
}  // namespace 

IndexBuildPhaseEnum IndexBuildPhase_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kIndexBuildPhase_kInitialized) {
        return IndexBuildPhaseEnum::kInitialized;
    }
    if (value == kIndexBuildPhase_kCollectionScan) {
        return IndexBuildPhaseEnum::kCollectionScan;
    }
    if (value == kIndexBuildPhase_kBulkLoad) {
        return IndexBuildPhaseEnum::kBulkLoad;
    }
    if (value == kIndexBuildPhase_kDrainWrites) {
        return IndexBuildPhaseEnum::kDrainWrites;
    }
    ctxt.throwBadEnumValue(value);
}

StringData IndexBuildPhase_serializer(IndexBuildPhaseEnum value) {
    if (value == IndexBuildPhaseEnum::kInitialized) {
        return kIndexBuildPhase_kInitialized;
    }
    if (value == IndexBuildPhaseEnum::kCollectionScan) {
        return kIndexBuildPhase_kCollectionScan;
    }
    if (value == IndexBuildPhaseEnum::kBulkLoad) {
        return kIndexBuildPhase_kBulkLoad;
    }
    if (value == IndexBuildPhaseEnum::kDrainWrites) {
        return kIndexBuildPhase_kDrainWrites;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

constexpr StringData MultikeyPath::kMultikeyComponentsFieldName;


MultikeyPath::MultikeyPath() : _hasMultikeyComponents(false) {
    // Used for initialization only
}
MultikeyPath::MultikeyPath(std::vector<std::int32_t> multikeyComponents) : _multikeyComponents(std::move(multikeyComponents)), _hasMultikeyComponents(true) {
    // Used for initialization only
}


MultikeyPath MultikeyPath::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<MultikeyPath>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void MultikeyPath::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<1> usedFields;
    const size_t kMultikeyComponentsBit = 0;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kMultikeyComponentsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Array))) {
                if (MONGO_unlikely(usedFields[kMultikeyComponentsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMultikeyComponentsBit);

                _hasMultikeyComponents = true;
            }
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kMultikeyComponentsFieldName, &ctxt);
            std::vector<std::int32_t> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, NumberInt)) {
                        values.emplace_back(arrayElement._numberInt());
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _multikeyComponents = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kMultikeyComponentsBit]) {
            ctxt.throwMissingField(kMultikeyComponentsFieldName);
        }
    }

}


void MultikeyPath::serialize(BSONObjBuilder* builder) const {
    invariant(_hasMultikeyComponents);

    {
        builder->append(kMultikeyComponentsFieldName, _multikeyComponents);
    }

}


BSONObj MultikeyPath::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData IndexStateInfo::kDuplicateKeyTrackerTableFieldName;
constexpr StringData IndexStateInfo::kFileNameFieldName;
constexpr StringData IndexStateInfo::kIsMultikeyFieldName;
constexpr StringData IndexStateInfo::kMultikeyPathsFieldName;
constexpr StringData IndexStateInfo::kNumKeysFieldName;
constexpr StringData IndexStateInfo::kRangesFieldName;
constexpr StringData IndexStateInfo::kSideWritesTableFieldName;
constexpr StringData IndexStateInfo::kSkippedRecordTrackerTableFieldName;
constexpr StringData IndexStateInfo::kSpecFieldName;


IndexStateInfo::IndexStateInfo() : _sideWritesTable(mongo::idl::preparsedValue<decltype(_sideWritesTable)>()), _spec(mongo::idl::preparsedValue<decltype(_spec)>()), _isMultikey(mongo::idl::preparsedValue<decltype(_isMultikey)>()), _hasSideWritesTable(false), _hasSpec(false), _hasIsMultikey(false), _hasMultikeyPaths(false) {
    // Used for initialization only
}
IndexStateInfo::IndexStateInfo(std::string sideWritesTable, mongo::BSONObj spec, bool isMultikey, std::vector<mongo::MultikeyPath> multikeyPaths) : _sideWritesTable(std::move(sideWritesTable)), _spec(std::move(spec)), _isMultikey(std::move(isMultikey)), _multikeyPaths(std::move(multikeyPaths)), _hasSideWritesTable(true), _hasSpec(true), _hasIsMultikey(true), _hasMultikeyPaths(true) {
    // Used for initialization only
}


IndexStateInfo IndexStateInfo::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<IndexStateInfo>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void IndexStateInfo::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<9> usedFields;
    const size_t kSideWritesTableBit = 0;
    const size_t kDuplicateKeyTrackerTableBit = 1;
    const size_t kSkippedRecordTrackerTableBit = 2;
    const size_t kFileNameBit = 3;
    const size_t kNumKeysBit = 4;
    const size_t kRangesBit = 5;
    const size_t kSpecBit = 6;
    const size_t kIsMultikeyBit = 7;
    const size_t kMultikeyPathsBit = 8;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kSideWritesTableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kSideWritesTableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSideWritesTableBit);

                _hasSideWritesTable = true;
                _sideWritesTable = element.str();
            }
        }
        else if (fieldName == kDuplicateKeyTrackerTableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kDuplicateKeyTrackerTableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kDuplicateKeyTrackerTableBit);

                _duplicateKeyTrackerTable = element.str();
            }
        }
        else if (fieldName == kSkippedRecordTrackerTableFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kSkippedRecordTrackerTableBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSkippedRecordTrackerTableBit);

                _skippedRecordTrackerTable = element.str();
            }
        }
        else if (fieldName == kFileNameFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kFileNameBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kFileNameBit);

                _fileName = element.str();
            }
        }
        else if (fieldName == kNumKeysFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, NumberLong))) {
                if (MONGO_unlikely(usedFields[kNumKeysBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kNumKeysBit);

                _numKeys = element._numberLong();
            }
        }
        else if (fieldName == kRangesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Array))) {
                if (MONGO_unlikely(usedFields[kRangesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kRangesBit);

            }
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kRangesFieldName, &ctxt);
            std::vector<mongo::SorterRange> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        IDLParserErrorContext tempContext(kRangesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::SorterRange::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _ranges = std::move(values);
        }
        else if (fieldName == kSpecFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Object))) {
                if (MONGO_unlikely(usedFields[kSpecBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kSpecBit);

                _hasSpec = true;
                const BSONObj localObject = element.Obj();
                _spec = BSONObj::getOwned(localObject);
            }
        }
        else if (fieldName == kIsMultikeyFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Bool))) {
                if (MONGO_unlikely(usedFields[kIsMultikeyBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIsMultikeyBit);

                _hasIsMultikey = true;
                _isMultikey = element.boolean();
            }
        }
        else if (fieldName == kMultikeyPathsFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Array))) {
                if (MONGO_unlikely(usedFields[kMultikeyPathsBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kMultikeyPathsBit);

                _hasMultikeyPaths = true;
            }
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kMultikeyPathsFieldName, &ctxt);
            std::vector<mongo::MultikeyPath> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        IDLParserErrorContext tempContext(kMultikeyPathsFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::MultikeyPath::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _multikeyPaths = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kSideWritesTableBit]) {
            ctxt.throwMissingField(kSideWritesTableFieldName);
        }
        if (!usedFields[kSpecBit]) {
            ctxt.throwMissingField(kSpecFieldName);
        }
        if (!usedFields[kIsMultikeyBit]) {
            ctxt.throwMissingField(kIsMultikeyFieldName);
        }
        if (!usedFields[kMultikeyPathsBit]) {
            ctxt.throwMissingField(kMultikeyPathsFieldName);
        }
    }

}


void IndexStateInfo::serialize(BSONObjBuilder* builder) const {
    invariant(_hasSideWritesTable && _hasSpec && _hasIsMultikey && _hasMultikeyPaths);

    builder->append(kSideWritesTableFieldName, _sideWritesTable);

    if (_duplicateKeyTrackerTable.is_initialized()) {
        builder->append(kDuplicateKeyTrackerTableFieldName, _duplicateKeyTrackerTable.get());
    }

    if (_skippedRecordTrackerTable.is_initialized()) {
        builder->append(kSkippedRecordTrackerTableFieldName, _skippedRecordTrackerTable.get());
    }

    if (_fileName.is_initialized()) {
        builder->append(kFileNameFieldName, _fileName.get());
    }

    if (_numKeys.is_initialized()) {
        builder->append(kNumKeysFieldName, _numKeys.get());
    }

    if (_ranges.is_initialized()) {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kRangesFieldName));
        for (const auto& item : _ranges.get()) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

    builder->append(kSpecFieldName, _spec);

    builder->append(kIsMultikeyFieldName, _isMultikey);

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kMultikeyPathsFieldName));
        for (const auto& item : _multikeyPaths) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

}


BSONObj IndexStateInfo::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

constexpr StringData ResumeIndexInfo::kBuildUUIDFieldName;
constexpr StringData ResumeIndexInfo::kCollectionScanPositionFieldName;
constexpr StringData ResumeIndexInfo::kCollectionUUIDFieldName;
constexpr StringData ResumeIndexInfo::kIndexesFieldName;
constexpr StringData ResumeIndexInfo::kPhaseFieldName;


ResumeIndexInfo::ResumeIndexInfo() : _buildUUID(mongo::idl::preparsedValue<decltype(_buildUUID)>()), _phase(mongo::idl::preparsedValue<decltype(_phase)>()), _collectionUUID(mongo::idl::preparsedValue<decltype(_collectionUUID)>()), _hasBuildUUID(false), _hasPhase(false), _hasCollectionUUID(false), _hasIndexes(false) {
    // Used for initialization only
}
ResumeIndexInfo::ResumeIndexInfo(mongo::UUID buildUUID, mongo::IndexBuildPhaseEnum phase, mongo::UUID collectionUUID, std::vector<mongo::IndexStateInfo> indexes) : _buildUUID(std::move(buildUUID)), _phase(std::move(phase)), _collectionUUID(std::move(collectionUUID)), _indexes(std::move(indexes)), _hasBuildUUID(true), _hasPhase(true), _hasCollectionUUID(true), _hasIndexes(true) {
    // Used for initialization only
}


ResumeIndexInfo ResumeIndexInfo::parse(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    auto object = mongo::idl::preparsedValue<ResumeIndexInfo>();
    object.parseProtected(ctxt, bsonObject);
    return object;
}
void ResumeIndexInfo::parseProtected(const IDLParserErrorContext& ctxt, const BSONObj& bsonObject) {
    std::bitset<5> usedFields;
    const size_t kBuildUUIDBit = 0;
    const size_t kPhaseBit = 1;
    const size_t kCollectionUUIDBit = 2;
    const size_t kCollectionScanPositionBit = 3;
    const size_t kIndexesBit = 4;

    for (const auto& element :bsonObject) {
        const auto fieldName = element.fieldNameStringData();


        if (fieldName == kBuildUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kBuildUUIDBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kBuildUUIDBit);

                _hasBuildUUID = true;
                _buildUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kPhaseFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, String))) {
                if (MONGO_unlikely(usedFields[kPhaseBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kPhaseBit);

                _hasPhase = true;
                IDLParserErrorContext tempContext(kPhaseFieldName, &ctxt);
                _phase = IndexBuildPhase_parse(tempContext, element.valueStringData());
            }
        }
        else if (fieldName == kCollectionUUIDFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertBinDataType(element, newUUID))) {
                if (MONGO_unlikely(usedFields[kCollectionUUIDBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kCollectionUUIDBit);

                _hasCollectionUUID = true;
                _collectionUUID = UUID(uassertStatusOK(UUID::parse(element)));
            }
        }
        else if (fieldName == kCollectionScanPositionFieldName) {
            if (MONGO_unlikely(usedFields[kCollectionScanPositionBit])) {
                ctxt.throwDuplicateField(element);
            }

            usedFields.set(kCollectionScanPositionBit);

            _collectionScanPosition = RecordId::deserializeToken(element);
        }
        else if (fieldName == kIndexesFieldName) {
            if (MONGO_likely(ctxt.checkAndAssertType(element, Array))) {
                if (MONGO_unlikely(usedFields[kIndexesBit])) {
                    ctxt.throwDuplicateField(element);
                }

                usedFields.set(kIndexesBit);

                _hasIndexes = true;
            }
            std::uint32_t expectedFieldNumber{0};
            const IDLParserErrorContext arrayCtxt(kIndexesFieldName, &ctxt);
            std::vector<mongo::IndexStateInfo> values;

            const BSONObj arrayObject = element.Obj();
            for (const auto& arrayElement : arrayObject) {
                const auto arrayFieldName = arrayElement.fieldNameStringData();
                std::uint32_t fieldNumber;

                Status status = NumberParser{}(arrayFieldName, &fieldNumber);
                if (status.isOK()) {
                    if (fieldNumber != expectedFieldNumber) {
                        arrayCtxt.throwBadArrayFieldNumberSequence(fieldNumber, expectedFieldNumber);
                    }

                    if (arrayCtxt.checkAndAssertType(arrayElement, Object)) {
                        IDLParserErrorContext tempContext(kIndexesFieldName, &ctxt);
                        const auto localObject = arrayElement.Obj();
                        values.emplace_back(mongo::IndexStateInfo::parse(tempContext, localObject));
                    }
                }
                else {
                    arrayCtxt.throwBadArrayFieldNumberValue(arrayFieldName);
                }
                ++expectedFieldNumber;
            }
            _indexes = std::move(values);
        }
        else {
            ctxt.throwUnknownField(fieldName);
        }
    }

    if (MONGO_unlikely(!usedFields.all())) {
        if (!usedFields[kBuildUUIDBit]) {
            ctxt.throwMissingField(kBuildUUIDFieldName);
        }
        if (!usedFields[kPhaseBit]) {
            ctxt.throwMissingField(kPhaseFieldName);
        }
        if (!usedFields[kCollectionUUIDBit]) {
            ctxt.throwMissingField(kCollectionUUIDFieldName);
        }
        if (!usedFields[kIndexesBit]) {
            ctxt.throwMissingField(kIndexesFieldName);
        }
    }

}


void ResumeIndexInfo::serialize(BSONObjBuilder* builder) const {
    invariant(_hasBuildUUID && _hasPhase && _hasCollectionUUID && _hasIndexes);

    {
        ConstDataRange tempCDR = _buildUUID.toCDR();
        builder->append(kBuildUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    {
        builder->append(kPhaseFieldName, ::mongo::IndexBuildPhase_serializer(_phase));
    }

    {
        ConstDataRange tempCDR = _collectionUUID.toCDR();
        builder->append(kCollectionUUIDFieldName, BSONBinData(tempCDR.data(), tempCDR.length(), newUUID));
    }

    if (_collectionScanPosition.is_initialized()) {
        _collectionScanPosition.get().serializeToken(kCollectionScanPositionFieldName, builder);
    }

    {
        BSONArrayBuilder arrayBuilder(builder->subarrayStart(kIndexesFieldName));
        for (const auto& item : _indexes) {
            BSONObjBuilder subObjBuilder(arrayBuilder.subobjStart());
            item.serialize(&subObjBuilder);
        }
    }

}


BSONObj ResumeIndexInfo::toBSON() const {
    BSONObjBuilder builder;
    serialize(&builder);
    return builder.obj();
}

}  // namespace mongo
