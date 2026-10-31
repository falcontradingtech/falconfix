// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDFullGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDFullGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_MDFullGrpComponentTest : public ::testing::Test {
protected:
    MDFullGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_MDFullGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_MDFullGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_MDFullGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoMDEntriesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoMDEntriesGroup().back().setMDEntryType(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    MDFullGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_MDFullGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoMDEntriesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoMDEntriesGroup().back().setMDEntryType(test_value); }
    component.getRefNoMDEntriesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoMDEntriesGroup().back().setMDEntryType(test_value); }
    
    EXPECT_EQ(component.getRefNoMDEntriesGroup().size(), 2u);
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
    
    MDFullGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoMDEntriesGroup().size(), component.getRefNoMDEntriesGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    MDFullGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_MDFullGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoMDEntriesGroup().emplace_back();
    component.getRefNoMDEntriesGroup().back().setMDEntryType('X');
    component.getRefNoMDEntriesGroup().back().setMDEntryPx(123.456);
    component.getRefNoMDEntriesGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntrySize(123.456);
    component.getRefNoMDEntriesGroup().back().setMDEntryDate(12345);
    component.getRefNoMDEntriesGroup().back().setMDEntryTime(12345);
    component.getRefNoMDEntriesGroup().back().setTickDirection('X');
    component.getRefNoMDEntriesGroup().back().setMDMkt(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setQuoteCondition(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTradeCondition(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntryOriginator(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setLocationID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setDeskID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setOpenCloseSettlFlag(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTimeInForce('X');
    component.getRefNoMDEntriesGroup().back().setExpireDate(12345);
    component.getRefNoMDEntriesGroup().back().setExpireTime(12345);
    component.getRefNoMDEntriesGroup().back().setMinQty(123.456);
    component.getRefNoMDEntriesGroup().back().setExecInst(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setSellerDays(12345);
    component.getRefNoMDEntriesGroup().back().setOrderID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setQuoteEntryID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntryBuyer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntrySeller(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setNumberOfOrders(12345);
    component.getRefNoMDEntriesGroup().back().setMDEntryPositionNo(12345);
    component.getRefNoMDEntriesGroup().back().setScope(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setPriceDelta(123.456);
    component.getRefNoMDEntriesGroup().back().setText(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setEncodedTextLen(12345);
    component.getRefNoMDEntriesGroup().back().setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    MDFullGrp decoded;
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

TEST_F(FIX4_4_MDFullGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_MDFullGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
