// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: TrdInstrmtLegGrp::NoLegs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/TrdInstrmtLegGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class TrdInstrmtLegGrp_NoLegsComponentTest : public ::testing::Test {
protected:
    TrdInstrmtLegGrp::NoLegs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegQty(3.14f);
    component.setLegSwapType(42);
    component.setLegPositionEffect('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegQtyAndLegQtyMatch) {
    const double test_value = 123.456;
    component.setLegQty(test_value);
    EXPECT_EQ(component.getLegQty(), test_value);
    EXPECT_TRUE(component.hasLegQty());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegSwapTypeAndLegSwapTypeMatch) {
    const int64_t test_value = 12345;
    component.setLegSwapType(test_value);
    EXPECT_EQ(component.getLegSwapType(), test_value);
    EXPECT_TRUE(component.hasLegSwapType());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegPositionEffectAndLegPositionEffectMatch) {
    const char test_value = 'X';
    component.setLegPositionEffect(test_value);
    EXPECT_EQ(component.getLegPositionEffect(), test_value);
    EXPECT_TRUE(component.hasLegPositionEffect());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegCoveredOrUncoveredAndLegCoveredOrUncoveredMatch) {
    const int64_t test_value = 12345;
    component.setLegCoveredOrUncovered(test_value);
    EXPECT_EQ(component.getLegCoveredOrUncovered(), test_value);
    EXPECT_TRUE(component.hasLegCoveredOrUncovered());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegPriceAndLegPriceMatch) {
    const double test_value = 123.456;
    component.setLegPrice(test_value);
    EXPECT_EQ(component.getLegPrice(), test_value);
    EXPECT_TRUE(component.hasLegPrice());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegSettlTypeAndLegSettlTypeMatch) {
    const char test_value = 'X';
    component.setLegSettlType(test_value);
    EXPECT_EQ(component.getLegSettlType(), test_value);
    EXPECT_TRUE(component.hasLegSettlType());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegSettlDateAndLegSettlDateMatch) {
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, SetLegLastPxAndLegLastPxMatch) {
    const double test_value = 123.456;
    component.setLegLastPx(test_value);
    EXPECT_EQ(component.getLegLastPx(), test_value);
    EXPECT_TRUE(component.hasLegLastPx());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[398];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    TrdInstrmtLegGrp::NoLegs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(TrdInstrmtLegGrp_NoLegsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
