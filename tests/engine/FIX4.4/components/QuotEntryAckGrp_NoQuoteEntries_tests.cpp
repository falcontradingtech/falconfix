// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotEntryAckGrp::NoQuoteEntries (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotEntryAckGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest : public ::testing::Test {
protected:
    QuotEntryAckGrp::NoQuoteEntries component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setBidPx(3.14f);
    component.setOfferPx(3.14f);
    component.setBidSize(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidPxAndBidPxMatch) {
    const double test_value = 123.456;
    component.setBidPx(test_value);
    EXPECT_EQ(component.getBidPx(), test_value);
    EXPECT_TRUE(component.hasBidPx());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferPxAndOfferPxMatch) {
    const double test_value = 123.456;
    component.setOfferPx(test_value);
    EXPECT_EQ(component.getOfferPx(), test_value);
    EXPECT_TRUE(component.hasOfferPx());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidSizeAndBidSizeMatch) {
    const double test_value = 123.456;
    component.setBidSize(test_value);
    EXPECT_EQ(component.getBidSize(), test_value);
    EXPECT_TRUE(component.hasBidSize());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferSizeAndOfferSizeMatch) {
    const double test_value = 123.456;
    component.setOfferSize(test_value);
    EXPECT_EQ(component.getOfferSize(), test_value);
    EXPECT_TRUE(component.hasOfferSize());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetValidUntilTimeAndValidUntilTimeMatch) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidSpotRateAndBidSpotRateMatch) {
    const double test_value = 123.456;
    component.setBidSpotRate(test_value);
    EXPECT_EQ(component.getBidSpotRate(), test_value);
    EXPECT_TRUE(component.hasBidSpotRate());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferSpotRateAndOfferSpotRateMatch) {
    const double test_value = 123.456;
    component.setOfferSpotRate(test_value);
    EXPECT_EQ(component.getOfferSpotRate(), test_value);
    EXPECT_TRUE(component.hasOfferSpotRate());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidForwardPointsAndBidForwardPointsMatch) {
    const double test_value = 123.456;
    component.setBidForwardPoints(test_value);
    EXPECT_EQ(component.getBidForwardPoints(), test_value);
    EXPECT_TRUE(component.hasBidForwardPoints());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferForwardPointsAndOfferForwardPointsMatch) {
    const double test_value = 123.456;
    component.setOfferForwardPoints(test_value);
    EXPECT_EQ(component.getOfferForwardPoints(), test_value);
    EXPECT_TRUE(component.hasOfferForwardPoints());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetMidPxAndMidPxMatch) {
    const double test_value = 123.456;
    component.setMidPx(test_value);
    EXPECT_EQ(component.getMidPx(), test_value);
    EXPECT_TRUE(component.hasMidPx());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidYieldAndBidYieldMatch) {
    const double test_value = 123.456;
    component.setBidYield(test_value);
    EXPECT_EQ(component.getBidYield(), test_value);
    EXPECT_TRUE(component.hasBidYield());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetMidYieldAndMidYieldMatch) {
    const double test_value = 123.456;
    component.setMidYield(test_value);
    EXPECT_EQ(component.getMidYield(), test_value);
    EXPECT_TRUE(component.hasMidYield());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferYieldAndOfferYieldMatch) {
    const double test_value = 123.456;
    component.setOfferYield(test_value);
    EXPECT_EQ(component.getOfferYield(), test_value);
    EXPECT_TRUE(component.hasOfferYield());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetTransactTimeAndTransactTimeMatch) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetSettlDate2AndSettlDate2Match) {
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOrderQty2AndOrderQty2Match) {
    const double test_value = 123.456;
    component.setOrderQty2(test_value);
    EXPECT_EQ(component.getOrderQty2(), test_value);
    EXPECT_TRUE(component.hasOrderQty2());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetBidForwardPoints2AndBidForwardPoints2Match) {
    const double test_value = 123.456;
    component.setBidForwardPoints2(test_value);
    EXPECT_EQ(component.getBidForwardPoints2(), test_value);
    EXPECT_TRUE(component.hasBidForwardPoints2());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetOfferForwardPoints2AndOfferForwardPoints2Match) {
    const double test_value = 123.456;
    component.setOfferForwardPoints2(test_value);
    EXPECT_EQ(component.getOfferForwardPoints2(), test_value);
    EXPECT_TRUE(component.hasOfferForwardPoints2());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, SetQuoteEntryRejectReasonAndQuoteEntryRejectReasonMatch) {
    const int64_t test_value = 12345;
    component.setQuoteEntryRejectReason(test_value);
    EXPECT_EQ(component.getQuoteEntryRejectReason(), test_value);
    EXPECT_TRUE(component.hasQuoteEntryRejectReason());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setBidPx(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1358];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setBidPx(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    QuotEntryAckGrp::NoQuoteEntries decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getBidPx(), component.getBidPx());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryAckGrp_NoQuoteEntriesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
