// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RgstDistInstGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RgstDistInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class RgstDistInstGrpComponentTest : public ::testing::Test {
protected:
    RgstDistInstGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(RgstDistInstGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setDistribPaymentMethod(42);
    component.setDistribPercentage(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(RgstDistInstGrpComponentTest, SetDistribPaymentMethodAndDistribPaymentMethodMatch) {
    const int64_t test_value = 12345;
    component.setDistribPaymentMethod(test_value);
    EXPECT_EQ(component.getDistribPaymentMethod(), test_value);
    EXPECT_TRUE(component.hasDistribPaymentMethod());
}

TEST_F(RgstDistInstGrpComponentTest, SetDistribPercentageAndDistribPercentageMatch) {
    const double test_value = 123.456;
    component.setDistribPercentage(test_value);
    EXPECT_EQ(component.getDistribPercentage(), test_value);
    EXPECT_TRUE(component.hasDistribPercentage());
}

TEST_F(RgstDistInstGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setDistribPaymentMethod(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(RgstDistInstGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[552];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    RgstDistInstGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(RgstDistInstGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
