// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SideCrossOrdCxlGrp::NoSides (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SideCrossOrdCxlGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SideCrossOrdCxlGrp_NoSidesComponentTest : public ::testing::Test {
protected:
    SideCrossOrdCxlGrp::NoSides component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSide('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, SetOrigOrdModTimeAndOrigOrdModTimeMatch) {
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSide('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[666];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setSide(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SideCrossOrdCxlGrp::NoSides decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSide(), component.getSide());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(SideCrossOrdCxlGrp_NoSidesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}
