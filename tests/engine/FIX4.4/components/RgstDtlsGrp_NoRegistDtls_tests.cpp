// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RgstDtlsGrp::NoRegistDtls (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RgstDtlsGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class RgstDtlsGrp_NoRegistDtlsComponentTest : public ::testing::Test {
protected:
    RgstDtlsGrp::NoRegistDtls component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(RgstDtlsGrp_NoRegistDtlsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setOwnerType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(RgstDtlsGrp_NoRegistDtlsComponentTest, SetOwnerTypeAndOwnerTypeMatch) {
    const int64_t test_value = 12345;
    component.setOwnerType(test_value);
    EXPECT_EQ(component.getOwnerType(), test_value);
    EXPECT_TRUE(component.hasOwnerType());
}

TEST_F(RgstDtlsGrp_NoRegistDtlsComponentTest, SetDateOfBirthAndDateOfBirthMatch) {
}

TEST_F(RgstDtlsGrp_NoRegistDtlsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setOwnerType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(RgstDtlsGrp_NoRegistDtlsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[446];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    RgstDtlsGrp::NoRegistDtls decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(RgstDtlsGrp_NoRegistDtlsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
