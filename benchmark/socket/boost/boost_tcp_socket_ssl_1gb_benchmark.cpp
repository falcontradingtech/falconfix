// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <benchmark/benchmark.h>

#include "socket_bench_harness.h"

namespace {

static void BoostTCPSocketSSLSend1GbWithAck(benchmark::State &state) {
    falconfix::bench::BoostSocketBenchHarness harness;
    harness.runSyncSslAckTransfer(state, static_cast<std::size_t>(state.range(0)));
}

BENCHMARK(BoostTCPSocketSSLSend1GbWithAck)
    ->Arg(64 * 1024)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond)
    ->UseManualTime();

static void BoostTCPSocketSSLSendAsync1GbWithAck(benchmark::State &state) {
    falconfix::bench::BoostSocketBenchHarness harness;
    harness.runAsyncSslAckTransfer(state, static_cast<std::size_t>(state.range(0)));
}

BENCHMARK(BoostTCPSocketSSLSendAsync1GbWithAck)
    ->Arg(64 * 1024)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond)
    ->UseManualTime();

} // namespace
