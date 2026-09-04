// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotQualGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotQualGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class QuotQualGrpComponentTest : public ::testing::Test {
protected:
    QuotQualGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(QuotQualGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setQuoteQualifier('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(QuotQualGrpComponentTest, SetQuoteQualifierAndQuoteQualifierMatch) {
    const char test_value = 'X';
    component.setQuoteQualifier(test_value);
    EXPECT_EQ(component.getQuoteQualifier(), test_value);
    EXPECT_TRUE(component.hasQuoteQualifier());
}

TEST_F(QuotQualGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setQuoteQualifier('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(QuotQualGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[12];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    QuotQualGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(QuotQualGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
