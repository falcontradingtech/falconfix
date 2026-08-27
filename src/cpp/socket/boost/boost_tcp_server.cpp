// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <algorithm>

#include <socket/boost/boost_tcp_server.h>
#include <socket/boost/boost_tcp_socket.h>

#include <utils/error_codes.h>

namespace falconfix::socket {

    BoostTCPServer::BoostTCPServer(std::shared_ptr<boost::asio::io_context> ctx) noexcept
        : m_ioContext(std::move(ctx)), m_acceptor(*m_ioContext) {}

    BoostTCPServer::~BoostTCPServer() {
        close();
    }

    bool BoostTCPServer::listen(std::string_view host, uint16_t port, TCPMode mode) noexcept {
        if (mode == TCPMode::SSL) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::system::NOT_IMPLEMENTED, "SSL not implemented");
            return false;
        }

        try {
            boost::asio::ip::tcp::endpoint endpoint = (host.empty() ?
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) :
                boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(host.data()), port));

            m_acceptor.open(endpoint.protocol());
            m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            m_acceptor.bind(endpoint);
            m_acceptor.listen();

            m_isListening.store(true, std::memory_order_release);

            applyOptions();
            startAccept();

            return true;
        }
        catch (const std::exception &e) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::socket::BIND_FAILED, e.what());
            return false;
        }
    }

    void BoostTCPServer::close() noexcept {
        const bool wasListening = m_isListening.exchange(false, std::memory_order_acq_rel);

        boost::system::error_code ec;
        m_acceptor.close(ec);

        for (const auto &client : m_clients) {
            if (client)
                client->close();
        }
        m_clients.clear();

        if (!wasListening)
            return;
    }

    bool BoostTCPServer::isListening() const noexcept {
        return m_isListening.load(std::memory_order_acquire);
    }

    void BoostTCPServer::setCallbacks(TCPAcceptCallbacks callbacks) noexcept {
        m_callbacks = callbacks;
    }

    void BoostTCPServer::setOptions(const SocketOptions &options) noexcept {
        m_options = options;
        applyOptions();
    }

    void BoostTCPServer::startAccept() noexcept {
        if (!m_isListening.load(std::memory_order_acquire))
            return;

        auto self = shared_from_this();
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(*m_ioContext);

        m_acceptor.async_accept(*socket, [self, socket](const boost::system::error_code &ec) {
            if (ec) {
                if (!self->m_isListening.load(std::memory_order_acquire) || ec == boost::asio::error::operation_aborted)
                    return;

                self->m_callbacks.onError(self->m_callbacks.m_ctx, falconfix::errors::code::socket::ACCEPT_FAILED, ec.message());
                return;
            }

            std::erase_if(self->m_clients, [](const std::shared_ptr<BoostTCPSocket> &client) {
                return !client || !client->isOpen();
            });

            auto client = std::make_shared<BoostTCPSocket>(self->m_ioContext);
            auto *raw = client.get();
            self->m_clients.push_back(client);
            self->m_callbacks.onAccept(self->m_callbacks.m_ctx, client);
            raw->setOptions(self->m_options);
            raw->attach(std::move(*socket));

            self->startAccept();
        });
    }

    void BoostTCPServer::applyOptions() noexcept {
        boost::system::error_code ec;

        if (m_options.m_recvBufferSize > 0)
            m_acceptor.set_option(boost::asio::socket_base::receive_buffer_size(m_options.m_recvBufferSize), ec);

        if (m_options.m_sendBufferSize > 0)
            m_acceptor.set_option(boost::asio::socket_base::send_buffer_size(m_options.m_sendBufferSize), ec);
    }

} // namespace falconfix::socket
