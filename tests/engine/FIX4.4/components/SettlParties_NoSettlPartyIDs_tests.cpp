// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlParties::NoSettlPartyIDs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlParties.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SettlParties_NoSettlPartyIDsComponentTest : public ::testing::Test {
protected:
    SettlParties::NoSettlPartyIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SettlParties_NoSettlPartyIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSettlPartyIDSource('A');
    component.setSettlPartyRole(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SettlParties_NoSettlPartyIDsComponentTest, SetSettlPartyIDSourceAndSettlPartyIDSourceMatch) {
    const char test_value = 'X';
    component.setSettlPartyIDSource(test_value);
    EXPECT_EQ(component.getSettlPartyIDSource(), test_value);
    EXPECT_TRUE(component.hasSettlPartyIDSource());
}

TEST_F(SettlParties_NoSettlPartyIDsComponentTest, SetSettlPartyRoleAndSettlPartyRoleMatch) {
    const int64_t test_value = 12345;
    component.setSettlPartyRole(test_value);
    EXPECT_EQ(component.getSettlPartyRole(), test_value);
    EXPECT_TRUE(component.hasSettlPartyRole());
}

TEST_F(SettlParties_NoSettlPartyIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSettlPartyIDSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SettlParties_NoSettlPartyIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[136];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    SettlParties::NoSettlPartyIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(SettlParties_NoSettlPartyIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
