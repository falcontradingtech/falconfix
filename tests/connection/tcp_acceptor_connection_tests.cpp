// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <connection/tcp_acceptor_connection.h>

namespace {
using namespace falconfix;

class FakeTCPSocket final : public socket::ITCPSocket {
public:
	bool asyncWriteResult{true}; std::size_t writeResult{0}; int closeCalls{0}; int setCallbacksCalls{0};
	socket::SocketCallbacks callbacks{}; socket::SocketOptions options{};
	bool connect(std::string_view, uint16_t, std::string_view) noexcept override { return true; }
	void close() noexcept override { ++closeCalls; }
	bool isOpen() const noexcept override { return true; }
	std::size_t write(const char *, std::size_t) noexcept override { return writeResult; }
	bool writeAsync(const char *, std::size_t) noexcept override { return asyncWriteResult; }
	void setCallbacks(socket::SocketCallbacks value) noexcept override { ++setCallbacksCalls; callbacks = value; }
	void setOptions(const socket::SocketOptions &value) noexcept override { options = value; }
	std::string_view peerIP() const noexcept override { return {}; }
	uint16_t peerPort() const noexcept override { return 0; }
	std::size_t pendingWrites() const noexcept override { return 0; }
};
class FakeTCPServer final : public socket::ITCPServer {
public:
	bool listenResult{true}; int closeCalls{0}; int setCallbacksCalls{0}; std::string listenHost; uint16_t listenPort{0};
	socket::TCPMode listenMode{socket::TCPMode::Plain}; socket::TCPAcceptCallbacks callbacks{}; socket::SocketOptions options{};
	bool listen(std::string_view host, uint16_t port, socket::TCPMode mode) noexcept override { listenHost = host; listenPort = port; listenMode = mode; return listenResult; }
	void close() noexcept override { ++closeCalls; }
	bool isListening() const noexcept override { return listenResult; }
	void setCallbacks(socket::TCPAcceptCallbacks value) noexcept override { ++setCallbacksCalls; callbacks = value; }
	void setOptions(const socket::SocketOptions &value) noexcept override { options = value; }
};
class FakeSocketContext final : public socket::ISocketContext {
public:
	std::shared_ptr<socket::ITCPServer> server; socket::TCPMode requestedServerMode{socket::TCPMode::Plain};
	bool start(std::size_t) noexcept override { return true; } void stop() noexcept override {}
	std::shared_ptr<socket::ITCPSocket> createTCPSocket(socket::TCPMode) noexcept override { return nullptr; }
	std::shared_ptr<socket::ITCPServer> createTCPServer(socket::TCPMode mode) noexcept override { requestedServerMode = mode; return server; }
	std::shared_ptr<socket::IUDPSocket> createUDPSocket() noexcept override { return nullptr; }
};
class FakeSession final : public SessionBase {
public:
	SessionID id{"FIX.4.4", "SENDER", "TARGET"}; int connectedCalls{0}; int disconnectedCalls{0}; int receiveCalls{0}; const char *receivedData{nullptr}; std::size_t receivedSize{0};
	const SessionID &sessionID() const noexcept override { return id; }
	ProtocolVersion protocolVersion() const noexcept override { return ProtocolVersion::FIX44; }
	void bindConnection(IConnection &) noexcept override {}
	FFStatus onConnected() noexcept override { ++connectedCalls; return FF_OK(); }
	FFStatus onReceive(const char *data, std::size_t size) noexcept override { ++receiveCalls; receivedData = data; receivedSize = size; return FF_OK(); }
	FFStatus onDisconnected() noexcept override { ++disconnectedCalls; return FF_OK(); }
	FFStatus onTimer() noexcept override { return FF_OK(); } FFStatus doLogout(std::string_view) noexcept override { return FF_OK(); }
	bool isDisconnected() const noexcept override { return false; } bool isWithinSchedule(int64_t) const noexcept override { return true; }
#ifdef FALCONFIX_ENABLE_TEST_API
	FFStatus validateSessionIdentityForTest(std::string_view, std::string_view, std::string_view) noexcept override { return FF_OK(); }
	FFStatus validateSendingTimeForTest(int64_t) noexcept override { return FF_OK(); } void setScheduleForTest(int64_t, int64_t) noexcept override {}
	void setNextSenderSeqNumForTest(int64_t) noexcept override {} void setNextTargetSeqNumForTest(int64_t) noexcept override {}
	int64_t nextSenderSeqNumForTest() const noexcept override { return 0; } int64_t nextTargetSeqNumForTest() const noexcept override { return 0; }
	void setAwaitingTestResponseForTest(bool, uint64_t) noexcept override {} bool awaitingTestResponseForTest() const noexcept override { return false; }
	uint64_t testRequestIDForTest() const noexcept override { return 0; }
#endif
};
SessionConfig::Connection makeConfig() {
	SessionConfig::Connection config{}; config.socketAcceptPort = 7123; config.tcpNoDelay = false; config.socketSendBufferSize = 11; config.socketRecvBufferSize = 22; config.socketConnectTimeoutMs = 33; config.socketMaxWriteChunkSize = 44; config.sslCertFile = "cert.pem"; config.sslKeyFile = "key.pem"; config.sslCaFile = "ca.pem"; config.sslServerName = "server"; config.sslVerifyPeer = true; return config;
}
TEST(TCPAcceptorConnectionTests, StartReturnsNotOpenWhenServerCannotBeCreated) {
	FakeSocketContext context; FakeSession session; TCPAcceptorConnection connection(context, session, makeConfig());
	EXPECT_EQ(connection.start().code, errors::code::socket::NOT_OPEN);
}
TEST(TCPAcceptorConnectionTests, StartConfiguresPlainServerWithDefaultHostAndReportsListenFailure) {
	auto server = std::make_shared<FakeTCPServer>(); server->listenResult = false; FakeSocketContext context; context.server = server; FakeSession session; const auto config = makeConfig(); TCPAcceptorConnection connection(context, session, config);
	EXPECT_EQ(connection.start().code, errors::code::socket::LISTEN_FAILED); EXPECT_EQ(context.requestedServerMode, socket::TCPMode::Plain); EXPECT_EQ(server->listenHost, "0.0.0.0"); EXPECT_EQ(server->listenPort, config.socketAcceptPort);
	EXPECT_EQ(server->options.m_noDelay, config.tcpNoDelay); EXPECT_EQ(server->options.m_sendBufferSize, config.socketSendBufferSize); EXPECT_EQ(server->options.m_recvBufferSize, config.socketRecvBufferSize); EXPECT_EQ(server->options.m_connectTimeoutMs, config.socketConnectTimeoutMs); EXPECT_EQ(server->options.m_maxWriteChunkSize, config.socketMaxWriteChunkSize); EXPECT_EQ(server->options.m_tlsCertFile, config.sslCertFile); EXPECT_EQ(server->options.m_tlsKeyFile, config.sslKeyFile); EXPECT_EQ(server->options.m_tlsCaFile, config.sslCaFile); EXPECT_EQ(server->options.m_tlsServerName, config.sslServerName); EXPECT_EQ(server->options.m_tlsVerifyPeer, config.sslVerifyPeer);
}
TEST(TCPAcceptorConnectionTests, StartConfiguresSslServerWithExplicitHost) {
	auto server = std::make_shared<FakeTCPServer>(); FakeSocketContext context; context.server = server; FakeSession session; auto config = makeConfig(); config.useSSL = true; config.socketAcceptHost = "127.0.0.1"; TCPAcceptorConnection connection(context, session, config);
	EXPECT_TRUE(connection.start().ok()); EXPECT_EQ(context.requestedServerMode, socket::TCPMode::SSL); EXPECT_EQ(server->listenHost, config.socketAcceptHost); EXPECT_EQ(server->listenMode, socket::TCPMode::SSL);
}
TEST(TCPAcceptorConnectionTests, WriteAndWriteAsyncReportDisconnectedAndSocketResults) {
	auto server = std::make_shared<FakeTCPServer>(); FakeSocketContext context; context.server = server; FakeSession session; TCPAcceptorConnection connection(context, session, makeConfig());
	EXPECT_EQ(connection.write("abc", 3).code, errors::code::socket::NOT_CONNECTED); EXPECT_EQ(connection.writeAsync("abc", 3).code, errors::code::socket::NOT_CONNECTED); ASSERT_TRUE(connection.start().ok()); auto socket = std::make_shared<FakeTCPSocket>(); server->callbacks.onAccept(server->callbacks.m_ctx, socket);
	socket->writeResult = 3; socket->asyncWriteResult = true; EXPECT_TRUE(connection.write("abc", 3).ok()); EXPECT_TRUE(connection.writeAsync("abc", 3).ok()); socket->writeResult = 2; socket->asyncWriteResult = false; EXPECT_EQ(connection.write("abc", 3).code, errors::code::socket::WRITE_FAILED); EXPECT_EQ(connection.writeAsync("abc", 3).code, errors::code::socket::WRITE_FAILED);
}
TEST(TCPAcceptorConnectionTests, CallbacksHandleNullAndDispatchConnectedDataAndClosedEvents) {
	auto server = std::make_shared<FakeTCPServer>(); FakeSocketContext context; context.server = server; FakeSession session; TCPAcceptorConnection connection(context, session, makeConfig()); ASSERT_TRUE(connection.start().ok()); auto socket = std::make_shared<FakeTCPSocket>();
	server->callbacks.onAccept(nullptr, socket); server->callbacks.onAccept(server->callbacks.m_ctx, nullptr); EXPECT_EQ(socket->setCallbacksCalls, 0); server->callbacks.onAccept(server->callbacks.m_ctx, socket); const auto callbacks = socket->callbacks;
	callbacks.onConnected(nullptr); callbacks.onRead(nullptr, {"ignored", 7}); callbacks.onClosed(nullptr); callbacks.onRead(callbacks.m_ctx, {"ignored", 7}); EXPECT_EQ(session.receiveCalls, 0); callbacks.onConnected(callbacks.m_ctx); callbacks.onConnected(callbacks.m_ctx); EXPECT_EQ(session.connectedCalls, 1); callbacks.onRead(callbacks.m_ctx, {"data", 4}); EXPECT_EQ(session.receiveCalls, 1); EXPECT_EQ(session.receivedData, std::string_view{"data"}); EXPECT_EQ(session.receivedSize, 4U); callbacks.onClosed(callbacks.m_ctx); EXPECT_EQ(session.disconnectedCalls, 1); EXPECT_EQ(connection.write("data", 4).code, errors::code::socket::NOT_CONNECTED);
}
TEST(TCPAcceptorConnectionTests, AcceptingSecondSocketDetachesThePreviousOne) {
	auto server = std::make_shared<FakeTCPServer>(); FakeSocketContext context; context.server = server; FakeSession session; TCPAcceptorConnection connection(context, session, makeConfig()); ASSERT_TRUE(connection.start().ok()); auto firstSocket = std::make_shared<FakeTCPSocket>(); auto secondSocket = std::make_shared<FakeTCPSocket>(); server->callbacks.onAccept(server->callbacks.m_ctx, firstSocket); server->callbacks.onAccept(server->callbacks.m_ctx, secondSocket); EXPECT_EQ(firstSocket->setCallbacksCalls, 2); EXPECT_EQ(secondSocket->setCallbacksCalls, 1);
}
TEST(TCPAcceptorConnectionTests, DetachAndCloseHandlePresentAndAbsentResources) {
	FakeSocketContext emptyContext; FakeSession emptySession; TCPAcceptorConnection emptyConnection(emptyContext, emptySession, makeConfig()); emptyConnection.detach(); emptyConnection.close();
	auto server = std::make_shared<FakeTCPServer>(); FakeSocketContext context; context.server = server; FakeSession session; TCPAcceptorConnection connection(context, session, makeConfig()); ASSERT_TRUE(connection.start().ok()); auto socket = std::make_shared<FakeTCPSocket>(); server->callbacks.onAccept(server->callbacks.m_ctx, socket); connection.detach(); EXPECT_EQ(socket->setCallbacksCalls, 2); EXPECT_EQ(server->setCallbacksCalls, 2); connection.close(); EXPECT_EQ(socket->closeCalls, 1); EXPECT_EQ(server->closeCalls, 1);
}
} // namespace