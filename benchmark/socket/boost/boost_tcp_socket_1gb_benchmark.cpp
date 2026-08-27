// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <benchmark/benchmark.h>

#include "socket_bench_harness.h"

namespace {

static void BoostTCPSocketSend1GbWithAck(benchmark::State &state) {
    falconfix::bench::BoostSocketBenchHarness harness;
    harness.runSyncAckTransfer(state, static_cast<std::size_t>(state.range(0)));
}

BENCHMARK(BoostTCPSocketSend1GbWithAck)
    ->Arg(64 * 1024)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond)
    ->UseManualTime();

static void BoostTCPSocketSendAsync1GbWithAck(benchmark::State &state) {
    falconfix::bench::BoostSocketBenchHarness harness;
    harness.runAsyncAckTransfer(state, static_cast<std::size_t>(state.range(0)));
}

BENCHMARK(BoostTCPSocketSendAsync1GbWithAck)
    ->Arg(64 * 1024)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond)
    ->UseManualTime();

} // namespace
