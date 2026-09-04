// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlInstGrp::NoSettlInst (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SettlInstGrp_NoSettlInstComponentTest : public ::testing::Test {
protected:
    SettlInstGrp::NoSettlInst component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSettlInstTransType('A');
    component.setSide('A');
    component.setProduct(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSettlInstTransTypeAndSettlInstTransTypeMatch) {
    const char test_value = 'X';
    component.setSettlInstTransType(test_value);
    EXPECT_EQ(component.getSettlInstTransType(), test_value);
    EXPECT_TRUE(component.hasSettlInstTransType());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetProductAndProductMatch) {
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    EXPECT_EQ(component.getProduct(), test_value);
    EXPECT_TRUE(component.hasProduct());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetEffectiveTimeAndEffectiveTimeMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetLastUpdateTimeAndLastUpdateTimeMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetPaymentMethodAndPaymentMethodMatch) {
    const int64_t test_value = 12345;
    component.setPaymentMethod(test_value);
    EXPECT_EQ(component.getPaymentMethod(), test_value);
    EXPECT_TRUE(component.hasPaymentMethod());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardStartDateAndCardStartDateMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardExpDateAndCardExpDateMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetPaymentDateAndPaymentDateMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSettlInstTransType('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, EncodeDecodeRoundtrip) {
    char buffer[998];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setSettlInstTransType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SettlInstGrp::NoSettlInst decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSettlInstTransType(), component.getSettlInstTransType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
