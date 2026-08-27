// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <connection/tcp_acceptor_connection.h>

namespace falconfix {

namespace {

socket::SocketOptions buildSocketOptions(const SessionConfig::Connection &config) noexcept {
    socket::SocketOptions options{};
    options.m_noDelay = config.tcpNoDelay;
    options.m_sendBufferSize = config.socketSendBufferSize;
    options.m_recvBufferSize = config.socketRecvBufferSize;
    options.m_connectTimeoutMs = config.socketConnectTimeoutMs;
    options.m_maxWriteChunkSize = config.socketMaxWriteChunkSize;
    options.m_tlsCertFile = config.sslCertFile;
    options.m_tlsKeyFile = config.sslKeyFile;
    options.m_tlsCaFile = config.sslCaFile;
    options.m_tlsServerName = config.sslServerName;
    options.m_tlsVerifyPeer = config.sslVerifyPeer;
    return options;
}

} // namespace

TCPAcceptorConnection::TCPAcceptorConnection(socket::ISocketContext &engine,
                                             SessionBase &session,
                                             const SessionConfig::Connection &config) noexcept
    : m_engine(engine),
      m_session(session),
      m_config(config) {
}

FFStatus TCPAcceptorConnection::start() noexcept {
    setTransportState(TransportState::Idle);

    const auto mode = (m_config.useSSL ? socket::TCPMode::SSL : socket::TCPMode::Plain);
    m_server = m_engine.createTCPServer(mode);
    if (!m_server) [[unlikely]] {
        return FF_SOCKET_ERR(NOT_OPEN);
    }

    socket::TCPAcceptCallbacks callbacks{};
    callbacks.m_ctx = this;
    callbacks.onAccept = &TCPAcceptorConnection::onAccept;

    m_server->setCallbacks(callbacks);
    m_server->setOptions(buildSocketOptions(m_config));

    const std::string host = m_config.socketAcceptHost.empty()
        ? "0.0.0.0"
        : m_config.socketAcceptHost;

    const bool ok = m_server->listen(
        host,
        static_cast<uint16_t>(m_config.socketAcceptPort),
        mode
    );

    return ok ? FF_OK() : FF_SOCKET_ERR(LISTEN_FAILED);
}


FFStatus TCPAcceptorConnection::write(const char *data, std::size_t size) noexcept {
    if (!m_socket) [[unlikely]] {
        return FF_SOCKET_ERR(NOT_CONNECTED);
    }

    const auto written = m_socket->write(data, size);
    return written == size ? FF_OK() : FF_SOCKET_ERR(WRITE_FAILED);
}

FFStatus TCPAcceptorConnection::writeAsync(const char *data, std::size_t size) noexcept {
    if (!m_socket) [[unlikely]] {
        return FF_SOCKET_ERR(NOT_CONNECTED);
    }

    const bool ok = m_socket->writeAsync(data, size);

    return ok ? FF_OK() : FF_SOCKET_ERR(WRITE_FAILED);
}

void TCPAcceptorConnection::close() noexcept {
    setTransportState(TransportState::Closing);

    if (m_socket) {
        m_socket->close();
    }

    if (m_server) {
        m_server->close();
        m_server = nullptr;
    }

    if (!m_socket) {
        setTransportState(TransportState::Idle);
    }
}

void TCPAcceptorConnection::detach() noexcept {
    if (m_socket) {
        socket::SocketCallbacks cb{};
        m_socket->setCallbacks(cb);
    }
    if (m_server) {
        socket::TCPAcceptCallbacks cb{};
        m_server->setCallbacks(cb);
    }
}

void TCPAcceptorConnection::onAccept(void *ctx, std::shared_ptr<socket::ITCPSocket> socket) noexcept {
    auto *self = static_cast<TCPAcceptorConnection *>(ctx);
    if (!self || !socket) {
        return;
    }

    if (self->m_socket) {
        socket::SocketCallbacks oldCallbacks{};
        self->m_socket->setCallbacks(oldCallbacks);
        self->m_socket.reset();
    }

    self->m_socket = socket;
    self->setTransportState(TransportState::Connecting);

    socket::SocketCallbacks callbacks{};
    callbacks.m_ctx = self;
    callbacks.onConnected = &TCPAcceptorConnection::onConnected;
    callbacks.onRead = &TCPAcceptorConnection::onRead;
    callbacks.onClosed = &TCPAcceptorConnection::onClosed;

    self->m_socket->setCallbacks(callbacks);
    self->m_socket->setOptions(buildSocketOptions(self->m_config));
}

void TCPAcceptorConnection::onConnected(void *ctx) noexcept {
    auto *self = static_cast<TCPAcceptorConnection *>(ctx);
    if (self && self->transitionTransportState(TransportState::Connecting, TransportState::Connected)) {
        (void)self->m_session.onConnected();
    }
}

void TCPAcceptorConnection::onRead(void *ctx, socket::SocketReadView view) noexcept {
    auto *self = static_cast<TCPAcceptorConnection *>(ctx);
    if (self) {
        if (self->transportState() != TransportState::Connected) {
            return;
        }

        (void)self->m_session.onReceive(view.m_data, view.m_size);
    }
}

void TCPAcceptorConnection::onClosed(void *ctx) noexcept {
    auto *self = static_cast<TCPAcceptorConnection *>(ctx);
    if (self) {
        self->setTransportState(TransportState::Idle);
        self->m_socket = nullptr;
        (void)self->m_session.onDisconnected();
    }
}

} // namespace falconfix
