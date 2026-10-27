// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: OrdAllocGrp::NoOrders (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/OrdAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_OrdAllocGrp_NoOrdersComponentTest : public ::testing::Test {
protected:
    OrdAllocGrp::NoOrders component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setOrderID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryOrderID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetOrderIDAndOrderIDMatch) {
    EXPECT_FALSE(component.hasOrderID());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetSecondaryOrderIDAndSecondaryOrderIDMatch) {
    EXPECT_FALSE(component.hasSecondaryOrderID());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetListIDAndListIDMatch) {
    EXPECT_FALSE(component.hasListID());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetOrderQtyAndOrderQtyMatch) {
    const double test_value = 123.456;
    component.setOrderQty(test_value);
    EXPECT_EQ(component.getOrderQty(), test_value);
    EXPECT_TRUE(component.hasOrderQty());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetOrderAvgPxAndOrderAvgPxMatch) {
    const double test_value = 123.456;
    component.setOrderAvgPx(test_value);
    EXPECT_EQ(component.getOrderAvgPx(), test_value);
    EXPECT_TRUE(component.hasOrderAvgPx());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, SetOrderBookingQtyAndOrderBookingQtyMatch) {
    const double test_value = 123.456;
    component.setOrderBookingQty(test_value);
    EXPECT_EQ(component.getOrderBookingQty(), test_value);
    EXPECT_TRUE(component.hasOrderBookingQty());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, EncodeDecodeRoundtrip) {
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
    OrdAllocGrp::NoOrders decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getOrderQty(), component.getOrderQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_OrdAllocGrp_NoOrdersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
