// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#if defined(FALCONFIX_ENABLE_FIX42) && defined(FALCONFIX_ENABLE_FIX44)

#include <FIX4.2/messages/Heartbeat.h>
#include <FIX4.2/messages/MarketDataIncrementalRefresh.h>
#include <FIX4.2/samples/fix_messages.h>
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

class MultiVersionApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};

    std::atomic<int32_t> heartbeat42Count{0};
    std::atomic<int32_t> heartbeat44Count{0};
    std::atomic<int32_t> app42Count{0};
    std::atomic<int32_t> app44Count{0};

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }

    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix42::messages::Heartbeat>()) {
            ++heartbeat42Count;
        }
        else if (msg.is<fix44::messages::Heartbeat>()) {
            ++heartbeat44Count;
        }
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix42::messages::MarketDataIncrementalRefresh>()) {
            ++app42Count;
        }
        else if (msg.is<fix44::messages::MarketDataIncrementalRefresh>()) {
            ++app44Count;
        }
        return FF_OK();
    }
};

std::string makeServerConfig(int32_t fix42Port, int32_t fix44Port) {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=acceptor\n";
    out << "SocketAcceptHost=127.0.0.1\n";
    out << "HeartBtInt=1\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n";
    out << "ValidateSendingTime=N\n\n";

    out << "[SESSION]\n";
    out << "BeginString=FIX4.2\n";
    out << "SenderCompID=SERVER42\n";
    out << "TargetCompID=CLIENT42\n";
    out << "SocketAcceptPort=" << fix42Port << "\n\n";

    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=SERVER44\n";
    out << "TargetCompID=CLIENT44\n";
    out << "SocketAcceptPort=" << fix44Port << "\n";

    return out.str();
}

std::string makeClientConfig(std::string_view beginString,
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

TEST(MultiVersionTests, ServerHandlesFIX42AndFIX44ClientsOnDifferentPorts) {
    spdlog::drop_all();

    const int32_t fix42Port = falconfix::test::findFreeLoopbackPort();
    const int32_t fix44Port = falconfix::test::findFreeLoopbackPort();
    ASSERT_GT(fix42Port, 0);
    ASSERT_GT(fix44Port, 0);
    ASSERT_NE(fix42Port, fix44Port);

    const auto serverCfg = makeServerConfig(fix42Port, fix44Port);
    const auto client42Cfg = makeClientConfig("FIX4.2", fix42Port, "CLIENT42", "SERVER42");
    const auto client44Cfg = makeClientConfig("FIX4.4", fix44Port, "CLIENT44", "SERVER44");

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext client42SocketEngine;
    falconfix::socket::BoostSocketContext client44SocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(1));
    ASSERT_TRUE(client42SocketEngine.start(1));
    ASSERT_TRUE(client44SocketEngine.start(1));

    MultiVersionApp serverApp;
    MultiVersionApp client42App;
    MultiVersionApp client44App;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );
    auto client42Settings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(client42Cfg)
    );
    auto client44Settings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(client44Cfg)
    );

    ASSERT_EQ(serverSettings.size(), 2U);
    ASSERT_EQ(client42Settings.size(), 1U);
    ASSERT_EQ(client44Settings.size(), 1U);

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime client42Engine(client42App, std::move(client42Settings), client42SocketEngine);
    falconfix::FIXRuntime client44Engine(client44App, std::move(client44Settings), client44SocketEngine);

    auto rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = client42Engine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = client44Engine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() >= 2 &&
               client42App.onLogonCount.load() >= 1 &&
               client44App.onLogonCount.load() >= 1;
    }))
        << "server onLogon=" << serverApp.onLogonCount.load()
        << " client42 onLogon=" << client42App.onLogonCount.load()
        << " client44 onLogon=" << client44App.onLogonCount.load();

    ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
        return serverApp.heartbeat42Count.load() > 0 &&
               serverApp.heartbeat44Count.load() > 0 &&
               client42App.heartbeat42Count.load() > 0 &&
               client44App.heartbeat44Count.load() > 0;
    }))
        << "server hb42=" << serverApp.heartbeat42Count.load()
        << " server hb44=" << serverApp.heartbeat44Count.load()
        << " client42 hb42=" << client42App.heartbeat42Count.load()
        << " client44 hb44=" << client44App.heartbeat44Count.load();

    fix42::messages::MarketDataIncrementalRefresh md42;
    fix44::messages::MarketDataIncrementalRefresh md44;

    rc = md42.setString(fix42::samples::map_samples["MarketDataIncrementalRefresh"]);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = md44.setString(fix44::samples::map_samples["MarketDataIncrementalRefresh"]);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    const falconfix::SessionID serverSid42("FIX4.2", "SERVER42", "CLIENT42");
    const falconfix::SessionID serverSid44("FIX4.4", "SERVER44", "CLIENT44");

    rc = serverEngine.sendToTarget(md42, serverSid42);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = serverEngine.sendToTarget(md44, serverSid44);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
        return client42App.app42Count.load() > 0 &&
               client44App.app44Count.load() > 0;
    }))
        << "client42 app42=" << client42App.app42Count.load()
        << " client44 app44=" << client44App.app44Count.load();

    EXPECT_EQ(client42App.app44Count.load(), 0);
    EXPECT_EQ(client44App.app42Count.load(), 0);

    client44Engine.stop();
    client42Engine.stop();
    serverEngine.stop();
    client44SocketEngine.stop();
    client42SocketEngine.stop();
    serverSocketEngine.stop();
    spdlog::drop_all();
}

#endif
