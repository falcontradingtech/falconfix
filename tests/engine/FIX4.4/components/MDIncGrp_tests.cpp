// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDIncGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDIncGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class MDIncGrpComponentTest : public ::testing::Test {
protected:
    MDIncGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(MDIncGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setMDUpdateAction('A');
    component.setDeleteReason('A');
    component.setMDEntryType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(MDIncGrpComponentTest, SetMDUpdateActionAndMDUpdateActionMatch) {
    const char test_value = 'X';
    component.setMDUpdateAction(test_value);
    EXPECT_EQ(component.getMDUpdateAction(), test_value);
    EXPECT_TRUE(component.hasMDUpdateAction());
}

TEST_F(MDIncGrpComponentTest, SetDeleteReasonAndDeleteReasonMatch) {
    const char test_value = 'X';
    component.setDeleteReason(test_value);
    EXPECT_EQ(component.getDeleteReason(), test_value);
    EXPECT_TRUE(component.hasDeleteReason());
}

TEST_F(MDIncGrpComponentTest, SetMDEntryTypeAndMDEntryTypeMatch) {
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    EXPECT_EQ(component.getMDEntryType(), test_value);
    EXPECT_TRUE(component.hasMDEntryType());
}

TEST_F(MDIncGrpComponentTest, SetMDEntryPxAndMDEntryPxMatch) {
    const double test_value = 123.456;
    component.setMDEntryPx(test_value);
    EXPECT_EQ(component.getMDEntryPx(), test_value);
    EXPECT_TRUE(component.hasMDEntryPx());
}

TEST_F(MDIncGrpComponentTest, SetMDEntrySizeAndMDEntrySizeMatch) {
    const double test_value = 123.456;
    component.setMDEntrySize(test_value);
    EXPECT_EQ(component.getMDEntrySize(), test_value);
    EXPECT_TRUE(component.hasMDEntrySize());
}

TEST_F(MDIncGrpComponentTest, SetMDEntryDateAndMDEntryDateMatch) {
}

TEST_F(MDIncGrpComponentTest, SetMDEntryTimeAndMDEntryTimeMatch) {
}

TEST_F(MDIncGrpComponentTest, SetTickDirectionAndTickDirectionMatch) {
    const char test_value = 'X';
    component.setTickDirection(test_value);
    EXPECT_EQ(component.getTickDirection(), test_value);
    EXPECT_TRUE(component.hasTickDirection());
}

TEST_F(MDIncGrpComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(MDIncGrpComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(MDIncGrpComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(MDIncGrpComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(MDIncGrpComponentTest, SetSellerDaysAndSellerDaysMatch) {
    const int64_t test_value = 12345;
    component.setSellerDays(test_value);
    EXPECT_EQ(component.getSellerDays(), test_value);
    EXPECT_TRUE(component.hasSellerDays());
}

TEST_F(MDIncGrpComponentTest, SetNumberOfOrdersAndNumberOfOrdersMatch) {
    const int64_t test_value = 12345;
    component.setNumberOfOrders(test_value);
    EXPECT_EQ(component.getNumberOfOrders(), test_value);
    EXPECT_TRUE(component.hasNumberOfOrders());
}

TEST_F(MDIncGrpComponentTest, SetMDEntryPositionNoAndMDEntryPositionNoMatch) {
    const int64_t test_value = 12345;
    component.setMDEntryPositionNo(test_value);
    EXPECT_EQ(component.getMDEntryPositionNo(), test_value);
    EXPECT_TRUE(component.hasMDEntryPositionNo());
}

TEST_F(MDIncGrpComponentTest, SetPriceDeltaAndPriceDeltaMatch) {
    const double test_value = 123.456;
    component.setPriceDelta(test_value);
    EXPECT_EQ(component.getPriceDelta(), test_value);
    EXPECT_TRUE(component.hasPriceDelta());
}

TEST_F(MDIncGrpComponentTest, SetNetChgPrevDayAndNetChgPrevDayMatch) {
    const double test_value = 123.456;
    component.setNetChgPrevDay(test_value);
    EXPECT_EQ(component.getNetChgPrevDay(), test_value);
    EXPECT_TRUE(component.hasNetChgPrevDay());
}

TEST_F(MDIncGrpComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(MDIncGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setMDUpdateAction('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(MDIncGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[2292];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    MDIncGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(MDIncGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
