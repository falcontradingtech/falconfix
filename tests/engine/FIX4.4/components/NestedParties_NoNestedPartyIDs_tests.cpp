// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NestedParties::NoNestedPartyIDs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NestedParties.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_NestedParties_NoNestedPartyIDsComponentTest : public ::testing::Test {
protected:
    NestedParties::NoNestedPartyIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_NestedParties_NoNestedPartyIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setNestedPartyIDSource('A');
    component.setNestedPartyRole(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_NestedParties_NoNestedPartyIDsComponentTest, SetNestedPartyIDSourceAndNestedPartyIDSourceMatch) {
    const char test_value = 'X';
    component.setNestedPartyIDSource(test_value);
    EXPECT_EQ(component.getNestedPartyIDSource(), test_value);
    EXPECT_TRUE(component.hasNestedPartyIDSource());
}

TEST_F(FIX4_4_NestedParties_NoNestedPartyIDsComponentTest, SetNestedPartyRoleAndNestedPartyRoleMatch) {
    const int64_t test_value = 12345;
    component.setNestedPartyRole(test_value);
    EXPECT_EQ(component.getNestedPartyRole(), test_value);
    EXPECT_TRUE(component.hasNestedPartyRole());
}

TEST_F(FIX4_4_NestedParties_NoNestedPartyIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setNestedPartyIDSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_NestedParties_NoNestedPartyIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[136];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setNestedPartyIDSource(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    NestedParties::NoNestedPartyIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getNestedPartyIDSource(), component.getNestedPartyIDSource());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_NestedParties_NoNestedPartyIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
