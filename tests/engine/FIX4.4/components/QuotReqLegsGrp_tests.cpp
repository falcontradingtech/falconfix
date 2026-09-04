// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotReqLegsGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotReqLegsGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class QuotReqLegsGrpComponentTest : public ::testing::Test {
protected:
    QuotReqLegsGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(QuotReqLegsGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegQty(3.14f);
    component.setLegSwapType(42);
    component.setLegSettlType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(QuotReqLegsGrpComponentTest, SetLegQtyAndLegQtyMatch) {
    const double test_value = 123.456;
    component.setLegQty(test_value);
    EXPECT_EQ(component.getLegQty(), test_value);
    EXPECT_TRUE(component.hasLegQty());
}

TEST_F(QuotReqLegsGrpComponentTest, SetLegSwapTypeAndLegSwapTypeMatch) {
    const int64_t test_value = 12345;
    component.setLegSwapType(test_value);
    EXPECT_EQ(component.getLegSwapType(), test_value);
    EXPECT_TRUE(component.hasLegSwapType());
}

TEST_F(QuotReqLegsGrpComponentTest, SetLegSettlTypeAndLegSettlTypeMatch) {
    const char test_value = 'X';
    component.setLegSettlType(test_value);
    EXPECT_EQ(component.getLegSettlType(), test_value);
    EXPECT_TRUE(component.hasLegSettlType());
}

TEST_F(QuotReqLegsGrpComponentTest, SetLegSettlDateAndLegSettlDateMatch) {
}

TEST_F(QuotReqLegsGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(QuotReqLegsGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[146];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    QuotReqLegsGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(QuotReqLegsGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
