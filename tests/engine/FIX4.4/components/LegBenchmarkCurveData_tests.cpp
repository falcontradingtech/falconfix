// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegBenchmarkCurveData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegBenchmarkCurveData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class LegBenchmarkCurveDataComponentTest : public ::testing::Test {
protected:
    LegBenchmarkCurveData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(LegBenchmarkCurveDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLegBenchmarkPrice(3.14f);
    component.setLegBenchmarkPriceType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(LegBenchmarkCurveDataComponentTest, SetLegBenchmarkPriceAndLegBenchmarkPriceMatch) {
    const double test_value = 123.456;
    component.setLegBenchmarkPrice(test_value);
    EXPECT_EQ(component.getLegBenchmarkPrice(), test_value);
    EXPECT_TRUE(component.hasLegBenchmarkPrice());
}

TEST_F(LegBenchmarkCurveDataComponentTest, SetLegBenchmarkPriceTypeAndLegBenchmarkPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setLegBenchmarkPriceType(test_value);
    EXPECT_EQ(component.getLegBenchmarkPriceType(), test_value);
    EXPECT_TRUE(component.hasLegBenchmarkPriceType());
}

TEST_F(LegBenchmarkCurveDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLegBenchmarkPrice(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(LegBenchmarkCurveDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[330];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    LegBenchmarkCurveData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(LegBenchmarkCurveDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
