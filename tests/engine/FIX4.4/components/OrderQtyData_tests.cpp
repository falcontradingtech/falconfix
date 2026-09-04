// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: OrderQtyData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/OrderQtyData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_OrderQtyDataComponentTest : public ::testing::Test {
protected:
    OrderQtyData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_OrderQtyDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setOrderQty(3.14f);
    component.setCashOrderQty(3.14f);
    component.setOrderPercent(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, SetOrderQtyAndOrderQtyMatch) {
    const double test_value = 123.456;
    component.setOrderQty(test_value);
    EXPECT_EQ(component.getOrderQty(), test_value);
    EXPECT_TRUE(component.hasOrderQty());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, SetCashOrderQtyAndCashOrderQtyMatch) {
    const double test_value = 123.456;
    component.setCashOrderQty(test_value);
    EXPECT_EQ(component.getCashOrderQty(), test_value);
    EXPECT_TRUE(component.hasCashOrderQty());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, SetOrderPercentAndOrderPercentMatch) {
    const double test_value = 123.456;
    component.setOrderPercent(test_value);
    EXPECT_EQ(component.getOrderPercent(), test_value);
    EXPECT_TRUE(component.hasOrderPercent());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, SetRoundingDirectionAndRoundingDirectionMatch) {
    const char test_value = 'X';
    component.setRoundingDirection(test_value);
    EXPECT_EQ(component.getRoundingDirection(), test_value);
    EXPECT_TRUE(component.hasRoundingDirection());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, SetRoundingModulusAndRoundingModulusMatch) {
    const double test_value = 123.456;
    component.setRoundingModulus(test_value);
    EXPECT_EQ(component.getRoundingModulus(), test_value);
    EXPECT_TRUE(component.hasRoundingModulus());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setOrderQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[242];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setOrderQty(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    OrderQtyData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getOrderQty(), component.getOrderQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_OrderQtyDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
