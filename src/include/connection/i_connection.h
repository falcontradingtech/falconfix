// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

#include <socket/socket_types.h>
#include <utils/error_codes.h>

namespace falconfix {

class IConnection {
public:
    enum class TransportState : uint8_t {
        Idle,
        Connecting,
        Connected,
        Closing
    };

    virtual ~IConnection() = default;

    virtual FFStatus start() noexcept = 0;
    virtual FFStatus write(const char *data, std::size_t size) noexcept = 0;
    virtual FFStatus writeAsync(const char *data, std::size_t size) noexcept = 0;
    virtual void close() noexcept = 0;
    virtual void detach() noexcept = 0;
    virtual FFStatus onTimer() noexcept = 0;

protected:
    TransportState transportState() const noexcept {
        return m_transportState.load(std::memory_order_acquire);
    }

    void setTransportState(TransportState state) noexcept {
        m_transportState.store(state, std::memory_order_release);
    }

    bool transitionTransportState(TransportState expected, TransportState desired) noexcept {
        return m_transportState.compare_exchange_strong(
            expected,
            desired,
            std::memory_order_acq_rel,
            std::memory_order_acquire
        );
    }

private:
    std::atomic<TransportState> m_transportState{TransportState::Idle};
};

} // namespace falconfix
