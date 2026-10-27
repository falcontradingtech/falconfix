// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SpreadOrBenchmarkCurveData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SpreadOrBenchmarkCurveData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SpreadOrBenchmarkCurveDataComponentTest : public ::testing::Test {
protected:
    SpreadOrBenchmarkCurveData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const double test_value = 123.456; component.setSpread(test_value); }
    { const std::string_view test_value{"AAA"}; component.setBenchmarkCurveCurrency(test_value); }
    { const std::string_view test_value{"AAA"}; component.setBenchmarkCurveName(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetSpreadAndSpreadMatch) {
    const double test_value = 123.456;
    component.setSpread(test_value);
    EXPECT_EQ(component.getSpread(), test_value);
    EXPECT_TRUE(component.hasSpread());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkCurveCurrencyAndBenchmarkCurveCurrencyMatch) {
    EXPECT_FALSE(component.hasBenchmarkCurveCurrency());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkCurveNameAndBenchmarkCurveNameMatch) {
    EXPECT_FALSE(component.hasBenchmarkCurveName());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkCurvePointAndBenchmarkCurvePointMatch) {
    EXPECT_FALSE(component.hasBenchmarkCurvePoint());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkPriceAndBenchmarkPriceMatch) {
    const double test_value = 123.456;
    component.setBenchmarkPrice(test_value);
    EXPECT_EQ(component.getBenchmarkPrice(), test_value);
    EXPECT_TRUE(component.hasBenchmarkPrice());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkPriceTypeAndBenchmarkPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setBenchmarkPriceType(test_value);
    EXPECT_EQ(component.getBenchmarkPriceType(), test_value);
    EXPECT_TRUE(component.hasBenchmarkPriceType());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkSecurityIDAndBenchmarkSecurityIDMatch) {
    EXPECT_FALSE(component.hasBenchmarkSecurityID());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, SetBenchmarkSecurityIDSourceAndBenchmarkSecurityIDSourceMatch) {
    EXPECT_FALSE(component.hasBenchmarkSecurityIDSource());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const double test_value = 123.456; component.setSpread(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setSpread(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SpreadOrBenchmarkCurveData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSpread(), component.getSpread());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SpreadOrBenchmarkCurveDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
