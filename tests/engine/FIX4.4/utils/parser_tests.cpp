// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include <FIX4.4/utils/parser.h>

namespace {

using namespace fix44::parser;

std::pair<const char*, const char*> range(std::string_view s) {
	return {s.data(), s.data() + s.size()};
}

class FIX4_4_ParserTests : public ::testing::Test {};

TEST_F(FIX4_4_ParserTests, ParseInt64_Valid) {
	auto [b, e] = range("1234567890123");
	EXPECT_EQ(parse_int64(b, e), 1234567890123LL);
}

TEST_F(FIX4_4_ParserTests, ParseInt64_Negative) {
	auto [b, e] = range("-42");
	EXPECT_EQ(parse_int64(b, e), -42LL);
}

TEST_F(FIX4_4_ParserTests, ParseInt64_InvalidReturnsZero) {
	auto [b, e] = range("abc");
	EXPECT_EQ(parse_int64(b, e), 0LL);
}

TEST_F(FIX4_4_ParserTests, ParseInt32_Valid) {
	auto [b, e] = range("65535");
	EXPECT_EQ(parse_int32(b, e), 65535);
}

TEST_F(FIX4_4_ParserTests, ParseInt32_InvalidReturnsZero) {
	auto [b, e] = range("x");
	EXPECT_EQ(parse_int32(b, e), 0);
}

TEST_F(FIX4_4_ParserTests, ParseDouble_Valid) {
	auto [b, e] = range("3.14159");
	EXPECT_DOUBLE_EQ(parse_double(b, e), 3.14159);
}

TEST_F(FIX4_4_ParserTests, ParseDouble_InvalidReturnsZero) {
	auto [b, e] = range("notanumber");
	EXPECT_DOUBLE_EQ(parse_double(b, e), 0.0);
}

TEST_F(FIX4_4_ParserTests, ParseBool_True) {
	auto [b, e] = range("Y");
	EXPECT_TRUE(parse_bool(b, e));
}

TEST_F(FIX4_4_ParserTests, ParseBool_False) {
	auto [b, e] = range("N");
	EXPECT_FALSE(parse_bool(b, e));
}

TEST_F(FIX4_4_ParserTests, ParseBool_EmptyReturnsFalse) {
	const char* p = "";
	EXPECT_FALSE(parse_bool(p, p));
}

TEST_F(FIX4_4_ParserTests, ParseChar_Valid) {
	auto [b, e] = range("A");
	EXPECT_EQ(parse_char(b, e), 'A');
}

TEST_F(FIX4_4_ParserTests, ParseChar_EmptyReturnsNul) {
	const char* p = "";
	EXPECT_EQ(parse_char(p, p), '\0');
}

TEST_F(FIX4_4_ParserTests, ParseDate32_Valid) {
	auto [b, e] = range("20260101");
	EXPECT_EQ(parse_date32(b, e), 20260101);
}

TEST_F(FIX4_4_ParserTests, ParseDate32_TooShortReturnsZero) {
	auto [b, e] = range("2026");
	EXPECT_EQ(parse_date32(b, e), 0);
}

TEST_F(FIX4_4_ParserTests, ParseDate32_NonDigitReturnsZero) {
	auto [b, e] = range("2026X101");
	EXPECT_EQ(parse_date32(b, e), 0);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_NoMillis) {
	auto [b, e] = range("12:34:56");
	EXPECT_EQ(parse_timeonly32(b, e), 12 * 10000000 + 34 * 100000 + 56 * 1000);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_WithMillis) {
	auto [b, e] = range("12:34:56.789");
	EXPECT_EQ(parse_timeonly32(b, e), 12 * 10000000 + 34 * 100000 + 56 * 1000 + 789);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_PartialMillis) {
	auto [b, e] = range("00:00:00.5");
	EXPECT_EQ(parse_timeonly32(b, e), 500);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_MillisStopsOnNonDigit) {
	auto [b, e] = range("00:00:00.5x");
	EXPECT_EQ(parse_timeonly32(b, e), 500);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_TooShortReturnsZero) {
	auto [b, e] = range("12:34");
	EXPECT_EQ(parse_timeonly32(b, e), 0);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_BadSeparatorReturnsZero) {
	auto [b, e] = range("12-34-56");
	EXPECT_EQ(parse_timeonly32(b, e), 0);
}

TEST_F(FIX4_4_ParserTests, ParseTimeOnly32_NonDigitReturnsZero) {
	auto [b, e] = range("a2:34:56");
	EXPECT_EQ(parse_timeonly32(b, e), 0);
}

TEST_F(FIX4_4_ParserTests, ParseTimestamp64_Valid) {
	auto [b, e] = range("20260101123456");
	EXPECT_EQ(parse_timestamp64(b, e), 20260101123456LL);
}

TEST_F(FIX4_4_ParserTests, ParseTimestamp64_InvalidReturnsZero) {
	auto [b, e] = range("bad");
	EXPECT_EQ(parse_timestamp64(b, e), 0LL);
}

} // namespace
