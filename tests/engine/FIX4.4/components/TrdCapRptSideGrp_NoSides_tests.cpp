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

class TrdCapRptSideGrp_NoSidesComponentTest : public ::testing::Test {
protected:
    TrdCapRptSideGrp::NoSides component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSide('A');
    component.setAcctIDSource(42);
    component.setAccountType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetProcessCodeAndProcessCodeMatch) {
    const char test_value = 'X';
    component.setProcessCode(test_value);
    EXPECT_EQ(component.getProcessCode(), test_value);
    EXPECT_TRUE(component.hasProcessCode());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetOddLotAndOddLotMatch) {
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetTransBkdTimeAndTransBkdTimeMatch) {
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetGrossTradeAmtAndGrossTradeAmtMatch) {
    const double test_value = 123.456;
    component.setGrossTradeAmt(test_value);
    EXPECT_EQ(component.getGrossTradeAmt(), test_value);
    EXPECT_TRUE(component.hasGrossTradeAmt());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetNumDaysInterestAndNumDaysInterestMatch) {
    const int64_t test_value = 12345;
    component.setNumDaysInterest(test_value);
    EXPECT_EQ(component.getNumDaysInterest(), test_value);
    EXPECT_TRUE(component.hasNumDaysInterest());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetExDateAndExDateMatch) {
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetAccruedInterestRateAndAccruedInterestRateMatch) {
    const double test_value = 123.456;
    component.setAccruedInterestRate(test_value);
    EXPECT_EQ(component.getAccruedInterestRate(), test_value);
    EXPECT_TRUE(component.hasAccruedInterestRate());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetAccruedInterestAmtAndAccruedInterestAmtMatch) {
    const double test_value = 123.456;
    component.setAccruedInterestAmt(test_value);
    EXPECT_EQ(component.getAccruedInterestAmt(), test_value);
    EXPECT_TRUE(component.hasAccruedInterestAmt());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetInterestAtMaturityAndInterestAtMaturityMatch) {
    const double test_value = 123.456;
    component.setInterestAtMaturity(test_value);
    EXPECT_EQ(component.getInterestAtMaturity(), test_value);
    EXPECT_TRUE(component.hasInterestAtMaturity());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetEndAccruedInterestAmtAndEndAccruedInterestAmtMatch) {
    const double test_value = 123.456;
    component.setEndAccruedInterestAmt(test_value);
    EXPECT_EQ(component.getEndAccruedInterestAmt(), test_value);
    EXPECT_TRUE(component.hasEndAccruedInterestAmt());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetStartCashAndStartCashMatch) {
    const double test_value = 123.456;
    component.setStartCash(test_value);
    EXPECT_EQ(component.getStartCash(), test_value);
    EXPECT_TRUE(component.hasStartCash());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetEndCashAndEndCashMatch) {
    const double test_value = 123.456;
    component.setEndCash(test_value);
    EXPECT_EQ(component.getEndCash(), test_value);
    EXPECT_TRUE(component.hasEndCash());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetConcessionAndConcessionMatch) {
    const double test_value = 123.456;
    component.setConcession(test_value);
    EXPECT_EQ(component.getConcession(), test_value);
    EXPECT_TRUE(component.hasConcession());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetTotalTakedownAndTotalTakedownMatch) {
    const double test_value = 123.456;
    component.setTotalTakedown(test_value);
    EXPECT_EQ(component.getTotalTakedown(), test_value);
    EXPECT_TRUE(component.hasTotalTakedown());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetNetMoneyAndNetMoneyMatch) {
    const double test_value = 123.456;
    component.setNetMoney(test_value);
    EXPECT_EQ(component.getNetMoney(), test_value);
    EXPECT_TRUE(component.hasNetMoney());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrAmtAndSettlCurrAmtMatch) {
    const double test_value = 123.456;
    component.setSettlCurrAmt(test_value);
    EXPECT_EQ(component.getSettlCurrAmt(), test_value);
    EXPECT_TRUE(component.hasSettlCurrAmt());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrFxRateAndSettlCurrFxRateMatch) {
    const double test_value = 123.456;
    component.setSettlCurrFxRate(test_value);
    EXPECT_EQ(component.getSettlCurrFxRate(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRate());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetSettlCurrFxRateCalcAndSettlCurrFxRateCalcMatch) {
    const char test_value = 'X';
    component.setSettlCurrFxRateCalc(test_value);
    EXPECT_EQ(component.getSettlCurrFxRateCalc(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRateCalc());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetPositionEffectAndPositionEffectMatch) {
    const char test_value = 'X';
    component.setPositionEffect(test_value);
    EXPECT_EQ(component.getPositionEffect(), test_value);
    EXPECT_TRUE(component.hasPositionEffect());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetSideMultiLegReportingTypeAndSideMultiLegReportingTypeMatch) {
    const int64_t test_value = 12345;
    component.setSideMultiLegReportingType(test_value);
    EXPECT_EQ(component.getSideMultiLegReportingType(), test_value);
    EXPECT_TRUE(component.hasSideMultiLegReportingType());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetTradeAllocIndicatorAndTradeAllocIndicatorMatch) {
    const int64_t test_value = 12345;
    component.setTradeAllocIndicator(test_value);
    EXPECT_EQ(component.getTradeAllocIndicator(), test_value);
    EXPECT_TRUE(component.hasTradeAllocIndicator());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, SetPreallocMethodAndPreallocMethodMatch) {
    const char test_value = 'X';
    component.setPreallocMethod(test_value);
    EXPECT_EQ(component.getPreallocMethod(), test_value);
    EXPECT_TRUE(component.hasPreallocMethod());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSide('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2752];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    TrdCapRptSideGrp::NoSides decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(TrdCapRptSideGrp_NoSidesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
