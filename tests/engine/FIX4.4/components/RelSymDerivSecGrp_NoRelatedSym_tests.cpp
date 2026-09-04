// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RelSymDerivSecGrp::NoRelatedSym (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RelSymDerivSecGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class RelSymDerivSecGrp_NoRelatedSymComponentTest : public ::testing::Test {
protected:
    RelSymDerivSecGrp::NoRelatedSym component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(RelSymDerivSecGrp_NoRelatedSymComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setExpirationCycle(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(RelSymDerivSecGrp_NoRelatedSymComponentTest, SetExpirationCycleAndExpirationCycleMatch) {
    const int64_t test_value = 12345;
    component.setExpirationCycle(test_value);
    EXPECT_EQ(component.getExpirationCycle(), test_value);
    EXPECT_TRUE(component.hasExpirationCycle());
}

TEST_F(RelSymDerivSecGrp_NoRelatedSymComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(RelSymDerivSecGrp_NoRelatedSymComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setExpirationCycle(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(RelSymDerivSecGrp_NoRelatedSymComponentTest, EncodeDecodeRoundtrip) {
    char buffer[466];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    RelSymDerivSecGrp::NoRelatedSym decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(RelSymDerivSecGrp_NoRelatedSymComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
