// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: BidCompReqGrp::NoBidComponents (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/BidCompReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest : public ::testing::Test {
protected:
    BidCompReqGrp::NoBidComponents component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setListID(test_value); }
    { const char test_value = 'X'; component.setSide(test_value); }
    { const std::string_view test_value{"AAA"}; component.setTradingSessionID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetListIDAndListIDMatch) {
    EXPECT_FALSE(component.hasListID());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetTradingSessionIDAndTradingSessionIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionID());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetTradingSessionSubIDAndTradingSessionSubIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionSubID());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetNetGrossIndAndNetGrossIndMatch) {
    const int64_t test_value = 12345;
    component.setNetGrossInd(test_value);
    EXPECT_EQ(component.getNetGrossInd(), test_value);
    EXPECT_TRUE(component.hasNetGrossInd());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetSettlTypeAndSettlTypeMatch) {
    const char test_value = 'X';
    component.setSettlType(test_value);
    EXPECT_EQ(component.getSettlType(), test_value);
    EXPECT_TRUE(component.hasSettlType());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetAccountAndAccountMatch) {
    EXPECT_FALSE(component.hasAccount());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setListID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, EncodeDecodeRoundtrip) {
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
    BidCompReqGrp::NoBidComponents decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSide(), component.getSide());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setListID(std::string_view{"AAA"});
    component.setSide('X');
    component.setTradingSessionID(std::string_view{"AAA"});
    component.setTradingSessionSubID(std::string_view{"AAA"});
    component.setNetGrossInd(12345);
    component.setSettlType('X');
    component.setSettlDate(12345);
    component.setAccount(std::string_view{"AAA"});
    component.setAcctIDSource(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    BidCompReqGrp::NoBidComponents decoded;
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

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_BidCompReqGrp_NoBidComponentsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
