// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>

#include <logger/engine_log.h>
#include <logger/message_log.h>
#include <session/session_settings.h>

namespace falconfix {

class LogFactory {
  public:
    static std::unique_ptr<EngineLog> createEngineLog(const SessionSettings &settings);
    static std::unique_ptr<MessageLog> createSessionLog(const SessionConfig &config);
    static void disableLogging() noexcept;
    static bool isLoggingDisabled() noexcept;
  private:
    inline static std::atomic_bool m_disabled{false};
};

} // namespace falconfix
