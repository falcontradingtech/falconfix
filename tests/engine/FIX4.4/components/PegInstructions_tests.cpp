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

class PegInstructionsComponentTest : public ::testing::Test {
protected:
    PegInstructions component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PegInstructionsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPegOffsetValue(3.14f);
    component.setPegMoveType(42);
    component.setPegOffsetType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PegInstructionsComponentTest, SetPegOffsetValueAndPegOffsetValueMatch) {
    const double test_value = 123.456;
    component.setPegOffsetValue(test_value);
    EXPECT_EQ(component.getPegOffsetValue(), test_value);
    EXPECT_TRUE(component.hasPegOffsetValue());
}

TEST_F(PegInstructionsComponentTest, SetPegMoveTypeAndPegMoveTypeMatch) {
    const int64_t test_value = 12345;
    component.setPegMoveType(test_value);
    EXPECT_EQ(component.getPegMoveType(), test_value);
    EXPECT_TRUE(component.hasPegMoveType());
}

TEST_F(PegInstructionsComponentTest, SetPegOffsetTypeAndPegOffsetTypeMatch) {
    const int64_t test_value = 12345;
    component.setPegOffsetType(test_value);
    EXPECT_EQ(component.getPegOffsetType(), test_value);
    EXPECT_TRUE(component.hasPegOffsetType());
}

TEST_F(PegInstructionsComponentTest, SetPegLimitTypeAndPegLimitTypeMatch) {
    const int64_t test_value = 12345;
    component.setPegLimitType(test_value);
    EXPECT_EQ(component.getPegLimitType(), test_value);
    EXPECT_TRUE(component.hasPegLimitType());
}

TEST_F(PegInstructionsComponentTest, SetPegRoundDirectionAndPegRoundDirectionMatch) {
    const int64_t test_value = 12345;
    component.setPegRoundDirection(test_value);
    EXPECT_EQ(component.getPegRoundDirection(), test_value);
    EXPECT_TRUE(component.hasPegRoundDirection());
}

TEST_F(PegInstructionsComponentTest, SetPegScopeAndPegScopeMatch) {
    const int64_t test_value = 12345;
    component.setPegScope(test_value);
    EXPECT_EQ(component.getPegScope(), test_value);
    EXPECT_TRUE(component.hasPegScope());
}

TEST_F(PegInstructionsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPegOffsetValue(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PegInstructionsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[308];
    
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

TEST_F(PegInstructionsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
