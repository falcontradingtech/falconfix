// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>

#include <session/session_config.h>
#include <session/session_time.h>

namespace falconfix::session_schedule {

inline bool isWeekdayEnabled(uint8_t weekdaysMask, int weekday) noexcept {
    return weekday >= 0 && weekday < 7 && (weekdaysMask & (1u << weekday)) != 0;
}

inline int64_t weeklySecond(int weekday, int64_t secondsSinceMidnight) noexcept {
    return static_cast<int64_t>(weekday) * 86400 + secondsSinceMidnight;
}

inline bool isWithinTimeWindow(int64_t startSeconds,
                               int64_t endSeconds,
                               int64_t nowSeconds) noexcept {
    if (startSeconds <= endSeconds) {
        return nowSeconds >= startSeconds && nowSeconds <= endSeconds;
    }

    return nowSeconds >= startSeconds || nowSeconds <= endSeconds;
}

inline bool isWithinScheduleAtSeconds(const SessionConfig::SessionSchedule &schedule,
                                      int64_t nowSeconds) noexcept {
    if (!schedule.enabled) {
        return true;
    }

    return isWithinTimeWindow(schedule.startSeconds, schedule.endSeconds, nowSeconds);
}

inline bool isWithinScheduleNow(const SessionConfig::SessionSchedule &schedule,
                                int64_t epochSeconds = -1) {
    if (!schedule.enabled) {
        return true;
    }

    const auto point = falconfix::session_time::localTimePoint(
        schedule.timeZone,
        epochSeconds,
        schedule.useLocalTime
    );

    if (schedule.useWeeklyWindow) {
        const int64_t nowWeekSecond = weeklySecond(point.weekday, point.secondsSinceMidnight);
        const int64_t startWeekSecond = weeklySecond(schedule.startDay, schedule.startSeconds);
        const int64_t endWeekSecond = weeklySecond(schedule.endDay, schedule.endSeconds);

        if (startWeekSecond <= endWeekSecond) {
            return nowWeekSecond >= startWeekSecond && nowWeekSecond <= endWeekSecond;
        }

        return nowWeekSecond >= startWeekSecond || nowWeekSecond <= endWeekSecond;
    }

    if (!schedule.useWeekdays) {
        return isWithinTimeWindow(schedule.startSeconds, schedule.endSeconds, point.secondsSinceMidnight);
    }

    if (schedule.startSeconds <= schedule.endSeconds) {
        return isWeekdayEnabled(schedule.weekdaysMask, point.weekday) &&
               point.secondsSinceMidnight >= schedule.startSeconds &&
               point.secondsSinceMidnight <= schedule.endSeconds;
    }

    if (point.secondsSinceMidnight >= schedule.startSeconds) {
        return isWeekdayEnabled(schedule.weekdaysMask, point.weekday);
    }

    if (point.secondsSinceMidnight <= schedule.endSeconds) {
        const int previousWeekday = (point.weekday + 6) % 7;
        return isWeekdayEnabled(schedule.weekdaysMask, previousWeekday);
    }

    return false;
}

} // namespace falconfix::session_schedule
