// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: Parties::NoPartyIDs (FIX4.3)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.3/components/Parties.h>
#include <FIX4.3/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix43::components;

class FIX4_3_Parties_NoPartyIDsComponentTest : public ::testing::Test {
protected:
    Parties::NoPartyIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_3_Parties_NoPartyIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPartyIDSource('A');
    component.setPartyRole(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_3_Parties_NoPartyIDsComponentTest, SetPartyIDSourceAndPartyIDSourceMatch) {
    const char test_value = 'X';
    component.setPartyIDSource(test_value);
    EXPECT_EQ(component.getPartyIDSource(), test_value);
    EXPECT_TRUE(component.hasPartyIDSource());
}

TEST_F(FIX4_3_Parties_NoPartyIDsComponentTest, SetPartyRoleAndPartyRoleMatch) {
    const int64_t test_value = 12345;
    component.setPartyRole(test_value);
    EXPECT_EQ(component.getPartyRole(), test_value);
    EXPECT_TRUE(component.hasPartyRole());
}

TEST_F(FIX4_3_Parties_NoPartyIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPartyIDSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_3_Parties_NoPartyIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[210];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setPartyIDSource(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    Parties::NoPartyIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getPartyIDSource(), component.getPartyIDSource());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_3_Parties_NoPartyIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
