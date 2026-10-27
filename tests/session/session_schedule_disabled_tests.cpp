// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <session/session_schedule.h>

namespace {

using falconfix::SessionConfig;

TEST(SessionScheduleDisabledTests, DisabledScheduleIsAlwaysWithinAtSeconds) {
	SessionConfig::SessionSchedule schedule;
	schedule.enabled = false;

	// When disabled, the window bounds are irrelevant: always within schedule.
	EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 0));
	EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 999999));
}

TEST(SessionScheduleDisabledTests, DisabledScheduleIsAlwaysWithinNow) {
	SessionConfig::SessionSchedule schedule;
	schedule.enabled = false;

	EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleNow(schedule, 1768212000));
}

TEST(SessionScheduleDisabledTests, EnabledScheduleRespectsDailyWindowAtSeconds) {
	SessionConfig::SessionSchedule schedule;
	schedule.enabled = true;
	schedule.startSeconds = 100;
	schedule.endSeconds = 200;

	EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 150));
	EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 50));
	EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 250));
}

TEST(SessionScheduleDisabledTests, EnabledScheduleHandlesOvernightWrapAround) {
	SessionConfig::SessionSchedule schedule;
	schedule.enabled = true;
	// Window crosses midnight: start > end.
	schedule.startSeconds = 200;
	schedule.endSeconds = 100;

	EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 250));
	EXPECT_TRUE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 50));
	EXPECT_FALSE(falconfix::session_schedule::isWithinScheduleAtSeconds(schedule, 150));
}

} // namespace
