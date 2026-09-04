// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegSecAltIDGrp::NoLegSecurityAltID (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegSecAltIDGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class LegSecAltIDGrp_NoLegSecurityAltIDComponentTest : public ::testing::Test {
protected:
    LegSecAltIDGrp::NoLegSecurityAltID component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(LegSecAltIDGrp_NoLegSecurityAltIDComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(LegSecAltIDGrp_NoLegSecurityAltIDComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(LegSecAltIDGrp_NoLegSecurityAltIDComponentTest, EncodeDecodeRoundtrip) {
    char buffer[148];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    LegSecAltIDGrp::NoLegSecurityAltID decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(LegSecAltIDGrp_NoLegSecurityAltIDComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
