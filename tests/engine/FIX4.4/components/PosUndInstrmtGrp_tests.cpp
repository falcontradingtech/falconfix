// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PosUndInstrmtGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PosUndInstrmtGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PosUndInstrmtGrpComponentTest : public ::testing::Test {
protected:
    PosUndInstrmtGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PosUndInstrmtGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setUnderlyingSettlPrice(3.14f);
    component.setUnderlyingSettlPriceType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PosUndInstrmtGrpComponentTest, SetUnderlyingSettlPriceAndUnderlyingSettlPriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingSettlPrice(test_value);
    EXPECT_EQ(component.getUnderlyingSettlPrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingSettlPrice());
}

TEST_F(PosUndInstrmtGrpComponentTest, SetUnderlyingSettlPriceTypeAndUnderlyingSettlPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingSettlPriceType(test_value);
    EXPECT_EQ(component.getUnderlyingSettlPriceType(), test_value);
    EXPECT_TRUE(component.hasUnderlyingSettlPriceType());
}

TEST_F(PosUndInstrmtGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setUnderlyingSettlPrice(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PosUndInstrmtGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[108];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    PosUndInstrmtGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(PosUndInstrmtGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
