// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#if defined(_WIN32)
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    define WIN32_LEAN_AND_MEAN
#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#    include <windows.h>
#endif

#include <cstdint>
#include <ctime>

namespace falconfix {

// Fast monotonic millisecond ticker.
//
// Uses CLOCK_MONOTONIC_COARSE on Linux â€” backed by the vDSO, so no syscall is
// issued on most kernels.  Resolution is one kernel tick (~4 ms at HZ=250),
// which is more than enough for heartbeat-interval checks (seconds range).
//
// Equivalent of Windows GetTickCount64().
struct TickClock {
    [[nodiscard]] static uint64_t getTickCount() noexcept {
#if defined(_WIN32)
        return static_cast<uint64_t>(::GetTickCount64());
#else
        struct timespec ts{};
    #if defined(CLOCK_MONOTONIC_COARSE)
        clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
    #else
        clock_gettime(CLOCK_MONOTONIC, &ts);
    #endif
        return static_cast<uint64_t>(ts.tv_sec) * 1000ULL
             + static_cast<uint64_t>(ts.tv_nsec) / 1'000'000ULL;
#endif
    }

    [[nodiscard]] static uint64_t getTickCountNS() noexcept {
#if defined(_WIN32)
        // On Windows, GetTickCount64 does not have nanosecond precision.
        // QueryPerformanceCounter is the standard API for high resolution.
        LARGE_INTEGER freq, count;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&count);
        // Calculation to avoid overflow before division:
        return (static_cast<uint64_t>(count.QuadPart) * 1'000'000'000ULL) / freq.QuadPart;
#else
        struct timespec ts{};
        // we use CLOCK_MONOTONIC for full precision (nanoseconds).
        // Unlike _COARSE, this may result in a real syscall,
        // although on modern Linux CLOCK_MONOTONIC also usually goes through the vDSO.
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<uint64_t>(ts.tv_sec) * 1'000'000'000ULL
             + static_cast<uint64_t>(ts.tv_nsec);
#endif
    }

};

} // namespace falconfix
