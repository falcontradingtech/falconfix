// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <string_view>

namespace falconfix {

class MessageLog {
public:
    virtual ~MessageLog() = default;

    virtual void incoming(std::string_view session, std::string_view msg) noexcept = 0;
    virtual void outgoing(std::string_view session, std::string_view msg) noexcept = 0;
    virtual void event(std::string_view session, std::string_view msg) noexcept = 0;
};

class NullMessageLog final : public MessageLog {
public:
    void incoming(std::string_view, std::string_view) noexcept override {}
    void outgoing(std::string_view, std::string_view) noexcept override {}
    void event(std::string_view, std::string_view) noexcept override {}
};

} // namespace falconfix
