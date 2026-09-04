// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegPreAllocGrp::NoLegAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegPreAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class LegPreAllocGrp_NoLegAllocsComponentTest : public ::testing::Test {
protected:
    LegPreAllocGrp::NoLegAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(LegPreAllocGrp_NoLegAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegAllocQty(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(LegPreAllocGrp_NoLegAllocsComponentTest, SetLegAllocQtyAndLegAllocQtyMatch) {
    const double test_value = 123.456;
    component.setLegAllocQty(test_value);
    EXPECT_EQ(component.getLegAllocQty(), test_value);
    EXPECT_TRUE(component.hasLegAllocQty());
}

TEST_F(LegPreAllocGrp_NoLegAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegAllocQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(LegPreAllocGrp_NoLegAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[354];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    LegPreAllocGrp::NoLegAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(LegPreAllocGrp_NoLegAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
