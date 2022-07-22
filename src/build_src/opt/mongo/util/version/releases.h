/**
 *    Copyright (C) 2021-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <array>
#include <fmt/format.h>
#include <utility>

#include "mongo/base/error_codes.h"
#include "mongo/base/string_data.h"
#include "mongo/util/assert_util.h"


namespace mongo::multiversion {

/**
 * The combination of the fields (version, targetVersion, previousVersion) in the
 * featureCompatibilityVersion document in the server configuration collection
 * (admin.system.version) are represented by this enum and determine this node's behavior.
 *
 * Features can be gated for specific versions, or ranges of versions above or below some
 * minimum or maximum version, respectively.
 *
 * While upgrading from version X to Y or downgrading from Y to X, the server supports the
 * features of the older of the two versions.
 *
 * For versions X and Y, the legal enums and featureCompatibilityVersion documents are:
 *
 * kFullyDowngradedTo_X
 * (X, Unset, Unset): Only version X features are available, and new and existing storage
 *                    engine entries use the X format
 *
 * kUpgradingFrom_X_To_Y
 * (X, Y, Unset): Only version X features are available, but new storage engine entries
 *                use the Y format, and existing entries may have either the X or
 *                Y format
 *
 * kVersion_X
 * (X, Unset, Unset): X features are available, and new and existing storage engine
 *                    entries use the X format
 *
 * kDowngradingFrom_X_To_Y
 * (Y, Y, X): Only Y features are available and new storage engine entries use the
 *            Y format, but existing entries may have either the Y or X format
 *
 * kUnsetDefaultLastLTSBehavior
 * (Unset, Unset, Unset): This is the case on startup before the fCV document is loaded into
 *                        memory. isVersionInitialized() will return false, and getVersion()
 *                        will return the default (kUnsetDefaultLastLTSBehavior).
 *
 */
enum class FeatureCompatibilityVersion {
    kInvalid,
    kUnsetDefaultLastLTSBehavior,

    kFullyDowngradedTo_5_0,
    kVersion_5_1,
    kVersion_5_2,
    kVersion_5_3,
    kVersion_6_0,
    kDowngradingFrom_6_1_To_6_0,
    kUpgradingFrom_6_0_To_6_1,
    kVersion_6_1,
};

constexpr size_t kSince_4_0 = 11;

// Last LTS was "6.0".
constexpr size_t kSinceLastLTS = 1;

constexpr inline StringData kParameterName = "featureCompatibilityVersion"_sd;

class GenericFCV {

public:
    static constexpr auto kLatest = FeatureCompatibilityVersion::kVersion_6_1;
    static constexpr auto kLastContinuous = FeatureCompatibilityVersion::kVersion_6_0;
    static constexpr auto kLastLTS = FeatureCompatibilityVersion::kVersion_6_0;

    static constexpr auto kUpgradingFromLastLTSToLatest = FeatureCompatibilityVersion::kUpgradingFrom_6_0_To_6_1;
    static constexpr auto kDowngradingFromLatestToLastLTS = FeatureCompatibilityVersion::kDowngradingFrom_6_1_To_6_0;
    static constexpr auto kUpgradingFromLastContinuousToLatest = FeatureCompatibilityVersion::kUpgradingFrom_6_0_To_6_1;
    static constexpr auto kDowngradingFromLatestToLastContinuous = FeatureCompatibilityVersion::kDowngradingFrom_6_1_To_6_0;
    static constexpr auto kUpgradingFromLastLTSToLastContinuous = FeatureCompatibilityVersion::kInvalid;
};

/**
 * A table with mappings between versions of the type 'X.Y', as well as versions that are not of the
 * type 'X.Y' (i.e. the transition FCVs, 'kInvalid', and 'kUnsetDefaultLastLTSBehavior') and their
 * corresponding strings.
 */
inline constexpr std::array extendedFCVTable {
    // The table's entries must appear in the same order in which the enums were defined.
    std::pair{FeatureCompatibilityVersion::kInvalid, "invalid"_sd},
    std::pair{FeatureCompatibilityVersion::kUnsetDefaultLastLTSBehavior, "unset"_sd},
    std::pair{FeatureCompatibilityVersion::kFullyDowngradedTo_5_0, "5.0"_sd},
    std::pair{FeatureCompatibilityVersion::kVersion_5_1, "5.1"_sd},
    std::pair{FeatureCompatibilityVersion::kVersion_5_2, "5.2"_sd},
    std::pair{FeatureCompatibilityVersion::kVersion_5_3, "5.3"_sd},
    std::pair{FeatureCompatibilityVersion::kVersion_6_0, "6.0"_sd},
    std::pair{FeatureCompatibilityVersion::kDowngradingFrom_6_1_To_6_0, "downgrading from 6.1 to 6.0"_sd},
    std::pair{FeatureCompatibilityVersion::kUpgradingFrom_6_0_To_6_1, "upgrading from 6.0 to 6.1"_sd},
    std::pair{FeatureCompatibilityVersion::kVersion_6_1, "6.1"_sd},
};

constexpr decltype(auto) findExtended(FeatureCompatibilityVersion v) {
    return extendedFCVTable[static_cast<size_t>(v)];
}

constexpr StringData toString(FeatureCompatibilityVersion v) {
    return findExtended(v).second;
}

/**
 * Pointers to nodes of the extended table that represent numbered software versions.
 * Other FCV enum members, such as those representing transitions, are excluded.
 */
inline constexpr std::array standardFCVTable {
    &findExtended(FeatureCompatibilityVersion::kFullyDowngradedTo_5_0),
    &findExtended(FeatureCompatibilityVersion::kVersion_5_1),
    &findExtended(FeatureCompatibilityVersion::kVersion_5_2),
    &findExtended(FeatureCompatibilityVersion::kVersion_5_3),
    &findExtended(FeatureCompatibilityVersion::kVersion_6_0),
    &findExtended(FeatureCompatibilityVersion::kVersion_6_1),
};

/**
 * Parses 'versionString', of the form "X.Y", to its corresponding FCV enum. For example, "5.1"
 * will be parsed as FeatureCompatibilityVersion::kVersion_5_1.
 * Throws 'ErrorCodes::BadValue' when 'versionString' is not of the form "X.Y", and has no matching
 * enum.
 */
inline FeatureCompatibilityVersion parseVersionForFeatureFlags(StringData versionString) {
    for (auto ptr : standardFCVTable)
        if (ptr->second == versionString)
            return ptr->first;
    uasserted(ErrorCodes::BadValue,
              fmt::format("Invalid FCV version {} for feature flag.", versionString));
}

/*
 * Returns whether the given FCV is a standard FCV enum, i.e. an enum that corresponds to a numbered
 * version of the form 'X.Y'. Non-standard enums are transition enums, 'kInvalid' and
 * 'kUnsetDefaultLastLTSBehavior'.
 */
constexpr bool isStandardFCV(FeatureCompatibilityVersion v) {
    for (auto ptr : standardFCVTable)
        if (ptr->first == v)
            return true;
    return false;
}

}  // namespace mongo::multiversion

/* vim: set filetype=cpp: */
