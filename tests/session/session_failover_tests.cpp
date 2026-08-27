// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <support/test_runtime_support.h>

namespace {

std::string makeAcceptorConfig(int32_t port,
                               std::string_view sender,
                               std::string_view target) {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=acceptor\n";
    out << "SocketAcceptHost=127.0.0.1\n";
    out << "SocketAcceptPort=" << port << "\n";
    out << "HeartBtInt=1\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n\n";
    out << "ResetOnLogout=Y\n";
    out << "ResetOnDisconnect=Y\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

std::string makeInitiatorFailoverConfig(int32_t primaryPort,
                                        int32_t secondaryPort,
                                        std::string_view sender,
                                        std::string_view target) {
    std::ostringstream out;
    out << "[DEFAULT]\n";
    out << "ConnectionType=initiator\n";
    out << "SocketConnectHost=127.0.0.1\n";
    out << "SocketConnectPort=" << primaryPort << "\n";
    out << "SocketConnectHost1=127.0.0.1\n";
    out << "SocketConnectPort1=" << secondaryPort << "\n";
    out << "ReconnectInterval=1\n";
    out << "HeartBtInt=1\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n\n";
    out << "ResetOnLogout=Y\n";
    out << "ResetOnDisconnect=Y\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

class FailoverTestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
};

} // namespace

TEST(FIXSessionTests, InitiatorFailsOverToBackupAndReturnsToPrimaryAfterDisconnect) {
    spdlog::drop_all();

    const int32_t primaryPort = falconfix::test::findFreeLoopbackPort();
    const int32_t secondaryPort = falconfix::test::findFreeLoopbackPort();
    ASSERT_GT(primaryPort, 0);
    ASSERT_GT(secondaryPort, 0);
    ASSERT_NE(primaryPort, secondaryPort);

    const auto primaryCfg = makeAcceptorConfig(primaryPort, "SERVER", "CLIENT");
    const auto secondaryCfg = makeAcceptorConfig(secondaryPort, "SERVER", "CLIENT");
    const auto initiatorCfg = makeInitiatorFailoverConfig(primaryPort, secondaryPort, "CLIENT", "SERVER");

    falconfix::socket::BoostSocketContext primarySocketEngine;
    falconfix::socket::BoostSocketContext secondarySocketEngine;
    falconfix::socket::BoostSocketContext clientSocketEngine;
    ASSERT_TRUE(primarySocketEngine.start(2));
    ASSERT_TRUE(secondarySocketEngine.start(2));
    ASSERT_TRUE(clientSocketEngine.start(2));

    FailoverTestApp primaryServerApp;
    FailoverTestApp secondaryServerApp;
    FailoverTestApp clientApp;

    auto primarySettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(primaryCfg)
    );
    auto secondarySettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(secondaryCfg)
    );
    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(initiatorCfg)
    );

    falconfix::FIXRuntime secondaryServer(secondaryServerApp, std::move(secondarySettings), secondarySocketEngine);
    falconfix::FIXRuntime primaryServer(primaryServerApp, std::move(primarySettings), primarySocketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

    auto rc = secondaryServer.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(falconfix::test::waitUntil([&] {
        return secondaryServerApp.onLogonCount.load() >= 1 &&
               clientApp.onLogonCount.load() >= 1;
    }, std::chrono::seconds(8)))
        << "secondary onLogon=" << secondaryServerApp.onLogonCount.load()
        << " client onLogon=" << clientApp.onLogonCount.load();

    EXPECT_EQ(primaryServerApp.onLogonCount.load(), 0);

    secondaryServer.stop();

    ASSERT_TRUE(falconfix::test::waitUntil([&] {
        return clientApp.onLogoutCount.load() >= 1;
    }, std::chrono::seconds(5)))
        << "client onLogout=" << clientApp.onLogoutCount.load();

    rc = primaryServer.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(falconfix::test::waitUntil([&] {
        return primaryServerApp.onLogonCount.load() >= 1 &&
               clientApp.onLogonCount.load() >= 2;
    }, std::chrono::seconds(8)))
        << "primary onLogon=" << primaryServerApp.onLogonCount.load()
        << " client onLogon=" << clientApp.onLogonCount.load()
        << " client onLogout=" << clientApp.onLogoutCount.load();

    clientEngine.stop();
    primaryServer.stop();
    clientSocketEngine.stop();
    secondarySocketEngine.stop();
    primarySocketEngine.stop();
    spdlog::drop_all();
}
