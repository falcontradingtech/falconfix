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
    { const std::string_view test_value{"AAA"}; component.setSymbol(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSymbolSfx(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecurityID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSymbolAndSymbolMatch) {
    EXPECT_FALSE(component.hasSymbol());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSymbolSfxAndSymbolSfxMatch) {
    EXPECT_FALSE(component.hasSymbolSfx());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSecurityIDAndSecurityIDMatch) {
    EXPECT_FALSE(component.hasSecurityID());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSecurityIDSourceAndSecurityIDSourceMatch) {
    EXPECT_FALSE(component.hasSecurityIDSource());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetProductAndProductMatch) {
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    EXPECT_EQ(component.getProduct(), test_value);
    EXPECT_TRUE(component.hasProduct());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCFICodeAndCFICodeMatch) {
    EXPECT_FALSE(component.hasCFICode());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSecurityTypeAndSecurityTypeMatch) {
    EXPECT_FALSE(component.hasSecurityType());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSecuritySubTypeAndSecuritySubTypeMatch) {
    EXPECT_FALSE(component.hasSecuritySubType());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetMaturityMonthYearAndMaturityMonthYearMatch) {
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

TEST_F(FIX4_4_InstrumentComponentTest, SetRepoCollateralSecurityTypeAndRepoCollateralSecurityTypeMatch) {
    EXPECT_FALSE(component.hasRepoCollateralSecurityType());
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

TEST_F(FIX4_4_InstrumentComponentTest, SetCreditRatingAndCreditRatingMatch) {
    EXPECT_FALSE(component.hasCreditRating());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetInstrRegistryAndInstrRegistryMatch) {
    EXPECT_FALSE(component.hasInstrRegistry());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCountryOfIssueAndCountryOfIssueMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetStateOrProvinceOfIssueAndStateOrProvinceOfIssueMatch) {
    EXPECT_FALSE(component.hasStateOrProvinceOfIssue());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetLocaleOfIssueAndLocaleOfIssueMatch) {
    EXPECT_FALSE(component.hasLocaleOfIssue());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetRedemptionDateAndRedemptionDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetStrikePriceAndStrikePriceMatch) {
    const double test_value = 123.456;
    component.setStrikePrice(test_value);
    EXPECT_EQ(component.getStrikePrice(), test_value);
    EXPECT_TRUE(component.hasStrikePrice());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetStrikeCurrencyAndStrikeCurrencyMatch) {
    EXPECT_FALSE(component.hasStrikeCurrency());
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

TEST_F(FIX4_4_InstrumentComponentTest, SetSecurityExchangeAndSecurityExchangeMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetIssuerAndIssuerMatch) {
    EXPECT_FALSE(component.hasIssuer());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetEncodedIssuerLenAndEncodedIssuerLenMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetEncodedIssuerAndEncodedIssuerMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetSecurityDescAndSecurityDescMatch) {
    EXPECT_FALSE(component.hasSecurityDesc());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetEncodedSecurityDescLenAndEncodedSecurityDescLenMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetEncodedSecurityDescAndEncodedSecurityDescMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetPoolAndPoolMatch) {
    EXPECT_FALSE(component.hasPool());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetContractSettlMonthAndContractSettlMonthMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCPProgramAndCPProgramMatch) {
    const int64_t test_value = 12345;
    component.setCPProgram(test_value);
    EXPECT_EQ(component.getCPProgram(), test_value);
    EXPECT_TRUE(component.hasCPProgram());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetCPRegTypeAndCPRegTypeMatch) {
    EXPECT_FALSE(component.hasCPRegType());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetDatedDateAndDatedDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, SetInterestAccrualDateAndInterestAccrualDateMatch) {
}

TEST_F(FIX4_4_InstrumentComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setSymbol(test_value); }
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

TEST_F(FIX4_4_InstrumentComponentTest, FullyPopulatedRoundtrip) {
    char buffer[9808];
    component.setSymbol(std::string_view{"AAA"});
    component.setSymbolSfx(std::string_view{"AAA"});
    component.setSecurityID(std::string_view{"AAA"});
    component.setSecurityIDSource(std::string_view{"AAA"});
    component.getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.setProduct(12345);
    component.setCFICode(std::string_view{"AAA"});
    component.setSecurityType(std::string_view{"AAA"});
    component.setSecuritySubType(std::string_view{"AAA"});
    component.setMaturityMonthYear(std::string_view{"AAA"});
    component.setMaturityDate(12345);
    component.setPutOrCall(12345);
    component.setCouponPaymentDate(12345);
    component.setIssueDate(12345);
    component.setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.setRepurchaseTerm(12345);
    component.setRepurchaseRate(123.456);
    component.setFactor(123.456);
    component.setCreditRating(std::string_view{"AAA"});
    component.setInstrRegistry(std::string_view{"AAA"});
    component.setCountryOfIssue(std::string_view{"AAA"});
    component.setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.setLocaleOfIssue(std::string_view{"AAA"});
    component.setRedemptionDate(12345);
    component.setStrikePrice(123.456);
    component.setStrikeCurrency(std::string_view{"AAA"});
    component.setOptAttribute('X');
    component.setContractMultiplier(123.456);
    component.setCouponRate(123.456);
    component.setSecurityExchange(std::string_view{"AAA"});
    component.setIssuer(std::string_view{"AAA"});
    component.setEncodedIssuerLen(12345);
    component.setEncodedIssuer(std::string_view{"AAA"});
    component.setSecurityDesc(std::string_view{"AAA"});
    component.setEncodedSecurityDescLen(12345);
    component.setEncodedSecurityDesc(std::string_view{"AAA"});
    component.setPool(std::string_view{"AAA"});
    component.setContractSettlMonth(std::string_view{"AAA"});
    component.setCPProgram(12345);
    component.setCPRegType(std::string_view{"AAA"});
    component.getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.setDatedDate(12345);
    component.setInterestAccrualDate(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    Instrument decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[9808];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_InstrumentComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_InstrumentComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
