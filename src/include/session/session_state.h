// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// session/session_state.h
#pragma once

#include <cstdint>

namespace falconfix {

enum class SessionStatus : uint8_t {
    Disconnected = 0,
    Connecting,
    Connected,
    LogonSent,
    LoggedOn,
    LogoutSent,
    LogoutReceived,
    Closing
};

struct SessionState {
    SessionStatus status{SessionStatus::Disconnected};

    bool sentLogon{false};
    bool receivedLogon{false};
    bool sentLogout{false};
    bool receivedLogout{false};
    bool awaitingResend{false};
    bool resetSeqNumsOnDisconnect{false};
    bool awaitingTestResponse{false};

    uint64_t lastSentMs{0};
    uint64_t lastReceivedMs{0};
    uint64_t logoutSentMs{0};    
    uint64_t testRequestSentMs{0};    
    uint64_t testRequestID{0};

    [[nodiscard]] bool loggedOn() const noexcept {
        return status == SessionStatus::LoggedOn;
    }

    [[nodiscard]] bool closing() const noexcept {
        return status == SessionStatus::Closing ||
            status == SessionStatus::LogoutSent ||
            status == SessionStatus::LogoutReceived;
    }
};

} // namespace falconfix
