// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <string_view>


namespace falconfix {

class EngineLog {
public:
    virtual ~EngineLog() = default;

    virtual void info(std::string_view msg) noexcept = 0;
    virtual void warn(std::string_view msg) noexcept = 0;
    virtual void error(std::string_view msg) noexcept = 0;
};

class NullEngineLog final : public EngineLog {
public:
    void info(std::string_view) noexcept override {}
    void warn(std::string_view) noexcept override {}
    void error(std::string_view) noexcept override {}
};

} // namespace falconfix
