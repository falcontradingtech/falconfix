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

class FIX4_4_AllocGrp_NoAllocsComponentTest : public ::testing::Test {
protected:
    AllocGrp::NoAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setAllocAccount(test_value); }
    { const int64_t test_value = 12345; component.setAllocAcctIDSource(test_value); }
    { const char test_value = 'X'; component.setMatchStatus(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocAccountAndAllocAccountMatch) {
    EXPECT_FALSE(component.hasAllocAccount());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocAcctIDSourceAndAllocAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    EXPECT_EQ(component.getAllocAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAllocAcctIDSource());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetMatchStatusAndMatchStatusMatch) {
    const char test_value = 'X';
    component.setMatchStatus(test_value);
    EXPECT_EQ(component.getMatchStatus(), test_value);
    EXPECT_TRUE(component.hasMatchStatus());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocPriceAndAllocPriceMatch) {
    const double test_value = 123.456;
    component.setAllocPrice(test_value);
    EXPECT_EQ(component.getAllocPrice(), test_value);
    EXPECT_TRUE(component.hasAllocPrice());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocQtyAndAllocQtyMatch) {
    const double test_value = 123.456;
    component.setAllocQty(test_value);
    EXPECT_EQ(component.getAllocQty(), test_value);
    EXPECT_TRUE(component.hasAllocQty());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetIndividualAllocIDAndIndividualAllocIDMatch) {
    EXPECT_FALSE(component.hasIndividualAllocID());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetProcessCodeAndProcessCodeMatch) {
    const char test_value = 'X';
    component.setProcessCode(test_value);
    EXPECT_EQ(component.getProcessCode(), test_value);
    EXPECT_TRUE(component.hasProcessCode());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetNotifyBrokerOfCreditAndNotifyBrokerOfCreditMatch) {
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocHandlInstAndAllocHandlInstMatch) {
    const int64_t test_value = 12345;
    component.setAllocHandlInst(test_value);
    EXPECT_EQ(component.getAllocHandlInst(), test_value);
    EXPECT_TRUE(component.hasAllocHandlInst());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocTextAndAllocTextMatch) {
    EXPECT_FALSE(component.hasAllocText());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetEncodedAllocTextLenAndEncodedAllocTextLenMatch) {
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetEncodedAllocTextAndEncodedAllocTextMatch) {
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocAvgPxAndAllocAvgPxMatch) {
    const double test_value = 123.456;
    component.setAllocAvgPx(test_value);
    EXPECT_EQ(component.getAllocAvgPx(), test_value);
    EXPECT_TRUE(component.hasAllocAvgPx());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocNetMoneyAndAllocNetMoneyMatch) {
    const double test_value = 123.456;
    component.setAllocNetMoney(test_value);
    EXPECT_EQ(component.getAllocNetMoney(), test_value);
    EXPECT_TRUE(component.hasAllocNetMoney());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetSettlCurrAmtAndSettlCurrAmtMatch) {
    const double test_value = 123.456;
    component.setSettlCurrAmt(test_value);
    EXPECT_EQ(component.getSettlCurrAmt(), test_value);
    EXPECT_TRUE(component.hasSettlCurrAmt());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocSettlCurrAmtAndAllocSettlCurrAmtMatch) {
    const double test_value = 123.456;
    component.setAllocSettlCurrAmt(test_value);
    EXPECT_EQ(component.getAllocSettlCurrAmt(), test_value);
    EXPECT_TRUE(component.hasAllocSettlCurrAmt());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetSettlCurrencyAndSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasSettlCurrency());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocSettlCurrencyAndAllocSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasAllocSettlCurrency());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetSettlCurrFxRateAndSettlCurrFxRateMatch) {
    const double test_value = 123.456;
    component.setSettlCurrFxRate(test_value);
    EXPECT_EQ(component.getSettlCurrFxRate(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRate());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetSettlCurrFxRateCalcAndSettlCurrFxRateCalcMatch) {
    const char test_value = 'X';
    component.setSettlCurrFxRateCalc(test_value);
    EXPECT_EQ(component.getSettlCurrFxRateCalc(), test_value);
    EXPECT_TRUE(component.hasSettlCurrFxRateCalc());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocAccruedInterestAmtAndAllocAccruedInterestAmtMatch) {
    const double test_value = 123.456;
    component.setAllocAccruedInterestAmt(test_value);
    EXPECT_EQ(component.getAllocAccruedInterestAmt(), test_value);
    EXPECT_TRUE(component.hasAllocAccruedInterestAmt());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocInterestAtMaturityAndAllocInterestAtMaturityMatch) {
    const double test_value = 123.456;
    component.setAllocInterestAtMaturity(test_value);
    EXPECT_EQ(component.getAllocInterestAtMaturity(), test_value);
    EXPECT_TRUE(component.hasAllocInterestAtMaturity());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetAllocSettlInstTypeAndAllocSettlInstTypeMatch) {
    const int64_t test_value = 12345;
    component.setAllocSettlInstType(test_value);
    EXPECT_EQ(component.getAllocSettlInstType(), test_value);
    EXPECT_TRUE(component.hasAllocSettlInstType());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setAllocAccount(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1208];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    AllocGrp::NoAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getAllocAcctIDSource(), component.getAllocAcctIDSource());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4832];
    component.setAllocAccount(std::string_view{"AAA"});
    component.setAllocAcctIDSource(12345);
    component.setMatchStatus('X');
    component.setAllocPrice(123.456);
    component.setAllocQty(123.456);
    component.setIndividualAllocID(std::string_view{"AAA"});
    component.setProcessCode('X');
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().emplace_back();
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().back().setNestedPartySubID(std::string_view{"AAA"});
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().back().setNestedPartySubIDType(12345);
    component.setNotifyBrokerOfCredit(true);
    component.setAllocHandlInst(12345);
    component.setAllocText(std::string_view{"AAA"});
    component.setEncodedAllocTextLen(12345);
    component.setEncodedAllocText(std::string_view{"AAA"});
    component.getRefCommissionData().setCommission(123.456);
    component.getRefCommissionData().setCommType('X');
    component.getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefCommissionData().setFundRenewWaiv('X');
    component.setAllocAvgPx(123.456);
    component.setAllocNetMoney(123.456);
    component.setSettlCurrAmt(123.456);
    component.setAllocSettlCurrAmt(123.456);
    component.setSettlCurrency(std::string_view{"AAA"});
    component.setAllocSettlCurrency(std::string_view{"AAA"});
    component.setSettlCurrFxRate(123.456);
    component.setSettlCurrFxRateCalc('X');
    component.setAllocAccruedInterestAmt(123.456);
    component.setAllocInterestAtMaturity(123.456);
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().emplace_back();
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeAmt(123.456);
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeCurr(std::string_view{"AAA"});
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeType('X');
    component.getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeBasis(12345);
    component.getRefClrInstGrp().getRefNoClearingInstructionsGroup().emplace_back();
    component.getRefClrInstGrp().getRefNoClearingInstructionsGroup().back().setClearingInstruction(12345);
    component.setAllocSettlInstType(12345);
    component.getRefSettlInstructionsData().setSettlDeliveryType(12345);
    component.getRefSettlInstructionsData().setStandInstDbType(12345);
    component.getRefSettlInstructionsData().setStandInstDbName(std::string_view{"AAA"});
    component.getRefSettlInstructionsData().setStandInstDbID(std::string_view{"AAA"});
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().emplace_back();
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setSettlInstSource('X');
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setDlvyInstType('X');
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().emplace_back();
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyID(std::string_view{"AAA"});
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyIDSource('X');
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyRole(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    AllocGrp::NoAllocs decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4832];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_AllocGrp_NoAllocsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
