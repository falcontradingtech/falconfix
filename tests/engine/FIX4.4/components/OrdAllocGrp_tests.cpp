// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: OrdAllocGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/OrdAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class OrdAllocGrpComponentTest : public ::testing::Test {
protected:
    OrdAllocGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(OrdAllocGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setOrderQty(3.14f);
    component.setOrderAvgPx(3.14f);
    component.setOrderBookingQty(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(OrdAllocGrpComponentTest, SetOrderQtyAndOrderQtyMatch) {
    const double test_value = 123.456;
    component.setOrderQty(test_value);
    EXPECT_EQ(component.getOrderQty(), test_value);
    EXPECT_TRUE(component.hasOrderQty());
}

TEST_F(OrdAllocGrpComponentTest, SetOrderAvgPxAndOrderAvgPxMatch) {
    const double test_value = 123.456;
    component.setOrderAvgPx(test_value);
    EXPECT_EQ(component.getOrderAvgPx(), test_value);
    EXPECT_TRUE(component.hasOrderAvgPx());
}

TEST_F(OrdAllocGrpComponentTest, SetOrderBookingQtyAndOrderBookingQtyMatch) {
    const double test_value = 123.456;
    component.setOrderBookingQty(test_value);
    EXPECT_EQ(component.getOrderBookingQty(), test_value);
    EXPECT_TRUE(component.hasOrderBookingQty());
}

TEST_F(OrdAllocGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setOrderQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(OrdAllocGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[536];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    OrdAllocGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(OrdAllocGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
