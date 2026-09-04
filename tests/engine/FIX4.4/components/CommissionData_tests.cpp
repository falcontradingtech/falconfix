// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: CommissionData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/CommissionData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_CommissionDataComponentTest : public ::testing::Test {
protected:
    CommissionData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_CommissionDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setCommission(3.14f);
    component.setCommType('A');
    component.setFundRenewWaiv('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_CommissionDataComponentTest, SetCommissionAndCommissionMatch) {
    const double test_value = 123.456;
    component.setCommission(test_value);
    EXPECT_EQ(component.getCommission(), test_value);
    EXPECT_TRUE(component.hasCommission());
}

TEST_F(FIX4_4_CommissionDataComponentTest, SetCommTypeAndCommTypeMatch) {
    const char test_value = 'X';
    component.setCommType(test_value);
    EXPECT_EQ(component.getCommType(), test_value);
    EXPECT_TRUE(component.hasCommType());
}

TEST_F(FIX4_4_CommissionDataComponentTest, SetFundRenewWaivAndFundRenewWaivMatch) {
    const char test_value = 'X';
    component.setFundRenewWaiv(test_value);
    EXPECT_EQ(component.getFundRenewWaiv(), test_value);
    EXPECT_TRUE(component.hasFundRenewWaiv());
}

TEST_F(FIX4_4_CommissionDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setCommission(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_CommissionDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[152];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setCommission(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    CommissionData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getCommission(), component.getCommission());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_CommissionDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
