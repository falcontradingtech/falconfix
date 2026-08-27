// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <atomic>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <socket/i_tcp_server.h>
#include <socket/boost/boost_tcp_socket.h>

namespace falconfix::socket {

    class BoostTCPServer final
        : public ITCPServer,
          public std::enable_shared_from_this<BoostTCPServer> {
        std::shared_ptr<boost::asio::io_context> m_ioContext;
        boost::asio::ip::tcp::acceptor m_acceptor;
        std::vector<std::shared_ptr<BoostTCPSocket>> m_clients;

        TCPAcceptCallbacks m_callbacks{};
        SocketOptions m_options{};

        std::atomic<bool> m_isListening{false};

    public:
        explicit BoostTCPServer(std::shared_ptr<boost::asio::io_context> ctx) noexcept;
        ~BoostTCPServer() override;

        bool listen(std::string_view host, uint16_t port, TCPMode mode = TCPMode::Plain) noexcept override;
        void close() noexcept override;
        [[nodiscard]] bool isListening() const noexcept override;

        void setCallbacks(TCPAcceptCallbacks callbacks) noexcept override;
        void setOptions(const SocketOptions &options) noexcept override;

    private:
        void startAccept() noexcept;
        void applyOptions() noexcept;
    };

} // namespace falconfix::socket
