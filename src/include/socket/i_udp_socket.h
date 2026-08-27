// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

#include <socket/socket_types.h>

namespace falconfix::socket {

    struct UdpEndpoint {
        const char *ip{nullptr};
        uint16_t port{0};
    };

    struct UdpReadView {
        const char *data{nullptr};
        std::size_t size{0};
        UdpEndpoint from{};
    };

    struct UdpCallbacks {
        void *ctx{nullptr};

        void (*onRead)(void *ctx, UdpReadView view) noexcept{nullptr};
        void (*onError)(void *ctx, falconfix::errors::ErrorCode error, std::string_view msg) noexcept{nullptr};
    };

    class IUDPSocket {
    public:
        virtual ~IUDPSocket() = default;

        virtual bool bind(std::string_view ip, uint16_t port, std::string_view iface = {}) noexcept = 0;
        virtual void close() noexcept = 0;
        [[nodiscard]] virtual bool isOpen() const noexcept = 0;
        [[nodiscard]] virtual std::size_t sendTo(const char *data, std::size_t size, const UdpEndpoint &to) noexcept = 0;
        [[nodiscard]] virtual bool sendAsyncTo(const SocketWriteOp &op, const UdpEndpoint &to) noexcept = 0;

        virtual void setCallbacks(UdpCallbacks callbacks) noexcept = 0;
        virtual void setSendBufferSize(int size) noexcept = 0;
        virtual void setRecvBufferSize(int size) noexcept = 0;
    };

} // namespace falconfix::socket
