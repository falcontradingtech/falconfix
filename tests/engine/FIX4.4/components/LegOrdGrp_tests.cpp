// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegOrdGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegOrdGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class LegOrdGrpComponentTest : public ::testing::Test {
protected:
    LegOrdGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(LegOrdGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegQty(3.14f);
    component.setLegSwapType(42);
    component.setLegPositionEffect('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(LegOrdGrpComponentTest, SetLegQtyAndLegQtyMatch) {
    const double test_value = 123.456;
    component.setLegQty(test_value);
    EXPECT_EQ(component.getLegQty(), test_value);
    EXPECT_TRUE(component.hasLegQty());
}

TEST_F(LegOrdGrpComponentTest, SetLegSwapTypeAndLegSwapTypeMatch) {
    const int64_t test_value = 12345;
    component.setLegSwapType(test_value);
    EXPECT_EQ(component.getLegSwapType(), test_value);
    EXPECT_TRUE(component.hasLegSwapType());
}

TEST_F(LegOrdGrpComponentTest, SetLegPositionEffectAndLegPositionEffectMatch) {
    const char test_value = 'X';
    component.setLegPositionEffect(test_value);
    EXPECT_EQ(component.getLegPositionEffect(), test_value);
    EXPECT_TRUE(component.hasLegPositionEffect());
}

TEST_F(LegOrdGrpComponentTest, SetLegCoveredOrUncoveredAndLegCoveredOrUncoveredMatch) {
    const int64_t test_value = 12345;
    component.setLegCoveredOrUncovered(test_value);
    EXPECT_EQ(component.getLegCoveredOrUncovered(), test_value);
    EXPECT_TRUE(component.hasLegCoveredOrUncovered());
}

TEST_F(LegOrdGrpComponentTest, SetLegPriceAndLegPriceMatch) {
    const double test_value = 123.456;
    component.setLegPrice(test_value);
    EXPECT_EQ(component.getLegPrice(), test_value);
    EXPECT_TRUE(component.hasLegPrice());
}

TEST_F(LegOrdGrpComponentTest, SetLegSettlTypeAndLegSettlTypeMatch) {
    const char test_value = 'X';
    component.setLegSettlType(test_value);
    EXPECT_EQ(component.getLegSettlType(), test_value);
    EXPECT_TRUE(component.hasLegSettlType());
}

TEST_F(LegOrdGrpComponentTest, SetLegSettlDateAndLegSettlDateMatch) {
}

TEST_F(LegOrdGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(LegOrdGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[340];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    LegOrdGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(LegOrdGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
