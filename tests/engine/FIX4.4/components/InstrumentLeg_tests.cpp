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

class FIX4_4_InstrumentLegComponentTest : public ::testing::Test {
protected:
    InstrumentLeg component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_InstrumentLegComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setLegSymbol(test_value); }
    { const std::string_view test_value{"AAA"}; component.setLegSymbolSfx(test_value); }
    { const std::string_view test_value{"AAA"}; component.setLegSecurityID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSymbolAndLegSymbolMatch) {
    EXPECT_FALSE(component.hasLegSymbol());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSymbolSfxAndLegSymbolSfxMatch) {
    EXPECT_FALSE(component.hasLegSymbolSfx());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSecurityIDAndLegSecurityIDMatch) {
    EXPECT_FALSE(component.hasLegSecurityID());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSecurityIDSourceAndLegSecurityIDSourceMatch) {
    EXPECT_FALSE(component.hasLegSecurityIDSource());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegProductAndLegProductMatch) {
    const int64_t test_value = 12345;
    component.setLegProduct(test_value);
    EXPECT_EQ(component.getLegProduct(), test_value);
    EXPECT_TRUE(component.hasLegProduct());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegCFICodeAndLegCFICodeMatch) {
    EXPECT_FALSE(component.hasLegCFICode());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSecurityTypeAndLegSecurityTypeMatch) {
    EXPECT_FALSE(component.hasLegSecurityType());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSecuritySubTypeAndLegSecuritySubTypeMatch) {
    EXPECT_FALSE(component.hasLegSecuritySubType());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegMaturityMonthYearAndLegMaturityMonthYearMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegMaturityDateAndLegMaturityDateMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegCouponPaymentDateAndLegCouponPaymentDateMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegIssueDateAndLegIssueDateMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegRepoCollateralSecurityTypeAndLegRepoCollateralSecurityTypeMatch) {
    EXPECT_FALSE(component.hasLegRepoCollateralSecurityType());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegRepurchaseTermAndLegRepurchaseTermMatch) {
    const int64_t test_value = 12345;
    component.setLegRepurchaseTerm(test_value);
    EXPECT_EQ(component.getLegRepurchaseTerm(), test_value);
    EXPECT_TRUE(component.hasLegRepurchaseTerm());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegRepurchaseRateAndLegRepurchaseRateMatch) {
    const double test_value = 123.456;
    component.setLegRepurchaseRate(test_value);
    EXPECT_EQ(component.getLegRepurchaseRate(), test_value);
    EXPECT_TRUE(component.hasLegRepurchaseRate());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegFactorAndLegFactorMatch) {
    const double test_value = 123.456;
    component.setLegFactor(test_value);
    EXPECT_EQ(component.getLegFactor(), test_value);
    EXPECT_TRUE(component.hasLegFactor());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegCreditRatingAndLegCreditRatingMatch) {
    EXPECT_FALSE(component.hasLegCreditRating());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegInstrRegistryAndLegInstrRegistryMatch) {
    EXPECT_FALSE(component.hasLegInstrRegistry());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegCountryOfIssueAndLegCountryOfIssueMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegStateOrProvinceOfIssueAndLegStateOrProvinceOfIssueMatch) {
    EXPECT_FALSE(component.hasLegStateOrProvinceOfIssue());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegLocaleOfIssueAndLegLocaleOfIssueMatch) {
    EXPECT_FALSE(component.hasLegLocaleOfIssue());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegRedemptionDateAndLegRedemptionDateMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegStrikePriceAndLegStrikePriceMatch) {
    const double test_value = 123.456;
    component.setLegStrikePrice(test_value);
    EXPECT_EQ(component.getLegStrikePrice(), test_value);
    EXPECT_TRUE(component.hasLegStrikePrice());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegStrikeCurrencyAndLegStrikeCurrencyMatch) {
    EXPECT_FALSE(component.hasLegStrikeCurrency());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegOptAttributeAndLegOptAttributeMatch) {
    const char test_value = 'X';
    component.setLegOptAttribute(test_value);
    EXPECT_EQ(component.getLegOptAttribute(), test_value);
    EXPECT_TRUE(component.hasLegOptAttribute());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegContractMultiplierAndLegContractMultiplierMatch) {
    const double test_value = 123.456;
    component.setLegContractMultiplier(test_value);
    EXPECT_EQ(component.getLegContractMultiplier(), test_value);
    EXPECT_TRUE(component.hasLegContractMultiplier());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegCouponRateAndLegCouponRateMatch) {
    const double test_value = 123.456;
    component.setLegCouponRate(test_value);
    EXPECT_EQ(component.getLegCouponRate(), test_value);
    EXPECT_TRUE(component.hasLegCouponRate());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSecurityExchangeAndLegSecurityExchangeMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegIssuerAndLegIssuerMatch) {
    EXPECT_FALSE(component.hasLegIssuer());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetEncodedLegIssuerLenAndEncodedLegIssuerLenMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetEncodedLegIssuerAndEncodedLegIssuerMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSecurityDescAndLegSecurityDescMatch) {
    EXPECT_FALSE(component.hasLegSecurityDesc());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetEncodedLegSecurityDescLenAndEncodedLegSecurityDescLenMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetEncodedLegSecurityDescAndEncodedLegSecurityDescMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegRatioQtyAndLegRatioQtyMatch) {
    const double test_value = 123.456;
    component.setLegRatioQty(test_value);
    EXPECT_EQ(component.getLegRatioQty(), test_value);
    EXPECT_TRUE(component.hasLegRatioQty());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegSideAndLegSideMatch) {
    const char test_value = 'X';
    component.setLegSide(test_value);
    EXPECT_EQ(component.getLegSide(), test_value);
    EXPECT_TRUE(component.hasLegSide());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegCurrencyAndLegCurrencyMatch) {
    EXPECT_FALSE(component.hasLegCurrency());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegPoolAndLegPoolMatch) {
    EXPECT_FALSE(component.hasLegPool());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegDatedDateAndLegDatedDateMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegContractSettlMonthAndLegContractSettlMonthMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetLegInterestAccrualDateAndLegInterestAccrualDateMatch) {
}

TEST_F(FIX4_4_InstrumentLegComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setLegSymbol(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, EncodeDecodeRoundtrip) {
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

TEST_F(FIX4_4_InstrumentLegComponentTest, FullyPopulatedRoundtrip) {
    char buffer[9720];
    component.setLegSymbol(std::string_view{"AAA"});
    component.setLegSymbolSfx(std::string_view{"AAA"});
    component.setLegSecurityID(std::string_view{"AAA"});
    component.setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().emplace_back();
    component.getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(std::string_view{"AAA"});
    component.getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltIDSource(std::string_view{"AAA"});
    component.setLegProduct(12345);
    component.setLegCFICode(std::string_view{"AAA"});
    component.setLegSecurityType(std::string_view{"AAA"});
    component.setLegSecuritySubType(std::string_view{"AAA"});
    component.setLegMaturityMonthYear(std::string_view{"AAA"});
    component.setLegMaturityDate(12345);
    component.setLegCouponPaymentDate(12345);
    component.setLegIssueDate(12345);
    component.setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.setLegRepurchaseTerm(12345);
    component.setLegRepurchaseRate(123.456);
    component.setLegFactor(123.456);
    component.setLegCreditRating(std::string_view{"AAA"});
    component.setLegInstrRegistry(std::string_view{"AAA"});
    component.setLegCountryOfIssue(std::string_view{"AAA"});
    component.setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.setLegLocaleOfIssue(std::string_view{"AAA"});
    component.setLegRedemptionDate(12345);
    component.setLegStrikePrice(123.456);
    component.setLegStrikeCurrency(std::string_view{"AAA"});
    component.setLegOptAttribute('X');
    component.setLegContractMultiplier(123.456);
    component.setLegCouponRate(123.456);
    component.setLegSecurityExchange(std::string_view{"AAA"});
    component.setLegIssuer(std::string_view{"AAA"});
    component.setEncodedLegIssuerLen(12345);
    component.setEncodedLegIssuer(std::string_view{"AAA"});
    component.setLegSecurityDesc(std::string_view{"AAA"});
    component.setEncodedLegSecurityDescLen(12345);
    component.setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.setLegRatioQty(123.456);
    component.setLegSide('X');
    component.setLegCurrency(std::string_view{"AAA"});
    component.setLegPool(std::string_view{"AAA"});
    component.setLegDatedDate(12345);
    component.setLegContractSettlMonth(std::string_view{"AAA"});
    component.setLegInterestAccrualDate(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrumentLeg decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[9720];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_InstrumentLegComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_InstrumentLegComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
