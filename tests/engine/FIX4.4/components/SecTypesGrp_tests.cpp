// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SecTypesGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SecTypesGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SecTypesGrpComponentTest : public ::testing::Test {
protected:
    SecTypesGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SecTypesGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setProduct(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SecTypesGrpComponentTest, SetProductAndProductMatch) {
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    EXPECT_EQ(component.getProduct(), test_value);
    EXPECT_TRUE(component.hasProduct());
}

TEST_F(SecTypesGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setProduct(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SecTypesGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[272];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    SecTypesGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(SecTypesGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
