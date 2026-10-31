// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDIncGrp::NoMDEntries (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDIncGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_MDIncGrp_NoMDEntriesComponentTest : public ::testing::Test {
protected:
    MDIncGrp::NoMDEntries component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const char test_value = 'X'; component.setMDUpdateAction(test_value); }
    { const char test_value = 'X'; component.setDeleteReason(test_value); }
    { const char test_value = 'X'; component.setMDEntryType(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDUpdateActionAndMDUpdateActionMatch) {
    const char test_value = 'X';
    component.setMDUpdateAction(test_value);
    EXPECT_EQ(component.getMDUpdateAction(), test_value);
    EXPECT_TRUE(component.hasMDUpdateAction());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetDeleteReasonAndDeleteReasonMatch) {
    const char test_value = 'X';
    component.setDeleteReason(test_value);
    EXPECT_EQ(component.getDeleteReason(), test_value);
    EXPECT_TRUE(component.hasDeleteReason());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryTypeAndMDEntryTypeMatch) {
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    EXPECT_EQ(component.getMDEntryType(), test_value);
    EXPECT_TRUE(component.hasMDEntryType());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryIDAndMDEntryIDMatch) {
    EXPECT_FALSE(component.hasMDEntryID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryRefIDAndMDEntryRefIDMatch) {
    EXPECT_FALSE(component.hasMDEntryRefID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetFinancialStatusAndFinancialStatusMatch) {
    EXPECT_FALSE(component.hasFinancialStatus());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetCorporateActionAndCorporateActionMatch) {
    EXPECT_FALSE(component.hasCorporateAction());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryPxAndMDEntryPxMatch) {
    const double test_value = 123.456;
    component.setMDEntryPx(test_value);
    EXPECT_EQ(component.getMDEntryPx(), test_value);
    EXPECT_TRUE(component.hasMDEntryPx());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntrySizeAndMDEntrySizeMatch) {
    const double test_value = 123.456;
    component.setMDEntrySize(test_value);
    EXPECT_EQ(component.getMDEntrySize(), test_value);
    EXPECT_TRUE(component.hasMDEntrySize());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryDateAndMDEntryDateMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryTimeAndMDEntryTimeMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetTickDirectionAndTickDirectionMatch) {
    const char test_value = 'X';
    component.setTickDirection(test_value);
    EXPECT_EQ(component.getTickDirection(), test_value);
    EXPECT_TRUE(component.hasTickDirection());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDMktAndMDMktMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetTradingSessionIDAndTradingSessionIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetTradingSessionSubIDAndTradingSessionSubIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionSubID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetQuoteConditionAndQuoteConditionMatch) {
    EXPECT_FALSE(component.hasQuoteCondition());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetTradeConditionAndTradeConditionMatch) {
    EXPECT_FALSE(component.hasTradeCondition());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryOriginatorAndMDEntryOriginatorMatch) {
    EXPECT_FALSE(component.hasMDEntryOriginator());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetLocationIDAndLocationIDMatch) {
    EXPECT_FALSE(component.hasLocationID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetDeskIDAndDeskIDMatch) {
    EXPECT_FALSE(component.hasDeskID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetOpenCloseSettlFlagAndOpenCloseSettlFlagMatch) {
    EXPECT_FALSE(component.hasOpenCloseSettlFlag());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetExecInstAndExecInstMatch) {
    EXPECT_FALSE(component.hasExecInst());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetSellerDaysAndSellerDaysMatch) {
    const int64_t test_value = 12345;
    component.setSellerDays(test_value);
    EXPECT_EQ(component.getSellerDays(), test_value);
    EXPECT_TRUE(component.hasSellerDays());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetOrderIDAndOrderIDMatch) {
    EXPECT_FALSE(component.hasOrderID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetQuoteEntryIDAndQuoteEntryIDMatch) {
    EXPECT_FALSE(component.hasQuoteEntryID());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryBuyerAndMDEntryBuyerMatch) {
    EXPECT_FALSE(component.hasMDEntryBuyer());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntrySellerAndMDEntrySellerMatch) {
    EXPECT_FALSE(component.hasMDEntrySeller());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetNumberOfOrdersAndNumberOfOrdersMatch) {
    const int64_t test_value = 12345;
    component.setNumberOfOrders(test_value);
    EXPECT_EQ(component.getNumberOfOrders(), test_value);
    EXPECT_TRUE(component.hasNumberOfOrders());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetMDEntryPositionNoAndMDEntryPositionNoMatch) {
    const int64_t test_value = 12345;
    component.setMDEntryPositionNo(test_value);
    EXPECT_EQ(component.getMDEntryPositionNo(), test_value);
    EXPECT_TRUE(component.hasMDEntryPositionNo());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetScopeAndScopeMatch) {
    EXPECT_FALSE(component.hasScope());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetPriceDeltaAndPriceDeltaMatch) {
    const double test_value = 123.456;
    component.setPriceDelta(test_value);
    EXPECT_EQ(component.getPriceDelta(), test_value);
    EXPECT_TRUE(component.hasPriceDelta());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetNetChgPrevDayAndNetChgPrevDayMatch) {
    const double test_value = 123.456;
    component.setNetChgPrevDay(test_value);
    EXPECT_EQ(component.getNetChgPrevDay(), test_value);
    EXPECT_TRUE(component.hasNetChgPrevDay());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const char test_value = 'X'; component.setMDUpdateAction(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2292];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setMDUpdateAction(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    MDIncGrp::NoMDEntries decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getMDUpdateAction(), component.getMDUpdateAction());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, FullyPopulatedRoundtrip) {
    char buffer[9168];
    component.setMDUpdateAction('X');
    component.setDeleteReason('X');
    component.setMDEntryType('X');
    component.setMDEntryID(std::string_view{"AAA"});
    component.setMDEntryRefID(std::string_view{"AAA"});
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
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().emplace_back();
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().emplace_back();
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().emplace_back();
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipValue(std::string_view{"AAA"});
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
    component.setFinancialStatus(std::string_view{"AAA"});
    component.setCorporateAction(std::string_view{"AAA"});
    component.setMDEntryPx(123.456);
    component.setCurrency(std::string_view{"AAA"});
    component.setMDEntrySize(123.456);
    component.setMDEntryDate(12345);
    component.setMDEntryTime(12345);
    component.setTickDirection('X');
    component.setMDMkt(std::string_view{"AAA"});
    component.setTradingSessionID(std::string_view{"AAA"});
    component.setTradingSessionSubID(std::string_view{"AAA"});
    component.setQuoteCondition(std::string_view{"AAA"});
    component.setTradeCondition(std::string_view{"AAA"});
    component.setMDEntryOriginator(std::string_view{"AAA"});
    component.setLocationID(std::string_view{"AAA"});
    component.setDeskID(std::string_view{"AAA"});
    component.setOpenCloseSettlFlag(std::string_view{"AAA"});
    component.setTimeInForce('X');
    component.setExpireDate(12345);
    component.setExpireTime(12345);
    component.setMinQty(123.456);
    component.setExecInst(std::string_view{"AAA"});
    component.setSellerDays(12345);
    component.setOrderID(std::string_view{"AAA"});
    component.setQuoteEntryID(std::string_view{"AAA"});
    component.setMDEntryBuyer(std::string_view{"AAA"});
    component.setMDEntrySeller(std::string_view{"AAA"});
    component.setNumberOfOrders(12345);
    component.setMDEntryPositionNo(12345);
    component.setScope(std::string_view{"AAA"});
    component.setPriceDelta(123.456);
    component.setNetChgPrevDay(123.456);
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    MDIncGrp::NoMDEntries decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[9168];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_MDIncGrp_NoMDEntriesComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
