// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotEntryAckGrp::NoQuoteEntries (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotEntryAckGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest : public ::testing::Test {
protected:
    QuotEntryAckGrp::NoQuoteEntries component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setQuoteEntryID(test_value); }
    { const double test_value = 123.456; component.setBidPx(test_value); }
    { const double test_value = 123.456; component.setOfferPx(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetQuoteEntryIDAndQuoteEntryIDMatch) {
    EXPECT_FALSE(component.hasQuoteEntryID());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidPxAndBidPxMatch) {
    const double test_value = 123.456;
    component.setBidPx(test_value);
    EXPECT_EQ(component.getBidPx(), test_value);
    EXPECT_TRUE(component.hasBidPx());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferPxAndOfferPxMatch) {
    const double test_value = 123.456;
    component.setOfferPx(test_value);
    EXPECT_EQ(component.getOfferPx(), test_value);
    EXPECT_TRUE(component.hasOfferPx());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidSizeAndBidSizeMatch) {
    const double test_value = 123.456;
    component.setBidSize(test_value);
    EXPECT_EQ(component.getBidSize(), test_value);
    EXPECT_TRUE(component.hasBidSize());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferSizeAndOfferSizeMatch) {
    const double test_value = 123.456;
    component.setOfferSize(test_value);
    EXPECT_EQ(component.getOfferSize(), test_value);
    EXPECT_TRUE(component.hasOfferSize());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetValidUntilTimeAndValidUntilTimeMatch) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidSpotRateAndBidSpotRateMatch) {
    const double test_value = 123.456;
    component.setBidSpotRate(test_value);
    EXPECT_EQ(component.getBidSpotRate(), test_value);
    EXPECT_TRUE(component.hasBidSpotRate());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferSpotRateAndOfferSpotRateMatch) {
    const double test_value = 123.456;
    component.setOfferSpotRate(test_value);
    EXPECT_EQ(component.getOfferSpotRate(), test_value);
    EXPECT_TRUE(component.hasOfferSpotRate());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidForwardPointsAndBidForwardPointsMatch) {
    const double test_value = 123.456;
    component.setBidForwardPoints(test_value);
    EXPECT_EQ(component.getBidForwardPoints(), test_value);
    EXPECT_TRUE(component.hasBidForwardPoints());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferForwardPointsAndOfferForwardPointsMatch) {
    const double test_value = 123.456;
    component.setOfferForwardPoints(test_value);
    EXPECT_EQ(component.getOfferForwardPoints(), test_value);
    EXPECT_TRUE(component.hasOfferForwardPoints());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetMidPxAndMidPxMatch) {
    const double test_value = 123.456;
    component.setMidPx(test_value);
    EXPECT_EQ(component.getMidPx(), test_value);
    EXPECT_TRUE(component.hasMidPx());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidYieldAndBidYieldMatch) {
    const double test_value = 123.456;
    component.setBidYield(test_value);
    EXPECT_EQ(component.getBidYield(), test_value);
    EXPECT_TRUE(component.hasBidYield());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetMidYieldAndMidYieldMatch) {
    const double test_value = 123.456;
    component.setMidYield(test_value);
    EXPECT_EQ(component.getMidYield(), test_value);
    EXPECT_TRUE(component.hasMidYield());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferYieldAndOfferYieldMatch) {
    const double test_value = 123.456;
    component.setOfferYield(test_value);
    EXPECT_EQ(component.getOfferYield(), test_value);
    EXPECT_TRUE(component.hasOfferYield());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetTransactTimeAndTransactTimeMatch) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetTradingSessionIDAndTradingSessionIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionID());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetTradingSessionSubIDAndTradingSessionSubIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionSubID());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetSettlDate2AndSettlDate2Match) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOrderQty2AndOrderQty2Match) {
    const double test_value = 123.456;
    component.setOrderQty2(test_value);
    EXPECT_EQ(component.getOrderQty2(), test_value);
    EXPECT_TRUE(component.hasOrderQty2());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidForwardPoints2AndBidForwardPoints2Match) {
    const double test_value = 123.456;
    component.setBidForwardPoints2(test_value);
    EXPECT_EQ(component.getBidForwardPoints2(), test_value);
    EXPECT_TRUE(component.hasBidForwardPoints2());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferForwardPoints2AndOfferForwardPoints2Match) {
    const double test_value = 123.456;
    component.setOfferForwardPoints2(test_value);
    EXPECT_EQ(component.getOfferForwardPoints2(), test_value);
    EXPECT_TRUE(component.hasOfferForwardPoints2());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetQuoteEntryRejectReasonAndQuoteEntryRejectReasonMatch) {
    const int64_t test_value = 12345;
    component.setQuoteEntryRejectReason(test_value);
    EXPECT_EQ(component.getQuoteEntryRejectReason(), test_value);
    EXPECT_TRUE(component.hasQuoteEntryRejectReason());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setQuoteEntryID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1358];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setBidPx(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    QuotEntryAckGrp::NoQuoteEntries decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getBidPx(), component.getBidPx());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, FullyPopulatedRoundtrip) {
    char buffer[5432];
    component.setQuoteEntryID(std::string_view{"AAA"});
    component.getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefInstrument().setProduct(12345);
    component.getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefInstrument().setMaturityDate(12345);
    component.getRefInstrument().setPutOrCall(12345);
    component.getRefInstrument().setCouponPaymentDate(12345);
    component.getRefInstrument().setIssueDate(12345);
    component.getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefInstrument().setRepurchaseTerm(12345);
    component.getRefInstrument().setRepurchaseRate(123.456);
    component.getRefInstrument().setFactor(123.456);
    component.getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefInstrument().setRedemptionDate(12345);
    component.getRefInstrument().setStrikePrice(123.456);
    component.getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefInstrument().setOptAttribute('X');
    component.getRefInstrument().setContractMultiplier(123.456);
    component.getRefInstrument().setCouponRate(123.456);
    component.getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefInstrument().setCPProgram(12345);
    component.getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefInstrument().setDatedDate(12345);
    component.getRefInstrument().setInterestAccrualDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().emplace_back();
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().emplace_back();
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegProduct(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSide('X');
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    component.setBidPx(123.456);
    component.setOfferPx(123.456);
    component.setBidSize(123.456);
    component.setOfferSize(123.456);
    component.setValidUntilTime(12345);
    component.setBidSpotRate(123.456);
    component.setOfferSpotRate(123.456);
    component.setBidForwardPoints(123.456);
    component.setOfferForwardPoints(123.456);
    component.setMidPx(123.456);
    component.setBidYield(123.456);
    component.setMidYield(123.456);
    component.setOfferYield(123.456);
    component.setTransactTime(12345);
    component.setTradingSessionID(std::string_view{"AAA"});
    component.setTradingSessionSubID(std::string_view{"AAA"});
    component.setSettlDate(12345);
    component.setOrdType('X');
    component.setSettlDate2(12345);
    component.setOrderQty2(123.456);
    component.setBidForwardPoints2(123.456);
    component.setOfferForwardPoints2(123.456);
    component.setCurrency(std::string_view{"AAA"});
    component.setQuoteEntryRejectReason(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotEntryAckGrp::NoQuoteEntries decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[5432];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
