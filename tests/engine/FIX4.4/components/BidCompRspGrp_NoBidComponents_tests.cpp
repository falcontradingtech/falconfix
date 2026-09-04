// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: BidCompRspGrp::NoBidComponents (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/BidCompRspGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class BidCompRspGrp_NoBidComponentsComponentTest : public ::testing::Test {
protected:
    BidCompRspGrp::NoBidComponents component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSide('A');
    component.setPrice(3.14f);
    component.setPriceType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetPriceAndPriceMatch) {
    const double test_value = 123.456;
    component.setPrice(test_value);
    EXPECT_EQ(component.getPrice(), test_value);
    EXPECT_TRUE(component.hasPrice());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetPriceTypeAndPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setPriceType(test_value);
    EXPECT_EQ(component.getPriceType(), test_value);
    EXPECT_TRUE(component.hasPriceType());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetFairValueAndFairValueMatch) {
    const double test_value = 123.456;
    component.setFairValue(test_value);
    EXPECT_EQ(component.getFairValue(), test_value);
    EXPECT_TRUE(component.hasFairValue());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetNetGrossIndAndNetGrossIndMatch) {
    const int64_t test_value = 12345;
    component.setNetGrossInd(test_value);
    EXPECT_EQ(component.getNetGrossInd(), test_value);
    EXPECT_TRUE(component.hasNetGrossInd());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetSettlTypeAndSettlTypeMatch) {
    const char test_value = 'X';
    component.setSettlType(test_value);
    EXPECT_EQ(component.getSettlType(), test_value);
    EXPECT_TRUE(component.hasSettlType());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSide('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[748];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    BidCompRspGrp::NoBidComponents decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(BidCompRspGrp_NoBidComponentsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
