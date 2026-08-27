// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <socket/socket_types.h>
#include <socket/socket_options.h>

namespace falconfix::socket {

    class ITCPSocket {
    public:
        virtual ~ITCPSocket() = default;

        virtual bool connect(std::string_view host, uint16_t port, std::string_view iface = {}) noexcept = 0;
        virtual void close() noexcept = 0;
        [[nodiscard]] virtual bool isOpen() const noexcept = 0;
        [[nodiscard]] virtual std::size_t write(const char *data, std::size_t size) noexcept = 0;
        [[nodiscard]] virtual bool writeAsync(const char *data, std::size_t size) noexcept = 0;

        virtual void setCallbacks(SocketCallbacks callbacks) noexcept = 0;
        virtual void setOptions(const SocketOptions &options) noexcept = 0;

        [[nodiscard]] virtual std::string_view peerIP() const noexcept = 0;
        [[nodiscard]] virtual uint16_t peerPort() const noexcept = 0;
        [[nodiscard]] virtual std::size_t pendingWrites() const noexcept = 0;
    };

} // namespace falconfix::socket
