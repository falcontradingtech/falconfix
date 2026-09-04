// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotQualGrp::NoQuoteQualifiers (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotQualGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotQualGrp_NoQuoteQualifiersComponentTest : public ::testing::Test {
protected:
    QuotQualGrp::NoQuoteQualifiers component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotQualGrp_NoQuoteQualifiersComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setQuoteQualifier('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotQualGrp_NoQuoteQualifiersComponentTest, SetQuoteQualifierAndQuoteQualifierMatch) {
    const char test_value = 'X';
    component.setQuoteQualifier(test_value);
    EXPECT_EQ(component.getQuoteQualifier(), test_value);
    EXPECT_TRUE(component.hasQuoteQualifier());
}

TEST_F(FIX4_4_QuotQualGrp_NoQuoteQualifiersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setQuoteQualifier('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotQualGrp_NoQuoteQualifiersComponentTest, EncodeDecodeRoundtrip) {
    char buffer[12];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setQuoteQualifier(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    QuotQualGrp::NoQuoteQualifiers decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getQuoteQualifier(), component.getQuoteQualifier());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotQualGrp_NoQuoteQualifiersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
