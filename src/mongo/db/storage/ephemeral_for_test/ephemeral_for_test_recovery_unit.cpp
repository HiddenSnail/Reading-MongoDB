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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kStorage

#include "mongo/platform/basic.h"

#include "mongo/db/storage/ephemeral_for_test/ephemeral_for_test_recovery_unit.h"

#include "mongo/db/storage/sorted_data_interface.h"
#include "mongo/logv2/log.h"

namespace mongo {

EphemeralForTestRecoveryUnit::~EphemeralForTestRecoveryUnit() {
    invariant(!_inUnitOfWork(), toString(_getState()));
}

void EphemeralForTestRecoveryUnit::beginUnitOfWork(OperationContext* opCtx) {
    invariant(!_inUnitOfWork(), toString(_getState()));
    _setState(State::kInactiveInUnitOfWork);
}

void EphemeralForTestRecoveryUnit::doCommitUnitOfWork() {
    invariant(_inUnitOfWork(), toString(_getState()));
    _setState(State::kCommitting);

    try {
        for (Changes::iterator it = _changes.begin(), end = _changes.end(); it != end; ++it) {
            (*it)->commit(boost::none);
        }
        _changes.clear();
    } catch (...) {
        std::terminate();
    }

    // This ensures that the journal listener gets called on each commit.
    // SERVER-22575: Remove this once we add a generic mechanism to periodically wait
    // for durability.
    if (_waitUntilDurableCallback) {
        _waitUntilDurableCallback();
    }

    _setState(State::kInactive);
}

void EphemeralForTestRecoveryUnit::doAbortUnitOfWork() {
    invariant(_inUnitOfWork(), toString(_getState()));
    _setState(State::kAborting);

    try {
        for (Changes::reverse_iterator it = _changes.rbegin(), end = _changes.rend(); it != end;
             ++it) {
            auto change = *it;
            LOGV2_DEBUG(22217,
                        2,
                        "CUSTOM ROLLBACK {demangleName_typeid_change}",
                        "demangleName_typeid_change"_attr = demangleName(typeid(*change)));
            change->rollback();
        }
        _changes.clear();
    } catch (...) {
        std::terminate();
    }

    _setState(State::kInactive);
}

bool EphemeralForTestRecoveryUnit::waitUntilDurable(OperationContext* opCtx) {
    if (_waitUntilDurableCallback) {
        _waitUntilDurableCallback();
    }
    return true;
}

bool EphemeralForTestRecoveryUnit::inActiveTxn() const {
    return _inUnitOfWork();
}

void EphemeralForTestRecoveryUnit::doAbandonSnapshot() {
    invariant(!_inUnitOfWork(), toString(_getState()));
}

Status EphemeralForTestRecoveryUnit::obtainMajorityCommittedSnapshot() {
    return Status::OK();
}

void EphemeralForTestRecoveryUnit::registerChange(std::unique_ptr<Change> change) {
    _changes.push_back(std::move(change));
}
}  // namespace mongo
