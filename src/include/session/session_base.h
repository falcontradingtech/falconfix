// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <utils/error_codes.h>

#include <connection/i_connection.h>
#include <app/fix_message_ref.h>
#include <session/session_id.h>

namespace falconfix {

class SessionBase {
public:
    virtual ~SessionBase() = default;

    virtual const SessionID &sessionID() const noexcept = 0;
    virtual ProtocolVersion protocolVersion() const noexcept = 0;
    virtual void bindConnection(IConnection &connection) noexcept = 0;

    virtual FFStatus onConnected() noexcept = 0;
    virtual FFStatus onReceive(const char *data, std::size_t size) noexcept = 0;
    virtual FFStatus onDisconnected() noexcept = 0;
    virtual FFStatus onTimer() noexcept = 0;

    virtual FFStatus doLogout(std::string_view reason = {}) noexcept = 0;
    virtual bool isDisconnected() const noexcept = 0;
    virtual bool isWithinSchedule(int64_t nowsecs = 0) const noexcept = 0;

#ifdef FALCONFIX_ENABLE_TEST_API
    virtual FFStatus validateSessionIdentityForTest(std::string_view beginString,
                                                    std::string_view senderCompID,
                                                    std::string_view targetCompID) noexcept = 0;
    virtual FFStatus validateSendingTimeForTest(int64_t sendingTimeMs) noexcept = 0;
    virtual void setScheduleForTest(int64_t start, int64_t end) noexcept = 0;

    virtual void setNextSenderSeqNumForTest(int64_t seqNum) noexcept = 0;
    virtual void setNextTargetSeqNumForTest(int64_t seqNum) noexcept = 0;
    virtual int64_t nextSenderSeqNumForTest() const noexcept = 0;
    virtual int64_t nextTargetSeqNumForTest() const noexcept = 0;
    virtual void setAwaitingTestResponseForTest(bool awaiting, uint64_t testRequestID) noexcept = 0;
    virtual bool awaitingTestResponseForTest() const noexcept = 0;
    virtual uint64_t testRequestIDForTest() const noexcept = 0;
#endif
};

} // namespace falconfix
