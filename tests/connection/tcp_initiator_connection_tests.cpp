// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <memory>

#include <gtest/gtest.h>

#include <connection/i_connection.h>
#include <session/session_base.h>
#include <session/session_settings.h>
#include <socket/i_socket_context.h>
#include <socket/i_tcp_socket.h>
#include <connection/tcp_initiator_connection.h>

namespace {
using namespace falconfix;

class FakeSocket final : public socket::ITCPSocket {
public:
	bool connectResult{true}; bool asyncResult{true}; std::size_t writeResult{0}; int closeCalls{0}; int callbackCalls{0};
	std::string host; uint16_t port{0}; socket::TCPMode mode{socket::TCPMode::Plain}; socket::SocketCallbacks callbacks{}; socket::SocketOptions options{};
	bool connect(std::string_view value, uint16_t valuePort, std::string_view) noexcept override { host = value; port = valuePort; return connectResult; }
	void close() noexcept override { ++closeCalls; }
	bool isOpen() const noexcept override { return true; }
	std::size_t write(const char *, std::size_t) noexcept override { return writeResult; }
	bool writeAsync(const char *, std::size_t) noexcept override { return asyncResult; }
	void setCallbacks(socket::SocketCallbacks value) noexcept override { ++callbackCalls; callbacks = value; }
	void setOptions(const socket::SocketOptions &value) noexcept override { options = value; }
	std::string_view peerIP() const noexcept override { return {}; }
	uint16_t peerPort() const noexcept override { return 0; }
	std::size_t pendingWrites() const noexcept override { return 0; }
};
class FakeContext final : public socket::ISocketContext {
public:
	std::vector<std::shared_ptr<socket::ITCPSocket>> sockets; std::size_t index{0}; int createCalls{0}; socket::TCPMode mode{socket::TCPMode::Plain};
	bool start(std::size_t) noexcept override { return true; } void stop() noexcept override {}
	std::shared_ptr<socket::ITCPSocket> createTCPSocket(socket::TCPMode value) noexcept override { ++createCalls; mode = value; return index < sockets.size() ? sockets[index++] : nullptr; }
	std::shared_ptr<socket::ITCPServer> createTCPServer(socket::TCPMode) noexcept override { return nullptr; }
	std::shared_ptr<socket::IUDPSocket> createUDPSocket() noexcept override { return nullptr; }
};
class FakeSession final : public SessionBase {
public:
	SessionID id{"FIX.4.4", "SENDER", "TARGET"}; bool disconnected{false}; bool inSchedule{true}; int connectedCalls{0}; int disconnectedCalls{0}; int receiveCalls{0};
	const SessionID &sessionID() const noexcept override { return id; } ProtocolVersion protocolVersion() const noexcept override { return ProtocolVersion::FIX44; } void bindConnection(IConnection &) noexcept override {}
	FFStatus onConnected() noexcept override { ++connectedCalls; return FF_OK(); } FFStatus onReceive(const char *, std::size_t) noexcept override { ++receiveCalls; return FF_OK(); } FFStatus onDisconnected() noexcept override { ++disconnectedCalls; return FF_OK(); }
	FFStatus onTimer() noexcept override { return FF_OK(); } FFStatus doLogout(std::string_view) noexcept override { return FF_OK(); } bool isDisconnected() const noexcept override { return disconnected; } bool isWithinSchedule(int64_t) const noexcept override { return inSchedule; }
#ifdef FALCONFIX_ENABLE_TEST_API
	FFStatus validateSessionIdentityForTest(std::string_view, std::string_view, std::string_view) noexcept override { return FF_OK(); } FFStatus validateSendingTimeForTest(int64_t) noexcept override { return FF_OK(); } void setScheduleForTest(int64_t, int64_t) noexcept override {} void setNextSenderSeqNumForTest(int64_t) noexcept override {} void setNextTargetSeqNumForTest(int64_t) noexcept override {} int64_t nextSenderSeqNumForTest() const noexcept override { return 0; } int64_t nextTargetSeqNumForTest() const noexcept override { return 0; } void setAwaitingTestResponseForTest(bool, uint64_t) noexcept override {} bool awaitingTestResponseForTest() const noexcept override { return false; } uint64_t testRequestIDForTest() const noexcept override { return 0; }
#endif
};
SessionConfig::Connection config() { SessionConfig::Connection value{}; value.socketConnectHost = "primary"; value.socketConnectPort = 1000; value.tcpNoDelay = false; value.socketSendBufferSize = 10; value.socketRecvBufferSize = 20; value.socketConnectTimeoutMs = 30; value.socketMaxWriteChunkSize = 40; value.sslCertFile = "cert"; value.sslKeyFile = "key"; value.sslCaFile = "ca"; value.sslServerName = "name"; value.sslVerifyPeer = true; return value; }

TEST(TCPInitiatorConnectionTests, StartHandlesConnectingMissingSocketAndSuccessfulPlainConnection) {
	FakeContext context; FakeSession session; auto socket = std::make_shared<FakeSocket>(); context.sockets = {socket}; auto value = config(); TCPInitiatorConnection connection(context, session, value);
	EXPECT_TRUE(connection.start().ok()); EXPECT_EQ(socket->host, value.socketConnectHost); EXPECT_EQ(socket->port, value.socketConnectPort); EXPECT_EQ(context.mode, socket::TCPMode::Plain); EXPECT_EQ(socket->options.m_tlsCertFile, value.sslCertFile); EXPECT_EQ(socket->options.m_maxWriteChunkSize, value.socketMaxWriteChunkSize);
	EXPECT_TRUE(connection.start().ok()); EXPECT_EQ(context.createCalls, 1);
	FakeContext missingContext; FakeSession missingSession; TCPInitiatorConnection missing(missingContext, missingSession, config()); EXPECT_EQ(missing.start().code, errors::code::socket::NOT_OPEN);
}
TEST(TCPInitiatorConnectionTests, StartUsesSslEndpointsAdvancesAfterFailureAndReplacesPreviousSocket) {
	auto first = std::make_shared<FakeSocket>(); first->connectResult = false; auto second = std::make_shared<FakeSocket>();
	FakeContext context; context.sockets = {first, second}; FakeSession session; auto value = config(); value.useSSL = true; value.socketConnectEndpoints = {{"one", 1}, {"two", 2}}; TCPInitiatorConnection connection(context, session, value);
	EXPECT_EQ(connection.start().code, errors::code::socket::CONNECT_FAILED); EXPECT_EQ(first->host, "one"); EXPECT_TRUE(connection.start().ok()); EXPECT_EQ(context.mode, socket::TCPMode::SSL); EXPECT_EQ(second->host, "two"); EXPECT_EQ(first->callbackCalls, 2);
}
TEST(TCPInitiatorConnectionTests, WriteAsyncWriteCloseAndDetachCoverSocketPresence) {
	FakeContext context; FakeSession session; TCPInitiatorConnection absent(context, session, config());
	EXPECT_EQ(absent.write("x", 1).code, errors::code::socket::NOT_OPEN); EXPECT_EQ(absent.writeAsync("x", 1).code, errors::code::socket::NOT_OPEN); absent.detach(); absent.close();
	auto socket = std::make_shared<FakeSocket>(); context.sockets = {socket}; TCPInitiatorConnection connection(context, session, config()); ASSERT_TRUE(connection.start().ok()); socket->writeResult = 1; socket->asyncResult = true; EXPECT_TRUE(connection.write("x", 1).ok()); EXPECT_TRUE(connection.writeAsync("x", 1).ok()); socket->writeResult = 0; socket->asyncResult = false; EXPECT_EQ(connection.write("x", 1).code, errors::code::socket::WRITE_FAILED); EXPECT_EQ(connection.writeAsync("x", 1).code, errors::code::socket::WRITE_FAILED); connection.detach(); connection.close(); EXPECT_EQ(socket->closeCalls, 1);
}
TEST(TCPInitiatorConnectionTests, CallbacksCoverStateTransitionsCloseAndErrorPaths) {
	auto first = std::make_shared<FakeSocket>(); auto second = std::make_shared<FakeSocket>(); auto third = std::make_shared<FakeSocket>();
	FakeContext context; context.sockets = {first, second, third}; FakeSession session; auto value = config(); value.socketConnectEndpoints = {{"one", 1}, {"two", 2}}; TCPInitiatorConnection connection(context, session, value); ASSERT_TRUE(connection.start().ok()); auto callbacks = first->callbacks;
	callbacks.onConnected(nullptr); callbacks.onRead(nullptr, {"x", 1}); callbacks.onClosed(nullptr); callbacks.onError(nullptr, errors::code::socket::CONNECT_FAILED, "error");
	callbacks.onRead(callbacks.m_ctx, {"x", 1}); EXPECT_EQ(session.connectedCalls, 1); EXPECT_EQ(session.receiveCalls, 1); callbacks.onConnected(callbacks.m_ctx); EXPECT_EQ(session.connectedCalls, 1);
	callbacks.onClosed(callbacks.m_ctx); EXPECT_EQ(session.disconnectedCalls, 1); EXPECT_EQ(first->callbackCalls, 2); callbacks.onClosed(callbacks.m_ctx); EXPECT_EQ(session.disconnectedCalls, 2);
	ASSERT_TRUE(connection.start().ok()); auto errorCallbacks = second->callbacks; errorCallbacks.onError(errorCallbacks.m_ctx, errors::code::socket::CONNECT_FAILED, "error"); EXPECT_EQ(second->callbackCalls, 2);
	FakeContext closingContext; auto closingSocket = std::make_shared<FakeSocket>(); closingContext.sockets = {closingSocket}; FakeSession closingSession; TCPInitiatorConnection closing(closingContext, closingSession, config()); ASSERT_TRUE(closing.start().ok()); closingSocket->callbacks.onClosed(closingSocket->callbacks.m_ctx); EXPECT_EQ(closingSession.disconnectedCalls, 1);
}
TEST(TCPInitiatorConnectionTests, TimerReturnsEarlyOrRestartsWhenDisconnectedWithinSchedule) {
	FakeContext context; auto socket = std::make_shared<FakeSocket>(); context.sockets = {socket}; FakeSession session; TCPInitiatorConnection connection(context, session, config()); IConnection &base = connection;
	EXPECT_TRUE(base.onTimer().ok()); session.disconnected = true; session.inSchedule = false; EXPECT_TRUE(base.onTimer().ok()); session.inSchedule = true; EXPECT_TRUE(base.onTimer().ok()); EXPECT_EQ(context.createCalls, 1);
}
} // namespace