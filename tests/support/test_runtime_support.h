// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <thread>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#elif defined(__linux__)

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#endif

namespace falconfix::test {

#ifdef _WIN32

namespace {
    struct WSAInitializerRAII {
        WSAInitializerRAII() noexcept {
            WSADATA wsa_data;
            WSAStartup(MAKEWORD(2, 2), &wsa_data);
        }

        ~WSAInitializerRAII() noexcept {
            WSACleanup();
        }
    };

    inline const WSAInitializerRAII g_wsa_initializer;
}

#endif

inline int32_t findFreeLoopbackPort() noexcept {
#ifdef _WIN32
    const SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return 0;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;

    if (::bind(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) != 0) {
        ::closesocket(sock);
        return 0;
    }

    int addr_len = sizeof(addr);
    if (::getsockname(sock, reinterpret_cast<sockaddr *>(&addr), &addr_len) != 0) {
        ::closesocket(sock);
        return 0;
    }

    const int32_t port = ntohs(addr.sin_port);
    ::closesocket(sock);
    return port;
#elif defined(__linux__)
    const int32_t fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        return 0;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;

    if (::bind(fd, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) != 0) {
        ::close(fd);
        return 0;
    }

    socklen_t len = sizeof(addr);
    if (::getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &len) != 0) {
        ::close(fd);
        return 0;
    }

    const int32_t port = ntohs(addr.sin_port);
    ::close(fd);
    return port;
#else
    return 0;
#endif
}

inline int32_t nextTestPort() noexcept {
    return findFreeLoopbackPort();
}


inline int64_t hms(int64_t hour, int64_t minute, int64_t second = 0) noexcept {
    return hour * 3600 + minute * 60 + second;
}

inline bool waitUntil(const std::function<bool()> &predicate,
                      std::chrono::milliseconds timeout,
                      std::chrono::milliseconds step = std::chrono::milliseconds(10)) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (predicate()) {
            return true;
        }
        std::this_thread::sleep_for(step);
    }
    return predicate();
}

inline bool waitUntilWithRetries(const std::function<bool()> &predicate,
                                 int32_t maxRetries = 10,
                                 std::chrono::milliseconds stepDelay = std::chrono::milliseconds(1000)) {
    for (int32_t attempt = 1; attempt <= maxRetries; ++attempt) {
        if (predicate()) {
            return true;
        }
        if (attempt < maxRetries) {
            std::this_thread::sleep_for(stepDelay);
        }
    }
    return false;
}

} // namespace falconfix::test
