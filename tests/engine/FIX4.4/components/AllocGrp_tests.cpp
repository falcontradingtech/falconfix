// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: AllocGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/AllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_AllocGrpComponentTest : public ::testing::Test {
protected:
    AllocGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_AllocGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_AllocGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_AllocGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoAllocsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoAllocsGroup().back().setAllocAccount(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    AllocGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_AllocGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoAllocsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoAllocsGroup().back().setAllocAccount(test_value); }
    component.getRefNoAllocsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoAllocsGroup().back().setAllocAccount(test_value); }
    
    EXPECT_EQ(component.getRefNoAllocsGroup().size(), 2u);
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
    
    AllocGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoAllocsGroup().size(), component.getRefNoAllocsGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    AllocGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_AllocGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoAllocsGroup().emplace_back();
    component.getRefNoAllocsGroup().back().setAllocAccount(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().setAllocAcctIDSource(12345);
    component.getRefNoAllocsGroup().back().setMatchStatus('X');
    component.getRefNoAllocsGroup().back().setAllocPrice(123.456);
    component.getRefNoAllocsGroup().back().setAllocQty(123.456);
    component.getRefNoAllocsGroup().back().setIndividualAllocID(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().setProcessCode('X');
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().emplace_back();
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().back().setNestedPartySubID(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().back().setNestedPartySubIDType(12345);
    component.getRefNoAllocsGroup().back().setNotifyBrokerOfCredit(true);
    component.getRefNoAllocsGroup().back().setAllocHandlInst(12345);
    component.getRefNoAllocsGroup().back().setAllocText(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().setEncodedAllocTextLen(12345);
    component.getRefNoAllocsGroup().back().setEncodedAllocText(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefCommissionData().setCommission(123.456);
    component.getRefNoAllocsGroup().back().getRefCommissionData().setCommType('X');
    component.getRefNoAllocsGroup().back().getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefCommissionData().setFundRenewWaiv('X');
    component.getRefNoAllocsGroup().back().setAllocAvgPx(123.456);
    component.getRefNoAllocsGroup().back().setAllocNetMoney(123.456);
    component.getRefNoAllocsGroup().back().setSettlCurrAmt(123.456);
    component.getRefNoAllocsGroup().back().setAllocSettlCurrAmt(123.456);
    component.getRefNoAllocsGroup().back().setSettlCurrency(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().setAllocSettlCurrency(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().setSettlCurrFxRate(123.456);
    component.getRefNoAllocsGroup().back().setSettlCurrFxRateCalc('X');
    component.getRefNoAllocsGroup().back().setAllocAccruedInterestAmt(123.456);
    component.getRefNoAllocsGroup().back().setAllocInterestAtMaturity(123.456);
    component.getRefNoAllocsGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().emplace_back();
    component.getRefNoAllocsGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeAmt(123.456);
    component.getRefNoAllocsGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeCurr(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeType('X');
    component.getRefNoAllocsGroup().back().getRefMiscFeesGrp().getRefNoMiscFeesGroup().back().setMiscFeeBasis(12345);
    component.getRefNoAllocsGroup().back().getRefClrInstGrp().getRefNoClearingInstructionsGroup().emplace_back();
    component.getRefNoAllocsGroup().back().getRefClrInstGrp().getRefNoClearingInstructionsGroup().back().setClearingInstruction(12345);
    component.getRefNoAllocsGroup().back().setAllocSettlInstType(12345);
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().setSettlDeliveryType(12345);
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().setStandInstDbType(12345);
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().setStandInstDbName(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().setStandInstDbID(std::string_view{"AAA"});
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().emplace_back();
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setSettlInstSource('X');
    component.getRefNoAllocsGroup().back().getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setDlvyInstType('X');
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    AllocGrp decoded;
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

TEST_F(FIX4_4_AllocGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_AllocGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
