// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include <connection/connection_factory.h>
#include <connection/i_connection.h>
#include <session/session_base.h>
#include <session/session_settings.h>
#include <socket/i_socket_context.h>
#include <socket/i_tcp_socket.h>

namespace {
using namespace falconfix;

class FactoryFakeContext final : public socket::ISocketContext {
public:
	bool start(std::size_t) noexcept override { return true; }
	void stop() noexcept override {}
	std::shared_ptr<socket::ITCPSocket> createTCPSocket(socket::TCPMode) noexcept override { return nullptr; }
	std::shared_ptr<socket::ITCPServer> createTCPServer(socket::TCPMode) noexcept override { return nullptr; }
	std::shared_ptr<socket::IUDPSocket> createUDPSocket() noexcept override { return nullptr; }
};

class FactoryFakeSession final : public SessionBase {
public:
	SessionID id{"FIX.4.4", "SENDER", "TARGET"};
	const SessionID &sessionID() const noexcept override { return id; }
	ProtocolVersion protocolVersion() const noexcept override { return ProtocolVersion::FIX44; }
	void bindConnection(IConnection &) noexcept override {}
	FFStatus onConnected() noexcept override { return FF_OK(); }
	FFStatus onReceive(const char *, std::size_t) noexcept override { return FF_OK(); }
	FFStatus onDisconnected() noexcept override { return FF_OK(); }
	FFStatus onTimer() noexcept override { return FF_OK(); }
	FFStatus doLogout(std::string_view) noexcept override { return FF_OK(); }
	bool isDisconnected() const noexcept override { return false; }
	bool isWithinSchedule(int64_t) const noexcept override { return true; }
#ifdef FALCONFIX_ENABLE_TEST_API
	FFStatus validateSessionIdentityForTest(std::string_view, std::string_view, std::string_view) noexcept override { return FF_OK(); }
	FFStatus validateSendingTimeForTest(int64_t) noexcept override { return FF_OK(); }
	void setScheduleForTest(int64_t, int64_t) noexcept override {}
	void setNextSenderSeqNumForTest(int64_t) noexcept override {}
	void setNextTargetSeqNumForTest(int64_t) noexcept override {}
	int64_t nextSenderSeqNumForTest() const noexcept override { return 0; }
	int64_t nextTargetSeqNumForTest() const noexcept override { return 0; }
	void setAwaitingTestResponseForTest(bool, uint64_t) noexcept override {}
	bool awaitingTestResponseForTest() const noexcept override { return false; }
	uint64_t testRequestIDForTest() const noexcept override { return 0; }
#endif
};

TEST(ConnectionFactoryTests, CreatesInitiatorConnection) {
	FactoryFakeContext ctx;
	FactoryFakeSession session;
	SessionConfig cfg;
	cfg.connection.connectionType = ConnectionType::Initiator;

	auto conn = ConnectionFactory::create(ctx, session, cfg);
	EXPECT_NE(conn, nullptr);
}

TEST(ConnectionFactoryTests, CreatesAcceptorConnection) {
	FactoryFakeContext ctx;
	FactoryFakeSession session;
	SessionConfig cfg;
	cfg.connection.connectionType = ConnectionType::Acceptor;

	auto conn = ConnectionFactory::create(ctx, session, cfg);
	EXPECT_NE(conn, nullptr);
}

TEST(ConnectionFactoryTests, ReturnsNullForUnknownConnectionType) {
	FactoryFakeContext ctx;
	FactoryFakeSession session;
	SessionConfig cfg;
	cfg.connection.connectionType = static_cast<ConnectionType>(255);

	auto conn = ConnectionFactory::create(ctx, session, cfg);
	EXPECT_EQ(conn, nullptr);
}

} // namespace
