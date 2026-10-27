// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>

#include <utils/string_utils.h>

namespace falconfix {

namespace {

std::string toStr(int64_t v) {
	char buf[32];
	std::size_t n = itoa(v, buf);
	return std::string(buf, n);
}

std::string utoStr(std::size_t v) {
	char buf[32];
	std::size_t n = utoa(v, buf);
	return std::string(buf, n);
}

} // namespace

TEST(StringUtilsCountDigitsTests, HandlesFullRangeOfMagnitudes) {
	EXPECT_EQ(count_digits(0), 1);
	EXPECT_EQ(count_digits(9), 1);
	EXPECT_EQ(count_digits(10), 2);
	EXPECT_EQ(count_digits(12345), 5);
	EXPECT_EQ(count_digits(1'000'000'000LL), 10);
	EXPECT_EQ(count_digits(10'000'000'000LL), 11);
	EXPECT_EQ(count_digits(1'000'000'000'000LL), 13);
	EXPECT_EQ(count_digits(1'000'000'000'000'000LL), 16);
	EXPECT_EQ(count_digits(1'000'000'000'000'000'000LL), 19);
}

TEST(StringUtilsCountDigitsTests, HandlesNegativeAndInt64Min) {
	EXPECT_EQ(count_digits(-5), 1);
	EXPECT_EQ(count_digits(-12345), 5);
	EXPECT_EQ(count_digits(INT64_MIN), 19);
}

TEST(StringUtilsItoaTests, ConvertsPositiveValues) {
	EXPECT_EQ(toStr(0), "0");
	EXPECT_EQ(toStr(7), "7");
	EXPECT_EQ(toStr(12345), "12345");
	EXPECT_EQ(toStr(9'223'372'036'854'775'807LL), "9223372036854775807");
}

TEST(StringUtilsItoaTests, ConvertsNegativeValues) {
	EXPECT_EQ(toStr(-1), "-1");
	EXPECT_EQ(toStr(-42), "-42");
	EXPECT_EQ(toStr(-9999999), "-9999999");
}

TEST(StringUtilsUtoaTests, ConvertsUnsignedValues) {
	EXPECT_EQ(utoStr(0U), "0");
	EXPECT_EQ(utoStr(1U), "1");
	EXPECT_EQ(utoStr(4096U), "4096");
	EXPECT_EQ(utoStr(1000000000ULL), "1000000000");
}

TEST(StringUtilsAtoiTests, ParsesPositiveAndNegative) {
	auto parse = [](const char *s) {
		return atoi(s, s + std::strlen(s));
	};
	EXPECT_EQ(parse("0"), 0);
	EXPECT_EQ(parse("7"), 7);
	EXPECT_EQ(parse("12345"), 12345);
	EXPECT_EQ(parse("-42"), -42);
	EXPECT_EQ(parse("9223372036854775807"), 9223372036854775807LL);
}

TEST(StringUtilsAtoiTests, RejectsEmptyAndOverlongInput) {
	const char *empty = "";
	EXPECT_EQ(atoi(empty, empty), 0);

	const char *tooLong = "123456789012345678901"; // 21 digits > 19
	EXPECT_EQ(atoi(tooLong, tooLong + std::strlen(tooLong)), 0);
}

TEST(StringUtilsIsDigitTests, DetectsDigits) {
	EXPECT_TRUE(is_digit('0'));
	EXPECT_TRUE(is_digit('5'));
	EXPECT_TRUE(is_digit('9'));
	EXPECT_FALSE(is_digit('a'));
	EXPECT_FALSE(is_digit(':')); // ':' == '9' + 1, just above the digit range
}

TEST(StringUtilsParseUint32Tests, ValidAndInvalidInputs) {
	uint32_t out = 0;

	const char *good = "4096";
	EXPECT_TRUE(parse_uint32(good, good + 4, out));
	EXPECT_EQ(out, 4096U);

	EXPECT_FALSE(parse_uint32(nullptr, nullptr, out));

	const char *empty = "x";
	EXPECT_FALSE(parse_uint32(empty, empty, out)); // begin == end

	const char *mixed = "12ab";
	EXPECT_FALSE(parse_uint32(mixed, mixed + 4, out)); // trailing non-digits
}

TEST(StringUtilsDtoaTests, HandlesNan) {
	char buf[32];
	const double nan = std::nan("");
	std::size_t n = dtoa(nan, buf);
	EXPECT_EQ(std::string(buf, n), "nan");
}

TEST(StringUtilsDtoaTests, HandlesZero) {
	char buf[32];
	std::size_t n = dtoa(0.0, buf);
	EXPECT_EQ(std::string(buf, n), "0");
}

TEST(StringUtilsDtoaTests, HandlesTypicalValues) {
	char buf[32];
	std::size_t n = dtoa(3.14, buf, 2);
	const std::string s(buf, n);
	EXPECT_FALSE(s.empty());
	EXPECT_NE(s.find("3"), std::string::npos);
}

} // namespace falconfix
