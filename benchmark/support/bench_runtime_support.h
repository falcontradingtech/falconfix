// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <future>
#include <string>
#include <string_view>

namespace falconfix::bench {

inline std::uint16_t nextBenchPort() noexcept {
    static std::atomic<std::uint16_t> nextPort{39090};
    return nextPort.fetch_add(1, std::memory_order_acq_rel);
}

inline bool waitReady(std::future<void> &future, std::chrono::steady_clock::duration timeout) {
    return future.wait_for(timeout) == std::future_status::ready;
}

class ScopedTempDir {
    std::filesystem::path m_path;

public:
    explicit ScopedTempDir(std::string_view prefix) {
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        m_path = std::filesystem::temp_directory_path() /
            (std::string(prefix) + "_" + std::to_string(stamp));
        std::filesystem::create_directories(m_path);
    }

    ~ScopedTempDir() {
        if (!m_path.empty()) {
            std::error_code ec;
            std::filesystem::remove_all(m_path, ec);
        }
    }

    const std::filesystem::path &path() const noexcept {
        return m_path;
    }
};

} // namespace falconfix::bench
