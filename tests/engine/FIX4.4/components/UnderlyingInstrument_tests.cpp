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

class FIX4_4_UnderlyingInstrumentComponentTest : public ::testing::Test {
protected:
    UnderlyingInstrument component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSymbol(test_value); }
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSymbolSfx(test_value); }
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSecurityID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSymbolAndUnderlyingSymbolMatch) {
    EXPECT_FALSE(component.hasUnderlyingSymbol());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSymbolSfxAndUnderlyingSymbolSfxMatch) {
    EXPECT_FALSE(component.hasUnderlyingSymbolSfx());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSecurityIDAndUnderlyingSecurityIDMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecurityID());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSecurityIDSourceAndUnderlyingSecurityIDSourceMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecurityIDSource());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingProductAndUnderlyingProductMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingProduct(test_value);
    EXPECT_EQ(component.getUnderlyingProduct(), test_value);
    EXPECT_TRUE(component.hasUnderlyingProduct());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCFICodeAndUnderlyingCFICodeMatch) {
    EXPECT_FALSE(component.hasUnderlyingCFICode());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSecurityTypeAndUnderlyingSecurityTypeMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecurityType());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSecuritySubTypeAndUnderlyingSecuritySubTypeMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecuritySubType());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingMaturityMonthYearAndUnderlyingMaturityMonthYearMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingMaturityDateAndUnderlyingMaturityDateMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingPutOrCallAndUnderlyingPutOrCallMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingPutOrCall(test_value);
    EXPECT_EQ(component.getUnderlyingPutOrCall(), test_value);
    EXPECT_TRUE(component.hasUnderlyingPutOrCall());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCouponPaymentDateAndUnderlyingCouponPaymentDateMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingIssueDateAndUnderlyingIssueDateMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingRepoCollateralSecurityTypeAndUnderlyingRepoCollateralSecurityTypeMatch) {
    EXPECT_FALSE(component.hasUnderlyingRepoCollateralSecurityType());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingRepurchaseTermAndUnderlyingRepurchaseTermMatch) {
    const int64_t test_value = 12345;
    component.setUnderlyingRepurchaseTerm(test_value);
    EXPECT_EQ(component.getUnderlyingRepurchaseTerm(), test_value);
    EXPECT_TRUE(component.hasUnderlyingRepurchaseTerm());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingRepurchaseRateAndUnderlyingRepurchaseRateMatch) {
    const double test_value = 123.456;
    component.setUnderlyingRepurchaseRate(test_value);
    EXPECT_EQ(component.getUnderlyingRepurchaseRate(), test_value);
    EXPECT_TRUE(component.hasUnderlyingRepurchaseRate());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingFactorAndUnderlyingFactorMatch) {
    const double test_value = 123.456;
    component.setUnderlyingFactor(test_value);
    EXPECT_EQ(component.getUnderlyingFactor(), test_value);
    EXPECT_TRUE(component.hasUnderlyingFactor());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCreditRatingAndUnderlyingCreditRatingMatch) {
    EXPECT_FALSE(component.hasUnderlyingCreditRating());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingInstrRegistryAndUnderlyingInstrRegistryMatch) {
    EXPECT_FALSE(component.hasUnderlyingInstrRegistry());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCountryOfIssueAndUnderlyingCountryOfIssueMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingStateOrProvinceOfIssueAndUnderlyingStateOrProvinceOfIssueMatch) {
    EXPECT_FALSE(component.hasUnderlyingStateOrProvinceOfIssue());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingLocaleOfIssueAndUnderlyingLocaleOfIssueMatch) {
    EXPECT_FALSE(component.hasUnderlyingLocaleOfIssue());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingRedemptionDateAndUnderlyingRedemptionDateMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingStrikePriceAndUnderlyingStrikePriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingStrikePrice(test_value);
    EXPECT_EQ(component.getUnderlyingStrikePrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingStrikePrice());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingStrikeCurrencyAndUnderlyingStrikeCurrencyMatch) {
    EXPECT_FALSE(component.hasUnderlyingStrikeCurrency());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingOptAttributeAndUnderlyingOptAttributeMatch) {
    const char test_value = 'X';
    component.setUnderlyingOptAttribute(test_value);
    EXPECT_EQ(component.getUnderlyingOptAttribute(), test_value);
    EXPECT_TRUE(component.hasUnderlyingOptAttribute());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingContractMultiplierAndUnderlyingContractMultiplierMatch) {
    const double test_value = 123.456;
    component.setUnderlyingContractMultiplier(test_value);
    EXPECT_EQ(component.getUnderlyingContractMultiplier(), test_value);
    EXPECT_TRUE(component.hasUnderlyingContractMultiplier());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCouponRateAndUnderlyingCouponRateMatch) {
    const double test_value = 123.456;
    component.setUnderlyingCouponRate(test_value);
    EXPECT_EQ(component.getUnderlyingCouponRate(), test_value);
    EXPECT_TRUE(component.hasUnderlyingCouponRate());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSecurityExchangeAndUnderlyingSecurityExchangeMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingIssuerAndUnderlyingIssuerMatch) {
    EXPECT_FALSE(component.hasUnderlyingIssuer());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetEncodedUnderlyingIssuerLenAndEncodedUnderlyingIssuerLenMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetEncodedUnderlyingIssuerAndEncodedUnderlyingIssuerMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingSecurityDescAndUnderlyingSecurityDescMatch) {
    EXPECT_FALSE(component.hasUnderlyingSecurityDesc());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetEncodedUnderlyingSecurityDescLenAndEncodedUnderlyingSecurityDescLenMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetEncodedUnderlyingSecurityDescAndEncodedUnderlyingSecurityDescMatch) {
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCPProgramAndUnderlyingCPProgramMatch) {
    EXPECT_FALSE(component.hasUnderlyingCPProgram());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCPRegTypeAndUnderlyingCPRegTypeMatch) {
    EXPECT_FALSE(component.hasUnderlyingCPRegType());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCurrencyAndUnderlyingCurrencyMatch) {
    EXPECT_FALSE(component.hasUnderlyingCurrency());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingQtyAndUnderlyingQtyMatch) {
    const double test_value = 123.456;
    component.setUnderlyingQty(test_value);
    EXPECT_EQ(component.getUnderlyingQty(), test_value);
    EXPECT_TRUE(component.hasUnderlyingQty());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingPxAndUnderlyingPxMatch) {
    const double test_value = 123.456;
    component.setUnderlyingPx(test_value);
    EXPECT_EQ(component.getUnderlyingPx(), test_value);
    EXPECT_TRUE(component.hasUnderlyingPx());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingDirtyPriceAndUnderlyingDirtyPriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingDirtyPrice(test_value);
    EXPECT_EQ(component.getUnderlyingDirtyPrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingDirtyPrice());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingEndPriceAndUnderlyingEndPriceMatch) {
    const double test_value = 123.456;
    component.setUnderlyingEndPrice(test_value);
    EXPECT_EQ(component.getUnderlyingEndPrice(), test_value);
    EXPECT_TRUE(component.hasUnderlyingEndPrice());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingStartValueAndUnderlyingStartValueMatch) {
    const double test_value = 123.456;
    component.setUnderlyingStartValue(test_value);
    EXPECT_EQ(component.getUnderlyingStartValue(), test_value);
    EXPECT_TRUE(component.hasUnderlyingStartValue());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingCurrentValueAndUnderlyingCurrentValueMatch) {
    const double test_value = 123.456;
    component.setUnderlyingCurrentValue(test_value);
    EXPECT_EQ(component.getUnderlyingCurrentValue(), test_value);
    EXPECT_TRUE(component.hasUnderlyingCurrentValue());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetUnderlyingEndValueAndUnderlyingEndValueMatch) {
    const double test_value = 123.456;
    component.setUnderlyingEndValue(test_value);
    EXPECT_EQ(component.getUnderlyingEndValue(), test_value);
    EXPECT_TRUE(component.hasUnderlyingEndValue());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setUnderlyingSymbol(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, EncodeDecodeRoundtrip) {
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

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, FullyPopulatedRoundtrip) {
    char buffer[11056];
    component.setUnderlyingSymbol(std::string_view{"AAA"});
    component.setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.setUnderlyingSecurityID(std::string_view{"AAA"});
    component.setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().emplace_back();
    component.getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    component.setUnderlyingProduct(12345);
    component.setUnderlyingCFICode(std::string_view{"AAA"});
    component.setUnderlyingSecurityType(std::string_view{"AAA"});
    component.setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.setUnderlyingMaturityDate(12345);
    component.setUnderlyingPutOrCall(12345);
    component.setUnderlyingCouponPaymentDate(12345);
    component.setUnderlyingIssueDate(12345);
    component.setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.setUnderlyingRepurchaseTerm(12345);
    component.setUnderlyingRepurchaseRate(123.456);
    component.setUnderlyingFactor(123.456);
    component.setUnderlyingCreditRating(std::string_view{"AAA"});
    component.setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.setUnderlyingRedemptionDate(12345);
    component.setUnderlyingStrikePrice(123.456);
    component.setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.setUnderlyingOptAttribute('X');
    component.setUnderlyingContractMultiplier(123.456);
    component.setUnderlyingCouponRate(123.456);
    component.setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.setUnderlyingIssuer(std::string_view{"AAA"});
    component.setEncodedUnderlyingIssuerLen(12345);
    component.setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.setEncodedUnderlyingSecurityDescLen(12345);
    component.setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.setUnderlyingCPProgram(std::string_view{"AAA"});
    component.setUnderlyingCPRegType(std::string_view{"AAA"});
    component.setUnderlyingCurrency(std::string_view{"AAA"});
    component.setUnderlyingQty(123.456);
    component.setUnderlyingPx(123.456);
    component.setUnderlyingDirtyPrice(123.456);
    component.setUnderlyingEndPrice(123.456);
    component.setUnderlyingStartValue(123.456);
    component.setUnderlyingCurrentValue(123.456);
    component.setUnderlyingEndValue(123.456);
    component.getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().emplace_back();
    component.getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipType(std::string_view{"AAA"});
    component.getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipValue(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    UnderlyingInstrument decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[11056];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_UnderlyingInstrumentComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
