// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <cstdint>
#include <ctime>
#include <string>

#include <FIX4.4/utils/datetime.h>

namespace {

using namespace fix44::datetime;

// Compact UTCTIMESTAMP encoding: YYYYMMDDHHMMSS * 1000 + ms
int64_t makeCompact(int y, int mon, int d, int h, int m, int s, int ms) {
	int64_t base = ((((((static_cast<int64_t>(y) * 100 + mon) * 100 + d) * 100 + h) * 100 + m) * 100 + s));
	return base * 1000 + ms;
}

class FIX4_4_DateTimeTests : public ::testing::Test {
protected:
	char buffer[32]{};
	std::string capture(std::size_t len) { return std::string(buffer, len); }
};

TEST_F(FIX4_4_DateTimeTests, FormatUTCTimestamp_WithMillis) {
	const auto compact = makeCompact(2026, 1, 2, 3, 4, 5, 678);
	const auto len = format_UTCTimestamp(static_cast<UTCTIMESTAMP>(compact), buffer, true);
	EXPECT_EQ(len, 21u);
	EXPECT_EQ(capture(len), "20260102-03:04:05.678");
}

TEST_F(FIX4_4_DateTimeTests, FormatUTCTimestamp_WithoutMillis) {
	const auto compact = makeCompact(2026, 12, 31, 23, 59, 59, 999);
	const auto len = format_UTCTimestamp(static_cast<UTCTIMESTAMP>(compact), buffer, false);
	EXPECT_EQ(len, 17u);
	EXPECT_EQ(capture(len), "20261231-23:59:59");
}

TEST_F(FIX4_4_DateTimeTests, FormatUTCTimestampCompact_WithMillis) {
	const auto compact = makeCompact(2026, 1, 2, 3, 4, 5, 678);
	const auto len = format_UTCTimestamp_compact(static_cast<UTCTIMESTAMP>(compact), buffer, true);
	EXPECT_EQ(len, 17u);
	EXPECT_EQ(capture(len), "20260102030405678");
}

TEST_F(FIX4_4_DateTimeTests, FormatUTCTimestampCompact_WithoutMillis) {
	const auto compact = makeCompact(2026, 1, 2, 3, 4, 5, 678);
	const auto len = format_UTCTimestamp_compact(static_cast<UTCTIMESTAMP>(compact), buffer, false);
	EXPECT_EQ(len, 14u);
	EXPECT_EQ(capture(len), "20260102030405");
}

TEST_F(FIX4_4_DateTimeTests, FormatUTCDateOnly) {
	const auto len = format_UTCDateOnly(static_cast<UTCDATEONLY>(20260102), buffer);
	EXPECT_EQ(len, 8u);
	EXPECT_EQ(capture(len), "20260102");
}

TEST_F(FIX4_4_DateTimeTests, FormatUTCTimeOnly) {
	const int64_t ms = ((3 * 60 + 4) * 60 + 5) * 1000 + 678;
	const auto len = format_UTCTimeOnly(static_cast<UTCTIMEONLY>(ms), buffer);
	EXPECT_EQ(len, 12u);
	EXPECT_EQ(capture(len), "03:04:05.678");
}

TEST_F(FIX4_4_DateTimeTests, FormatLocalMktDate) {
	const auto len = format_LocalMktDate(static_cast<LOCALMKTDATE>(20260102), buffer);
	EXPECT_EQ(len, 8u);
	EXPECT_EQ(capture(len), "20260102");
}

TEST_F(FIX4_4_DateTimeTests, NowUTCTimestampStr_WithMillis) {
	const auto len = now_UTCTimestampStr(buffer, true);
	EXPECT_EQ(len, 17u);
	EXPECT_TRUE(buffer[14] >= '0' && buffer[14] <= '9');
}

TEST_F(FIX4_4_DateTimeTests, NowUTCTimestampStr_WithoutMillis) {
	const auto len = now_UTCTimestampStr(buffer, false);
	EXPECT_EQ(len, 14u);
}

TEST_F(FIX4_4_DateTimeTests, NowFIXUTCTimestampStr_WithMillis) {
	const auto len = now_FIXUTCTimestampStr(buffer, true);
	EXPECT_EQ(len, 21u);
	EXPECT_EQ(buffer[8], '-');
	EXPECT_EQ(buffer[17], '.');
}

TEST_F(FIX4_4_DateTimeTests, NowFIXUTCTimestampStr_WithoutMillis) {
	const auto len = now_FIXUTCTimestampStr(buffer, false);
	EXPECT_EQ(len, 17u);
	EXPECT_EQ(buffer[8], '-');
}

TEST_F(FIX4_4_DateTimeTests, NowAccessorsAreConsistent) {
	const auto ts = now_UTCTimestamp();
	EXPECT_GT(ts, 0);

	const auto date = now_UTCDateOnly();
	EXPECT_GE(date, 20200101);

	const auto timeOnly = now_UTCTimeOnly();
	EXPECT_GE(timeOnly, 0);
	EXPECT_LT(timeOnly, 86400000);

	const auto localDate = now_LocalMktDate();
	EXPECT_EQ(static_cast<int64_t>(localDate), static_cast<int64_t>(date));

	const auto secs = detail::seconds_since_midnight_utc();
	EXPECT_GE(secs, 0);
	EXPECT_LT(secs, 86400);
}

TEST_F(FIX4_4_DateTimeTests, GmtimeUtc_Epoch) {
	std::tm tm{};
	gmtime_utc(static_cast<std::time_t>(0), tm);
	EXPECT_EQ(tm.tm_year + 1900, 1970);
	EXPECT_EQ(tm.tm_mon + 1, 1);
	EXPECT_EQ(tm.tm_mday, 1);
}

} // namespace
