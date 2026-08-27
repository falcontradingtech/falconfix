// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <config/fix_config.h>
#include <session/session_settings.h>

#include "../support/test_config_builder.h"
#include "../support/test_runtime_support.h"

class SessionSettingsTests : public ::testing::Test
{
};

namespace {



using Protocol = falconfix::test::DefaultProtocol;

std::string multiSessionCfg() {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=initiator\n";
    out << "SocketConnectHost=127.0.0.1\n";
    out << "SocketConnectPort=35001\n";
    out << "HeartBtInt=5\n";
    out << "StoreType=memory\n";
    out << "LogType=none\n\n";

    out << "[SESSION]\n";
    out << "BeginString=" << Protocol::BEGIN_STRING << "\n";
    out << "SenderCompID=CLIENT_A\n";
    out << "TargetCompID=SERVER_A\n";
    out << "StartTime=09:00:00\n";
    out << "EndTime=17:00:00\n";
    out << "ResetSeqNumsDaily=Y\n\n";

    out << "[SESSION]\n";
    out << "BeginString=" << Protocol::BEGIN_STRING << "\n";
    out << "SenderCompID=CLIENT_B\n";
    out << "TargetCompID=SERVER_B\n";
    out << "StartTime=22:00:00\n";
    out << "EndTime=06:00:00\n";
    out << "ResetSeqNumsDaily=N\n";
    return out.str();
}

} // namespace

TEST_F(SessionSettingsTests, Something)
{
    EXPECT_EQ(1, 1);
}

TEST_F(SessionSettingsTests, LoadsMultipleSessionsFromIni)
{
    const auto ini = multiSessionCfg();
    const auto cfg = falconfix::FIXConfig::parse(ini);
    const auto settings = falconfix::SessionSettings::fromConfig(cfg);

    ASSERT_EQ(settings.size(), 2U);

    const auto &first = settings.sessions()[0];
    EXPECT_EQ(first.sessionID.beginString(), Protocol::BEGIN_STRING);
    EXPECT_EQ(first.sessionID.senderCompID(), "CLIENT_A");
    EXPECT_EQ(first.sessionID.targetCompID(), "SERVER_A");
    EXPECT_TRUE(first.schedule.enabled);
    EXPECT_EQ(first.schedule.startSeconds, falconfix::test::hms(9, 0));
    EXPECT_EQ(first.schedule.endSeconds, falconfix::test::hms(17, 0));
    EXPECT_TRUE(first.schedule.resetSeqNumsDaily);

    const auto &second = settings.sessions()[1];
    EXPECT_EQ(second.sessionID.beginString(), Protocol::BEGIN_STRING);
    EXPECT_EQ(second.sessionID.senderCompID(), "CLIENT_B");
    EXPECT_EQ(second.sessionID.targetCompID(), "SERVER_B");
    EXPECT_TRUE(second.schedule.enabled);
    EXPECT_EQ(second.schedule.startSeconds, falconfix::test::hms(22, 0));
    EXPECT_EQ(second.schedule.endSeconds, falconfix::test::hms(6, 0));
    EXPECT_FALSE(second.schedule.resetSeqNumsDaily);
}

TEST_F(SessionSettingsTests, SelectsSessionByIndexFromMultiSessionIni)
{
    const auto ini = multiSessionCfg();
    const auto first = falconfix::test::TestConfigBuilder<>::fromString(ini, 0).config();
    EXPECT_EQ(first.sessionID.senderCompID(), "CLIENT_A");
    EXPECT_EQ(first.sessionID.targetCompID(), "SERVER_A");
    EXPECT_EQ(first.schedule.startSeconds, falconfix::test::hms(9, 0));
    EXPECT_EQ(first.schedule.endSeconds, falconfix::test::hms(17, 0));
    EXPECT_TRUE(first.schedule.resetSeqNumsDaily);

    const auto second = falconfix::test::TestConfigBuilder<>::fromString(ini, 1).config();
    EXPECT_EQ(second.sessionID.senderCompID(), "CLIENT_B");
    EXPECT_EQ(second.sessionID.targetCompID(), "SERVER_B");
    EXPECT_EQ(second.schedule.startSeconds, falconfix::test::hms(22, 0));
    EXPECT_EQ(second.schedule.endSeconds, falconfix::test::hms(6, 0));
    EXPECT_FALSE(second.schedule.resetSeqNumsDaily);
}

TEST_F(SessionSettingsTests, RejectsOutOfRangeSessionIndex)
{
    const auto ini = multiSessionCfg();
    EXPECT_THROW(
        (void)falconfix::test::TestConfigBuilder<>::fromString(ini, 2),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, ParsesAlternateInitiatorEndpoints)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "SocketConnectHost1=127.0.0.2\n"
        "SocketConnectPort1=35002\n"
        "SocketConnectHost2=127.0.0.3\n"
        "SocketConnectPort2=35003\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto cfg = falconfix::FIXConfig::parse(ini);
    const auto settings = falconfix::SessionSettings::fromConfig(cfg);

    ASSERT_EQ(settings.size(), 1U);
    const auto &connection = settings.sessions()[0].connection;

    ASSERT_EQ(connection.socketConnectEndpoints.size(), 3U);
    EXPECT_EQ(connection.socketConnectEndpoints[0].host, "127.0.0.1");
    EXPECT_EQ(connection.socketConnectEndpoints[0].port, 35001);
    EXPECT_EQ(connection.socketConnectEndpoints[1].host, "127.0.0.2");
    EXPECT_EQ(connection.socketConnectEndpoints[1].port, 35002);
    EXPECT_EQ(connection.socketConnectEndpoints[2].host, "127.0.0.3");
    EXPECT_EQ(connection.socketConnectEndpoints[2].port, 35003);
}

TEST_F(SessionSettingsTests, RejectsNonConsecutiveAlternateInitiatorEndpoints)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "SocketConnectHost2=127.0.0.3\n"
        "SocketConnectPort2=35003\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, RejectsMismatchedAlternateInitiatorEndpointPair)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "SocketConnectHost1=127.0.0.2\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, AcceptsLegacyAliasesForSSLAndTimeouts)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "ReconnectIntervalMs=1500\n"
        "LogoutTimeoutMs=2500\n"
        "SocketUseSSL=Y\n"
        "SSLCaFile=/tmp/ca.pem\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &connection = settings.sessions()[0].connection;
    EXPECT_EQ(connection.reconnectIntervalMs, 1500U);
    EXPECT_EQ(connection.logoutTimeoutMs, 2500U);
    EXPECT_TRUE(connection.useSSL);
    EXPECT_EQ(connection.sslCaFile, "/tmp/ca.pem");
}

TEST_F(SessionSettingsTests, ParsesSSLServerName)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "UseSSL=Y\n"
        "SSLServerName=fix.broker.local\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &connection = settings.sessions()[0].connection;
    EXPECT_TRUE(connection.useSSL);
    EXPECT_EQ(connection.sslServerName, "fix.broker.local");
}

TEST_F(SessionSettingsTests, ParsesMillisecondsInTimeStamp)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "MillisecondsInTimeStamp=N\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &connection = settings.sessions()[0].connection;
    EXPECT_FALSE(connection.millisecondsInTimeStamp);
}

TEST_F(SessionSettingsTests, ParsesEnableResendRequest)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "EnableResendRequest=N\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &connection = settings.sessions()[0].connection;
    EXPECT_FALSE(connection.enableResendRequest);
}

TEST_F(SessionSettingsTests, ValidateSendingTimeDefaultsToFalse)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &connection = settings.sessions()[0].connection;
    EXPECT_FALSE(connection.validateSendingTime);
}

TEST_F(SessionSettingsTests, CanonicalSettingNamesTakePrecedenceOverLegacyAliases)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "ReconnectInterval=7\n"
        "ReconnectIntervalMs=1500\n"
        "LogoutTimeout=9\n"
        "LogoutTimeoutMs=2500\n"
        "UseSSL=N\n"
        "SocketUseSSL=Y\n"
        "SSLCAFile=/tmp/canonical-ca.pem\n"
        "SSLCaFile=/tmp/legacy-ca.pem\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &connection = settings.sessions()[0].connection;
    EXPECT_EQ(connection.reconnectIntervalMs, 7000U);
    EXPECT_EQ(connection.logoutTimeoutMs, 9000U);
    EXPECT_FALSE(connection.useSSL);
    EXPECT_EQ(connection.sslCaFile, "/tmp/canonical-ca.pem");
}

TEST_F(SessionSettingsTests, ParsesNonStopSessionAndDisablesScheduleWindow)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "NonStopSession=Y\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n"
        "ResetSeqNumsDaily=Y\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &schedule = settings.sessions()[0].schedule;
    EXPECT_TRUE(schedule.nonStopSession);
    EXPECT_FALSE(schedule.enabled);
    EXPECT_EQ(schedule.startSeconds, falconfix::test::hms(9, 0));
    EXPECT_EQ(schedule.endSeconds, falconfix::test::hms(17, 0));
    EXPECT_TRUE(schedule.resetSeqNumsDaily);
}

TEST_F(SessionSettingsTests, ParsesTimeZoneForSchedule)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "TimeZone=America/New_York\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &schedule = settings.sessions()[0].schedule;
    EXPECT_EQ(schedule.timeZone, "America/New_York");
    EXPECT_TRUE(schedule.enabled);
}

TEST_F(SessionSettingsTests, RejectsInvalidScheduleTimeZone)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "TimeZone=FalconFIX/InvalidZone\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, ParsesScheduleWeekdays)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "Weekdays=Mon,Wed,Fri\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &schedule = settings.sessions()[0].schedule;
    EXPECT_TRUE(schedule.useWeekdays);
    EXPECT_EQ(schedule.weekdaysMask, static_cast<uint8_t>((1u << 1) | (1u << 3) | (1u << 5)));
}

TEST_F(SessionSettingsTests, RejectsInvalidWeekdaysToken)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "Weekdays=Mon,Funday\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, RejectsWeekdaysTogetherWithStartDayAndEndDay)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "Weekdays=Mon,Tue,Wed\n"
        "StartDay=Mon\n"
        "EndDay=Fri\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, ParsesStartDayAndEndDayWeeklySchedule)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "StartDay=Mon\n"
        "EndDay=Fri\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &schedule = settings.sessions()[0].schedule;
    EXPECT_TRUE(schedule.useWeeklyWindow);
    EXPECT_EQ(schedule.startDay, 1);
    EXPECT_EQ(schedule.endDay, 5);
}

TEST_F(SessionSettingsTests, RejectsStartDayWithoutEndDay)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "StartDay=Mon\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, RejectsUseLocalTimeTogetherWithTimeZone)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "UseLocalTime=Y\n"
        "TimeZone=America/New_York\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    EXPECT_THROW(
        (void)falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini)),
        std::runtime_error
    );
}

TEST_F(SessionSettingsTests, ParsesUseLocalTimeSchedule)
{
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=initiator\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=memory\n"
        "LogType=none\n"
        "UseLocalTime=Y\n"
        "StartTime=09:00:00\n"
        "EndTime=17:00:00\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"
    };

    const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
    ASSERT_EQ(settings.size(), 1U);

    const auto &schedule = settings.sessions()[0].schedule;
    EXPECT_TRUE(schedule.useLocalTime);
    EXPECT_TRUE(schedule.timeZone.empty());
}
