// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ListOrdGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ListOrdGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class ListOrdGrpComponentTest : public ::testing::Test {
protected:
    ListOrdGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(ListOrdGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setListSeqNo(42);
    component.setSettlInstMode('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(ListOrdGrpComponentTest, SetListSeqNoAndListSeqNoMatch) {
    const int64_t test_value = 12345;
    component.setListSeqNo(test_value);
    EXPECT_EQ(component.getListSeqNo(), test_value);
    EXPECT_TRUE(component.hasListSeqNo());
}

TEST_F(ListOrdGrpComponentTest, SetSettlInstModeAndSettlInstModeMatch) {
    const char test_value = 'X';
    component.setSettlInstMode(test_value);
    EXPECT_EQ(component.getSettlInstMode(), test_value);
    EXPECT_TRUE(component.hasSettlInstMode());
}

TEST_F(ListOrdGrpComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(ListOrdGrpComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(ListOrdGrpComponentTest, SetDayBookingInstAndDayBookingInstMatch) {
    const char test_value = 'X';
    component.setDayBookingInst(test_value);
    EXPECT_EQ(component.getDayBookingInst(), test_value);
    EXPECT_TRUE(component.hasDayBookingInst());
}

TEST_F(ListOrdGrpComponentTest, SetBookingUnitAndBookingUnitMatch) {
    const char test_value = 'X';
    component.setBookingUnit(test_value);
    EXPECT_EQ(component.getBookingUnit(), test_value);
    EXPECT_TRUE(component.hasBookingUnit());
}

TEST_F(ListOrdGrpComponentTest, SetPreallocMethodAndPreallocMethodMatch) {
    const char test_value = 'X';
    component.setPreallocMethod(test_value);
    EXPECT_EQ(component.getPreallocMethod(), test_value);
    EXPECT_TRUE(component.hasPreallocMethod());
}

TEST_F(ListOrdGrpComponentTest, SetSettlTypeAndSettlTypeMatch) {
    const char test_value = 'X';
    component.setSettlType(test_value);
    EXPECT_EQ(component.getSettlType(), test_value);
    EXPECT_TRUE(component.hasSettlType());
}

TEST_F(ListOrdGrpComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetCashMarginAndCashMarginMatch) {
    const char test_value = 'X';
    component.setCashMargin(test_value);
    EXPECT_EQ(component.getCashMargin(), test_value);
    EXPECT_TRUE(component.hasCashMargin());
}

TEST_F(ListOrdGrpComponentTest, SetHandlInstAndHandlInstMatch) {
    const char test_value = 'X';
    component.setHandlInst(test_value);
    EXPECT_EQ(component.getHandlInst(), test_value);
    EXPECT_TRUE(component.hasHandlInst());
}

TEST_F(ListOrdGrpComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(ListOrdGrpComponentTest, SetMaxFloorAndMaxFloorMatch) {
    const double test_value = 123.456;
    component.setMaxFloor(test_value);
    EXPECT_EQ(component.getMaxFloor(), test_value);
    EXPECT_TRUE(component.hasMaxFloor());
}

TEST_F(ListOrdGrpComponentTest, SetProcessCodeAndProcessCodeMatch) {
    const char test_value = 'X';
    component.setProcessCode(test_value);
    EXPECT_EQ(component.getProcessCode(), test_value);
    EXPECT_TRUE(component.hasProcessCode());
}

TEST_F(ListOrdGrpComponentTest, SetPrevClosePxAndPrevClosePxMatch) {
    const double test_value = 123.456;
    component.setPrevClosePx(test_value);
    EXPECT_EQ(component.getPrevClosePx(), test_value);
    EXPECT_TRUE(component.hasPrevClosePx());
}

TEST_F(ListOrdGrpComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(ListOrdGrpComponentTest, SetSideValueIndAndSideValueIndMatch) {
    const int64_t test_value = 12345;
    component.setSideValueInd(test_value);
    EXPECT_EQ(component.getSideValueInd(), test_value);
    EXPECT_TRUE(component.hasSideValueInd());
}

TEST_F(ListOrdGrpComponentTest, SetLocateReqdAndLocateReqdMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetTransactTimeAndTransactTimeMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetQtyTypeAndQtyTypeMatch) {
    const int64_t test_value = 12345;
    component.setQtyType(test_value);
    EXPECT_EQ(component.getQtyType(), test_value);
    EXPECT_TRUE(component.hasQtyType());
}

TEST_F(ListOrdGrpComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(ListOrdGrpComponentTest, SetPriceTypeAndPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setPriceType(test_value);
    EXPECT_EQ(component.getPriceType(), test_value);
    EXPECT_TRUE(component.hasPriceType());
}

TEST_F(ListOrdGrpComponentTest, SetPriceAndPriceMatch) {
    const double test_value = 123.456;
    component.setPrice(test_value);
    EXPECT_EQ(component.getPrice(), test_value);
    EXPECT_TRUE(component.hasPrice());
}

TEST_F(ListOrdGrpComponentTest, SetStopPxAndStopPxMatch) {
    const double test_value = 123.456;
    component.setStopPx(test_value);
    EXPECT_EQ(component.getStopPx(), test_value);
    EXPECT_TRUE(component.hasStopPx());
}

TEST_F(ListOrdGrpComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(ListOrdGrpComponentTest, SetEffectiveTimeAndEffectiveTimeMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetGTBookingInstAndGTBookingInstMatch) {
    const int64_t test_value = 12345;
    component.setGTBookingInst(test_value);
    EXPECT_EQ(component.getGTBookingInst(), test_value);
    EXPECT_TRUE(component.hasGTBookingInst());
}

TEST_F(ListOrdGrpComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(ListOrdGrpComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(ListOrdGrpComponentTest, SetForexReqAndForexReqMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetBookingTypeAndBookingTypeMatch) {
    const int64_t test_value = 12345;
    component.setBookingType(test_value);
    EXPECT_EQ(component.getBookingType(), test_value);
    EXPECT_TRUE(component.hasBookingType());
}

TEST_F(ListOrdGrpComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(ListOrdGrpComponentTest, SetSettlDate2AndSettlDate2Match) {
}

TEST_F(ListOrdGrpComponentTest, SetOrderQty2AndOrderQty2Match) {
    const double test_value = 123.456;
    component.setOrderQty2(test_value);
    EXPECT_EQ(component.getOrderQty2(), test_value);
    EXPECT_TRUE(component.hasOrderQty2());
}

TEST_F(ListOrdGrpComponentTest, SetPrice2AndPrice2Match) {
    const double test_value = 123.456;
    component.setPrice2(test_value);
    EXPECT_EQ(component.getPrice2(), test_value);
    EXPECT_TRUE(component.hasPrice2());
}

TEST_F(ListOrdGrpComponentTest, SetPositionEffectAndPositionEffectMatch) {
    const char test_value = 'X';
    component.setPositionEffect(test_value);
    EXPECT_EQ(component.getPositionEffect(), test_value);
    EXPECT_TRUE(component.hasPositionEffect());
}

TEST_F(ListOrdGrpComponentTest, SetCoveredOrUncoveredAndCoveredOrUncoveredMatch) {
    const int64_t test_value = 12345;
    component.setCoveredOrUncovered(test_value);
    EXPECT_EQ(component.getCoveredOrUncovered(), test_value);
    EXPECT_TRUE(component.hasCoveredOrUncovered());
}

TEST_F(ListOrdGrpComponentTest, SetMaxShowAndMaxShowMatch) {
    const double test_value = 123.456;
    component.setMaxShow(test_value);
    EXPECT_EQ(component.getMaxShow(), test_value);
    EXPECT_TRUE(component.hasMaxShow());
}

TEST_F(ListOrdGrpComponentTest, SetTargetStrategyAndTargetStrategyMatch) {
    const int64_t test_value = 12345;
    component.setTargetStrategy(test_value);
    EXPECT_EQ(component.getTargetStrategy(), test_value);
    EXPECT_TRUE(component.hasTargetStrategy());
}

TEST_F(ListOrdGrpComponentTest, SetParticipationRateAndParticipationRateMatch) {
    const double test_value = 123.456;
    component.setParticipationRate(test_value);
    EXPECT_EQ(component.getParticipationRate(), test_value);
    EXPECT_TRUE(component.hasParticipationRate());
}

TEST_F(ListOrdGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setListSeqNo(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(ListOrdGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2866];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    ListOrdGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(ListOrdGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
