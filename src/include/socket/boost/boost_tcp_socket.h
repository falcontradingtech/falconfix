// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <array>
#include <atomic>
#include <deque>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/steady_timer.hpp>

#include <socket/i_tcp_socket.h>

namespace falconfix::socket {

    class BoostTCPSocket final
        : public ITCPSocket,
          public std::enable_shared_from_this<BoostTCPSocket> {
        std::shared_ptr<boost::asio::io_context> m_ioContext;
        boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
        boost::asio::ip::tcp::resolver m_resolver;
        boost::asio::ip::tcp::socket m_socket;

        SocketCallbacks m_callbacks{};
        SocketOptions m_options{};

        std::atomic<bool> m_isOpen{false};
        std::atomic<bool> m_isConnecting{false};
        std::atomic<bool> m_hasEverOpened{false};
        std::array<char, 64> m_peerIP{};
        std::size_t m_peerIPSize{0};
        uint16_t m_peerPort{0};

        std::array<char, 64 * 1024> m_readBuffer{};
        std::deque<std::string> m_writeQueue;
        std::string m_currentWrite{};
        std::size_t m_currentOffset{0};
        bool m_isWriting{false};
        std::atomic<std::size_t> m_pendingWriteCount{0};
        std::atomic<bool> m_isClosed{false};

        boost::asio::steady_timer m_connectTimer;

    public:
        explicit BoostTCPSocket(std::shared_ptr<boost::asio::io_context> ctx) noexcept;
        ~BoostTCPSocket() override;

        bool connect(std::string_view host, uint16_t port, std::string_view iface = {}) noexcept override;
        void attach(boost::asio::ip::tcp::socket &&socket) noexcept;
        void close() noexcept override;

        [[nodiscard]] bool isOpen() const noexcept override {
            return m_isOpen.load(std::memory_order_acquire);
        }

        [[nodiscard]] std::size_t write(const char *data, std::size_t size) noexcept override;
        [[nodiscard]] bool writeAsync(const char *data, std::size_t size) noexcept override;

        void setCallbacks(SocketCallbacks callbacks) noexcept override;
        void setOptions(const SocketOptions &options) noexcept override;

        [[nodiscard]] std::string_view peerIP() const noexcept override { return {m_peerIP.data(), m_peerIPSize}; }
        [[nodiscard]] uint16_t peerPort() const noexcept override { return m_peerPort; }
        [[nodiscard]] std::size_t pendingWrites() const noexcept override { return m_pendingWriteCount.load(std::memory_order_acquire);}

    private:
        void applyOptions() noexcept;
        void closeInternal() noexcept;
        void enqueueWrite(std::string &&data) noexcept;
        void startRead() noexcept;
        void startAsyncWrite() noexcept;
        void continueAsyncWrite() noexcept;
        void handleConnect(const boost::system::error_code &ec) noexcept;
        void startConnectTimeout() noexcept;
        void reportError(falconfix::errors::ErrorCode code, std::string_view message) noexcept;


    };

} // namespace falconfix::socket
