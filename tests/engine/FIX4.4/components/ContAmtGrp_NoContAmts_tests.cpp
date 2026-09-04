// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ContAmtGrp::NoContAmts (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ContAmtGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class ContAmtGrp_NoContAmtsComponentTest : public ::testing::Test {
protected:
    ContAmtGrp::NoContAmts component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(ContAmtGrp_NoContAmtsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setContAmtType(42);
    component.setContAmtValue(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(ContAmtGrp_NoContAmtsComponentTest, SetContAmtTypeAndContAmtTypeMatch) {
    const int64_t test_value = 12345;
    component.setContAmtType(test_value);
    EXPECT_EQ(component.getContAmtType(), test_value);
    EXPECT_TRUE(component.hasContAmtType());
}

TEST_F(ContAmtGrp_NoContAmtsComponentTest, SetContAmtValueAndContAmtValueMatch) {
    const double test_value = 123.456;
    component.setContAmtValue(test_value);
    EXPECT_EQ(component.getContAmtValue(), test_value);
    EXPECT_TRUE(component.hasContAmtValue());
}

TEST_F(ContAmtGrp_NoContAmtsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setContAmtType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(ContAmtGrp_NoContAmtsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[182];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    ContAmtGrp::NoContAmts decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(ContAmtGrp_NoContAmtsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
