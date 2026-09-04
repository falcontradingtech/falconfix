// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: BidDescReqGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/BidDescReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class BidDescReqGrpComponentTest : public ::testing::Test {
protected:
    BidDescReqGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(BidDescReqGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setBidDescriptorType(42);
    component.setSideValueInd(42);
    component.setLiquidityValue(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(BidDescReqGrpComponentTest, SetBidDescriptorTypeAndBidDescriptorTypeMatch) {
    const int64_t test_value = 12345;
    component.setBidDescriptorType(test_value);
    EXPECT_EQ(component.getBidDescriptorType(), test_value);
    EXPECT_TRUE(component.hasBidDescriptorType());
}

TEST_F(BidDescReqGrpComponentTest, SetSideValueIndAndSideValueIndMatch) {
    const int64_t test_value = 12345;
    component.setSideValueInd(test_value);
    EXPECT_EQ(component.getSideValueInd(), test_value);
    EXPECT_TRUE(component.hasSideValueInd());
}

TEST_F(BidDescReqGrpComponentTest, SetLiquidityValueAndLiquidityValueMatch) {
    const double test_value = 123.456;
    component.setLiquidityValue(test_value);
    EXPECT_EQ(component.getLiquidityValue(), test_value);
    EXPECT_TRUE(component.hasLiquidityValue());
}

TEST_F(BidDescReqGrpComponentTest, SetLiquidityNumSecuritiesAndLiquidityNumSecuritiesMatch) {
    const int64_t test_value = 12345;
    component.setLiquidityNumSecurities(test_value);
    EXPECT_EQ(component.getLiquidityNumSecurities(), test_value);
    EXPECT_TRUE(component.hasLiquidityNumSecurities());
}

TEST_F(BidDescReqGrpComponentTest, SetLiquidityPctLowAndLiquidityPctLowMatch) {
    const double test_value = 123.456;
    component.setLiquidityPctLow(test_value);
    EXPECT_EQ(component.getLiquidityPctLow(), test_value);
    EXPECT_TRUE(component.hasLiquidityPctLow());
}

TEST_F(BidDescReqGrpComponentTest, SetLiquidityPctHighAndLiquidityPctHighMatch) {
    const double test_value = 123.456;
    component.setLiquidityPctHigh(test_value);
    EXPECT_EQ(component.getLiquidityPctHigh(), test_value);
    EXPECT_TRUE(component.hasLiquidityPctHigh());
}

TEST_F(BidDescReqGrpComponentTest, SetEFPTrackingErrorAndEFPTrackingErrorMatch) {
    const double test_value = 123.456;
    component.setEFPTrackingError(test_value);
    EXPECT_EQ(component.getEFPTrackingError(), test_value);
    EXPECT_TRUE(component.hasEFPTrackingError());
}

TEST_F(BidDescReqGrpComponentTest, SetFairValueAndFairValueMatch) {
    const double test_value = 123.456;
    component.setFairValue(test_value);
    EXPECT_EQ(component.getFairValue(), test_value);
    EXPECT_TRUE(component.hasFairValue());
}

TEST_F(BidDescReqGrpComponentTest, SetOutsideIndexPctAndOutsideIndexPctMatch) {
    const double test_value = 123.456;
    component.setOutsideIndexPct(test_value);
    EXPECT_EQ(component.getOutsideIndexPct(), test_value);
    EXPECT_TRUE(component.hasOutsideIndexPct());
}

TEST_F(BidDescReqGrpComponentTest, SetValueOfFuturesAndValueOfFuturesMatch) {
    const double test_value = 123.456;
    component.setValueOfFutures(test_value);
    EXPECT_EQ(component.getValueOfFutures(), test_value);
    EXPECT_TRUE(component.hasValueOfFutures());
}

TEST_F(BidDescReqGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setBidDescriptorType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(BidDescReqGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[630];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    BidDescReqGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(BidDescReqGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
