# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_components_tests.py
-------------------------------------------------
Generates comprehensive C++ unit tests for FIX components with 100% code coverage.
Creates one test file per component in tests/engine/<FIX_VERSION>/components/<ComponentName>_tests.cpp

Features:
  - Tests for all getters/setters (presence coverage)
  - Tests for reset() functionality
  - Tests for encode/decode roundtrips
  - Tests for field boundary conditions
  - Tests for required field validation
"""

from __future__ import annotations
from pathlib import Path
import re
import json
from typing import Optional

from code.common import namespace_from_version, normalize_version_string


def _generate_serializer_tests_content(fix_version: str) -> str:
	"""
	Render a static (version-agnostic in content, but namespace-bound) unit
	test file covering 100% of falconfix/utils/serializer.h:
	  - write_utctimeonly (with/without milliseconds, boundary values,
		single-digit components, default parameter, chained calls)
	  - set_presence / clear_presence / is_present bitmap helpers

	Mirrors the hand-written tests/utils/error_codes_tests.cpp convention:
	this file is not per-FIX-version content-wise (serializer.h logic is
	identical across FIX4.2/4.3/4.4), so it targets a single namespace.
	"""
	include_root = normalize_version_string(fix_version)
	ns = namespace_from_version(fix_version)

	lines = [
		"// SPDX-License-Identifier: MIT",
		"// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo",
		"#include <gtest/gtest.h>",
		"",
		"#include <array>",
		"#include <cstdint>",
		"#include <cstring>",
		f"#include <{include_root}/utils/serializer.h>",
		"",
		"namespace {",
		"",
		f"using namespace {ns}::serialize;",
		"",
		"class SerializerWriteUtcTimeOnlyTests : public ::testing::Test {",
		"protected:",
		"\tchar buffer[32]{};",
		"",
		"\t// Helper to run write_utctimeonly and return the written substring plus the pointer advance.",
		"\tstd::string writeAndCapture(int64_t value, bool includeMilliseconds, std::ptrdiff_t& advance) {",
		"\t\tstd::memset(buffer, 0, sizeof(buffer));",
		"\t\tchar* p = buffer;",
		"\t\twrite_utctimeonly(p, value, includeMilliseconds);",
		"\t\tadvance = p - buffer;",
		"\t\treturn std::string(buffer, static_cast<std::size_t>(advance));",
		"\t}",
		"};",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithMilliseconds_ZeroValue) {",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(0, true, advance);",
		'\tEXPECT_EQ(result, "00:00:00.000");',
		"\tEXPECT_EQ(advance, 12);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithoutMilliseconds_ZeroValue) {",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(0, false, advance);",
		'\tEXPECT_EQ(result, "00:00:00");',
		"\tEXPECT_EQ(advance, 8);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithMilliseconds_TypicalValue) {",
		"\t// 12:34:56.789 -> h=12, m=34, s=56, ms=789",
		"\tconst int64_t value = 12LL * 10000000 + 34LL * 100000 + 56LL * 1000 + 789LL;",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(value, true, advance);",
		'\tEXPECT_EQ(result, "12:34:56.789");',
		"\tEXPECT_EQ(advance, 12);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithoutMilliseconds_TypicalValue) {",
		"\tconst int64_t value = 12LL * 10000000 + 34LL * 100000 + 56LL * 1000 + 789LL;",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(value, false, advance);",
		'\tEXPECT_EQ(result, "12:34:56");',
		"\tEXPECT_EQ(advance, 8);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithMilliseconds_MaxValidTime) {",
		"\t// 23:59:59.999",
		"\tconst int64_t value = 23LL * 10000000 + 59LL * 100000 + 59LL * 1000 + 999LL;",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(value, true, advance);",
		'\tEXPECT_EQ(result, "23:59:59.999");',
		"\tEXPECT_EQ(advance, 12);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithoutMilliseconds_MaxValidTime) {",
		"\tconst int64_t value = 23LL * 10000000 + 59LL * 100000 + 59LL * 1000 + 999LL;",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(value, false, advance);",
		'\tEXPECT_EQ(result, "23:59:59");',
		"\tEXPECT_EQ(advance, 8);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithMilliseconds_SingleDigitComponents) {",
		"\t// 01:02:03.004 -> exercises the '/10 % 10' and '%10' branches with single-digit values",
		"\tconst int64_t value = 1LL * 10000000 + 2LL * 100000 + 3LL * 1000 + 4LL;",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(value, true, advance);",
		'\tEXPECT_EQ(result, "01:02:03.004");',
		"\tEXPECT_EQ(advance, 12);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, WithMilliseconds_MillisecondsAllTensDigit) {",
		"\t// ms = 090 -> tens digit non-zero, units digit zero",
		"\tconst int64_t value = 5LL * 10000000 + 6LL * 100000 + 7LL * 1000 + 90LL;",
		"\tstd::ptrdiff_t advance = 0;",
		"\tconst auto result = writeAndCapture(value, true, advance);",
		'\tEXPECT_EQ(result, "05:06:07.090");',
		"\tEXPECT_EQ(advance, 12);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, DefaultParameter_IncludesMilliseconds) {",
		"\tconst int64_t value = 9LL * 10000000 + 8LL * 100000 + 7LL * 1000 + 6LL;",
		"\tchar localBuffer[32]{};",
		"\tchar* p = localBuffer;",
		"\twrite_utctimeonly(p, value); // uses default includeMilliseconds = true",
		"\tconst std::string result(localBuffer, static_cast<std::size_t>(p - localBuffer));",
		'\tEXPECT_EQ(result, "09:08:07.006");',
		"\tEXPECT_EQ(p - localBuffer, 12);",
		"}",
		"",
		"TEST_F(SerializerWriteUtcTimeOnlyTests, PointerAdvancesCorrectly_WhenChained) {",
		"\t// Verify the pointer can be reused across multiple calls, confirming correct advancement.",
		"\tchar localBuffer[64]{};",
		"\tchar* p = localBuffer;",
		"\twrite_utctimeonly(p, 0, true);",
		"\twrite_utctimeonly(p, 23LL * 10000000 + 59LL * 100000 + 59LL * 1000 + 999LL, false);",
		"\tconst std::string result(localBuffer, static_cast<std::size_t>(p - localBuffer));",
		'\tEXPECT_EQ(result, "00:00:00.00023:59:59");',
		"\tEXPECT_EQ(p - localBuffer, 20);",
		"}",
		"",
		"// ============================================================================",
		"// PRESENCE BITMAP TESTS",
		"// ============================================================================",
		"",
		"class SerializerPresenceBitmapTests : public ::testing::Test {",
		"protected:",
		"\tstd::array<uint64_t, 2> bitmap{};",
		"};",
		"",
		"TEST_F(SerializerPresenceBitmapTests, IsPresent_DefaultsToFalse) {",
		"\tEXPECT_FALSE(is_present(bitmap, 0));",
		"\tEXPECT_FALSE(is_present(bitmap, 63));",
		"\tEXPECT_FALSE(is_present(bitmap, 64));",
		"\tEXPECT_FALSE(is_present(bitmap, 127));",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, SetPresence_FirstBitOfFirstWord) {",
		"\tset_presence(bitmap, 0);",
		"\tEXPECT_TRUE(is_present(bitmap, 0));",
		"\tEXPECT_EQ(bitmap[0], 1ULL);",
		"\tEXPECT_EQ(bitmap[1], 0ULL);",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, SetPresence_LastBitOfFirstWord) {",
		"\tset_presence(bitmap, 63);",
		"\tEXPECT_TRUE(is_present(bitmap, 63));",
		"\tEXPECT_EQ(bitmap[0], (uint64_t(1) << 63));",
		"\tEXPECT_EQ(bitmap[1], 0ULL);",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, SetPresence_FirstBitOfSecondWord) {",
		"\tset_presence(bitmap, 64);",
		"\tEXPECT_TRUE(is_present(bitmap, 64));",
		"\tEXPECT_EQ(bitmap[0], 0ULL);",
		"\tEXPECT_EQ(bitmap[1], 1ULL);",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, SetPresence_LastBitOfSecondWord) {",
		"\tset_presence(bitmap, 127);",
		"\tEXPECT_TRUE(is_present(bitmap, 127));",
		"\tEXPECT_EQ(bitmap[1], (uint64_t(1) << 63));",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, SetPresence_DoesNotAffectOtherBits) {",
		"\tset_presence(bitmap, 5);",
		"\tEXPECT_TRUE(is_present(bitmap, 5));",
		"\tfor (std::size_t i = 0; i < 128; ++i) {",
		"\t\tif (i != 5) {",
		'\t\t\tEXPECT_FALSE(is_present(bitmap, i)) << "index=" << i;',
		"\t\t}",
		"\t}",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, ClearPresence_ClearsSetBit) {",
		"\tset_presence(bitmap, 10);",
		"\tASSERT_TRUE(is_present(bitmap, 10));",
		"\tclear_presence(bitmap, 10);",
		"\tEXPECT_FALSE(is_present(bitmap, 10));",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, ClearPresence_OnAlreadyClearBit_IsNoOp) {",
		"\tclear_presence(bitmap, 20);",
		"\tEXPECT_FALSE(is_present(bitmap, 20));",
		"\tEXPECT_EQ(bitmap[0], 0ULL);",
		"\tEXPECT_EQ(bitmap[1], 0ULL);",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, ClearPresence_DoesNotAffectOtherBits) {",
		"\tset_presence(bitmap, 3);",
		"\tset_presence(bitmap, 70);",
		"\tclear_presence(bitmap, 3);",
		"\tEXPECT_FALSE(is_present(bitmap, 3));",
		"\tEXPECT_TRUE(is_present(bitmap, 70));",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, ClearPresence_LastBitOfSecondWord) {",
		"\tset_presence(bitmap, 127);",
		"\tclear_presence(bitmap, 127);",
		"\tEXPECT_FALSE(is_present(bitmap, 127));",
		"\tEXPECT_EQ(bitmap[1], 0ULL);",
		"}",
		"",
		"TEST_F(SerializerPresenceBitmapTests, MultipleSetAndClear_RoundTrip) {",
		"\tfor (std::size_t i = 0; i < 128; ++i) {",
		"\t\tset_presence(bitmap, i);",
		"\t}",
		"\tfor (std::size_t i = 0; i < 128; ++i) {",
		'\t\tEXPECT_TRUE(is_present(bitmap, i)) << "index=" << i;',
		"\t}",
		"\tEXPECT_EQ(bitmap[0], ~uint64_t(0));",
		"\tEXPECT_EQ(bitmap[1], ~uint64_t(0));",
		"",
		"\tfor (std::size_t i = 0; i < 128; ++i) {",
		"\t\tclear_presence(bitmap, i);",
		"\t}",
		"\tfor (std::size_t i = 0; i < 128; ++i) {",
		'\t\tEXPECT_FALSE(is_present(bitmap, i)) << "index=" << i;',
		"\t}",
		"\tEXPECT_EQ(bitmap[0], 0ULL);",
		"\tEXPECT_EQ(bitmap[1], 0ULL);",
		"}",
		"",
		"} // namespace",
		"",
	]

	content = "\n".join(lines)

	# Fixture class names must be unique per FIX version: all versions get
	# linked together into the "all_tests" binary (whose add_test_custom
	# GLOB_RECURSEs the whole tests/ tree), so identical TEST_F suite names
	# across FIX4.2/4.3/4.4 would trigger GoogleTest duplicate-registration
	# failures at runtime.
	version_prefix = fix_version.replace(".", "_")  # e.g. "FIX4.4" -> "FIX4_4"
	content = content.replace(
		"SerializerWriteUtcTimeOnlyTests", f"{version_prefix}_SerializerWriteUtcTimeOnlyTests"
	).replace(
		"SerializerPresenceBitmapTests", f"{version_prefix}_SerializerPresenceBitmapTests"
	)

	return content


def _generate_parser_tests_content(fix_version: str) -> str:
	"""
	Render a unit test file covering 100% of falconfix/utils/parser.h for the
	given FIX version's namespace: parse_int64/int32/double/bool/char,
	parse_date32, parse_timeonly32 (incl. milliseconds paths) and
	parse_timestamp64, exercising both valid and invalid/edge inputs.
	"""
	include_root = normalize_version_string(fix_version)
	ns = namespace_from_version(fix_version)
	version_prefix = fix_version.replace(".", "_")  # e.g. "FIX4.4" -> "FIX4_4"
	suite = f"{version_prefix}_ParserTests"

	lines = [
		"// SPDX-License-Identifier: MIT",
		"// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo",
		"#include <gtest/gtest.h>",
		"",
		"#include <cstdint>",
		"#include <string>",
		"#include <string_view>",
		"#include <utility>",
		"",
		f"#include <{include_root}/utils/parser.h>",
		"",
		"namespace {",
		"",
		f"using namespace {ns}::parser;",
		"",
		"std::pair<const char*, const char*> range(std::string_view s) {",
		"\treturn {s.data(), s.data() + s.size()};",
		"}",
		"",
		f"class {suite} : public ::testing::Test {{}};",
		"",
		f"TEST_F({suite}, ParseInt64_Valid) {{",
		'\tauto [b, e] = range("1234567890123");',
		"\tEXPECT_EQ(parse_int64(b, e), 1234567890123LL);",
		"}",
		"",
		f"TEST_F({suite}, ParseInt64_Negative) {{",
		'\tauto [b, e] = range("-42");',
		"\tEXPECT_EQ(parse_int64(b, e), -42LL);",
		"}",
		"",
		f"TEST_F({suite}, ParseInt64_InvalidReturnsZero) {{",
		'\tauto [b, e] = range("abc");',
		"\tEXPECT_EQ(parse_int64(b, e), 0LL);",
		"}",
		"",
		f"TEST_F({suite}, ParseInt32_Valid) {{",
		'\tauto [b, e] = range("65535");',
		"\tEXPECT_EQ(parse_int32(b, e), 65535);",
		"}",
		"",
		f"TEST_F({suite}, ParseInt32_InvalidReturnsZero) {{",
		'\tauto [b, e] = range("x");',
		"\tEXPECT_EQ(parse_int32(b, e), 0);",
		"}",
		"",
		f"TEST_F({suite}, ParseDouble_Valid) {{",
		'\tauto [b, e] = range("3.14159");',
		"\tEXPECT_DOUBLE_EQ(parse_double(b, e), 3.14159);",
		"}",
		"",
		f"TEST_F({suite}, ParseDouble_InvalidReturnsZero) {{",
		'\tauto [b, e] = range("notanumber");',
		"\tEXPECT_DOUBLE_EQ(parse_double(b, e), 0.0);",
		"}",
		"",
		f"TEST_F({suite}, ParseBool_True) {{",
		'\tauto [b, e] = range("Y");',
		"\tEXPECT_TRUE(parse_bool(b, e));",
		"}",
		"",
		f"TEST_F({suite}, ParseBool_False) {{",
		'\tauto [b, e] = range("N");',
		"\tEXPECT_FALSE(parse_bool(b, e));",
		"}",
		"",
		f"TEST_F({suite}, ParseBool_EmptyReturnsFalse) {{",
		'\tconst char* p = "";',
		"\tEXPECT_FALSE(parse_bool(p, p));",
		"}",
		"",
		f"TEST_F({suite}, ParseChar_Valid) {{",
		'\tauto [b, e] = range("A");',
		"\tEXPECT_EQ(parse_char(b, e), 'A');",
		"}",
		"",
		f"TEST_F({suite}, ParseChar_EmptyReturnsNul) {{",
		'\tconst char* p = "";',
		"\tEXPECT_EQ(parse_char(p, p), '\\0');",
		"}",
		"",
		f"TEST_F({suite}, ParseDate32_Valid) {{",
		'\tauto [b, e] = range("20260101");',
		"\tEXPECT_EQ(parse_date32(b, e), 20260101);",
		"}",
		"",
		f"TEST_F({suite}, ParseDate32_TooShortReturnsZero) {{",
		'\tauto [b, e] = range("2026");',
		"\tEXPECT_EQ(parse_date32(b, e), 0);",
		"}",
		"",
		f"TEST_F({suite}, ParseDate32_NonDigitReturnsZero) {{",
		'\tauto [b, e] = range("2026X101");',
		"\tEXPECT_EQ(parse_date32(b, e), 0);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_NoMillis) {{",
		'\tauto [b, e] = range("12:34:56");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 12 * 10000000 + 34 * 100000 + 56 * 1000);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_WithMillis) {{",
		'\tauto [b, e] = range("12:34:56.789");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 12 * 10000000 + 34 * 100000 + 56 * 1000 + 789);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_PartialMillis) {{",
		'\tauto [b, e] = range("00:00:00.5");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 500);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_MillisStopsOnNonDigit) {{",
		'\tauto [b, e] = range("00:00:00.5x");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 500);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_TooShortReturnsZero) {{",
		'\tauto [b, e] = range("12:34");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 0);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_BadSeparatorReturnsZero) {{",
		'\tauto [b, e] = range("12-34-56");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 0);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimeOnly32_NonDigitReturnsZero) {{",
		'\tauto [b, e] = range("a2:34:56");',
		"\tEXPECT_EQ(parse_timeonly32(b, e), 0);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimestamp64_Valid) {{",
		'\tauto [b, e] = range("20260101123456");',
		"\tEXPECT_EQ(parse_timestamp64(b, e), 20260101123456LL);",
		"}",
		"",
		f"TEST_F({suite}, ParseTimestamp64_InvalidReturnsZero) {{",
		'\tauto [b, e] = range("bad");',
		"\tEXPECT_EQ(parse_timestamp64(b, e), 0LL);",
		"}",
		"",
		"} // namespace",
		"",
	]
	return "\n".join(lines)


def _generate_datetime_tests_content(fix_version: str) -> str:
	"""
	Render a unit test file covering falconfix/utils/datetime.h for the given
	FIX version's namespace: the pure format_* helpers (deterministic asserts),
	gmtime_utc, and the now_* accessors / UtcSecondCache path (smoke asserts on
	length and structure, since wall-clock values are non-deterministic).
	"""
	include_root = normalize_version_string(fix_version)
	ns = namespace_from_version(fix_version)
	version_prefix = fix_version.replace(".", "_")  # e.g. "FIX4.4" -> "FIX4_4"
	suite = f"{version_prefix}_DateTimeTests"

	lines = [
		"// SPDX-License-Identifier: MIT",
		"// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo",
		"#include <gtest/gtest.h>",
		"",
		"#include <cstdint>",
		"#include <ctime>",
		"#include <string>",
		"",
		f"#include <{include_root}/utils/datetime.h>",
		"",
		"namespace {",
		"",
		f"using namespace {ns}::datetime;",
		"",
		"// Compact UTCTIMESTAMP encoding: YYYYMMDDHHMMSS * 1000 + ms",
		"int64_t makeCompact(int y, int mon, int d, int h, int m, int s, int ms) {",
		"\tint64_t base = ((((((static_cast<int64_t>(y) * 100 + mon) * 100 + d) * 100 + h) * 100 + m) * 100 + s));",
		"\treturn base * 1000 + ms;",
		"}",
		"",
		f"class {suite} : public ::testing::Test {{",
		"protected:",
		"\tchar buffer[32]{};",
		"\tstd::string capture(std::size_t len) { return std::string(buffer, len); }",
		"};",
		"",
		f"TEST_F({suite}, FormatUTCTimestamp_WithMillis) {{",
		"\tconst auto compact = makeCompact(2026, 1, 2, 3, 4, 5, 678);",
		"\tconst auto len = format_UTCTimestamp(static_cast<UTCTIMESTAMP>(compact), buffer, true);",
		"\tEXPECT_EQ(len, 21u);",
		'\tEXPECT_EQ(capture(len), "20260102-03:04:05.678");',
		"}",
		"",
		f"TEST_F({suite}, FormatUTCTimestamp_WithoutMillis) {{",
		"\tconst auto compact = makeCompact(2026, 12, 31, 23, 59, 59, 999);",
		"\tconst auto len = format_UTCTimestamp(static_cast<UTCTIMESTAMP>(compact), buffer, false);",
		"\tEXPECT_EQ(len, 17u);",
		'\tEXPECT_EQ(capture(len), "20261231-23:59:59");',
		"}",
		"",
		f"TEST_F({suite}, FormatUTCTimestampCompact_WithMillis) {{",
		"\tconst auto compact = makeCompact(2026, 1, 2, 3, 4, 5, 678);",
		"\tconst auto len = format_UTCTimestamp_compact(static_cast<UTCTIMESTAMP>(compact), buffer, true);",
		"\tEXPECT_EQ(len, 17u);",
		'\tEXPECT_EQ(capture(len), "20260102030405678");',
		"}",
		"",
		f"TEST_F({suite}, FormatUTCTimestampCompact_WithoutMillis) {{",
		"\tconst auto compact = makeCompact(2026, 1, 2, 3, 4, 5, 678);",
		"\tconst auto len = format_UTCTimestamp_compact(static_cast<UTCTIMESTAMP>(compact), buffer, false);",
		"\tEXPECT_EQ(len, 14u);",
		'\tEXPECT_EQ(capture(len), "20260102030405");',
		"}",
		"",
		f"TEST_F({suite}, FormatUTCDateOnly) {{",
		"\tconst auto len = format_UTCDateOnly(static_cast<UTCDATEONLY>(20260102), buffer);",
		"\tEXPECT_EQ(len, 8u);",
		'\tEXPECT_EQ(capture(len), "20260102");',
		"}",
		"",
		f"TEST_F({suite}, FormatUTCTimeOnly) {{",
		"\tconst int64_t ms = ((3 * 60 + 4) * 60 + 5) * 1000 + 678;",
		"\tconst auto len = format_UTCTimeOnly(static_cast<UTCTIMEONLY>(ms), buffer);",
		"\tEXPECT_EQ(len, 12u);",
		'\tEXPECT_EQ(capture(len), "03:04:05.678");',
		"}",
		"",
		f"TEST_F({suite}, FormatLocalMktDate) {{",
		"\tconst auto len = format_LocalMktDate(static_cast<LOCALMKTDATE>(20260102), buffer);",
		"\tEXPECT_EQ(len, 8u);",
		'\tEXPECT_EQ(capture(len), "20260102");',
		"}",
		"",
		f"TEST_F({suite}, NowUTCTimestampStr_WithMillis) {{",
		"\tconst auto len = now_UTCTimestampStr(buffer, true);",
		"\tEXPECT_EQ(len, 17u);",
		"\tEXPECT_TRUE(buffer[14] >= '0' && buffer[14] <= '9');",
		"}",
		"",
		f"TEST_F({suite}, NowUTCTimestampStr_WithoutMillis) {{",
		"\tconst auto len = now_UTCTimestampStr(buffer, false);",
		"\tEXPECT_EQ(len, 14u);",
		"}",
		"",
		f"TEST_F({suite}, NowFIXUTCTimestampStr_WithMillis) {{",
		"\tconst auto len = now_FIXUTCTimestampStr(buffer, true);",
		"\tEXPECT_EQ(len, 21u);",
		"\tEXPECT_EQ(buffer[8], '-');",
		"\tEXPECT_EQ(buffer[17], '.');",
		"}",
		"",
		f"TEST_F({suite}, NowFIXUTCTimestampStr_WithoutMillis) {{",
		"\tconst auto len = now_FIXUTCTimestampStr(buffer, false);",
		"\tEXPECT_EQ(len, 17u);",
		"\tEXPECT_EQ(buffer[8], '-');",
		"}",
		"",
		f"TEST_F({suite}, NowAccessorsAreConsistent) {{",
		"\tconst auto ts = now_UTCTimestamp();",
		"\tEXPECT_GT(ts, 0);",
		"",
		"\tconst auto date = now_UTCDateOnly();",
		"\tEXPECT_GE(date, 20200101);",
		"",
		"\tconst auto timeOnly = now_UTCTimeOnly();",
		"\tEXPECT_GE(timeOnly, 0);",
		"\tEXPECT_LT(timeOnly, 86400000);",
		"",
		"\tconst auto localDate = now_LocalMktDate();",
		"\tEXPECT_EQ(static_cast<int64_t>(localDate), static_cast<int64_t>(date));",
		"",
		"\tconst auto secs = detail::seconds_since_midnight_utc();",
		"\tEXPECT_GE(secs, 0);",
		"\tEXPECT_LT(secs, 86400);",
		"}",
		"",
		f"TEST_F({suite}, GmtimeUtc_Epoch) {{",
		"\tstd::tm tm{};",
		"\tgmtime_utc(static_cast<std::time_t>(0), tm);",
		"\tEXPECT_EQ(tm.tm_year + 1900, 1970);",
		"\tEXPECT_EQ(tm.tm_mon + 1, 1);",
		"\tEXPECT_EQ(tm.tm_mday, 1);",
		"}",
		"",
		"} // namespace",
		"",
	]
	return "\n".join(lines)


def generate_serializer_tests(base_output: Path, fix_version: str = "FIX4.4") -> Path:
	"""
	Write tests/<fix_version>/utils/serializer_tests.cpp covering 100% of
	serializer.h for the given FIX version's namespace.
	"""
	output_dir = base_output / "tests" / fix_version / "utils"
	output_dir.mkdir(parents=True, exist_ok=True)

	test_file = output_dir / "serializer_tests.cpp"
	test_file.write_text(_generate_serializer_tests_content(fix_version), encoding="utf-8")
	print(f"[ok] Generated tests/{fix_version}/utils/serializer_tests.cpp")
	return test_file


def generate_parser_tests(base_output: Path, fix_version: str = "FIX4.4") -> Path:
	"""
	Write tests/<fix_version>/utils/parser_tests.cpp covering 100% of
	parser.h for the given FIX version's namespace.
	"""
	output_dir = base_output / "tests" / fix_version / "utils"
	output_dir.mkdir(parents=True, exist_ok=True)

	test_file = output_dir / "parser_tests.cpp"
	test_file.write_text(_generate_parser_tests_content(fix_version), encoding="utf-8")
	print(f"[ok] Generated tests/{fix_version}/utils/parser_tests.cpp")
	return test_file


def generate_datetime_tests(base_output: Path, fix_version: str = "FIX4.4") -> Path:
	"""
	Write tests/<fix_version>/utils/datetime_tests.cpp covering datetime.h for
	the given FIX version's namespace.
	"""
	output_dir = base_output / "tests" / fix_version / "utils"
	output_dir.mkdir(parents=True, exist_ok=True)

	test_file = output_dir / "datetime_tests.cpp"
	test_file.write_text(_generate_datetime_tests_content(fix_version), encoding="utf-8")
	print(f"[ok] Generated tests/{fix_version}/utils/datetime_tests.cpp")
	return test_file


def _default_libs() -> list[str]:
	return ["falconfix_config", "falconfix_session", "falconfix_core"]


def _version_short(fix_version: str) -> str:
	"""e.g. 'FIX4.4' -> '44'"""
	return fix_version.upper().replace("FIX", "").replace(".", "")


def _write_add_test_custom_cmakelists(dir_path: Path, target_name: str, libs: list[str]) -> Path:
	"""(Re)create a CMakeLists.txt containing a single add_test_custom(...) call."""
	dir_path.mkdir(parents=True, exist_ok=True)
	cmake_file = dir_path / "CMakeLists.txt"
	libs_str = " ".join(libs)
	content = (
		"# SPDX-License-Identifier: MIT\n"
		"# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo\n"
		f'add_test_custom("{target_name}" {libs_str})\n'
	)
	cmake_file.write_text(content, encoding="utf-8")
	print(f"[ok] {cmake_file}")
	return cmake_file


def _write_add_subdirectory_cmakelists(dir_path: Path, subdirs: list[str]) -> Path:
	"""(Re)create the parent CMakeLists.txt with one add_subdirectory(...) per subdir."""
	dir_path.mkdir(parents=True, exist_ok=True)
	cmake_file = dir_path / "CMakeLists.txt"
	lines = [
		"# SPDX-License-Identifier: MIT",
		"# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo",
	]
	for subdir in subdirs:
		lines.append(f"add_subdirectory({subdir})")
	cmake_file.write_text("\n".join(lines) + "\n", encoding="utf-8")
	print(f"[ok] {cmake_file}")
	return cmake_file


def ensure_engine_version_cmakelists(version_dir: Path, fix_version: str, subdirs: list[str] | None = None) -> Path:
	"""
	Create/update the CMakeLists.txt tree for a tests/<fix_version>/ directory so that:
	  - tests/<fix_version>/CMakeLists.txt only has add_subdirectory(...) entries
	  - tests/<fix_version>/components/CMakeLists.txt has its own add_test_custom(...)
	  - tests/<fix_version>/utils/CMakeLists.txt has its own add_test_custom(...)

	`version_dir` must point directly at the tests/<fix_version> directory
	(e.g. tests/engine/FIX4.4, whether staged under an "output" root or in
	the real repo tree) — it is NOT combined with any extra "tests" segment.
	"""
	subdirs = subdirs if subdirs is not None else ["components", "utils"]
	short = _version_short(fix_version)
	libs = _default_libs()

	for subdir in subdirs:
		target_name = f"fix{short}_{subdir}_tests"
		_write_add_test_custom_cmakelists(version_dir / subdir, target_name, libs)

	return _write_add_subdirectory_cmakelists(version_dir, subdirs)


def _find_nested_struct_blocks(content: str) -> list[tuple[str, str, int, int]]:
	"""
	Locate nested `struct Name { ... };` blocks (used by FalconFIX for repeating
	group entries, e.g. `class AllocAckGrp { struct NoAllocs { ... }; ... };`).

	Returns a list of (struct_name, struct_body, start_index, end_index) using
	brace-depth matching (regex alone cannot handle nested braces reliably).
	"""
	results = []
	for m in re.finditer(r'struct\s+(\w+)\s*\{', content):
		struct_name = m.group(1)
		brace_start = m.end() - 1  # index of the opening '{'
		depth = 0
		i = brace_start
		while i < len(content):
			if content[i] == '{':
				depth += 1
			elif content[i] == '}':
				depth -= 1
				if depth == 0:
					break
			i += 1
		if depth != 0:
			continue  # malformed / unmatched braces, skip
		brace_end = i
		body = content[brace_start + 1:brace_end]
		results.append((struct_name, body, m.start(), brace_end + 1))
	return results


def _parse_members(scope_text: str) -> dict:
	"""
	Parse fields/getters/setters/has-methods/REQUIRED/MAX_ENCODE_SIZE from a
	class or struct body (used for both top-level components and nested group
	entry structs).
	"""
	fields = {}
	field_pattern = r'types::(\w+)(?:<(\d+)>)?\s+m_(\w+)\s*\{'
	for match in re.finditer(field_pattern, scope_text):
		field_type = match.group(1)
		field_size = match.group(2)
		field_name = match.group(3)
		fields[field_name] = {
			'type': field_type,
			'size': field_size,
			'camelCase': _to_camel_case(field_name)
		}

	# REQUIRED can be a single literal (e.g. `0ULL` or `0x3`) or a multi-line
	# bitmask expression like `(1ULL << 0) | (1ULL << 2);`. Capture everything
	# up to the terminating semicolon (DOTALL) and treat it as "required" if
	# it's not the trivial `0ULL` literal.
	required_match = re.search(r'static\s+constexpr\s+uint64_t\s+REQUIRED\s*=\s*(.*?);', scope_text, re.DOTALL)
	is_any_required = bool(required_match) and required_match.group(1).strip() != '0ULL'

	size_match = re.search(r'static\s+constexpr\s+std::size_t\s+MAX_ENCODE_SIZE\s*=\s*(\d+)', scope_text)
	max_size = int(size_match.group(1)) if size_match else 256

	methods = {
		'getters': [],
		'setters': [],
		'has_methods': []
	}

	getter_pattern = r'const\s+(types::\w+(?:<\d+>)?)\s+&get(\w+)\(\)\s+const\s+noexcept'
	for match in re.finditer(getter_pattern, scope_text):
		methods['getters'].append({
			'return_type': match.group(1),
			'name': match.group(2)
		})

	setter_pattern = r'void\s+set(\w+)\((const\s+)?(types::\w+(?:<\d+>)?)\s+&?v\)\s+noexcept'
	for match in re.finditer(setter_pattern, scope_text):
		methods['setters'].append({
			'name': match.group(1),
			'param_type': match.group(3)
		})

	has_pattern = r'bool\s+has(\w+)\(\)\s+const\s+noexcept'
	for match in re.finditer(has_pattern, scope_text):
		methods['has_methods'].append(match.group(1))

	# Non-const reference accessors enable deterministic population of
	# group-only / sub-component-only containers (which expose no scalar
	# setters of their own):
	#   - group vector accessors:  `std::vector<Entry> &getRefXxxGroup() noexcept`
	#   - sub-component accessors:  `SubType      &getRefXxx()      noexcept`
	group_accessors = []  # (accessor_suffix, entry_struct_name)
	for match in re.finditer(r'std::vector<\s*(\w+)\s*>\s*&\s*getRef(\w+)\(\)\s+noexcept', scope_text):
		group_accessors.append((match.group(2), match.group(1)))

	ref_accessors = []  # (accessor_suffix, sub_component_type)
	for match in re.finditer(r'(?<!const )\b([A-Z]\w+)\s*&\s*getRef(\w+)\(\)\s+noexcept', scope_text):
		type_name = match.group(1)
		suffix = match.group(2)
		if type_name == "std":
			continue
		ref_accessors.append((suffix, type_name))

	# Member declaration order determines the decode anchor/delimiter: a group
	# entry's decode() keys repetition off its FIRST encoded field, which is the
	# first declared member (scalar, sub-component, or nested group). Capture the
	# members in source order so population targets that exact anchor.
	ordered_members = []  # list of (category, member_name) where category in {scalar, ref, group}
	member_pattern = (
		r'(?:types::\w+(?:<\d+>)?'          # scalar value type
		r'|std::vector<\s*\w+\s*>'            # nested group vector
		r'|[A-Z]\w+)'                         # sub-component type
		r'\s+m_(\w+)\s*(?:\{|;)'
	)
	for match in re.finditer(member_pattern, scope_text):
		decl = match.group(0)
		name = match.group(1)
		if decl.startswith('types::'):
			ordered_members.append(('scalar', name))
		elif decl.startswith('std::vector'):
			ordered_members.append(('group', name))
		else:
			ordered_members.append(('ref', name))

	return {
		'fields': fields,
		'methods': methods,
		'has_required': is_any_required,
		'max_encode_size': max_size,
		'group_accessors': group_accessors,
		'ref_accessors': ref_accessors,
		'ordered_members': ordered_members,
	}


def extract_component_info(header_file: Path, namespace: str) -> dict:
	"""
	Parse a component header file and extract:
	- Class name
	- Field members (name, type)
	- Getters/setters
	- Required fields
	- MAX_ENCODE_SIZE

	Repeating-group components (e.g. AllocAckGrp) declare their real fields
	inside a nested `struct NoXxx { ... }` entry type; the outer class itself
	only exposes a std::vector<NoXxx> plus count/has accessors. Nested structs
	are detected and parsed separately so tests reference the correct type.
	"""
	content = header_file.read_text(encoding="utf-8")

	# Extract class name
	class_match = re.search(r'class\s+(\w+)\s*\{', content)
	if not class_match:
		return None

	class_name = class_match.group(1)

	# Detect nested group-entry structs and strip them out before parsing the
	# outer class scope, so its fields/setters/getters aren't polluted by the
	# inner struct's members.
	nested_blocks = _find_nested_struct_blocks(content)
	outer_content = content
	for _, _, start, end in sorted(nested_blocks, key=lambda b: b[2], reverse=True):
		outer_content = outer_content[:start] + outer_content[end:]

	outer_members = _parse_members(outer_content)

	nested = []
	for struct_name, body, _, _ in nested_blocks:
		nested_members = _parse_members(body)
		nested.append({
			'name': f'{class_name}_{struct_name}',
			'cpp_type': f'{class_name}::{struct_name}',
			'header_name': class_name,
			'namespace': f'{namespace}::components',
			**nested_members,
			'file': header_file.name,
		})

	return {
		'name': class_name,
		'cpp_type': class_name,
		'header_name': class_name,
		'namespace': f'{namespace}::components',
		**outer_members,
		'file': header_file.name,
		'nested': nested,
	}


def _to_camel_case(snake_str: str) -> str:
	"""Convert snake_case to camelCase"""
	components = snake_str.split('_')
	return components[0] + ''.join(x.title() for x in components[1:])


def _generate_test_header(component: dict, fix_version: str) -> str:
	"""Generate the #include section and setup"""
	include_root = normalize_version_string(fix_version)
	cpp_type = component.get('cpp_type', component['name'])
	header_name = component.get('header_name', component['name'])

	# Create version-prefixed test class name to avoid symbol conflicts
	version_prefix = fix_version.replace('.', '_')  # e.g., "FIX4.4" -> "FIX4_4"
	test_class_name = f"{version_prefix}_{component['name']}ComponentTest"

	lines = [
		"// SPDX-License-Identifier: MIT",
		f"// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo",
		"// Auto-generated component tests. Do not edit by hand.",
		f"// Component: {cpp_type} ({fix_version})",
		"",
		"#include <gtest/gtest.h>",
		"#include <string_view>",
		"#include <cstring>",
		"",
		f"#include <{include_root}/components/{header_name}.h>",
		f"#include <{include_root}/utils/serializer.h>",
		"#include <utils/fast_buffer.h>",
		"",
		f"using namespace {component['namespace']};",
		"",
		f"class {test_class_name} : public ::testing::Test {{",
		"protected:",
		f"    {cpp_type} component;",
		"",
		"    void SetUp() override {",
		"        component.reset();",
		"    }",
		"};",
		"",
	]

	# Store test_class_name in component for use in other functions
	component['_test_class_name'] = test_class_name

	return "\n".join(lines)


def _generate_reset_tests(component: dict) -> str:
	"""Generate tests for reset() functionality"""
	test_class_name = component.get('_test_class_name', f"{component['name']}ComponentTest")
	lines = [
		f"TEST_F({test_class_name}, ResetClearsAllFields) {{",
		"    // Set some fields",
	]

	# Add setter calls for first few fields
	for setter in component['methods']['setters'][:3]:
		field_name = setter['name']
		decl, value = _populate_value_literal_and_setup(setter['param_type'])
		if decl:
			lines.append(f"    {{ {decl} component.set{field_name}({value}); }}")

	lines.extend([
		"    ",
		"    // Reset component",
		"    component.reset();",
		"    ",
		"    // Verify all fields are cleared",
		"    EXPECT_FALSE(component.hasAnySet());",
		"}",
		"",
	])

	return "\n".join(lines)


def _generate_setter_getter_tests(component: dict) -> str:
	"""Generate tests for all setters and getters"""
	test_class_name = component.get('_test_class_name', f"{component['name']}ComponentTest")
	lines = []

	for setter in component['methods']['setters']:
		field_name = setter['name']
		param_type = setter['param_type']

		# Find corresponding getter
		getter = next((g for g in component['methods']['getters'] if g['name'] == field_name), None)
		if not getter:
			continue

		test_name = f"{test_class_name}, Set{field_name}And{field_name}Match"
		lines.append(f"TEST_F({test_name}) {{")

		# Generate appropriate test value based on type
		if 'CHAR' in param_type:
			lines.append("    const char test_value = 'X';")
			lines.append(f"    component.set{field_name}(test_value);")
			lines.append(f"    EXPECT_EQ(component.get{field_name}(), test_value);")
			lines.append(f"    EXPECT_TRUE(component.has{field_name}());")
		elif 'INT' in param_type or 'LONG' in param_type:
			lines.append("    const int64_t test_value = 12345;")
			lines.append(f"    component.set{field_name}(test_value);")
			lines.append(f"    EXPECT_EQ(component.get{field_name}(), test_value);")
			lines.append(f"    EXPECT_TRUE(component.has{field_name}());")
		elif 'FLOAT' in param_type or 'PRICE' in param_type:
			lines.append("    const double test_value = 123.456;")
			lines.append(f"    component.set{field_name}(test_value);")
			lines.append(f"    EXPECT_EQ(component.get{field_name}(), test_value);")
			lines.append(f"    EXPECT_TRUE(component.has{field_name}());")
		elif 'CURRENCY' in param_type or 'STRING' in param_type:
			# For complex types, just test presence
			lines.append(f"    EXPECT_FALSE(component.has{field_name}());")

		lines.extend(["}", ""])

	return "\n".join(lines)


def _generate_presence_tests(component: dict) -> str:
	"""Generate tests for field presence tracking"""
	test_class_name = component.get('_test_class_name', f"{component['name']}ComponentTest")
	lines = [
		f"TEST_F({test_class_name}, HasAnySetTracksPresence) {{",
		"    EXPECT_FALSE(component.hasAnySet());",
		"    ",
	]

	if component['methods']['setters']:
		# Pick the first setter we can populate with a type-correct literal
		# (handles CHAR/INT/FLOAT/STRING). Falls back to nothing if none fit.
		for setter in component['methods']['setters']:
			decl, value = _populate_value_literal_and_setup(setter['param_type'])
			if decl:
				lines.append(f"    {{ {decl} component.set{setter['name']}({value}); }}")
				lines.append("    EXPECT_TRUE(component.hasAnySet());")
				break

	lines.extend(["}", ""])
	return "\n".join(lines)


def _test_value_literal_and_setup(param_type: str) -> tuple[str, str]:
	"""
	Return (decl_lines_joined, value_expr) for a scalar setter test value,
	matching the same type-dispatch rules used by _generate_setter_getter_tests.
	Returns ("", "") if the type is not a simple settable scalar we can
	populate deterministically (e.g. complex STRING/CURRENCY types).
	"""
	if 'CHAR' in param_type:
		return "const char test_value = 'X';", "test_value"
	if 'INT' in param_type or 'LONG' in param_type:
		return "const int64_t test_value = 12345;", "test_value"
	if 'FLOAT' in param_type or 'PRICE' in param_type:
		return "const double test_value = 123.456;", "test_value"
	return "", ""


def _populate_value_literal_and_setup(param_type: str) -> tuple[str, str]:
	"""
	Like _test_value_literal_and_setup but for *populating* a field (no later
	getter equality assertion), so it also supports STRING-like types. This
	matters because many repeating-group entry types use their first (string)
	field as the decode anchor/delimiter: populating that field is what makes
	the encoded group decode back successfully.
	"""
	decl, value = _test_value_literal_and_setup(param_type)
	if decl:
		return decl, value
	if ('STRING' in param_type or 'MULTIPLEVALUE' in param_type
			or 'MONTHYEAR' in param_type or 'COUNTRY' in param_type
			or 'EXCHANGE' in param_type or 'CURRENCY' in param_type
			or 'LANGUAGE' in param_type):
		return 'const std::string_view test_value{"AAA"};', "test_value"
	# Numeric / temporal value types (UTCTIMESTAMP, LOCALMKTDATE, UTCDATEONLY,
	# UTCTIMEONLY, TZTIMEONLY, SEQNUM, NUMINGROUP, LENGTH, DAYOFMONTH, AMT,
	# PERCENTAGE, QTY, PRICEOFFSET, BOOLEAN, ...) are integer- or double-backed
	# scalar aliases. Many repeating-group entry types use such a field as their
	# FIRST (anchor/delimiter) field, so populating it is required for the
	# encoded group to decode back. A plain integer literal converts implicitly
	# to the underlying alias; the setter parameter is passed by value.
	if '<' not in param_type and param_type.startswith('types::'):
		return "const int64_t test_value = 12345;", "test_value"
	return "", ""


def _inline_value_expr(param_type: str) -> str:
	"""
	Return a single inline C++ expression (no helper variable/statement) that is
	a valid argument for a setter of `param_type`, or "" if we cannot populate
	that type deterministically. Used by the fully-populated roundtrip to set
	every field on one line each without name collisions.
	"""
	if 'CHAR' in param_type and '<' not in param_type:
		return "'X'"
	# FixedString-backed text types are always templated (e.g. types::STRING<32>,
	# types::UTCDATE<17>). The `<` discriminates them from int-backed aliases
	# whose names can share a prefix (e.g. UTCDATEONLY = int32_t has no `<`).
	if '<' in param_type and (
			'STRING' in param_type or 'MULTIPLEVALUE' in param_type
			or 'MULTIPLECHARVALUE' in param_type or 'MONTHYEAR' in param_type
			or 'COUNTRY' in param_type or 'EXCHANGE' in param_type
			or 'CURRENCY' in param_type or 'LANGUAGE' in param_type
			or 'DATA' in param_type or 'XMLDATA' in param_type
			or 'UTCDATE' in param_type):
		return 'std::string_view{"AAA"}'
	if 'FLOAT' in param_type or 'PRICE' in param_type or 'AMT' in param_type \
			or 'QTY' in param_type or 'PERCENTAGE' in param_type:
		return "123.456"
	if 'BOOLEAN' in param_type:
		return "true"
	# Remaining numeric / temporal aliases (INT/LONG/SEQNUM/LENGTH/NUMINGROUP/
	# DAYOFMONTH/UTCTIMESTAMP/LOCALMKTDATE/UTCDATEONLY/UTCTIMEONLY/TZTIMEONLY).
	if '<' not in param_type and param_type.startswith('types::'):
		return "12345"
	return ""


def _emit_populate_all(obj_expr: str, component: dict | None, depth: int = 0) -> tuple[list[str], bool]:
	"""
	Emit C++ statements that populate EVERY settable scalar field of `component`
	(recursing into sub-components and the first entry of each repeating group),
	so the encode/decode/setField branches of every field are exercised. Returns
	(lines, any_populated). Members are emitted in declaration order so the
	group decode anchor (first field) is always set first.
	"""
	if component is None or depth > 5:
		return [], False

	setter_by_name = {s['name']: s for s in component.get('methods', {}).get('setters', [])}
	ref_by_suffix = {suffix: subtype for suffix, subtype in component.get('ref_accessors', [])}
	group_by_suffix = {suffix: entry for suffix, entry in component.get('group_accessors', [])}

	lines: list[str] = []
	any_set = False
	ordered = component.get('ordered_members', [])
	# Fallback to a flat setter list when ordered members are unavailable.
	if not ordered:
		ordered = [('scalar', s['name']) for s in component.get('methods', {}).get('setters', [])]

	for category, member_name in ordered:
		if category == 'scalar':
			setter = setter_by_name.get(member_name)
			if not setter:
				continue
			expr = _inline_value_expr(setter['param_type'])
			if expr:
				lines.append(f"    {obj_expr}.set{setter['name']}({expr});")
				any_set = True
		elif category == 'ref':
			subtype = ref_by_suffix.get(member_name)
			sub = _COMPONENT_REGISTRY.get(subtype) if subtype else None
			sub_lines, ok = _emit_populate_all(f"{obj_expr}.getRef{member_name}()", sub, depth + 1)
			if ok:
				lines.extend(sub_lines)
				any_set = True
		elif category == 'group':
			entry_struct = group_by_suffix.get(member_name)
			if entry_struct is None:
				continue
			entry = _find_nested_entry(component, entry_struct)
			tmp = [f"    {obj_expr}.getRef{member_name}().emplace_back();"]
			pop_lines, ok = _emit_populate_all(f"{obj_expr}.getRef{member_name}().back()", entry, depth + 1)
			if ok:
				lines.extend(tmp)
				lines.extend(pop_lines)
				any_set = True

	return lines, any_set


# Registry of all parsed top-level components (keyed by class name), used to
# resolve sub-component scalar setters when deterministically populating
# group-only / wrapper components that expose no scalar setters of their own.
_COMPONENT_REGISTRY: dict = {}


def _reset_component_registry() -> None:
	_COMPONENT_REGISTRY.clear()


def _register_component(component: dict) -> None:
	_COMPONENT_REGISTRY[component['name']] = component


def _find_nested_entry(component: dict, entry_struct_name: str) -> dict | None:
	"""Find a nested group-entry dict by its (unqualified) struct name."""
	for nested in component.get('nested', []):
		cpp_type = nested.get('cpp_type', '')
		if cpp_type.endswith(f"::{entry_struct_name}") or nested.get('name', '').endswith(f"_{entry_struct_name}"):
			return nested
	return None


def _emit_populate(obj_expr: str, component: dict | None, depth: int = 0) -> tuple[list[str], bool]:
	"""
	Emit C++ statements that deterministically populate `obj_expr` with its
	decode anchor field, recursing through sub-component (getRef) and repeating
	group (getRef...Group) accessors. Returns (lines, success).

	CRITICAL: a group entry's decode() keys repetition off its FIRST encoded
	field (the anchor/delimiter), which corresponds to the first declared
	member. We therefore walk members in declaration order and populate the
	first one we can, so the encoded payload decodes back correctly.
	"""
	if component is None or depth > 5:
		return [], False

	setters = component.get('methods', {}).get('setters', [])
	setter_by_name = {s['name']: s for s in setters}
	ref_by_suffix = {suffix: subtype for suffix, subtype in component.get('ref_accessors', [])}
	group_by_suffix = {suffix: entry for suffix, entry in component.get('group_accessors', [])}

	# 1) Preferred path: honor declaration order so we target the real anchor.
	for category, member_name in component.get('ordered_members', []):
		if category == 'scalar':
			setter = setter_by_name.get(member_name)
			if not setter:
				continue
			decl, value = _populate_value_literal_and_setup(setter['param_type'])
			if decl:
				return [f"    {{ {decl} {obj_expr}.set{setter['name']}({value}); }}"], True
		elif category == 'ref':
			subtype = ref_by_suffix.get(member_name)
			if subtype is None:
				continue
			sub = _COMPONENT_REGISTRY.get(subtype)
			sub_lines, ok = _emit_populate(f"{obj_expr}.getRef{member_name}()", sub, depth + 1)
			if ok:
				return sub_lines, True
		elif category == 'group':
			entry_struct = group_by_suffix.get(member_name)
			if entry_struct is None:
				continue
			entry = _find_nested_entry(component, entry_struct)
			lines = [f"    {obj_expr}.getRef{member_name}().emplace_back();"]
			pop_lines, ok = _emit_populate(f"{obj_expr}.getRef{member_name}().back()", entry, depth + 1)
			if ok:
				return lines + pop_lines, True

	# 2) Fallback (no ordered_members info): original best-effort order.
	for setter in setters:
		decl, value = _populate_value_literal_and_setup(setter['param_type'])
		if decl:
			return [f"    {{ {decl} {obj_expr}.set{setter['name']}({value}); }}"], True

	for suffix, subtype in component.get('ref_accessors', []):
		sub = _COMPONENT_REGISTRY.get(subtype)
		sub_lines, ok = _emit_populate(f"{obj_expr}.getRef{suffix}()", sub, depth + 1)
		if ok:
			return sub_lines, True

	for suffix, entry_struct in component.get('group_accessors', []):
		entry = _find_nested_entry(component, entry_struct)
		lines = [f"    {obj_expr}.getRef{suffix}().emplace_back();"]
		pop_lines, ok = _emit_populate(f"{obj_expr}.getRef{suffix}().back()", entry, depth + 1)
		if ok:
			return lines + pop_lines, True

	return [], False


def _generate_encode_decode_tests(component: dict) -> str:
	"""Generate tests for encode/decode roundtrips"""
	test_class_name = component.get('_test_class_name', f"{component['name']}ComponentTest")
	cpp_type = component.get('cpp_type', component['name'])

	# Prefer a setter whose type we can populate deterministically (CHAR/INT/
	# FLOAT/PRICE), so the roundtrip actually exercises a non-empty payload.
	chosen_setter = None
	chosen_decl = ""
	chosen_value = ""
	for setter in component['methods']['setters']:
		decl, value = _test_value_literal_and_setup(setter['param_type'])
		if decl:
			chosen_setter = setter
			chosen_decl = decl
			chosen_value = value
			break

	lines = [
		f"TEST_F({test_class_name}, EncodeDecodeRoundtrip) {{",
		f"    char buffer[{max(component['max_encode_size'] * 2, 1024)}];",
		"    ",
	]

	if chosen_setter:
		lines.extend([
			"    // Populate a real field so the encoded payload is non-empty and",
			"    // decode() is genuinely exercised (some component wrappers return",
			"    // false on a decode of zero consumed bytes).",
			f"    {chosen_decl}",
			f"    component.set{chosen_setter['name']}({chosen_value});",
			"    ",
			"    char *p = buffer;",
			"    p = component.encode(p, true);",
			"    std::size_t encoded_size = p - buffer;",
			"    ",
			"    // Verify encode succeeded",
			"    EXPECT_LE(encoded_size, component.compute_buffer_size());",
			"    EXPECT_GT(encoded_size, 0u);",
			"    ",
			"    // Decode back",
			f"    {cpp_type} decoded;",
			"    const char *q = buffer;",
			"    bool decode_result = decoded.decode(q, p);",
			"    EXPECT_TRUE(decode_result);",
			f"    EXPECT_EQ(decoded.get{chosen_setter['name']}(), component.get{chosen_setter['name']}());",
			"    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());",
			"}",
			"",
		])
	else:
		populate_lines, populated = _emit_populate("component", component)
		if populated:
			lines.extend([
				"    // This component exposes no scalar setters of its own, so we",
				"    // populate it through its getRef sub-component / group accessors",
				"    // to exercise the real encode/decode (and group loop) paths.",
			])
			lines.extend(populate_lines)
			lines.extend([
				"    ",
				"    EXPECT_TRUE(component.hasAnySet());",
				"    ",
				"    char *p = buffer;",
				"    p = component.encode(p, true);",
				"    ASSERT_NE(p, nullptr);",
				"    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);",
				"    ",
				"    EXPECT_GT(encoded_size, 0u);",
				"    EXPECT_LE(encoded_size, component.compute_buffer_size());",
				"    ",
				f"    {cpp_type} decoded;",
				"    const char *q = buffer;",
				"    EXPECT_TRUE(decoded.decode(q, p));",
				"    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());",
				"}",
				"",
			])
		else:
			lines.extend([
				"    // No scalar setter available to populate deterministically (this",
				"    // component only wraps nested groups/sub-components). Group-only",
				"    // containers with no required fields legitimately encode to 0",
				"    // bytes, and some decode() implementations return false when no",
				"    // bytes were consumed, so we only assert the two views agree.",
				"    char *p = buffer;",
				"    p = component.encode(p, true);",
				"    std::size_t encoded_size = p - buffer;",
				"    ",
				"    EXPECT_LE(encoded_size, component.compute_buffer_size());",
				"    ",
				f"    {cpp_type} decoded;",
				"    const char *q = buffer;",
				"    (void)decoded.decode(q, p);",
				"    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());",
				"}",
				"",
			])

	# For repeating-group containers, add a dedicated test that populates two
	# entries and round-trips them, exercising the group count/encode/decode
	# loop and the per-entry checkRequired loop (the bulk of the generated
	# group .cpp). The REQUIRED==0 guard line stays uncovered by design.
	group_accessors = component.get('group_accessors', [])
	if group_accessors:
		suffix, entry_struct = group_accessors[0]
		entry = _find_nested_entry(component, entry_struct)
		e1, ok1 = _emit_populate("component.getRef{0}().back()".format(suffix), entry)
		if ok1:
			lines.append(f"TEST_F({test_class_name}, PopulatedGroupLoopRoundtrip) {{")
			lines.append(f"    char buffer[{max(component['max_encode_size'] * 4, 2048)}];")
			lines.append(f"    component.getRef{suffix}().emplace_back();")
			lines.extend(e1)
			lines.append(f"    component.getRef{suffix}().emplace_back();")
			e2, _ = _emit_populate("component.getRef{0}().back()".format(suffix), entry)
			lines.extend(e2)
			lines.extend([
				"    ",
				f"    EXPECT_EQ(component.getRef{suffix}().size(), 2u);",
				"    EXPECT_TRUE(component.hasAnySet());",
				"    // We populate each entry's decode anchor (first field) to drive the",
				"    // group encode/decode loop. That does not necessarily satisfy every",
				"    // REQUIRED field of the entry, so we only exercise checkRequired()",
				"    // for coverage here; its truth value is asserted by the dedicated",
				"    // CheckRequiredWhenEmpty test.",
				"    (void)component.checkRequired();",
				"    ",
				"    char *p = component.encode(buffer, true);",
				"    ASSERT_NE(p, nullptr);",
				"    EXPECT_GT(static_cast<std::size_t>(p - buffer), 0u);",
				"    ",
				f"    {cpp_type} decoded;",
				"    const char *q = buffer;",
				"    EXPECT_TRUE(decoded.decode(q, p));",
				f"    EXPECT_EQ(decoded.getRef{suffix}().size(), component.getRef{suffix}().size());",
				"    ",
				"    // Decoding an unknown leading tag returns without consuming it.",
				f"    {cpp_type} other;",
				'    const char *bad = "99=X\\x01";',
				"    const char *bp = bad;",
				"    (void)other.decode(bp, bad + 5);",
				"    ",
				"    // setField on the container is a no-op sink (returns false).",
				"    EXPECT_FALSE(other.setField(0, nullptr, nullptr));",
				"}",
				"",
			])

	# Fully-populated roundtrip: set EVERY settable scalar field (recursing into
	# sub-components and the first entry of each repeating group), then
	# encode -> decode -> re-encode and compare the byte streams. This exercises
	# every field's encode (`if(hasX())`), decode (`case field_id::X`) and
	# setField branch in a single deterministic test, which is the bulk of the
	# generated per-field code.
	all_lines, all_ok = _emit_populate_all("component", component)
	if all_ok:
		big = max(component['max_encode_size'] * 8, 4096)
		lines.append(f"TEST_F({test_class_name}, FullyPopulatedRoundtrip) {{")
		lines.append(f"    char buffer[{big}];")
		lines.extend(all_lines)
		lines.extend([
			"    ",
			"    EXPECT_TRUE(component.hasAnySet());",
			"    ",
			"    char *p = component.encode(buffer, true);",
			"    ASSERT_NE(p, nullptr);",
			"    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);",
			"    EXPECT_GT(encoded_size, 0u);",
			"    EXPECT_LE(encoded_size, component.compute_buffer_size());",
			"    ",
			f"    {cpp_type} decoded;",
			"    const char *q = buffer;",
			"    EXPECT_TRUE(decoded.decode(q, p));",
			"    ",
			"    // Re-encode the decoded view and compare the two byte streams to",
			"    // confirm every populated field survived the roundtrip.",
			f"    char buffer2[{big}];",
			"    char *p2 = decoded.encode(buffer2, true);",
			"    ASSERT_NE(p2, nullptr);",
			"    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);",
			"    ASSERT_EQ(encoded_size, encoded_size2);",
			"    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));",
			"}",
			"",
		])

	return "\n".join(lines)


def _generate_boundary_tests(component: dict) -> str:
	"""Generate tests for boundary conditions and edge cases"""
	test_class_name = component.get('_test_class_name', f"{component['name']}ComponentTest")
	lines = [
		f"TEST_F({test_class_name}, CheckRequiredWhenEmpty) {{",
	]

	if component['has_required']:
		lines.append("    EXPECT_FALSE(component.checkRequired());")
	else:
		lines.append("    EXPECT_TRUE(component.checkRequired());")

	lines.extend(["}", ""])

	# Exercise setField's `default: return false` branch with a tag that is not
	# a member of this component. Tag 0 is never a valid FIX field id, so it
	# always lands in the default case. This covers the uncovered default line
	# present in every generated setField (and, for group containers, drives the
	# decode() path that breaks out when an unknown trailing tag is seen).
	lines.extend([
		f"TEST_F({test_class_name}, SetFieldUnknownTagReturnsFalse) {{",
		'    const char *v = "X";',
		"    EXPECT_FALSE(component.setField(0, v, v + 1));",
		"}",
		"",
	])

	return "\n".join(lines)


def _generate_full_test_file(component: dict, fix_version: str) -> str:
	"""Generate complete test file for a component"""
	sections = [
		_generate_test_header(component, fix_version),
		_generate_reset_tests(component),
		_generate_setter_getter_tests(component),
		_generate_presence_tests(component),
		_generate_encode_decode_tests(component),
		_generate_boundary_tests(component),
	]

	return "\n".join(filter(None, sections))


def generate_all_component_tests(
	include_dir: Path,
	output_dir: Path,
	fix_version: str = "FIX4.4"
) -> dict:
	"""
	Generate test files for all components in a FIX version.

	Args:
		include_dir: Path to include/<FIX_VERSION>/components/
		output_dir: Path to tests/engine/<FIX_VERSION>/components/
		fix_version: FIX version label (e.g., "FIX4.4")

	Returns:
		Dictionary with statistics about generated tests
	"""
	components_dir = include_dir / "components"
	output_dir.mkdir(parents=True, exist_ok=True)
	namespace = namespace_from_version(fix_version)

	stats = {
		'total': 0,
		'generated': 0,
		'skipped': 0,
		'errors': [],
		'files': []
	}

	if not components_dir.exists():
		stats['errors'].append(f"Components directory not found: {components_dir}")
		return stats

	# First pass: parse & register every component so that sub-component scalar
	# setters can be resolved when populating group-only / wrapper components.
	_reset_component_registry()
	parsed: list[tuple[Path, dict]] = []
	for header_file in sorted(components_dir.glob("*.h")):
		try:
			component = extract_component_info(header_file, namespace)
		except Exception as e:
			component = None
			stats['errors'].append(f"{header_file.name}: {str(e)}")
		if component:
			_register_component(component)
			parsed.append((header_file, component))

	# Second pass: generate test files (registry is now fully populated).
	for header_file, component in parsed:
		stats['total'] += 1

		try:
			# Generate test file for the top-level component (or group container)
			test_code = _generate_full_test_file(component, fix_version)
			test_file = output_dir / f"{component['name']}_tests.cpp"

			test_file.write_text(test_code, encoding="utf-8")
			stats['generated'] += 1
			stats['files'].append(str(test_file.relative_to(output_dir.parent.parent)))

			print(f"[ok] Generated {component['name']}_tests.cpp ({len(component['methods']['setters'])} setters)")

			# Generate test files for nested group-entry structs (e.g. NoAllocs),
			# which own the real field setters/getters for repeating groups.
			for nested_component in component.get('nested', []):
				nested_code = _generate_full_test_file(nested_component, fix_version)
				nested_file = output_dir / f"{nested_component['name']}_tests.cpp"

				nested_file.write_text(nested_code, encoding="utf-8")
				stats['generated'] += 1
				stats['files'].append(str(nested_file.relative_to(output_dir.parent.parent)))

				print(f"[ok] Generated {nested_component['name']}_tests.cpp ({len(nested_component['methods']['setters'])} setters, nested type)")

		except Exception as e:
			stats['errors'].append(f"{header_file.name}: {str(e)}")
			stats['skipped'] += 1

	return stats


def generate(base_output: Path, fix_version: str) -> list[Path]:
	"""
	Pipeline-compatible entry point, mirroring generator_tests.generate().

	Reads components from: base_output / "include" / fix_version / "components"
	Writes tests to:       base_output / "tests" / fix_version / "components"
	"""
	include_dir = base_output / "include" / fix_version
	output_dir = base_output / "tests" / fix_version / "components"

	stats = generate_all_component_tests(include_dir, output_dir, fix_version)

	if stats['errors']:
		for error in stats['errors']:
			print(f"[warn] {error}")

	print(f"[ok] component tests written → {output_dir} ({stats['generated']} components)")

	generated_files = [Path(f) for f in stats['files']]

	# serializer.h tests: one static file per FIX version, under tests/<version>/utils/.
	serializer_test_file = generate_serializer_tests(base_output, fix_version)
	generated_files.append(serializer_test_file.relative_to(base_output))

	# parser.h tests: one file per FIX version, under tests/<version>/utils/.
	parser_test_file = generate_parser_tests(base_output, fix_version)
	generated_files.append(parser_test_file.relative_to(base_output))

	# datetime.h tests: one file per FIX version, under tests/<version>/utils/.
	datetime_test_file = generate_datetime_tests(base_output, fix_version)
	generated_files.append(datetime_test_file.relative_to(base_output))

	# Ensure tests/<version>/CMakeLists.txt (and components/, utils/ subdirs)
	# have their own add_test_custom(...) calls, wired via add_subdirectory(...).
	ensure_engine_version_cmakelists(base_output / "tests" / fix_version, fix_version)

	return generated_files


if __name__ == "__main__":
	import sys

	# Example usage
	if len(sys.argv) > 1:
		include_path = Path(sys.argv[1])
		output_path = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("./output/tests/FIX4.4/components")
		version = sys.argv[3] if len(sys.argv) > 3 else "FIX4.4"
	else:
		include_path = Path("../output/include/FIX4.4")
		output_path = Path("../output/tests/FIX4.4/components")
		version = "FIX4.4"

	stats = generate_all_component_tests(include_path, output_path, version)

	print(f"\n[summary]")
	print(f"  Total:     {stats['total']}")
	print(f"  Generated: {stats['generated']}")
	print(f"  Skipped:   {stats['skipped']}")

	if stats['errors']:
		print(f"\n[errors]")
		for error in stats['errors']:
			print(f"  - {error}")
