// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <benchmark/benchmark.h>

#include <utils/tick_clock.h>

#include <chrono>
#include <ctime>
#include <cstdint>
#include <utility>

#if !defined(_WIN32)
#include <time.h>
#endif

using namespace falconfix;

static void BM_std_clock(benchmark::State &state) {
    for (auto _ : state) {
        auto now = std::clock();
        benchmark::DoNotOptimize(std::move(now));
    }
}

static void BM_chrono_system_clock_now(benchmark::State &state) {
    for (auto _ : state) {
        auto now = std::chrono::system_clock::now();
        benchmark::DoNotOptimize(std::move(now));
    }
}

static void BM_chrono_steady_clock_now(benchmark::State &state) {
    for (auto _ : state) {
        auto now = std::chrono::steady_clock::now();
        benchmark::DoNotOptimize(std::move(now));
    }
}

static void BM_chrono_elapsed_seconds_double(benchmark::State &state) {
    auto t0 = std::chrono::system_clock::now();

    for (auto _ : state) {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration<double>(now - t0).count();
        benchmark::DoNotOptimize(std::move(elapsed));
    }
}

static void BM_tickclock_ms_now(benchmark::State &state) {
    for (auto _ : state) {
        auto now = TickClock::getTickCount();
        benchmark::DoNotOptimize(std::move(now));
    }
}

static void BM_tickclock_ms_elapsed(benchmark::State &state) {
    uint64_t t0 = TickClock::getTickCount();

    for (auto _ : state) {
        uint64_t now = TickClock::getTickCount();
        uint64_t elapsed = now - t0;
        benchmark::DoNotOptimize(std::move(elapsed));
    }
}

static void BM_tickclock_ns_now(benchmark::State &state) {
    for (auto _ : state) {
        uint64_t now = TickClock::getTickCountNS();
        benchmark::DoNotOptimize(std::move(now));
    }
}

static void BM_tickclock_ns_elapsed(benchmark::State &state) {
    uint64_t t0 = TickClock::getTickCountNS();

    for (auto _ : state) {
        uint64_t now = TickClock::getTickCountNS();
        uint64_t elapsed = now - t0;
        benchmark::DoNotOptimize(std::move(elapsed));
    }
}

#if !defined(_WIN32)

static void BM_clock_gettime_monotonic(benchmark::State &state) {
    for (auto _ : state) {
        struct timespec ts{};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        benchmark::DoNotOptimize(std::move(ts));
    }
}

#if defined(CLOCK_MONOTONIC_COARSE)
static void BM_clock_gettime_monotonic_coarse(benchmark::State &state) {
    for (auto _ : state) {
        struct timespec ts{};
        clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
        benchmark::DoNotOptimize(std::move(ts));
    }
}
#endif

#endif

BENCHMARK(BM_std_clock);
BENCHMARK(BM_chrono_system_clock_now);
BENCHMARK(BM_chrono_steady_clock_now);
BENCHMARK(BM_chrono_elapsed_seconds_double);
BENCHMARK(BM_tickclock_ms_now);
BENCHMARK(BM_tickclock_ms_elapsed);
BENCHMARK(BM_tickclock_ns_now);
BENCHMARK(BM_tickclock_ns_elapsed);

#if !defined(_WIN32)
BENCHMARK(BM_clock_gettime_monotonic);

#if defined(CLOCK_MONOTONIC_COARSE)
BENCHMARK(BM_clock_gettime_monotonic_coarse);
#endif

#endif
