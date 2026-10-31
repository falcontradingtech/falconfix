// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegPreAllocGrp::NoLegAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegPreAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest : public ::testing::Test {
protected:
    LegPreAllocGrp::NoLegAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setLegAllocAccount(test_value); }
    { const std::string_view test_value{"AAA"}; component.setLegIndividualAllocID(test_value); }
    { const double test_value = 123.456; component.setLegAllocQty(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, SetLegAllocAccountAndLegAllocAccountMatch) {
    EXPECT_FALSE(component.hasLegAllocAccount());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, SetLegIndividualAllocIDAndLegIndividualAllocIDMatch) {
    EXPECT_FALSE(component.hasLegIndividualAllocID());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, SetLegAllocQtyAndLegAllocQtyMatch) {
    const double test_value = 123.456;
    component.setLegAllocQty(test_value);
    EXPECT_EQ(component.getLegAllocQty(), test_value);
    EXPECT_TRUE(component.hasLegAllocQty());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, SetLegAllocAcctIDSourceAndLegAllocAcctIDSourceMatch) {
    EXPECT_FALSE(component.hasLegAllocAcctIDSource());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, SetLegSettlCurrencyAndLegSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasLegSettlCurrency());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setLegAllocAccount(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setLegAllocQty(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    LegPreAllocGrp::NoLegAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getLegAllocQty(), component.getLegAllocQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setLegAllocAccount(std::string_view{"AAA"});
    component.setLegIndividualAllocID(std::string_view{"AAA"});
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().emplace_back();
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyID(std::string_view{"AAA"});
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyIDSource('X');
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyRole(12345);
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().getRefNstdPtys2SubGrp().getRefNoNested2PartySubIDsGroup().emplace_back();
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().getRefNstdPtys2SubGrp().getRefNoNested2PartySubIDsGroup().back().setNested2PartySubID(std::string_view{"AAA"});
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().getRefNstdPtys2SubGrp().getRefNoNested2PartySubIDsGroup().back().setNested2PartySubIDType(12345);
    component.setLegAllocQty(123.456);
    component.setLegAllocAcctIDSource(std::string_view{"AAA"});
    component.setLegSettlCurrency(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    LegPreAllocGrp::NoLegAllocs decoded;
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

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_LegPreAllocGrp_NoLegAllocsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
