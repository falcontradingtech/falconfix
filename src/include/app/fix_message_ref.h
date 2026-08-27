// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>
#include <string_view>

namespace falconfix {

enum class ProtocolVersion : uint8_t {
    Unknown = 0,
    FIX40,
    FIX41,
    FIX42,
    FIX43,
    FIX44,
    FIX50,
    FIX50SP1,
    FIX50SP2,
    FIXT11,
};

template<typename Protocol>
consteval ProtocolVersion protocolVersionOf() noexcept {
    constexpr auto begin = Protocol::BEGIN_STRING;

    if constexpr (begin == "FIX4.0") return ProtocolVersion::FIX40;
    if constexpr (begin == "FIX4.1") return ProtocolVersion::FIX41;
    if constexpr (begin == "FIX4.2") return ProtocolVersion::FIX42;
    if constexpr (begin == "FIX4.3") return ProtocolVersion::FIX43;
    if constexpr (begin == "FIX4.4") return ProtocolVersion::FIX44;
    if constexpr (begin == "FIX5.0") return ProtocolVersion::FIX50;
    if constexpr (begin == "FIX5.0SP1") return ProtocolVersion::FIX50SP1;
    if constexpr (begin == "FIX5.0SP2") return ProtocolVersion::FIX50SP2;
    if constexpr (begin == "FIXT1.1") return ProtocolVersion::FIXT11;

    return ProtocolVersion::Unknown;
}

struct FIXMessageRef {
    void *ptr{nullptr};
    std::string_view msgType{};
    ProtocolVersion protocol{ProtocolVersion::Unknown};
    bool possDup{false};
    bool possResend{false};

    template<typename T>
    T &as() noexcept { return *static_cast<T *>(ptr); }

    template<typename T>
    const T &as() const noexcept { return *static_cast<const T *>(ptr); }

    template<typename Msg>
    [[nodiscard]] bool is() const noexcept {
        return ptr != nullptr &&
               protocol == protocolVersionOf<typename Msg::Protocol>() &&
               msgType == Msg::MSGTYPE;
    }

    [[nodiscard]] explicit operator bool() const noexcept { return ptr != nullptr; }
};

} // namespace falconfix
