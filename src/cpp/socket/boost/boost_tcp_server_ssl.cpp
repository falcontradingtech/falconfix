// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <socket/boost/boost_tcp_server_ssl.h>

#include <utils/error_codes.h>

namespace falconfix::socket {

    BoostTCPServerSSL::BoostTCPServerSSL(std::shared_ptr<boost::asio::io_context> ctx) noexcept
        : m_ioContext(std::move(ctx)),
          m_sslContext(std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tls_server)),
          m_acceptor(*m_ioContext) {}

    BoostTCPServerSSL::~BoostTCPServerSSL() {
        close();
    }

    bool BoostTCPServerSSL::configureSSLContext() noexcept {
        if (m_options.m_tlsCertFile.empty() || m_options.m_tlsKeyFile.empty()) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::missing_server_credentials");
            return false;
        }

        boost::system::error_code ec;
        (void)m_sslContext->set_options(
            boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::single_dh_use,
            ec);
        if (ec) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::tls::FAILURE, "tls::context_options_failed");
            return false;
        }

        (void)m_sslContext->use_certificate_chain_file(m_options.m_tlsCertFile, ec);
        if (ec) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::certificate_load_failed");
            return false;
        }

        (void)m_sslContext->use_private_key_file(
            m_options.m_tlsKeyFile,
            boost::asio::ssl::context::pem,
            ec);
        if (ec) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::private_key_load_failed");
            return false;
        }

        return true;
    }

    bool BoostTCPServerSSL::listen(std::string_view host, uint16_t port, TCPMode mode) noexcept {
        if (mode != TCPMode::SSL) {
            m_callbacks.onError(m_callbacks.m_ctx, falconfix::errors::code::tls::OPERATION_NOT_SUPPORTED, "tls::server_requires_ssl_mode");
            return false;
        }

        if (!configureSSLContext())
            return false;

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

    void BoostTCPServerSSL::close() noexcept {
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

    bool BoostTCPServerSSL::isListening() const noexcept {
        return m_isListening.load(std::memory_order_acquire);
    }

    void BoostTCPServerSSL::setCallbacks(TCPAcceptCallbacks callbacks) noexcept {
        m_callbacks = callbacks;
    }

    void BoostTCPServerSSL::setOptions(const SocketOptions &options) noexcept {
        m_options = options;
        applyOptions();
    }

    void BoostTCPServerSSL::startAccept() noexcept {
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

            std::erase_if(self->m_clients, [](const std::shared_ptr<BoostTCPSocketSSL> &client) {
                return !client || !client->isOpen();
            });

            auto client = std::make_shared<BoostTCPSocketSSL>(
                self->m_ioContext,
                boost::asio::ssl::stream_base::server,
                self->m_sslContext);
            client->setOptions(self->m_options);

            auto *raw = client.get();
            self->m_clients.push_back(client);
            self->m_callbacks.onAccept(self->m_callbacks.m_ctx, client);
            raw->attach(std::move(*socket));
            self->startAccept();
        });
    }

    void BoostTCPServerSSL::applyOptions() noexcept {
        boost::system::error_code ec;

        if (m_options.m_recvBufferSize > 0)
            m_acceptor.set_option(boost::asio::socket_base::receive_buffer_size(m_options.m_recvBufferSize), ec);

        if (m_options.m_sendBufferSize > 0)
            m_acceptor.set_option(boost::asio::socket_base::send_buffer_size(m_options.m_sendBufferSize), ec);
    }

} // namespace falconfix::socket
