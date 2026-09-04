// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrumentLeg (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/InstrumentLeg.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class InstrumentLegComponentTest : public ::testing::Test {
protected:
    InstrumentLeg component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(InstrumentLegComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegProduct(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(InstrumentLegComponentTest, SetLegProductAndLegProductMatch) {
    const int64_t test_value = 12345;
    component.setLegProduct(test_value);
    EXPECT_EQ(component.getLegProduct(), test_value);
    EXPECT_TRUE(component.hasLegProduct());
}

TEST_F(InstrumentLegComponentTest, SetLegMaturityDateAndLegMaturityDateMatch) {
}

TEST_F(InstrumentLegComponentTest, SetLegCouponPaymentDateAndLegCouponPaymentDateMatch) {
}

TEST_F(InstrumentLegComponentTest, SetLegIssueDateAndLegIssueDateMatch) {
}

TEST_F(InstrumentLegComponentTest, SetLegRepurchaseTermAndLegRepurchaseTermMatch) {
    const int64_t test_value = 12345;
    component.setLegRepurchaseTerm(test_value);
    EXPECT_EQ(component.getLegRepurchaseTerm(), test_value);
    EXPECT_TRUE(component.hasLegRepurchaseTerm());
}

TEST_F(InstrumentLegComponentTest, SetLegRepurchaseRateAndLegRepurchaseRateMatch) {
    const double test_value = 123.456;
    component.setLegRepurchaseRate(test_value);
    EXPECT_EQ(component.getLegRepurchaseRate(), test_value);
    EXPECT_TRUE(component.hasLegRepurchaseRate());
}

TEST_F(InstrumentLegComponentTest, SetLegFactorAndLegFactorMatch) {
    const double test_value = 123.456;
    component.setLegFactor(test_value);
    EXPECT_EQ(component.getLegFactor(), test_value);
    EXPECT_TRUE(component.hasLegFactor());
}

TEST_F(InstrumentLegComponentTest, SetLegRedemptionDateAndLegRedemptionDateMatch) {
}

TEST_F(InstrumentLegComponentTest, SetLegStrikePriceAndLegStrikePriceMatch) {
    const double test_value = 123.456;
    component.setLegStrikePrice(test_value);
    EXPECT_EQ(component.getLegStrikePrice(), test_value);
    EXPECT_TRUE(component.hasLegStrikePrice());
}

TEST_F(InstrumentLegComponentTest, SetLegOptAttributeAndLegOptAttributeMatch) {
    const char test_value = 'X';
    component.setLegOptAttribute(test_value);
    EXPECT_EQ(component.getLegOptAttribute(), test_value);
    EXPECT_TRUE(component.hasLegOptAttribute());
}

TEST_F(InstrumentLegComponentTest, SetLegContractMultiplierAndLegContractMultiplierMatch) {
    const double test_value = 123.456;
    component.setLegContractMultiplier(test_value);
    EXPECT_EQ(component.getLegContractMultiplier(), test_value);
    EXPECT_TRUE(component.hasLegContractMultiplier());
}

TEST_F(InstrumentLegComponentTest, SetLegCouponRateAndLegCouponRateMatch) {
    const double test_value = 123.456;
    component.setLegCouponRate(test_value);
    EXPECT_EQ(component.getLegCouponRate(), test_value);
    EXPECT_TRUE(component.hasLegCouponRate());
}

TEST_F(InstrumentLegComponentTest, SetEncodedLegIssuerLenAndEncodedLegIssuerLenMatch) {
}

TEST_F(InstrumentLegComponentTest, SetEncodedLegSecurityDescLenAndEncodedLegSecurityDescLenMatch) {
}

TEST_F(InstrumentLegComponentTest, SetLegRatioQtyAndLegRatioQtyMatch) {
    const double test_value = 123.456;
    component.setLegRatioQty(test_value);
    EXPECT_EQ(component.getLegRatioQty(), test_value);
    EXPECT_TRUE(component.hasLegRatioQty());
}

TEST_F(InstrumentLegComponentTest, SetLegSideAndLegSideMatch) {
    const char test_value = 'X';
    component.setLegSide(test_value);
    EXPECT_EQ(component.getLegSide(), test_value);
    EXPECT_TRUE(component.hasLegSide());
}

TEST_F(InstrumentLegComponentTest, SetLegDatedDateAndLegDatedDateMatch) {
}

TEST_F(InstrumentLegComponentTest, SetLegInterestAccrualDateAndLegInterestAccrualDateMatch) {
}

TEST_F(InstrumentLegComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegProduct(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(InstrumentLegComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2430];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setLegProduct(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    InstrumentLeg decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getLegProduct(), component.getLegProduct());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(InstrumentLegComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
