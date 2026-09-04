// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: AllocGrp::NoAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/AllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class AllocGrp_NoAllocsComponentTest : public ::testing::Test {
protected:
    AllocGrp::NoAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(AllocGrp_NoAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setAllocAcctIDSource(42);
    component.setMatchStatus('A');
    component.setAllocPrice(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocAcctIDSourceAndAllocAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    EXPECT_EQ(component.getAllocAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAllocAcctIDSource());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetMatchStatusAndMatchStatusMatch) {
    const char test_value = 'X';
    component.setMatchStatus(test_value);
    EXPECT_EQ(component.getMatchStatus(), test_value);
    EXPECT_TRUE(component.hasMatchStatus());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocPriceAndAllocPriceMatch) {
    const double test_value = 123.456;
    component.setAllocPrice(test_value);
    EXPECT_EQ(component.getAllocPrice(), test_value);
    EXPECT_TRUE(component.hasAllocPrice());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocQtyAndAllocQtyMatch) {
    const double test_value = 123.456;
    component.setAllocQty(test_value);
    EXPECT_EQ(component.getAllocQty(), test_value);
    EXPECT_TRUE(component.hasAllocQty());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetProcessCodeAndProcessCodeMatch) {
    const char test_value = 'X';
    component.setProcessCode(test_value);
    EXPECT_EQ(component.getProcessCode(), test_value);
    EXPECT_TRUE(component.hasProcessCode());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetNotifyBrokerOfCreditAndNotifyBrokerOfCreditMatch) {
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocHandlInstAndAllocHandlInstMatch) {
    const int64_t test_value = 12345;
    component.setAllocHandlInst(test_value);
    EXPECT_EQ(component.getAllocHandlInst(), test_value);
    EXPECT_TRUE(component.hasAllocHandlInst());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetEncodedAllocTextLenAndEncodedAllocTextLenMatch) {
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocAvgPxAndAllocAvgPxMatch) {
    const double test_value = 123.456;
    component.setAllocAvgPx(test_value);
    EXPECT_EQ(component.getAllocAvgPx(), test_value);
    EXPECT_TRUE(component.hasAllocAvgPx());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocNetMoneyAndAllocNetMoneyMatch) {
    const double test_value = 123.456;
    component.setAllocNetMoney(test_value);
    EXPECT_EQ(component.getAllocNetMoney(), test_value);
    EXPECT_TRUE(component.hasAllocNetMoney());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetSettlCurrAmtAndSettlCurrAmtMatch) {
    const double test_value = 123.456;
    component.setSettlCurrAmt(test_value);
    EXPECT_EQ(component.getSettlCurrAmt(), test_value);
    EXPECT_TRUE(component.hasSettlCurrAmt());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocSettlCurrAmtAndAllocSettlCurrAmtMatch) {
    const double test_value = 123.456;
    component.setAllocSettlCurrAmt(test_value);
    EXPECT_EQ(component.getAllocSettlCurrAmt(), test_value);
    EXPECT_TRUE(component.hasAllocSettlCurrAmt());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetSettlCurrFxRateAndSettlCurrFxRateMatch) {
    const double test_value = 123.456;
    component.setSettlCurrFxRate(test_value);
    EXPECT_EQ(component.getSettlCurrFxRate(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRate());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetSettlCurrFxRateCalcAndSettlCurrFxRateCalcMatch) {
    const char test_value = 'X';
    component.setSettlCurrFxRateCalc(test_value);
    EXPECT_EQ(component.getSettlCurrFxRateCalc(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRateCalc());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocAccruedInterestAmtAndAllocAccruedInterestAmtMatch) {
    const double test_value = 123.456;
    component.setAllocAccruedInterestAmt(test_value);
    EXPECT_EQ(component.getAllocAccruedInterestAmt(), test_value);
    EXPECT_TRUE(component.hasAllocAccruedInterestAmt());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocInterestAtMaturityAndAllocInterestAtMaturityMatch) {
    const double test_value = 123.456;
    component.setAllocInterestAtMaturity(test_value);
    EXPECT_EQ(component.getAllocInterestAtMaturity(), test_value);
    EXPECT_TRUE(component.hasAllocInterestAtMaturity());
}

TEST_F(AllocGrp_NoAllocsComponentTest, SetAllocSettlInstTypeAndAllocSettlInstTypeMatch) {
    const int64_t test_value = 12345;
    component.setAllocSettlInstType(test_value);
    EXPECT_EQ(component.getAllocSettlInstType(), test_value);
    EXPECT_TRUE(component.hasAllocSettlInstType());
}

TEST_F(AllocGrp_NoAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setAllocAcctIDSource(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(AllocGrp_NoAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1208];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    AllocGrp::NoAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(AllocGrp_NoAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
