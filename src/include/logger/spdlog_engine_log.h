// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <logger/engine_log.h>
#include <spdlog/spdlog.h>
#include <memory>

namespace falconfix {

class SpdlogEngineLog final : public EngineLog {
    std::shared_ptr<spdlog::logger> m_logger;

public:
    explicit SpdlogEngineLog(std::shared_ptr<spdlog::logger> logger);

    void info(std::string_view msg) noexcept override;
    void warn(std::string_view msg) noexcept override;
    void error(std::string_view msg) noexcept override;
};

} // namespace falconfix
