// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <benchmark/benchmark.h>

#include <../support/bench_protocols.h>
#include <../support/bench_runtime_support.h>

#include <socket/boost/boost_socket_context.h>
#include <utils/fast_buffer.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <future>
#include <memory>
#include <mutex>
#include <filesystem>
#include <string>
#include <string_view>
#include <thread>

namespace falconfix::bench {

template<typename ProtocolTraits = DefaultProtocolTraits>
class BoostSocketBenchHarness {
    struct ServerState {
        std::atomic<std::uint64_t> bytesReceived{0};
        std::atomic<bool> ackSent{false};
        std::uint64_t expectedBytes{0};
        std::mutex acceptMutex;
        std::condition_variable acceptCv;
        bool accepted{false};
    };

    struct ClientState {
        std::promise<void> connectedPromise;
        std::promise<void> ackPromise;
        std::string recvBuf;
        bool ackDone{false};
    };

    struct ServerClientContext {
        ServerState *state{nullptr};
        socket::ITCPSocket *socket{nullptr};
    };

    struct BurstServerState {
        std::atomic<std::uint64_t> bytesReceived{0};
        std::atomic<bool> ackSent{false};
        std::uint64_t expectedBytes{0};
        std::mutex acceptMutex;
        std::condition_variable acceptCv;
        bool accepted{false};
    };

    struct BurstClientState {
        std::promise<void> connectedPromise;
        std::promise<void> ackPromise;
        std::promise<void> errorPromise;
        std::atomic<bool> finished{false};
        std::string recvBuf;
        bool ackDone{false};
    };

    struct BurstServerClientContext {
        BurstServerState *state{nullptr};
        socket::ITCPSocket *socket{nullptr};
    };

    static constexpr std::uint64_t kOneGb = 1024ULL * 1024ULL * 1024ULL;
    static constexpr auto kConnectTimeout = std::chrono::seconds(5);
    static constexpr auto kAckTimeout = std::chrono::seconds(30);

    socket::BoostSocketContext m_context;
    const std::uint16_t m_syncPort;
    const std::uint16_t m_asyncPort;
    const std::uint16_t m_burstPort;

public:
    BoostSocketBenchHarness()
        : m_syncPort(nextBenchPort()),
          m_asyncPort(nextBenchPort()),
          m_burstPort(nextBenchPort()) {
        (void)m_context.start(2);
    }

    ~BoostSocketBenchHarness() {
        m_context.stop();
    }

    void runSyncAckTransfer(benchmark::State &state, std::size_t chunkSize) {
        const std::string chunk(chunkSize, 'A');
        auto server = m_context.createTCPServer();
        if (!server) {
            state.SkipWithError("server creation failed");
            return;
        }

        ServerState serverState{};
        serverState.expectedBytes = kOneGb;
        installSyncAcceptCallbacks(*server, serverState);

        if (!server->listen("127.0.0.1", m_syncPort)) {
            state.SkipWithError("server listen failed");
            return;
        }

        for (auto _ : state) {
            (void)_;
            resetServerState(serverState);

            ClientState clientState{};
            auto client = m_context.createTCPSocket();
            if (!client) {
                state.SkipWithError("client creation failed");
                break;
            }

            installClientCallbacks(*client, clientState);
            if (!client->connect("127.0.0.1", m_syncPort)) {
                state.SkipWithError("client connect failed");
                break;
            }

            auto connectedFuture = clientState.connectedPromise.get_future();
            auto ackFuture = clientState.ackPromise.get_future();

            if (!waitReady(connectedFuture, kConnectTimeout)) {
                state.SkipWithError("timeout waiting client connect");
                break;
            }
            if (!waitForAccept(serverState, kConnectTimeout)) {
                state.SkipWithError("timeout waiting server accept");
                break;
            }

            const auto start = std::chrono::steady_clock::now();
            std::uint64_t sentTotal = 0;
            while (sentTotal < kOneGb) {
                auto toSend = chunkSize;
                const auto remaining = kOneGb - sentTotal;
                if (remaining < toSend) {
                    toSend = static_cast<std::size_t>(remaining);
                }

                const auto sent = client->write(chunk.data(), toSend);
                if (sent == 0) {
                    state.SkipWithError("client write failed");
                    break;
                }

                sentTotal += sent;
            }

            if (sentTotal != kOneGb) {
                client->close();
                break;
            }

            if (!waitReady(ackFuture, kAckTimeout)) {
                state.SkipWithError("timeout waiting ACK");
                client->close();
                break;
            }

            const auto end = std::chrono::steady_clock::now();
            state.SetIterationTime(std::chrono::duration<double>(end - start).count());
            client->close();
        }

        server->close();
        state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kOneGb));
    }

    void runAsyncAckTransfer(benchmark::State &state, std::size_t chunkSize) {
        const std::string chunk(chunkSize, 'A');
        auto server = m_context.createTCPServer();
        if (!server) {
            state.SkipWithError("server creation failed");
            return;
        }

        ServerState serverState{};
        serverState.expectedBytes = kOneGb;
        installSyncAcceptCallbacks(*server, serverState);

        if (!server->listen("127.0.0.1", m_asyncPort)) {
            state.SkipWithError("server listen failed");
            return;
        }

        for (auto _ : state) {
            (void)_;
            resetServerState(serverState);

            ClientState clientState{};
            auto client = m_context.createTCPSocket();
            if (!client) {
                state.SkipWithError("client creation failed");
                break;
            }

            installClientCallbacks(*client, clientState);
            if (!client->connect("127.0.0.1", m_asyncPort)) {
                state.SkipWithError("client connect failed");
                break;
            }

            auto connectedFuture = clientState.connectedPromise.get_future();
            auto ackFuture = clientState.ackPromise.get_future();

            if (!waitReady(connectedFuture, kConnectTimeout)) {
                state.SkipWithError("timeout waiting client connect");
                break;
            }
            if (!waitForAccept(serverState, kConnectTimeout)) {
                state.SkipWithError("timeout waiting server accept");
                break;
            }

            const auto start = std::chrono::steady_clock::now();
            if (!enqueueAllAsyncWrites(client.get(), chunk.data(), chunkSize, kOneGb, start + kAckTimeout)) {
                state.SkipWithError("timeout waiting async writes");
                client->close();
                break;
            }

            if (!waitReady(ackFuture, kAckTimeout)) {
                state.SkipWithError("timeout waiting ACK");
                client->close();
                break;
            }

            const auto end = std::chrono::steady_clock::now();
            state.SetIterationTime(std::chrono::duration<double>(end - start).count());
            client->close();
        }

        server->close();
        state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kOneGb));
    }

    void runAsyncBurstTransfer(benchmark::State &state, std::uint64_t messageCount) {
        const std::string payload = makeIncrementalPayload();
        if (payload.empty()) {
            state.SkipWithError("failed to build incremental payload");
            return;
        }

        auto server = m_context.createTCPServer();
        if (!server) {
            state.SkipWithError("server creation failed");
            return;
        }

        BurstServerState serverState{};
        serverState.expectedBytes = messageCount * payload.size();
        installBurstAcceptCallbacks(*server, serverState);

        if (!server->listen("127.0.0.1", m_burstPort)) {
            state.SkipWithError("server listen failed");
            return;
        }

        for (auto _ : state) {
            (void)_;
            resetBurstServerState(serverState, messageCount * payload.size());

            BurstClientState clientState{};
            auto client = m_context.createTCPSocket();
            if (!client) {
                state.SkipWithError("client creation failed");
                break;
            }

            installBurstClientCallbacks(*client, clientState);
            if (!client->connect("127.0.0.1", m_burstPort)) {
                state.SkipWithError("client connect failed");
                break;
            }

            auto connectedFuture = clientState.connectedPromise.get_future();
            auto ackFuture = clientState.ackPromise.get_future();
            auto errorFuture = clientState.errorPromise.get_future();

            if (!waitReady(connectedFuture, kConnectTimeout)) {
                state.SkipWithError("timeout waiting client connect");
                break;
            }
            if (!waitForAccept(serverState, kConnectTimeout)) {
                state.SkipWithError("timeout waiting server accept");
                break;
            }

            const auto start = std::chrono::steady_clock::now();
            for (std::uint64_t i = 0; i < messageCount; ++i) {
                if (!client->writeAsync(payload.data(), payload.size())) {
                    state.SkipWithError("client writeAsync enqueue failed");
                    break;
                }
            }

            if (state.skipped()) {
                client->close();
                break;
            }

            if (errorFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                state.SkipWithError("client async write failed");
                client->close();
                break;
            }

            if (!waitReady(ackFuture, kAckTimeout)) {
                state.SkipWithError("timeout waiting ACK");
                client->close();
                break;
            }

            const auto end = std::chrono::steady_clock::now();
            state.SetIterationTime(std::chrono::duration<double>(end - start).count());
            client->close();
        }

        server->close();
        state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(messageCount));
        state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(messageCount * payload.size()));
    }

    void runSyncBurstTransfer(benchmark::State &state, std::uint64_t messageCount) {
        const std::string payload = makeIncrementalPayload();
        if (payload.empty()) {
            state.SkipWithError("failed to build incremental payload");
            return;
        }

        auto server = m_context.createTCPServer();
        if (!server) {
            state.SkipWithError("server creation failed");
            return;
        }

        BurstServerState serverState{};
        serverState.expectedBytes = messageCount * payload.size();
        installBurstAcceptCallbacks(*server, serverState);

        if (!server->listen("127.0.0.1", m_burstPort)) {
            state.SkipWithError("server listen failed");
            return;
        }

        for (auto _ : state) {
            (void)_;
            resetBurstServerState(serverState, messageCount * payload.size());

            ClientState clientState{};
            auto client = m_context.createTCPSocket();
            if (!client) {
                state.SkipWithError("client creation failed");
                break;
            }

            installClientCallbacks(*client, clientState);
            if (!client->connect("127.0.0.1", m_burstPort)) {
                state.SkipWithError("client connect failed");
                break;
            }

            auto connectedFuture = clientState.connectedPromise.get_future();
            auto ackFuture = clientState.ackPromise.get_future();

            if (!waitReady(connectedFuture, kConnectTimeout)) {
                state.SkipWithError("timeout waiting client connect");
                break;
            }
            if (!waitForAccept(serverState, kConnectTimeout)) {
                state.SkipWithError("timeout waiting server accept");
                break;
            }

            const auto start = std::chrono::steady_clock::now();
            std::uint64_t sentMessages = 0;
            for (; sentMessages < messageCount; ++sentMessages) {
                const auto sent = client->write(payload.data(), payload.size());
                if (sent != payload.size()) {
                    state.SkipWithError("client burst write failed");
                    break;
                }
            }

            if (sentMessages != messageCount) {
                client->close();
                break;
            }

            if (!waitReady(ackFuture, kAckTimeout)) {
                state.SkipWithError("timeout waiting ACK");
                client->close();
                break;
            }

            const auto end = std::chrono::steady_clock::now();
            state.SetIterationTime(std::chrono::duration<double>(end - start).count());
            client->close();
        }

        server->close();
        state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(messageCount));
        state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(messageCount * payload.size()));
    }

    void runSyncSslAckTransfer(benchmark::State &state, std::size_t chunkSize) {
        const auto sslOptions = makeSslSocketOptions();
        if (sslOptions.m_tlsCertFile.empty() || sslOptions.m_tlsKeyFile.empty()) {
            state.SkipWithError("ssl certs not found");
            return;
        }

        const std::string chunk(chunkSize, 'A');
        auto server = m_context.createTCPServer(socket::TCPMode::SSL);
        if (!server) {
            state.SkipWithError("server creation failed");
            return;
        }
        server->setOptions(sslOptions);

        ServerState serverState{};
        serverState.expectedBytes = kOneGb;
        installSyncAcceptCallbacks(*server, serverState);

        if (!server->listen("127.0.0.1", m_syncPort, socket::TCPMode::SSL)) {
            state.SkipWithError("server listen failed");
            return;
        }

        for (auto _ : state) {
            (void)_;
            resetServerState(serverState);

            ClientState clientState{};
            auto client = m_context.createTCPSocket(socket::TCPMode::SSL);
            if (!client) {
                state.SkipWithError("client creation failed");
                break;
            }
            client->setOptions(sslOptions);

            installClientCallbacks(*client, clientState);
            if (!client->connect("127.0.0.1", m_syncPort)) {
                state.SkipWithError("client connect failed");
                break;
            }

            auto connectedFuture = clientState.connectedPromise.get_future();
            auto ackFuture = clientState.ackPromise.get_future();

            if (!waitReady(connectedFuture, kConnectTimeout)) {
                state.SkipWithError("timeout waiting client connect");
                break;
            }
            if (!waitForAccept(serverState, kConnectTimeout)) {
                state.SkipWithError("timeout waiting server accept");
                break;
            }

            const auto start = std::chrono::steady_clock::now();
            std::uint64_t sentTotal = 0;
            while (sentTotal < kOneGb) {
                auto toSend = chunkSize;
                const auto remaining = kOneGb - sentTotal;
                if (remaining < toSend) {
                    toSend = static_cast<std::size_t>(remaining);
                }

                const auto sent = client->write(chunk.data(), toSend);
                if (sent == 0) {
                    state.SkipWithError("client write failed");
                    break;
                }

                sentTotal += sent;
            }

            if (sentTotal != kOneGb) {
                client->close();
                break;
            }

            if (!waitReady(ackFuture, kAckTimeout)) {
                state.SkipWithError("timeout waiting ACK");
                client->close();
                break;
            }

            const auto end = std::chrono::steady_clock::now();
            state.SetIterationTime(std::chrono::duration<double>(end - start).count());
            client->close();
        }

        server->close();
        state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kOneGb));
    }

    void runAsyncSslAckTransfer(benchmark::State &state, std::size_t chunkSize) {
        const auto sslOptions = makeSslSocketOptions();
        if (sslOptions.m_tlsCertFile.empty() || sslOptions.m_tlsKeyFile.empty()) {
            state.SkipWithError("ssl certs not found");
            return;
        }

        const std::string chunk(chunkSize, 'A');
        auto server = m_context.createTCPServer(socket::TCPMode::SSL);
        if (!server) {
            state.SkipWithError("server creation failed");
            return;
        }
        server->setOptions(sslOptions);

        ServerState serverState{};
        serverState.expectedBytes = kOneGb;
        installSyncAcceptCallbacks(*server, serverState);

        if (!server->listen("127.0.0.1", m_asyncPort, socket::TCPMode::SSL)) {
            state.SkipWithError("server listen failed");
            return;
        }

        for (auto _ : state) {
            (void)_;
            resetServerState(serverState);

            ClientState clientState{};
            auto client = m_context.createTCPSocket(socket::TCPMode::SSL);
            if (!client) {
                state.SkipWithError("client creation failed");
                break;
            }
            client->setOptions(sslOptions);

            installClientCallbacks(*client, clientState);
            if (!client->connect("127.0.0.1", m_asyncPort)) {
                state.SkipWithError("client connect failed");
                break;
            }

            auto connectedFuture = clientState.connectedPromise.get_future();
            auto ackFuture = clientState.ackPromise.get_future();

            if (!waitReady(connectedFuture, kConnectTimeout)) {
                state.SkipWithError("timeout waiting client connect");
                break;
            }
            if (!waitForAccept(serverState, kConnectTimeout)) {
                state.SkipWithError("timeout waiting server accept");
                break;
            }

            const auto start = std::chrono::steady_clock::now();
            if (!enqueueAllAsyncWrites(client.get(), chunk.data(), chunkSize, kOneGb, start + kAckTimeout)) {
                state.SkipWithError("timeout waiting async writes");
                client->close();
                break;
            }

            if (!waitReady(ackFuture, kAckTimeout)) {
                state.SkipWithError("timeout waiting ACK");
                client->close();
                break;
            }

            const auto end = std::chrono::steady_clock::now();
            state.SetIterationTime(std::chrono::duration<double>(end - start).count());
            client->close();
        }

        server->close();
        state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kOneGb));
    }

private:
    static std::filesystem::path findRepoFile(std::string_view relativePath) {
        constexpr std::string_view candidates[] = {
            ".",
            "..",
            "../..",
            "../../..",
            "../../../..",
            "../../../../.."
        };

        for (const auto candidate : candidates) {
            const auto path = std::filesystem::path(candidate) / relativePath;
            if (std::filesystem::exists(path)) {
                return std::filesystem::absolute(path);
            }
        }

        return {};
    }

    static socket::SocketOptions makeSslSocketOptions() {
        socket::SocketOptions options{};
        options.m_connectTimeoutMs = 5000;
        options.m_tlsCertFile = findRepoFile("benchmark/support/certs/server.crt").string();
        options.m_tlsKeyFile = findRepoFile("benchmark/support/certs/server.key").string();
        options.m_tlsCaFile = options.m_tlsCertFile;
        options.m_tlsVerifyPeer = true;
        options.m_tlsServerName = "localhost";
        return options;
    }

    static std::string makeIncrementalPayload() {
        falconfix::SessionID sid{ProtocolTraits::beginString, "SERVER", "CLIENT"};
        typename ProtocolTraits::IncrementalRefresh inc{};
        inc.setMDReqID(typename ProtocolTraits::String32{"MARKETDATA_REQ"});

        auto &entries = inc.getRefMDIncGrp().getRefNoMDEntriesGroup();
        auto &group = entries.emplace_back();
        group.setMDUpdateAction('1');
        group.setMDEntryType('0');
        group.setMDEntryPx(100.01);
        group.setMDEntrySize(100);
        group.getRefInstrument().setSymbol("SYM000001");
        group.getRefInstrument().setSecurityID("1");
        group.getRefInstrument().setSecurityIDSource("8");

        FFStatus rc = ProtocolTraits::prepareOutbound(inc, 1, sid);
        if (!rc.ok()) {
            return {};
        }

        falconfix::FastBuffer out;
        rc = inc.toString(out);
        if (!rc.ok()) {
            return {};
        }

        return std::string(out.data(), out.size());
    }

    static void resetServerState(ServerState &state) {
        state.bytesReceived.store(0, std::memory_order_relaxed);
        state.ackSent.store(false, std::memory_order_release);
        std::lock_guard<std::mutex> lock(state.acceptMutex);
        state.accepted = false;
    }

    static void resetBurstServerState(BurstServerState &state, std::uint64_t expectedBytes) {
        state.bytesReceived.store(0, std::memory_order_relaxed);
        state.ackSent.store(false, std::memory_order_release);
        state.expectedBytes = expectedBytes;
        std::lock_guard<std::mutex> lock(state.acceptMutex);
        state.accepted = false;
    }

    template<typename StateT>
    static bool waitForAccept(StateT &state, std::chrono::steady_clock::duration timeout) {
        std::unique_lock<std::mutex> lock(state.acceptMutex);
        return state.acceptCv.wait_for(lock, timeout, [&state]() { return state.accepted; });
    }

    static void installSyncAcceptCallbacks(socket::ITCPServer &server, ServerState &state) {
        socket::TCPAcceptCallbacks acceptCallbacks{};
        acceptCallbacks.m_ctx = &state;
        acceptCallbacks.onAccept = [](void *ctx, std::shared_ptr<socket::ITCPSocket> socket) noexcept {
            auto *serverState = static_cast<ServerState *>(ctx);
            auto *clientCtx = new ServerClientContext{serverState, socket.get()};

            socket::SocketCallbacks callbacks{};
            callbacks.m_ctx = clientCtx;
            callbacks.onRead = [](void *ctx, socket::SocketReadView view) noexcept {
                auto *clientCtx = static_cast<ServerClientContext *>(ctx);
                const auto total = clientCtx->state->bytesReceived.fetch_add(view.m_size, std::memory_order_relaxed) + view.m_size;
                if (total < clientCtx->state->expectedBytes) {
                    return;
                }

                bool expected = false;
                if (!clientCtx->state->ackSent.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
                    return;
                }

                static constexpr char ack[] = "ACK";
                (void)clientCtx->socket->writeAsync(ack, sizeof(ack) - 1);
            };
            callbacks.onClosed = [](void *ctx) noexcept {
                delete static_cast<ServerClientContext *>(ctx);
            };

            socket->setCallbacks(callbacks);

            {
                std::lock_guard<std::mutex> lock(serverState->acceptMutex);
                serverState->accepted = true;
            }
            serverState->acceptCv.notify_one();
        };

        server.setCallbacks(acceptCallbacks);
    }

    static void installBurstAcceptCallbacks(socket::ITCPServer &server, BurstServerState &state) {
        socket::TCPAcceptCallbacks acceptCallbacks{};
        acceptCallbacks.m_ctx = &state;
        acceptCallbacks.onAccept = [](void *ctx, std::shared_ptr<socket::ITCPSocket> socket) noexcept {
            auto *serverState = static_cast<BurstServerState *>(ctx);
            auto *clientCtx = new BurstServerClientContext{serverState, socket.get()};

            socket::SocketCallbacks callbacks{};
            callbacks.m_ctx = clientCtx;
            callbacks.onRead = [](void *ctx, socket::SocketReadView view) noexcept {
                auto *clientCtx = static_cast<BurstServerClientContext *>(ctx);
                const auto total = clientCtx->state->bytesReceived.fetch_add(view.m_size, std::memory_order_relaxed) + view.m_size;
                if (total < clientCtx->state->expectedBytes) {
                    return;
                }

                bool expected = false;
                if (!clientCtx->state->ackSent.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
                    return;
                }

                static constexpr char ack[] = "ACK";
                (void)clientCtx->socket->writeAsync(ack, sizeof(ack) - 1);
            };
            callbacks.onClosed = [](void *ctx) noexcept {
                delete static_cast<BurstServerClientContext *>(ctx);
            };

            socket->setCallbacks(callbacks);

            {
                std::lock_guard<std::mutex> lock(serverState->acceptMutex);
                serverState->accepted = true;
            }
            serverState->acceptCv.notify_one();
        };

        server.setCallbacks(acceptCallbacks);
    }

    static void installClientCallbacks(socket::ITCPSocket &client, ClientState &state) {
        socket::SocketCallbacks callbacks{};
        callbacks.m_ctx = &state;
        callbacks.onConnected = [](void *ctx) noexcept {
            auto *clientState = static_cast<ClientState *>(ctx);
            try {
                clientState->connectedPromise.set_value();
            } catch (...) {
            }
        };
        callbacks.onRead = [](void *ctx, socket::SocketReadView view) noexcept {
            auto *clientState = static_cast<ClientState *>(ctx);
            clientState->recvBuf.append(view.m_data, view.m_size);
            if (!clientState->ackDone && clientState->recvBuf.find("ACK") != std::string::npos) {
                clientState->ackDone = true;
                try {
                    clientState->ackPromise.set_value();
                } catch (...) {
                }
            }
        };

        client.setCallbacks(callbacks);
    }

    static void installBurstClientCallbacks(socket::ITCPSocket &client, BurstClientState &state) {
        socket::SocketCallbacks callbacks{};
        callbacks.m_ctx = &state;
        callbacks.onConnected = [](void *ctx) noexcept {
            auto *clientState = static_cast<BurstClientState *>(ctx);
            try {
                clientState->connectedPromise.set_value();
            } catch (...) {
            }
        };
        callbacks.onRead = [](void *ctx, socket::SocketReadView view) noexcept {
            auto *clientState = static_cast<BurstClientState *>(ctx);
            clientState->recvBuf.append(view.m_data, view.m_size);
            if (!clientState->ackDone && clientState->recvBuf.find("ACK") != std::string::npos) {
                clientState->ackDone = true;
                if (!clientState->finished.exchange(true, std::memory_order_acq_rel)) {
                    try {
                        clientState->ackPromise.set_value();
                    } catch (...) {
                    }
                }
            }
        };
        callbacks.onError = [](void *ctx, falconfix::errors::ErrorCode, std::string_view) noexcept {
            auto *clientState = static_cast<BurstClientState *>(ctx);
            if (!clientState->finished.exchange(true, std::memory_order_acq_rel)) {
                try {
                    clientState->errorPromise.set_value();
                } catch (...) {
                }
            }
        };

        client.setCallbacks(callbacks);
    }

    static bool enqueueAllAsyncWrites(
        socket::ITCPSocket *client,
        const char *data,
        std::size_t chunkSize,
        std::uint64_t totalBytes,
        std::chrono::steady_clock::time_point deadline) noexcept {
        constexpr std::size_t kMaxPendingAsyncWrites = 128;

        std::uint64_t enqueued = 0;
        while (enqueued < totalBytes) {
            while (client->pendingWrites() >= kMaxPendingAsyncWrites) {
                if (std::chrono::steady_clock::now() >= deadline) {
                    return false;
                }

                std::this_thread::sleep_for(std::chrono::microseconds(50));
            }

            const auto remaining = totalBytes - enqueued;
            const auto toSend = remaining < chunkSize ? static_cast<std::size_t>(remaining) : chunkSize;
            if (!client->writeAsync(data, toSend)) {
                return false;
            }

            enqueued += toSend;
        }

        while (client->pendingWrites() > 0) {
            if (std::chrono::steady_clock::now() >= deadline) {
                return false;
            }

            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }

        return true;
    }
};

} // namespace falconfix::bench
