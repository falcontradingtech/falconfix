// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>

#include <connection/i_connection.h>
#include <session/session_base.h>
#include <session/session_settings.h>
#include <socket/i_socket_context.h>
#include <socket/i_tcp_server.h>
#include <socket/i_tcp_socket.h>

namespace falconfix {

class TCPAcceptorConnection final : public IConnection {
    socket::ISocketContext &m_engine;
    SessionBase &m_session;
    SessionConfig::Connection m_config;

    std::shared_ptr<socket::ITCPServer> m_server;
    std::shared_ptr<socket::ITCPSocket> m_socket;

public:
    TCPAcceptorConnection(socket::ISocketContext &engine,
                          SessionBase &session,
                          const SessionConfig::Connection &config) noexcept;

    FFStatus start() noexcept override;
    FFStatus write(const char *data, std::size_t size) noexcept override;
    FFStatus writeAsync(const char *data, std::size_t size) noexcept override;
    void close() noexcept override;
    void detach() noexcept override;

private:
    static void onAccept(void *ctx, std::shared_ptr<socket::ITCPSocket> socket) noexcept;
    static void onConnected(void *ctx) noexcept;
    static void onRead(void *ctx, socket::SocketReadView view) noexcept;
    static void onClosed(void *ctx) noexcept;
    FFStatus onTimer() noexcept override {  return FF_OK(); }
};

} // namespace falconfix
