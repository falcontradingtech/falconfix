// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <chrono>
#include <cstring>
#include <future>
#include <thread>

#include <boost/asio/ssl/host_name_verification.hpp>

#include <socket/boost/boost_tcp_socket_ssl.h>

#include <utils/error_codes.h>

namespace falconfix::socket {

    namespace {

        inline bool isRetryableWriteError(const boost::system::error_code &ec) noexcept {
            return ec == boost::asio::error::would_block ||
                   ec == boost::asio::error::try_again ||
                   ec == boost::asio::error::interrupted;
        }

        std::shared_ptr<boost::asio::ssl::context> makeSSLContext(
            boost::asio::ssl::stream_base::handshake_type handshakeType) {
            if (handshakeType == boost::asio::ssl::stream_base::server)
                return std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tls_server);

            return std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tls_client);
        }

    } // namespace

    BoostTCPSocketSSL::BoostTCPSocketSSL(
        std::shared_ptr<boost::asio::io_context> ctx,
        boost::asio::ssl::stream_base::handshake_type handshakeType,
        std::shared_ptr<boost::asio::ssl::context> sslContext) noexcept
        : m_ioContext(std::move(ctx)),
          m_sslContext(sslContext ? std::move(sslContext) : makeSSLContext(handshakeType)),
          m_strand(m_ioContext->get_executor()),
          m_resolver(*m_ioContext),
          m_stream(*m_ioContext, *m_sslContext),
          m_connectTimer(*m_ioContext),
          m_handshakeType(handshakeType) {}

    BoostTCPSocketSSL::~BoostTCPSocketSSL() {
        closeInternal();
    }

    bool BoostTCPSocketSSL::configureSSLContext() noexcept {
        boost::system::error_code ec;

        if (m_handshakeType == boost::asio::ssl::stream_base::server) {
            if (m_options.m_tlsCertFile.empty() || m_options.m_tlsKeyFile.empty()) {
                reportError(falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::missing_server_credentials");
                return false;
            }

            (void)m_sslContext->set_options(
                boost::asio::ssl::context::default_workarounds |
                boost::asio::ssl::context::no_sslv2 |
                boost::asio::ssl::context::single_dh_use,
                ec);
            if (ec) {
                reportError(falconfix::errors::code::tls::FAILURE, "tls::context_options_failed");
                return false;
            }

            (void)m_sslContext->use_certificate_chain_file(m_options.m_tlsCertFile, ec);
            if (ec) {
                reportError(falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::certificate_load_failed");
                return false;
            }

            (void)m_sslContext->use_private_key_file(
                m_options.m_tlsKeyFile,
                boost::asio::ssl::context::pem,
                ec);
            if (ec) {
                reportError(falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::private_key_load_failed");
                return false;
            }
        }
        else {
            const auto verifyMode = m_options.m_tlsVerifyPeer
                ? boost::asio::ssl::verify_peer
                : boost::asio::ssl::verify_none;

            (void)m_sslContext->set_verify_mode(verifyMode, ec);
            if (ec) {
                reportError(falconfix::errors::code::tls::FAILURE, "tls::verify_mode_failed");
                return false;
            }

            m_stream.set_verify_mode(verifyMode, ec);
            if (ec) {
                reportError(falconfix::errors::code::tls::FAILURE, "tls::verify_mode_failed");
                return false;
            }

            if (m_options.m_tlsVerifyPeer && !m_options.m_tlsCaFile.empty()) {
                (void)m_sslContext->load_verify_file(m_options.m_tlsCaFile, ec);
                if (ec) {
                    reportError(falconfix::errors::code::tls::CERTIFICATE_ERROR, "tls::ca_load_failed");
                    return false;
                }
            }
        }

        return true;
    }

    bool BoostTCPSocketSSL::connect(std::string_view host, uint16_t port, std::string_view iface) noexcept {
        if (m_isOpen.load(std::memory_order_acquire) || m_isConnecting.exchange(true, std::memory_order_acq_rel))
            return false;

        m_isClosed.store(false, std::memory_order_release);
        m_hasEverOpened.store(false, std::memory_order_release);
        m_pendingWriteCount.store(0, std::memory_order_release);
        m_isWriting = false;
        m_writeQueue.clear();
        m_currentWrite = PendingWrite{};
        m_currentOffset = 0;
        m_handshakeServerName.clear();

        if (!configureSSLContext()) {
            m_isConnecting.store(false, std::memory_order_release);
            return false;
        }

        if (!prepareClientHandshake(host)) {
            m_isConnecting.store(false, std::memory_order_release);
            return false;
        }

        boost::system::error_code ec;
        auto address = boost::asio::ip::make_address(host, ec);
        if (!ec) {
            boost::asio::ip::tcp::endpoint endpoint(address, port);

            m_stream.next_layer().open(endpoint.protocol(), ec);
            if (ec) {
                m_isConnecting.store(false, std::memory_order_release);
                reportError(falconfix::errors::code::socket::CONNECT_FAILED, "socket::connect_failed");
                return false;
            }

            if (!iface.empty()) {
                auto localAddress = boost::asio::ip::make_address(iface, ec);
                if (ec) {
                    m_isConnecting.store(false, std::memory_order_release);
                    reportError(falconfix::errors::code::socket::INVALID_INTERFACE, "socket::invalid_interface");
                    return false;
                }

                m_stream.next_layer().bind(boost::asio::ip::tcp::endpoint(localAddress, 0), ec);
                if (ec) {
                    m_isConnecting.store(false, std::memory_order_release);
                    reportError(falconfix::errors::code::socket::INTERFACE_BIND, "socket::interface_bind");
                    return false;
                }
            }

            startConnectTimeout();

            auto self = shared_from_this();
            m_stream.next_layer().async_connect(endpoint, boost::asio::bind_executor(m_strand, [self](const boost::system::error_code &connectEc) {
                self->handleConnect(connectEc);
            }));

            return true;
        }

        startConnectTimeout();

        auto self = shared_from_this();
        m_resolver.async_resolve(host, std::to_string(port), boost::asio::bind_executor(
            m_strand,
            [self, iface = std::string(iface)](
                const boost::system::error_code &resolveEc,
                const boost::asio::ip::tcp::resolver::results_type &results
            ) {
                if (resolveEc) {
                    self->m_isConnecting.store(false, std::memory_order_release);
                    self->m_connectTimer.cancel();
                    self->reportError(falconfix::errors::code::socket::RESOLVE_FAILED, "socket::resolve_failed");
                    return;
                }

                boost::system::error_code ec;
                auto endpoint = *results.begin();

                self->m_stream.next_layer().open(endpoint.endpoint().protocol(), ec);
                if (ec) {
                    self->handleConnect(ec);
                    return;
                }

                if (!iface.empty()) {
                    auto localAddress = boost::asio::ip::make_address(iface, ec);
                    if (ec) {
                        self->m_isConnecting.store(false, std::memory_order_release);
                        self->reportError(falconfix::errors::code::socket::INVALID_INTERFACE, "socket::invalid_interface");
                        self->closeInternal();
                        return;
                    }

                    self->m_stream.next_layer().bind(boost::asio::ip::tcp::endpoint(localAddress, 0), ec);
                    if (ec) {
                        self->m_isConnecting.store(false, std::memory_order_release);
                        self->reportError(falconfix::errors::code::socket::INTERFACE_BIND, "socket::interface_bind");
                        self->closeInternal();
                        return;
                    }
                }

                boost::asio::async_connect(
                    self->m_stream.next_layer(),
                    results,
                    boost::asio::bind_executor(
                        self->m_strand,
                        [self](const boost::system::error_code &connectEc, const boost::asio::ip::tcp::endpoint &) {
                            self->handleConnect(connectEc);
                        }
                    )
                );
            }
        ));

        return true;
    }

    bool BoostTCPSocketSSL::prepareClientHandshake(std::string_view host) noexcept {
        if (m_handshakeType != boost::asio::ssl::stream_base::client)
            return true;

        m_handshakeServerName = m_options.m_tlsServerName;
        if (m_handshakeServerName.empty()) {
            boost::system::error_code ec;
            (void)boost::asio::ip::make_address(host, ec);
            if (ec) {
                m_handshakeServerName.assign(host.data(), host.size());
            }
        }

        if (m_handshakeServerName.empty())
            return true;

        if (!SSL_set_tlsext_host_name(m_stream.native_handle(), m_handshakeServerName.c_str())) {
            reportError(falconfix::errors::code::tls::FAILURE, "tls::sni_set_failed");
            return false;
        }

        if (m_options.m_tlsVerifyPeer) {
            m_stream.set_verify_callback(boost::asio::ssl::host_name_verification(m_handshakeServerName));
        }

        return true;
    }

    void BoostTCPSocketSSL::startConnectTimeout() noexcept {
        if (m_options.m_connectTimeoutMs <= 0)
            return;

        auto self = shared_from_this();
        m_connectTimer.expires_after(std::chrono::milliseconds(m_options.m_connectTimeoutMs));
        m_connectTimer.async_wait(boost::asio::bind_executor(m_strand, [self](const boost::system::error_code &timerEc) {
            if (timerEc || !self->m_isConnecting.load(std::memory_order_acquire))
                return;

            self->m_isConnecting.store(false, std::memory_order_release);
            self->reportError(falconfix::errors::code::socket::OPERATION_TIMEOUT, "socket::connect_timeout");
            self->closeInternal();
        }));
    }

    void BoostTCPSocketSSL::startHandshake() noexcept {
        auto self = shared_from_this();
        m_stream.async_handshake(m_handshakeType, boost::asio::bind_executor(m_strand, [self](const boost::system::error_code &handshakeEc) {
            self->handleHandshake(handshakeEc);
        }));
    }

    void BoostTCPSocketSSL::handleConnect(const boost::system::error_code &ec) noexcept {
        if (m_isClosed.load(std::memory_order_acquire))
            return;

        m_connectTimer.cancel();
        if (ec) {
            m_isConnecting.store(false, std::memory_order_release);
            if (ec != boost::asio::error::operation_aborted)
                reportError(falconfix::errors::code::socket::CONNECT_FAILED, "socket::connect_failed");
            closeInternal();
            return;
        }

        applyOptions();
        startHandshake();
    }

    void BoostTCPSocketSSL::handleHandshake(const boost::system::error_code &ec) noexcept {
        if (m_isClosed.load(std::memory_order_acquire))
            return;

        m_isConnecting.store(false, std::memory_order_release);
        m_connectTimer.cancel();

        if (ec) {
            if (ec != boost::asio::error::operation_aborted)
                reportError(falconfix::errors::code::tls::HANDSHAKE_FAILED, "tls::handshake_failed");
            closeInternal();
            return;
        }

        boost::system::error_code endpointEc;
        auto ep = m_stream.lowest_layer().remote_endpoint(endpointEc);
        if (!endpointEc) {
            const auto ip = ep.address().to_string();
            m_peerIPSize = ip.size() < m_peerIP.size() ? ip.size() : m_peerIP.size() - 1;
            std::memcpy(m_peerIP.data(), ip.data(), m_peerIPSize);
            m_peerIP[m_peerIPSize] = '\0';
            m_peerPort = ep.port();
        }

        if (m_handshakeType == boost::asio::ssl::stream_base::server) {
            if (const char *serverName = SSL_get_servername(m_stream.native_handle(), TLSEXT_NAMETYPE_host_name)) {
                m_handshakeServerName = serverName;
            }
        }

        m_isOpen.store(true, std::memory_order_release);
        m_hasEverOpened.store(true, std::memory_order_release);

        startRead();
        m_callbacks.onConnected(m_callbacks.m_ctx);
    }

    void BoostTCPSocketSSL::attach(boost::asio::ip::tcp::socket &&socket) noexcept {
        m_isClosed.store(false, std::memory_order_release);
        m_isConnecting.store(true, std::memory_order_release);
        m_hasEverOpened.store(false, std::memory_order_release);
        m_handshakeServerName.clear();

        if (!configureSSLContext()) {
            m_isConnecting.store(false, std::memory_order_release);
            return;
        }

        m_stream.next_layer() = std::move(socket);
        applyOptions();
        startHandshake();
    }

    void BoostTCPSocketSSL::closeInternal() noexcept {
        if (m_isClosed.exchange(true, std::memory_order_acq_rel))
            return;

        boost::system::error_code ec;
        m_isConnecting.store(false, std::memory_order_release);
        m_isOpen.store(false, std::memory_order_release);
        m_isWriting = false;
        m_connectTimer.cancel();
        m_resolver.cancel();

        // Avoid a blocking TLS shutdown during teardown. Tests and short-lived
        // benchmark flows can legitimately close while peer shutdown is still
        // in flight, so we only cancel and close the underlying transport here.
        m_stream.lowest_layer().cancel(ec);
        m_stream.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        m_stream.lowest_layer().close(ec);

        const auto pendingWrites = m_writeQueue.size();
        for (auto &pending : m_writeQueue) {
            finishSyncWrite(pending.syncState, false, 0);
        }
        m_writeQueue.clear();
        if (pendingWrites > 0) {
            m_pendingWriteCount.fetch_sub(pendingWrites, std::memory_order_acq_rel);
        }

        if (!m_currentWrite.data.empty()) {
            finishSyncWrite(m_currentWrite.syncState, false, 0);
            m_currentWrite = PendingWrite{};
            m_currentOffset = 0;
        }

        if (m_hasEverOpened.exchange(false, std::memory_order_acq_rel))
            m_callbacks.onClosed(m_callbacks.m_ctx);
    }

    void BoostTCPSocketSSL::close() noexcept {
        const bool wasOpen = m_isOpen.exchange(false, std::memory_order_acq_rel);
        const bool wasConnecting = m_isConnecting.exchange(false, std::memory_order_acq_rel);
        if (!wasOpen && !wasConnecting)
            return;

        auto self = shared_from_this();
        if (m_strand.running_in_this_thread()) {
            self->closeInternal();
            return;
        }

        std::promise<void> closed;
        auto done = closed.get_future();
        boost::asio::post(m_strand, [self, closed = std::move(closed)]() mutable {
            self->closeInternal();
            closed.set_value();
        });
        done.wait();
    }

    std::size_t BoostTCPSocketSSL::write(const char *data, std::size_t size) noexcept {
        if (!m_isOpen.load(std::memory_order_acquire) || !data || size == 0)
            return 0;

        if (m_strand.running_in_this_thread()) {
            std::size_t totalSent = 0;
            while (totalSent < size) {
                boost::system::error_code ec;
                const auto sent = m_stream.write_some(
                    boost::asio::buffer(data + totalSent, size - totalSent),
                    ec
                );
                if (ec) {
                    if (isRetryableWriteError(ec)) {
                        std::this_thread::sleep_for(std::chrono::microseconds(5));
                        continue;
                    }

                    reportError(falconfix::errors::code::socket::WRITE_FAILED, "socket::write_failed");
                    return totalSent;
                }

                if (sent == 0) {
                    return totalSent;
                }

                totalSent += sent;
            }

            return totalSent;
        }

        SyncWriteState state;
        PendingWrite pending{};
        pending.data.assign(data, size);
        pending.syncState = &state;

        auto self = shared_from_this();
        boost::asio::post(m_strand, [self, pending = std::move(pending)]() mutable {
            self->enqueueWrite(std::move(pending));
        });

        std::unique_lock<std::mutex> lock(state.mutex);
        state.cv.wait(lock, [&state]() { return state.done; });
        return state.success ? state.bytes : 0;
    }

    bool BoostTCPSocketSSL::writeAsync(const char *data, std::size_t size) noexcept {
        if (!m_isOpen.load(std::memory_order_acquire))
            return false;

        if (!data || size == 0)
            return false;

        PendingWrite pending{};
        pending.data.assign(data, size);
        auto self = shared_from_this();
        boost::asio::post(m_strand, [self, pending = std::move(pending)]() mutable {
            self->enqueueWrite(std::move(pending));
        });
        return true;
    }

    void BoostTCPSocketSSL::setCallbacks(SocketCallbacks callbacks) noexcept {
        m_callbacks = callbacks;
    }

    void BoostTCPSocketSSL::setOptions(const SocketOptions &options) noexcept {
        m_options = options;
        applyOptions();
    }

    void BoostTCPSocketSSL::applyOptions() noexcept {
        boost::system::error_code ec;

        if (m_options.m_noDelay)
            m_stream.next_layer().set_option(boost::asio::ip::tcp::no_delay(true), ec);

        if (m_options.m_sendBufferSize > 0)
            m_stream.next_layer().set_option(boost::asio::socket_base::send_buffer_size(m_options.m_sendBufferSize), ec);

        if (m_options.m_recvBufferSize > 0)
            m_stream.next_layer().set_option(boost::asio::socket_base::receive_buffer_size(m_options.m_recvBufferSize), ec);
    }

    void BoostTCPSocketSSL::startRead() noexcept {
        if (!m_isOpen.load(std::memory_order_acquire))
            return;

        m_stream.async_read_some(
            boost::asio::buffer(m_readBuffer.data(), m_readBuffer.size()),
            boost::asio::bind_executor(
                m_strand,
                [self = shared_from_this()](const boost::system::error_code &ec, std::size_t bytes) {
                    if (ec) {
                        if (ec == boost::asio::error::operation_aborted || self->m_isClosed.load(std::memory_order_acquire))
                            return;

                        if (ec != boost::asio::error::eof)
                            self->reportError(falconfix::errors::code::socket::READ_FAILED, "socket::read_failed");

                        self->close();
                        return;
                    }

                    SocketReadView view{self->m_readBuffer.data(), bytes};
                    self->m_callbacks.onRead(self->m_callbacks.m_ctx, view);
                    self->startRead();
                }
            )
        );
    }

    void BoostTCPSocketSSL::enqueueWrite(PendingWrite &&data) noexcept {
        if (m_isClosed.load(std::memory_order_acquire) || !m_isOpen.load(std::memory_order_acquire)) {
            finishSyncWrite(data.syncState, false, 0);
            return;
        }

        m_writeQueue.emplace_back(std::move(data));
        m_pendingWriteCount.fetch_add(1, std::memory_order_acq_rel);

        if (!m_isWriting) {
            m_isWriting = true;
            startAsyncWrite();
        }
    }

    void BoostTCPSocketSSL::startAsyncWrite() noexcept {
        if (!m_isOpen.load(std::memory_order_acquire)) {
            m_isWriting = false;
            return;
        }

        if (m_writeQueue.empty()) {
            m_isWriting = false;
            return;
        }

        m_currentWrite = std::move(m_writeQueue.front());
        m_writeQueue.pop_front();
        m_pendingWriteCount.fetch_sub(1, std::memory_order_acq_rel);

        m_currentOffset = 0;
        continueAsyncWrite();
    }

    void BoostTCPSocketSSL::continueAsyncWrite() noexcept {
        const std::size_t remaining = m_currentWrite.data.size() - m_currentOffset;
        const std::size_t chunkSize = remaining < m_options.m_maxWriteChunkSize ? remaining : m_options.m_maxWriteChunkSize;

        m_stream.async_write_some(
            boost::asio::buffer(m_currentWrite.data.data() + m_currentOffset, chunkSize),
            boost::asio::bind_executor(
                m_strand,
                [self = shared_from_this()](const boost::system::error_code &ec, std::size_t bytes) {
                    if (ec) {
                        self->finishSyncWrite(self->m_currentWrite.syncState, false, 0);
                        self->m_currentWrite = PendingWrite{};
                        self->m_isWriting = false;
                        self->close();
                        return;
                    }

                    self->m_currentOffset += bytes;
                    if (self->m_currentOffset < self->m_currentWrite.data.size()) {
                        self->continueAsyncWrite();
                        return;
                    }

                    self->finishSyncWrite(self->m_currentWrite.syncState, true, self->m_currentWrite.data.size());
                    self->m_currentWrite = PendingWrite{};
                    self->startAsyncWrite();
                }
            )
        );
    }

    void BoostTCPSocketSSL::finishSyncWrite(SyncWriteState *state, bool success, std::size_t bytes) noexcept {
        if (!state)
            return;

        {
            std::lock_guard<std::mutex> lock(state->mutex);
            state->done = true;
            state->success = success;
            state->bytes = bytes;
        }
        state->cv.notify_one();
    }

    void BoostTCPSocketSSL::reportError(falconfix::errors::ErrorCode code, std::string_view message) noexcept {
        m_callbacks.onError(m_callbacks.m_ctx, code, message);
    }

} // namespace falconfix::socket
