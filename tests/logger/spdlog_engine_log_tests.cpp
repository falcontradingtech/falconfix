// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <algorithm>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <logger/spdlog_engine_log.h>

namespace falconfix {

TEST(SpdlogEngineLogTests, WritesMessagesAtEachSupportedSeverity) {
	auto sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(3);
	auto logger = std::make_shared<spdlog::logger>("spdlog-engine-log-test", sink);
	logger->set_pattern("%l:%v");
	SpdlogEngineLog engineLog(logger);

	engineLog.info("connected");
	engineLog.warn("slow peer");
	engineLog.error("disconnected");

	const auto messages = sink->last_formatted();
	ASSERT_EQ(messages.size(), 3U);

	// Normalize line endings for cross-platform compatibility
	auto normalize = [](const std::string& s) {
		std::string result = s;
		// Remove carriage returns, keep only line feeds
		result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
		return result;
	};

	EXPECT_EQ(normalize(messages[0]), "info:connected\n");
	EXPECT_EQ(normalize(messages[1]), "warning:slow peer\n");
	EXPECT_EQ(normalize(messages[2]), "error:disconnected\n");
}

} // namespace falconfix