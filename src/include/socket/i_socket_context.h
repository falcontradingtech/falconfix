// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>

#include <socket/i_tcp_socket.h>
#include <socket/i_tcp_server.h>
#include <socket/i_udp_socket.h>

namespace falconfix::socket {

    class ISocketContext {
    public:
        virtual ~ISocketContext() = default;

        virtual bool start(std::size_t threadCount) noexcept = 0;
        virtual void stop() noexcept = 0;

        [[nodiscard]] virtual std::shared_ptr<ITCPSocket> createTCPSocket(TCPMode mode = TCPMode::Plain) noexcept = 0;
        [[nodiscard]] virtual std::shared_ptr<ITCPServer> createTCPServer(TCPMode mode = TCPMode::Plain) noexcept = 0;

        [[nodiscard]] virtual std::shared_ptr<IUDPSocket> createUDPSocket() noexcept = 0;
    };

} // namespace falconfix::socket
