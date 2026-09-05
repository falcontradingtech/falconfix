// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <support/app_test_support.h>

#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <thread>

namespace {

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};

    void onCreate(const falconfix::SessionID &) override {
        ++onCreateCount;
    }

    void onLogon(const falconfix::SessionID &) override {
        ++onLogonCount;
    }

    void onLogout(const falconfix::SessionID &) override {
        ++onLogoutCount;
    }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }
};

static constexpr std::string_view serverCfg = R"ini(
[DEFAULT]
ConnectionType=acceptor
SocketAcceptHost=127.0.0.1
SocketAcceptPort=32011
HeartBtInt=5
StoreType=memory
LogType=screen

[SESSION]
BeginString=FIX4.4
SenderCompID=SERVER
TargetCompID=CLIENT
)ini";

static constexpr std::string_view clientCfg = R"ini(
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=32011
HeartBtInt=5
StoreType=memory
LogType=screen

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT
TargetCompID=SERVER
)ini";

} // namespace

TEST(FIXSessionTests, ClientServerLogonHandshake) {
    spdlog::drop_all();

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings =
        falconfix::SessionSettings::fromConfig(
            falconfix::FIXConfig::parse(serverCfg)
        );

    auto clientSettings =
        falconfix::SessionSettings::fromConfig(
            falconfix::FIXConfig::parse(clientCfg)
        );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);

    while (std::chrono::steady_clock::now() < deadline) {
        if (serverApp.onLogonCount.load() > 0 &&
            clientApp.onLogonCount.load() > 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_EQ(serverApp.onCreateCount.load(), 1);
    EXPECT_EQ(clientApp.onCreateCount.load(), 1);

    EXPECT_GT(serverApp.onLogonCount.load(), 0);
    EXPECT_GT(clientApp.onLogonCount.load(), 0);

    clientEngine.stop();

    const auto logoutDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < logoutDeadline) {
        if (serverApp.onLogoutCount.load() > 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_GT(serverApp.onLogoutCount.load(), 0);

    // Wait for server to fully process LOGOUT and finish all message handlers
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    serverEngine.stop();
    socketEngine.stop();
    spdlog::drop_all();
}
