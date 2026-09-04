// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDIncGrp::NoMDEntries (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDIncGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class MDIncGrp_NoMDEntriesComponentTest : public ::testing::Test {
protected:
    MDIncGrp::NoMDEntries component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(MDIncGrp_NoMDEntriesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setMDUpdateAction('A');
    component.setDeleteReason('A');
    component.setMDEntryType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDUpdateActionAndMDUpdateActionMatch) {
    const char test_value = 'X';
    component.setMDUpdateAction(test_value);
    EXPECT_EQ(component.getMDUpdateAction(), test_value);
    EXPECT_TRUE(component.hasMDUpdateAction());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetDeleteReasonAndDeleteReasonMatch) {
    const char test_value = 'X';
    component.setDeleteReason(test_value);
    EXPECT_EQ(component.getDeleteReason(), test_value);
    EXPECT_TRUE(component.hasDeleteReason());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDEntryTypeAndMDEntryTypeMatch) {
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    EXPECT_EQ(component.getMDEntryType(), test_value);
    EXPECT_TRUE(component.hasMDEntryType());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDEntryPxAndMDEntryPxMatch) {
    const double test_value = 123.456;
    component.setMDEntryPx(test_value);
    EXPECT_EQ(component.getMDEntryPx(), test_value);
    EXPECT_TRUE(component.hasMDEntryPx());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDEntrySizeAndMDEntrySizeMatch) {
    const double test_value = 123.456;
    component.setMDEntrySize(test_value);
    EXPECT_EQ(component.getMDEntrySize(), test_value);
    EXPECT_TRUE(component.hasMDEntrySize());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDEntryDateAndMDEntryDateMatch) {
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDEntryTimeAndMDEntryTimeMatch) {
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetTickDirectionAndTickDirectionMatch) {
    const char test_value = 'X';
    component.setTickDirection(test_value);
    EXPECT_EQ(component.getTickDirection(), test_value);
    EXPECT_TRUE(component.hasTickDirection());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetSellerDaysAndSellerDaysMatch) {
    const int64_t test_value = 12345;
    component.setSellerDays(test_value);
    EXPECT_EQ(component.getSellerDays(), test_value);
    EXPECT_TRUE(component.hasSellerDays());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetNumberOfOrdersAndNumberOfOrdersMatch) {
    const int64_t test_value = 12345;
    component.setNumberOfOrders(test_value);
    EXPECT_EQ(component.getNumberOfOrders(), test_value);
    EXPECT_TRUE(component.hasNumberOfOrders());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetMDEntryPositionNoAndMDEntryPositionNoMatch) {
    const int64_t test_value = 12345;
    component.setMDEntryPositionNo(test_value);
    EXPECT_EQ(component.getMDEntryPositionNo(), test_value);
    EXPECT_TRUE(component.hasMDEntryPositionNo());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetPriceDeltaAndPriceDeltaMatch) {
    const double test_value = 123.456;
    component.setPriceDelta(test_value);
    EXPECT_EQ(component.getPriceDelta(), test_value);
    EXPECT_TRUE(component.hasPriceDelta());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetNetChgPrevDayAndNetChgPrevDayMatch) {
    const double test_value = 123.456;
    component.setNetChgPrevDay(test_value);
    EXPECT_EQ(component.getNetChgPrevDay(), test_value);
    EXPECT_TRUE(component.hasNetChgPrevDay());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setMDUpdateAction('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2292];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setMDUpdateAction(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    MDIncGrp::NoMDEntries decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getMDUpdateAction(), component.getMDUpdateAction());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(MDIncGrp_NoMDEntriesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}
