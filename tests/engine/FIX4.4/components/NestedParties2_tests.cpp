// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NestedParties2 (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NestedParties2.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class NestedParties2ComponentTest : public ::testing::Test {
protected:
    NestedParties2 component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(NestedParties2ComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setNested2PartyIDSource('A');
    component.setNested2PartyRole(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(NestedParties2ComponentTest, SetNested2PartyIDSourceAndNested2PartyIDSourceMatch) {
    const char test_value = 'X';
    component.setNested2PartyIDSource(test_value);
    EXPECT_EQ(component.getNested2PartyIDSource(), test_value);
    EXPECT_TRUE(component.hasNested2PartyIDSource());
}

TEST_F(NestedParties2ComponentTest, SetNested2PartyRoleAndNested2PartyRoleMatch) {
    const int64_t test_value = 12345;
    component.setNested2PartyRole(test_value);
    EXPECT_EQ(component.getNested2PartyRole(), test_value);
    EXPECT_TRUE(component.hasNested2PartyRole());
}

TEST_F(NestedParties2ComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setNested2PartyIDSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(NestedParties2ComponentTest, EncodeDecodeRoundtrip) {
    char buffer[136];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    NestedParties2 decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(NestedParties2ComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
