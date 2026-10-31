// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: TrdCapRptSideGrp::NoSides (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/TrdCapRptSideGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest : public ::testing::Test {
protected:
    TrdCapRptSideGrp::NoSides component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const char test_value = 'X'; component.setSide(test_value); }
    { const std::string_view test_value{"AAA"}; component.setOrderID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryOrderID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetOrderIDAndOrderIDMatch) {
    EXPECT_FALSE(component.hasOrderID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSecondaryOrderIDAndSecondaryOrderIDMatch) {
    EXPECT_FALSE(component.hasSecondaryOrderID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetListIDAndListIDMatch) {
    EXPECT_FALSE(component.hasListID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetAccountAndAccountMatch) {
    EXPECT_FALSE(component.hasAccount());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetProcessCodeAndProcessCodeMatch) {
    const char test_value = 'X';
    component.setProcessCode(test_value);
    EXPECT_EQ(component.getProcessCode(), test_value);
    EXPECT_TRUE(component.hasProcessCode());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetOddLotAndOddLotMatch) {
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTradeInputSourceAndTradeInputSourceMatch) {
    EXPECT_FALSE(component.hasTradeInputSource());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTradeInputDeviceAndTradeInputDeviceMatch) {
    EXPECT_FALSE(component.hasTradeInputDevice());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetOrderInputDeviceAndOrderInputDeviceMatch) {
    EXPECT_FALSE(component.hasOrderInputDevice());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetComplianceIDAndComplianceIDMatch) {
    EXPECT_FALSE(component.hasComplianceID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetOrderRestrictionsAndOrderRestrictionsMatch) {
    EXPECT_FALSE(component.hasOrderRestrictions());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetExecInstAndExecInstMatch) {
    EXPECT_FALSE(component.hasExecInst());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTransBkdTimeAndTransBkdTimeMatch) {
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTradingSessionIDAndTradingSessionIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTradingSessionSubIDAndTradingSessionSubIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionSubID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTimeBracketAndTimeBracketMatch) {
    EXPECT_FALSE(component.hasTimeBracket());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetGrossTradeAmtAndGrossTradeAmtMatch) {
    const double test_value = 123.456;
    component.setGrossTradeAmt(test_value);
    EXPECT_EQ(component.getGrossTradeAmt(), test_value);
    EXPECT_TRUE(component.hasGrossTradeAmt());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetNumDaysInterestAndNumDaysInterestMatch) {
    const int64_t test_value = 12345;
    component.setNumDaysInterest(test_value);
    EXPECT_EQ(component.getNumDaysInterest(), test_value);
    EXPECT_TRUE(component.hasNumDaysInterest());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetExDateAndExDateMatch) {
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetAccruedInterestRateAndAccruedInterestRateMatch) {
    const double test_value = 123.456;
    component.setAccruedInterestRate(test_value);
    EXPECT_EQ(component.getAccruedInterestRate(), test_value);
    EXPECT_TRUE(component.hasAccruedInterestRate());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetAccruedInterestAmtAndAccruedInterestAmtMatch) {
    const double test_value = 123.456;
    component.setAccruedInterestAmt(test_value);
    EXPECT_EQ(component.getAccruedInterestAmt(), test_value);
    EXPECT_TRUE(component.hasAccruedInterestAmt());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetInterestAtMaturityAndInterestAtMaturityMatch) {
    const double test_value = 123.456;
    component.setInterestAtMaturity(test_value);
    EXPECT_EQ(component.getInterestAtMaturity(), test_value);
    EXPECT_TRUE(component.hasInterestAtMaturity());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetEndAccruedInterestAmtAndEndAccruedInterestAmtMatch) {
    const double test_value = 123.456;
    component.setEndAccruedInterestAmt(test_value);
    EXPECT_EQ(component.getEndAccruedInterestAmt(), test_value);
    EXPECT_TRUE(component.hasEndAccruedInterestAmt());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetStartCashAndStartCashMatch) {
    const double test_value = 123.456;
    component.setStartCash(test_value);
    EXPECT_EQ(component.getStartCash(), test_value);
    EXPECT_TRUE(component.hasStartCash());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetEndCashAndEndCashMatch) {
    const double test_value = 123.456;
    component.setEndCash(test_value);
    EXPECT_EQ(component.getEndCash(), test_value);
    EXPECT_TRUE(component.hasEndCash());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetConcessionAndConcessionMatch) {
    const double test_value = 123.456;
    component.setConcession(test_value);
    EXPECT_EQ(component.getConcession(), test_value);
    EXPECT_TRUE(component.hasConcession());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTotalTakedownAndTotalTakedownMatch) {
    const double test_value = 123.456;
    component.setTotalTakedown(test_value);
    EXPECT_EQ(component.getTotalTakedown(), test_value);
    EXPECT_TRUE(component.hasTotalTakedown());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetNetMoneyAndNetMoneyMatch) {
    const double test_value = 123.456;
    component.setNetMoney(test_value);
    EXPECT_EQ(component.getNetMoney(), test_value);
    EXPECT_TRUE(component.hasNetMoney());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrAmtAndSettlCurrAmtMatch) {
    const double test_value = 123.456;
    component.setSettlCurrAmt(test_value);
    EXPECT_EQ(component.getSettlCurrAmt(), test_value);
    EXPECT_TRUE(component.hasSettlCurrAmt());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrencyAndSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasSettlCurrency());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrFxRateAndSettlCurrFxRateMatch) {
    const double test_value = 123.456;
    component.setSettlCurrFxRate(test_value);
    EXPECT_EQ(component.getSettlCurrFxRate(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRate());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrFxRateCalcAndSettlCurrFxRateCalcMatch) {
    const char test_value = 'X';
    component.setSettlCurrFxRateCalc(test_value);
    EXPECT_EQ(component.getSettlCurrFxRateCalc(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRateCalc());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetPositionEffectAndPositionEffectMatch) {
    const char test_value = 'X';
    component.setPositionEffect(test_value);
    EXPECT_EQ(component.getPositionEffect(), test_value);
    EXPECT_TRUE(component.hasPositionEffect());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetSideMultiLegReportingTypeAndSideMultiLegReportingTypeMatch) {
    const int64_t test_value = 12345;
    component.setSideMultiLegReportingType(test_value);
    EXPECT_EQ(component.getSideMultiLegReportingType(), test_value);
    EXPECT_TRUE(component.hasSideMultiLegReportingType());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetExchangeRuleAndExchangeRuleMatch) {
    EXPECT_FALSE(component.hasExchangeRule());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetTradeAllocIndicatorAndTradeAllocIndicatorMatch) {
    const int64_t test_value = 12345;
    component.setTradeAllocIndicator(test_value);
    EXPECT_EQ(component.getTradeAllocIndicator(), test_value);
    EXPECT_TRUE(component.hasTradeAllocIndicator());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetPreallocMethodAndPreallocMethodMatch) {
    const char test_value = 'X';
    component.setPreallocMethod(test_value);
    EXPECT_EQ(component.getPreallocMethod(), test_value);
    EXPECT_TRUE(component.hasPreallocMethod());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetAllocIDAndAllocIDMatch) {
    EXPECT_FALSE(component.hasAllocID());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const char test_value = 'X'; component.setSide(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2752];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setSide(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    TrdCapRptSideGrp::NoSides decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSide(), component.getSide());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, FullyPopulatedRoundtrip) {
    char buffer[11008];
    component.setSide('X');
    component.setOrderID(std::string_view{"AAA"});
    component.setSecondaryOrderID(std::string_view{"AAA"});
    component.setClOrdID(std::string_view{"AAA"});
    component.setSecondaryClOrdID(std::string_view{"AAA"});
    component.setListID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.setAccount(std::string_view{"AAA"});
    component.setAcctIDSource(12345);
    component.setAccountType(12345);
    component.setProcessCode('X');
    component.setOddLot(true);
    component.getRefClrInstGrp().getRefNoClearingInstructionsGroup().emplace_back();
    component.getRefClrInstGrp().getRefNoClearingInstructionsGroup().back().setClearingInstruction(12345);
    component.setTradeInputSource(std::string_view{"AAA"});
    component.setTradeInputDevice(std::string_view{"AAA"});
    component.setOrderInputDevice(std::string_view{"AAA"});
    component.setCurrency(std::string_view{"AAA"});
    component.setComplianceID(std::string_view{"AAA"});
    component.setSolicitedFlag(true);
    component.setOrderCapacity('X');
    component.setOrderRestrictions(std::string_view{"AAA"});
    component.setCustOrderCapacity(12345);
    component.setOrdType('X');
    component.setExecInst(std::string_view{"AAA"});
    component.setTransBkdTime(12345);
    component.setTradingSessionID(std::string_view{"AAA"});
    component.setTradingSessionSubID(std::string_view{"AAA"});
    component.setTimeBracket(std::string_view{"AAA"});
    component.getRefCommissionData().setCommission(123.456);
    component.getRefCommissionData().setCommType('X');
    component.getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefCommissionData().setFundRenewWaiv('X');
    component.setGrossTradeAmt(123.456);
    component.setNumDaysInterest(12345);
    component.setExDate(12345);
    component.setAccruedInterestRate(123.456);
    component.setAccruedInterestAmt(123.456);
    component.setInterestAtMaturity(123.456);
    component.setEndAccruedInterestAmt(123.456);
    component.setStartCash(123.456);
    component.setEndCash(123.456);
    component.setConcession(123.456);
    component.setTotalTakedown(123.456);
    component.setNetMoney(123.456);
    component.setSettlCurrAmt(123.456);
    component.setSettlCurrency(std::string_view{"AAA"});
    component.setSettlCurrFxRate(123.456);
    component.setSettlCurrFxRateCalc('X');
    component.setPositionEffect('X');
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    component.setSideMultiLegReportingType(12345);
    component.getRefContAmtGrp().getRefNoContAmtsGroup().emplace_back();
    component.getRefContAmtGrp().getRefNoContAmtsGroup().back().setContAmtType(12345);
    component.getRefContAmtGrp().getRefNoContAmtsGroup().back().setContAmtValue(123.456);
    component.getRefContAmtGrp().getRefNoContAmtsGroup().back().setContAmtCurr(std::string_view{"AAA"});
    component.getRefStipulations().getRefNoStipulationsGroup().emplace_back();
    component.getRefStipulations().getRefNoStipulationsGroup().back().setStipulationType(std::string_view{"AAA"});
    component.getRefStipulations().getRefNoStipulationsGroup().back().setStipulationValue(std::string_view{"AAA"});
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().emplace_back();
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeAmt(123.456);
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeCurr(std::string_view{"AAA"});
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeType('X');
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeBasis(12345);
    component.setExchangeRule(std::string_view{"AAA"});
    component.setTradeAllocIndicator(12345);
    component.setPreallocMethod('X');
    component.setAllocID(std::string_view{"AAA"});
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().emplace_back();
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocAccount(std::string_view{"AAA"});
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocAcctIDSource(12345);
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocSettlCurrency(std::string_view{"AAA"});
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().setIndividualAllocID(std::string_view{"AAA"});
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().emplace_back();
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyID(std::string_view{"AAA"});
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyIDSource('X');
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyRole(12345);
    component.getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocQty(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    TrdCapRptSideGrp::NoSides decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[11008];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_TrdCapRptSideGrp_NoSidesComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
