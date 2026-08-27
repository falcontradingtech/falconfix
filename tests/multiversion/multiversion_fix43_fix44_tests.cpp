// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#if defined(FALCONFIX_ENABLE_FIX43) && defined(FALCONFIX_ENABLE_FIX44)

#include <FIX4.3/messages/Heartbeat.h>
#include <FIX4.3/messages/MarketDataIncrementalRefresh.h>
#include <FIX4.3/samples/fix_messages.h>
#include <FIX4.4/messages/Heartbeat.h>
#include <FIX4.4/messages/MarketDataIncrementalRefresh.h>
#include <FIX4.4/samples/fix_messages.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>
#include <utils/error_codes.h>

#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <sstream>
#include <string>

#include "../support/test_runtime_support.h"

namespace {

class MultiVersion43x44App final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};

    std::atomic<int32_t> heartbeat43Count{0};
    std::atomic<int32_t> heartbeat44Count{0};
    std::atomic<int32_t> app43Count{0};
    std::atomic<int32_t> app44Count{0};

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }

    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix43::messages::Heartbeat>()) {
            ++heartbeat43Count;
        }
        else if (msg.is<fix44::messages::Heartbeat>()) {
            ++heartbeat44Count;
        }
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix43::messages::MarketDataIncrementalRefresh>()) {
            ++app43Count;
        }
        else if (msg.is<fix44::messages::MarketDataIncrementalRefresh>()) {
            ++app44Count;
        }
        return FF_OK();
    }
};

std::string makeServerConfig43x44(int32_t fix43Port, int32_t fix44Port) {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=acceptor\n";
    out << "SocketAcceptHost=127.0.0.1\n";
    out << "HeartBtInt=1\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n";
    out << "ValidateSendingTime=N\n\n";

    out << "[SESSION]\n";
    out << "BeginString=FIX4.3\n";
    out << "SenderCompID=SERVER43\n";
    out << "TargetCompID=CLIENT43\n";
    out << "SocketAcceptPort=" << fix43Port << "\n\n";

    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=SERVER44\n";
    out << "TargetCompID=CLIENT44\n";
    out << "SocketAcceptPort=" << fix44Port << "\n";

    return out.str();
}

std::string makeClientConfig43x44(std::string_view beginString,
                                  int32_t port,
                                  std::string_view sender,
                                  std::string_view target) {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=initiator\n";
    out << "SocketConnectHost=127.0.0.1\n";
    out << "SocketConnectPort=" << port << "\n";
    out << "HeartBtInt=1\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n";
    out << "ValidateSendingTime=N\n\n";

    out << "[SESSION]\n";
    out << "BeginString=" << beginString << "\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

} // namespace

TEST(MultiVersionTests, ServerHandlesFIX43AndFIX44ClientsOnDifferentPorts) {
    spdlog::drop_all();

    const int32_t fix43Port = falconfix::test::findFreeLoopbackPort();
    const int32_t fix44Port = falconfix::test::findFreeLoopbackPort();
    ASSERT_GT(fix43Port, 0);
    ASSERT_GT(fix44Port, 0);
    ASSERT_NE(fix43Port, fix44Port);

    const auto serverCfg = makeServerConfig43x44(fix43Port, fix44Port);
    const auto client43Cfg = makeClientConfig43x44("FIX4.3", fix43Port, "CLIENT43", "SERVER43");
    const auto client44Cfg = makeClientConfig43x44("FIX4.4", fix44Port, "CLIENT44", "SERVER44");

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext client43SocketEngine;
    falconfix::socket::BoostSocketContext client44SocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(1));
    ASSERT_TRUE(client43SocketEngine.start(1));
    ASSERT_TRUE(client44SocketEngine.start(1));

    MultiVersion43x44App serverApp;
    MultiVersion43x44App client43App;
    MultiVersion43x44App client44App;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );
    auto client43Settings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(client43Cfg)
    );
    auto client44Settings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(client44Cfg)
    );

    ASSERT_EQ(serverSettings.size(), 2U);
    ASSERT_EQ(client43Settings.size(), 1U);
    ASSERT_EQ(client44Settings.size(), 1U);

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime client43Engine(client43App, std::move(client43Settings), client43SocketEngine);
    falconfix::FIXRuntime client44Engine(client44App, std::move(client44Settings), client44SocketEngine);

    auto rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = client43Engine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = client44Engine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(falconfix::test::waitUntil([&] {
        return serverApp.onLogonCount.load() >= 2 &&
               client43App.onLogonCount.load() >= 1 &&
               client44App.onLogonCount.load() >= 1;
    }, std::chrono::seconds(5)));

    ASSERT_TRUE(falconfix::test::waitUntil([&] {
        return serverApp.heartbeat43Count.load() > 0 &&
               serverApp.heartbeat44Count.load() > 0 &&
               client43App.heartbeat43Count.load() > 0 &&
               client44App.heartbeat44Count.load() > 0;
    }, std::chrono::seconds(5)));

    fix43::messages::MarketDataIncrementalRefresh md43;
    fix44::messages::MarketDataIncrementalRefresh md44;

    rc = md43.setString(fix43::samples::map_samples["MarketDataIncrementalRefresh"]);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = md44.setString(fix44::samples::map_samples["MarketDataIncrementalRefresh"]);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    const falconfix::SessionID serverSid43("FIX4.3", "SERVER43", "CLIENT43");
    const falconfix::SessionID serverSid44("FIX4.4", "SERVER44", "CLIENT44");

    rc = serverEngine.sendToTarget(md43, serverSid43);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = serverEngine.sendToTarget(md44, serverSid44);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(falconfix::test::waitUntil([&] {
        return client43App.app43Count.load() > 0 &&
               client44App.app44Count.load() > 0;
    }, std::chrono::seconds(5)));

    EXPECT_EQ(client43App.app44Count.load(), 0);
    EXPECT_EQ(client44App.app43Count.load(), 0);

    client44Engine.stop();
    client43Engine.stop();
    serverEngine.stop();
    client44SocketEngine.stop();
    client43SocketEngine.stop();
    serverSocketEngine.stop();
    spdlog::drop_all();
}

#endif
