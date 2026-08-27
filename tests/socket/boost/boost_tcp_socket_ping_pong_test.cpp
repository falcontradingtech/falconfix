// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <socket/boost/boost_socket_context.h>

#include <atomic>
#include <chrono>
#include <future>
#include <mutex>
#include <string>
#include <vector>

namespace {

struct ServerState {
    std::promise<void> acceptedPromise;
    std::mutex clientsMutex;
    std::vector<falconfix::socket::ITCPSocket *> clients;
};

struct ClientState {
    std::promise<void> connectedPromise;
    std::promise<std::string> pongPromise;

    std::atomic<bool> done{false};
    std::mutex recvMutex;
    std::string recvBuf;
};

struct ServerReplyState {
    falconfix::socket::ITCPSocket *socket{nullptr};
    std::string recvBuf;
    bool replied{false};
};

void serverOnAcceptAsyncReply(void *ctx, std::shared_ptr<falconfix::socket::ITCPSocket> socket) noexcept {
    auto *state = static_cast<ServerState *>(ctx);

    {
        std::lock_guard<std::mutex> lock(state->clientsMutex);
        state->clients.push_back(socket.get());
    }

    try {
        state->acceptedPromise.set_value();
    }
    catch (...) {
    }

    falconfix::socket::SocketCallbacks callbacks{};
    auto *replyState = new ServerReplyState{socket.get()};
    callbacks.m_ctx = replyState;
    callbacks.onRead = [](void *ctx, falconfix::socket::SocketReadView view) noexcept {
        auto *state = static_cast<ServerReplyState *>(ctx);
        state->recvBuf.append(view.m_data, view.m_size);
        if (state->replied || state->recvBuf.find("PING") == std::string::npos)
            return;

        state->replied = true;
        static constexpr char kReply[] = "PONG";
        (void)state->socket->writeAsync(kReply, sizeof(kReply) - 1);
    };
    callbacks.onClosed = [](void *ctx) noexcept {
        delete static_cast<ServerReplyState *>(ctx);
    };

    socket->setCallbacks(callbacks);
}

void clientOnConnected(void *ctx) noexcept {
    auto *state = static_cast<ClientState *>(ctx);

    try {
        state->connectedPromise.set_value();
    }
    catch (...) {
    }
}

void clientOnRead(void *ctx, falconfix::socket::SocketReadView view) noexcept {
    auto *state = static_cast<ClientState *>(ctx);

    std::lock_guard<std::mutex> lock(state->recvMutex);
    state->recvBuf.append(view.m_data, view.m_size);

    if (!state->done.load(std::memory_order_acquire) &&
        state->recvBuf.find("PONG") != std::string::npos) {
        state->done.store(true, std::memory_order_release);

        try {
            state->pongPromise.set_value("PONG");
        }
        catch (...) {
        }
    }
}

} // namespace

TEST(SocketBoostTests, PingPongOverLoopback) {
    constexpr uint16_t kPort = 32001;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    ServerState serverState;
    ClientState clientState;

    auto server = engine.createTCPServer();
    ASSERT_NE(server, nullptr);

    falconfix::socket::TCPAcceptCallbacks acceptCallbacks{};
    acceptCallbacks.m_ctx = &serverState;
    acceptCallbacks.onAccept = serverOnAcceptAsyncReply;

    server->setCallbacks(acceptCallbacks);
    ASSERT_TRUE(server->listen("127.0.0.1", kPort));

    auto client = engine.createTCPSocket();
    ASSERT_NE(client, nullptr);

    falconfix::socket::SocketCallbacks clientCallbacks{};
    clientCallbacks.m_ctx = &clientState;
    clientCallbacks.onConnected = clientOnConnected;
    clientCallbacks.onRead = clientOnRead;

    client->setCallbacks(clientCallbacks);
    ASSERT_TRUE(client->connect("127.0.0.1", kPort));

    auto connectedFuture = clientState.connectedPromise.get_future();
    ASSERT_EQ(connectedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    auto acceptedFuture = serverState.acceptedPromise.get_future();
    ASSERT_EQ(acceptedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    static constexpr char kPing[] = "PING";
    ASSERT_EQ(client->write(kPing, sizeof(kPing) - 1), sizeof(kPing) - 1);

    auto pongFuture = clientState.pongPromise.get_future();
    ASSERT_EQ(pongFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);
    EXPECT_EQ(pongFuture.get(), "PONG");

    client->close();
    server->close();
    engine.stop();
}

TEST(SocketBoostTests, AsyncPingPongOverLoopback) {
    constexpr uint16_t kPort = 32002;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    ServerState serverState;
    ClientState clientState;
    auto server = engine.createTCPServer();
    ASSERT_NE(server, nullptr);

    falconfix::socket::TCPAcceptCallbacks acceptCallbacks{};
    acceptCallbacks.m_ctx = &serverState;
    acceptCallbacks.onAccept = serverOnAcceptAsyncReply;

    server->setCallbacks(acceptCallbacks);
    ASSERT_TRUE(server->listen("127.0.0.1", kPort));

    auto client = engine.createTCPSocket();
    ASSERT_NE(client, nullptr);

    falconfix::socket::SocketCallbacks clientCallbacks{};
    clientCallbacks.m_ctx = &clientState;
    clientCallbacks.onConnected = clientOnConnected;
    clientCallbacks.onRead = clientOnRead;

    client->setCallbacks(clientCallbacks);
    ASSERT_TRUE(client->connect("127.0.0.1", kPort));

    auto connectedFuture = clientState.connectedPromise.get_future();
    ASSERT_EQ(connectedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    auto acceptedFuture = serverState.acceptedPromise.get_future();
    ASSERT_EQ(acceptedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    static constexpr char kPing[] = "PING";
    ASSERT_TRUE(client->writeAsync(kPing, sizeof(kPing) - 1));

    auto pongFuture = clientState.pongPromise.get_future();
    ASSERT_EQ(pongFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);
    EXPECT_EQ(pongFuture.get(), "PONG");

    client->close();
    server->close();
    engine.stop();
}
