// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <spdlog/spdlog.h>

#include <FIX4.4/messages/MarketDataIncrementalRefresh.h>
#include <FIX4.4/messages/MarketDataRequest.h>
#include <FIX4.4/messages/MarketDataSnapshotFullRefresh.h>
#include <FIX4.4/messages/SecurityList.h>
#include <FIX4.4/messages/SecurityListRequest.h>

#include <app/fix_application.h>
#include <config/fix_config.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

namespace {

struct Instrument {
    uint32_t securityId{0};
    std::string symbol;
    std::string exchange;
};

struct Snapshot {
    uint32_t securityId{0};
    double bidPrice{0.0};
    double askPrice{0.0};
    uint32_t bidSize{0};
    uint32_t askSize{0};
};

struct Incremental {
    uint32_t securityId{0};
    double price{0.0};
    uint32_t size{0};
};

constexpr const char *kMDReqID = "MARKETDATA_REQ";

std::vector<Instrument> makeInstruments(uint32_t count) {
    static constexpr const char *kExchanges[5] = {
        "NYSE", "NASDAQ", "ARCA", "BATS", "IEX"
    };

    std::vector<Instrument> out;
    out.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        char symbol[32]{};
        std::snprintf(symbol, sizeof(symbol), "SYM%06u", i + 1);

        out.push_back(Instrument{
            i + 1,
            symbol,
            kExchanges[i % 5]
        });
    }

    return out;
}

std::vector<Snapshot> makeSnapshots(const std::vector<Instrument> &instruments) {
    std::vector<Snapshot> out;
    out.reserve(instruments.size());

    for (const auto &instrument : instruments) {
        const double bidPrice = 100.0 + (instrument.securityId * 0.01);
        out.push_back(Snapshot{
            instrument.securityId,
            bidPrice,
            bidPrice + 0.01,
            100,
            100
        });
    }

    return out;
}

std::vector<Incremental> makeIncrementals(const std::vector<Instrument> &instruments, uint32_t count) {
    std::vector<Incremental> out;
    out.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        const auto &instrument = instruments[i % instruments.size()];
        out.push_back(Incremental{
            instrument.securityId,
            100.0 + (instrument.securityId * 0.01) + (i * 0.0001),
            100 + (i % 10)
        });
    }

    return out;
}

class BenchmarkLikeServerApp final : public falconfix::FIXApplication {
public:
    BenchmarkLikeServerApp(uint32_t securityCount, uint32_t incrementalCount)
        : m_instruments(makeInstruments(securityCount)),
          m_snapshots(makeSnapshots(m_instruments)),
          m_incrementals(makeIncrementals(m_instruments, incrementalCount)) {
        for (const auto &instrument : m_instruments) {
            m_instrumentMap.emplace(instrument.symbol, &instrument);
        }
    }

    ~BenchmarkLikeServerApp() override {
        shutdown();
    }

    void bindEngine(falconfix::FIXRuntime& engine) {
        m_engine = &engine;
    }

    void shutdown() {
        m_stop.store(true, std::memory_order_release);
        if (m_senderThread.joinable()) {
            m_senderThread.join();
        }
    }

    void onCreate(const falconfix::SessionID &sid) override {
        m_sessionID = sid;
    }

    void onLogon(const falconfix::SessionID &) override {
        m_loggedOn.store(true, std::memory_order_release);
    }

    void onLogout(const falconfix::SessionID &) override {
        m_loggedOn.store(false, std::memory_order_release);
    }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef message, const falconfix::SessionID &sid) noexcept override {
        if (message.is<fix44::messages::SecurityListRequest>()) {
            return onSecurityListRequest(message.as<fix44::messages::SecurityListRequest>(), sid);
        }

        if (message.is<fix44::messages::MarketDataRequest>()) {
            return onMarketDataRequest(message.as<fix44::messages::MarketDataRequest>(), sid);
        }

        return FF_OK();
    }

    uint64_t incrementalsSent() const noexcept {
        return m_incrementalsSent.load(std::memory_order_acquire);
    }

    uint64_t incrementalSendFailures() const noexcept {
        return m_incrementalSendFailures.load(std::memory_order_acquire);
    }

    int32_t firstSendError() const noexcept {
        return m_firstSendError.load(std::memory_order_acquire);
    }

private:
    bool sendLocked(const falconfix::SessionID &sid, auto &message) {
        if (!m_engine) {
            return false;
        }

        std::lock_guard<std::mutex> lock(m_sendMutex);
        FFStatus rc = m_engine->sendToTarget(message, sid);
        if (!rc.ok()) {
            int32_t expected = 0;
            (void)m_firstSendError.compare_exchange_strong(expected, rc.code, std::memory_order_acq_rel);
        }
        return rc.ok();
    }

    FFStatus onSecurityListRequest(fix44::messages::SecurityListRequest &request,
                                   const falconfix::SessionID &sid) noexcept {
        fix44::messages::SecurityList reply{};
        reply.setSecurityReqID(request.getSecurityReqID());
        reply.setSecurityResponseID("SECLIST_1");
        reply.setSecurityRequestResult(0);
        reply.setLastFragment(true);

        auto &symbolsGroup = reply.getRefSecListGrp().getRefNoRelatedSymGroup();
        symbolsGroup.reserve(m_instruments.size());

        for (const auto &instrument : m_instruments) {
            auto &group = symbolsGroup.emplace_back();
            group.getRefInstrument().setSymbol(instrument.symbol.c_str());
            group.getRefInstrument().setSecurityID(std::to_string(instrument.securityId).c_str());
            group.getRefInstrument().setSecurityExchange(instrument.exchange.c_str());
        }

        sendLocked(sid, reply);
        return FF_OK();
    }

    FFStatus onMarketDataRequest(fix44::messages::MarketDataRequest &request,
                                 const falconfix::SessionID &sid) noexcept {
        std::vector<const Instrument *> requestedInstruments;
        auto &relatedSymbols = request.getRefInstrmtMDReqGrp().getRefNoRelatedSymGroup();
        requestedInstruments.reserve(relatedSymbols.size());

        for (auto &group : relatedSymbols) {
            const auto &symbol = group.getRefInstrument().getSymbol();
            const auto it = m_instrumentMap.find(symbol.c_str());
            if (it != m_instrumentMap.end()) {
                requestedInstruments.push_back(it->second);
            }
        }

        for (const auto *instrument : requestedInstruments) {
            fix44::messages::MarketDataSnapshotFullRefresh snapshot{};
            snapshot.setMDReqID(fix44::types::STRING<32>{kMDReqID});
            snapshot.getRefInstrument().setSymbol(instrument->symbol.c_str());

            const auto &book = m_snapshots[instrument->securityId - 1];
            auto &entries = snapshot.getRefMDFullGrp().getRefNoMDEntriesGroup();

            auto &bid = entries.emplace_back();
            bid.setMDEntryType('0');
            bid.setMDEntryPx(book.bidPrice);
            bid.setMDEntrySize(book.bidSize);

            auto &ask = entries.emplace_back();
            ask.setMDEntryType('1');
            ask.setMDEntryPx(book.askPrice);
            ask.setMDEntrySize(book.askSize);

            sendLocked(sid, snapshot);
        }

        startIncrementalSender(sid);
        return FF_OK();
    }

    void startIncrementalSender(const falconfix::SessionID &sid) {
        if (m_senderThread.joinable()) {
            return;
        }

        m_senderThread = std::thread([this, sid]() {
            for (const auto &update : m_incrementals) {
                if (m_stop.load(std::memory_order_acquire) ||
                    !m_loggedOn.load(std::memory_order_acquire)) {
                    break;
                }

                const auto &instrument = m_instruments[update.securityId - 1];
                fix44::messages::MarketDataIncrementalRefresh inc{};
                inc.setMDReqID(fix44::types::STRING<32>{kMDReqID});

                auto &entries = inc.getRefMDIncGrp().getRefNoMDEntriesGroup();
                auto &group = entries.emplace_back();
                group.setMDUpdateAction('1');
                group.setMDEntryType('0');
                group.setMDEntryPx(update.price);
                group.setMDEntrySize(update.size);
                group.getRefInstrument().setSymbol(instrument.symbol.c_str());
                group.getRefInstrument().setSecurityID(std::to_string(update.securityId).c_str());
                group.getRefInstrument().setSecurityIDSource("8");

                const bool sent = sendLocked(sid, inc);
                if (sent) {
                    m_incrementalsSent.fetch_add(1, std::memory_order_acq_rel);
                } else {
                    m_incrementalSendFailures.fetch_add(1, std::memory_order_acq_rel);
                }
            }
        });
    }

    falconfix::FIXRuntime* m_engine{nullptr};
    falconfix::SessionID m_sessionID;
    std::vector<Instrument> m_instruments;
    std::vector<Snapshot> m_snapshots;
    std::vector<Incremental> m_incrementals;
    std::unordered_map<std::string, const Instrument *> m_instrumentMap;
    std::mutex m_sendMutex;
    std::atomic<bool> m_loggedOn{false};
    std::atomic<bool> m_stop{false};
    std::atomic<uint64_t> m_incrementalsSent{0};
    std::atomic<uint64_t> m_incrementalSendFailures{0};
    std::atomic<int32_t> m_firstSendError{0};
    std::thread m_senderThread;
};

class BenchmarkLikeClientApp final : public falconfix::FIXApplication {
public:
    BenchmarkLikeClientApp(uint32_t securityCount, uint32_t incrementalCount)
        : m_securityCount(securityCount),
          m_incrementalTarget(incrementalCount) {
        m_symbols.reserve(securityCount);
    }

    void bindEngine(falconfix::FIXRuntime& engine) {
        m_engine = &engine;
    }

    void onCreate(const falconfix::SessionID &sid) override {
        m_sessionID = sid;
    }

    void onLogon(const falconfix::SessionID &sid) override {
        m_sessionID = sid;
        sendSecurityListRequest(sid);
    }

    void onLogout(const falconfix::SessionID &) override {}

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override {
        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef message, const falconfix::SessionID &sid) noexcept override {
        if (message.is<fix44::messages::SecurityList>()) {
            return onSecurityList(message.as<fix44::messages::SecurityList>(), sid);
        }

        if (message.is<fix44::messages::MarketDataSnapshotFullRefresh>()) {
            m_snapshotsReceived.fetch_add(1, std::memory_order_acq_rel);
            return FF_OK();
        }

        if (message.is<fix44::messages::MarketDataIncrementalRefresh>()) {
            const auto count = m_incrementalsReceived.fetch_add(1, std::memory_order_acq_rel) + 1;
            if (count == m_incrementalTarget) {
                m_finished.store(true, std::memory_order_release);
            }
            return FF_OK();
        }

        return FF_OK();
    }

    bool finished() const noexcept {
        return m_finished.load(std::memory_order_acquire);
    }

    uint64_t snapshotsReceived() const noexcept {
        return m_snapshotsReceived.load(std::memory_order_acquire);
    }

    uint64_t incrementalsReceived() const noexcept {
        return m_incrementalsReceived.load(std::memory_order_acquire);
    }

    std::size_t symbolsReceived() const noexcept {
        return m_symbols.size();
    }

private:
    void sendSecurityListRequest(const falconfix::SessionID &sid) {
        if (!m_engine) {
            return;
        }

        fix44::messages::SecurityListRequest request;
        request.setSecurityReqID(fix44::types::STRING<32>{"SECLIST_REQ"});
        request.setSecurityListRequestType(4);
        (void)m_engine->sendToTarget(request, sid);
    }

    void sendMarketDataRequest(const falconfix::SessionID &sid) {
        if (!m_engine) {
            return;
        }

        fix44::messages::MarketDataRequest request;
        request.setMDReqID(fix44::types::STRING<32>{kMDReqID});
        request.setSubscriptionRequestType('1');
        request.setMarketDepth(1);
        request.setMDUpdateType(0);

        auto &entryTypes = request.getRefMDReqGrp().getRefNoMDEntryTypesGroup();
        entryTypes.resize(2);
        entryTypes[0].setMDEntryType('0');
        entryTypes[1].setMDEntryType('1');

        auto &relatedSymbols = request.getRefInstrmtMDReqGrp().getRefNoRelatedSymGroup();
        relatedSymbols.resize(m_symbols.size());

        for (std::size_t i = 0; i < m_symbols.size(); ++i) {
            relatedSymbols[i].getRefInstrument().setSymbol(m_symbols[i].c_str());
        }

        (void)m_engine->sendToTarget(request, sid);
    }

    FFStatus onSecurityList(fix44::messages::SecurityList &message,
                            const falconfix::SessionID &sid) noexcept {
        auto &symbols = message.getRefSecListGrp().getRefNoRelatedSymGroup();
        for (const auto &group : symbols) {
            m_symbols.emplace_back(group.getRefInstrument().getSymbol().c_str());
        }

        bool lastFragment = true;
        if (message.hasLastFragment()) {
            lastFragment = message.getLastFragment();
        }

        if (lastFragment) {
            sendMarketDataRequest(sid);
        }

        return FF_OK();
    }

    falconfix::FIXRuntime* m_engine{nullptr};
    falconfix::SessionID m_sessionID;
    uint32_t m_securityCount{0};
    uint32_t m_incrementalTarget{0};
    std::vector<std::string> m_symbols;
    std::atomic<bool> m_finished{false};
    std::atomic<uint64_t> m_snapshotsReceived{0};
    std::atomic<uint64_t> m_incrementalsReceived{0};
};

std::string makeServerCfg(uint16_t port) {
    std::ostringstream out;
    out
        << "[DEFAULT]\n"
        << "ConnectionType=acceptor\n"
        << "SocketAcceptHost=127.0.0.1\n"
        << "SocketAcceptPort=" << port << "\n"
        << "HeartBtInt=5\n"
        << "StoreType=memory\n"
        << "LogType=none\n\n"
        << "[SESSION]\n"
        << "BeginString=FIX4.4\n"
        << "SenderCompID=SERVER\n"
        << "TargetCompID=CLIENT\n";
    return out.str();
}

std::string makeClientCfg(uint16_t port) {
    std::ostringstream out;
    out
        << "[DEFAULT]\n"
        << "ConnectionType=initiator\n"
        << "SocketConnectHost=127.0.0.1\n"
        << "SocketConnectPort=" << port << "\n"
        << "HeartBtInt=5\n"
        << "StoreType=memory\n"
        << "LogType=none\n\n"
        << "[SESSION]\n"
        << "BeginString=FIX4.4\n"
        << "SenderCompID=CLIENT\n"
        << "TargetCompID=SERVER\n";
    return out.str();
}

} // namespace

struct BenchmarkLikeFlowResult {
    bool finished{false};
    std::size_t symbolsReceived{0};
    uint64_t snapshotsReceived{0};
    uint64_t incrementalsReceived{0};
    uint64_t incrementalsSent{0};
    uint64_t incrementalSendFailures{0};
    int32_t firstSendError{0};
};

BenchmarkLikeFlowResult runBenchmarkLikeFlow(uint32_t symbols, uint32_t incrementals) {
    spdlog::drop_all();

    static std::atomic<uint16_t> nextPort{32100};
    const uint16_t port = nextPort.fetch_add(1, std::memory_order_acq_rel);

    falconfix::socket::BoostSocketContext socketEngine;
    EXPECT_TRUE(socketEngine.start(2));

    BenchmarkLikeServerApp serverApp(symbols, incrementals);
    BenchmarkLikeClientApp clientApp(symbols, incrementals);

    auto serverSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(makeServerCfg(port))
    );
    auto clientSettings = falconfix::SessionSettings::fromConfig(
        falconfix::FIXConfig::parse(makeClientCfg(port))
    );

    falconfix::FIXRuntime serverEngine(serverApp, std::move(serverSettings), socketEngine);
    falconfix::FIXRuntime clientEngine(clientApp, std::move(clientSettings), socketEngine);

    serverApp.bindEngine(serverEngine);
    clientApp.bindEngine(clientEngine);

    FFStatus rc = serverEngine.start();
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    rc = clientEngine.start();
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    const auto timeoutSeconds = incrementals >= 100000 ? 30 : 5;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(timeoutSeconds);
    while (std::chrono::steady_clock::now() < deadline) {
        if (clientApp.finished()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    const bool finished = clientApp.finished();

    clientEngine.stop();
    serverApp.shutdown();
    serverEngine.stop();
    socketEngine.stop();
    spdlog::drop_all();

    return BenchmarkLikeFlowResult{
        finished,
        clientApp.symbolsReceived(),
        clientApp.snapshotsReceived(),
        clientApp.incrementalsReceived(),
        serverApp.incrementalsSent(),
        serverApp.incrementalSendFailures(),
        serverApp.firstSendError()
    };
}

TEST(FIXSessionTests, BenchmarkLikeFlowDeliversAllIncrementalsFromBurst) {
    constexpr uint32_t kSymbols = 2;
    constexpr uint32_t kIncrementals = 10;

    const auto result = runBenchmarkLikeFlow(kSymbols, kIncrementals);

    EXPECT_TRUE(result.finished)
        << "symbolsReceived=" << result.symbolsReceived
        << " snapshotsReceived=" << result.snapshotsReceived
        << " incrementalsReceived=" << result.incrementalsReceived
        << " incrementalsSent=" << result.incrementalsSent
        << " incrementalSendFailures=" << result.incrementalSendFailures
        << " firstSendError=" << result.firstSendError;
    EXPECT_EQ(result.symbolsReceived, kSymbols);
    EXPECT_EQ(result.snapshotsReceived, kSymbols);
    EXPECT_EQ(result.incrementalsSent, kIncrementals);
    EXPECT_EQ(result.incrementalsReceived, kIncrementals);
}

TEST(FIXSessionTests, BenchmarkLikeFlowDelivers100KIncrementals) {
    constexpr uint32_t kSymbols = 2;
    constexpr uint32_t kIncrementals = 100000;

    const auto result = runBenchmarkLikeFlow(kSymbols, kIncrementals);

    EXPECT_TRUE(result.finished)
        << "symbolsReceived=" << result.symbolsReceived
        << " snapshotsReceived=" << result.snapshotsReceived
        << " incrementalsReceived=" << result.incrementalsReceived
        << " incrementalsSent=" << result.incrementalsSent
        << " incrementalSendFailures=" << result.incrementalSendFailures
        << " firstSendError=" << result.firstSendError;
    EXPECT_EQ(result.symbolsReceived, kSymbols);
    EXPECT_EQ(result.snapshotsReceived, kSymbols);
    EXPECT_EQ(result.incrementalsSent, kIncrementals);
    EXPECT_EQ(result.incrementalsReceived, kIncrementals);
}

TEST(FIXSessionTests, DISABLED_BenchmarkLikeFlowHitsAsyncWriteBackpressureAt100K) {
    constexpr uint32_t kSymbols = 2;
    constexpr uint32_t kIncrementals = 100000;

    const auto result = runBenchmarkLikeFlow(kSymbols, kIncrementals);

    EXPECT_EQ(result.symbolsReceived, kSymbols);
    EXPECT_EQ(result.snapshotsReceived, kSymbols);
    EXPECT_GT(result.incrementalSendFailures, 0u)
        << "incrementalsSent=" << result.incrementalsSent
        << " incrementalsReceived=" << result.incrementalsReceived
        << " firstSendError=" << result.firstSendError;
    EXPECT_EQ(result.incrementalsSent + result.incrementalSendFailures, kIncrementals);
    EXPECT_EQ(result.incrementalsReceived, result.incrementalsSent);
    EXPECT_EQ(result.firstSendError, falconfix::errors::code::socket::WRITE_FAILED);
}
