#include <benchmark/benchmark.h>

#include <string>
#include <vector>

#include <FIX4.4/messages/ExecutionReport.h>
#include <FIX4.4/messages/MarketDataIncrementalRefresh.h>
#include <FIX4.4/samples/fix_messages.h>
#include <FIX4.4/utils/datetime.h>

using namespace fix44;

struct OBEntry {
    double px;
    double size;
    char side;
    char updateAction;
    int32_t entryDate;
    int32_t positionNo;
    int32_t numberOfOrders;
    const char *orderId;
    const char *buyer;
    const char *seller;
    const char *symbol;
};

static const std::vector<OBEntry> INPUT_BOOK = {
    {101.25, 10.0, '0', '0', 20250101, 1, 10, "ORDER1001", "TRADER_A", "TRADER_X", "NVDA"},
    {101.30, 15.0, '1', '0', 20250101, 2, 12, "ORDER1002", "TRADER_B", "TRADER_Y", "NVDA"}
};

static void BM_FalconFix_Encode_MDIncremental(benchmark::State &state) {
    falconfix::FastBuffer buffer;
    for (auto _ : state) {
        fix44::messages::MarketDataIncrementalRefresh msg;
        auto &hdr = msg.getRefHeader();
        hdr.setBeginString("FIX.4.4");
        hdr.setSenderCompID("SENDER");
        hdr.setTargetCompID("TARGET");
        hdr.setMsgSeqNum(1337);
        hdr.setSendingTime(fix44::datetime::now_UTCTimestamp());

        auto &entries = msg.getRefMDIncGrp().getRefNoMDEntriesGroup();
        entries.resize(INPUT_BOOK.size());
        for (std::size_t i = 0; i < INPUT_BOOK.size(); ++i) {
            const auto &entry = INPUT_BOOK[i];
            auto &mdEntry = entries[i];
            auto &instrument = mdEntry.getRefInstrument();
            mdEntry.setMDEntryPx(entry.px);
            mdEntry.setMDEntrySize(entry.size);
            mdEntry.setMDEntryType(entry.side);
            mdEntry.setMDUpdateAction(entry.updateAction);
            mdEntry.setMDEntryDate(entry.entryDate);
            mdEntry.setOrderID(types::STRING<32>{entry.orderId});
            mdEntry.setMDEntryPositionNo(entry.positionNo);
            mdEntry.setNumberOfOrders(entry.numberOfOrders);
            mdEntry.setMDEntryBuyer(types::STRING<32>{entry.buyer});
            mdEntry.setMDEntrySeller(types::STRING<32>{entry.seller});
            instrument.setSymbol(types::STRING<32>{entry.symbol});
        }

        buffer.clear();
        if (!msg.toString(buffer).ok()) {
            state.SkipWithError("Encoding failed");
        }

        benchmark::DoNotOptimize(buffer);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_FalconFix_Encode_MDIncremental)->Unit(benchmark::kNanosecond);

static void BM_FalconFix_Encode_ExecutionReport(benchmark::State &state) {
    falconfix::FastBuffer buffer;
    for (auto _ : state) {
        fix44::messages::ExecutionReport msg;
        auto &hdr = msg.getRefHeader();
        auto &instrument = msg.getRefInstrument();
        hdr.setBeginString("FIX.4.4");
        hdr.setSenderCompID("SENDER");
        hdr.setTargetCompID("TARGET");
        hdr.setMsgSeqNum(1337);
        hdr.setSendingTime(fix44::datetime::now_UTCTimestamp());

        msg.setOrderID(types::STRING<32>{"ORDER1001"});
        msg.setExecID(types::STRING<32>{"EXEC1001"});
        msg.setExecType('0');
        msg.setOrdStatus('0');
        msg.setSide('1');
        msg.setLeavesQty(100.0);
        msg.setCumQty(0.0);
        msg.setAvgPx(0.0);
        instrument.setSymbol(types::STRING<32>{"NVDA"});

        buffer.clear();
        if (!msg.toString(buffer).ok()) {
            state.SkipWithError("Encoding failed");
        }

        benchmark::DoNotOptimize(buffer);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_FalconFix_Encode_ExecutionReport)->Unit(benchmark::kNanosecond);

extern "C" const char *falconfix_bench_mdinc_sample_cstr() {
    static std::string cached;

    if (cached.empty()) {
        const auto raw = samples::map_samples["MarketDataIncrementalRefresh"];
        cached.assign(raw.data(), raw.size());
    }

    return cached.c_str();
}

extern "C" const char *falconfix_bench_exec_report_sample_cstr() {
    static std::string cached;

    if (cached.empty()) {
        const auto raw = samples::map_samples["ExecutionReport"];
        cached.assign(raw.data(), raw.size());
    }

    return cached.c_str();
}

static void BM_FalconFix_Decode_MDIncremental(benchmark::State &state) {
    const auto raw = samples::map_samples["MarketDataIncrementalRefresh"];

    for (auto _ : state) {
        fix44::messages::MarketDataIncrementalRefresh msg;
        bool ok = msg.setString(raw).ok();
        if (!ok) {
            state.SkipWithError("Decoding failed");
            break;
        }

        benchmark::DoNotOptimize(ok);
        benchmark::DoNotOptimize(msg);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_FalconFix_Decode_MDIncremental)->Unit(benchmark::kNanosecond);

static void BM_FalconFix_Decode_ExecutionReport(benchmark::State &state) {
    const auto raw = samples::map_samples["ExecutionReport"];

    for (auto _ : state) {
        fix44::messages::ExecutionReport msg;
        bool ok = msg.setString(raw).ok();
        if (!ok) {
            state.SkipWithError("Decoding failed");
            break;
        }

        benchmark::DoNotOptimize(ok);
        benchmark::DoNotOptimize(msg);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_FalconFix_Decode_ExecutionReport)->Unit(benchmark::kNanosecond);
