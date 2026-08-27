// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <atomic>

#include <utils/error_codes.h>

#include <app/fix_application.h>

#include <socket/i_socket_context.h>
#include <connection/i_connection.h>
#include <connection/connection_factory.h>

#include <session/session_id.h>
#include <session/session_settings.h>
#include <session/session_registry.h>

#include <runtime/protocol_factory.h>
#include <logger/log_factory.h>
#include <store/message_store_factory.h>



namespace falconfix {

class FIXRuntime {
    FIXApplication &m_app;
    SessionSettings m_settings;
    std::unique_ptr<EngineLog> m_log;
    falconfix::socket::ISocketContext &m_socketEngine;

    SessionRegistry m_registry;
    std::vector<std::unique_ptr<SessionBase>> m_sessions;
    std::vector<std::unique_ptr<IConnection>> m_connections;

    std::atomic<bool> m_running{false};
    std::thread m_timerThread;

public:
    FIXRuntime(FIXApplication &app, SessionSettings settings, socket::ISocketContext &socketEngine)
        : m_app(app),
          m_settings(std::move(settings)),
          m_log(LogFactory::createEngineLog(m_settings)),
          m_socketEngine(socketEngine) {
    }

    ~FIXRuntime() {
        stop();
    }

    FFStatus start() noexcept {
        bool expected = false;
        if (!m_running.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
            return FF_APP_ERR(ENGINE_ALREADY_RUNNING, 0);
        }
        m_log->info("engine starting");

        for (const auto &cfg : m_settings.sessions()) {
            auto sessionLog = LogFactory::createSessionLog(cfg);
            auto store = MessageStoreFactory::create(cfg);

            auto session = ProtocolFactory::createSession(
                m_app,
                cfg,
                std::move(sessionLog),
                std::move(store)
            );
            if (!session) [[unlikely]] {
                const auto rc = FF_FIX_SESSION_ERR(SESSION_CREATE_FAILED, 0);
                m_log->error(falconfix::errors::format_error(rc));
                return rc;
            }
            m_app.onCreate(session->sessionID());

            auto connection = ConnectionFactory::create(
                m_socketEngine,
                *session,
                cfg
            );
            if (!connection) [[unlikely]] {
                const auto rc = FF_SOCKET_ERR(NOT_OPEN);
                m_log->error(falconfix::errors::format_error(rc));
                return rc;
            }

            session->bindConnection(*connection);
            m_registry.add(session.get());

            m_connections.emplace_back(std::move(connection));
            m_sessions.emplace_back(std::move(session));

            FFStatus rc = m_connections.back()->start();
            if (!rc.ok()) [[unlikely]] {
                m_log->error(falconfix::errors::format_error(rc));
                return rc;
            }
        }

        m_timerThread = std::thread([this]() {
            while (m_running.load(std::memory_order_acquire)) {
                for (auto &session : m_sessions) {
                    (void)session->onTimer();
                }
                for (auto &connection : m_connections) {
                    (void)connection->onTimer();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });

        m_log->info("engine started");
        return FF_OK();
    }

    void stop() noexcept {
        if (!m_running.exchange(false, std::memory_order_acq_rel)) {
            return;
        }
        m_log->info("engine stopping");

        if (m_timerThread.joinable()) {
            m_timerThread.join();
        }

        for (auto &session : m_sessions) {
            (void)session->doLogout("engine shutdown");
        }
        for (auto &connection : m_connections) {
            connection->detach();
        }
        for (auto &connection : m_connections) {
            if (connection) {
                connection->close();
            }
        }
        m_connections.clear();
        m_sessions.clear();
        m_log->info("engine stopped");
    }

    SessionBase *findSession(const SessionID &sid) noexcept {
        return m_registry.find(sid);
    }

    template<typename Msg>
    FFStatus sendToTarget(Msg &msg, const SessionID &sid) noexcept {
        SessionBase *base = findSession(sid);
        if (!base) [[unlikely]] {
            return FF_FIX_SESSION_ERR(SESSION_REJECT, 0);
        }

        using Protocol = typename Msg::Protocol;
        using SessionT = typename Protocol::SessionType;

        if (base->protocolVersion() != Protocol::VERSION) [[unlikely]] {
            return FF_FIX_PARSE_ERR(INVALID_MESSAGE_TYPE, 0);
        }

        return static_cast<SessionT *>(base)->send(msg);
    }

    FFStatus sendLogout(const SessionID &sid, std::string_view reason = {}) noexcept {
        SessionBase *session = m_registry.find(sid);
        if (!session) [[unlikely]] {
            return FF_FIX_SESSION_ERR(SESSION_NOT_FOUND, 0);
        }

        return session->doLogout(reason);
    }

#ifdef FALCONFIX_ENABLE_TEST_API
    void dumpSessionsForTest() const {
        for (const auto &session : m_sessions) {
            std::cout << "ENGINE SID: "
                    << session->sessionID().toString()
                    << " hash=" << session->sessionID().hash64()
                    << "\n";
        }
    }

    FFStatus disconnectForTest(const SessionID &sid) noexcept {
        for (std::size_t i = 0; i < m_sessions.size() && i < m_connections.size(); ++i) {
            if (m_sessions[i] && m_sessions[i]->sessionID() == sid) {
                if (!m_connections[i]) {
                    return FF_SOCKET_ERR(NOT_OPEN);
                }

                m_connections[i]->close();
                return FF_OK();
            }
        }

        return FF_FIX_SESSION_ERR(SESSION_NOT_FOUND, 0);
    }
#endif    
};

} // namespace falconfix
