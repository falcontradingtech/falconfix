// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <memory>

#include <app/fix_application.h>
#include <logger/message_log.h>
#include <runtime/protocol_factory.h>
#include <session/session_settings.h>
#include <store/memory_message_store.h>

#include <FIX4.3/engine/protocol.h>

namespace {
using namespace falconfix;

class MinimalApplication final : public FIXApplication {
public:
	void onCreate(const SessionID &) override {}
	void onLogon(const SessionID &) override {}
	void onLogout(const SessionID &) override {}
	FFStatus toAdmin(FIXMessageRef, const SessionID &) noexcept override { return FF_OK(); }
	FFStatus fromAdmin(FIXMessageRef, const SessionID &) noexcept override { return FF_OK(); }
	FFStatus toApp(FIXMessageRef, const SessionID &) noexcept override { return FF_OK(); }
	FFStatus fromApp(FIXMessageRef, const SessionID &) noexcept override { return FF_OK(); }
};

SessionConfig makeConfig(std::string_view beginString, ProtocolVersion version) {
	SessionConfig cfg;
	cfg.sessionID = SessionID(beginString, "SENDER", "TARGET");
	cfg.protocol.version = version;
	return cfg;
}

TEST(ProtocolFactoryTests, CreatesFix43SessionByBeginString) {
	MinimalApplication app;
	auto cfg = makeConfig(fix43::Protocol::BEGIN_STRING, ProtocolVersion::FIX43);

	auto session = ProtocolFactory::createSession(
		app,
		cfg,
		std::make_unique<NullMessageLog>(),
		std::make_unique<MemoryMessageStore>());

	ASSERT_NE(session, nullptr);
	EXPECT_EQ(session->sessionID().beginString().view(), fix43::Protocol::BEGIN_STRING);
}

TEST(ProtocolFactoryTests, ReturnsNullForUnknownProtocol) {
	MinimalApplication app;
	auto cfg = makeConfig("FIX.9.9", ProtocolVersion::Unknown);

	auto session = ProtocolFactory::createSession(
		app,
		cfg,
		std::make_unique<NullMessageLog>(),
		std::make_unique<MemoryMessageStore>());

	EXPECT_EQ(session, nullptr);
}

} // namespace
