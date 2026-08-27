// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <chrono>
#include <cstring>
#include <future>
#include <thread>

#include <utility>

#include <socket/boost/boost_tcp_socket.h>

#include <utils/error_codes.h>


namespace falconfix::socket {

    namespace {

        inline bool isRetryableWriteError(const boost::system::error_code &ec) noexcept {
            return ec == boost::asio::error::would_block ||
                   ec == boost::asio::error::try_again ||
                   ec == boost::asio::error::interrupted;
        }

    } // namespace

    BoostTCPSocket::BoostTCPSocket(std::shared_ptr<boost::asio::io_context> ctx) noexcept
        : m_ioContext(std::move(ctx)),
          m_strand(m_ioContext->get_executor()),
          m_resolver(*m_ioContext),
          m_socket(*m_ioContext),
          m_connectTimer(*m_ioContext) {}

    BoostTCPSocket::~BoostTCPSocket() {
        closeInternal();
    }

    bool BoostTCPSocket::connect(std::string_view host, uint16_t port, std::string_view iface) noexcept {
        if (m_isOpen.load(std::memory_order_acquire) || m_isConnecting.exchange(true, std::memory_order_acq_rel))
            return false;

        m_isClosed.store(false, std::memory_order_release);
        m_hasEverOpened.store(false, std::memory_order_release);
        m_pendingWriteCount.store(0, std::memory_order_release);
        m_isWriting = false;
        m_writeQueue.clear();
        m_currentWrite.clear();
        m_currentOffset = 0;

        boost::system::error_code ec;
        auto address = boost::asio::ip::make_address(host, ec);
        if (!ec) {
            boost::asio::ip::tcp::endpoint endpoint(address, port);

            m_socket.open(endpoint.protocol(), ec);
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

                m_socket.bind(boost::asio::ip::tcp::endpoint(localAddress, 0), ec);
                if (ec) {
                    m_isConnecting.store(false, std::memory_order_release);
                    reportError(falconfix::errors::code::socket::INTERFACE_BIND, "socket::interface_bind");
                    return false;
                }
            }

            startConnectTimeout();

            auto self = shared_from_this();
            m_socket.async_connect(endpoint, boost::asio::bind_executor(m_strand, [self](const boost::system::error_code &connectEc) {
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

                self->m_socket.open(endpoint.endpoint().protocol(), ec);
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

                    self->m_socket.bind(boost::asio::ip::tcp::endpoint(localAddress, 0), ec);
                    if (ec) {
                        self->m_isConnecting.store(false, std::memory_order_release);
                        self->reportError(falconfix::errors::code::socket::INTERFACE_BIND, "socket::interface_bind");
                        self->closeInternal();
                        return;
                    }
                }

                boost::asio::async_connect(
                    self->m_socket,
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

    void BoostTCPSocket::startConnectTimeout() noexcept {
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

    void BoostTCPSocket::reportError(falconfix::errors::ErrorCode code, std::string_view message) noexcept {
        m_callbacks.onError(m_callbacks.m_ctx, code, message);
    }

    void BoostTCPSocket::handleConnect(const boost::system::error_code &ec) noexcept {
        if (m_isClosed.load(std::memory_order_acquire))
            return;

        m_isConnecting.store(false, std::memory_order_release);
        m_connectTimer.cancel();
        if (ec) {
            if (ec != boost::asio::error::operation_aborted) {
                reportError(falconfix::errors::code::socket::CONNECT_FAILED, "socket::connect_failed");
            }
            closeInternal();
            return;
        }

        m_isOpen.store(true, std::memory_order_release);
        m_hasEverOpened.store(true, std::memory_order_release);

        applyOptions();
        startRead();

        m_callbacks.onConnected(m_callbacks.m_ctx);
    }

    void BoostTCPSocket::attach(boost::asio::ip::tcp::socket &&socket) noexcept {
        m_isClosed.store(false, std::memory_order_release);
        m_isConnecting.store(false, std::memory_order_release);
        m_socket = std::move(socket);
        boost::system::error_code ec;
        auto ep = m_socket.remote_endpoint(ec);

        if (!ec) {
            auto ip = ep.address().to_string();

            m_peerIPSize = ip.size() < m_peerIP.size() ? ip.size() : m_peerIP.size() - 1;
            std::memcpy(m_peerIP.data(), ip.data(), m_peerIPSize);
            m_peerIP[m_peerIPSize] = '\0';
            m_peerPort = ep.port();
        }

        m_isOpen.store(true, std::memory_order_release);
        m_hasEverOpened.store(true, std::memory_order_release);

        applyOptions();
        startRead();

        m_callbacks.onConnected(m_callbacks.m_ctx);
    }

    void BoostTCPSocket::closeInternal() noexcept {
        if (m_isClosed.exchange(true, std::memory_order_acq_rel))
            return;

        boost::system::error_code ec;
        m_isConnecting.store(false, std::memory_order_release);
        m_isOpen.store(false, std::memory_order_release);
        m_isWriting = false;
        m_connectTimer.cancel();
        m_resolver.cancel();

        m_socket.cancel(ec);
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        m_socket.close(ec);

        const auto pendingWrites = m_writeQueue.size();
        m_writeQueue.clear();
        if (pendingWrites > 0) {
            m_pendingWriteCount.fetch_sub(pendingWrites, std::memory_order_acq_rel);
        }

        if (!m_currentWrite.empty()) {
            m_currentWrite.clear();
            m_currentOffset = 0;
        }

        if (m_hasEverOpened.exchange(false, std::memory_order_acq_rel)) {
            m_callbacks.onClosed(m_callbacks.m_ctx);
        }
    }

    void BoostTCPSocket::close() noexcept {
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

    std::size_t BoostTCPSocket::write(const char *data, std::size_t size) noexcept {
        if (!m_isOpen.load(std::memory_order_acquire) || !data || size == 0)
            return 0;

        std::size_t totalSent = 0;
        while (totalSent < size) {
            boost::system::error_code ec;
            const auto sent = m_socket.send(boost::asio::buffer(data + totalSent, size - totalSent), 0, ec);
            if (ec) {
                if (isRetryableWriteError(ec)) {
                    std::this_thread::sleep_for(std::chrono::microseconds(5));
                    continue;
                }

                reportError(falconfix::errors::code::socket::WRITE_FAILED, "socket::write_failed");
                return totalSent;
            }

            if (sent == 0)
                return totalSent;

            totalSent += sent;
        }

        return totalSent;
    }

    bool BoostTCPSocket::writeAsync(const char *data, std::size_t size) noexcept {
        if (!m_isOpen.load(std::memory_order_acquire))
            return false;

        if (!data || size == 0)
            return false;

        std::string pending(data, size);

        auto self = shared_from_this();
        boost::asio::post(m_strand, [self, pending = std::move(pending)]() mutable {
            self->enqueueWrite(std::move(pending));
        });
        return true;
    }

    void BoostTCPSocket::setCallbacks(SocketCallbacks callbacks) noexcept {
        m_callbacks = callbacks;
    }

    void BoostTCPSocket::setOptions(const SocketOptions &options) noexcept {
        m_options = options;
        applyOptions();
    }

    void BoostTCPSocket::applyOptions() noexcept {
        boost::system::error_code ec;

        if (m_options.m_noDelay) {
            m_socket.set_option(boost::asio::ip::tcp::no_delay(true), ec);
        }

        if (m_options.m_sendBufferSize > 0) {
            m_socket.set_option(
                boost::asio::socket_base::send_buffer_size(m_options.m_sendBufferSize),
                ec);
        }

        if (m_options.m_recvBufferSize > 0) {
            m_socket.set_option(
                boost::asio::socket_base::receive_buffer_size(m_options.m_recvBufferSize),
                ec);
        }
    }

    void BoostTCPSocket::startRead() noexcept {
        if (!m_isOpen.load(std::memory_order_acquire))
            return;

        m_socket.async_read_some(
            boost::asio::buffer(m_readBuffer.data(), m_readBuffer.size()),
            boost::asio::bind_executor(
                m_strand,
                [self = shared_from_this()](const boost::system::error_code &ec, std::size_t bytes) {
                    if (ec) {
                        if (ec == boost::asio::error::operation_aborted || self->m_isClosed.load(std::memory_order_acquire))
                            return;

                        if (ec != boost::asio::error::eof) {
                            self->reportError(falconfix::errors::code::socket::READ_FAILED, "socket::read_failed");
                        }
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

    void BoostTCPSocket::enqueueWrite(std::string &&data) noexcept {
        if (m_isClosed.load(std::memory_order_acquire) || !m_isOpen.load(std::memory_order_acquire)) {
            return;
        }

        m_writeQueue.emplace_back(std::move(data));
        m_pendingWriteCount.fetch_add(1, std::memory_order_acq_rel);

        if (!m_isWriting) {
            m_isWriting = true;
            startAsyncWrite();
        }
    }

    void BoostTCPSocket::startAsyncWrite() noexcept {
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

    void BoostTCPSocket::continueAsyncWrite() noexcept {
        const std::size_t remaining = m_currentWrite.size() - m_currentOffset;
        const std::size_t chunkSize = remaining < m_options.m_maxWriteChunkSize ? remaining : m_options.m_maxWriteChunkSize;

        m_socket.async_send(
            boost::asio::buffer(m_currentWrite.data() + m_currentOffset, chunkSize),
            boost::asio::bind_executor(
                m_strand,
                [self = shared_from_this()](const boost::system::error_code &ec, std::size_t bytes) {
                    if (ec) {
                        self->m_currentWrite.clear();
                        self->m_isWriting = false;
                        self->close();
                        return;
                    }

                    self->m_currentOffset += bytes;

                    if (self->m_currentOffset < self->m_currentWrite.size()) {
                        self->continueAsyncWrite();
                        return;
                    }

                    self->m_currentWrite.clear();
                    self->startAsyncWrite();
                }
            )
        );
    }

} // namespace falconfix::socket
