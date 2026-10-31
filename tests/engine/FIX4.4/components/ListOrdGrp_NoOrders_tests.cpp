// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ListOrdGrp::NoOrders (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ListOrdGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_ListOrdGrp_NoOrdersComponentTest : public ::testing::Test {
protected:
    ListOrdGrp::NoOrders component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryClOrdID(test_value); }
    { const int64_t test_value = 12345; component.setListSeqNo(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetListSeqNoAndListSeqNoMatch) {
    const int64_t test_value = 12345;
    component.setListSeqNo(test_value);
    EXPECT_EQ(component.getListSeqNo(), test_value);
    EXPECT_TRUE(component.hasListSeqNo());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetClOrdLinkIDAndClOrdLinkIDMatch) {
    EXPECT_FALSE(component.hasClOrdLinkID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSettlInstModeAndSettlInstModeMatch) {
    const char test_value = 'X';
    component.setSettlInstMode(test_value);
    EXPECT_EQ(component.getSettlInstMode(), test_value);
    EXPECT_TRUE(component.hasSettlInstMode());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetAccountAndAccountMatch) {
    EXPECT_FALSE(component.hasAccount());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetDayBookingInstAndDayBookingInstMatch) {
    const char test_value = 'X';
    component.setDayBookingInst(test_value);
    EXPECT_EQ(component.getDayBookingInst(), test_value);
    EXPECT_TRUE(component.hasDayBookingInst());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetBookingUnitAndBookingUnitMatch) {
    const char test_value = 'X';
    component.setBookingUnit(test_value);
    EXPECT_EQ(component.getBookingUnit(), test_value);
    EXPECT_TRUE(component.hasBookingUnit());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetAllocIDAndAllocIDMatch) {
    EXPECT_FALSE(component.hasAllocID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetPreallocMethodAndPreallocMethodMatch) {
    const char test_value = 'X';
    component.setPreallocMethod(test_value);
    EXPECT_EQ(component.getPreallocMethod(), test_value);
    EXPECT_TRUE(component.hasPreallocMethod());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSettlTypeAndSettlTypeMatch) {
    const char test_value = 'X';
    component.setSettlType(test_value);
    EXPECT_EQ(component.getSettlType(), test_value);
    EXPECT_TRUE(component.hasSettlType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetCashMarginAndCashMarginMatch) {
    const char test_value = 'X';
    component.setCashMargin(test_value);
    EXPECT_EQ(component.getCashMargin(), test_value);
    EXPECT_TRUE(component.hasCashMargin());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetClearingFeeIndicatorAndClearingFeeIndicatorMatch) {
    EXPECT_FALSE(component.hasClearingFeeIndicator());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetHandlInstAndHandlInstMatch) {
    const char test_value = 'X';
    component.setHandlInst(test_value);
    EXPECT_EQ(component.getHandlInst(), test_value);
    EXPECT_TRUE(component.hasHandlInst());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetExecInstAndExecInstMatch) {
    EXPECT_FALSE(component.hasExecInst());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetMaxFloorAndMaxFloorMatch) {
    const double test_value = 123.456;
    component.setMaxFloor(test_value);
    EXPECT_EQ(component.getMaxFloor(), test_value);
    EXPECT_TRUE(component.hasMaxFloor());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetExDestinationAndExDestinationMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetProcessCodeAndProcessCodeMatch) {
    const char test_value = 'X';
    component.setProcessCode(test_value);
    EXPECT_EQ(component.getProcessCode(), test_value);
    EXPECT_TRUE(component.hasProcessCode());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetPrevClosePxAndPrevClosePxMatch) {
    const double test_value = 123.456;
    component.setPrevClosePx(test_value);
    EXPECT_EQ(component.getPrevClosePx(), test_value);
    EXPECT_TRUE(component.hasPrevClosePx());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSideValueIndAndSideValueIndMatch) {
    const int64_t test_value = 12345;
    component.setSideValueInd(test_value);
    EXPECT_EQ(component.getSideValueInd(), test_value);
    EXPECT_TRUE(component.hasSideValueInd());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetLocateReqdAndLocateReqdMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTransactTimeAndTransactTimeMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetQtyTypeAndQtyTypeMatch) {
    const int64_t test_value = 12345;
    component.setQtyType(test_value);
    EXPECT_EQ(component.getQtyType(), test_value);
    EXPECT_TRUE(component.hasQtyType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetPriceTypeAndPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setPriceType(test_value);
    EXPECT_EQ(component.getPriceType(), test_value);
    EXPECT_TRUE(component.hasPriceType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetPriceAndPriceMatch) {
    const double test_value = 123.456;
    component.setPrice(test_value);
    EXPECT_EQ(component.getPrice(), test_value);
    EXPECT_TRUE(component.hasPrice());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetStopPxAndStopPxMatch) {
    const double test_value = 123.456;
    component.setStopPx(test_value);
    EXPECT_EQ(component.getStopPx(), test_value);
    EXPECT_TRUE(component.hasStopPx());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetComplianceIDAndComplianceIDMatch) {
    EXPECT_FALSE(component.hasComplianceID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetIOIIDAndIOIIDMatch) {
    EXPECT_FALSE(component.hasIOIID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetQuoteIDAndQuoteIDMatch) {
    EXPECT_FALSE(component.hasQuoteID());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetEffectiveTimeAndEffectiveTimeMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetGTBookingInstAndGTBookingInstMatch) {
    const int64_t test_value = 12345;
    component.setGTBookingInst(test_value);
    EXPECT_EQ(component.getGTBookingInst(), test_value);
    EXPECT_TRUE(component.hasGTBookingInst());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetOrderRestrictionsAndOrderRestrictionsMatch) {
    EXPECT_FALSE(component.hasOrderRestrictions());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetForexReqAndForexReqMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSettlCurrencyAndSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasSettlCurrency());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetBookingTypeAndBookingTypeMatch) {
    const int64_t test_value = 12345;
    component.setBookingType(test_value);
    EXPECT_EQ(component.getBookingType(), test_value);
    EXPECT_TRUE(component.hasBookingType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSettlDate2AndSettlDate2Match) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetOrderQty2AndOrderQty2Match) {
    const double test_value = 123.456;
    component.setOrderQty2(test_value);
    EXPECT_EQ(component.getOrderQty2(), test_value);
    EXPECT_TRUE(component.hasOrderQty2());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetPrice2AndPrice2Match) {
    const double test_value = 123.456;
    component.setPrice2(test_value);
    EXPECT_EQ(component.getPrice2(), test_value);
    EXPECT_TRUE(component.hasPrice2());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetPositionEffectAndPositionEffectMatch) {
    const char test_value = 'X';
    component.setPositionEffect(test_value);
    EXPECT_EQ(component.getPositionEffect(), test_value);
    EXPECT_TRUE(component.hasPositionEffect());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetCoveredOrUncoveredAndCoveredOrUncoveredMatch) {
    const int64_t test_value = 12345;
    component.setCoveredOrUncovered(test_value);
    EXPECT_EQ(component.getCoveredOrUncovered(), test_value);
    EXPECT_TRUE(component.hasCoveredOrUncovered());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetMaxShowAndMaxShowMatch) {
    const double test_value = 123.456;
    component.setMaxShow(test_value);
    EXPECT_EQ(component.getMaxShow(), test_value);
    EXPECT_TRUE(component.hasMaxShow());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTargetStrategyAndTargetStrategyMatch) {
    const int64_t test_value = 12345;
    component.setTargetStrategy(test_value);
    EXPECT_EQ(component.getTargetStrategy(), test_value);
    EXPECT_TRUE(component.hasTargetStrategy());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetTargetStrategyParametersAndTargetStrategyParametersMatch) {
    EXPECT_FALSE(component.hasTargetStrategyParameters());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetParticipationRateAndParticipationRateMatch) {
    const double test_value = 123.456;
    component.setParticipationRate(test_value);
    EXPECT_EQ(component.getParticipationRate(), test_value);
    EXPECT_TRUE(component.hasParticipationRate());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetDesignationAndDesignationMatch) {
    EXPECT_FALSE(component.hasDesignation());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2866];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setListSeqNo(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    ListOrdGrp::NoOrders decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getListSeqNo(), component.getListSeqNo());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, FullyPopulatedRoundtrip) {
    char buffer[11464];
    component.setClOrdID(std::string_view{"AAA"});
    component.setSecondaryClOrdID(std::string_view{"AAA"});
    component.setListSeqNo(12345);
    component.setClOrdLinkID(std::string_view{"AAA"});
    component.setSettlInstMode('X');
    component.getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.setTradeOriginationDate(12345);
    component.setTradeDate(12345);
    component.setAccount(std::string_view{"AAA"});
    component.setAcctIDSource(12345);
    component.setAccountType(12345);
    component.setDayBookingInst('X');
    component.setBookingUnit('X');
    component.setAllocID(std::string_view{"AAA"});
    component.setPreallocMethod('X');
    component.getRefPreAllocGrp().getRefNoAllocsGroup().emplace_back();
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocAccount(std::string_view{"AAA"});
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocAcctIDSource(12345);
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocSettlCurrency(std::string_view{"AAA"});
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().setIndividualAllocID(std::string_view{"AAA"});
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocQty(123.456);
    component.setSettlType('X');
    component.setSettlDate(12345);
    component.setCashMargin('X');
    component.setClearingFeeIndicator(std::string_view{"AAA"});
    component.setHandlInst('X');
    component.setExecInst(std::string_view{"AAA"});
    component.setMinQty(123.456);
    component.setMaxFloor(123.456);
    component.setExDestination(std::string_view{"AAA"});
    component.getRefTrdgSesGrp().getRefNoTradingSessionsGroup().emplace_back();
    component.getRefTrdgSesGrp().getRefNoTradingSessionsGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefTrdgSesGrp().getRefNoTradingSessionsGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.setProcessCode('X');
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
    component.setPrevClosePx(123.456);
    component.setSide('X');
    component.setSideValueInd(12345);
    component.setLocateReqd(true);
    component.setTransactTime(12345);
    component.getRefStipulations().getRefNoStipulationsGroup().emplace_back();
    component.getRefStipulations().getRefNoStipulationsGroup().back().setStipulationType(std::string_view{"AAA"});
    component.getRefStipulations().getRefNoStipulationsGroup().back().setStipulationValue(std::string_view{"AAA"});
    component.setQtyType(12345);
    component.getRefOrderQtyData().setOrderQty(123.456);
    component.getRefOrderQtyData().setCashOrderQty(123.456);
    component.getRefOrderQtyData().setOrderPercent(123.456);
    component.getRefOrderQtyData().setRoundingDirection('X');
    component.getRefOrderQtyData().setRoundingModulus(123.456);
    component.setOrdType('X');
    component.setPriceType(12345);
    component.setPrice(123.456);
    component.setStopPx(123.456);
    component.getRefSpreadOrBenchmarkCurveData().setSpread(123.456);
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkCurveCurrency(std::string_view{"AAA"});
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkCurveName(std::string_view{"AAA"});
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkCurvePoint(std::string_view{"AAA"});
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkPrice(123.456);
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkPriceType(12345);
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkSecurityID(std::string_view{"AAA"});
    component.getRefSpreadOrBenchmarkCurveData().setBenchmarkSecurityIDSource(std::string_view{"AAA"});
    component.getRefYieldData().setYieldType(std::string_view{"AAA"});
    component.getRefYieldData().setYield(123.456);
    component.getRefYieldData().setYieldCalcDate(12345);
    component.getRefYieldData().setYieldRedemptionDate(12345);
    component.getRefYieldData().setYieldRedemptionPrice(123.456);
    component.getRefYieldData().setYieldRedemptionPriceType(12345);
    component.setCurrency(std::string_view{"AAA"});
    component.setComplianceID(std::string_view{"AAA"});
    component.setSolicitedFlag(true);
    component.setIOIID(std::string_view{"AAA"});
    component.setQuoteID(std::string_view{"AAA"});
    component.setTimeInForce('X');
    component.setEffectiveTime(12345);
    component.setExpireDate(12345);
    component.setExpireTime(12345);
    component.setGTBookingInst(12345);
    component.getRefCommissionData().setCommission(123.456);
    component.getRefCommissionData().setCommType('X');
    component.getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefCommissionData().setFundRenewWaiv('X');
    component.setOrderCapacity('X');
    component.setOrderRestrictions(std::string_view{"AAA"});
    component.setCustOrderCapacity(12345);
    component.setForexReq(true);
    component.setSettlCurrency(std::string_view{"AAA"});
    component.setBookingType(12345);
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    component.setSettlDate2(12345);
    component.setOrderQty2(123.456);
    component.setPrice2(123.456);
    component.setPositionEffect('X');
    component.setCoveredOrUncovered(12345);
    component.setMaxShow(123.456);
    component.getRefPegInstructions().setPegOffsetValue(123.456);
    component.getRefPegInstructions().setPegMoveType(12345);
    component.getRefPegInstructions().setPegOffsetType(12345);
    component.getRefPegInstructions().setPegLimitType(12345);
    component.getRefPegInstructions().setPegRoundDirection(12345);
    component.getRefPegInstructions().setPegScope(12345);
    component.getRefDiscretionInstructions().setDiscretionInst('X');
    component.getRefDiscretionInstructions().setDiscretionOffsetValue(123.456);
    component.getRefDiscretionInstructions().setDiscretionMoveType(12345);
    component.getRefDiscretionInstructions().setDiscretionOffsetType(12345);
    component.getRefDiscretionInstructions().setDiscretionLimitType(12345);
    component.getRefDiscretionInstructions().setDiscretionRoundDirection(12345);
    component.getRefDiscretionInstructions().setDiscretionScope(12345);
    component.setTargetStrategy(12345);
    component.setTargetStrategyParameters(std::string_view{"AAA"});
    component.setParticipationRate(123.456);
    component.setDesignation(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    ListOrdGrp::NoOrders decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[11464];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
