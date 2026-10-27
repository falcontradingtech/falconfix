// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include <spdlog/spdlog.h>

#include <app/fix_application.h>
#include <config/fix_config.h>
#include <runtime/fix_runtime.h>
#include <socket/boost/boost_socket_context.h>

#include <support/app_test_support.h>
#include <support/test_runtime_support.h>

namespace {

class NoopApp final : public falconfix::FIXApplication {
public:
	void onCreate(const falconfix::SessionID &) override {}
	void onLogon(const falconfix::SessionID &) override {}
	void onLogout(const falconfix::SessionID &) override {}
	FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
	FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
	FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
	FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
};

std::string acceptorConfig(int port) {
	std::ostringstream out;
	out << "[DEFAULT]\n";
	out << "ConnectionType=acceptor\n";
	out << "SocketAcceptHost=127.0.0.1\n";
	out << "SocketAcceptPort=" << port << "\n";
	out << "HeartBtInt=5\n";
	out << "StoreType=memory\n";
	out << "LogType=none\n\n";
	out << "[SESSION]\n";
	out << "BeginString=FIX4.4\n";
	out << "SenderCompID=SERVER\n";
	out << "TargetCompID=CLIENT\n";
	return out.str();
}

} // namespace

TEST(FIXRuntimeTests, SecondStartReturnsEngineAlreadyRunning) {
	spdlog::drop_all();

	const int port = falconfix::test::findFreeLoopbackPort();
	ASSERT_GT(port, 0);

	falconfix::socket::BoostSocketContext socketEngine;
	ASSERT_TRUE(socketEngine.start(2));

	NoopApp app;
	auto settings = falconfix::SessionSettings::fromConfig(
		falconfix::FIXConfig::parse(acceptorConfig(port)));

	falconfix::FIXRuntime engine(app, std::move(settings), socketEngine);

	FFStatus rc = engine.start();
	ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

	const FFStatus second = engine.start();
	EXPECT_FALSE(second.ok());

	engine.stop();
	socketEngine.stop();
	spdlog::drop_all();
}

TEST(FIXRuntimeTests, SendAndLogoutToUnknownSessionFails) {
	spdlog::drop_all();

	const int port = falconfix::test::findFreeLoopbackPort();
	ASSERT_GT(port, 0);

	falconfix::socket::BoostSocketContext socketEngine;
	ASSERT_TRUE(socketEngine.start(2));

	NoopApp app;
	auto settings = falconfix::SessionSettings::fromConfig(
		falconfix::FIXConfig::parse(acceptorConfig(port)));

	falconfix::FIXRuntime engine(app, std::move(settings), socketEngine);

	FFStatus rc = engine.start();
	ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

	const falconfix::SessionID unknownSid(app_test_support::beginString(), "NOPE", "NADA");

	app_test_support::TestMessage msg;
	EXPECT_FALSE(engine.sendToTarget(msg, unknownSid).ok());
	EXPECT_FALSE(engine.sendLogout(unknownSid, "no such session").ok());

	EXPECT_EQ(engine.findSession(unknownSid), nullptr);

	engine.stop();
	socketEngine.stop();
	spdlog::drop_all();
}
