// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlInstGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SettlInstGrpComponentTest : public ::testing::Test {
protected:
    SettlInstGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SettlInstGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSettlInstTransType('A');
    component.setSide('A');
    component.setProduct(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SettlInstGrpComponentTest, SetSettlInstTransTypeAndSettlInstTransTypeMatch) {
    const char test_value = 'X';
    component.setSettlInstTransType(test_value);
    EXPECT_EQ(component.getSettlInstTransType(), test_value);
    EXPECT_TRUE(component.hasSettlInstTransType());
}

TEST_F(SettlInstGrpComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(SettlInstGrpComponentTest, SetProductAndProductMatch) {
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    EXPECT_EQ(component.getProduct(), test_value);
    EXPECT_TRUE(component.hasProduct());
}

TEST_F(SettlInstGrpComponentTest, SetEffectiveTimeAndEffectiveTimeMatch) {
}

TEST_F(SettlInstGrpComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(SettlInstGrpComponentTest, SetLastUpdateTimeAndLastUpdateTimeMatch) {
}

TEST_F(SettlInstGrpComponentTest, SetPaymentMethodAndPaymentMethodMatch) {
    const int64_t test_value = 12345;
    component.setPaymentMethod(test_value);
    EXPECT_EQ(component.getPaymentMethod(), test_value);
    EXPECT_TRUE(component.hasPaymentMethod());
}

TEST_F(SettlInstGrpComponentTest, SetCardStartDateAndCardStartDateMatch) {
}

TEST_F(SettlInstGrpComponentTest, SetCardExpDateAndCardExpDateMatch) {
}

TEST_F(SettlInstGrpComponentTest, SetPaymentDateAndPaymentDateMatch) {
}

TEST_F(SettlInstGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSettlInstTransType('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SettlInstGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[998];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    SettlInstGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(SettlInstGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
