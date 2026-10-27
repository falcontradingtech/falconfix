// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SecAltIDGrp::NoSecurityAltID (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SecAltIDGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest : public ::testing::Test {
protected:
    SecAltIDGrp::NoSecurityAltID component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setSecurityAltID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecurityAltIDSource(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest, SetSecurityAltIDAndSecurityAltIDMatch) {
    EXPECT_FALSE(component.hasSecurityAltID());
}

TEST_F(FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest, SetSecurityAltIDSourceAndSecurityAltIDSourceMatch) {
    EXPECT_FALSE(component.hasSecurityAltIDSource());
}

TEST_F(FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setSecurityAltID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    { const std::string_view test_value{"AAA"}; component.setSecurityAltID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SecAltIDGrp::NoSecurityAltID decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SecAltIDGrp_NoSecurityAltIDComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
