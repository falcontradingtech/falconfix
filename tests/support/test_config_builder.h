// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include <session/session_settings.h>

#include "test_protocols.h"

namespace falconfix::test {

inline const char *yn(bool value) noexcept {
    return value ? "Y" : "N";
}

inline const char *toIni(falconfix::StoreType v) noexcept {
    switch (v) {
    case falconfix::StoreType::Memory: return "memory";
    case falconfix::StoreType::File:   return "file";
    case falconfix::StoreType::None:   return "none";
    default: return "none";
    }
}

inline const char *toIni(falconfix::LogType v) noexcept {
    switch (v) {
    case falconfix::LogType::Screen: return "screen";
    case falconfix::LogType::File:   return "file";
    case falconfix::LogType::None:   return "none";
    default: return "none";
    }
}

inline std::string formatHms(int64_t totalSeconds) {
    constexpr int64_t kDaySeconds = 24 * 60 * 60;
    int64_t normalized = totalSeconds % kDaySeconds;
    if (normalized < 0) {
        normalized += kDaySeconds;
    }

    const int64_t hour = normalized / 3600;
    const int64_t minute = (normalized / 60) % 60;
    const int64_t second = normalized % 60;

    std::ostringstream out;
    out << std::setw(2) << std::setfill('0') << hour
        << ':'
        << std::setw(2) << std::setfill('0') << minute
        << ':'
        << std::setw(2) << std::setfill('0') << second;
    return out.str();
}

template<typename Protocol = DefaultProtocol>
class TestConfigBuilder {
    falconfix::SessionConfig m_cfg{};

public:
    static TestConfigBuilder fromConfig(const falconfix::FIXConfig &cfg,
                                        std::size_t sessionIndex = 0) {
        auto settings = falconfix::SessionSettings::fromConfig(cfg);
        if (settings.empty()) {
            throw std::runtime_error("TestConfigBuilder::fromConfig found no sessions");
        }
        if (sessionIndex >= settings.size()) {
            throw std::runtime_error("TestConfigBuilder::fromConfig session index out of range");
        }

        TestConfigBuilder builder;
        builder.m_cfg = settings.sessions()[sessionIndex];
        return builder;
    }

    static TestConfigBuilder fromString(std::string_view iniText,
                                        std::size_t sessionIndex = 0) {
        return fromConfig(falconfix::FIXConfig::parse(iniText), sessionIndex);
    }

    TestConfigBuilder &config(const falconfix::SessionConfig &cfg) {
        m_cfg = cfg;
        return *this;
    }

    const falconfix::SessionConfig &config() const noexcept {
        return m_cfg;
    }

    TestConfigBuilder &acceptor(int32_t port,
                                std::string sender = "SERVER",
                                std::string target = "CLIENT") {
        m_cfg.connection.connectionType = falconfix::ConnectionType::Acceptor;
        m_cfg.connection.socketAcceptHost = "127.0.0.1";
        m_cfg.connection.socketAcceptPort = port;
        m_cfg.sessionID = {std::string(Protocol::BEGIN_STRING), std::move(sender), std::move(target)}; 
        return *this;
    }

    TestConfigBuilder &initiator(int32_t port,
                                 std::string sender = "CLIENT",
                                 std::string target = "SERVER") {
        m_cfg.connection.connectionType = falconfix::ConnectionType::Initiator;
        m_cfg.connection.socketConnectHost = "127.0.0.1";
        m_cfg.connection.socketConnectPort = port;
        m_cfg.sessionID = {std::string(Protocol::BEGIN_STRING), std::move(sender), std::move(target)}; 
        return *this;
    }

    std::string build() const {
        std::ostringstream out;

        const auto &c = m_cfg.connection;
        const auto &s = m_cfg.sessionID;
        const auto &st = m_cfg.storage;
        const auto &l = m_cfg.logging;
        const auto &sch = m_cfg.schedule;

        out << "[DEFAULT]\n";

        out << "ConnectionType="
            << (c.connectionType == falconfix::ConnectionType::Acceptor ? "acceptor" : "initiator")
            << "\n";

        if (c.connectionType == falconfix::ConnectionType::Acceptor) {
            out << "SocketAcceptHost=" << c.socketAcceptHost << "\n";
            out << "SocketAcceptPort=" << c.socketAcceptPort << "\n";
        }
        else {
            out << "SocketConnectHost=" << c.socketConnectHost << "\n";
            out << "SocketConnectPort=" << c.socketConnectPort << "\n";
        }

        out << "HeartBtInt=" << (c.heartBtIntMs / 1000ULL) << "\n";
        out << "ReconnectIntervalMs=" << c.reconnectIntervalMs << "\n";
        out << "LogoutTimeoutMs=" << c.logoutTimeoutMs << "\n";

        out << "DisableHeartbeat=" << yn(c.disableHeartbeat) << "\n";
        out << "IgnoreTestRequest=" << yn(c.ignoreTestRequest) << "\n";
        out << "EnableResendRequest=" << yn(c.enableResendRequest) << "\n";
        out << "SendRedundantResendRequests=" << yn(c.sendRedundantResendRequests) << "\n";

        out << "ResetOnLogon=" << yn(c.resetOnLogon) << "\n";
        out << "ResetOnLogout=" << yn(c.resetOnLogout) << "\n";
        out << "ResetOnDisconnect=" << yn(c.resetOnDisconnect) << "\n";

        out << "SocketUseSSL=" << yn(c.useSSL) << "\n";
        out << "SSLVerifyPeer=" << yn(c.sslVerifyPeer) << "\n";

        if (!c.sslCertFile.empty()) out << "SSLCertFile=" << c.sslCertFile << "\n";
        if (!c.sslKeyFile.empty()) out << "SSLKeyFile=" << c.sslKeyFile << "\n";
        if (!c.sslCaFile.empty()) out << "SSLCaFile=" << c.sslCaFile << "\n";

        out << "ValidateSendingTime=" << yn(c.validateSendingTime) << "\n";
        out << "MaxLatencyMs=" << c.maxLatencyMs << "\n";
        out << "MillisecondsInTimeStamp=" << yn(c.millisecondsInTimeStamp) << "\n";

        out << "StoreType=" << toIni(st.storeType) << "\n";
        out << "FileStorePath=" << st.path << "\n";
        out << "PersistMessages=" << yn(st.persistMessages) << "\n";

        out << "LogType=" << toIni(l.logType) << "\n";


        if (sch.enabled) {
            out << "StartTime=" << formatHms(sch.startSeconds) << "\n";
            out << "EndTime=" << formatHms(sch.endSeconds) << "\n";
            out << "ResetSeqNumsDaily=" << yn(sch.resetSeqNumsDaily) << "\n";
        }

        out << "\n[SESSION]\n";
        out << "BeginString=" << s.beginString() << "\n";
        out << "SenderCompID=" << s.senderCompID() << "\n";
        out << "TargetCompID=" << s.targetCompID() << "\n";

        return out.str();
    }


    
};

} // namespace falconfix::test
