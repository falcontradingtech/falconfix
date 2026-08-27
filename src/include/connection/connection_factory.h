// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// connection/connection_factory.h
#pragma once

#include <memory>

#include <connection/i_connection.h>
#include <connection/tcp_acceptor_connection.h>
#include <connection/tcp_initiator_connection.h>
#include <session/session_base.h>
#include <session/session_settings.h>
#include <socket/i_socket_context.h>

namespace falconfix {

class ConnectionFactory {
public:
    static std::unique_ptr<IConnection>
    create(socket::ISocketContext &socketEngine,
           SessionBase &session,
           const SessionConfig &cfg) {
        switch (cfg.connection.connectionType) {
        case ConnectionType::Initiator:
            return std::make_unique<TCPInitiatorConnection>(
                socketEngine,
                session,
                cfg.connection
            );

        case ConnectionType::Acceptor:
            return std::make_unique<TCPAcceptorConnection>(
                socketEngine,
                session,
                cfg.connection
            );

        default:
            return nullptr;
        }
    }
};

} // namespace falconfix
