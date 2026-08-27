// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>
#include <string_view>

#include <socket/socket_types.h>
#include <socket/socket_options.h>

namespace falconfix::socket {

    class ITCPServer {
    public:
        virtual ~ITCPServer() = default;

        virtual bool listen(std::string_view host, uint16_t port, TCPMode mode = TCPMode::Plain) noexcept = 0;
        virtual void close() noexcept = 0;
        [[nodiscard]] virtual bool isListening() const noexcept = 0;

        virtual void setCallbacks(TCPAcceptCallbacks callbacks) noexcept = 0;
        virtual void setOptions(const SocketOptions &options) noexcept = 0;
    };

} // namespace falconfix::socket
