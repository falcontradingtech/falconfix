// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <atomic>
#include <memory>

#include <logger/engine_log.h>
#include <logger/message_log.h>
#include <session/session_settings.h>

namespace falconfix {

class LogFactory {
  public:
    static std::unique_ptr<EngineLog> createEngineLog(const SessionSettings &settings);
    static std::unique_ptr<MessageLog> createSessionLog(const SessionConfig &config);
    static void disableLogging() noexcept { m_disabled.store(true, std::memory_order_release); }
    static bool isLoggingDisabled() noexcept { return m_disabled.load(std::memory_order_acquire); }
  private:
    inline static std::atomic_bool m_disabled{false};
};

} // namespace falconfix
