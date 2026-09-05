// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <support/test_runtime_support.h>

#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <functional>
#include <sstream>
#include <string>
#include <thread>

namespace {

std::filesystem::path findRepoFile(std::string_view relativePath) {
    constexpr std::string_view kCandidates[] = {
        ".",
        "..",
        "../..",
        "../../..",
        "../../../..",
        "../../../../.."
    };

    for (const auto candidate : kCandidates) {
        const auto path = std::filesystem::path(candidate) / relativePath;
        if (std::filesystem::exists(path)) {
            return std::filesystem::absolute(path);
        }
    }

    return {};
}

std::string makeSSLConfig(std::string_view connectionType,
                          int port,
                          std::string_view sender,
                          std::string_view target,
                          const std::filesystem::path &certFile,
                          const std::filesystem::path &keyFile,
                          bool verifyPeer = false,
                          std::string_view serverName = {}) {
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
    out << "UseSSL=Y\n";
    out << "SSLVerifyPeer=" << (verifyPeer ? "Y" : "N") << "\n";
    if (connectionType == "acceptor") {
        out << "SSLCertFile=" << certFile.string() << "\n";
        out << "SSLKeyFile=" << keyFile.string() << "\n";
    } else if (verifyPeer) {
        out << "SSLCAFile=" << certFile.string() << "\n";
        if (!serverName.empty()) {
            out << "SSLServerName=" << serverName << "\n";
        }
    }
    out << "\n[SESSION]\n";
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

    void onCreate(const falconfix::SessionID &) override { ++onCreateCount; }
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
};

} // namespace

TEST(FIXSessionTests, SSLClientServerLogonHandshake) {
    spdlog::drop_all();

    const auto certFile = findRepoFile("tests/socket/certs/server.crt");
    const auto keyFile = findRepoFile("tests/socket/certs/server.key");
    ASSERT_FALSE(certFile.empty());
    ASSERT_FALSE(keyFile.empty());

    const int kPort = falconfix::test::findFreeLoopbackPort();
    const auto serverCfg = makeSSLConfig("acceptor", kPort, "SERVER_SSL", "CLIENT_SSL", certFile, keyFile);
    const auto clientCfg = makeSSLConfig("initiator", kPort, "CLIENT_SSL", "SERVER_SSL", certFile, keyFile);
    const falconfix::SessionID clientSid("FIX4.4", "CLIENT_SSL", "SERVER_SSL");

    {
        falconfix::socket::BoostSocketContext serverSocketEngine;
        falconfix::socket::BoostSocketContext clientSocketEngine;
        ASSERT_TRUE(serverSocketEngine.start(2));
        ASSERT_TRUE(clientSocketEngine.start(2));

        TestApp serverApp;
        TestApp clientApp;

        auto serverSettings =
            falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(serverCfg));
        auto clientSettings =
            falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(clientCfg));

        ASSERT_FALSE(serverSettings.sessions().empty());
        ASSERT_FALSE(clientSettings.sessions().empty());
        EXPECT_TRUE(serverSettings.sessions().front().connection.useSSL);
        EXPECT_TRUE(clientSettings.sessions().front().connection.useSSL);
        EXPECT_EQ(serverSettings.sessions().front().connection.sslCertFile, certFile.string());
        EXPECT_EQ(serverSettings.sessions().front().connection.sslKeyFile, keyFile.string());

        falconfix::FIXRuntime serverEngine(
            serverApp, std::move(serverSettings), serverSocketEngine);
        falconfix::FIXRuntime clientEngine(
            clientApp, std::move(clientSettings), clientSocketEngine);

        FFStatus rc = serverEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        rc = clientEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0; }, 6,
            std::chrono::milliseconds(5000)));
        
        EXPECT_EQ(serverApp.onCreateCount.load(), 1);
        EXPECT_EQ(clientApp.onCreateCount.load(), 1);
        EXPECT_GT(serverApp.onLogonCount.load(), 0);
        EXPECT_GT(clientApp.onLogonCount.load(), 0);

        rc = clientEngine.sendLogout(clientSid, "test shutdown");
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogoutCount.load() > 0 && clientApp.onLogoutCount.load() > 0;
        }, 6, std::chrono::milliseconds(5000)));

        clientEngine.stop();
        serverEngine.stop();
        clientSocketEngine.stop();
        serverSocketEngine.stop();
    }
    spdlog::drop_all();
}

TEST(FIXSessionTests, SSLInitiatorReconnectsAfterDisconnect) {
    spdlog::drop_all();

    const auto certFile = findRepoFile("tests/socket/certs/server.crt");
    const auto keyFile = findRepoFile("tests/socket/certs/server.key");
    ASSERT_FALSE(certFile.empty());
    ASSERT_FALSE(keyFile.empty());

    const int kPort = falconfix::test::findFreeLoopbackPort();
    const auto serverCfg = makeSSLConfig("acceptor", kPort, "SERVER_SSL_RE", "CLIENT_SSL_RE", certFile, keyFile);
    const auto clientCfg = makeSSLConfig("initiator", kPort, "CLIENT_SSL_RE", "SERVER_SSL_RE", certFile, keyFile);

    {
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

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
        }));

        const falconfix::SessionID clientSid("FIX4.4", "CLIENT_SSL_RE", "SERVER_SSL_RE");

        rc = clientEngine.disconnectForTest(clientSid);
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogonCount.load() >= 2 && clientApp.onLogonCount.load() >= 2;
        }))
            << "server onLogon=" << serverApp.onLogonCount.load()
            << " client onLogon=" << clientApp.onLogonCount.load()
            << " server onLogout=" << serverApp.onLogoutCount.load()
            << " client onLogout=" << clientApp.onLogoutCount.load();

        EXPECT_GE(serverApp.onLogoutCount.load(), 1);
        EXPECT_GE(clientApp.onLogoutCount.load(), 1);

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
    spdlog::drop_all();
}

TEST(FIXSessionTests, SSLClientVerifiesConfiguredServerName) {
    spdlog::drop_all();

    const auto certFile = findRepoFile("tests/socket/certs/server.crt");
    const auto keyFile = findRepoFile("tests/socket/certs/server.key");
    ASSERT_FALSE(certFile.empty());
    ASSERT_FALSE(keyFile.empty());

    const int kPort = falconfix::test::findFreeLoopbackPort();
    const auto serverCfg = makeSSLConfig("acceptor", kPort, "SERVER_SSL_V", "CLIENT_SSL_V", certFile, keyFile);
    const auto clientCfg = makeSSLConfig(
        "initiator", kPort, "CLIENT_SSL_V", "SERVER_SSL_V", certFile, keyFile, true, "localhost");

    {
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

        ASSERT_EQ(clientSettings.sessions().front().connection.sslServerName, "localhost");
        ASSERT_TRUE(clientSettings.sessions().front().connection.sslVerifyPeer);

        falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), serverSocketEngine);
        falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), clientSocketEngine);

        FFStatus rc = serverEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        rc = clientEngine.start();
        ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

        ASSERT_TRUE(falconfix::test::waitUntilWithRetries([&] {
            return serverApp.onLogonCount.load() > 0 && clientApp.onLogonCount.load() > 0;
        }));

        clientEngine.stop();
        serverEngine.stop();
        clientSocketEngine.stop();
        serverSocketEngine.stop();
    }

    spdlog::drop_all();
}
