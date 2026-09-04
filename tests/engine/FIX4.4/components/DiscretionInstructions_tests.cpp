// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: DiscretionInstructions (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/DiscretionInstructions.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class DiscretionInstructionsComponentTest : public ::testing::Test {
protected:
    DiscretionInstructions component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(DiscretionInstructionsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setDiscretionInst('A');
    component.setDiscretionOffsetValue(3.14f);
    component.setDiscretionMoveType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionInstAndDiscretionInstMatch) {
    const char test_value = 'X';
    component.setDiscretionInst(test_value);
    EXPECT_EQ(component.getDiscretionInst(), test_value);
    EXPECT_TRUE(component.hasDiscretionInst());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionOffsetValueAndDiscretionOffsetValueMatch) {
    const double test_value = 123.456;
    component.setDiscretionOffsetValue(test_value);
    EXPECT_EQ(component.getDiscretionOffsetValue(), test_value);
    EXPECT_TRUE(component.hasDiscretionOffsetValue());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionMoveTypeAndDiscretionMoveTypeMatch) {
    const int64_t test_value = 12345;
    component.setDiscretionMoveType(test_value);
    EXPECT_EQ(component.getDiscretionMoveType(), test_value);
    EXPECT_TRUE(component.hasDiscretionMoveType());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionOffsetTypeAndDiscretionOffsetTypeMatch) {
    const int64_t test_value = 12345;
    component.setDiscretionOffsetType(test_value);
    EXPECT_EQ(component.getDiscretionOffsetType(), test_value);
    EXPECT_TRUE(component.hasDiscretionOffsetType());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionLimitTypeAndDiscretionLimitTypeMatch) {
    const int64_t test_value = 12345;
    component.setDiscretionLimitType(test_value);
    EXPECT_EQ(component.getDiscretionLimitType(), test_value);
    EXPECT_TRUE(component.hasDiscretionLimitType());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionRoundDirectionAndDiscretionRoundDirectionMatch) {
    const int64_t test_value = 12345;
    component.setDiscretionRoundDirection(test_value);
    EXPECT_EQ(component.getDiscretionRoundDirection(), test_value);
    EXPECT_TRUE(component.hasDiscretionRoundDirection());
}

TEST_F(DiscretionInstructionsComponentTest, SetDiscretionScopeAndDiscretionScopeMatch) {
    const int64_t test_value = 12345;
    component.setDiscretionScope(test_value);
    EXPECT_EQ(component.getDiscretionScope(), test_value);
    EXPECT_TRUE(component.hasDiscretionScope());
}

TEST_F(DiscretionInstructionsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setDiscretionInst('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(DiscretionInstructionsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[320];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setDiscretionInst(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    DiscretionInstructions decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getDiscretionInst(), component.getDiscretionInst());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(DiscretionInstructionsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
