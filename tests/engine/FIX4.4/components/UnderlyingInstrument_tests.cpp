// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: UnderlyingInstrument (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/UnderlyingInstrument.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class UnderlyingInstrumentComponentTest : public ::testing::Test {
protected:
    UnderlyingInstrument component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(UnderlyingInstrumentComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setUnderlyingProduct(42);
    component.setUnderlyingPutOrCall(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingProductAndUnderlyingProductMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingProduct(test_value);
    EXPECT_EQ(component.getUnderlyingProduct(), test_value);
    EXPECT_TRUE(component.hasUnderlyingProduct());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingMaturityDateAndUnderlyingMaturityDateMatch) {
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingPutOrCallAndUnderlyingPutOrCallMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingPutOrCall(test_value);
    EXPECT_EQ(component.getUnderlyingPutOrCall(), test_value);
    EXPECT_TRUE(component.hasUnderlyingPutOrCall());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingCouponPaymentDateAndUnderlyingCouponPaymentDateMatch) {
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingIssueDateAndUnderlyingIssueDateMatch) {
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingRepurchaseTermAndUnderlyingRepurchaseTermMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingRepurchaseTerm(test_value);
    EXPECT_EQ(component.getUnderlyingRepurchaseTerm(), test_value);
    EXPECT_TRUE(component.hasUnderlyingRepurchaseTerm());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingRepurchaseRateAndUnderlyingRepurchaseRateMatch) {
    const double test_value = 123.456;
    component.setUnderlyingRepurchaseRate(test_value);
    EXPECT_EQ(component.getUnderlyingRepurchaseRate(), test_value);
    EXPECT_TRUE(component.hasUnderlyingRepurchaseRate());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingFactorAndUnderlyingFactorMatch) {
    const double test_value = 123.456;
    component.setUnderlyingFactor(test_value);
    EXPECT_EQ(component.getUnderlyingFactor(), test_value);
    EXPECT_TRUE(component.hasUnderlyingFactor());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingRedemptionDateAndUnderlyingRedemptionDateMatch) {
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingStrikePriceAndUnderlyingStrikePriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingStrikePrice(test_value);
    EXPECT_EQ(component.getUnderlyingStrikePrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingStrikePrice());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingOptAttributeAndUnderlyingOptAttributeMatch) {
    const char test_value = 'X';
    component.setUnderlyingOptAttribute(test_value);
    EXPECT_EQ(component.getUnderlyingOptAttribute(), test_value);
    EXPECT_TRUE(component.hasUnderlyingOptAttribute());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingContractMultiplierAndUnderlyingContractMultiplierMatch) {
    const double test_value = 123.456;
    component.setUnderlyingContractMultiplier(test_value);
    EXPECT_EQ(component.getUnderlyingContractMultiplier(), test_value);
    EXPECT_TRUE(component.hasUnderlyingContractMultiplier());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingCouponRateAndUnderlyingCouponRateMatch) {
    const double test_value = 123.456;
    component.setUnderlyingCouponRate(test_value);
    EXPECT_EQ(component.getUnderlyingCouponRate(), test_value);
    EXPECT_TRUE(component.hasUnderlyingCouponRate());
}

TEST_F(UnderlyingInstrumentComponentTest, SetEncodedUnderlyingIssuerLenAndEncodedUnderlyingIssuerLenMatch) {
}

TEST_F(UnderlyingInstrumentComponentTest, SetEncodedUnderlyingSecurityDescLenAndEncodedUnderlyingSecurityDescLenMatch) {
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingQtyAndUnderlyingQtyMatch) {
    const double test_value = 123.456;
    component.setUnderlyingQty(test_value);
    EXPECT_EQ(component.getUnderlyingQty(), test_value);
    EXPECT_TRUE(component.hasUnderlyingQty());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingPxAndUnderlyingPxMatch) {
    const double test_value = 123.456;
    component.setUnderlyingPx(test_value);
    EXPECT_EQ(component.getUnderlyingPx(), test_value);
    EXPECT_TRUE(component.hasUnderlyingPx());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingDirtyPriceAndUnderlyingDirtyPriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingDirtyPrice(test_value);
    EXPECT_EQ(component.getUnderlyingDirtyPrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingDirtyPrice());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingEndPriceAndUnderlyingEndPriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingEndPrice(test_value);
    EXPECT_EQ(component.getUnderlyingEndPrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingEndPrice());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingStartValueAndUnderlyingStartValueMatch) {
    const double test_value = 123.456;
    component.setUnderlyingStartValue(test_value);
    EXPECT_EQ(component.getUnderlyingStartValue(), test_value);
    EXPECT_TRUE(component.hasUnderlyingStartValue());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingCurrentValueAndUnderlyingCurrentValueMatch) {
    const double test_value = 123.456;
    component.setUnderlyingCurrentValue(test_value);
    EXPECT_EQ(component.getUnderlyingCurrentValue(), test_value);
    EXPECT_TRUE(component.hasUnderlyingCurrentValue());
}

TEST_F(UnderlyingInstrumentComponentTest, SetUnderlyingEndValueAndUnderlyingEndValueMatch) {
    const double test_value = 123.456;
    component.setUnderlyingEndValue(test_value);
    EXPECT_EQ(component.getUnderlyingEndValue(), test_value);
    EXPECT_TRUE(component.hasUnderlyingEndValue());
}

TEST_F(UnderlyingInstrumentComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setUnderlyingProduct(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(UnderlyingInstrumentComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2764];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setUnderlyingProduct(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    UnderlyingInstrument decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getUnderlyingProduct(), component.getUnderlyingProduct());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(UnderlyingInstrumentComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
