/**
 * WARNING: This is a generated file. Do not modify.
 *
 * Source: buildscripts/idl/idlc.py --include src --base_dir build/opt --target_arch x86_64 --header build/opt/mongo/db/pipeline/document_source_merge_modes_gen.h --output build/opt/mongo/db/pipeline/document_source_merge_modes_gen.cpp src/mongo/db/pipeline/document_source_merge_modes.idl
 */

#include "mongo/platform/basic.h"

#include "mongo/db/pipeline/document_source_merge_modes_gen.h"

#include <bitset>
#include <set>

#include "mongo/bson/bsonobjbuilder.h"
#include "mongo/db/command_generic_argument.h"
#include "mongo/db/commands.h"

namespace mongo {

/**
 * Possible merge mode values for $merge's 'whenMatched' field.
 */
namespace  {
constexpr StringData kMergeWhenMatchedMode_kFail = "fail"_sd;
constexpr StringData kMergeWhenMatchedMode_kKeepExisting = "keepExisting"_sd;
constexpr StringData kMergeWhenMatchedMode_kMerge = "merge"_sd;
constexpr StringData kMergeWhenMatchedMode_kPipeline = "pipeline"_sd;
constexpr StringData kMergeWhenMatchedMode_kReplace = "replace"_sd;
}  // namespace 

MergeWhenMatchedModeEnum MergeWhenMatchedMode_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kMergeWhenMatchedMode_kFail) {
        return MergeWhenMatchedModeEnum::kFail;
    }
    if (value == kMergeWhenMatchedMode_kKeepExisting) {
        return MergeWhenMatchedModeEnum::kKeepExisting;
    }
    if (value == kMergeWhenMatchedMode_kMerge) {
        return MergeWhenMatchedModeEnum::kMerge;
    }
    if (value == kMergeWhenMatchedMode_kPipeline) {
        return MergeWhenMatchedModeEnum::kPipeline;
    }
    if (value == kMergeWhenMatchedMode_kReplace) {
        return MergeWhenMatchedModeEnum::kReplace;
    }
    ctxt.throwBadEnumValue(value);
}

StringData MergeWhenMatchedMode_serializer(MergeWhenMatchedModeEnum value) {
    if (value == MergeWhenMatchedModeEnum::kFail) {
        return kMergeWhenMatchedMode_kFail;
    }
    if (value == MergeWhenMatchedModeEnum::kKeepExisting) {
        return kMergeWhenMatchedMode_kKeepExisting;
    }
    if (value == MergeWhenMatchedModeEnum::kMerge) {
        return kMergeWhenMatchedMode_kMerge;
    }
    if (value == MergeWhenMatchedModeEnum::kPipeline) {
        return kMergeWhenMatchedMode_kPipeline;
    }
    if (value == MergeWhenMatchedModeEnum::kReplace) {
        return kMergeWhenMatchedMode_kReplace;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

/**
 * Possible merge mode values for $merge's 'whenNotMatched'. field
 */
namespace  {
constexpr StringData kMergeWhenNotMatchedMode_kDiscard = "discard"_sd;
constexpr StringData kMergeWhenNotMatchedMode_kFail = "fail"_sd;
constexpr StringData kMergeWhenNotMatchedMode_kInsert = "insert"_sd;
}  // namespace 

MergeWhenNotMatchedModeEnum MergeWhenNotMatchedMode_parse(const IDLParserErrorContext& ctxt, StringData value) {
    if (value == kMergeWhenNotMatchedMode_kDiscard) {
        return MergeWhenNotMatchedModeEnum::kDiscard;
    }
    if (value == kMergeWhenNotMatchedMode_kFail) {
        return MergeWhenNotMatchedModeEnum::kFail;
    }
    if (value == kMergeWhenNotMatchedMode_kInsert) {
        return MergeWhenNotMatchedModeEnum::kInsert;
    }
    ctxt.throwBadEnumValue(value);
}

StringData MergeWhenNotMatchedMode_serializer(MergeWhenNotMatchedModeEnum value) {
    if (value == MergeWhenNotMatchedModeEnum::kDiscard) {
        return kMergeWhenNotMatchedMode_kDiscard;
    }
    if (value == MergeWhenNotMatchedModeEnum::kFail) {
        return kMergeWhenNotMatchedMode_kFail;
    }
    if (value == MergeWhenNotMatchedModeEnum::kInsert) {
        return kMergeWhenNotMatchedMode_kInsert;
    }
    MONGO_UNREACHABLE;
    return StringData();
}

}  // namespace mongo
