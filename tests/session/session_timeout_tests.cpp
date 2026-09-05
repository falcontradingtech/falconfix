// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <functional>

#include <spdlog/spdlog.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <support/app_test_support.h>
#include <support/test_runtime_support.h>

namespace {

std::string makeConfig(std::string_view connectionType,
                       int32_t port,
                       std::string_view sender,
                       std::string_view target,
                       bool disableHeartbeat,
                       bool ignoreTestRequest) {
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
    out << "LogType=screen\n";
    out << "DisableHeartbeat=" << (disableHeartbeat ? "Y" : "N") << "\n";
    out << "IgnoreTestRequest=" << (ignoreTestRequest ? "Y" : "N") << "\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};
    std::atomic<int32_t> testRequestInCount{0};

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
};

bool waitUntilWithRetries(const std::function<bool()> &predicate,
               std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (predicate()) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return predicate();
}

} // namespace

TEST(FIXSessionTests, TestRequestTimeoutDisconnectsSession) {
    const int32_t kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT", true, true);
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER", false, false);

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext clientSocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(2));
    ASSERT_TRUE(clientSocketEngine.start(2));

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

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
    }, std::chrono::seconds(3)));

    EXPECT_TRUE(waitUntilWithRetries([&] {
        return clientApp.onLogoutCount.load() > 0 || serverApp.onLogoutCount.load() > 0;
    }, std::chrono::seconds(5)));

    serverEngine.stop();
    clientEngine.stop();
    clientSocketEngine.stop();
    serverSocketEngine.stop();
}

TEST(FIXSessionTests, InitiatorReconnectsAfterTestRequestTimeout) {
    const int32_t kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT", true, true);
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER", false, false);

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext clientSocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(2));
    ASSERT_TRUE(clientSocketEngine.start(2));

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

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
    }, std::chrono::seconds(3)));

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return clientApp.onLogoutCount.load() > 0 || serverApp.onLogoutCount.load() > 0;
    }, std::chrono::seconds(5)));

    ASSERT_TRUE(waitUntilWithRetries([&] {
        return serverApp.onLogonCount.load() >= 2 && clientApp.onLogonCount.load() >= 2;
    }, std::chrono::seconds(8)))
        << "server onLogon=" << serverApp.onLogonCount.load()
        << " client onLogon=" << clientApp.onLogonCount.load()
        << " server onLogout=" << serverApp.onLogoutCount.load()
        << " client onLogout=" << clientApp.onLogoutCount.load();

    serverEngine.stop();
    clientEngine.stop();
    clientSocketEngine.stop();
    serverSocketEngine.stop();
}
