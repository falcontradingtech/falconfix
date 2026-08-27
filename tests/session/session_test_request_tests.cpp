// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>

#include <socket/boost/boost_socket_context.h>
#include <app/fix_application.h>
#include <runtime/fix_runtime.h>

#include <support/app_test_support.h>

namespace {

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};
    std::atomic<int32_t> testRequestInCount{0};

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

    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.msgType == "1") {
            ++testRequestInCount;
        }
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
SocketAcceptPort=32012
HeartBtInt=1
StoreType=memory
LogType=screen

[SESSION]
BeginString=FIX4.4
SenderCompID=SERVER_TR
TargetCompID=CLIENT_TR
)ini";

static constexpr std::string_view clientCfg = R"ini(
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=32012
HeartBtInt=30
StoreType=memory
LogType=screen

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT_TR
TargetCompID=SERVER_TR
)ini";

} // namespace

TEST(FIXSessionTests, SendsTestRequestAfterInactivity) {
    spdlog::drop_all();

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(serverCfg));
    auto clientSettings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(clientCfg));

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    const auto logonDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < logonDeadline) {
        if (serverApp.onLogonCount.load() > 0 &&
            clientApp.onLogonCount.load() > 0) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_GT(serverApp.onLogonCount.load(), 0);
    EXPECT_GT(clientApp.onLogonCount.load(), 0);

    const auto testRequestDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(6);
    while (std::chrono::steady_clock::now() < testRequestDeadline) {
        if (clientApp.testRequestInCount.load() > 0) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    EXPECT_GT(clientApp.testRequestInCount.load(), 0);

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();
    spdlog::drop_all();
}
