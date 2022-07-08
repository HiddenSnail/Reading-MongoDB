/**
 *    Copyright (C) 2019-present MongoDB, Inc.
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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kDefault

#include "mongo/platform/basic.h"

#include "mongo/executor/non_auth_task_executor.h"

#include <utility>

#include "mongo/executor/network_interface_factory.h"
#include "mongo/executor/network_interface_thread_pool.h"
#include "mongo/executor/thread_pool_task_executor.h"
#include "mongo/platform/atomic_word.h"
#include "mongo/util/assert_util.h"

namespace mongo {
namespace executor {

namespace {

struct State {
    State() : hasStarted(false) {
        std::shared_ptr<NetworkInterface> ni =
            makeNetworkInterface("NonAuthExecutor", nullptr, nullptr, [] {
                ConnectionPool::Options options;
                options.skipAuthentication = true;
                return options;
            }());
        auto tp = std::make_unique<NetworkInterfaceThreadPool>(ni.get());
        executor = std::make_unique<ThreadPoolTaskExecutor>(std::move(tp), std::move(ni));
    }

    AtomicWord<bool> hasStarted;
    std::unique_ptr<TaskExecutor> executor;
};

const auto getExecutor = ServiceContext::declareDecoration<State>();

ServiceContext::ConstructorActionRegisterer nonAuthExecutorCAR{
    "NonAuthTaskExecutor",
    [](ServiceContext* service) {},
    [](ServiceContext* service) {
        // Destruction implicitly performs the needed shutdown and join()
        getExecutor(service).executor.reset();
    }};

}  // namespace

TaskExecutor* getNonAuthTaskExecutor(ServiceContext* svc) {
    auto& state = getExecutor(svc);
    invariant(state.executor);

    if (!state.hasStarted.load() && !state.hasStarted.swap(true)) {
        state.executor->startup();
    }

    return state.executor.get();
}

}  // namespace executor
}  // namespace mongo
