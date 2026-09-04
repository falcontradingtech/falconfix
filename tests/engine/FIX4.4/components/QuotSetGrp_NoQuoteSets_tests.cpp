// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotSetGrp::NoQuoteSets (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotSetGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest : public ::testing::Test {
protected:
    QuotSetGrp::NoQuoteSets component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setTotNoQuoteEntries(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, SetQuoteSetValidUntilTimeAndQuoteSetValidUntilTimeMatch) {
}

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, SetTotNoQuoteEntriesAndTotNoQuoteEntriesMatch) {
    const int64_t test_value = 12345;
    component.setTotNoQuoteEntries(test_value);
    EXPECT_EQ(component.getTotNoQuoteEntries(), test_value);
    EXPECT_TRUE(component.hasTotNoQuoteEntries());
}

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, SetLastFragmentAndLastFragmentMatch) {
}

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setQuoteSetValidUntilTime(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[180];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setTotNoQuoteEntries(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    QuotSetGrp::NoQuoteSets decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getTotNoQuoteEntries(), component.getTotNoQuoteEntries());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetGrp_NoQuoteSetsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}
