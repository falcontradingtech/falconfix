// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegQuotGrp::NoLegs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegQuotGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class LegQuotGrp_NoLegsComponentTest : public ::testing::Test {
protected:
    LegQuotGrp::NoLegs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(LegQuotGrp_NoLegsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegQty(3.14f);
    component.setLegSwapType(42);
    component.setLegSettlType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegQtyAndLegQtyMatch) {
    const double test_value = 123.456;
    component.setLegQty(test_value);
    EXPECT_EQ(component.getLegQty(), test_value);
    EXPECT_TRUE(component.hasLegQty());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegSwapTypeAndLegSwapTypeMatch) {
    const int64_t test_value = 12345;
    component.setLegSwapType(test_value);
    EXPECT_EQ(component.getLegSwapType(), test_value);
    EXPECT_TRUE(component.hasLegSwapType());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegSettlTypeAndLegSettlTypeMatch) {
    const char test_value = 'X';
    component.setLegSettlType(test_value);
    EXPECT_EQ(component.getLegSettlType(), test_value);
    EXPECT_TRUE(component.hasLegSettlType());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegSettlDateAndLegSettlDateMatch) {
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegPriceTypeAndLegPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setLegPriceType(test_value);
    EXPECT_EQ(component.getLegPriceType(), test_value);
    EXPECT_TRUE(component.hasLegPriceType());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegBidPxAndLegBidPxMatch) {
    const double test_value = 123.456;
    component.setLegBidPx(test_value);
    EXPECT_EQ(component.getLegBidPx(), test_value);
    EXPECT_TRUE(component.hasLegBidPx());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, SetLegOfferPxAndLegOfferPxMatch) {
    const double test_value = 123.456;
    component.setLegOfferPx(test_value);
    EXPECT_EQ(component.getLegOfferPx(), test_value);
    EXPECT_TRUE(component.hasLegOfferPx());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(LegQuotGrp_NoLegsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[312];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    LegQuotGrp::NoLegs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(LegQuotGrp_NoLegsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
