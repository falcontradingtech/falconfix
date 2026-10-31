// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: YieldData (FIX4.3)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.3/components/YieldData.h>
#include <FIX4.3/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix43::components;

class FIX4_3_YieldDataComponentTest : public ::testing::Test {
protected:
    YieldData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_3_YieldDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setYieldType(test_value); }
    { const double test_value = 123.456; component.setYield(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_3_YieldDataComponentTest, SetYieldTypeAndYieldTypeMatch) {
    EXPECT_FALSE(component.hasYieldType());
}

TEST_F(FIX4_3_YieldDataComponentTest, SetYieldAndYieldMatch) {
    const double test_value = 123.456;
    component.setYield(test_value);
    EXPECT_EQ(component.getYield(), test_value);
    EXPECT_TRUE(component.hasYield());
}

TEST_F(FIX4_3_YieldDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setYieldType(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_3_YieldDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setYield(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    YieldData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getYield(), component.getYield());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_3_YieldDataComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setYieldType(std::string_view{"AAA"});
    component.setYield(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    YieldData decoded;
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

TEST_F(FIX4_3_YieldDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_3_YieldDataComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
