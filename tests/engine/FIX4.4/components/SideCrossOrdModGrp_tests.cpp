// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SideCrossOrdModGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SideCrossOrdModGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SideCrossOrdModGrpComponentTest : public ::testing::Test {
protected:
    SideCrossOrdModGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SideCrossOrdModGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSide('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(SideCrossOrdModGrpComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(SideCrossOrdModGrpComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetDayBookingInstAndDayBookingInstMatch) {
    const char test_value = 'X';
    component.setDayBookingInst(test_value);
    EXPECT_EQ(component.getDayBookingInst(), test_value);
    EXPECT_TRUE(component.hasDayBookingInst());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetBookingUnitAndBookingUnitMatch) {
    const char test_value = 'X';
    component.setBookingUnit(test_value);
    EXPECT_EQ(component.getBookingUnit(), test_value);
    EXPECT_TRUE(component.hasBookingUnit());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetPreallocMethodAndPreallocMethodMatch) {
    const char test_value = 'X';
    component.setPreallocMethod(test_value);
    EXPECT_EQ(component.getPreallocMethod(), test_value);
    EXPECT_TRUE(component.hasPreallocMethod());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetQtyTypeAndQtyTypeMatch) {
    const int64_t test_value = 12345;
    component.setQtyType(test_value);
    EXPECT_EQ(component.getQtyType(), test_value);
    EXPECT_TRUE(component.hasQtyType());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetForexReqAndForexReqMatch) {
}

TEST_F(SideCrossOrdModGrpComponentTest, SetBookingTypeAndBookingTypeMatch) {
    const int64_t test_value = 12345;
    component.setBookingType(test_value);
    EXPECT_EQ(component.getBookingType(), test_value);
    EXPECT_TRUE(component.hasBookingType());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(SideCrossOrdModGrpComponentTest, SetPositionEffectAndPositionEffectMatch) {
    const char test_value = 'X';
    component.setPositionEffect(test_value);
    EXPECT_EQ(component.getPositionEffect(), test_value);
    EXPECT_TRUE(component.hasPositionEffect());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetCoveredOrUncoveredAndCoveredOrUncoveredMatch) {
    const int64_t test_value = 12345;
    component.setCoveredOrUncovered(test_value);
    EXPECT_EQ(component.getCoveredOrUncovered(), test_value);
    EXPECT_TRUE(component.hasCoveredOrUncovered());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetCashMarginAndCashMarginMatch) {
    const char test_value = 'X';
    component.setCashMargin(test_value);
    EXPECT_EQ(component.getCashMargin(), test_value);
    EXPECT_TRUE(component.hasCashMargin());
}

TEST_F(SideCrossOrdModGrpComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
}

TEST_F(SideCrossOrdModGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSide('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SideCrossOrdModGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1308];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    SideCrossOrdModGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(SideCrossOrdModGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
