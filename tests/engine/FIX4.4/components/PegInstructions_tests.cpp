// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PegInstructions (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PegInstructions.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_PegInstructionsComponentTest : public ::testing::Test {
protected:
    PegInstructions component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_PegInstructionsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const double test_value = 123.456; component.setPegOffsetValue(test_value); }
    { const int64_t test_value = 12345; component.setPegMoveType(test_value); }
    { const int64_t test_value = 12345; component.setPegOffsetType(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetPegOffsetValueAndPegOffsetValueMatch) {
    const double test_value = 123.456;
    component.setPegOffsetValue(test_value);
    EXPECT_EQ(component.getPegOffsetValue(), test_value);
    EXPECT_TRUE(component.hasPegOffsetValue());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetPegMoveTypeAndPegMoveTypeMatch) {
    const int64_t test_value = 12345;
    component.setPegMoveType(test_value);
    EXPECT_EQ(component.getPegMoveType(), test_value);
    EXPECT_TRUE(component.hasPegMoveType());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetPegOffsetTypeAndPegOffsetTypeMatch) {
    const int64_t test_value = 12345;
    component.setPegOffsetType(test_value);
    EXPECT_EQ(component.getPegOffsetType(), test_value);
    EXPECT_TRUE(component.hasPegOffsetType());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetPegLimitTypeAndPegLimitTypeMatch) {
    const int64_t test_value = 12345;
    component.setPegLimitType(test_value);
    EXPECT_EQ(component.getPegLimitType(), test_value);
    EXPECT_TRUE(component.hasPegLimitType());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetPegRoundDirectionAndPegRoundDirectionMatch) {
    const int64_t test_value = 12345;
    component.setPegRoundDirection(test_value);
    EXPECT_EQ(component.getPegRoundDirection(), test_value);
    EXPECT_TRUE(component.hasPegRoundDirection());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetPegScopeAndPegScopeMatch) {
    const int64_t test_value = 12345;
    component.setPegScope(test_value);
    EXPECT_EQ(component.getPegScope(), test_value);
    EXPECT_TRUE(component.hasPegScope());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const double test_value = 123.456; component.setPegOffsetValue(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setPegOffsetValue(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    PegInstructions decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getPegOffsetValue(), component.getPegOffsetValue());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setPegOffsetValue(123.456);
    component.setPegMoveType(12345);
    component.setPegOffsetType(12345);
    component.setPegLimitType(12345);
    component.setPegRoundDirection(12345);
    component.setPegScope(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    PegInstructions decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4096];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_PegInstructionsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_PegInstructionsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
