// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: YieldData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/YieldData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class YieldDataComponentTest : public ::testing::Test {
protected:
    YieldData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(YieldDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setYield(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(YieldDataComponentTest, SetYieldAndYieldMatch) {
    const double test_value = 123.456;
    component.setYield(test_value);
    EXPECT_EQ(component.getYield(), test_value);
    EXPECT_TRUE(component.hasYield());
}

TEST_F(YieldDataComponentTest, SetYieldCalcDateAndYieldCalcDateMatch) {
}

TEST_F(YieldDataComponentTest, SetYieldRedemptionDateAndYieldRedemptionDateMatch) {
}

TEST_F(YieldDataComponentTest, SetYieldRedemptionPriceAndYieldRedemptionPriceMatch) {
    const double test_value = 123.456;
    component.setYieldRedemptionPrice(test_value);
    EXPECT_EQ(component.getYieldRedemptionPrice(), test_value);
    EXPECT_TRUE(component.hasYieldRedemptionPrice());
}

TEST_F(YieldDataComponentTest, SetYieldRedemptionPriceTypeAndYieldRedemptionPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setYieldRedemptionPriceType(test_value);
    EXPECT_EQ(component.getYieldRedemptionPriceType(), test_value);
    EXPECT_TRUE(component.hasYieldRedemptionPriceType());
}

TEST_F(YieldDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setYield(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(YieldDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[292];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setYield(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    YieldData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getYield(), component.getYield());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(YieldDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
