// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <atomic>
#include <memory>
#include <vector>


#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <socket/boost/boost_tcp_socket_ssl.h>
#include <socket/i_tcp_server.h>

namespace falconfix::socket {

    class BoostTCPServerSSL final
        : public ITCPServer,
          public std::enable_shared_from_this<BoostTCPServerSSL> {
        std::shared_ptr<boost::asio::io_context> m_ioContext;
        std::shared_ptr<boost::asio::ssl::context> m_sslContext;
        boost::asio::ip::tcp::acceptor m_acceptor;
        std::vector<std::shared_ptr<BoostTCPSocketSSL>> m_clients;

        TCPAcceptCallbacks m_callbacks{};
        SocketOptions m_options{};

        std::atomic<bool> m_isListening{false};

    public:
        explicit BoostTCPServerSSL(std::shared_ptr<boost::asio::io_context> ctx) noexcept;
        ~BoostTCPServerSSL() override;

        bool listen(std::string_view host, uint16_t port, TCPMode mode = TCPMode::SSL) noexcept override;
        void close() noexcept override;
        [[nodiscard]] bool isListening() const noexcept override;

        void setCallbacks(TCPAcceptCallbacks callbacks) noexcept override;
        void setOptions(const SocketOptions &options) noexcept override;

    private:
        bool configureSSLContext() noexcept;
        void startAccept() noexcept;
        void applyOptions() noexcept;
    };

} // namespace falconfix::socket
