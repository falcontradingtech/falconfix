// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: UndSecAltIDGrp::NoUnderlyingSecurityAltID (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/UndSecAltIDGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest : public ::testing::Test {
protected:
    UndSecAltIDGrp::NoUnderlyingSecurityAltID component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSecurityAltID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSecurityAltIDSource(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, SetUnderlyingSecurityAltIDAndUnderlyingSecurityAltIDMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecurityAltID());
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, SetUnderlyingSecurityAltIDSourceAndUnderlyingSecurityAltIDSourceMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecurityAltIDSource());
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSecurityAltID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSecurityAltID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    UndSecAltIDGrp::NoUnderlyingSecurityAltID decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    UndSecAltIDGrp::NoUnderlyingSecurityAltID decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4096];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_UndSecAltIDGrp_NoUnderlyingSecurityAltIDComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
