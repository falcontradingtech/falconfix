// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NestedParties3::NoNested3PartyIDs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NestedParties3.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class NestedParties3_NoNested3PartyIDsComponentTest : public ::testing::Test {
protected:
    NestedParties3::NoNested3PartyIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(NestedParties3_NoNested3PartyIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setNested3PartyIDSource('A');
    component.setNested3PartyRole(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(NestedParties3_NoNested3PartyIDsComponentTest, SetNested3PartyIDSourceAndNested3PartyIDSourceMatch) {
    const char test_value = 'X';
    component.setNested3PartyIDSource(test_value);
    EXPECT_EQ(component.getNested3PartyIDSource(), test_value);
    EXPECT_TRUE(component.hasNested3PartyIDSource());
}

TEST_F(NestedParties3_NoNested3PartyIDsComponentTest, SetNested3PartyRoleAndNested3PartyRoleMatch) {
    const int64_t test_value = 12345;
    component.setNested3PartyRole(test_value);
    EXPECT_EQ(component.getNested3PartyRole(), test_value);
    EXPECT_TRUE(component.hasNested3PartyRole());
}

TEST_F(NestedParties3_NoNested3PartyIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setNested3PartyIDSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(NestedParties3_NoNested3PartyIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[136];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    NestedParties3::NoNested3PartyIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(NestedParties3_NoNested3PartyIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
