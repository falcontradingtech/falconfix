// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>

#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include "test_config_builder.h"
#include "test_runtime_support.h"
#include "test_protocols.h"

namespace falconfix::test {

template<typename ServerAppT,
         typename ClientAppT = ServerAppT,
         typename Protocol = DefaultProtocol>
class TestEnginePair {
    using ServerApp = ServerAppT;
    using ClientApp = ClientAppT;

    int32_t m_port{0};

    falconfix::socket::BoostSocketContext m_socketEngine;

    ServerApp m_serverApp;
    ClientApp m_clientApp;

    std::unique_ptr<falconfix::FIXRuntime> m_serverEngine;
    std::unique_ptr<falconfix::FIXRuntime> m_clientEngine;

public:
    TestEnginePair()
        : m_port(nextTestPort()) {}

    bool start(const falconfix::SessionConfig &serverCfg,
               const falconfix::SessionConfig &clientCfg) {
        if (!m_socketEngine.start(2)) {
            return false;
        }

        const std::string serverIni =
            TestConfigBuilder<Protocol>{}.config(serverCfg).build();

        const std::string clientIni =
            TestConfigBuilder<Protocol>{}.config(clientCfg).build();

        auto serverSettings = falconfix::SessionSettings::fromConfig(
            falconfix::FIXConfig::parse(serverIni)
        );

        auto clientSettings = falconfix::SessionSettings::fromConfig(
            falconfix::FIXConfig::parse(clientIni)
        );

        m_serverEngine = std::make_unique<falconfix::FIXRuntime>(
            m_serverApp,
            std::move(serverSettings),
            m_socketEngine
        );

        m_clientEngine = std::make_unique<falconfix::FIXRuntime>(
            m_clientApp,
            std::move(clientSettings),
            m_socketEngine
        );

        FFStatus rc = m_serverEngine->start();
        if (!rc.ok()) {
            return false;
        }

        rc = m_clientEngine->start();
        return rc.ok();
    }

    bool startDefault() {
        auto serverCfg = defaultServerConfig();
        auto clientCfg = defaultClientConfig();
        return start(serverCfg, clientCfg);
    }

    void stop() {
        if (m_clientEngine) {
            m_clientEngine->stop();
        }

        if (m_serverEngine) {
            m_serverEngine->stop();
        }

        m_socketEngine.stop();
    }

    bool waitLogon(std::chrono::milliseconds timeout = std::chrono::seconds(3)) {
        return waitUntilWithRetries([&] {
            return m_serverApp.onLogonCount.load() > 0 &&
                   m_clientApp.onLogonCount.load() > 0;
        }, timeout);
    }

    falconfix::SessionID serverSid() const {
        return {Protocol::BEGIN_STRING, "SERVER", "CLIENT"};
    }

    falconfix::SessionID clientSid() const {
        return {Protocol::BEGIN_STRING, "CLIENT", "SERVER"};
    }

    falconfix::FIXRuntime &serverEngine() noexcept { return *m_serverEngine; }
    falconfix::FIXRuntime &clientEngine() noexcept { return *m_clientEngine; }

    ServerApp &serverApp() noexcept { return m_serverApp; }
    ClientApp &clientApp() noexcept { return m_clientApp; }

    int32_t port() const noexcept { return m_port; }

private:
    falconfix::SessionConfig defaultServerConfig() const {
        falconfix::SessionConfig cfg{};

        cfg.sessionID = falconfix::SessionID(
            Protocol::BEGIN_STRING,
            "SERVER",
            "CLIENT"
        );

        cfg.protocol.version = Protocol::VERSION;

        cfg.connection.connectionType = falconfix::ConnectionType::Acceptor;
        cfg.connection.socketAcceptHost = "127.0.0.1";
        cfg.connection.socketAcceptPort = m_port;
        cfg.connection.heartBtIntMs = 30000;
        cfg.connection.validateSendingTime = false;

        cfg.storage.storeType = falconfix::StoreType::Memory;
        cfg.storage.persistMessages = true;

        cfg.logging.logType = falconfix::LogType::Screen;

        return cfg;
    }

    falconfix::SessionConfig defaultClientConfig() const {
        falconfix::SessionConfig cfg{};

        cfg.sessionID = falconfix::SessionID(
            Protocol::BEGIN_STRING,
            "CLIENT",
            "SERVER"
        );

        cfg.protocol.version = Protocol::VERSION;

        cfg.connection.connectionType = falconfix::ConnectionType::Initiator;
        cfg.connection.socketConnectHost = "127.0.0.1";
        cfg.connection.socketConnectPort = m_port;
        cfg.connection.heartBtIntMs = 30000;
        cfg.connection.validateSendingTime = false;

        cfg.storage.storeType = falconfix::StoreType::Memory;
        cfg.storage.persistMessages = true;

        cfg.logging.logType = falconfix::LogType::Screen;

        return cfg;
    }    

};

} // namespace falconfix::test
