// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <array>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>

#include <socket/i_tcp_socket.h>

namespace falconfix::socket {

    class BoostTCPSocketSSL final
        : public ITCPSocket,
          public std::enable_shared_from_this<BoostTCPSocketSSL> {
        struct SyncWriteState {
            std::mutex mutex;
            std::condition_variable cv;
            bool done{false};
            bool success{false};
            std::size_t bytes{0};
        };

        struct PendingWrite {
            std::string data{};
            SyncWriteState *syncState{nullptr};
        };

        std::shared_ptr<boost::asio::io_context> m_ioContext;
        std::shared_ptr<boost::asio::ssl::context> m_sslContext;
        boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
        boost::asio::ip::tcp::resolver m_resolver;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_stream;
        std::string m_handshakeServerName{};

        SocketCallbacks m_callbacks{};
        SocketOptions m_options{};

        std::atomic<bool> m_isOpen{false};
        std::atomic<bool> m_isConnecting{false};
        std::atomic<bool> m_hasEverOpened{false};
        std::array<char, 64> m_peerIP{};
        std::size_t m_peerIPSize{0};
        uint16_t m_peerPort{0};

        std::array<char, 64 * 1024> m_readBuffer{};
        std::deque<PendingWrite> m_writeQueue;
        PendingWrite m_currentWrite{};
        std::size_t m_currentOffset{0};
        bool m_isWriting{false};
        std::atomic<std::size_t> m_pendingWriteCount{0};
        std::atomic<bool> m_isClosed{false};

        boost::asio::steady_timer m_connectTimer;
        boost::asio::ssl::stream_base::handshake_type m_handshakeType;

    public:
        explicit BoostTCPSocketSSL(
            std::shared_ptr<boost::asio::io_context> ctx,
            boost::asio::ssl::stream_base::handshake_type handshakeType = boost::asio::ssl::stream_base::client,
            std::shared_ptr<boost::asio::ssl::context> sslContext = nullptr) noexcept;
        ~BoostTCPSocketSSL() override;

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

        [[nodiscard]] std::string_view peerIP() const noexcept override {
            return {m_peerIP.data(), m_peerIPSize};
        }

        [[nodiscard]] uint16_t peerPort() const noexcept override {
            return m_peerPort;
        }

        [[nodiscard]] std::string_view tlsServerName() const noexcept {
            return m_handshakeServerName;
        }

        [[nodiscard]] std::size_t pendingWrites() const noexcept override {
            return m_pendingWriteCount.load(std::memory_order_acquire);
        }

    private:
        bool configureSSLContext() noexcept;
        bool prepareClientHandshake(std::string_view host) noexcept;
        void applyOptions() noexcept;
        void closeInternal() noexcept;
        void enqueueWrite(PendingWrite &&data) noexcept;
        static void finishSyncWrite(SyncWriteState *state, bool success, std::size_t bytes) noexcept;
        void startRead() noexcept;
        void startAsyncWrite() noexcept;
        void continueAsyncWrite() noexcept;
        void startConnectTimeout() noexcept;
        void startHandshake() noexcept;
        void handleConnect(const boost::system::error_code &ec) noexcept;
        void handleHandshake(const boost::system::error_code &ec) noexcept;
        void reportError(falconfix::errors::ErrorCode code, std::string_view message) noexcept;
    };

} // namespace falconfix::socket
