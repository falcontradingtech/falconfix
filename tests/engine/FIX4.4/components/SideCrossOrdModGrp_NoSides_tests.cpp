// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SideCrossOrdModGrp::NoSides (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SideCrossOrdModGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest : public ::testing::Test {
protected:
    SideCrossOrdModGrp::NoSides component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const char test_value = 'X'; component.setSide(test_value); }
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryClOrdID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetClOrdLinkIDAndClOrdLinkIDMatch) {
    EXPECT_FALSE(component.hasClOrdLinkID());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetAccountAndAccountMatch) {
    EXPECT_FALSE(component.hasAccount());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetDayBookingInstAndDayBookingInstMatch) {
    const char test_value = 'X';
    component.setDayBookingInst(test_value);
    EXPECT_EQ(component.getDayBookingInst(), test_value);
    EXPECT_TRUE(component.hasDayBookingInst());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetBookingUnitAndBookingUnitMatch) {
    const char test_value = 'X';
    component.setBookingUnit(test_value);
    EXPECT_EQ(component.getBookingUnit(), test_value);
    EXPECT_TRUE(component.hasBookingUnit());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetPreallocMethodAndPreallocMethodMatch) {
    const char test_value = 'X';
    component.setPreallocMethod(test_value);
    EXPECT_EQ(component.getPreallocMethod(), test_value);
    EXPECT_TRUE(component.hasPreallocMethod());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetAllocIDAndAllocIDMatch) {
    EXPECT_FALSE(component.hasAllocID());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetQtyTypeAndQtyTypeMatch) {
    const int64_t test_value = 12345;
    component.setQtyType(test_value);
    EXPECT_EQ(component.getQtyType(), test_value);
    EXPECT_TRUE(component.hasQtyType());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetOrderCapacityAndOrderCapacityMatch) {
    const char test_value = 'X';
    component.setOrderCapacity(test_value);
    EXPECT_EQ(component.getOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasOrderCapacity());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetOrderRestrictionsAndOrderRestrictionsMatch) {
    EXPECT_FALSE(component.hasOrderRestrictions());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetCustOrderCapacityAndCustOrderCapacityMatch) {
    const int64_t test_value = 12345;
    component.setCustOrderCapacity(test_value);
    EXPECT_EQ(component.getCustOrderCapacity(), test_value);
    EXPECT_TRUE(component.hasCustOrderCapacity());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetForexReqAndForexReqMatch) {
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetSettlCurrencyAndSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasSettlCurrency());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetBookingTypeAndBookingTypeMatch) {
    const int64_t test_value = 12345;
    component.setBookingType(test_value);
    EXPECT_EQ(component.getBookingType(), test_value);
    EXPECT_TRUE(component.hasBookingType());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetPositionEffectAndPositionEffectMatch) {
    const char test_value = 'X';
    component.setPositionEffect(test_value);
    EXPECT_EQ(component.getPositionEffect(), test_value);
    EXPECT_TRUE(component.hasPositionEffect());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetCoveredOrUncoveredAndCoveredOrUncoveredMatch) {
    const int64_t test_value = 12345;
    component.setCoveredOrUncovered(test_value);
    EXPECT_EQ(component.getCoveredOrUncovered(), test_value);
    EXPECT_TRUE(component.hasCoveredOrUncovered());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetCashMarginAndCashMarginMatch) {
    const char test_value = 'X';
    component.setCashMargin(test_value);
    EXPECT_EQ(component.getCashMargin(), test_value);
    EXPECT_TRUE(component.hasCashMargin());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetClearingFeeIndicatorAndClearingFeeIndicatorMatch) {
    EXPECT_FALSE(component.hasClearingFeeIndicator());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetSolicitedFlagAndSolicitedFlagMatch) {
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetSideComplianceIDAndSideComplianceIDMatch) {
    EXPECT_FALSE(component.hasSideComplianceID());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const char test_value = 'X'; component.setSide(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1308];
    
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
    SideCrossOrdModGrp::NoSides decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSide(), component.getSide());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, FullyPopulatedRoundtrip) {
    char buffer[5232];
    component.setSide('X');
    component.setClOrdID(std::string_view{"AAA"});
    component.setSecondaryClOrdID(std::string_view{"AAA"});
    component.setClOrdLinkID(std::string_view{"AAA"});
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
    component.setPreallocMethod('X');
    component.setAllocID(std::string_view{"AAA"});
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
    component.setQtyType(12345);
    component.getRefOrderQtyData().setOrderQty(123.456);
    component.getRefOrderQtyData().setCashOrderQty(123.456);
    component.getRefOrderQtyData().setOrderPercent(123.456);
    component.getRefOrderQtyData().setRoundingDirection('X');
    component.getRefOrderQtyData().setRoundingModulus(123.456);
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
    component.setPositionEffect('X');
    component.setCoveredOrUncovered(12345);
    component.setCashMargin('X');
    component.setClearingFeeIndicator(std::string_view{"AAA"});
    component.setSolicitedFlag(true);
    component.setSideComplianceID(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SideCrossOrdModGrp::NoSides decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[5232];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_SideCrossOrdModGrp_NoSidesComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
