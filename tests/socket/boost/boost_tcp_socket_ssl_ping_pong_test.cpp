// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <socket/boost/boost_socket_context.h>
#include <socket/boost/boost_tcp_socket_ssl.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <future>
#include <mutex>
#include <string>
#include <vector>

namespace {

struct SSLServerState {
    std::promise<void> acceptedPromise;
    std::promise<void> connectedPromise;
    std::promise<std::string> readPromise;
    std::mutex clientsMutex;
    std::vector<falconfix::socket::ITCPSocket *> clients;
    std::atomic<bool> readDone{false};
};

struct ClientState {
    std::promise<void> connectedPromise;
    std::atomic<bool> done{false};
};

struct ErrorState {
    std::promise<std::pair<int, std::string>> errorPromise;
    std::atomic<bool> errorSet{false};
};

struct ServerReadState {
    SSLServerState *serverState{nullptr};
    std::string recvBuf;
};

void clientOnConnected(void *ctx) noexcept {
    auto *state = static_cast<ClientState *>(ctx);

    try {
        state->connectedPromise.set_value();
    }
    catch (...) {
    }
}

void captureError(void *ctx,
                  falconfix::errors::ErrorCode code,
                  std::string_view message) noexcept {
    auto *state = static_cast<ErrorState *>(ctx);
    if (!state || state->errorSet.exchange(true, std::memory_order_acq_rel))
        return;

    try {
        state->errorPromise.set_value({code, std::string(message)});
    }
    catch (...) {
    }
}

void serverOnAcceptCaptureRead(void *ctx, std::shared_ptr<falconfix::socket::ITCPSocket> socket) noexcept {
    auto *state = static_cast<SSLServerState *>(ctx);

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
    auto *readState = new ServerReadState{state};
    callbacks.m_ctx = readState;
    callbacks.onConnected = [](void *ctx) noexcept {
        auto *state = static_cast<ServerReadState *>(ctx);

        try {
            state->serverState->connectedPromise.set_value();
        }
        catch (...) {
        }
    };
    callbacks.onRead = [](void *ctx, falconfix::socket::SocketReadView view) noexcept {
        auto *state = static_cast<ServerReadState *>(ctx);
        state->recvBuf.append(view.m_data, view.m_size);

        if (state->serverState->readDone.exchange(true, std::memory_order_acq_rel))
            return;

        try {
            state->serverState->readPromise.set_value(state->recvBuf);
        }
        catch (...) {
        }
    };
    callbacks.onClosed = [](void *ctx) noexcept {
        delete static_cast<ServerReadState *>(ctx);
    };

    socket->setCallbacks(callbacks);
}

std::filesystem::path findRepoFile(std::string_view relativePath) {
    constexpr std::string_view kRoots[] = {
        ".",
        "..",
        "../..",
        "../../..",
        "../../../..",
        "../../../../.."
    };

    for (const auto root : kRoots) {
        const auto path = std::filesystem::path(root) / std::string(relativePath);
        if (std::filesystem::exists(path))
            return std::filesystem::absolute(path);
    }

    return {};
}

falconfix::socket::SocketOptions makeSSLSocketOptions() {
    falconfix::socket::SocketOptions options{};
    options.m_connectTimeoutMs = 3000;
    auto certPath = findRepoFile("server.crt");
    if (certPath.empty())
        certPath = findRepoFile("tests/socket/certs/server.crt");

    auto keyPath = findRepoFile("server.key");
    if (keyPath.empty())
        keyPath = findRepoFile("tests/socket/certs/server.key");

    options.m_tlsCertFile = certPath.string();
    options.m_tlsKeyFile = keyPath.string();
    options.m_tlsCaFile = options.m_tlsCertFile;
    options.m_tlsVerifyPeer = true;
    options.m_tlsServerName = "localhost";
    return options;
}

} // namespace

TEST(SocketSSLBoostTests, PingPongOverLoopback) {
    constexpr uint16_t kPort = 32103;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    SSLServerState serverState;
    ClientState clientState;

    const auto sslOptions = makeSSLSocketOptions();
    ASSERT_FALSE(sslOptions.m_tlsCertFile.empty());
    ASSERT_FALSE(sslOptions.m_tlsKeyFile.empty());

    auto server = engine.createTCPServer(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(server, nullptr);
    server->setOptions(sslOptions);

    falconfix::socket::TCPAcceptCallbacks acceptCallbacks{};
    acceptCallbacks.m_ctx = &serverState;
    acceptCallbacks.onAccept = serverOnAcceptCaptureRead;

    server->setCallbacks(acceptCallbacks);
    ASSERT_TRUE(server->listen("127.0.0.1", kPort, falconfix::socket::TCPMode::SSL));

    auto client = engine.createTCPSocket(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(client, nullptr);
    client->setOptions(sslOptions);

    falconfix::socket::SocketCallbacks clientCallbacks{};
    clientCallbacks.m_ctx = &clientState;
    clientCallbacks.onConnected = clientOnConnected;

    client->setCallbacks(clientCallbacks);
    ASSERT_TRUE(client->connect("127.0.0.1", kPort));

    auto connectedFuture = clientState.connectedPromise.get_future();
    ASSERT_EQ(connectedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    auto acceptedFuture = serverState.acceptedPromise.get_future();
    ASSERT_EQ(acceptedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    static constexpr char kPing[] = "PING";
    ASSERT_EQ(client->write(kPing, sizeof(kPing) - 1), sizeof(kPing) - 1);

    auto readFuture = serverState.readPromise.get_future();
    ASSERT_EQ(readFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);
    EXPECT_NE(readFuture.get().find("PING"), std::string::npos);

    client->close();
    server->close();
    engine.stop();
}

TEST(SocketSSLBoostTests, AsyncPingPongOverLoopback) {
    constexpr uint16_t kPort = 32104;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    SSLServerState serverState;
    ClientState clientState;
    const auto sslOptions = makeSSLSocketOptions();
    ASSERT_FALSE(sslOptions.m_tlsCertFile.empty());
    ASSERT_FALSE(sslOptions.m_tlsKeyFile.empty());

    auto server = engine.createTCPServer(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(server, nullptr);
    server->setOptions(sslOptions);

    falconfix::socket::TCPAcceptCallbacks acceptCallbacks{};
    acceptCallbacks.m_ctx = &serverState;
    acceptCallbacks.onAccept = serverOnAcceptCaptureRead;

    server->setCallbacks(acceptCallbacks);
    ASSERT_TRUE(server->listen("127.0.0.1", kPort, falconfix::socket::TCPMode::SSL));

    auto client = engine.createTCPSocket(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(client, nullptr);
    client->setOptions(sslOptions);

    falconfix::socket::SocketCallbacks clientCallbacks{};
    clientCallbacks.m_ctx = &clientState;
    clientCallbacks.onConnected = clientOnConnected;

    client->setCallbacks(clientCallbacks);
    ASSERT_TRUE(client->connect("127.0.0.1", kPort));

    auto connectedFuture = clientState.connectedPromise.get_future();
    ASSERT_EQ(connectedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    auto acceptedFuture = serverState.acceptedPromise.get_future();
    ASSERT_EQ(acceptedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    static constexpr char kPing[] = "PING";
    ASSERT_TRUE(client->writeAsync(kPing, sizeof(kPing) - 1));

    auto readFuture = serverState.readPromise.get_future();
    ASSERT_EQ(readFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);
    EXPECT_NE(readFuture.get().find("PING"), std::string::npos);

    client->close();
    server->close();
    engine.stop();
}

TEST(SocketSSLBoostTests, ClientSendsConfiguredServerNameInTLSHandshake) {
    constexpr uint16_t kPort = 32108;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    SSLServerState serverState;
    ClientState clientState;
    const auto sslOptions = makeSSLSocketOptions();
    ASSERT_FALSE(sslOptions.m_tlsCertFile.empty());
    ASSERT_FALSE(sslOptions.m_tlsKeyFile.empty());

    auto server = engine.createTCPServer(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(server, nullptr);
    server->setOptions(sslOptions);

    falconfix::socket::TCPAcceptCallbacks acceptCallbacks{};
    acceptCallbacks.m_ctx = &serverState;
    acceptCallbacks.onAccept = serverOnAcceptCaptureRead;

    server->setCallbacks(acceptCallbacks);
    ASSERT_TRUE(server->listen("127.0.0.1", kPort, falconfix::socket::TCPMode::SSL));

    auto client = engine.createTCPSocket(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(client, nullptr);
    client->setOptions(sslOptions);

    falconfix::socket::SocketCallbacks clientCallbacks{};
    clientCallbacks.m_ctx = &clientState;
    clientCallbacks.onConnected = clientOnConnected;

    client->setCallbacks(clientCallbacks);
    ASSERT_TRUE(client->connect("127.0.0.1", kPort));

    auto connectedFuture = clientState.connectedPromise.get_future();
    ASSERT_EQ(connectedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    auto acceptedFuture = serverState.acceptedPromise.get_future();
    ASSERT_EQ(acceptedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    auto serverConnectedFuture = serverState.connectedPromise.get_future();
    ASSERT_EQ(serverConnectedFuture.wait_for(std::chrono::seconds(3)), std::future_status::ready);

    falconfix::socket::ITCPSocket *acceptedSocketBase = nullptr;
    {
        std::lock_guard<std::mutex> lock(serverState.clientsMutex);
        ASSERT_FALSE(serverState.clients.empty());
        acceptedSocketBase = serverState.clients.front();
    }

    auto *acceptedSocket = dynamic_cast<falconfix::socket::BoostTCPSocketSSL *>(acceptedSocketBase);
    ASSERT_NE(acceptedSocket, nullptr);
    EXPECT_EQ(acceptedSocket->tlsServerName(), "localhost");

    client->close();
    server->close();
    engine.stop();
}

TEST(SocketSSLBoostTests, ServerListenFailsWithoutCredentials) {
    constexpr uint16_t kPort = 32105;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    ErrorState errorState;

    auto server = engine.createTCPServer(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(server, nullptr);

    falconfix::socket::TCPAcceptCallbacks callbacks{};
    callbacks.m_ctx = &errorState;
    callbacks.onError = captureError;
    server->setCallbacks(callbacks);

    falconfix::socket::SocketOptions options{};
    server->setOptions(options);

    EXPECT_FALSE(server->listen("127.0.0.1", kPort, falconfix::socket::TCPMode::SSL));

    auto errorFuture = errorState.errorPromise.get_future();
    ASSERT_EQ(errorFuture.wait_for(std::chrono::seconds(1)), std::future_status::ready);

    const auto [code, message] = errorFuture.get();
    EXPECT_EQ(code, falconfix::errors::code::tls::CERTIFICATE_ERROR);
    EXPECT_NE(message.find("missing_server_credentials"), std::string::npos);

    server->close();
    engine.stop();
}

TEST(SocketSSLBoostTests, ClientFailsWhenCAFileIsInvalid) {
    constexpr uint16_t kPort = 32106;

    falconfix::socket::BoostSocketContext engine;
    ASSERT_TRUE(engine.start(2));

    ErrorState serverErrorState;
    ErrorState clientErrorState;

    auto serverOptions = makeSSLSocketOptions();
    ASSERT_FALSE(serverOptions.m_tlsCertFile.empty());
    ASSERT_FALSE(serverOptions.m_tlsKeyFile.empty());
    serverOptions.m_tlsVerifyPeer = false;

    auto server = engine.createTCPServer(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(server, nullptr);
    server->setOptions(serverOptions);

    falconfix::socket::TCPAcceptCallbacks serverCallbacks{};
    serverCallbacks.m_ctx = &serverErrorState;
    serverCallbacks.onAccept = [](void *, std::shared_ptr<falconfix::socket::ITCPSocket>) noexcept {};
    serverCallbacks.onError = captureError;
    server->setCallbacks(serverCallbacks);
    ASSERT_TRUE(server->listen("127.0.0.1", kPort, falconfix::socket::TCPMode::SSL));

    auto client = engine.createTCPSocket(falconfix::socket::TCPMode::SSL);
    ASSERT_NE(client, nullptr);

    falconfix::socket::SocketOptions clientOptions{};
    clientOptions.m_connectTimeoutMs = 3000;
    clientOptions.m_tlsVerifyPeer = true;
    clientOptions.m_tlsCaFile = "/definitely/missing/falconfix-test-ca.pem";
    client->setOptions(clientOptions);

    falconfix::socket::SocketCallbacks clientCallbacks{};
    clientCallbacks.m_ctx = &clientErrorState;
    clientCallbacks.onConnected = [](void *) noexcept {};
    clientCallbacks.onError = captureError;
    client->setCallbacks(clientCallbacks);

    EXPECT_FALSE(client->connect("127.0.0.1", kPort));

    auto errorFuture = clientErrorState.errorPromise.get_future();
    ASSERT_EQ(errorFuture.wait_for(std::chrono::seconds(1)), std::future_status::ready);

    const auto [code, message] = errorFuture.get();
    EXPECT_EQ(code, falconfix::errors::code::tls::CERTIFICATE_ERROR);
    EXPECT_NE(message.find("ca_load_failed"), std::string::npos);

    client->close();
    server->close();
    engine.stop();
}
