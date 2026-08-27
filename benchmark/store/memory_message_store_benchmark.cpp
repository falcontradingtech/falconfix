// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <benchmark/benchmark.h>

#include "store_bench_support.h"

#include <string>

namespace {

void BM_MemoryMessageStore_StoreOutgoing(benchmark::State &state) {
    const std::size_t payloadSize = static_cast<std::size_t>(state.range(0));
    const std::string payload(payloadSize, 'X');
    falconfix::MemoryMessageStore store;
    int64_t seq = 1;

    for (auto _ : state) {
        benchmark::DoNotOptimize(store.storeOutgoing(seq++, "TEST", true, payload).ok());
        if (seq > 4096) {
            store.reset();
            seq = 1;
        }
    }

    state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(payloadSize));
}

void BM_MemoryMessageStore_GetOutgoingWindow(benchmark::State &state) {
    const int64_t window = state.range(0);
    const int64_t total = 32768;
    const std::string payload(128, 'Y');
    falconfix::MemoryMessageStore store;

    for (int64_t i = 1; i <= total; ++i) {
        store.storeOutgoing(i, "TEST", true, payload);
    }

    auto out = falconfix::bench::makeOutgoingWindowBuffer(static_cast<std::size_t>(window));
    int64_t begin = 1;

    for (auto _ : state) {
        const int64_t end = begin + window - 1;
        benchmark::DoNotOptimize(store.getOutgoing(begin, end, out).ok());
        benchmark::DoNotOptimize(out.size());
        begin += window;
        if (begin + window >= total) {
            begin = 1;
        }
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * window);
}

void BM_NullMessageStore_StoreOutgoing(benchmark::State &state) {
    const std::size_t payloadSize = static_cast<std::size_t>(state.range(0));
    const std::string payload(payloadSize, 'N');
    falconfix::NullMessageStore store;
    int64_t seq = 1;

    for (auto _ : state) {
        benchmark::DoNotOptimize(store.storeOutgoing(seq++, "TEST", true, payload).ok());
    }

    state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(payloadSize));
}

void BM_FileMessageStore_StoreOutgoing(benchmark::State &state) {
    const std::size_t payloadSize = static_cast<std::size_t>(state.range(0));
    const std::string payload(payloadSize, 'F');
    falconfix::bench::FileStoreFixture fixture("store_out", "BENCHFS", "BENCHFC");
    auto &store = fixture.store();
    int64_t seq = 1;

    for (auto _ : state) {
        benchmark::DoNotOptimize(store.storeOutgoing(seq++, "TEST", true, payload).ok());
        if (seq > 8192) {
            store.reset();
            seq = 1;
        }
    }

    state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(payloadSize));
}

void BM_FileMessageStore_GetOutgoingWindow(benchmark::State &state) {
    const int64_t window = state.range(0);
    const int64_t total = 8192;
    const std::string payload(128, 'G');
    falconfix::bench::FileStoreFixture fixture("get_window", "BENCHGS", "BENCHGC");
    auto &store = fixture.store();

    for (int64_t i = 1; i <= total; ++i) {
        store.storeOutgoing(i, "TEST", true, payload);
    }

    auto out = falconfix::bench::makeOutgoingWindowBuffer(static_cast<std::size_t>(window));
    int64_t begin = 1;

    for (auto _ : state) {
        const int64_t end = begin + window - 1;
        benchmark::DoNotOptimize(store.getOutgoing(begin, end, out).ok());
        benchmark::DoNotOptimize(out.size());
        begin += window;
        if (begin + window >= total) {
            begin = 1;
        }
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * window);
}

} // namespace

BENCHMARK(BM_MemoryMessageStore_StoreOutgoing)->Arg(64)->Arg(256)->Arg(1024)->Arg(4096);
BENCHMARK(BM_MemoryMessageStore_GetOutgoingWindow)->Arg(16)->Arg(128)->Arg(512)->Arg(2048);
BENCHMARK(BM_NullMessageStore_StoreOutgoing)->Arg(64)->Arg(256)->Arg(1024)->Arg(4096);
BENCHMARK(BM_FileMessageStore_StoreOutgoing)->Arg(64)->Arg(256)->Arg(1024);
BENCHMARK(BM_FileMessageStore_GetOutgoingWindow)->Arg(16)->Arg(128)->Arg(512);
