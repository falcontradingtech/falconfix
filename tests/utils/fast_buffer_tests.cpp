// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <cstring>
#include <string>
#include <string_view>
#include <utility>

#include <utils/fast_buffer.h>

namespace falconfix {

TEST(FastBufferTests, DefaultConstructedIsEmpty) {
	FastBuffer buf;
	EXPECT_TRUE(buf.empty());
	EXPECT_EQ(buf.size(), 0U);
	EXPECT_EQ(buf.view(), std::string_view{});
}

TEST(FastBufferTests, ReserveGrowsCapacity) {
	FastBuffer buf;
	buf.reserve(10000);
	EXPECT_GE(buf.capacity(), 10001U);
	EXPECT_TRUE(buf.empty());
}

TEST(FastBufferTests, ExplicitCapacityConstructor) {
	FastBuffer buf(2048);
	EXPECT_GE(buf.capacity(), 2049U);
}

TEST(FastBufferTests, AppendStringViewAndView) {
	FastBuffer buf;
	buf.append(std::string_view{"hello"});
	buf.append(' ');
	buf.append("world", 5);
	EXPECT_EQ(buf.view(), "hello world");
	EXPECT_EQ(buf.size(), 11U);
}

TEST(FastBufferTests, AppendLiteralDeducesSize) {
	FastBuffer buf;
	buf.append("FIX.4.2");
	EXPECT_EQ(buf.view(), "FIX.4.2");
}

TEST(FastBufferTests, AppendFillCharacters) {
	FastBuffer buf;
	buf.append(FastBuffer::size_type{4}, 'x');
	EXPECT_EQ(buf.view(), "xxxx");
}

TEST(FastBufferTests, AppendTriggersGrowBeyondInitialCapacity) {
	FastBuffer buf;
	const std::string chunk(1000, 'a');
	// Append enough to force at least one reallocation (grow path).
	for (int i = 0; i < 10; ++i) {
		buf.append(std::string_view{chunk});
	}
	EXPECT_EQ(buf.size(), 10000U);
	EXPECT_GE(buf.capacity(), 10001U);
	EXPECT_EQ(buf.view(), std::string(10000, 'a'));
}

TEST(FastBufferTests, ConsumePartialAdvancesReadPointer) {
	FastBuffer buf;
	buf.append("abcdef");
	buf.consume(2);
	EXPECT_EQ(buf.view(), "cdef");
	EXPECT_EQ(buf.size(), 4U);
}

TEST(FastBufferTests, ConsumeAllClearsBuffer) {
	FastBuffer buf;
	buf.append("abc");
	buf.consume(3);
	EXPECT_TRUE(buf.empty());
	EXPECT_EQ(buf.size(), 0U);
}

TEST(FastBufferTests, ConsumeMoreThanSizeClears) {
	FastBuffer buf;
	buf.append("abc");
	buf.consume(100);
	EXPECT_TRUE(buf.empty());
}

TEST(FastBufferTests, ClearResetsSizeButKeepsCapacity) {
	FastBuffer buf;
	buf.append("payload");
	const auto cap = buf.capacity();
	buf.clear();
	EXPECT_TRUE(buf.empty());
	EXPECT_EQ(buf.capacity(), cap);
}

TEST(FastBufferTests, CommitAndFinalizeNullTerminate) {
	FastBuffer buf;
	buf.reserve(16);
	char *w = buf.data() + buf.size();
	std::memcpy(w, "data", 4);
	buf.commit(4);
	EXPECT_EQ(buf.view(), "data");
	EXPECT_EQ(buf.data()[buf.size()], '\0');
}

TEST(FastBufferTests, ReserveNoexceptSucceeds) {
	FastBuffer buf;
	EXPECT_TRUE(buf.reserve_noexcept(5000));
	EXPECT_GE(buf.capacity(), 5001U);
}

TEST(FastBufferTests, IndexOperatorAccessesData) {
	FastBuffer buf;
	buf.append("abc");
	EXPECT_EQ(buf[0], 'a');
	buf[0] = 'z';
	EXPECT_EQ(buf.view(), "zbc");
}

TEST(FastBufferTests, ImplicitStringViewConversion) {
	FastBuffer buf;
	buf.append("convert");
	std::string_view sv = buf;
	EXPECT_EQ(sv, "convert");
}

TEST(FastBufferTests, MoveConstructorTransfersOwnership) {
	FastBuffer src;
	src.append("moved");
	FastBuffer dst(std::move(src));
	EXPECT_EQ(dst.view(), "moved");
	EXPECT_TRUE(src.empty());
	EXPECT_EQ(src.capacity(), 0U);
}

TEST(FastBufferTests, MoveAssignmentReleasesAndTransfers) {
	FastBuffer src;
	src.append("source");

	FastBuffer dst;
	dst.append("existing");
	dst = std::move(src);

	EXPECT_EQ(dst.view(), "source");
	EXPECT_TRUE(src.empty());
}

TEST(FastBufferTests, EnsureWritableCompactsAfterConsume) {
	FastBuffer buf(16);
	const FastBuffer::size_type cap = buf.capacity();

	// Fill almost to capacity so the write tail is nearly exhausted.
	const FastBuffer::size_type firstChunk = cap - 8;
	buf.append(std::string(firstChunk, 'a'));

	// Consume most of it, freeing head space but leaving the write pointer near m_cap.
	buf.consume(firstChunk - 4);
	EXPECT_EQ(buf.size(), 4U);

	// This append needs more than the remaining tail, but head+tail is enough:
	// it must trigger the in-place memmove compaction (not a reallocation).
	buf.append(std::string(100, 'b'));

	EXPECT_EQ(buf.size(), 104U);
	EXPECT_EQ(buf.capacity(), cap); // no growth happened
	EXPECT_EQ(buf.view(), std::string(4, 'a') + std::string(100, 'b'));
}

} // namespace falconfix
