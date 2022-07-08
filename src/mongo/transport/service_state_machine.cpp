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

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kNetwork

#include "mongo/platform/basic.h"

#include "mongo/transport/service_state_machine.h"

#include <memory>

#include "mongo/config.h"
#include "mongo/db/client.h"
#include "mongo/db/dbmessage.h"
#include "mongo/db/stats/counters.h"
#include "mongo/db/traffic_recorder.h"
#include "mongo/logv2/log.h"
#include "mongo/rpc/message.h"
#include "mongo/rpc/op_msg.h"
#include "mongo/transport/message_compressor_manager.h"
#include "mongo/transport/service_entry_point.h"
#include "mongo/transport/service_executor_task_names.h"
#include "mongo/transport/session.h"
#include "mongo/transport/transport_layer.h"
#include "mongo/util/assert_util.h"
#include "mongo/util/concurrency/idle_thread_block.h"
#include "mongo/util/concurrency/thread_name.h"
#include "mongo/util/debug_util.h"
#include "mongo/util/exit.h"
#include "mongo/util/fail_point.h"
#include "mongo/util/net/socket_exception.h"
#include "mongo/util/quick_exit.h"

namespace mongo {
namespace {
MONGO_FAIL_POINT_DEFINE(doNotSetMoreToCome);
/**
 * Creates and returns a legacy exhaust message, if exhaust is allowed. The returned message is to
 * be used as the subsequent 'synthetic' exhaust request. Returns an empty message if exhaust is not
 * allowed. Any messages that do not have an opcode of OP_MSG are considered legacy.
 */
Message makeLegacyExhaustMessage(Message* m, const DbResponse& dbresponse) {
    // OP_QUERY responses are always of type OP_REPLY.
    invariant(dbresponse.response.operation() == opReply);

    if (!dbresponse.shouldRunAgainForExhaust) {
        return Message();
    }

    // Legacy find operations via the OP_QUERY/OP_GET_MORE network protocol never provide the next
    // invocation for exhaust.
    invariant(!dbresponse.nextInvocation);

    DbMessage dbmsg(*m);
    invariant(dbmsg.messageShouldHaveNs());
    const char* ns = dbmsg.getns();

    MsgData::View header = dbresponse.response.header();
    QueryResult::View qr = header.view2ptr();
    long long cursorid = qr.getCursorId();

    if (cursorid == 0) {
        return Message();
    }

    // Generate a message that will act as the subsequent 'synthetic' exhaust request.
    BufBuilder b(512);
    b.appendNum(static_cast<int>(0));          // size set later in setLen()
    b.appendNum(header.getId());               // message id
    b.appendNum(header.getResponseToMsgId());  // in response to
    b.appendNum(static_cast<int>(dbGetMore));  // opCode is OP_GET_MORE
    b.appendNum(static_cast<int>(0));          // Must be ZERO (reserved)
    b.appendStr(StringData(ns));               // Namespace
    b.appendNum(static_cast<int>(0));          // ntoreturn
    b.appendNum(cursorid);                     // cursor id from the OP_REPLY

    MsgData::View(b.buf()).setLen(b.len());

    return Message(b.release());
}

/**
 * Given a request and its already generated response, checks for exhaust flags. If exhaust is
 * allowed, produces the subsequent request message, and modifies the response message to indicate
 * it is part of an exhaust stream. Returns the subsequent request message, which is known as a
 * 'synthetic' exhaust request. Returns an empty message if exhaust is not allowed.
 */
Message makeExhaustMessage(Message requestMsg, DbResponse* dbresponse) {
    if (requestMsg.operation() == dbQuery) {
        return makeLegacyExhaustMessage(&requestMsg, *dbresponse);
    }

    if (!OpMsgRequest::isFlagSet(requestMsg, OpMsg::kExhaustSupported)) {
        return Message();
    }

    if (!dbresponse->shouldRunAgainForExhaust) {
        return Message();
    }

    const bool checksumPresent = OpMsg::isFlagSet(requestMsg, OpMsg::kChecksumPresent);
    Message exhaustMessage;

    if (auto nextInvocation = dbresponse->nextInvocation) {
        // The command provided a new BSONObj for the next invocation.
        OpMsgBuilder builder;
        builder.setBody(*nextInvocation);
        exhaustMessage = builder.finish();
    } else {
        // Reuse the previous invocation for the next invocation.
        OpMsg::removeChecksum(&requestMsg);
        exhaustMessage = requestMsg;
    }

    // The id of the response is used as the request id of this 'synthetic' request. Re-checksum
    // if needed.
    exhaustMessage.header().setId(dbresponse->response.header().getId());
    exhaustMessage.header().setResponseToMsgId(dbresponse->response.header().getResponseToMsgId());
    OpMsg::setFlag(&exhaustMessage, OpMsg::kExhaustSupported);
    if (checksumPresent) {
        OpMsg::appendChecksum(&exhaustMessage);
    }

    OpMsg::removeChecksum(&dbresponse->response);
    // Indicate that the response is part of an exhaust stream (unless the 'doNotSetMoreToCome'
    // failpoint is set). Re-checksum if needed.
    if (!MONGO_unlikely(doNotSetMoreToCome.shouldFail())) {
        OpMsg::setFlag(&dbresponse->response, OpMsg::kMoreToCome);
    }
    if (checksumPresent) {
        OpMsg::appendChecksum(&dbresponse->response);
    }

    return exhaustMessage;
}

}  // namespace

using transport::ServiceExecutor;
using transport::TransportLayer;

/*
 * This class wraps up the logic for swapping/unswapping the Client during runNext().
 *
 * In debug builds this also ensures that only one thread is working on the SSM at once.
 */
class ServiceStateMachine::ThreadGuard {
    ThreadGuard(ThreadGuard&) = delete;
    ThreadGuard& operator=(ThreadGuard&) = delete;

public:
    explicit ThreadGuard(ServiceStateMachine* ssm) : _ssm{ssm} {
        auto owned = Ownership::kUnowned;
        _ssm->_owned.compareAndSwap(&owned, Ownership::kOwned);
        if (owned == Ownership::kStatic) {
            dassert(haveClient());
            dassert(Client::getCurrent() == _ssm->_dbClientPtr);
            _haveTakenOwnership = true;
            return;
        }

#ifdef MONGO_CONFIG_DEBUG_BUILD
        invariant(owned == Ownership::kUnowned);
        _ssm->_owningThread.store(stdx::this_thread::get_id());
#endif

        // Set up the thread name
        auto oldThreadName = getThreadName();
        if (oldThreadName != _ssm->_threadName) {
            _ssm->_oldThreadName = getThreadName().toString();
            setThreadName(_ssm->_threadName);
        }

        // Swap the current Client so calls to cc() work as expected
        Client::setCurrent(std::move(_ssm->_dbClient));
        _haveTakenOwnership = true;
    }

    // Constructing from a moved ThreadGuard invalidates the other thread guard.
    ThreadGuard(ThreadGuard&& other)
        : _ssm(other._ssm), _haveTakenOwnership(other._haveTakenOwnership) {
        other._haveTakenOwnership = false;
    }

    ThreadGuard& operator=(ThreadGuard&& other) {
        if (this != &other) {
            _ssm = other._ssm;
            _haveTakenOwnership = other._haveTakenOwnership;
            other._haveTakenOwnership = false;
        }
        return *this;
    };

    ThreadGuard() = delete;

    ~ThreadGuard() {
        if (_haveTakenOwnership)
            release();
    }

    explicit operator bool() const {
#ifdef MONGO_CONFIG_DEBUG_BUILD
        if (_haveTakenOwnership) {
            invariant(_ssm->_owned.load() != Ownership::kUnowned);
            invariant(_ssm->_owningThread.load() == stdx::this_thread::get_id());
            return true;
        } else {
            return false;
        }
#else
        return _haveTakenOwnership;
#endif
    }

    void markStaticOwnership() {
        dassert(static_cast<bool>(*this));
        _ssm->_owned.store(Ownership::kStatic);
    }

    void release() {
        auto owned = _ssm->_owned.load();

#ifdef MONGO_CONFIG_DEBUG_BUILD
        dassert(_haveTakenOwnership);
        dassert(owned != Ownership::kUnowned);
        dassert(_ssm->_owningThread.load() == stdx::this_thread::get_id());
#endif
        if (owned != Ownership::kStatic) {
            if (haveClient()) {
                _ssm->_dbClient = Client::releaseCurrent();
            }

            if (!_ssm->_oldThreadName.empty()) {
                setThreadName(_ssm->_oldThreadName);
            }
        }

        // If the session has ended, then it's unsafe to do anything but call the cleanup hook.
        if (_ssm->state() == State::Ended) {
            // The cleanup hook gets moved out of _ssm->_cleanupHook so that it can only be called
            // once.
            auto cleanupHook = std::move(_ssm->_cleanupHook);
            if (cleanupHook)
                cleanupHook();

            // It's very important that the Guard returns here and that the SSM's state does not
            // get modified in any way after the cleanup hook is called.
            return;
        }

        _haveTakenOwnership = false;
        // If owned != Ownership::kOwned here then it can only equal Ownership::kStatic and we
        // should just return
        if (owned == Ownership::kOwned) {
            _ssm->_owned.store(Ownership::kUnowned);
        }
    }

private:
    ServiceStateMachine* _ssm;
    bool _haveTakenOwnership = false;
};

std::shared_ptr<ServiceStateMachine> ServiceStateMachine::create(ServiceContext* svcContext,
                                                                 transport::SessionHandle session,
                                                                 transport::Mode transportMode) {
    return std::make_shared<ServiceStateMachine>(svcContext, std::move(session), transportMode);
}

ServiceStateMachine::ServiceStateMachine(ServiceContext* svcContext,
                                         transport::SessionHandle session,
                                         transport::Mode transportMode)
    : _state{State::Created},
      _sep{svcContext->getServiceEntryPoint()},
      _transportMode(transportMode),
      _serviceContext(svcContext),
      _serviceExecutor(_serviceContext->getServiceExecutor()),
      _sessionHandle(session),
      _threadName{str::stream() << "conn" << _session()->id()},
      _dbClient{svcContext->makeClient(_threadName, std::move(session))},
      _dbClientPtr{_dbClient.get()} {}

const transport::SessionHandle& ServiceStateMachine::_session() const {
    return _sessionHandle;
}

void ServiceStateMachine::_sourceMessage(ThreadGuard guard) {
    invariant(_inMessage.empty());
    invariant(_state.load() == State::Source);
    _state.store(State::SourceWait);
    guard.release();

    auto sourceMsgImpl = [&] {
        if (_transportMode == transport::Mode::kSynchronous) {
            MONGO_IDLE_THREAD_BLOCK;
            return Future<Message>::makeReady(_session()->sourceMessage());
        } else {
            invariant(_transportMode == transport::Mode::kAsynchronous);
            return _session()->asyncSourceMessage();
        }
    };

    sourceMsgImpl().getAsync([this](StatusWith<Message> msg) {
        if (msg.isOK()) {
            _inMessage = std::move(msg.getValue());
            invariant(!_inMessage.empty());
        }
        _sourceCallback(msg.getStatus());
    });
}

void ServiceStateMachine::_sinkMessage(ThreadGuard guard, Message toSink) {
    // Sink our response to the client
    invariant(_state.load() == State::Process);
    _state.store(State::SinkWait);
    guard.release();

    auto sinkMsgImpl = [&] {
        if (_transportMode == transport::Mode::kSynchronous) {
            // We don't consider ourselves idle while sending the reply since we are still doing
            // work on behalf of the client. Contrast that with sourceMessage() where we are waiting
            // for the client to send us more work to do.
            return Future<void>::makeReady(_session()->sinkMessage(std::move(toSink)));
        } else {
            invariant(_transportMode == transport::Mode::kAsynchronous);
            return _session()->asyncSinkMessage(std::move(toSink));
        }
    };

    sinkMsgImpl().getAsync([this](Status status) { _sinkCallback(std::move(status)); });
}

void ServiceStateMachine::_sourceCallback(Status status) {
    // The first thing to do is create a ThreadGuard which will take ownership of the SSM in this
    // thread.
    ThreadGuard guard(this);

    // Make sure we just called sourceMessage();
    dassert(state() == State::SourceWait);
    auto remote = _session()->remote();

    if (status.isOK()) {
        _state.store(State::Process);

        // Since we know that we're going to process a message, call scheduleNext() immediately
        // to schedule the call to processMessage() on the serviceExecutor (or just unwind the
        // stack)

        // If this callback doesn't own the ThreadGuard, then we're being called recursively,
        // and the executor shouldn't start a new thread to process the message - it can use this
        // one just after this returns.
        return _scheduleNextWithGuard(std::move(guard),
                                      ServiceExecutor::kMayRecurse,
                                      transport::ServiceExecutorTaskName::kSSMProcessMessage);
    } else if (ErrorCodes::isInterruption(status.code()) ||
               ErrorCodes::isNetworkError(status.code())) {
        LOGV2_DEBUG(
            22986,
            2,
            "Session from {remote} encountered a network error during SourceMessage: {error}",
            "Session from remote encountered a network error during SourceMessage",
            "remote"_attr = remote,
            "error"_attr = status);
        _state.store(State::EndSession);
    } else if (status == TransportLayer::TicketSessionClosedStatus) {
        // Our session may have been closed internally.
        LOGV2_DEBUG(22987,
                    2,
                    "Session from {remote} was closed internally during SourceMessage",
                    "remote"_attr = remote);
        _state.store(State::EndSession);
    } else {
        LOGV2(22988,
              "Error receiving request from client. Ending connection from remote",
              "error"_attr = status,
              "remote"_attr = remote,
              "connectionId"_attr = _session()->id());
        _state.store(State::EndSession);
    }

    // There was an error receiving a message from the client and we've already printed the error
    // so call runNextInGuard() to clean up the session without waiting.
    _runNextInGuard(std::move(guard));
}

void ServiceStateMachine::_sinkCallback(Status status) {
    // The first thing to do is create a ThreadGuard which will take ownership of the SSM in this
    // thread.
    ThreadGuard guard(this);

    dassert(state() == State::SinkWait);

    // If there was an error sinking the message to the client, then we should print an error and
    // end the session. No need to unwind the stack, so this will runNextInGuard() and return.
    //
    // Otherwise, update the current state depending on whether we're in exhaust or not, and call
    // scheduleNext() to unwind the stack and do the next step.
    if (!status.isOK()) {
        LOGV2(22989,
              "Error sending response to client. Ending connection from remote",
              "error"_attr = status,
              "remote"_attr = _session()->remote(),
              "connectionId"_attr = _session()->id());
        _state.store(State::EndSession);
        return _runNextInGuard(std::move(guard));
    } else if (_inExhaust) {
        _state.store(State::Process);
        return _scheduleNextWithGuard(std::move(guard),
                                      ServiceExecutor::kDeferredTask |
                                          ServiceExecutor::kMayYieldBeforeSchedule,
                                      transport::ServiceExecutorTaskName::kSSMExhaustMessage);
    } else {
        _state.store(State::Source);
        return _scheduleNextWithGuard(std::move(guard),
                                      ServiceExecutor::kDeferredTask |
                                          ServiceExecutor::kMayYieldBeforeSchedule,
                                      transport::ServiceExecutorTaskName::kSSMSourceMessage);
    }
}

void ServiceStateMachine::_processMessage(ThreadGuard guard) {
    invariant(!_inMessage.empty());

    TrafficRecorder::get(_serviceContext)
        .observe(_sessionHandle, _serviceContext->getPreciseClockSource()->now(), _inMessage);

    auto& compressorMgr = MessageCompressorManager::forSession(_session());

    _compressorId = boost::none;
    if (_inMessage.operation() == dbCompressed) {
        MessageCompressorId compressorId;
        auto swm = compressorMgr.decompressMessage(_inMessage, &compressorId);
        uassertStatusOK(swm.getStatus());
        _inMessage = swm.getValue();
        _compressorId = compressorId;
    }

    networkCounter.hitLogicalIn(_inMessage.size());

    // Pass sourced Message to handler to generate response.
    auto opCtx = Client::getCurrent()->makeOperationContext();
    if (_inExhaust) {
        opCtx->markKillOnClientDisconnect();
    }

    // The handleRequest is implemented in a subclass for mongod/mongos and actually all the
    // database work for this request.
    DbResponse dbresponse = _sep->handleRequest(opCtx.get(), _inMessage);

    // opCtx must be killed and delisted here so that the operation cannot show up in currentOp
    // results after the response reaches the client. The destruction is postponed for later to
    // mitigate its performance impact on the critical path of execution.
    _serviceContext->killAndDelistOperation(opCtx.get(), ErrorCodes::OperationIsKilledAndDelisted);
    invariant(!_killedOpCtx);
    _killedOpCtx = std::move(opCtx);

    // Format our response, if we have one
    Message& toSink = dbresponse.response;
    if (!toSink.empty()) {
        invariant(!OpMsg::isFlagSet(_inMessage, OpMsg::kMoreToCome));
        invariant(!OpMsg::isFlagSet(toSink, OpMsg::kChecksumPresent));

        // Update the header for the response message.
        toSink.header().setId(nextMessageId());
        toSink.header().setResponseToMsgId(_inMessage.header().getId());
        if (OpMsg::isFlagSet(_inMessage, OpMsg::kChecksumPresent)) {
#ifdef MONGO_CONFIG_SSL
            if (!SSLPeerInfo::forSession(_session()).isTLS) {
                OpMsg::appendChecksum(&toSink);
            }
#else
            OpMsg::appendChecksum(&toSink);
#endif
        }

        // If the incoming message has the exhaust flag set, then we bypass the normal RPC behavior.
        // We will sink the response to the network, but we also synthesize a new request, as if we
        // sourced a new message from the network. This new request is sent to the database once
        // again to be processed. This cycle repeats as long as the command indicates the exhaust
        // stream should continue.
        _inMessage = makeExhaustMessage(_inMessage, &dbresponse);
        _inExhaust = !_inMessage.empty();

        networkCounter.hitLogicalOut(toSink.size());

        if (_compressorId) {
            auto swm = compressorMgr.compressMessage(toSink, &_compressorId.value());
            uassertStatusOK(swm.getStatus());
            toSink = swm.getValue();
        }

        TrafficRecorder::get(_serviceContext)
            .observe(_sessionHandle, _serviceContext->getPreciseClockSource()->now(), toSink);

        _sinkMessage(std::move(guard), std::move(toSink));

    } else {
        _state.store(State::Source);
        _inMessage.reset();
        _inExhaust = false;
        return _scheduleNextWithGuard(std::move(guard),
                                      ServiceExecutor::kDeferredTask,
                                      transport::ServiceExecutorTaskName::kSSMSourceMessage);
    }
}

void ServiceStateMachine::runNext() {
    return _runNextInGuard(ThreadGuard(this));
}

void ServiceStateMachine::_runNextInGuard(ThreadGuard guard) {
    auto curState = state();
    dassert(curState != State::Ended);

    // If this is the first run of the SSM, then update its state to Source
    if (curState == State::Created) {
        curState = State::Source;
        _state.store(curState);
    }

    // Destroy the opCtx (already killed) here, to potentially use the delay between clients'
    // requests to hide the destruction cost.
    if (MONGO_likely(_killedOpCtx)) {
        _killedOpCtx.reset();
    }

    // Make sure the current Client got set correctly
    dassert(Client::getCurrent() == _dbClientPtr);
    try {
        switch (curState) {
            case State::Source:
                _sourceMessage(std::move(guard));
                break;
            case State::Process:
                _processMessage(std::move(guard));
                break;
            case State::EndSession:
                _cleanupSession(std::move(guard));
                break;
            default:
                MONGO_UNREACHABLE;
        }

        return;
    } catch (const DBException& e) {
        LOGV2(22990,
              "DBException handling request, closing client connection: {error}",
              "DBException handling request, closing client connection",
              "error"_attr = redact(e));
    }
    // No need to catch std::exception, as std::terminate will be called when the exception bubbles
    // to the top of the stack

    if (!guard) {
        guard = ThreadGuard(this);
    }
    _state.store(State::EndSession);
    _cleanupSession(std::move(guard));
}

void ServiceStateMachine::start(Ownership ownershipModel) {
    _scheduleNextWithGuard(ThreadGuard(this),
                           transport::ServiceExecutor::kEmptyFlags,
                           transport::ServiceExecutorTaskName::kSSMStartSession,
                           ownershipModel);
}

void ServiceStateMachine::setServiceExecutor(ServiceExecutor* executor) {
    _serviceExecutor = executor;
}

void ServiceStateMachine::_scheduleNextWithGuard(ThreadGuard guard,
                                                 transport::ServiceExecutor::ScheduleFlags flags,
                                                 transport::ServiceExecutorTaskName taskName,
                                                 Ownership ownershipModel) {
    auto func = [ssm = shared_from_this(), ownershipModel] {
        ThreadGuard guard(ssm.get());
        if (ownershipModel == Ownership::kStatic)
            guard.markStaticOwnership();
        ssm->_runNextInGuard(std::move(guard));
    };
    guard.release();
    Status status = _serviceExecutor->schedule(std::move(func), flags, taskName);
    if (status.isOK()) {
        return;
    }

    // We've had an error, reacquire the ThreadGuard and destroy the SSM
    ThreadGuard terminateGuard(this);

    // The service executor failed to schedule the task. This could for example be that we failed
    // to start a worker thread. Terminate this connection to leave the system in a valid state.
    _terminateAndLogIfError(status);
    _cleanupSession(std::move(terminateGuard));
}

void ServiceStateMachine::terminate() {
    if (state() == State::Ended)
        return;

    _session()->end();
}

void ServiceStateMachine::terminateIfTagsDontMatch(transport::Session::TagMask tags) {
    if (state() == State::Ended)
        return;

    auto sessionTags = _session()->getTags();

    // If terminateIfTagsDontMatch gets called when we still are 'pending' where no tags have been
    // set, then skip the termination check.
    if ((sessionTags & tags) || (sessionTags & transport::Session::kPending)) {
        LOGV2(22991,
              "Skip closing connection for connection",
              "connectionId"_attr = _session()->id());
        return;
    }

    terminate();
}

void ServiceStateMachine::setCleanupHook(std::function<void()> hook) {
    invariant(state() == State::Created);
    _cleanupHook = std::move(hook);
}

ServiceStateMachine::State ServiceStateMachine::state() {
    return _state.load();
}

void ServiceStateMachine::_terminateAndLogIfError(Status status) {
    if (!status.isOK()) {
        LOGV2_WARNING_OPTIONS(22993,
                              {logv2::LogComponent::kExecutor},
                              "Terminating session due to error: {error}",
                              "Terminating session due to error",
                              "error"_attr = status);
        terminate();
    }
}

void ServiceStateMachine::_cleanupExhaustResources() noexcept try {
    if (!_inExhaust) {
        return;
    }
    auto request = OpMsgRequest::parse(_inMessage);
    // Clean up cursor for exhaust getMore request.
    if (request.getCommandName() == "getMore"_sd) {
        auto cursorId = request.body["getMore"].Long();
        auto opCtx = Client::getCurrent()->makeOperationContext();
        // Fire and forget. This is a best effort attempt to immediately clean up the exhaust
        // cursor. If the killCursors request fails here for any reasons, it will still be
        // cleaned up once the cursor times out.
        _sep->handleRequest(opCtx.get(), makeKillCursorsMessage(cursorId));
    }
} catch (const DBException& e) {
    LOGV2(22992,
          "Error cleaning up resources for exhaust requests: {error}",
          "Error cleaning up resources for exhaust requests",
          "error"_attr = e.toStatus());
}

void ServiceStateMachine::_cleanupSession(ThreadGuard guard) {
    // Ensure the delayed destruction of opCtx always happens before doing the cleanup.
    if (MONGO_likely(_killedOpCtx)) {
        _killedOpCtx.reset();
    }
    invariant(!_killedOpCtx);

    _cleanupExhaustResources();

    _state.store(State::Ended);

    _inMessage.reset();

    // By ignoring the return value of Client::releaseCurrent() we destroy the session.
    // _dbClient is now nullptr and _dbClientPtr is invalid and should never be accessed.
    Client::releaseCurrent();
}

}  // namespace mongo
