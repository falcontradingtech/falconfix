// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <socket/boost/boost_socket_context.h>

#include <socket/boost/boost_tcp_socket.h>
#include <socket/boost/boost_tcp_socket_ssl.h>
#include <socket/boost/boost_tcp_server.h>
#include <socket/boost/boost_tcp_server_ssl.h>

namespace falconfix::socket {

    BoostSocketContext::~BoostSocketContext() {
        stop();
    }

    bool BoostSocketContext::start(std::size_t threadCount) noexcept {
        if (m_running.exchange(true))
            return true;

        if (threadCount == 0)
            threadCount = 1;

        try {
            m_ioContext = std::make_shared<boost::asio::io_context>();
            m_workGuard = std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(m_ioContext->get_executor());
            m_threads.reserve(threadCount);

            for (std::size_t i = 0; i < threadCount; ++i) {
                m_threads.emplace_back([ctx = m_ioContext]() {
                    ctx->run();
                });
            }

            return true;
        }
        catch (...) {
            m_running.store(false);
            return false;
        }
    }

    void BoostSocketContext::stop() noexcept {
        if (!m_running.exchange(false))
            return;

        if (m_workGuard)
            m_workGuard.reset();
        if (m_ioContext)
            m_ioContext->stop();

        for (auto &thread : m_threads) {
            if (thread.joinable())
                thread.join();
        }

        m_threads.clear();
        m_ioContext.reset();
    }

    std::shared_ptr<ITCPSocket> BoostSocketContext::createTCPSocket(TCPMode mode) noexcept {
        if (!m_ioContext)
            return nullptr;

        if (mode == TCPMode::SSL)
            return std::make_shared<BoostTCPSocketSSL>(m_ioContext);

        return std::make_shared<BoostTCPSocket>(m_ioContext);
    }

    std::shared_ptr<ITCPServer> BoostSocketContext::createTCPServer(TCPMode mode) noexcept {
        if (!m_ioContext)
            return nullptr;

        if (mode == TCPMode::SSL)
            return std::make_shared<BoostTCPServerSSL>(m_ioContext);

        return std::make_shared<BoostTCPServer>(m_ioContext);
    }

    std::shared_ptr<IUDPSocket> BoostSocketContext::createUDPSocket() noexcept {
        return nullptr;
    }

} // namespace falconfix::socket
