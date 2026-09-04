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

class MDFullGrpComponentTest : public ::testing::Test {
protected:
    MDFullGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(MDFullGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setMDEntryType('A');
    component.setMDEntryPx(3.14f);
    component.setMDEntrySize(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(MDFullGrpComponentTest, SetMDEntryTypeAndMDEntryTypeMatch) {
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    EXPECT_EQ(component.getMDEntryType(), test_value);
    EXPECT_TRUE(component.hasMDEntryType());
}

TEST_F(MDFullGrpComponentTest, SetMDEntryPxAndMDEntryPxMatch) {
    const double test_value = 123.456;
    component.setMDEntryPx(test_value);
    EXPECT_EQ(component.getMDEntryPx(), test_value);
    EXPECT_TRUE(component.hasMDEntryPx());
}

TEST_F(MDFullGrpComponentTest, SetMDEntrySizeAndMDEntrySizeMatch) {
    const double test_value = 123.456;
    component.setMDEntrySize(test_value);
    EXPECT_EQ(component.getMDEntrySize(), test_value);
    EXPECT_TRUE(component.hasMDEntrySize());
}

TEST_F(MDFullGrpComponentTest, SetMDEntryDateAndMDEntryDateMatch) {
}

TEST_F(MDFullGrpComponentTest, SetMDEntryTimeAndMDEntryTimeMatch) {
}

TEST_F(MDFullGrpComponentTest, SetTickDirectionAndTickDirectionMatch) {
    const char test_value = 'X';
    component.setTickDirection(test_value);
    EXPECT_EQ(component.getTickDirection(), test_value);
    EXPECT_TRUE(component.hasTickDirection());
}

TEST_F(MDFullGrpComponentTest, SetTimeInForceAndTimeInForceMatch) {
    const char test_value = 'X';
    component.setTimeInForce(test_value);
    EXPECT_EQ(component.getTimeInForce(), test_value);
    EXPECT_TRUE(component.hasTimeInForce());
}

TEST_F(MDFullGrpComponentTest, SetExpireDateAndExpireDateMatch) {
}

TEST_F(MDFullGrpComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(MDFullGrpComponentTest, SetMinQtyAndMinQtyMatch) {
    const double test_value = 123.456;
    component.setMinQty(test_value);
    EXPECT_EQ(component.getMinQty(), test_value);
    EXPECT_TRUE(component.hasMinQty());
}

TEST_F(MDFullGrpComponentTest, SetSellerDaysAndSellerDaysMatch) {
    const int64_t test_value = 12345;
    component.setSellerDays(test_value);
    EXPECT_EQ(component.getSellerDays(), test_value);
    EXPECT_TRUE(component.hasSellerDays());
}

TEST_F(MDFullGrpComponentTest, SetNumberOfOrdersAndNumberOfOrdersMatch) {
    const int64_t test_value = 12345;
    component.setNumberOfOrders(test_value);
    EXPECT_EQ(component.getNumberOfOrders(), test_value);
    EXPECT_TRUE(component.hasNumberOfOrders());
}

TEST_F(MDFullGrpComponentTest, SetMDEntryPositionNoAndMDEntryPositionNoMatch) {
    const int64_t test_value = 12345;
    component.setMDEntryPositionNo(test_value);
    EXPECT_EQ(component.getMDEntryPositionNo(), test_value);
    EXPECT_TRUE(component.hasMDEntryPositionNo());
}

TEST_F(MDFullGrpComponentTest, SetPriceDeltaAndPriceDeltaMatch) {
    const double test_value = 123.456;
    component.setPriceDelta(test_value);
    EXPECT_EQ(component.getPriceDelta(), test_value);
    EXPECT_TRUE(component.hasPriceDelta());
}

TEST_F(MDFullGrpComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(MDFullGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setMDEntryType('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(MDFullGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1914];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    MDFullGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(MDFullGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
