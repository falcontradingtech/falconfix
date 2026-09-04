// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: Parties (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/Parties.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PartiesComponentTest : public ::testing::Test {
protected:
    Parties component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PartiesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPartyIDSource('A');
    component.setPartyRole(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PartiesComponentTest, SetPartyIDSourceAndPartyIDSourceMatch) {
    const char test_value = 'X';
    component.setPartyIDSource(test_value);
    EXPECT_EQ(component.getPartyIDSource(), test_value);
    EXPECT_TRUE(component.hasPartyIDSource());
}

TEST_F(PartiesComponentTest, SetPartyRoleAndPartyRoleMatch) {
    const int64_t test_value = 12345;
    component.setPartyRole(test_value);
    EXPECT_EQ(component.getPartyRole(), test_value);
    EXPECT_TRUE(component.hasPartyRole());
}

TEST_F(PartiesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPartyIDSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PartiesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[136];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    Parties decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(PartiesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
