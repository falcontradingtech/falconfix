// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <connection/tcp_initiator_connection.h>

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

TCPInitiatorConnection::TCPInitiatorConnection(
    socket::ISocketContext &engine,
    SessionBase &session,
    const SessionConfig::Connection &config) noexcept
    : m_engine(engine),
      m_session(session),
      m_config(config) {
}

SessionConfig::Connection::Endpoint
TCPInitiatorConnection::currentEndpoint() const noexcept {
    if (!m_config.socketConnectEndpoints.empty()) {
        return m_config.socketConnectEndpoints[m_nextConnectIndex % m_config.socketConnectEndpoints.size()];
    }

    return {m_config.socketConnectHost, m_config.socketConnectPort};
}

void TCPInitiatorConnection::advanceToNextEndpoint() noexcept {
    if (m_config.socketConnectEndpoints.size() > 1) {
        m_nextConnectIndex = (m_nextConnectIndex + 1) % m_config.socketConnectEndpoints.size();
    }
}

void TCPInitiatorConnection::resetToPrimaryEndpoint() noexcept {
    m_nextConnectIndex = 0;
}

FFStatus TCPInitiatorConnection::start() noexcept {
    if (transportState() == TransportState::Connecting) {
        return FF_OK();
    }

    if (m_socket) {
        socket::SocketCallbacks callbacks{};
        m_socket->setCallbacks(callbacks);
        m_socket.reset();
    }

    setTransportState(TransportState::Connecting);

    const auto mode = (m_config.useSSL ? socket::TCPMode::SSL : socket::TCPMode::Plain);
    m_socket = m_engine.createTCPSocket(mode);
    if (!m_socket) [[unlikely]] {
        setTransportState(TransportState::Idle);
        return FF_SOCKET_ERR(NOT_OPEN);
    }

    socket::SocketCallbacks callbacks{};
    callbacks.m_ctx = this;
    callbacks.onConnected = &TCPInitiatorConnection::onConnected;
    callbacks.onRead = &TCPInitiatorConnection::onRead;
    callbacks.onClosed = &TCPInitiatorConnection::onClosed;
    callbacks.onError = &TCPInitiatorConnection::onError;

    m_socket->setCallbacks(callbacks);
    m_socket->setOptions(buildSocketOptions(m_config));

    const auto endpoint = currentEndpoint();
    const bool ok = m_socket->connect(
        endpoint.host,
        static_cast<uint16_t>(endpoint.port)
    );

    if (!ok) {
        advanceToNextEndpoint();
    }

    return ok
        ? FF_OK()
        : (setTransportState(TransportState::Idle), FF_SOCKET_ERR(CONNECT_FAILED));
}

FFStatus TCPInitiatorConnection::write(const char *data, std::size_t size) noexcept {
    if (!m_socket) [[unlikely]] {
        return FF_SOCKET_ERR(NOT_OPEN);
    }

    const auto written = m_socket->write(data, size);

    return written == size
        ? FF_OK()
        : FF_SOCKET_ERR(WRITE_FAILED);
}

FFStatus TCPInitiatorConnection::writeAsync(const char *data, std::size_t size) noexcept {
    if (!m_socket) [[unlikely]] {
        return FF_SOCKET_ERR(NOT_OPEN);
    }

    const bool ok = m_socket->writeAsync(data, size);

    return ok ? FF_OK() : FF_SOCKET_ERR(WRITE_FAILED);
}

void TCPInitiatorConnection::close() noexcept {
    setTransportState(TransportState::Closing);
    if (m_socket) {
        m_socket->close();
        return;
    }

    setTransportState(TransportState::Idle);
}

void TCPInitiatorConnection::detach() noexcept {
    if (m_socket) {
        socket::SocketCallbacks cb{};
        m_socket->setCallbacks(cb);
    }
}

void TCPInitiatorConnection::onConnected(void *ctx) noexcept {
    auto *self = static_cast<TCPInitiatorConnection *>(ctx);
    if (self && self->transitionTransportState(TransportState::Connecting, TransportState::Connected)) {
        self->resetToPrimaryEndpoint();
        (void)self->m_session.onConnected();
    }
}

void TCPInitiatorConnection::onRead(void *ctx, socket::SocketReadView view) noexcept {
    auto *self = static_cast<TCPInitiatorConnection *>(ctx);
    if (self) {
        if (self->transitionTransportState(TransportState::Connecting, TransportState::Connected)) {
            self->resetToPrimaryEndpoint();
            (void)self->m_session.onConnected();
        }

        if (self->transportState() != TransportState::Connected) {
            return;
        }

        (void)self->m_session.onReceive(view.m_data, view.m_size);
    }
}

void TCPInitiatorConnection::onClosed(void *ctx) noexcept {
    auto *self = static_cast<TCPInitiatorConnection *>(ctx);
    if (self) {
        const auto state = self->transportState();
        self->setTransportState(TransportState::Idle);
        if (state == TransportState::Connecting) {
            self->advanceToNextEndpoint();
        }
        else {
            self->resetToPrimaryEndpoint();
        }
        (void)self->m_session.onDisconnected();
        if (self->m_socket) {
            socket::SocketCallbacks cb{};
            self->m_socket->setCallbacks(cb);
            self->m_socket.reset();
        }
    }
}

void TCPInitiatorConnection::onError(void *ctx,
                                     falconfix::errors::ErrorCode,
                                     std::string_view) noexcept {
    auto *self = static_cast<TCPInitiatorConnection *>(ctx);
    if (!self) {
        return;
    }

    if (self->transportState() == TransportState::Connecting) {
        self->setTransportState(TransportState::Idle);
        self->advanceToNextEndpoint();
        if (self->m_socket) {
            socket::SocketCallbacks cb{};
            self->m_socket->setCallbacks(cb);
            self->m_socket.reset();
        }
    }
}

FFStatus TCPInitiatorConnection::onTimer() noexcept {
    if (!m_session.isDisconnected()) {
        return FF_OK();
    }

    if (!m_session.isWithinSchedule()) {
        return FF_OK();
    }

    return start();
}

} // namespace falconfix
