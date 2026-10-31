// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: TrdCapRptSideGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/TrdCapRptSideGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_TrdCapRptSideGrpComponentTest : public ::testing::Test {
protected:
    TrdCapRptSideGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoSidesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoSidesGroup().back().setSide(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    TrdCapRptSideGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoSidesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoSidesGroup().back().setSide(test_value); }
    component.getRefNoSidesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoSidesGroup().back().setSide(test_value); }
    
    EXPECT_EQ(component.getRefNoSidesGroup().size(), 2u);
    EXPECT_TRUE(component.hasAnySet());
    // We populate each entry's decode anchor (first field) to drive the
    // group encode/decode loop. That does not necessarily satisfy every
    // REQUIRED field of the entry, so we only exercise checkRequired()
    // for coverage here; its truth value is asserted by the dedicated
    // CheckRequiredWhenEmpty test.
    (void)component.checkRequired();
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    EXPECT_GT(static_cast<std::size_t>(p - buffer), 0u);
    
    TrdCapRptSideGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoSidesGroup().size(), component.getRefNoSidesGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    TrdCapRptSideGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoSidesGroup().emplace_back();
    component.getRefNoSidesGroup().back().setSide('X');
    component.getRefNoSidesGroup().back().setOrderID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSecondaryOrderID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setClOrdID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSecondaryClOrdID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setListID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.getRefNoSidesGroup().back().setAccount(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setAcctIDSource(12345);
    component.getRefNoSidesGroup().back().setAccountType(12345);
    component.getRefNoSidesGroup().back().setProcessCode('X');
    component.getRefNoSidesGroup().back().setOddLot(true);
    component.getRefNoSidesGroup().back().getRefClrInstGrp().getRefNoClearingInstructionsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefClrInstGrp().getRefNoClearingInstructionsGroup().back().setClearingInstruction(12345);
    component.getRefNoSidesGroup().back().setTradeInputSource(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setTradeInputDevice(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setOrderInputDevice(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setComplianceID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSolicitedFlag(true);
    component.getRefNoSidesGroup().back().setOrderCapacity('X');
    component.getRefNoSidesGroup().back().setOrderRestrictions(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setCustOrderCapacity(12345);
    component.getRefNoSidesGroup().back().setOrdType('X');
    component.getRefNoSidesGroup().back().setExecInst(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setTransBkdTime(12345);
    component.getRefNoSidesGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setTimeBracket(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefCommissionData().setCommission(123.456);
    component.getRefNoSidesGroup().back().getRefCommissionData().setCommType('X');
    component.getRefNoSidesGroup().back().getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefCommissionData().setFundRenewWaiv('X');
    component.getRefNoSidesGroup().back().setGrossTradeAmt(123.456);
    component.getRefNoSidesGroup().back().setNumDaysInterest(12345);
    component.getRefNoSidesGroup().back().setExDate(12345);
    component.getRefNoSidesGroup().back().setAccruedInterestRate(123.456);
    component.getRefNoSidesGroup().back().setAccruedInterestAmt(123.456);
    component.getRefNoSidesGroup().back().setInterestAtMaturity(123.456);
    component.getRefNoSidesGroup().back().setEndAccruedInterestAmt(123.456);
    component.getRefNoSidesGroup().back().setStartCash(123.456);
    component.getRefNoSidesGroup().back().setEndCash(123.456);
    component.getRefNoSidesGroup().back().setConcession(123.456);
    component.getRefNoSidesGroup().back().setTotalTakedown(123.456);
    component.getRefNoSidesGroup().back().setNetMoney(123.456);
    component.getRefNoSidesGroup().back().setSettlCurrAmt(123.456);
    component.getRefNoSidesGroup().back().setSettlCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSettlCurrFxRate(123.456);
    component.getRefNoSidesGroup().back().setSettlCurrFxRateCalc('X');
    component.getRefNoSidesGroup().back().setPositionEffect('X');
    component.getRefNoSidesGroup().back().setText(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setEncodedTextLen(12345);
    component.getRefNoSidesGroup().back().setEncodedText(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSideMultiLegReportingType(12345);
    component.getRefNoSidesGroup().back().getRefContAmtGrp().getRefNoContAmtsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefContAmtGrp().getRefNoContAmtsGroup().back().setContAmtType(12345);
    component.getRefNoSidesGroup().back().getRefContAmtGrp().getRefNoContAmtsGroup().back().setContAmtValue(123.456);
    component.getRefNoSidesGroup().back().getRefContAmtGrp().getRefNoContAmtsGroup().back().setContAmtCurr(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefStipulations().getRefNoStipulationsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefStipulations().getRefNoStipulationsGroup().back().setStipulationType(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefStipulations().getRefNoStipulationsGroup().back().setStipulationValue(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeAmt(123.456);
    component.getRefNoSidesGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeCurr(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeType('X');
    component.getRefNoSidesGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeBasis(12345);
    component.getRefNoSidesGroup().back().setExchangeRule(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setTradeAllocIndicator(12345);
    component.getRefNoSidesGroup().back().setPreallocMethod('X');
    component.getRefNoSidesGroup().back().setAllocID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocAccount(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocAcctIDSource(12345);
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocSettlCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().setIndividualAllocID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyIDSource('X');
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyRole(12345);
    component.getRefNoSidesGroup().back().getRefTrdAllocGrp().getRefNoAllocsGroup().back().setAllocQty(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    TrdCapRptSideGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4096];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_TrdCapRptSideGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
