// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <memory>

#include <utils/error_codes.h>

namespace falconfix::socket {

    class ITCPSocket;

    struct SocketReadView {
        const char *m_data{nullptr};
        std::size_t m_size{0};
    };

    namespace detail {

        inline void noopConnected(void *) noexcept {}
        inline void noopClosed(void *) noexcept {}
        inline void noopRead(void *, SocketReadView) noexcept {}
        inline void noopError(void *, falconfix::errors::ErrorCode, std::string_view) noexcept {}
        inline void noopAccept(void *, std::shared_ptr<socket::ITCPSocket>) noexcept {}

    } // namespace detail

    enum class TCPMode : uint8_t {
        Plain,
        SSL
    };

    struct SocketWriteOp {
        const char *m_data{nullptr};
        std::size_t m_size{0};
    };

    struct SocketCallbacks {
        void *m_ctx{nullptr};

        void (*onConnected)(void *) noexcept{detail::noopConnected};
        void (*onClosed)(void *) noexcept{detail::noopClosed};
        void (*onRead)(void *, SocketReadView) noexcept{detail::noopRead};
        void (*onError)(void *, falconfix::errors::ErrorCode, std::string_view) noexcept{detail::noopError};
    };

    struct TCPAcceptCallbacks {
        void *m_ctx{nullptr};

        void (*onAccept)(void *, std::shared_ptr<ITCPSocket>) noexcept{detail::noopAccept};
        void (*onError)(void *, falconfix::errors::ErrorCode, std::string_view) noexcept{detail::noopError};
    };

    static_assert(std::is_trivially_copyable_v<SocketReadView>, "SocketReadView must be trivially copyable");

} // namespace falconfix::socket
