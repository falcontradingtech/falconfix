// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: Instrument (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/Instrument.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_InstrumentComponentTest : public ::testing::Test {
protected:
    Instrument component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_InstrumentComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setProduct(42);
    component.setPutOrCall(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetProductAndProductMatch) {
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    EXPECT_EQ(component.getProduct(), test_value);
    EXPECT_TRUE(component.hasProduct());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetMaturityDateAndMaturityDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetPutOrCallAndPutOrCallMatch) {
    const int64_t test_value = 12345;
    component.setPutOrCall(test_value);
    EXPECT_EQ(component.getPutOrCall(), test_value);
    EXPECT_TRUE(component.hasPutOrCall());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCouponPaymentDateAndCouponPaymentDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetIssueDateAndIssueDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetRepurchaseTermAndRepurchaseTermMatch) {
    const int64_t test_value = 12345;
    component.setRepurchaseTerm(test_value);
    EXPECT_EQ(component.getRepurchaseTerm(), test_value);
    EXPECT_TRUE(component.hasRepurchaseTerm());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetRepurchaseRateAndRepurchaseRateMatch) {
    const double test_value = 123.456;
    component.setRepurchaseRate(test_value);
    EXPECT_EQ(component.getRepurchaseRate(), test_value);
    EXPECT_TRUE(component.hasRepurchaseRate());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetFactorAndFactorMatch) {
    const double test_value = 123.456;
    component.setFactor(test_value);
    EXPECT_EQ(component.getFactor(), test_value);
    EXPECT_TRUE(component.hasFactor());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetRedemptionDateAndRedemptionDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetStrikePriceAndStrikePriceMatch) {
    const double test_value = 123.456;
    component.setStrikePrice(test_value);
    EXPECT_EQ(component.getStrikePrice(), test_value);
    EXPECT_TRUE(component.hasStrikePrice());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetOptAttributeAndOptAttributeMatch) {
    const char test_value = 'X';
    component.setOptAttribute(test_value);
    EXPECT_EQ(component.getOptAttribute(), test_value);
    EXPECT_TRUE(component.hasOptAttribute());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetContractMultiplierAndContractMultiplierMatch) {
    const double test_value = 123.456;
    component.setContractMultiplier(test_value);
    EXPECT_EQ(component.getContractMultiplier(), test_value);
    EXPECT_TRUE(component.hasContractMultiplier());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCouponRateAndCouponRateMatch) {
    const double test_value = 123.456;
    component.setCouponRate(test_value);
    EXPECT_EQ(component.getCouponRate(), test_value);
    EXPECT_TRUE(component.hasCouponRate());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetEncodedIssuerLenAndEncodedIssuerLenMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetEncodedSecurityDescLenAndEncodedSecurityDescLenMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCPProgramAndCPProgramMatch) {
    const int64_t test_value = 12345;
    component.setCPProgram(test_value);
    EXPECT_EQ(component.getCPProgram(), test_value);
    EXPECT_TRUE(component.hasCPProgram());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetDatedDateAndDatedDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetInterestAccrualDateAndInterestAccrualDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setProduct(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrumentComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2452];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    Instrument decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getProduct(), component.getProduct());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_InstrumentComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
