// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlInstructionsData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlInstructionsData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SettlInstructionsDataComponentTest : public ::testing::Test {
protected:
    SettlInstructionsData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SettlInstructionsDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSettlDeliveryType(42);
    component.setStandInstDbType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SettlInstructionsDataComponentTest, SetSettlDeliveryTypeAndSettlDeliveryTypeMatch) {
    const int64_t test_value = 12345;
    component.setSettlDeliveryType(test_value);
    EXPECT_EQ(component.getSettlDeliveryType(), test_value);
    EXPECT_TRUE(component.hasSettlDeliveryType());
}

TEST_F(SettlInstructionsDataComponentTest, SetStandInstDbTypeAndStandInstDbTypeMatch) {
    const int64_t test_value = 12345;
    component.setStandInstDbType(test_value);
    EXPECT_EQ(component.getStandInstDbType(), test_value);
    EXPECT_TRUE(component.hasStandInstDbType());
}

TEST_F(SettlInstructionsDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSettlDeliveryType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SettlInstructionsDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[248];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setSettlDeliveryType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SettlInstructionsData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSettlDeliveryType(), component.getSettlDeliveryType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(SettlInstructionsDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
