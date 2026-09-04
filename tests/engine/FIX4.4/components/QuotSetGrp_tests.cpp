// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotSetGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotSetGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class QuotSetGrpComponentTest : public ::testing::Test {
protected:
    QuotSetGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(QuotSetGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setTotNoQuoteEntries(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(QuotSetGrpComponentTest, SetQuoteSetValidUntilTimeAndQuoteSetValidUntilTimeMatch) {
}

TEST_F(QuotSetGrpComponentTest, SetTotNoQuoteEntriesAndTotNoQuoteEntriesMatch) {
    const int64_t test_value = 12345;
    component.setTotNoQuoteEntries(test_value);
    EXPECT_EQ(component.getTotNoQuoteEntries(), test_value);
    EXPECT_TRUE(component.hasTotNoQuoteEntries());
}

TEST_F(QuotSetGrpComponentTest, SetLastFragmentAndLastFragmentMatch) {
}

TEST_F(QuotSetGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setQuoteSetValidUntilTime(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(QuotSetGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[180];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    QuotSetGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(QuotSetGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
