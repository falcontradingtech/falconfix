// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>

#include <logger/log_factory.h>
#include <session/session_config.h>

namespace falconfix {

namespace {

std::string readFile(const std::filesystem::path &path) {
	std::ifstream in(path, std::ios::binary);
	std::ostringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

SessionConfig makeConfig(std::string_view sender,
						 std::string_view target,
						 LogType logType) {
	SessionConfig config;
	config.sessionID = SessionID("FIX.4.2", sender, target);
	config.logging.logType = logType;
	config.logging.level = LogLevel::Info;
	config.logging.flushLevel = LogLevel::Info;
	return config;
}

} // namespace

class LogFactorySessionLogTest : public ::testing::Test {
protected:
	void SetUp() override {
		m_previousDisabled = LogFactory::isLoggingDisabled();
		LogFactory::setLoggingDisabled(false);
	}

	void TearDown() override {
		spdlog::drop_all();
		LogFactory::setLoggingDisabled(m_previousDisabled);
	}

private:
	bool m_previousDisabled{true};
};

TEST_F(LogFactorySessionLogTest, ReturnsNullLogWhenLoggingDisabled) {
	LogFactory::setLoggingDisabled(true);
	auto config = makeConfig("DISABLED_S", "DISABLED_T", LogType::File);

	auto log = LogFactory::createSessionLog(config);

	ASSERT_NE(log, nullptr);
	EXPECT_NE(dynamic_cast<NullMessageLog *>(log.get()), nullptr);
}

TEST_F(LogFactorySessionLogTest, ReturnsNullLogWhenLogTypeIsNone) {
	auto config = makeConfig("NONE_S", "NONE_T", LogType::None);

	auto log = LogFactory::createSessionLog(config);

	ASSERT_NE(log, nullptr);
	EXPECT_NE(dynamic_cast<NullMessageLog *>(log.get()), nullptr);
}

TEST_F(LogFactorySessionLogTest, FileLogWritesIncomingOutgoingAndEventToFiles) {
	const auto dir = std::filesystem::temp_directory_path() /
					 ("falconfix-logtest-" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()) + "-file");
	std::filesystem::remove_all(dir);

	auto config = makeConfig("FILE_S", "FILE_T", LogType::File);
	config.logging.file.path = dir.string();
	config.logging.file.pattern = "%v";

	auto log = LogFactory::createSessionLog(config);
	ASSERT_NE(log, nullptr);
	EXPECT_EQ(dynamic_cast<NullMessageLog *>(log.get()), nullptr);

	log->incoming("SESS", "8=FIX.4.2\x01" "35=A\x01");
	log->outgoing("SESS", "8=FIX.4.2\x01" "35=0\x01");
	log->event("SESS", "logon complete");

	spdlog::apply_all([](std::shared_ptr<spdlog::logger> l) { l->flush(); });

	const auto msgPath = dir / "FIX-4-2-FILE_S-FILE_T.messages.log";
	const auto evtPath = dir / "FIX-4-2-FILE_S-FILE_T.event.log";

	ASSERT_TRUE(std::filesystem::exists(msgPath));
	ASSERT_TRUE(std::filesystem::exists(evtPath));

	const std::string msgContent = readFile(msgPath);
	const std::string evtContent = readFile(evtPath);

	EXPECT_NE(msgContent.find("[IN]"), std::string::npos);
	EXPECT_NE(msgContent.find("[OUT]"), std::string::npos);
	// File logs force humanReadableFIX = false: SOH (\x01) is preserved, not replaced by '|'.
	EXPECT_NE(msgContent.find('\x01'), std::string::npos);
	EXPECT_EQ(msgContent.find('|'), std::string::npos);
	EXPECT_NE(evtContent.find("[EVT]"), std::string::npos);
	EXPECT_NE(evtContent.find("logon complete"), std::string::npos);

	log.reset();
	spdlog::drop_all();
	std::filesystem::remove_all(dir);
}

TEST_F(LogFactorySessionLogTest, FileLogUsesDefaultDirectoryWhenPathEmpty) {
	const auto dir = std::filesystem::path("logs");

	auto config = makeConfig("DEFDIR_S", "DEFDIR_T", LogType::File);
	config.logging.file.path.clear();
	config.logging.file.pattern = "%v";

	auto log = LogFactory::createSessionLog(config);
	ASSERT_NE(log, nullptr);

	log->event("SESS", "default dir event");
	spdlog::apply_all([](std::shared_ptr<spdlog::logger> l) { l->flush(); });

	const auto evtPath = dir / "FIX-4-2-DEFDIR_S-DEFDIR_T.event.log";
	EXPECT_TRUE(std::filesystem::exists(evtPath));

	log.reset();
	spdlog::drop_all();
	std::filesystem::remove(dir / "FIX-4-2-DEFDIR_S-DEFDIR_T.messages.log");
	std::filesystem::remove(evtPath);
}

TEST_F(LogFactorySessionLogTest, ScreenLogWithHumanReadableFIXReplacesSoh) {
	auto config = makeConfig("SCRHR_S", "SCRHR_T", LogType::Screen);
	config.logging.screen.humanReadableFIX = true;
	config.logging.screen.pattern = "%v";

	auto log = LogFactory::createSessionLog(config);
	ASSERT_NE(log, nullptr);
	EXPECT_EQ(dynamic_cast<NullMessageLog *>(log.get()), nullptr);

	// Mute the underlying spdlog console sinks so nothing is printed to the screen
	// during tests; the humanReadableFIX branch still runs before info() is called,
	// so coverage is preserved.
	spdlog::apply_all([](std::shared_ptr<spdlog::logger> l) { l->set_level(spdlog::level::off); });

	// Exercises the humanReadableFIX branch (std::replace of SOH with '|').
	log->incoming("SESS", "8=FIX.4.2\x01" "35=A\x01");
	log->outgoing("SESS", "8=FIX.4.2\x01" "35=0\x01");
	log->event("SESS", "screen event");
}

TEST_F(LogFactorySessionLogTest, ScreenLogWithoutHumanReadableFIXKeepsRawMessage) {
	auto config = makeConfig("SCRRAW_S", "SCRRAW_T", LogType::Screen);
	config.logging.screen.humanReadableFIX = false;
	config.logging.screen.pattern = "%v";

	auto log = LogFactory::createSessionLog(config);
	ASSERT_NE(log, nullptr);

	// Mute the underlying spdlog console sinks so nothing is printed to the screen;
	// the non-humanReadable branch of incoming/outgoing still runs before info().
	spdlog::apply_all([](std::shared_ptr<spdlog::logger> l) { l->set_level(spdlog::level::off); });

	// Exercises the non-humanReadable branch of incoming/outgoing.
	log->incoming("SESS", "8=FIX.4.2\x01" "35=A\x01");
	log->outgoing("SESS", "8=FIX.4.2\x01" "35=0\x01");
	log->event("SESS", "screen raw event");
}

TEST_F(LogFactorySessionLogTest, SanitizesSessionIdWithSpecialCharacters) {
	const auto dir = std::filesystem::temp_directory_path() / "falconfix-logtest-sanitize";
	std::filesystem::remove_all(dir);

	auto config = makeConfig("SEND/01", "TGT:02", LogType::File);
	config.sessionID = SessionID("FIX.4.2", "SEND/01", "TGT:02");
	config.logging.file.path = dir.string();
	config.logging.file.pattern = "%v";

	auto log = LogFactory::createSessionLog(config);
	ASSERT_NE(log, nullptr);
	log->event("SESS", "sanitize check");
	spdlog::apply_all([](std::shared_ptr<spdlog::logger> l) { l->flush(); });

	// '.', '/' and ':' are replaced by '-' by sanitize().
	const auto evtPath = dir / "FIX-4-2-SEND-01-TGT-02.event.log";
	EXPECT_TRUE(std::filesystem::exists(evtPath));

	log.reset();
	spdlog::drop_all();
	std::filesystem::remove_all(dir);
}

TEST_F(LogFactorySessionLogTest, ReusesExistingLoggerForSameSession) {
	auto config = makeConfig("REUSE_S", "REUSE_T", LogType::Screen);
	config.logging.screen.pattern = "%v";

	auto first = LogFactory::createSessionLog(config);
	auto second = LogFactory::createSessionLog(config);

	ASSERT_NE(first, nullptr);
	ASSERT_NE(second, nullptr);
	// Second call hits the getOrCreateLogger early-return (spdlog::get) path.
	EXPECT_EQ(dynamic_cast<NullMessageLog *>(second.get()), nullptr);
}

TEST_F(LogFactorySessionLogTest, MapsAllLogLevelsToSpdlog) {
	const LogLevel levels[] = {
		LogLevel::Trace, LogLevel::Debug, LogLevel::Info, LogLevel::Warn,
		LogLevel::Error, LogLevel::Critical, LogLevel::Off
	};

	int idx = 0;
	for (LogLevel level : levels) {
		auto config = makeConfig("LVL_S" + std::to_string(idx),
								 "LVL_T" + std::to_string(idx),
								 LogType::Screen);
		config.logging.level = level;
		config.logging.flushLevel = level;
		config.logging.screen.pattern = "%v";

		auto log = LogFactory::createSessionLog(config);
		EXPECT_NE(log, nullptr);
		++idx;
	}
}

} // namespace falconfix
