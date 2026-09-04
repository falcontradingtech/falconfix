// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PreAllocGrp::NoAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PreAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PreAllocGrp_NoAllocsComponentTest : public ::testing::Test {
protected:
    PreAllocGrp::NoAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PreAllocGrp_NoAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setAllocAcctIDSource(42);
    component.setAllocQty(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PreAllocGrp_NoAllocsComponentTest, SetAllocAcctIDSourceAndAllocAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    EXPECT_EQ(component.getAllocAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAllocAcctIDSource());
}

TEST_F(PreAllocGrp_NoAllocsComponentTest, SetAllocQtyAndAllocQtyMatch) {
    const double test_value = 123.456;
    component.setAllocQty(test_value);
    EXPECT_EQ(component.getAllocQty(), test_value);
    EXPECT_TRUE(component.hasAllocQty());
}

TEST_F(PreAllocGrp_NoAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setAllocAcctIDSource(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PreAllocGrp_NoAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[326];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    PreAllocGrp::NoAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(PreAllocGrp_NoAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
