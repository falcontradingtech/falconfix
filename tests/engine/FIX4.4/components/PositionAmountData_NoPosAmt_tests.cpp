// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PositionAmountData::NoPosAmt (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PositionAmountData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PositionAmountData_NoPosAmtComponentTest : public ::testing::Test {
protected:
    PositionAmountData::NoPosAmt component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PositionAmountData_NoPosAmtComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPosAmt(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PositionAmountData_NoPosAmtComponentTest, SetPosAmtAndPosAmtMatch) {
    const double test_value = 123.456;
    component.setPosAmt(test_value);
    EXPECT_EQ(component.getPosAmt(), test_value);
    EXPECT_TRUE(component.hasPosAmt());
}

TEST_F(PositionAmountData_NoPosAmtComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPosAmt(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PositionAmountData_NoPosAmtComponentTest, EncodeDecodeRoundtrip) {
    char buffer[132];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setPosAmt(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    PositionAmountData::NoPosAmt decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getPosAmt(), component.getPosAmt());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(PositionAmountData_NoPosAmtComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
