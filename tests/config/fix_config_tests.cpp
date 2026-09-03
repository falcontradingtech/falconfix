// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <config/fix_config.h>
#include <fstream>
#include <sstream>
#include <filesystem>

class FIXConfigTests : public ::testing::Test
{
protected:
	std::string createTempFile(const std::string &content) {
		static int counter = 0;
		std::string filename = "test_config_" + std::to_string(++counter) + ".ini";
		std::ofstream file(filename);
		file << content;
		file.close();
		return filename;
	}

	void deleteTempFile(const std::string &filename) {
		std::filesystem::remove(filename);
	}
};

// ============================================================================
// Tests for parse() function - COMPREHENSIVE COVERAGE
// ============================================================================

TEST_F(FIXConfigTests, ParseEmpty)
{
	const auto cfg = falconfix::FIXConfig::parse("");
	EXPECT_EQ(cfg.sectionCount(), 0);
}

TEST_F(FIXConfigTests, ParseOnlyNewlines)
{
	const auto cfg = falconfix::FIXConfig::parse("\n\n\n");
	EXPECT_EQ(cfg.sectionCount(), 0);
}

TEST_F(FIXConfigTests, ParseOnlyWhitespace)
{
	const auto cfg = falconfix::FIXConfig::parse("   \n  \n   ");
	EXPECT_EQ(cfg.sectionCount(), 0);
}

TEST_F(FIXConfigTests, ParseSingleDefaultSection)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=value");
	EXPECT_EQ(cfg.sectionCount(), 0);
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.defaults(), "key"));
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "value");
}

TEST_F(FIXConfigTests, ParseSingleSession)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_EQ(cfg.sectionCount(), 1);
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.section(0), "key"));
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(0), "key"), "value");
}

TEST_F(FIXConfigTests, ParseMultipleSessions)
{
	const std::string ini = "[SESSION]\nkey1=value1\n[SESSION]\nkey2=value2";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.sectionCount(), 2);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(0), "key1"), "value1");
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(1), "key2"), "value2");
}

TEST_F(FIXConfigTests, ParseDefaultAndSessions)
{
	const std::string ini = "[DEFAULT]\ndefault_key=default_val\n[SESSION]\nkey1=value1";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.sectionCount(), 1);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "default_key"), "default_val");
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(0), "key1"), "value1");
}

TEST_F(FIXConfigTests, ParseCRLFLineEndings)
{
	const std::string ini = "[SESSION]\r\nkey=value\r\n";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.sectionCount(), 1);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(0), "key"), "value");
}

TEST_F(FIXConfigTests, ParseLFLineEndings)
{
	const std::string ini = "[SESSION]\nkey=value\n";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.sectionCount(), 1);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(0), "key"), "value");
}

TEST_F(FIXConfigTests, ParseIgnoresHashCommentLines)
{
	const std::string ini = "# this is a comment\n[DEFAULT]\nkey=value";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "value");
}

TEST_F(FIXConfigTests, ParseIgnoresSemicolonCommentLines)
{
	const std::string ini = "; this is a comment\n[SESSION]\nkey=value";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.section(0), "key"), "value");
}

TEST_F(FIXConfigTests, ParseTrimsLeadingSpacesFromKey)
{
	const std::string ini = "[DEFAULT]\n   key=value";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.defaults(), "key"));
}

TEST_F(FIXConfigTests, ParseTrimsTrailingSpacesFromValue)
{
	const std::string ini = "[DEFAULT]\nkey=value   ";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "value");
}

TEST_F(FIXConfigTests, ParseTrimsFromSectionHeader)
{
	const std::string ini = "[  DEFAULT  ]\nkey=value";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "value");
}

TEST_F(FIXConfigTests, ParseDefaultSectionCaseInsensitive)
{
	const auto cfg1 = falconfix::FIXConfig::parse("[default]\nkey=val1");
	const auto cfg2 = falconfix::FIXConfig::parse("[DEFAULT]\nkey=val2");
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg1.defaults(), "key"));
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg2.defaults(), "key"));
}

TEST_F(FIXConfigTests, ParseSessionSectionCaseInsensitive)
{
	const auto cfg1 = falconfix::FIXConfig::parse("[session]\nkey=val");
	const auto cfg2 = falconfix::FIXConfig::parse("[SESSION]\nkey=val");
	EXPECT_EQ(cfg1.sectionCount(), 1);
	EXPECT_EQ(cfg2.sectionCount(), 1);
}

TEST_F(FIXConfigTests, ParseThrowsOnUnterminatedSectionHeader)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("[DEFAULT"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseThrowsOnEmptySectionName)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("[]"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseThrowsOnEmptySectionNameWithSpaces)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("[   ]"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseThrowsOnUnsupportedSectionName)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("[UNKNOWN]\nkey=val"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseMultipleKeysInSection)
{
	const std::string ini = "[DEFAULT]\nkey1=val1\nkey2=val2\nkey3=val3";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.defaults().size(), 3);
}

TEST_F(FIXConfigTests, ParseEmptyValue)
{
	const std::string ini = "[DEFAULT]\nkey=";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "");
}

TEST_F(FIXConfigTests, ParseThrowsWhenMissingEquals)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("[DEFAULT]\nkeyvalue"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseThrowsOnEmptyKey)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("[DEFAULT]\n=value"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseValueWithMultipleEqualsigns)
{
	const std::string ini = "[DEFAULT]\nequation=a=b=c";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "equation"), "a=b=c");
}

TEST_F(FIXConfigTests, ParseThrowsKeyValueOutsideSection)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parse("key=value"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseMultipleSectionsWithMultipleKeys)
{
	const std::string ini = 
		"[DEFAULT]\ndefault1=val1\ndefault2=val2\n"
		"[SESSION]\nsession1_1=val1_1\nsession1_2=val1_2\n"
		"[SESSION]\nsession2_1=val2_1\n";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.sectionCount(), 2);
	EXPECT_EQ(cfg.defaults().size(), 2);
}

TEST_F(FIXConfigTests, ParseSpecialCharactersInKey)
{
	const std::string ini = "[DEFAULT]\nSpecial_Key-Name.X=value";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.defaults(), "Special_Key-Name.X"));
}

TEST_F(FIXConfigTests, ParseSpecialCharactersInValue)
{
	const std::string ini = "[DEFAULT]\nkey=!@$%^&*(){}[]<>?";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "!@$%^&*(){}[]<>?");
}

TEST_F(FIXConfigTests, ParseLargeValue)
{
	std::string largeValue(10000, 'x');
	const std::string ini = "[DEFAULT]\nkey=" + largeValue;
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), largeValue);
}

// ============================================================================
// Tests for parseFile() function
// ============================================================================

TEST_F(FIXConfigTests, ParseFileLoadsValidFile)
{
	const std::string content = "[DEFAULT]\nkey=value";
	const std::string filename = createTempFile(content);
	try {
		const auto cfg = falconfix::FIXConfig::parseFile(filename);
		EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "value");
	} catch (...) {
		deleteTempFile(filename);
		throw;
	}
	deleteTempFile(filename);
}

TEST_F(FIXConfigTests, ParseFileWithMultipleSessions)
{
	const std::string content = 
		"[DEFAULT]\nhost=localhost\n"
		"[SESSION]\nport=8080\n"
		"[SESSION]\nport=9090";
	const std::string filename = createTempFile(content);
	try {
		const auto cfg = falconfix::FIXConfig::parseFile(filename);
		EXPECT_EQ(cfg.sectionCount(), 2);
		EXPECT_TRUE(falconfix::FIXConfig::has(cfg.defaults(), "host"));
	} catch (...) {
		deleteTempFile(filename);
		throw;
	}
	deleteTempFile(filename);
}

TEST_F(FIXConfigTests, ParseFileThrowsOnNonexistentFile)
{
	EXPECT_THROW(
		falconfix::FIXConfig::parseFile("nonexistent_file_xyz_12345.ini"),
		falconfix::FIXConfig::ParseError
	);
}

TEST_F(FIXConfigTests, ParseFileEmptyFile)
{
	const std::string filename = createTempFile("");
	try {
		const auto cfg = falconfix::FIXConfig::parseFile(filename);
		EXPECT_EQ(cfg.sectionCount(), 0);
	} catch (...) {
		deleteTempFile(filename);
		throw;
	}
	deleteTempFile(filename);
}

// ============================================================================
// Tests for get() function
// ============================================================================

TEST_F(FIXConfigTests, GetReturnsValueWhenFound)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=value");
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "value");
}

TEST_F(FIXConfigTests, GetThrowsWhenKeyNotFound)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=value");
	EXPECT_THROW(
		falconfix::FIXConfig::get(cfg.defaults(), "nonexistent"),
		std::runtime_error
	);
}

TEST_F(FIXConfigTests, GetEmptyValue)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=");
	EXPECT_EQ(falconfix::FIXConfig::get(cfg.defaults(), "key"), "");
}

// ============================================================================
// Tests for tryGetResolved() function
// ============================================================================

TEST_F(FIXConfigTests, TryGetResolvedFromSession)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	std::string_view out;
	EXPECT_TRUE(cfg.tryGetResolved(0, "key", out));
	EXPECT_EQ(out, "value");
}

TEST_F(FIXConfigTests, TryGetResolvedFallsBackToDefaults)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=default_val\n[SESSION]\nother=val");
	std::string_view out;
	EXPECT_TRUE(cfg.tryGetResolved(0, "key", out));
	EXPECT_EQ(out, "default_val");
}

TEST_F(FIXConfigTests, TryGetResolvedSessionOverridesDefault)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=default_val\n[SESSION]\nkey=session_val");
	std::string_view out;
	EXPECT_TRUE(cfg.tryGetResolved(0, "key", out));
	EXPECT_EQ(out, "session_val");
}

TEST_F(FIXConfigTests, TryGetResolvedReturnsFalseWhenNotFound)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	std::string_view out;
	EXPECT_FALSE(cfg.tryGetResolved(0, "nonexistent", out));
}

TEST_F(FIXConfigTests, TryGetResolvedReturnsFalseForInvalidIndex)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	std::string_view out;
	EXPECT_FALSE(cfg.tryGetResolved(10, "key", out));
}

TEST_F(FIXConfigTests, TryGetResolvedMultipleSessions)
{
	const std::string ini = 
		"[DEFAULT]\ndefault_key=default_val\n"
		"[SESSION]\nkey1=val1\n"
		"[SESSION]\nkey2=val2";
	const auto cfg = falconfix::FIXConfig::parse(ini);

	std::string_view out1, out2, out3;
	EXPECT_TRUE(cfg.tryGetResolved(0, "key1", out1));
	EXPECT_EQ(out1, "val1");
	EXPECT_TRUE(cfg.tryGetResolved(1, "key2", out2));
	EXPECT_EQ(out2, "val2");
	EXPECT_TRUE(cfg.tryGetResolved(0, "default_key", out3));
	EXPECT_EQ(out3, "default_val");
}

// ============================================================================
// Tests for getResolved() function
// ============================================================================

TEST_F(FIXConfigTests, GetResolvedReturnsValueFromSession)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_EQ(cfg.getResolved(0, "key"), "value");
}

TEST_F(FIXConfigTests, GetResolvedFallsBackToDefaultsSection)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=default_val\n[SESSION]\nother=val");
	EXPECT_EQ(cfg.getResolved(0, "key"), "default_val");
}

TEST_F(FIXConfigTests, GetResolvedSessionOverridesDefault)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=default_val\n[SESSION]\nkey=session_val");
	EXPECT_EQ(cfg.getResolved(0, "key"), "session_val");
}

TEST_F(FIXConfigTests, GetResolvedThrowsWhenKeyNotFound)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_THROW(
		cfg.getResolved(0, "nonexistent"),
		std::runtime_error
	);
}

TEST_F(FIXConfigTests, GetResolvedThrowsForInvalidIndex)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_THROW(
		cfg.getResolved(10, "key"),
		std::runtime_error
	);
}

// ============================================================================
// Tests for tryGetAs() templates
// ============================================================================

TEST_F(FIXConfigTests, TryGetAsInt32ValidNumber)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=8080");
	int32_t value = 0;
	EXPECT_TRUE(cfg.tryGetAs<int32_t>(0, "port", value));
	EXPECT_EQ(value, 8080);
}

TEST_F(FIXConfigTests, TryGetAsInt32NegativeNumber)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nvalue=-100");
	int32_t value = 0;
	EXPECT_TRUE(cfg.tryGetAs<int32_t>(0, "value", value));
	EXPECT_EQ(value, -100);
}

TEST_F(FIXConfigTests, TryGetAsInt32InvalidNumber)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nvalue=notanumber");
	int32_t value = 0;
	EXPECT_FALSE(cfg.tryGetAs<int32_t>(0, "value", value));
}

TEST_F(FIXConfigTests, TryGetAsInt32MissingKey)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	int32_t value = 0;
	EXPECT_FALSE(cfg.tryGetAs<int32_t>(0, "nonexistent", value));
}

TEST_F(FIXConfigTests, TryGetAsUint16ValidNumber)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=8080");
	uint16_t value = 0;
	EXPECT_TRUE(cfg.tryGetAs<uint16_t>(0, "port", value));
	EXPECT_EQ(value, 8080);
}

TEST_F(FIXConfigTests, TryGetAsUint16MaxValue)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=65535");
	uint16_t value = 0;
	EXPECT_TRUE(cfg.tryGetAs<uint16_t>(0, "port", value));
	EXPECT_EQ(value, 65535);
}

TEST_F(FIXConfigTests, TryGetAsUint16OverflowValue)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=65536");
	uint16_t value = 0;
	EXPECT_FALSE(cfg.tryGetAs<uint16_t>(0, "port", value));
}

TEST_F(FIXConfigTests, TryGetAsUint16NegativeNumber)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=-1");
	uint16_t value = 0;
	EXPECT_FALSE(cfg.tryGetAs<uint16_t>(0, "port", value));
}

TEST_F(FIXConfigTests, TryGetAsBoolYes)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=Y");
	bool value = false;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_TRUE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolYES)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=YES");
	bool value = false;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_TRUE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolTrue)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=TRUE");
	bool value = false;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_TRUE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolOne)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=1");
	bool value = false;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_TRUE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolNo)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=N");
	bool value = true;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_FALSE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolNO)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=NO");
	bool value = true;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_FALSE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolFalse)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=FALSE");
	bool value = true;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_FALSE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolZero)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=0");
	bool value = true;
	EXPECT_TRUE(cfg.tryGetAs<bool>(0, "flag", value));
	EXPECT_FALSE(value);
}

TEST_F(FIXConfigTests, TryGetAsBoolInvalidValue)
{
	auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=maybe");
	bool value = false;
	EXPECT_FALSE(cfg.tryGetAs<bool>(0, "flag", value));
}

TEST_F(FIXConfigTests, TryGetAsString)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	std::string value;
	EXPECT_TRUE(cfg.tryGetAs<std::string>(0, "key", value));
	EXPECT_EQ(value, "value");
}

TEST_F(FIXConfigTests, TryGetAsStringEmpty)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=");
	std::string value = "default";
	EXPECT_TRUE(cfg.tryGetAs<std::string>(0, "key", value));
	EXPECT_EQ(value, "");
}

TEST_F(FIXConfigTests, TryGetAsStringMissingKey)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	std::string value;
	EXPECT_FALSE(cfg.tryGetAs<std::string>(0, "nonexistent", value));
}

// ============================================================================
// Tests for getAs() function
// ============================================================================

TEST_F(FIXConfigTests, GetAsInt32ValidValue)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=8080");
	EXPECT_EQ(cfg.getAs<int32_t>(0, "port"), 8080);
}

TEST_F(FIXConfigTests, GetAsInt32InvalidValue)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nvalue=notanumber");
	EXPECT_THROW(
		cfg.getAs<int32_t>(0, "value"),
		std::runtime_error
	);
}

TEST_F(FIXConfigTests, GetAsBool)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nflag=Y");
	EXPECT_TRUE(cfg.getAs<bool>(0, "flag"));
}

TEST_F(FIXConfigTests, GetAsString)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=hello");
	EXPECT_EQ(cfg.getAs<std::string>(0, "key"), "hello");
}

// ============================================================================
// Tests for getAsOr() function
// ============================================================================

TEST_F(FIXConfigTests, GetAsOrReturnsValueWhenFound)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=8080");
	EXPECT_EQ(cfg.getAsOr<int32_t>(0, "port", 9090), 8080);
}

TEST_F(FIXConfigTests, GetAsOrReturnsFallbackWhenNotFound)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_EQ(cfg.getAsOr<int32_t>(0, "nonexistent", 9090), 9090);
}

TEST_F(FIXConfigTests, GetAsOrReturnsFallbackWhenInvalid)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nport=notanumber");
	EXPECT_EQ(cfg.getAsOr<int32_t>(0, "port", 9090), 9090);
}

TEST_F(FIXConfigTests, GetAsOrBool)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_FALSE(cfg.getAsOr<bool>(0, "nonexistent", false));
	EXPECT_TRUE(cfg.getAsOr<bool>(0, "nonexistent", true));
}

TEST_F(FIXConfigTests, GetAsOrString)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=hello");
	EXPECT_EQ(cfg.getAsOr<std::string>(0, "key", "world"), "hello");
	EXPECT_EQ(cfg.getAsOr<std::string>(0, "nonexistent", "world"), "world");
}

// ============================================================================
// Tests for has() and section access functions
// ============================================================================

TEST_F(FIXConfigTests, HasReturnsTrueWhenKeyExists)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=value");
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.defaults(), "key"));
}

TEST_F(FIXConfigTests, HasReturnsFalseWhenKeyNotExists)
{
	const auto cfg = falconfix::FIXConfig::parse("[DEFAULT]\nkey=value");
	EXPECT_FALSE(falconfix::FIXConfig::has(cfg.defaults(), "nonexistent"));
}

TEST_F(FIXConfigTests, SectionReturnsCorrectSection)
{
	const std::string ini = "[SESSION]\nkey1=value1\n[SESSION]\nkey2=value2";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.section(0), "key1"));
	EXPECT_TRUE(falconfix::FIXConfig::has(cfg.section(1), "key2"));
}

TEST_F(FIXConfigTests, SectionThrowsOnInvalidIndex)
{
	const auto cfg = falconfix::FIXConfig::parse("[SESSION]\nkey=value");
	EXPECT_THROW(
		cfg.section(10),
		std::out_of_range
	);
}

TEST_F(FIXConfigTests, SectionCountReturnsZeroForEmpty)
{
	const auto cfg = falconfix::FIXConfig::parse("");
	EXPECT_EQ(cfg.sectionCount(), 0);
}

TEST_F(FIXConfigTests, SectionCountReturnsSessions)
{
	const std::string ini = "[SESSION]\nkey=val\n[SESSION]\nkey=val";
	const auto cfg = falconfix::FIXConfig::parse(ini);
	EXPECT_EQ(cfg.sectionCount(), 2);
}
