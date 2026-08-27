// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include <app/fix_message_ref.h>
#include <session/session_config.h>
#include <session/session_schedule.h>
#include <session/session_time.h>

namespace falconfix {

class FIXConfig;

class SessionSettings {
    std::vector<SessionConfig> m_sessions;

public:
    static SessionSettings fromConfig(const FIXConfig &cfg);
    static SessionSettings fromFile(std::string_view filePath);

    std::size_t size() const noexcept { return m_sessions.size(); }
    bool empty() const noexcept { return m_sessions.empty(); }

    const std::vector<SessionConfig> &sessions() const noexcept { return m_sessions; }
    const SessionConfig &operator[](std::size_t index) const noexcept { return m_sessions[index]; }

    static SessionID buildSessionID(const SessionConfig::Params &parameters);
    static ProtocolVersion parseProtocolVersion(std::string_view beginString) noexcept;
    static std::string_view protocolVersionName(ProtocolVersion version) noexcept;
};

} // namespace falconfix
