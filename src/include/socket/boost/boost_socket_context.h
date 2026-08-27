// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <atomic>

#include <boost/asio/io_context.hpp>
#include <boost/asio/executor_work_guard.hpp>

#include <socket/i_socket_context.h>

namespace falconfix::socket {

    class BoostSocketContext final : public ISocketContext {
        std::shared_ptr<boost::asio::io_context> m_ioContext;
        std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> m_workGuard;
        std::vector<std::thread> m_threads;
        std::atomic<bool> m_running{false};

    public:
        BoostSocketContext() = default;
        ~BoostSocketContext() override;

        bool start(std::size_t threadCount) noexcept override;
        void stop() noexcept override;

        [[nodiscard]] virtual std::shared_ptr<ITCPSocket> createTCPSocket(TCPMode mode = TCPMode::Plain) noexcept override;
        [[nodiscard]] virtual std::shared_ptr<ITCPServer> createTCPServer(TCPMode mode = TCPMode::Plain) noexcept override;
        [[nodiscard]] virtual std::shared_ptr<IUDPSocket> createUDPSocket() noexcept override; // Can return nullptr for now.
    };

} // namespace falconfix::socket
