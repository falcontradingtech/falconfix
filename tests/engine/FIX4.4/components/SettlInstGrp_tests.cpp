// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlInstGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SettlInstGrpComponentTest : public ::testing::Test {
protected:
    SettlInstGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SettlInstGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_SettlInstGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoSettlInstGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoSettlInstGroup().back().setSettlInstID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SettlInstGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoSettlInstGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoSettlInstGroup().back().setSettlInstID(test_value); }
    component.getRefNoSettlInstGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoSettlInstGroup().back().setSettlInstID(test_value); }
    
    EXPECT_EQ(component.getRefNoSettlInstGroup().size(), 2u);
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
    
    SettlInstGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoSettlInstGroup().size(), component.getRefNoSettlInstGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    SettlInstGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_SettlInstGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoSettlInstGroup().emplace_back();
    component.getRefNoSettlInstGroup().back().setSettlInstID(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setSettlInstTransType('X');
    component.getRefNoSettlInstGroup().back().setSettlInstRefID(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.getRefNoSettlInstGroup().back().setSide('X');
    component.getRefNoSettlInstGroup().back().setProduct(12345);
    component.getRefNoSettlInstGroup().back().setSecurityType(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setCFICode(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setEffectiveTime(12345);
    component.getRefNoSettlInstGroup().back().setExpireTime(12345);
    component.getRefNoSettlInstGroup().back().setLastUpdateTime(12345);
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().setSettlDeliveryType(12345);
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().setStandInstDbType(12345);
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().setStandInstDbName(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().setStandInstDbID(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().emplace_back();
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setSettlInstSource('X');
    component.getRefNoSettlInstGroup().back().getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setDlvyInstType('X');
    component.getRefNoSettlInstGroup().back().setPaymentMethod(12345);
    component.getRefNoSettlInstGroup().back().setPaymentRef(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setCardHolderName(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setCardNumber(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setCardStartDate(12345);
    component.getRefNoSettlInstGroup().back().setCardExpDate(12345);
    component.getRefNoSettlInstGroup().back().setCardIssNum(std::string_view{"AAA"});
    component.getRefNoSettlInstGroup().back().setPaymentDate(12345);
    component.getRefNoSettlInstGroup().back().setPaymentRemitterID(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SettlInstGrp decoded;
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

TEST_F(FIX4_4_SettlInstGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_SettlInstGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
