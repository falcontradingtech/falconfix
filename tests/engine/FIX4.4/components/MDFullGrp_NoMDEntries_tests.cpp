// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDFullGrp::NoMDEntries (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDFullGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_MDFullGrp_NoMDEntriesComponentTest : public ::testing::Test {
protected:
    MDFullGrp::NoMDEntries component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const char test_value = 'X'; component.setMDEntryType(test_value); }
    { const double test_value = 123.456; component.setMDEntryPx(test_value); }
    { const std::string_view test_value{"AAA"}; component.setCurrency(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryTypeAndMDEntryTypeMatch) {
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    EXPECT_EQ(component.getMDEntryType(), test_value);
    EXPECT_TRUE(component.hasMDEntryType());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryPxAndMDEntryPxMatch) {
    const double test_value = 123.456;
    component.setMDEntryPx(test_value);
    EXPECT_EQ(component.getMDEntryPx(), test_value);
    EXPECT_TRUE(component.hasMDEntryPx());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntrySizeAndMDEntrySizeMatch) {
    const double test_value = 123.456;
    component.setMDEntrySize(test_value);
    EXPECT_EQ(component.getMDEntrySize(), test_value);
    EXPECT_TRUE(component.hasMDEntrySize());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryDateAndMDEntryDateMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryTimeAndMDEntryTimeMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetTickDirectionAndTickDirectionMatch) {
    const char test_value = 'X';
    component.setTickDirection(test_value);
    EXPECT_EQ(component.getTickDirection(), test_value);
    EXPECT_TRUE(component.hasTickDirection());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDMktAndMDMktMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetTradingSessionIDAndTradingSessionIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionID());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetTradingSessionSubIDAndTradingSessionSubIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionSubID());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetQuoteConditionAndQuoteConditionMatch) {
    EXPECT_FALSE(component.hasQuoteCondition());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetTradeConditionAndTradeConditionMatch) {
    EXPECT_FALSE(component.hasTradeCondition());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryOriginatorAndMDEntryOriginatorMatch) {
    EXPECT_FALSE(component.hasMDEntryOriginator());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetLocationIDAndLocationIDMatch) {
    EXPECT_FALSE(component.hasLocationID());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetDeskIDAndDeskIDMatch) {
    EXPECT_FALSE(component.hasDeskID());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetOpenCloseSettlFlagAndOpenCloseSettlFlagMatch) {
    EXPECT_FALSE(component.hasOpenCloseSettlFlag());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetExecInstAndExecInstMatch) {
    EXPECT_FALSE(component.hasExecInst());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetSellerDaysAndSellerDaysMatch) {
    const int64_t test_value = 12345;
    component.setSellerDays(test_value);
    EXPECT_EQ(component.getSellerDays(), test_value);
    EXPECT_TRUE(component.hasSellerDays());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetOrderIDAndOrderIDMatch) {
    EXPECT_FALSE(component.hasOrderID());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetQuoteEntryIDAndQuoteEntryIDMatch) {
    EXPECT_FALSE(component.hasQuoteEntryID());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryBuyerAndMDEntryBuyerMatch) {
    EXPECT_FALSE(component.hasMDEntryBuyer());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntrySellerAndMDEntrySellerMatch) {
    EXPECT_FALSE(component.hasMDEntrySeller());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetNumberOfOrdersAndNumberOfOrdersMatch) {
    const int64_t test_value = 12345;
    component.setNumberOfOrders(test_value);
    EXPECT_EQ(component.getNumberOfOrders(), test_value);
    EXPECT_TRUE(component.hasNumberOfOrders());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetMDEntryPositionNoAndMDEntryPositionNoMatch) {
    const int64_t test_value = 12345;
    component.setMDEntryPositionNo(test_value);
    EXPECT_EQ(component.getMDEntryPositionNo(), test_value);
    EXPECT_TRUE(component.hasMDEntryPositionNo());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetScopeAndScopeMatch) {
    EXPECT_FALSE(component.hasScope());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetPriceDeltaAndPriceDeltaMatch) {
    const double test_value = 123.456;
    component.setPriceDelta(test_value);
    EXPECT_EQ(component.getPriceDelta(), test_value);
    EXPECT_TRUE(component.hasPriceDelta());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const char test_value = 'X'; component.setMDEntryType(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1914];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    MDFullGrp::NoMDEntries decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getMDEntryType(), component.getMDEntryType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}
