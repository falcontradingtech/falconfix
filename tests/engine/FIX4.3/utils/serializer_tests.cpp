// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <FIX4.3/utils/serializer.h>

namespace {

using namespace fix43::serialize;

class FIX4_3_SerializerWriteUtcTimeOnlyTests : public ::testing::Test {
protected:
	char buffer[32]{};

	// Helper to run write_utctimeonly and return the written substring plus the pointer advance.
	std::string writeAndCapture(int64_t value, bool includeMilliseconds, std::ptrdiff_t& advance) {
		std::memset(buffer, 0, sizeof(buffer));
		char* p = buffer;
		write_utctimeonly(p, value, includeMilliseconds);
		advance = p - buffer;
		return std::string(buffer, static_cast<std::size_t>(advance));
	}
};

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithMilliseconds_ZeroValue) {
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(0, true, advance);
	EXPECT_EQ(result, "00:00:00.000");
	EXPECT_EQ(advance, 12);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithoutMilliseconds_ZeroValue) {
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(0, false, advance);
	EXPECT_EQ(result, "00:00:00");
	EXPECT_EQ(advance, 8);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithMilliseconds_TypicalValue) {
	// 12:34:56.789 -> h=12, m=34, s=56, ms=789
	const int64_t value = 12LL * 10000000 + 34LL * 100000 + 56LL * 1000 + 789LL;
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(value, true, advance);
	EXPECT_EQ(result, "12:34:56.789");
	EXPECT_EQ(advance, 12);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithoutMilliseconds_TypicalValue) {
	const int64_t value = 12LL * 10000000 + 34LL * 100000 + 56LL * 1000 + 789LL;
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(value, false, advance);
	EXPECT_EQ(result, "12:34:56");
	EXPECT_EQ(advance, 8);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithMilliseconds_MaxValidTime) {
	// 23:59:59.999
	const int64_t value = 23LL * 10000000 + 59LL * 100000 + 59LL * 1000 + 999LL;
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(value, true, advance);
	EXPECT_EQ(result, "23:59:59.999");
	EXPECT_EQ(advance, 12);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithoutMilliseconds_MaxValidTime) {
	const int64_t value = 23LL * 10000000 + 59LL * 100000 + 59LL * 1000 + 999LL;
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(value, false, advance);
	EXPECT_EQ(result, "23:59:59");
	EXPECT_EQ(advance, 8);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithMilliseconds_SingleDigitComponents) {
	// 01:02:03.004 -> exercises the '/10 % 10' and '%10' branches with single-digit values
	const int64_t value = 1LL * 10000000 + 2LL * 100000 + 3LL * 1000 + 4LL;
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(value, true, advance);
	EXPECT_EQ(result, "01:02:03.004");
	EXPECT_EQ(advance, 12);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, WithMilliseconds_MillisecondsAllTensDigit) {
	// ms = 090 -> tens digit non-zero, units digit zero
	const int64_t value = 5LL * 10000000 + 6LL * 100000 + 7LL * 1000 + 90LL;
	std::ptrdiff_t advance = 0;
	const auto result = writeAndCapture(value, true, advance);
	EXPECT_EQ(result, "05:06:07.090");
	EXPECT_EQ(advance, 12);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, DefaultParameter_IncludesMilliseconds) {
	const int64_t value = 9LL * 10000000 + 8LL * 100000 + 7LL * 1000 + 6LL;
	char localBuffer[32]{};
	char* p = localBuffer;
	write_utctimeonly(p, value); // uses default includeMilliseconds = true
	const std::string result(localBuffer, static_cast<std::size_t>(p - localBuffer));
	EXPECT_EQ(result, "09:08:07.006");
	EXPECT_EQ(p - localBuffer, 12);
}

TEST_F(FIX4_3_SerializerWriteUtcTimeOnlyTests, PointerAdvancesCorrectly_WhenChained) {
	// Verify the pointer can be reused across multiple calls, confirming correct advancement.
	char localBuffer[64]{};
	char* p = localBuffer;
	write_utctimeonly(p, 0, true);
	write_utctimeonly(p, 23LL * 10000000 + 59LL * 100000 + 59LL * 1000 + 999LL, false);
	const std::string result(localBuffer, static_cast<std::size_t>(p - localBuffer));
	EXPECT_EQ(result, "00:00:00.00023:59:59");
	EXPECT_EQ(p - localBuffer, 20);
}

// ============================================================================
// PRESENCE BITMAP TESTS
// ============================================================================

class FIX4_3_SerializerPresenceBitmapTests : public ::testing::Test {
protected:
	std::array<uint64_t, 2> bitmap{};
};

TEST_F(FIX4_3_SerializerPresenceBitmapTests, IsPresent_DefaultsToFalse) {
	EXPECT_FALSE(is_present(bitmap, 0));
	EXPECT_FALSE(is_present(bitmap, 63));
	EXPECT_FALSE(is_present(bitmap, 64));
	EXPECT_FALSE(is_present(bitmap, 127));
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, SetPresence_FirstBitOfFirstWord) {
	set_presence(bitmap, 0);
	EXPECT_TRUE(is_present(bitmap, 0));
	EXPECT_EQ(bitmap[0], 1ULL);
	EXPECT_EQ(bitmap[1], 0ULL);
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, SetPresence_LastBitOfFirstWord) {
	set_presence(bitmap, 63);
	EXPECT_TRUE(is_present(bitmap, 63));
	EXPECT_EQ(bitmap[0], (uint64_t(1) << 63));
	EXPECT_EQ(bitmap[1], 0ULL);
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, SetPresence_FirstBitOfSecondWord) {
	set_presence(bitmap, 64);
	EXPECT_TRUE(is_present(bitmap, 64));
	EXPECT_EQ(bitmap[0], 0ULL);
	EXPECT_EQ(bitmap[1], 1ULL);
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, SetPresence_LastBitOfSecondWord) {
	set_presence(bitmap, 127);
	EXPECT_TRUE(is_present(bitmap, 127));
	EXPECT_EQ(bitmap[1], (uint64_t(1) << 63));
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, SetPresence_DoesNotAffectOtherBits) {
	set_presence(bitmap, 5);
	EXPECT_TRUE(is_present(bitmap, 5));
	for (std::size_t i = 0; i < 128; ++i) {
		if (i != 5) {
			EXPECT_FALSE(is_present(bitmap, i)) << "index=" << i;
		}
	}
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, ClearPresence_ClearsSetBit) {
	set_presence(bitmap, 10);
	ASSERT_TRUE(is_present(bitmap, 10));
	clear_presence(bitmap, 10);
	EXPECT_FALSE(is_present(bitmap, 10));
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, ClearPresence_OnAlreadyClearBit_IsNoOp) {
	clear_presence(bitmap, 20);
	EXPECT_FALSE(is_present(bitmap, 20));
	EXPECT_EQ(bitmap[0], 0ULL);
	EXPECT_EQ(bitmap[1], 0ULL);
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, ClearPresence_DoesNotAffectOtherBits) {
	set_presence(bitmap, 3);
	set_presence(bitmap, 70);
	clear_presence(bitmap, 3);
	EXPECT_FALSE(is_present(bitmap, 3));
	EXPECT_TRUE(is_present(bitmap, 70));
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, ClearPresence_LastBitOfSecondWord) {
	set_presence(bitmap, 127);
	clear_presence(bitmap, 127);
	EXPECT_FALSE(is_present(bitmap, 127));
	EXPECT_EQ(bitmap[1], 0ULL);
}

TEST_F(FIX4_3_SerializerPresenceBitmapTests, MultipleSetAndClear_RoundTrip) {
	for (std::size_t i = 0; i < 128; ++i) {
		set_presence(bitmap, i);
	}
	for (std::size_t i = 0; i < 128; ++i) {
		EXPECT_TRUE(is_present(bitmap, i)) << "index=" << i;
	}
	EXPECT_EQ(bitmap[0], ~uint64_t(0));
	EXPECT_EQ(bitmap[1], ~uint64_t(0));

	for (std::size_t i = 0; i < 128; ++i) {
		clear_presence(bitmap, i);
	}
	for (std::size_t i = 0; i < 128; ++i) {
		EXPECT_FALSE(is_present(bitmap, i)) << "index=" << i;
	}
	EXPECT_EQ(bitmap[0], 0ULL);
	EXPECT_EQ(bitmap[1], 0ULL);
}

} // namespace
