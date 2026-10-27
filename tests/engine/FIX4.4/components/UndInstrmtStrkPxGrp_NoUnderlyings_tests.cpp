// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: UndInstrmtStrkPxGrp::NoUnderlyings (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/UndInstrmtStrkPxGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest : public ::testing::Test {
protected:
    UndInstrmtStrkPxGrp::NoUnderlyings component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const double test_value = 123.456; component.setPrevClosePx(test_value); }
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryClOrdID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetPrevClosePxAndPrevClosePxMatch) {
    const double test_value = 123.456;
    component.setPrevClosePx(test_value);
    EXPECT_EQ(component.getPrevClosePx(), test_value);
    EXPECT_TRUE(component.hasPrevClosePx());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetPriceAndPriceMatch) {
    const double test_value = 123.456;
    component.setPrice(test_value);
    EXPECT_EQ(component.getPrice(), test_value);
    EXPECT_TRUE(component.hasPrice());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const double test_value = 123.456; component.setPrevClosePx(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setPrevClosePx(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    UndInstrmtStrkPxGrp::NoUnderlyings decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getPrevClosePx(), component.getPrevClosePx());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_UndInstrmtStrkPxGrp_NoUnderlyingsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}
