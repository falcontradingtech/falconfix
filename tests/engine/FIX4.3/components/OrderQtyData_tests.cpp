// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: OrderQtyData (FIX4.3)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.3/components/OrderQtyData.h>
#include <FIX4.3/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix43::components;

class FIX4_3_OrderQtyDataComponentTest : public ::testing::Test {
protected:
    OrderQtyData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_3_OrderQtyDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const double test_value = 123.456; component.setOrderQty(test_value); }
    { const double test_value = 123.456; component.setCashOrderQty(test_value); }
    { const double test_value = 123.456; component.setOrderPercent(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, SetOrderQtyAndOrderQtyMatch) {
    const double test_value = 123.456;
    component.setOrderQty(test_value);
    EXPECT_EQ(component.getOrderQty(), test_value);
    EXPECT_TRUE(component.hasOrderQty());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, SetCashOrderQtyAndCashOrderQtyMatch) {
    const double test_value = 123.456;
    component.setCashOrderQty(test_value);
    EXPECT_EQ(component.getCashOrderQty(), test_value);
    EXPECT_TRUE(component.hasCashOrderQty());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, SetOrderPercentAndOrderPercentMatch) {
    const double test_value = 123.456;
    component.setOrderPercent(test_value);
    EXPECT_EQ(component.getOrderPercent(), test_value);
    EXPECT_TRUE(component.hasOrderPercent());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, SetRoundingDirectionAndRoundingDirectionMatch) {
    const char test_value = 'X';
    component.setRoundingDirection(test_value);
    EXPECT_EQ(component.getRoundingDirection(), test_value);
    EXPECT_TRUE(component.hasRoundingDirection());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, SetRoundingModulusAndRoundingModulusMatch) {
    const double test_value = 123.456;
    component.setRoundingModulus(test_value);
    EXPECT_EQ(component.getRoundingModulus(), test_value);
    EXPECT_TRUE(component.hasRoundingModulus());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const double test_value = 123.456; component.setOrderQty(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
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

TEST_F(FIX4_3_OrderQtyDataComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setOrderQty(123.456);
    component.setCashOrderQty(123.456);
    component.setOrderPercent(123.456);
    component.setRoundingDirection('X');
    component.setRoundingModulus(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    OrderQtyData decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4096];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_3_OrderQtyDataComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
