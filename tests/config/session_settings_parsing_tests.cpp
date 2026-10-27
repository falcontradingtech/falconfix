// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include <config/fix_config.h>
#include <session/session_settings.h>
#include <session/session_types.h>

namespace {

falconfix::SessionConfig parseSingle(const std::string &ini) {
	const auto settings = falconfix::SessionSettings::fromConfig(falconfix::FIXConfig::parse(ini));
	EXPECT_EQ(settings.size(), 1U);
	return settings.sessions()[0];
}

std::string wrap(const std::string &defaultsBody, const std::string &sessionExtra = {}) {
	std::string ini =
		"[DEFAULT]\n"
		"ConnectionType=initiator\n"
		"SocketConnectHost=127.0.0.1\n"
		"SocketConnectPort=35001\n"
		"HeartBtInt=5\n";
	ini += defaultsBody;
	ini +=
		"\n[SESSION]\n"
		"BeginString=FIX4.4\n"
		"SenderCompID=CLIENT\n"
		"TargetCompID=SERVER\n";
	ini += sessionExtra;
	return ini;
}

} // namespace

TEST(SessionSettingsParsingTests, ParsesScreenLogTypeAndAllLogLevels) {
	const auto cfg = parseSingle(wrap(
		"LogType=screen\n"
		"LogLevel=trace\n"
		"LogFlushLevel=critical\n"
		"ScreenLogPattern=%v\n"
		"ScreenLogHumanReadableFIX=N\n"
		"StoreType=none\n"));

	EXPECT_EQ(cfg.logging.logType, falconfix::LogType::Screen);
	EXPECT_EQ(cfg.logging.level, falconfix::LogLevel::Trace);
	EXPECT_EQ(cfg.logging.flushLevel, falconfix::LogLevel::Critical);
	EXPECT_EQ(cfg.logging.screen.pattern, "%v");
	EXPECT_FALSE(cfg.logging.screen.humanReadableFIX);
}

TEST(SessionSettingsParsingTests, ParsesFileLogTypeWithRotation) {
	const auto cfg = parseSingle(wrap(
		"LogType=file\n"
		"LogLevel=debug\n"
		"FileLogPattern=%v\n"
		"FileLogPath=/tmp/logs\n"
		"FileLogRotationMaxDays=3\n"
		"FileLogRotationMaxSizeBytes=1024\n"
		"FileLogKeepFiles=7\n"
		"StoreType=none\n"));

	EXPECT_EQ(cfg.logging.logType, falconfix::LogType::File);
	EXPECT_EQ(cfg.logging.level, falconfix::LogLevel::Debug);
	EXPECT_EQ(cfg.logging.file.pattern, "%v");
	EXPECT_EQ(cfg.logging.file.path, "/tmp/logs");
	EXPECT_EQ(cfg.logging.file.rotationMaxDays, 3);
	EXPECT_EQ(cfg.logging.file.rotationMaxSizeBytes, 1024);
	EXPECT_EQ(cfg.logging.file.keepFiles, 7);
}

TEST(SessionSettingsParsingTests, ParsesWarnWarningAndErrorOffLevels) {
	EXPECT_EQ(parseSingle(wrap("LogLevel=warn\nStoreType=none\n")).logging.level,
			  falconfix::LogLevel::Warn);
	EXPECT_EQ(parseSingle(wrap("LogLevel=warning\nStoreType=none\n")).logging.level,
			  falconfix::LogLevel::Warn);
	EXPECT_EQ(parseSingle(wrap("LogLevel=error\nStoreType=none\n")).logging.level,
			  falconfix::LogLevel::Error);
	EXPECT_EQ(parseSingle(wrap("LogLevel=off\nStoreType=none\n")).logging.level,
			  falconfix::LogLevel::Off);
	EXPECT_EQ(parseSingle(wrap("LogLevel=info\nStoreType=none\n")).logging.level,
			  falconfix::LogLevel::Info);
	EXPECT_EQ(parseSingle(wrap("LogLevel=bogus\nStoreType=none\n")).logging.level,
			  falconfix::LogLevel::Info);
}

TEST(SessionSettingsParsingTests, ParsesFileStoreTypeWithPath) {
	const auto cfg = parseSingle(wrap(
		"StoreType=file\n"
		"FileStorePath=/tmp/store\n"
		"LogType=none\n"));

	EXPECT_EQ(cfg.storage.storeType, falconfix::StoreType::File);
	EXPECT_EQ(cfg.storage.path, "/tmp/store");
}

TEST(SessionSettingsParsingTests, ParsesAcceptorConnection) {
	const auto ini = std::string{
		"[DEFAULT]\n"
		"ConnectionType=acceptor\n"
		"SocketAcceptHost=0.0.0.0\n"
		"SocketAcceptPort=5001\n"
		"HeartBtInt=5\n"
		"StoreType=memory\n"
		"LogType=none\n\n"
		"[SESSION]\n"
		"BeginString=FIX4.4\n"
		"SenderCompID=SERVER\n"
		"TargetCompID=CLIENT\n"};

	const auto cfg = parseSingle(ini);
	EXPECT_EQ(cfg.connection.connectionType, falconfix::ConnectionType::Acceptor);
	EXPECT_EQ(cfg.connection.socketAcceptHost, "0.0.0.0");
	EXPECT_EQ(cfg.connection.socketAcceptPort, 5001);
	EXPECT_TRUE(cfg.connection.valid());
}

TEST(SessionSettingsParsingTests, ParsesSslCertAndKeyFilesAndTuningOptions) {
	const auto cfg = parseSingle(wrap(
		"UseSSL=Y\n"
		"SSLCertFile=/tmp/cert.pem\n"
		"SSLKeyFile=/tmp/key.pem\n"
		"SSLVerifyPeer=N\n"
		"SocketTcpNoDelay=N\n"
		"SocketSendBufferSize=4096\n"
		"SocketRecvBufferSize=8192\n"
		"SocketConnectTimeoutMs=2500\n"
		"SocketMaxWriteChunkSize=1234\n"
		"DisableHeartbeat=Y\n"
		"IgnoreTestRequest=Y\n"
		"SendRedundantResendRequests=Y\n"
		"ResetOnLogon=Y\n"
		"ResetOnLogout=Y\n"
		"ResetOnDisconnect=Y\n"
		"ValidateSendingTime=Y\n"
		"MaxLatencyMs=5000\n"
		"StoreType=none\n"));

	EXPECT_TRUE(cfg.connection.useSSL);
	EXPECT_EQ(cfg.connection.sslCertFile, "/tmp/cert.pem");
	EXPECT_EQ(cfg.connection.sslKeyFile, "/tmp/key.pem");
	EXPECT_FALSE(cfg.connection.sslVerifyPeer);
	EXPECT_FALSE(cfg.connection.tcpNoDelay);
	EXPECT_EQ(cfg.connection.socketSendBufferSize, 4096);
	EXPECT_EQ(cfg.connection.socketRecvBufferSize, 8192);
	EXPECT_EQ(cfg.connection.socketConnectTimeoutMs, 2500);
	EXPECT_EQ(cfg.connection.socketMaxWriteChunkSize, 1234U);
	EXPECT_TRUE(cfg.connection.disableHeartbeat);
	EXPECT_TRUE(cfg.connection.ignoreTestRequest);
	EXPECT_TRUE(cfg.connection.sendRedundantResendRequests);
	EXPECT_TRUE(cfg.connection.resetOnLogon);
	EXPECT_TRUE(cfg.connection.resetOnLogout);
	EXPECT_TRUE(cfg.connection.resetOnDisconnect);
	EXPECT_TRUE(cfg.connection.validateSendingTime);
	EXPECT_EQ(cfg.connection.maxLatencyMs, 5000);
}

TEST(SessionSettingsParsingTests, ParsesWeekdaysMask) {
	const auto cfg = parseSingle(wrap(
		"Weekdays=Mon,Wed,Fri\n"
		"StoreType=none\n"));

	EXPECT_TRUE(cfg.schedule.useWeekdays);
	const uint8_t expected = static_cast<uint8_t>((1u << 1) | (1u << 3) | (1u << 5));
	EXPECT_EQ(cfg.schedule.weekdaysMask, expected);
}

TEST(SessionSettingsParsingTests, ParsesWeeklyWindowFromStartDayEndDay) {
	const auto cfg = parseSingle(wrap(
		"StartDay=Monday\n"
		"EndDay=Friday\n"
		"StoreType=none\n"));

	EXPECT_TRUE(cfg.schedule.useWeeklyWindow);
	EXPECT_EQ(cfg.schedule.startDay, 1);
	EXPECT_EQ(cfg.schedule.endDay, 5);
}

TEST(SessionSettingsParsingTests, RejectsStartDayWithoutEndDay) {
	EXPECT_THROW(
		(void)parseSingle(wrap("StartDay=Monday\nStoreType=none\n")),
		std::runtime_error);
}

TEST(SessionSettingsParsingTests, RejectsWeekdaysCombinedWithWeeklyWindow) {
	EXPECT_THROW(
		(void)parseSingle(wrap(
			"Weekdays=Mon,Fri\n"
			"StartDay=Monday\n"
			"EndDay=Friday\n"
			"StoreType=none\n")),
		std::runtime_error);
}

TEST(SessionSettingsParsingTests, RejectsInvalidWeekdayToken) {
	EXPECT_THROW(
		(void)parseSingle(wrap("Weekdays=Funday\nStoreType=none\n")),
		std::runtime_error);
}

TEST(SessionSettingsParsingTests, InvalidTimeFormatYieldsZeroSeconds) {
	const auto cfg = parseSingle(wrap(
		"StartTime=9:00\n"
		"EndTime=99:99:99\n"
		"StoreType=none\n"));

	EXPECT_EQ(cfg.schedule.startSeconds, 0);
	EXPECT_EQ(cfg.schedule.endSeconds, 0);
}

TEST(SessionSettingsParsingTests, ParseProtocolVersionCoversAllVersions) {
	using PV = falconfix::ProtocolVersion;
	using SS = falconfix::SessionSettings;
	EXPECT_EQ(SS::parseProtocolVersion("FIX4.0"), PV::FIX40);
	EXPECT_EQ(SS::parseProtocolVersion("FIX4.1"), PV::FIX41);
	EXPECT_EQ(SS::parseProtocolVersion("FIX4.2"), PV::FIX42);
	EXPECT_EQ(SS::parseProtocolVersion("FIX4.3"), PV::FIX43);
	EXPECT_EQ(SS::parseProtocolVersion("FIX4.4"), PV::FIX44);
	EXPECT_EQ(SS::parseProtocolVersion("FIX5.0"), PV::FIX50);
	EXPECT_EQ(SS::parseProtocolVersion("FIX5.0SP1"), PV::FIX50SP1);
	EXPECT_EQ(SS::parseProtocolVersion("FIX5.0SP2"), PV::FIX50SP2);
	EXPECT_EQ(SS::parseProtocolVersion("FIXT1.1"), PV::FIXT11);
	EXPECT_EQ(SS::parseProtocolVersion("BOGUS"), PV::Unknown);
}

TEST(SessionSettingsParsingTests, ProtocolVersionNameCoversAllVersions) {
	using PV = falconfix::ProtocolVersion;
	using SS = falconfix::SessionSettings;
	EXPECT_EQ(SS::protocolVersionName(PV::FIX40), "FIX4.0");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX41), "FIX4.1");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX42), "FIX4.2");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX43), "FIX4.3");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX44), "FIX4.4");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX50), "FIX5.0");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX50SP1), "FIX5.0SP1");
	EXPECT_EQ(SS::protocolVersionName(PV::FIX50SP2), "FIX5.0SP2");
	EXPECT_EQ(SS::protocolVersionName(PV::FIXT11), "FIXT1.1");
	EXPECT_EQ(SS::protocolVersionName(PV::Unknown), "UNKNOWN");
}

TEST(SessionSettingsParsingTests, InvalidBoolValueFallsBackToDefault) {
    const auto cfg = parseSingle(wrap(
        "DisableHeartbeat=maybe\n"
        "UseSSL=perhaps\n"
        "StoreType=none\n"));

    EXPECT_FALSE(cfg.connection.disableHeartbeat);
    EXPECT_FALSE(cfg.connection.useSSL);
}

TEST(SessionSettingsParsingTests, UnknownConnectionStoreAndLogTypesFallBackToDefaults) {
    const auto ini = std::string{
        "[DEFAULT]\n"
        "ConnectionType=bogus\n"
        "SocketConnectHost=127.0.0.1\n"
        "SocketConnectPort=35001\n"
        "HeartBtInt=5\n"
        "StoreType=weird\n"
        "LogType=weird\n\n"
        "[SESSION]\n"
        "BeginString=FIX4.4\n"
        "SenderCompID=CLIENT\n"
        "TargetCompID=SERVER\n"};

    const auto cfg = parseSingle(ini);
    EXPECT_EQ(cfg.connection.connectionType, falconfix::ConnectionType::Initiator);
    EXPECT_EQ(cfg.storage.storeType, falconfix::StoreType::None);
    EXPECT_EQ(cfg.logging.logType, falconfix::LogType::None);
}

TEST(SessionSettingsParsingTests, TrimsTimeZoneWhitespace) {
    const auto cfg = parseSingle(wrap(
        "TimeZone=  UTC  \n"
        "StoreType=none\n"));

    EXPECT_EQ(cfg.schedule.timeZone, "UTC");
}

TEST(SessionSettingsParsingTests, InitiatorConnectionWithEndpointsIsValid) {
    const auto cfg = parseSingle(wrap("StoreType=none\n"));
    EXPECT_EQ(cfg.connection.connectionType, falconfix::ConnectionType::Initiator);
    EXPECT_TRUE(cfg.connection.valid());
}

TEST(SessionSettingsConnectionTests, InitiatorWithInvalidEndpointIsNotValid) {
    falconfix::SessionConfig::Connection c;
    c.connectionType = falconfix::ConnectionType::Initiator;
    // A present-but-invalid endpoint (empty host) makes valid() short-circuit to
    // false inside the endpoint loop.
    c.socketConnectEndpoints.push_back({"", 35001});
    EXPECT_FALSE(c.valid());
}

TEST(SessionSettingsConnectionTests, InitiatorWithoutEndpointsFallsBackToHostPort) {
    falconfix::SessionConfig::Connection c;
    c.connectionType = falconfix::ConnectionType::Initiator;
    c.socketConnectEndpoints.clear();

    c.socketConnectHost = "127.0.0.1";
    c.socketConnectPort = 35001;
    EXPECT_TRUE(c.valid());

    c.socketConnectPort = 0;
    EXPECT_FALSE(c.valid());
}

TEST(SessionSettingsParsingTests, ReconnectIntervalMsAliasIsParsed) {
    const auto cfg = parseSingle(wrap(
        "ReconnectIntervalMs=1500\n"
        "LogoutTimeoutMs=2500\n"
        "StoreType=none\n"));

    EXPECT_EQ(cfg.connection.reconnectIntervalMs, 1500U);
    EXPECT_EQ(cfg.connection.logoutTimeoutMs, 2500U);
}

TEST(SessionSettingsParsingTests, FromFileLoadsSettingsFromDisk) {
	const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
	const auto path = std::filesystem::temp_directory_path() /
		("falconfix_settings_" + std::to_string(stamp) + ".ini");

	{
		std::ofstream out(path);
		ASSERT_TRUE(out.is_open());
		out << "[DEFAULT]\n"
			   "ConnectionType=initiator\n"
			   "SocketConnectHost=127.0.0.1\n"
			   "SocketConnectPort=35001\n"
			   "HeartBtInt=5\n"
			   "StoreType=memory\n"
			   "LogType=none\n\n"
			   "[SESSION]\n"
			   "BeginString=FIX4.4\n"
			   "SenderCompID=CLIENT\n"
			   "TargetCompID=SERVER\n";
	}

	const auto settings = falconfix::SessionSettings::fromFile(path.string());
	ASSERT_EQ(settings.size(), 1U);
	EXPECT_EQ(settings.sessions()[0].sessionID.senderCompID(), "CLIENT");
	EXPECT_EQ(settings.sessions()[0].storage.storeType, falconfix::StoreType::Memory);

	std::filesystem::remove(path);
}
