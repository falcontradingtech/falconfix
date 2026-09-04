// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: CpctyConfGrp::NoCapacities (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/CpctyConfGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class CpctyConfGrp_NoCapacitiesComponentTest : public ::testing::Test {
protected:
    CpctyConfGrp::NoCapacities component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(CpctyConfGrp_NoCapacitiesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setOrderCapacity('A');
    component.setOrderCapacityQty(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(CpctyConfGrp_NoCapacitiesComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(CpctyConfGrp_NoCapacitiesComponentTest, SetOrderCapacityQtyAndOrderCapacityQtyMatch) {
    const double test_value = 123.456;
    component.setOrderCapacityQty(test_value);
    EXPECT_EQ(component.getOrderCapacityQty(), test_value);
    EXPECT_TRUE(component.hasOrderCapacityQty());
}

TEST_F(CpctyConfGrp_NoCapacitiesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setOrderCapacity('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(CpctyConfGrp_NoCapacitiesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[144];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    CpctyConfGrp::NoCapacities decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(CpctyConfGrp_NoCapacitiesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
