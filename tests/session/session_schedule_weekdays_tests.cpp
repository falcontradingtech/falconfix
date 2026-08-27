// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <session/session_schedule.h>

namespace {

falconfix::SessionConfig::SessionSchedule makeWeekdaySchedule() {
    falconfix::SessionConfig::SessionSchedule schedule;
    schedule.enabled = true;
    schedule.useWeekdays = true;
    schedule.timeZone = "UTC";
    return schedule;
}

} // namespace

TEST(SessionScheduleWeekdaysTests, AllowsConfiguredWeekdayInsideDaytimeWindow) {
    auto schedule = makeWeekdaySchedule();
    schedule.weekdaysMask = static_cast<uint8_t>(1u << 1); // Monday
    schedule.startSeconds = 9 * 3600;
    schedule.endSeconds = 17 * 3600;

    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768212000));  // 2026-01-12 10:00 UTC Monday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768206600)); // 2026-01-12 08:30 UTC Monday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768298400)); // 2026-01-13 10:00 UTC Tuesday
}

TEST(SessionScheduleWeekdaysTests, OvernightWindowCarriesStartWeekdayAcrossMidnight) {
    auto schedule = makeWeekdaySchedule();
    schedule.weekdaysMask = static_cast<uint8_t>(1u << 1); // Monday
    schedule.startSeconds = 22 * 3600;
    schedule.endSeconds = 6 * 3600;

    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768258800));  // 2026-01-12 23:00 UTC Monday
    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768273200));  // 2026-01-13 03:00 UTC Tuesday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768305600)); // 2026-01-13 12:00 UTC Tuesday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768172400)); // 2026-01-11 23:00 UTC Sunday
}

TEST(SessionScheduleWeekdaysTests, WeeklyWindowAllowsTimeBetweenStartAndEndDays) {
    auto schedule = makeWeekdaySchedule();
    schedule.useWeekdays = false;
    schedule.useWeeklyWindow = true;
    schedule.startDay = 1; // Monday
    schedule.endDay = 5;   // Friday
    schedule.startSeconds = 9 * 3600;
    schedule.endSeconds = 17 * 3600;

    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768212000));  // 2026-01-12 10:00 UTC Monday
    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768471200));  // 2026-01-15 10:00 UTC Thursday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768206600)); // 2026-01-12 08:30 UTC Monday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768586400)); // 2026-01-16 18:00 UTC Friday after end
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768125600)); // 2026-01-11 10:00 UTC Sunday
}

TEST(SessionScheduleWeekdaysTests, WeeklyWindowWrapsAcrossWeekendBoundary) {
    auto schedule = makeWeekdaySchedule();
    schedule.useWeekdays = false;
    schedule.useWeeklyWindow = true;
    schedule.startDay = 5; // Friday
    schedule.endDay = 1;   // Monday
    schedule.startSeconds = 22 * 3600;
    schedule.endSeconds = 6 * 3600;

    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768606200));  // 2026-01-16 23:30 UTC Friday
    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768755600));  // 2026-01-18 17:00 UTC Sunday
    EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768798800));  // 2026-01-19 05:00 UTC Monday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768557600)); // 2026-01-16 09:00 UTC Friday before start
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768885200)); // 2026-01-20 05:00 UTC Tuesday
    EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768809600)); // 2026-01-19 08:00 UTC Monday after end
}
