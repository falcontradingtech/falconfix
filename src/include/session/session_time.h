// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace falconfix::session_time {

struct LocalTimePoint {
    int64_t secondsSinceMidnight{0};
    int weekday{0}; // std::chrono::weekday c_encoding: Sunday=0
};

inline std::string_view normalizeTimeZone(std::string_view timeZone) noexcept {
    if (timeZone.empty() || timeZone == "UTC" || timeZone == "Z" || timeZone == "GMT") {
        return "UTC";
    }

    return timeZone;
}

inline const std::chrono::time_zone *resolveTimeZone(std::string_view timeZone,
                                                     bool useLocalTime = false) {
    if (useLocalTime) {
        return std::chrono::current_zone();
    }

    const auto normalized = normalizeTimeZone(timeZone);
    return std::chrono::locate_zone(normalized);
}

inline void validateTimeZone(std::string_view timeZone,
                             bool useLocalTime = false) {
    if (useLocalTime && !timeZone.empty()) {
        throw std::runtime_error("TimeZone cannot be used together with UseLocalTime");
    }

    if (useLocalTime) {
        (void)std::chrono::current_zone();
        return;
    }

    const auto normalized = normalizeTimeZone(timeZone);
    (void)std::chrono::locate_zone(normalized);
}

inline int64_t secondsSinceMidnight(std::string_view timeZone,
                                    int64_t epochSeconds = -1,
                                    bool useLocalTime = false) {
    const auto *zone = resolveTimeZone(timeZone, useLocalTime);
    const auto sysNow = (epochSeconds >= 0)
        ? std::chrono::sys_seconds{std::chrono::seconds{epochSeconds}}
        : std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

    const auto localNow = std::chrono::zoned_time{zone, sysNow}.get_local_time();
    const auto localDay = std::chrono::floor<std::chrono::days>(localNow);
    const auto localTod = std::chrono::duration_cast<std::chrono::seconds>(localNow - localDay).count();

    return static_cast<int64_t>(localTod);
}

inline LocalTimePoint localTimePoint(std::string_view timeZone,
                                     int64_t epochSeconds = -1,
                                     bool useLocalTime = false) {
    const auto *zone = resolveTimeZone(timeZone, useLocalTime);
    const auto sysNow = (epochSeconds >= 0)
        ? std::chrono::sys_seconds{std::chrono::seconds{epochSeconds}}
        : std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

    const auto localNow = std::chrono::zoned_time{zone, sysNow}.get_local_time();
    const auto localDay = std::chrono::floor<std::chrono::days>(localNow);
    const auto localTod = std::chrono::duration_cast<std::chrono::seconds>(localNow - localDay).count();

    return LocalTimePoint{
        .secondsSinceMidnight = static_cast<int64_t>(localTod),
        .weekday = static_cast<int>(std::chrono::weekday{localDay}.c_encoding()),
    };
}

} // namespace falconfix::session_time
