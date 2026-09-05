// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <iomanip>
#include <sstream>
#include <thread>

#include <spdlog/spdlog.h>

#include <app/fix_application.h>
#include <config/fix_config.h>
#include <runtime/fix_runtime.h>
#include <runtime/protocol_factory.h>
#include <logger/message_log.h>
#include <session/session_settings.h>

#include <socket/boost/boost_socket_context.h>
#include <store/memory_message_store.h>

#include <FIX4.4/messages/Heartbeat.h>
#include <FIX4.4/messages/Logout.h>

#include <support/app_test_support.h>
#include <support/test_runtime_support.h>
#include <session/session_time.h>

namespace {

class ScheduleApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};
    std::atomic<int32_t> heartbeatInCount{0};
    std::atomic<int32_t> logoutInCount{0};
    std::atomic<int32_t> logoutOutCount{0};
    std::atomic<int32_t> logonOutCount{0};

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix44::messages::Logon>()) {
            ++logonOutCount;
        } else if (msg.is<fix44::messages::Logout>()) {
            ++logoutOutCount;
        }
        return FF_OK();
    }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }

    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix44::messages::Heartbeat>()) {
            ++heartbeatInCount;
        } else if (msg.is<fix44::messages::Logout>()) {
            ++logoutInCount;
        }
        return FF_OK();
    }
};

struct ScheduleWindow {
    std::string startTime;
    std::string endTime;
};

struct EnginePairConfigs {
    std::string serverCfg;
    std::string clientCfg;
};

bool waitUntilWithRetries(const std::function<bool()> &predicate,
               std::chrono::milliseconds timeout = std::chrono::milliseconds(5000)) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (predicate()) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return predicate();
}

std::string formatHms(int64_t totalSeconds) {
    constexpr int64_t kDaySeconds = 24 * 60 * 60;
    int64_t normalized = totalSeconds % kDaySeconds;
    if (normalized < 0) {
        normalized += kDaySeconds;
    }

    const int64_t hour = normalized / 3600;
    const int64_t minute = (normalized / 60) % 60;
    const int64_t second = normalized % 60;

    std::ostringstream out;
    out << std::setw(2) << std::setfill('0') << hour
        << ':'
        << std::setw(2) << std::setfill('0') << minute
        << ':'
        << std::setw(2) << std::setfill('0') << second;
    return out.str();
}

ScheduleWindow makeShortLiveWindow() {
    const int64_t nowSeconds = fix44::datetime::detail::seconds_since_midnight_utc();
    return {
        .startTime = formatHms(nowSeconds - 2),
        .endTime = formatHms(nowSeconds + 4),
    };
}

ScheduleWindow makeWideOpenWindow() {
    return {
        .startTime = "00:00:00",
        .endTime = "23:59:59",
    };
}

std::string makeEngineIni(std::string_view connectionType,
                          int port,
                          const ScheduleWindow &window,
                          std::string_view senderCompID,
                          std::string_view targetCompID,
                          bool resetSeqNumsDaily) {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=" << connectionType << "\n";
    if (connectionType == "acceptor") {
        out << "SocketAcceptHost=127.0.0.1\n";
        out << "SocketAcceptPort=" << port << "\n";
    } else {
        out << "SocketConnectHost=127.0.0.1\n";
        out << "SocketConnectPort=" << port << "\n";
    }
    out << "HeartBtInt=1\n";
    out << "StoreType=memory\n";
    out << "LogType=none\n";
    out << "StartTime=" << window.startTime << "\n";
    out << "EndTime=" << window.endTime << "\n";
    out << "ResetSeqNumsDaily=" << (resetSeqNumsDaily ? "Y" : "N") << "\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << senderCompID << "\n";
    out << "TargetCompID=" << targetCompID << "\n";
    return out.str();
}

EnginePairConfigs makeMixedScheduleConfigs(int port,
                                           bool serverExpiresFirst,
                                           bool resetSeqNumsDaily) {
    const auto shortWindow = makeShortLiveWindow();
    const auto wideWindow = makeWideOpenWindow();
    return {
        .serverCfg = makeEngineIni(
            "acceptor",
            port,
            serverExpiresFirst ? shortWindow : wideWindow,
            "SERVER_SCHED_NET",
            "CLIENT_SCHED_NET",
            resetSeqNumsDaily
        ),
        .clientCfg = makeEngineIni(
            "initiator",
            port,
            serverExpiresFirst ? wideWindow : shortWindow,
            "CLIENT_SCHED_NET",
            "SERVER_SCHED_NET",
            resetSeqNumsDaily
        ),
    };
}

falconfix::SessionConfig parseScheduleConfig(std::string_view iniText) {
    auto settings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(iniText)
    );

    EXPECT_EQ(settings.size(), 1U);
    return settings.sessions().front();
}

std::unique_ptr<falconfix::SessionBase> makeScheduleSessionFromConfig(falconfix::SessionConfig cfg) {
    static ScheduleApp app;

    auto log = std::make_unique<falconfix::NullMessageLog>();
    auto store = std::make_unique<falconfix::MemoryMessageStore>();

    return falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::move(log),
        std::move(store)
    );
}

std::unique_ptr<falconfix::SessionBase> makeScheduleSession(std::string_view senderCompID,
                                                            std::string_view targetCompID,
                                                            std::string_view startTime,
                                                            std::string_view endTime) {
    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.parameters["StartTime"] = std::string(startTime);
    cfg.parameters["EndTime"] = std::string(endTime);
    cfg.schedule = falconfix::SessionConfig::SessionSchedule::fromParams(cfg.parameters);
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        senderCompID,
        targetCompID
    );

    return makeScheduleSessionFromConfig(std::move(cfg));
}

std::unique_ptr<falconfix::SessionBase> makeScheduleSessionFromIni(std::string_view iniText) {
    return makeScheduleSessionFromConfig(parseScheduleConfig(iniText));
}

int64_t hms(int64_t hour, int64_t minute, int64_t second = 0) {
    return hour * 3600 + minute * 60 + second;
}

static constexpr std::string_view daytimeScheduleCfg = R"ini(
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=32031
HeartBtInt=5
StoreType=memory
LogType=none
StartTime=09:00:00
EndTime=17:00:00
ResetSeqNumsDaily=Y

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT_SCHED_INI
TargetCompID=SERVER_SCHED_INI
)ini";

static constexpr std::string_view overnightScheduleCfg = R"ini(
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=32032
HeartBtInt=5
StoreType=memory
LogType=none
StartTime=22:00:00
EndTime=06:00:00
ResetSeqNumsDaily=N

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT_SCHED_OVN_INI
TargetCompID=SERVER_SCHED_OVN_INI
)ini";

static constexpr std::string_view nonStopSessionCfg = R"ini(
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=32033
HeartBtInt=5
StoreType=memory
LogType=none
NonStopSession=Y
StartTime=09:00:00
EndTime=17:00:00
ResetSeqNumsDaily=Y

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT_SCHED_NONSTOP
TargetCompID=SERVER_SCHED_NONSTOP
)ini";

static constexpr std::string_view timeZoneScheduleCfg = R"ini(
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=32034
HeartBtInt=5
StoreType=memory
LogType=none
TimeZone=America/New_York

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT_SCHED_TZ
TargetCompID=SERVER_SCHED_TZ
)ini";

} // namespace

TEST(SessionScheduleTests, AcceptsTimeInsideWindow) {
    auto session = makeScheduleSession("CLIENT_SCHED_IN", "SERVER_SCHED_IN", "09:00:00", "17:00:00");
    ASSERT_NE(session, nullptr);

    EXPECT_TRUE(session->isWithinSchedule(hms(10, 0)));
    EXPECT_TRUE(session->isWithinSchedule(hms(17, 0)));
}

TEST(SessionScheduleTests, RejectsTimeOutsideWindow) {
    auto session = makeScheduleSession("CLIENT_SCHED_OUT", "SERVER_SCHED_OUT", "09:00:00", "17:00:00");
    ASSERT_NE(session, nullptr);

    EXPECT_FALSE(session->isWithinSchedule(hms(8, 59, 59)));
    EXPECT_FALSE(session->isWithinSchedule(hms(18, 0)));
}

TEST(SessionScheduleTests, AcceptsOvernightWindow) {
    auto session = makeScheduleSession("CLIENT_SCHED_OVN", "SERVER_SCHED_OVN", "22:00:00", "06:00:00");
    ASSERT_NE(session, nullptr);

    EXPECT_TRUE(session->isWithinSchedule(hms(23, 0)));
    EXPECT_TRUE(session->isWithinSchedule(hms(2, 0)));
    EXPECT_FALSE(session->isWithinSchedule(hms(12, 0)));
}

TEST(SessionScheduleTests, LoadsDaytimeScheduleFromIni) {
    const auto cfg = parseScheduleConfig(daytimeScheduleCfg);
    EXPECT_TRUE(cfg.schedule.enabled);
    EXPECT_EQ(cfg.schedule.startSeconds, hms(9, 0));
    EXPECT_EQ(cfg.schedule.endSeconds, hms(17, 0));
    EXPECT_TRUE(cfg.schedule.resetSeqNumsDaily);

    auto session = makeScheduleSessionFromIni(daytimeScheduleCfg);
    ASSERT_NE(session, nullptr);

    EXPECT_TRUE(session->isWithinSchedule(hms(10, 0)));
    EXPECT_FALSE(session->isWithinSchedule(hms(18, 0)));
}

TEST(SessionScheduleTests, LoadsOvernightScheduleFromIni) {
    const auto cfg = parseScheduleConfig(overnightScheduleCfg);
    EXPECT_TRUE(cfg.schedule.enabled);
    EXPECT_EQ(cfg.schedule.startSeconds, hms(22, 0));
    EXPECT_EQ(cfg.schedule.endSeconds, hms(6, 0));
    EXPECT_FALSE(cfg.schedule.resetSeqNumsDaily);

    auto session = makeScheduleSessionFromIni(overnightScheduleCfg);
    ASSERT_NE(session, nullptr);

    EXPECT_TRUE(session->isWithinSchedule(hms(23, 0)));
    EXPECT_TRUE(session->isWithinSchedule(hms(2, 0)));
    EXPECT_FALSE(session->isWithinSchedule(hms(12, 0)));
}

TEST(SessionScheduleTests, NonStopSessionDisablesScheduleWindow) {
    const auto cfg = parseScheduleConfig(nonStopSessionCfg);
    EXPECT_TRUE(cfg.schedule.nonStopSession);
    EXPECT_FALSE(cfg.schedule.enabled);
    EXPECT_EQ(cfg.schedule.startSeconds, hms(9, 0));
    EXPECT_EQ(cfg.schedule.endSeconds, hms(17, 0));
    EXPECT_TRUE(cfg.schedule.resetSeqNumsDaily);

    auto session = makeScheduleSessionFromIni(nonStopSessionCfg);
    ASSERT_NE(session, nullptr);

    EXPECT_TRUE(session->isWithinSchedule(hms(1, 0)));
    EXPECT_TRUE(session->isWithinSchedule(hms(12, 0)));
    EXPECT_TRUE(session->isWithinSchedule(hms(23, 0)));
}

TEST(SessionScheduleTests, TimeZoneScheduleUsesConfiguredZoneForCurrentTime) {
    const int64_t zoneNow = falconfix::session_time::secondsSinceMidnight("America/New_York");

    auto cfg = parseScheduleConfig(timeZoneScheduleCfg);
    cfg.schedule.startSeconds = (zoneNow + 86400 - 2) % 86400;
    cfg.schedule.endSeconds = (zoneNow + 2) % 86400;
    cfg.schedule.enabled = true;

    auto session = makeScheduleSessionFromConfig(std::move(cfg));
    ASSERT_NE(session, nullptr);
    EXPECT_TRUE(session->isWithinSchedule());
}

TEST(SessionScheduleTests, TimeZoneScheduleRejectsCurrentTimeOutsideConfiguredZoneWindow) {
    const int64_t zoneNow = falconfix::session_time::secondsSinceMidnight("America/New_York");

    auto cfg = parseScheduleConfig(timeZoneScheduleCfg);
    cfg.schedule.startSeconds = (zoneNow + 3600) % 86400;
    cfg.schedule.endSeconds = (zoneNow + 7200) % 86400;
    cfg.schedule.enabled = true;

    auto session = makeScheduleSessionFromConfig(std::move(cfg));
    ASSERT_NE(session, nullptr);
    EXPECT_FALSE(session->isWithinSchedule());
}

/*
TEST(SessionScheduleTests, ServerDisconnectsWhenScheduleEnds) {
    spdlog::drop_all();

    const int kPort = falconfix::test::nextTestPort();
    const auto cfgs = makeMixedScheduleConfigs(kPort, true, true);

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    ScheduleApp serverApp;
    ScheduleApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(cfgs.serverCfg)
    );
    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(cfgs.clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
    }, std::chrono::seconds(3)));

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.heartbeatInCount.load() > 0 || clientApp.heartbeatInCount.load() > 0;
    }, std::chrono::seconds(4)));

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogoutCount.load() > 0 && clientApp.onLogoutCount.load() > 0;
    }, std::chrono::seconds(8)))
        << "server onLogout=" << serverApp.onLogoutCount.load()
        << " client onLogout=" << clientApp.onLogoutCount.load()
        << " server logoutIn=" << serverApp.logoutInCount.load()
        << " client logoutIn=" << clientApp.logoutInCount.load();

    EXPECT_GT(serverApp.heartbeatInCount.load(), 0);
    EXPECT_GT(clientApp.heartbeatInCount.load(), 0);
    EXPECT_GT(clientApp.logoutInCount.load(), 0);

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();
    spdlog::drop_all();
}

TEST(SessionScheduleTests, ClientDisconnectsWhenScheduleEnds) {
    spdlog::drop_all();

    const int kPort = falconfix::test::nextTestPort();
    const auto cfgs = makeMixedScheduleConfigs(kPort, false, true);

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    ScheduleApp serverApp;
    ScheduleApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(cfgs.serverCfg)
    );
    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(cfgs.clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
    }, std::chrono::seconds(3)));

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.heartbeatInCount.load() > 0 || clientApp.heartbeatInCount.load() > 0;
    }, std::chrono::seconds(4)));

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogoutCount.load() > 0 && clientApp.onLogoutCount.load() > 0;
    }, std::chrono::seconds(8)))
        << "server onLogout=" << serverApp.onLogoutCount.load()
        << " client onLogout=" << clientApp.onLogoutCount.load()
        << " server logoutIn=" << serverApp.logoutInCount.load()
        << " client logoutIn=" << clientApp.logoutInCount.load();

    EXPECT_GT(serverApp.heartbeatInCount.load(), 0);
    EXPECT_GT(clientApp.heartbeatInCount.load(), 0);
    EXPECT_GT(serverApp.logoutInCount.load(), 0);

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();
    spdlog::drop_all();
}
*/

TEST(SessionScheduleTests, InitiatorReconnectsWhenScheduleReopens) {
    spdlog::drop_all();

    const int kPort = falconfix::test::nextTestPort();
    const auto cfgs = makeMixedScheduleConfigs(kPort, false, false);

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    ScheduleApp serverApp;
    ScheduleApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(cfgs.serverCfg)
    );
    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(cfgs.clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() > 0 &&
               clientApp.onLogonCount.load() > 0;
    }, std::chrono::seconds(3)));

#ifndef FALCONFIX_ENABLE_TEST_API
    const falconfix::SessionID clientSid(
        app_test_support::beginString(),
        "CLIENT_SCHED_NET",
        "SERVER_SCHED_NET"
    );

    auto *clientSession = clientEngine.findSession(clientSid);
    ASSERT_NE(clientSession, nullptr);

    // Force schedule closed.
    clientSession->setScheduleForTest(1, 2);

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogoutCount.load() > 0 &&
               clientApp.onLogoutCount.load() > 0;
    }, std::chrono::seconds(8)))
        << "server onLogout=" << serverApp.onLogoutCount.load()
        << " client onLogout=" << clientApp.onLogoutCount.load()
        << " server logoutIn=" << serverApp.logoutInCount.load()
        << " client logoutIn=" << clientApp.logoutInCount.load()
        << " server logoutOut=" << serverApp.logoutOutCount.load()
        << " client logoutOut=" << clientApp.logoutOutCount.load();

    // Reopen schedule.
    clientSession->setScheduleForTest(0, 86400);

    const bool reconnected = waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() >= 2 &&
               clientApp.onLogonCount.load() >= 2;
    }, std::chrono::seconds(8));

    ASSERT_TRUE(reconnected)
        << "server onLogon=" << serverApp.onLogonCount.load()
        << " client onLogon=" << clientApp.onLogonCount.load()
        << " server logonOut=" << serverApp.logonOutCount.load()
        << " client logonOut=" << clientApp.logonOutCount.load()
        << " server onLogout=" << serverApp.onLogoutCount.load()
        << " client onLogout=" << clientApp.onLogoutCount.load();

    EXPECT_GE(serverApp.onLogoutCount.load(), 1);
    EXPECT_GE(clientApp.onLogoutCount.load(), 1);

    EXPECT_GE(serverApp.onLogonCount.load(), 2);
    EXPECT_GE(clientApp.onLogonCount.load(), 2);
#endif

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();
    spdlog::drop_all();
}
