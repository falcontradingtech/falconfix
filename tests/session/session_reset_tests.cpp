// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>

#include <FIX4.4/messages/Logon.h>

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
                       int32_t heartbeat,
                       bool resetOnLogout,
                       bool resetOnDisconnect,
                       bool resetOnLogon = false,
                       bool ignoreTestRequest = false) {
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
    out << "HeartBtInt=" << heartbeat <<"\n";
    out << "ResetOnLogout=" << (resetOnLogout ? "Y" : "N") << "\n";
    out << "ResetOnDisconnect=" << (resetOnDisconnect ? "Y" : "N") << "\n";
    out << "ResetOnLogon=" << (resetOnLogon ? "Y" : "N") << "\n";
    out << "IgnoreTestRequest=" << (ignoreTestRequest ? "Y" : "N") << "\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

bool waitUntil(const std::function<bool()> &predicate,
               std::chrono::milliseconds timeout = std::chrono::milliseconds(3000)) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (predicate()) {
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return predicate();
}

class TestApp;

bool waitForHandshake(int32_t minLogonCount,
                      TestApp &serverApp,
                      TestApp &clientApp,
                      std::chrono::milliseconds timeout = std::chrono::milliseconds(3000));

void sendBidirectionalAppMessages(falconfix::FIXRuntime& serverEngine,
                                  falconfix::FIXRuntime& clientEngine,
                                  const falconfix::SessionID &serverSid,
                                  const falconfix::SessionID &clientSid) {
    app_test_support::TestMessage serverToClient;
    FFStatus rc = serverEngine.sendToTarget(serverToClient, serverSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    app_test_support::TestMessage clientToServer;
    rc = clientEngine.sendToTarget(clientToServer, clientSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
}

void expectBaselineSeqNums(falconfix::FIXRuntime& engine, const falconfix::SessionID& sid) {
#ifndef FALCONFIX_ENABLE_TEST_API
    auto *session = engine.findSession(sid);
    ASSERT_NE(session, nullptr);
    EXPECT_EQ(session->nextSenderSeqNumForTest(), 2);
    EXPECT_EQ(session->nextTargetSeqNumForTest(), 2);
#else
    (void)engine;
    (void)sid;
#endif
}

void expectSeqNumsAdvanced(falconfix::FIXRuntime& engine, const falconfix::SessionID& sid) {
#ifndef FALCONFIX_ENABLE_TEST_API
    auto *session = engine.findSession(sid);
    ASSERT_NE(session, nullptr);
    EXPECT_GT(session->nextSenderSeqNumForTest(), 2);
    EXPECT_GT(session->nextTargetSeqNumForTest(), 2);
#else
    (void)engine;
    (void)sid;
#endif
}

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};
    std::atomic<int32_t> appInCount{0};
    std::atomic<int32_t> outboundLogonCount{0};
    std::atomic<int64_t> lastOutboundLogonSeqNum{0};

    void onCreate(const falconfix::SessionID &) override {
        ++onCreateCount;
    }

    void onLogon(const falconfix::SessionID &) override {
        ++onLogonCount;
    }

    void onLogout(const falconfix::SessionID &) override {
        ++onLogoutCount;
    }

    FFStatus toAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix44::messages::Logon>()) {
            ++outboundLogonCount;
            lastOutboundLogonSeqNum.store(
                msg.as<fix44::messages::Logon>().getRefHeader().getMsgSeqNum(),
                std::memory_order_release
            );
        }
        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<app_test_support::TestMessage>()) {
            ++appInCount;
        }
        return FF_OK();
    }
};

bool waitForHandshake(int32_t minLogonCount,
                      TestApp &serverApp,
                      TestApp &clientApp,
                      std::chrono::milliseconds timeout) {
    return waitUntil([&] {
        return serverApp.onLogonCount.load() >= minLogonCount &&
               clientApp.onLogonCount.load() >= minLogonCount;
    }, timeout);
}

} // end namespace

TEST(FIXSessionTests, ResetSeqNumFlagOnLogonResetsSession) {
    const int32_t kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT", 30, false, false, true);
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER", 30, false, false, true);

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext clientSocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(2));
    ASSERT_TRUE(clientSocketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );

    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitForHandshake(1, serverApp, clientApp));

#ifndef FALCONFIX_ENABLE_TEST_API
    const falconfix::SessionID clientSid(
        app_test_support::beginString(),
        "CLIENT",
        "SERVER"
    );

    const falconfix::SessionID serverSid(
        app_test_support::beginString(),
        "SERVER",
        "CLIENT"
    );

    EXPECT_EQ(clientEngine.findSession(clientSid)->nextSenderSeqNumForTest(), 2);
    EXPECT_EQ(serverEngine.findSession(serverSid)->nextSenderSeqNumForTest(), 2);
    EXPECT_EQ(clientEngine.findSession(clientSid)->nextTargetSeqNumForTest(), 2);
    EXPECT_EQ(serverEngine.findSession(serverSid)->nextTargetSeqNumForTest(), 2);

#endif

    clientEngine.stop();

    // Wait for the server to finish processing the logout before tearing down.
    // clientEngine.stop() sends the logout asynchronously; if serverEngine.stop()
    // runs concurrently with the server's receive thread still in handleLogoutReceived
    // (which calls connection().close()), the connection gets destroyed from two
    // threads simultaneously, causing a segfault.
    auto logoutDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (std::chrono::steady_clock::now() < logoutDeadline) {
        if (serverApp.onLogoutCount.load() > 0) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    serverEngine.stop();
    clientSocketEngine.stop();
    serverSocketEngine.stop();
}

TEST(FIXSessionTests, ResetOnLogoutResetsSequenceNumbers) {
    const int32_t kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT", 30, true, true);
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER", 30, true, true);

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext clientSocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(2));
    ASSERT_TRUE(clientSocketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );

    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitForHandshake(1, serverApp, clientApp));

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT", "SERVER");
    const falconfix::SessionID serverSid(app_test_support::beginString(), "SERVER", "CLIENT");

    sendBidirectionalAppMessages(serverEngine, clientEngine, serverSid, clientSid);
    ASSERT_TRUE(waitUntil([&] {
        return serverApp.appInCount.load() > 0 && clientApp.appInCount.load() > 0;
    }));

#ifndef FALCONFIX_ENABLE_TEST_API
    expectSeqNumsAdvanced(serverEngine, serverSid);
    expectSeqNumsAdvanced(clientEngine, clientSid);
#endif

    rc = clientEngine.sendLogout(clientSid, "clean logout for reset test");
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitForHandshake(2, serverApp, clientApp, std::chrono::milliseconds(3000)))
        << "Session did not reconnect after clean logout";

#ifndef FALCONFIX_ENABLE_TEST_API
    expectBaselineSeqNums(serverEngine, serverSid);
    expectBaselineSeqNums(clientEngine, clientSid);
#endif

    EXPECT_EQ(serverApp.lastOutboundLogonSeqNum.load(std::memory_order_acquire), 1);
    EXPECT_EQ(clientApp.lastOutboundLogonSeqNum.load(std::memory_order_acquire), 1);

    clientEngine.stop();

    ASSERT_TRUE(waitUntil([&] {
        return serverApp.onLogonCount.load() >= 2 &&
            clientApp.onLogonCount.load() >= 2;
    }, std::chrono::seconds(5)));

    serverEngine.stop();
    clientSocketEngine.stop();
    serverSocketEngine.stop();
}

TEST(FIXSessionTests, ResetOnDisconnectResetsSequenceNumbers) {
    const int32_t kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT", 30, true, true);
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER", 30, true, true);

    falconfix::socket::BoostSocketContext serverSocketEngine;
    falconfix::socket::BoostSocketContext clientSocketEngine;
    ASSERT_TRUE(serverSocketEngine.start(2));
    ASSERT_TRUE(clientSocketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );

    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitForHandshake(1, serverApp, clientApp));

#ifndef FALCONFIX_ENABLE_TEST_API
    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT", "SERVER");
    const falconfix::SessionID serverSid(app_test_support::beginString(), "SERVER", "CLIENT");

    sendBidirectionalAppMessages(serverEngine, clientEngine, serverSid, clientSid);
    ASSERT_TRUE(waitUntil([&] {
        return serverApp.appInCount.load() > 0 && clientApp.appInCount.load() > 0;
    }));

    expectSeqNumsAdvanced(serverEngine, serverSid);
    expectSeqNumsAdvanced(clientEngine, clientSid);

    rc = clientEngine.disconnectForTest(clientSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_TRUE(waitForHandshake(2, serverApp, clientApp, std::chrono::milliseconds(3000)))
        << "Session did not reconnect after transport disconnect";

    expectBaselineSeqNums(serverEngine, serverSid);
    expectBaselineSeqNums(clientEngine, clientSid);
#endif

    clientEngine.stop();
    serverEngine.stop();
    clientSocketEngine.stop();
    serverSocketEngine.stop();
}
