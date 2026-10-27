// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include <utils/fixed_string.h>

namespace falconfix {

TEST(FixedStringTests, EmptyReportsStateCorrectly) {
	FixedString<32> fs;
	EXPECT_TRUE(fs.empty());
	EXPECT_EQ(fs.size(), 0U);

	fs = FixedString<32>("data", 4);
	EXPECT_FALSE(fs.empty());
	EXPECT_EQ(fs.size(), 4U);

	fs.clear();
	EXPECT_TRUE(fs.empty());
}

TEST(FixedStringTests, OstreamInsertionWritesContent) {
	FixedString<32> fs("FIX.4.2", 7);

	std::ostringstream os;
	os << fs;

	EXPECT_EQ(os.str(), "FIX.4.2");
}

TEST(FixedStringTests, OstreamInsertionWritesEmpty) {
	FixedString<16> fs;

	std::ostringstream os;
	os << "[" << fs << "]";

	EXPECT_EQ(os.str(), "[]");
}

} // namespace falconfix
