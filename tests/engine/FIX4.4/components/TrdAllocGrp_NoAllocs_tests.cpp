// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: TrdAllocGrp::NoAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/TrdAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_TrdAllocGrp_NoAllocsComponentTest : public ::testing::Test {
protected:
    TrdAllocGrp::NoAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setAllocAccount(test_value); }
    { const int64_t test_value = 12345; component.setAllocAcctIDSource(test_value); }
    { const std::string_view test_value{"AAA"}; component.setAllocSettlCurrency(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, SetAllocAccountAndAllocAccountMatch) {
    EXPECT_FALSE(component.hasAllocAccount());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, SetAllocAcctIDSourceAndAllocAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    EXPECT_EQ(component.getAllocAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAllocAcctIDSource());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, SetAllocSettlCurrencyAndAllocSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasAllocSettlCurrency());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, SetIndividualAllocIDAndIndividualAllocIDMatch) {
    EXPECT_FALSE(component.hasIndividualAllocID());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, SetAllocQtyAndAllocQtyMatch) {
    const double test_value = 123.456;
    component.setAllocQty(test_value);
    EXPECT_EQ(component.getAllocQty(), test_value);
    EXPECT_TRUE(component.hasAllocQty());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setAllocAccount(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
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
    TrdAllocGrp::NoAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getAllocAcctIDSource(), component.getAllocAcctIDSource());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setAllocAccount(std::string_view{"AAA"});
    component.setAllocAcctIDSource(12345);
    component.setAllocSettlCurrency(std::string_view{"AAA"});
    component.setIndividualAllocID(std::string_view{"AAA"});
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().emplace_back();
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyID(std::string_view{"AAA"});
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyIDSource('X');
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().setNested2PartyRole(12345);
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().getRefNstdPtys2SubGrp().getRefNoNested2PartySubIDsGroup().emplace_back();
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().getRefNstdPtys2SubGrp().getRefNoNested2PartySubIDsGroup().back().setNested2PartySubID(std::string_view{"AAA"});
    component.getRefNestedParties2().getRefNoNested2PartyIDsGroup().back().getRefNstdPtys2SubGrp().getRefNoNested2PartySubIDsGroup().back().setNested2PartySubIDType(12345);
    component.setAllocQty(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    TrdAllocGrp::NoAllocs decoded;
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

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_TrdAllocGrp_NoAllocsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
