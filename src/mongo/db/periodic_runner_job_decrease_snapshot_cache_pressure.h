/**
 *    Copyright (C) 2018-present MongoDB, Inc.
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

#include <memory>

#include "mongo/db/service_context.h"
#include "mongo/platform/mutex.h"
#include "mongo/util/hierarchical_acquisition.h"
#include "mongo/util/periodic_runner.h"

namespace mongo {

/**
 * Periodically checks for storage engine cache pressure to determine whether the maintained
 * snapshot history window target setting should be decreased. Maintaining too much snapshot and
 * write history can slow down the system. Runs once every checkCachePressurePeriodSeconds.
 *
 * This function should only ever be called once, during mongod server startup (db.cpp).
 * The PeriodicRunner will handle shutting down the job on shutdown, no extra handling necessary.
 */
class PeriodicThreadToDecreaseSnapshotHistoryCachePressure {
public:
    static PeriodicThreadToDecreaseSnapshotHistoryCachePressure& get(
        ServiceContext* serviceContext);

    PeriodicJobAnchor* operator->() const noexcept;
    PeriodicJobAnchor& operator*() const noexcept;

private:
    void _init(ServiceContext* serviceContext);

    inline static const auto _serviceDecoration =
        ServiceContext::declareDecoration<PeriodicThreadToDecreaseSnapshotHistoryCachePressure>();

    mutable Mutex _mutex =
        MONGO_MAKE_LATCH(HierarchicalAcquisitionLevel(1),
                         "PeriodicThreadToDecreaseSnapshotHistoryCachePressure::_mutex");
    std::shared_ptr<PeriodicJobAnchor> _anchor;
};

}  // namespace mongo
