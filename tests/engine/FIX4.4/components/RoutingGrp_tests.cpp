// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RoutingGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RoutingGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class RoutingGrpComponentTest : public ::testing::Test {
protected:
    RoutingGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(RoutingGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setRoutingType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(RoutingGrpComponentTest, SetRoutingTypeAndRoutingTypeMatch) {
    const int64_t test_value = 12345;
    component.setRoutingType(test_value);
    EXPECT_EQ(component.getRoutingType(), test_value);
    EXPECT_TRUE(component.hasRoutingType());
}

TEST_F(RoutingGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setRoutingType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(RoutingGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[124];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    RoutingGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(RoutingGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
