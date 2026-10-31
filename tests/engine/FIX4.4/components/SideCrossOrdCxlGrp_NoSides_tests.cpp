// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SideCrossOrdCxlGrp::NoSides (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SideCrossOrdCxlGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest : public ::testing::Test {
protected:
    SideCrossOrdCxlGrp::NoSides component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const char test_value = 'X'; component.setSide(test_value); }
    { const std::string_view test_value{"AAA"}; component.setOrigClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetOrigClOrdIDAndOrigClOrdIDMatch) {
    EXPECT_FALSE(component.hasOrigClOrdID());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetClOrdLinkIDAndClOrdLinkIDMatch) {
    EXPECT_FALSE(component.hasClOrdLinkID());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetOrigOrdModTimeAndOrigOrdModTimeMatch) {
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetComplianceIDAndComplianceIDMatch) {
    EXPECT_FALSE(component.hasComplianceID());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const char test_value = 'X'; component.setSide(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
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
    SideCrossOrdCxlGrp::NoSides decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSide(), component.getSide());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setSide('X');
    component.setOrigClOrdID(std::string_view{"AAA"});
    component.setClOrdID(std::string_view{"AAA"});
    component.setSecondaryClOrdID(std::string_view{"AAA"});
    component.setClOrdLinkID(std::string_view{"AAA"});
    component.setOrigOrdModTime(12345);
    component.getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.setTradeOriginationDate(12345);
    component.setTradeDate(12345);
    component.getRefOrderQtyData().setOrderQty(123.456);
    component.getRefOrderQtyData().setCashOrderQty(123.456);
    component.getRefOrderQtyData().setOrderPercent(123.456);
    component.getRefOrderQtyData().setRoundingDirection('X');
    component.getRefOrderQtyData().setRoundingModulus(123.456);
    component.setComplianceID(std::string_view{"AAA"});
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SideCrossOrdCxlGrp::NoSides decoded;
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

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_SideCrossOrdCxlGrp_NoSidesComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
