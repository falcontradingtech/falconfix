// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: CompIDStatGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/CompIDStatGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class CompIDStatGrpComponentTest : public ::testing::Test {
protected:
    CompIDStatGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(CompIDStatGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setStatusValue(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(CompIDStatGrpComponentTest, SetStatusValueAndStatusValueMatch) {
    const int64_t test_value = 12345;
    component.setStatusValue(test_value);
    EXPECT_EQ(component.getStatusValue(), test_value);
    EXPECT_TRUE(component.hasStatusValue());
}

TEST_F(CompIDStatGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setStatusValue(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(CompIDStatGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[420];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    CompIDStatGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(CompIDStatGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
