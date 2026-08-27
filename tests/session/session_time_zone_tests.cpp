// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <chrono>

#include <session/session_time.h>

TEST(SessionTimeZoneTests, UtcAliasMapsToUtc) {
    constexpr int64_t epochSeconds = 1768489800; // 2026-01-15 15:10:00 UTC
    EXPECT_EQ(falconfix::session_time::secondsSinceMidnight("", epochSeconds), 15 * 3600 + 10 * 60);
    EXPECT_EQ(falconfix::session_time::secondsSinceMidnight("UTC", epochSeconds), 15 * 3600 + 10 * 60);
    EXPECT_EQ(falconfix::session_time::secondsSinceMidnight("Z", epochSeconds), 15 * 3600 + 10 * 60);
    EXPECT_EQ(falconfix::session_time::secondsSinceMidnight("GMT", epochSeconds), 15 * 3600 + 10 * 60);
}

TEST(SessionTimeZoneTests, ConvertsUtcToNamedTimeZoneWithDstAwareness) {
    constexpr int64_t winterEpochSeconds = 1768489800; // 2026-01-15 15:10:00 UTC
    constexpr int64_t summerEpochSeconds = 1784128200; // 2026-07-15 15:10:00 UTC

    EXPECT_EQ(
        falconfix::session_time::secondsSinceMidnight("America/New_York", winterEpochSeconds),
        10 * 3600 + 10 * 60
    );
    EXPECT_EQ(
        falconfix::session_time::secondsSinceMidnight("America/New_York", summerEpochSeconds),
        11 * 3600 + 10 * 60
    );
}

TEST(SessionTimeZoneTests, RejectsInvalidTimeZone) {
    EXPECT_THROW(
        (void)falconfix::session_time::validateTimeZone("FalconFIX/InvalidZone"),
        std::runtime_error
    );
}

TEST(SessionTimeZoneTests, UseLocalTimeUsesCurrentSystemZone) {
    constexpr int64_t epochSeconds = 1768489800; // 2026-01-15 15:10:00 UTC
    const auto point = falconfix::session_time::localTimePoint("", epochSeconds, true);

    const auto *zone = std::chrono::current_zone();
    const auto sysNow = std::chrono::sys_seconds{std::chrono::seconds{epochSeconds}};
    const auto localNow = std::chrono::zoned_time{zone, sysNow}.get_local_time();
    const auto localDay = std::chrono::floor<std::chrono::days>(localNow);
    const auto localTod = std::chrono::duration_cast<std::chrono::seconds>(localNow - localDay).count();

    EXPECT_EQ(point.secondsSinceMidnight, localTod);
    EXPECT_EQ(point.weekday, static_cast<int>(std::chrono::weekday{localDay}.c_encoding()));
}
