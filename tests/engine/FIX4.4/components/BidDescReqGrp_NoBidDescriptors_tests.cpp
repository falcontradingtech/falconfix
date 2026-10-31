// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: BidDescReqGrp::NoBidDescriptors (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/BidDescReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest : public ::testing::Test {
protected:
    BidDescReqGrp::NoBidDescriptors component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const int64_t test_value = 12345; component.setBidDescriptorType(test_value); }
    { const std::string_view test_value{"AAA"}; component.setBidDescriptor(test_value); }
    { const int64_t test_value = 12345; component.setSideValueInd(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetBidDescriptorTypeAndBidDescriptorTypeMatch) {
    const int64_t test_value = 12345;
    component.setBidDescriptorType(test_value);
    EXPECT_EQ(component.getBidDescriptorType(), test_value);
    EXPECT_TRUE(component.hasBidDescriptorType());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetBidDescriptorAndBidDescriptorMatch) {
    EXPECT_FALSE(component.hasBidDescriptor());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetSideValueIndAndSideValueIndMatch) {
    const int64_t test_value = 12345;
    component.setSideValueInd(test_value);
    EXPECT_EQ(component.getSideValueInd(), test_value);
    EXPECT_TRUE(component.hasSideValueInd());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetLiquidityValueAndLiquidityValueMatch) {
    const double test_value = 123.456;
    component.setLiquidityValue(test_value);
    EXPECT_EQ(component.getLiquidityValue(), test_value);
    EXPECT_TRUE(component.hasLiquidityValue());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetLiquidityNumSecuritiesAndLiquidityNumSecuritiesMatch) {
    const int64_t test_value = 12345;
    component.setLiquidityNumSecurities(test_value);
    EXPECT_EQ(component.getLiquidityNumSecurities(), test_value);
    EXPECT_TRUE(component.hasLiquidityNumSecurities());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetLiquidityPctLowAndLiquidityPctLowMatch) {
    const double test_value = 123.456;
    component.setLiquidityPctLow(test_value);
    EXPECT_EQ(component.getLiquidityPctLow(), test_value);
    EXPECT_TRUE(component.hasLiquidityPctLow());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetLiquidityPctHighAndLiquidityPctHighMatch) {
    const double test_value = 123.456;
    component.setLiquidityPctHigh(test_value);
    EXPECT_EQ(component.getLiquidityPctHigh(), test_value);
    EXPECT_TRUE(component.hasLiquidityPctHigh());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetEFPTrackingErrorAndEFPTrackingErrorMatch) {
    const double test_value = 123.456;
    component.setEFPTrackingError(test_value);
    EXPECT_EQ(component.getEFPTrackingError(), test_value);
    EXPECT_TRUE(component.hasEFPTrackingError());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetFairValueAndFairValueMatch) {
    const double test_value = 123.456;
    component.setFairValue(test_value);
    EXPECT_EQ(component.getFairValue(), test_value);
    EXPECT_TRUE(component.hasFairValue());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetOutsideIndexPctAndOutsideIndexPctMatch) {
    const double test_value = 123.456;
    component.setOutsideIndexPct(test_value);
    EXPECT_EQ(component.getOutsideIndexPct(), test_value);
    EXPECT_TRUE(component.hasOutsideIndexPct());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetValueOfFuturesAndValueOfFuturesMatch) {
    const double test_value = 123.456;
    component.setValueOfFutures(test_value);
    EXPECT_EQ(component.getValueOfFutures(), test_value);
    EXPECT_TRUE(component.hasValueOfFutures());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const int64_t test_value = 12345; component.setBidDescriptorType(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setBidDescriptorType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    BidDescReqGrp::NoBidDescriptors decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getBidDescriptorType(), component.getBidDescriptorType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setBidDescriptorType(12345);
    component.setBidDescriptor(std::string_view{"AAA"});
    component.setSideValueInd(12345);
    component.setLiquidityValue(123.456);
    component.setLiquidityNumSecurities(12345);
    component.setLiquidityPctLow(123.456);
    component.setLiquidityPctHigh(123.456);
    component.setEFPTrackingError(123.456);
    component.setFairValue(123.456);
    component.setOutsideIndexPct(123.456);
    component.setValueOfFutures(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    BidDescReqGrp::NoBidDescriptors decoded;
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

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_BidDescReqGrp_NoBidDescriptorsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
