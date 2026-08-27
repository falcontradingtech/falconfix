// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <session/session_id.h>
#include <utils/fast_buffer.h>

#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/messages/Heartbeat.h>

#include <string_view>

namespace {

std::string_view extractTagValue(std::string_view fixMessage, std::string_view tagPrefix) {
    const auto pos = fixMessage.find(tagPrefix);
    if (pos == std::string_view::npos) {
        return {};
    }

    const auto valueBegin = pos + tagPrefix.size();
    const auto valueEnd = fixMessage.find('\x01', valueBegin);
    if (valueEnd == std::string_view::npos) {
        return {};
    }

    return fixMessage.substr(valueBegin, valueEnd - valueBegin);
}

} // namespace

TEST(FIXSessionTests, TimestampSerializationRespectsMillisecondsInTimeStampSetting) {
    const falconfix::SessionID sid("FIX4.4", "CLIENT_TS", "SERVER_TS");

    fix44::messages::Heartbeat withMilliseconds;
    ASSERT_TRUE(fix44::engine::prepareOutbound(withMilliseconds, 1, sid).ok());

    falconfix::FastBuffer withMillisecondsBuffer;
    ASSERT_TRUE(withMilliseconds.toString(withMillisecondsBuffer, true).ok());

    const auto sendingTimeWithMilliseconds = extractTagValue(withMillisecondsBuffer.view(), "52=");
    ASSERT_FALSE(sendingTimeWithMilliseconds.empty());
    EXPECT_EQ(sendingTimeWithMilliseconds.size(), 17U);

    fix44::messages::Heartbeat withoutMilliseconds;
    ASSERT_TRUE(fix44::engine::prepareOutbound(withoutMilliseconds, 1, sid).ok());

    falconfix::FastBuffer withoutMillisecondsBuffer;
    ASSERT_TRUE(withoutMilliseconds.toString(withoutMillisecondsBuffer, false).ok());

    const auto sendingTimeWithoutMilliseconds = extractTagValue(withoutMillisecondsBuffer.view(), "52=");
    ASSERT_FALSE(sendingTimeWithoutMilliseconds.empty());
    EXPECT_EQ(sendingTimeWithoutMilliseconds.size(), 14U);
}
