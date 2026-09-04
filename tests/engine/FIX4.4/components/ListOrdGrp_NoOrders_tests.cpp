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
    component.setListSeqNo(42);
    component.setSettlInstMode('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetListSeqNoAndListSeqNoMatch) {
    const int64_t test_value = 12345;
    component.setListSeqNo(test_value);
    EXPECT_EQ(component.getListSeqNo(), test_value);
    EXPECT_TRUE(component.hasListSeqNo());
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

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetHandlInstAndHandlInstMatch) {
    const char test_value = 'X';
    component.setHandlInst(test_value);
    EXPECT_EQ(component.getHandlInst(), test_value);
    EXPECT_TRUE(component.hasHandlInst());
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

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
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

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetForexReqAndForexReqMatch) {
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetBookingTypeAndBookingTypeMatch) {
    const int64_t test_value = 12345;
    component.setBookingType(test_value);
    EXPECT_EQ(component.getBookingType(), test_value);
    EXPECT_TRUE(component.hasBookingType());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
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

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, SetParticipationRateAndParticipationRateMatch) {
    const double test_value = 123.456;
    component.setParticipationRate(test_value);
    EXPECT_EQ(component.getParticipationRate(), test_value);
    EXPECT_TRUE(component.hasParticipationRate());
}

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setListSeqNo(42);
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

TEST_F(FIX4_4_ListOrdGrp_NoOrdersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}
