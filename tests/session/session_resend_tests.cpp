// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <spdlog/spdlog.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <FIX4.4/messages/ResendRequest.h>
#include <FIX4.4/messages/SequenceReset.h>
#include <FIX4.4/messages/Heartbeat.h>

#include <support/app_test_support.h>
#include <support/test_runtime_support.h>

namespace {

std::string makeConfig(std::string_view connectionType,
                       int port,
                       std::string_view sender,
                       std::string_view target) {
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
    out << "HeartBtInt=30\n";
    out << "StoreType=memory\n";
    out << "LogType=screen\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

class TestApp final : public falconfix::FIXApplication {
public:
    struct ReplayFrame {
        bool gapFill{false};
        int64_t seqNum{0};
        int64_t newSeqNo{0};
        bool possDup{false};
    };

    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};
    std::atomic<int32_t> testRequestInCount{0};
    std::atomic<int32_t> resendRequestInCount{0};
    std::atomic<int32_t> sequenceResetInCount{0};
    std::atomic<int32_t> sequenceResetOutCount{0};
    std::atomic<int32_t> appReplayOutCount{0};
    std::atomic<int64_t> lastResendBeginSeqNo{0};
    std::atomic<int64_t> lastResendEndSeqNo{0};

    std::mutex replayFramesMutex;
    std::vector<ReplayFrame> replayFrames;

    void clearReplayFrames() {
        std::lock_guard<std::mutex> lock(replayFramesMutex);
        replayFrames.clear();
        resendRequestInCount.store(0);
        sequenceResetInCount.store(0);
        sequenceResetOutCount.store(0);
        appReplayOutCount.store(0);
        lastResendBeginSeqNo.store(0);
        lastResendEndSeqNo.store(0);
    }

    std::vector<ReplayFrame> snapshotReplayFrames() {
        std::lock_guard<std::mutex> lock(replayFramesMutex);
        return replayFrames;
    }

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
        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix44::messages::ResendRequest>()) {
            ++resendRequestInCount;

            const auto &rr = msg.as<fix44::messages::ResendRequest>();
            lastResendBeginSeqNo.store(rr.getBeginSeqNo());
            lastResendEndSeqNo.store(rr.getEndSeqNo());
        }

        if (msg.is<fix44::messages::SequenceReset>()) {
            ++sequenceResetInCount;

            const auto &sr = msg.as<fix44::messages::SequenceReset>();
            ReplayFrame frame;
            frame.gapFill = sr.getGapFillFlag();
            frame.seqNum = sr.getRefHeader().getMsgSeqNum();
            frame.newSeqNo = sr.getNewSeqNo();
            frame.possDup = sr.getRefHeader().hasPossDupFlag() && sr.getRefHeader().getPossDupFlag();

            std::lock_guard<std::mutex> lock(replayFramesMutex);
            replayFrames.push_back(frame);
        }

        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<app_test_support::TestMessage>()) {
            const auto &appMsg = msg.as<app_test_support::TestMessage>();
            const bool possDup = appMsg.getRefHeader().hasPossDupFlag() && appMsg.getRefHeader().getPossDupFlag();

            if (possDup) {
                ++appReplayOutCount;

                ReplayFrame frame;
                frame.gapFill = false;
                frame.seqNum = appMsg.getRefHeader().getMsgSeqNum();
                frame.newSeqNo = 0;
                frame.possDup = true;

                std::lock_guard<std::mutex> lock(replayFramesMutex);
                replayFrames.push_back(frame);
            }
        }

        return FF_OK();
    }
};

} // end namespace

TEST(FIXSessionTests, ResendRequestReplaysApplicationMessageWithPossDup) {
    const int kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT");
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER");

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );

    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);

    while (std::chrono::steady_clock::now() < deadline) {
        if (serverApp.onLogonCount.load() > 0 &&
            clientApp.onLogonCount.load() > 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_GT(serverApp.onLogonCount.load(), 0);
    ASSERT_GT(clientApp.onLogonCount.load(), 0);

#ifndef FALCONFIX_ENABLE_TEST_API
    const falconfix::SessionID serverSid(
        app_test_support::beginString(),
        "SERVER",
        "CLIENT"
    );

    const falconfix::SessionID clientSid(
        app_test_support::beginString(),
        "CLIENT",
        "SERVER"
    );

    auto *serverSession = serverEngine.findSession(serverSid);
    auto *clientSession = clientEngine.findSession(clientSid);
    ASSERT_NE(serverSession, nullptr);
    ASSERT_NE(clientSession, nullptr);

    app_test_support::TestMessage msg1;
    rc = serverEngine.sendToTarget(msg1, serverSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    app_test_support::TestMessage msg2;
    rc = serverEngine.sendToTarget(msg2, serverSid); // 34=2
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    app_test_support::TestMessage msg3;
    rc = serverEngine.sendToTarget(msg3, serverSid); // 34=3
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < deadline) {
        if (clientSession->nextTargetSeqNumForTest() >= 5) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ASSERT_EQ(clientSession->nextTargetSeqNumForTest(), 5);

    // force artificial gap: client expects 3 again
    clientSession->setNextTargetSeqNumForTest(3);

    app_test_support::TestMessage msg4;
    rc = serverEngine.sendToTarget(msg4, serverSid); // 34=4
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    const auto resendDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (std::chrono::steady_clock::now() < resendDeadline) {
        const auto resendCount = serverApp.resendRequestInCount.load();
        const auto nextTarget = clientSession->nextTargetSeqNumForTest();

        if ((resendCount > 0 && nextTarget >= 5) || resendCount > 5) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    const auto resendCount = serverApp.resendRequestInCount.load();
    const auto sequenceResetCount = clientApp.sequenceResetInCount.load();
    const auto nextTarget = clientSession->nextTargetSeqNumForTest();
    const auto nextSender = clientSession->nextSenderSeqNumForTest();

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();

    EXPECT_EQ(resendCount, 1)
        << "client should send a single ResendRequest for seq 3-4";

    EXPECT_EQ(sequenceResetCount, 0)
        << "server has seq 3 and 4, so no gap-fill should be needed";

    EXPECT_EQ(nextTarget, 6)
        << "client should replay seq 3, 4 and 5, then expect seq 6";

    EXPECT_LT(nextSender, 20)
        << "client sender seqnum exploded, indicating resend loop";
#endif

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();


}

TEST(FIXSessionTests, ResendRequestGroupsGapFillsAndReplaysAppMessages) {
    const int kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER", "CLIENT");
    const auto clientCfg = makeConfig("initiator", kPort, "CLIENT", "SERVER");

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );

    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < deadline) {
        if (serverApp.onLogonCount.load() > 0 &&
            clientApp.onLogonCount.load() > 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_GT(serverApp.onLogonCount.load(), 0);
    ASSERT_GT(clientApp.onLogonCount.load(), 0);

#ifndef FALCONFIX_ENABLE_TEST_API
    const falconfix::SessionID serverSid(
        app_test_support::beginString(),
        "SERVER",
        "CLIENT"
    );

    const falconfix::SessionID clientSid(
        app_test_support::beginString(),
        "CLIENT",
        "SERVER"
    );

    auto *serverSession = serverEngine.findSession(serverSid);
    auto *clientSession = clientEngine.findSession(clientSid);
    ASSERT_NE(serverSession, nullptr);
    ASSERT_NE(clientSession, nullptr);

    // Keep client from opening automatic gap flow while we prepare server history.
    clientSession->setNextTargetSeqNumForTest(100);

    // Build mixed range in server history: 2=admin, 3=missing, 4=app, 5=admin.
    fix44::messages::Heartbeat admin2;
    rc = serverEngine.sendToTarget(admin2, serverSid); // seq=2
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    serverSession->setNextSenderSeqNumForTest(4); // leave seq=3 missing

    app_test_support::TestMessage app4;
    rc = serverEngine.sendToTarget(app4, serverSid); // seq=4
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    fix44::messages::Heartbeat admin5;
    rc = serverEngine.sendToTarget(admin5, serverSid); // seq=5
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    // Let preloaded history messages drain before rewinding the client sequence.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    clientApp.clearReplayFrames();
    clientSession->setNextTargetSeqNumForTest(2);

    fix44::messages::ResendRequest rr;
    rr.setBeginSeqNo(2);
    rr.setEndSeqNo(5);
    rc = clientEngine.sendToTarget(rr, clientSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (std::chrono::steady_clock::now() < deadline) {
        if (serverApp.resendRequestInCount.load() >= 1 &&
            clientApp.appReplayOutCount.load() >= 1 &&
            clientSession->nextTargetSeqNumForTest() >= 6) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    const auto replayFrames = clientApp.snapshotReplayFrames();
    const auto nextTarget = clientSession->nextTargetSeqNumForTest();

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();

    ASSERT_EQ(serverApp.resendRequestInCount.load(), 1);
    EXPECT_EQ(serverApp.lastResendBeginSeqNo.load(), 2);
    EXPECT_EQ(serverApp.lastResendEndSeqNo.load(), 5);

    ASSERT_GE(replayFrames.size(), 3U);

    EXPECT_TRUE(replayFrames[0].gapFill);
    EXPECT_EQ(replayFrames[0].seqNum, 2);
    EXPECT_EQ(replayFrames[0].newSeqNo, 4);

    EXPECT_FALSE(replayFrames[1].gapFill);
    EXPECT_EQ(replayFrames[1].seqNum, 4);
    EXPECT_TRUE(replayFrames[1].possDup);

    EXPECT_TRUE(replayFrames[2].gapFill);
    EXPECT_EQ(replayFrames[2].seqNum, 5);
    EXPECT_EQ(replayFrames[2].newSeqNo, 6);

    EXPECT_EQ(nextTarget, 6)
        << "client should advance to seq 6 after processing range 2-5 with gap-fills and replay";
#endif

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();
}

TEST(FIXSessionTests, DisableEnableResendRequestSuppressesAutomaticResendRequest) {
    const int kPort = falconfix::test::nextTestPort();
    const auto serverCfg = makeConfig("acceptor", kPort, "SERVER_NR", "CLIENT_NR");

    std::ostringstream clientCfgOut;
    clientCfgOut << "[DEFAULT]\n";
    clientCfgOut << "ConnectionType=initiator\n";
    clientCfgOut << "SocketConnectHost=127.0.0.1\n";
    clientCfgOut << "SocketConnectPort=" << kPort << "\n";
    clientCfgOut << "HeartBtInt=30\n";
    clientCfgOut << "EnableResendRequest=N\n";
    clientCfgOut << "StoreType=memory\n";
    clientCfgOut << "LogType=screen\n\n";
    clientCfgOut << "[SESSION]\n";
    clientCfgOut << "BeginString=FIX4.4\n";
    clientCfgOut << "SenderCompID=CLIENT_NR\n";
    clientCfgOut << "TargetCompID=SERVER_NR\n";
    const auto clientCfg = clientCfgOut.str();

    falconfix::socket::BoostSocketContext socketEngine;
    ASSERT_TRUE(socketEngine.start(2));

    TestApp serverApp;
    TestApp clientApp;

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(serverCfg)
    );

    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(clientCfg)
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    FFStatus rc = serverEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < deadline) {
        if (serverApp.onLogonCount.load() > 0 &&
            clientApp.onLogonCount.load() > 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_GT(serverApp.onLogonCount.load(), 0);
    ASSERT_GT(clientApp.onLogonCount.load(), 0);

#ifndef FALCONFIX_ENABLE_TEST_API
    const falconfix::SessionID serverSid(
        app_test_support::beginString(),
        "SERVER_NR",
        "CLIENT_NR"
    );

    const falconfix::SessionID clientSid(
        app_test_support::beginString(),
        "CLIENT_NR",
        "SERVER_NR"
    );

    auto *clientSession = clientEngine.findSession(clientSid);
    ASSERT_NE(clientSession, nullptr);

    app_test_support::TestMessage msg1;
    rc = serverEngine.sendToTarget(msg1, serverSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    app_test_support::TestMessage msg2;
    rc = serverEngine.sendToTarget(msg2, serverSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    app_test_support::TestMessage msg3;
    rc = serverEngine.sendToTarget(msg3, serverSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < deadline) {
        if (clientSession->nextTargetSeqNumForTest() >= 5) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ASSERT_EQ(clientSession->nextTargetSeqNumForTest(), 5);

    clientApp.clearReplayFrames();
    clientSession->setNextTargetSeqNumForTest(3);

    app_test_support::TestMessage msg4;
    rc = serverEngine.sendToTarget(msg4, serverSid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    const auto resendCount = serverApp.resendRequestInCount.load();
    const auto nextTarget = clientSession->nextTargetSeqNumForTest();

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();

    EXPECT_EQ(resendCount, 0)
        << "client should not emit automatic ResendRequest when EnableResendRequest=N";

    EXPECT_EQ(nextTarget, 3)
        << "client should keep waiting for the missing sequence when auto resend is disabled";
#endif

    clientEngine.stop();
    serverEngine.stop();
    socketEngine.stop();
}
