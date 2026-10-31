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

class FIX4_4_SideCrossOrdModGrpComponentTest : public ::testing::Test {
protected:
    SideCrossOrdModGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, EncodeDecodeRoundtrip) {
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
    
    SideCrossOrdModGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, PopulatedGroupLoopRoundtrip) {
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
    
    SideCrossOrdModGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoSidesGroup().size(), component.getRefNoSidesGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    SideCrossOrdModGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoSidesGroup().emplace_back();
    component.getRefNoSidesGroup().back().setSide('X');
    component.getRefNoSidesGroup().back().setClOrdID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSecondaryClOrdID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setClOrdLinkID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.getRefNoSidesGroup().back().setTradeOriginationDate(12345);
    component.getRefNoSidesGroup().back().setTradeDate(12345);
    component.getRefNoSidesGroup().back().setAccount(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setAcctIDSource(12345);
    component.getRefNoSidesGroup().back().setAccountType(12345);
    component.getRefNoSidesGroup().back().setDayBookingInst('X');
    component.getRefNoSidesGroup().back().setBookingUnit('X');
    component.getRefNoSidesGroup().back().setPreallocMethod('X');
    component.getRefNoSidesGroup().back().setAllocID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocAccount(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocAcctIDSource(12345);
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocSettlCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setIndividualAllocID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefNoSidesGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocQty(123.456);
    component.getRefNoSidesGroup().back().setQtyType(12345);
    component.getRefNoSidesGroup().back().getRefOrderQtyData().setOrderQty(123.456);
    component.getRefNoSidesGroup().back().getRefOrderQtyData().setCashOrderQty(123.456);
    component.getRefNoSidesGroup().back().getRefOrderQtyData().setOrderPercent(123.456);
    component.getRefNoSidesGroup().back().getRefOrderQtyData().setRoundingDirection('X');
    component.getRefNoSidesGroup().back().getRefOrderQtyData().setRoundingModulus(123.456);
    component.getRefNoSidesGroup().back().getRefCommissionData().setCommission(123.456);
    component.getRefNoSidesGroup().back().getRefCommissionData().setCommType('X');
    component.getRefNoSidesGroup().back().getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().getRefCommissionData().setFundRenewWaiv('X');
    component.getRefNoSidesGroup().back().setOrderCapacity('X');
    component.getRefNoSidesGroup().back().setOrderRestrictions(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setCustOrderCapacity(12345);
    component.getRefNoSidesGroup().back().setForexReq(true);
    component.getRefNoSidesGroup().back().setSettlCurrency(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setBookingType(12345);
    component.getRefNoSidesGroup().back().setText(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setEncodedTextLen(12345);
    component.getRefNoSidesGroup().back().setEncodedText(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setPositionEffect('X');
    component.getRefNoSidesGroup().back().setCoveredOrUncovered(12345);
    component.getRefNoSidesGroup().back().setCashMargin('X');
    component.getRefNoSidesGroup().back().setClearingFeeIndicator(std::string_view{"AAA"});
    component.getRefNoSidesGroup().back().setSolicitedFlag(true);
    component.getRefNoSidesGroup().back().setSideComplianceID(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SideCrossOrdModGrp decoded;
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

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_SideCrossOrdModGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
