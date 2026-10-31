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

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, FullyPopulatedRoundtrip) {
    char buffer[7656];
    component.setMDEntryType('X');
    component.setMDEntryPx(123.456);
    component.setCurrency(std::string_view{"AAA"});
    component.setMDEntrySize(123.456);
    component.setMDEntryDate(12345);
    component.setMDEntryTime(12345);
    component.setTickDirection('X');
    component.setMDMkt(std::string_view{"AAA"});
    component.setTradingSessionID(std::string_view{"AAA"});
    component.setTradingSessionSubID(std::string_view{"AAA"});
    component.setQuoteCondition(std::string_view{"AAA"});
    component.setTradeCondition(std::string_view{"AAA"});
    component.setMDEntryOriginator(std::string_view{"AAA"});
    component.setLocationID(std::string_view{"AAA"});
    component.setDeskID(std::string_view{"AAA"});
    component.setOpenCloseSettlFlag(std::string_view{"AAA"});
    component.setTimeInForce('X');
    component.setExpireDate(12345);
    component.setExpireTime(12345);
    component.setMinQty(123.456);
    component.setExecInst(std::string_view{"AAA"});
    component.setSellerDays(12345);
    component.setOrderID(std::string_view{"AAA"});
    component.setQuoteEntryID(std::string_view{"AAA"});
    component.setMDEntryBuyer(std::string_view{"AAA"});
    component.setMDEntrySeller(std::string_view{"AAA"});
    component.setNumberOfOrders(12345);
    component.setMDEntryPositionNo(12345);
    component.setScope(std::string_view{"AAA"});
    component.setPriceDelta(123.456);
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    MDFullGrp::NoMDEntries decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[7656];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_MDFullGrp_NoMDEntriesComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
