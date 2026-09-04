// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SpreadOrBenchmarkCurveData (FIX4.3)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.3/components/SpreadOrBenchmarkCurveData.h>
#include <FIX4.3/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix43::components;

class FIX4_3_SpreadOrBenchmarkCurveDataComponentTest : public ::testing::Test {
protected:
    SpreadOrBenchmarkCurveData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_3_SpreadOrBenchmarkCurveDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSpread(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_3_SpreadOrBenchmarkCurveDataComponentTest, SetSpreadAndSpreadMatch) {
    const double test_value = 123.456;
    component.setSpread(test_value);
    EXPECT_EQ(component.getSpread(), test_value);
    EXPECT_TRUE(component.hasSpread());
}

TEST_F(FIX4_3_SpreadOrBenchmarkCurveDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSpread(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_3_SpreadOrBenchmarkCurveDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[280];
    
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

TEST_F(FIX4_3_SpreadOrBenchmarkCurveDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
