// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <benchmark/benchmark.h>

#include "socket_bench_harness.h"

namespace {

static void BoostTCPSocketSendIncrementalBurstWithAck(benchmark::State &state) {
    falconfix::bench::BoostSocketBenchHarness harness;
    harness.runSyncBurstTransfer(state, static_cast<std::uint64_t>(state.range(0)));
}

BENCHMARK(BoostTCPSocketSendIncrementalBurstWithAck)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond)
    ->UseManualTime();

static void BoostTCPSocketSendAsyncIncrementalBurstWithAck(benchmark::State &state) {
    falconfix::bench::BoostSocketBenchHarness harness;
    harness.runAsyncBurstTransfer(state, static_cast<std::uint64_t>(state.range(0)));
}

BENCHMARK(BoostTCPSocketSendAsyncIncrementalBurstWithAck)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond)
    ->UseManualTime();

} // namespace
