// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>

#include <app/fix_application.h>
#include <config/fix_config.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <FIX4.4/messages/Logon.h>

#include <support/app_test_support.h>
#include <support/test_runtime_support.h>

namespace {

std::filesystem::path makeTempStorePath(const char *suffix) {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    auto path = std::filesystem::temp_directory_path() /
        (std::string("falconfix_session_store_restore_") + suffix + "_" + std::to_string(stamp));
    std::filesystem::create_directories(path);
    return path;
}

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int32_t> onCreateCount{0};
    std::atomic<int32_t> onLogonCount{0};
    std::atomic<int32_t> onLogoutCount{0};
    std::atomic<int32_t> appInCount{0};
    std::atomic<int32_t> outboundLogonCount{0};
    std::atomic<int64_t> lastOutboundLogonSeqNum{0};

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

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
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<app_test_support::TestMessage>()) {
            ++appInCount;
        }
        return FF_OK();
    }
};

std::string makeConfig(std::string_view connectionType,
                       int port,
                       const std::filesystem::path &storePath,
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
    out << "HeartBtInt=5\n";
    out << "StoreType=file\n";
    out << "FileStorePath=" << storePath.string() << "\n";
    out << "LogType=none\n\n";
    out << "[SESSION]\n";
    out << "BeginString=FIX4.4\n";
    out << "SenderCompID=" << sender << "\n";
    out << "TargetCompID=" << target << "\n";
    return out.str();
}

} // namespace

/*
TEST(FIXSessionTests, RestoresSequenceNumbersFromFileStore) {
    spdlog::drop_all();

    const auto storeRoot = makeTempStorePath("seq_restore");
    const int kPort = falconfix::test::nextTestPort();

    const auto serverCfg = makeConfig("acceptor", kPort, storeRoot, "SERVER", "CLIENT");
    const auto clientCfg = makeConfig("initiator", kPort, storeRoot, "CLIENT", "SERVER");

    {
        falconfix::socket::BoostSocketContext serverSocketEngine;
        falconfix::socket::BoostSocketContext clientSocketEngine;
        ASSERT_TRUE(serverSocketEngine.start(2));
        ASSERT_TRUE(clientSocketEngine.start(2));

        TestApp serverApp;
        TestApp clientApp;

        auto serverSettings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(serverCfg));
        auto clientSettings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(clientCfg));

        falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
        falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

        FFStatus rc = serverEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        rc = clientEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
        }));

        const falconfix::SessionID serverSid(app_test_support::beginString(), "SERVER", "CLIENT");
        const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT", "SERVER");

        app_test_support::TestMessage serverToClient;
        rc = serverEngine.sendToTarget(serverToClient, serverSid);
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        app_test_support::TestMessage clientToServer;
        rc = clientEngine.sendToTarget(clientToServer, clientSid);
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.appInCount.load() > 0 && clientApp.appInCount.load() > 0;
        }));

        rc = clientEngine.sendLogout(clientSid, "test shutdown");
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogoutCount.load() > 0 && clientApp.onLogoutCount.load() > 0;
        }));

        clientEngine.stop();
        serverEngine.stop();
        clientSocketEngine.stop();
        serverSocketEngine.stop();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    {
        falconfix::socket::BoostSocketContext serverSocketEngine;
        falconfix::socket::BoostSocketContext clientSocketEngine;
        ASSERT_TRUE(serverSocketEngine.start(2));
        ASSERT_TRUE(clientSocketEngine.start(2));

        TestApp serverApp;
        TestApp clientApp;

        auto serverSettings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(serverCfg));
        auto clientSettings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(clientCfg));

        falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
        falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

        FFStatus rc = serverEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        rc = clientEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
        }));

        EXPECT_GT(serverApp.lastOutboundLogonSeqNum.load(std::memory_order_acquire), 1);
        EXPECT_GT(clientApp.lastOutboundLogonSeqNum.load(std::memory_order_acquire), 1);

        clientEngine.stop();
        serverEngine.stop();
        clientSocketEngine.stop();
        serverSocketEngine.stop();
    }

    std::filesystem::remove_all(storeRoot);
    spdlog::drop_all();
}
*/