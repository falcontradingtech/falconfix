// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotSetAckGrp::NoQuoteSets (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotSetAckGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest : public ::testing::Test {
protected:
    QuotSetAckGrp::NoQuoteSets component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setQuoteSetID(test_value); }
    { const int64_t test_value = 12345; component.setTotNoQuoteEntries(test_value); }
    { const int64_t test_value = 12345; component.setLastFragment(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, SetQuoteSetIDAndQuoteSetIDMatch) {
    EXPECT_FALSE(component.hasQuoteSetID());
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, SetTotNoQuoteEntriesAndTotNoQuoteEntriesMatch) {
    const int64_t test_value = 12345;
    component.setTotNoQuoteEntries(test_value);
    EXPECT_EQ(component.getTotNoQuoteEntries(), test_value);
    EXPECT_TRUE(component.hasTotNoQuoteEntries());
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, SetLastFragmentAndLastFragmentMatch) {
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setQuoteSetID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
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
    QuotSetAckGrp::NoQuoteSets decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getTotNoQuoteEntries(), component.getTotNoQuoteEntries());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setQuoteSetID(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().emplace_back();
    component.getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().emplace_back();
    component.getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipValue(std::string_view{"AAA"});
    component.setTotNoQuoteEntries(12345);
    component.setLastFragment(true);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().emplace_back();
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setQuoteEntryID(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setProduct(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().emplace_back();
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegProduct(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSide('X');
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidPx(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferPx(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidSize(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferSize(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setValidUntilTime(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidSpotRate(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferSpotRate(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidForwardPoints(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferForwardPoints(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setMidPx(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidYield(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setMidYield(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferYield(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setTransactTime(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setSettlDate(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOrdType('X');
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setSettlDate2(12345);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOrderQty2(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidForwardPoints2(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferForwardPoints2(123.456);
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setQuoteEntryRejectReason(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotSetAckGrp::NoQuoteSets decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4096];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_QuotSetAckGrp_NoQuoteSetsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
