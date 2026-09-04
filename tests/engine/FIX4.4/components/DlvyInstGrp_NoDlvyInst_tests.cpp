// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: DlvyInstGrp::NoDlvyInst (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/DlvyInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest : public ::testing::Test {
protected:
    DlvyInstGrp::NoDlvyInst component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSettlInstSource('A');
    component.setDlvyInstType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest, SetSettlInstSourceAndSettlInstSourceMatch) {
    const char test_value = 'X';
    component.setSettlInstSource(test_value);
    EXPECT_EQ(component.getSettlInstSource(), test_value);
    EXPECT_TRUE(component.hasSettlInstSource());
}

TEST_F(FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest, SetDlvyInstTypeAndDlvyInstTypeMatch) {
    const char test_value = 'X';
    component.setDlvyInstType(test_value);
    EXPECT_EQ(component.getDlvyInstType(), test_value);
    EXPECT_TRUE(component.hasDlvyInstType());
}

TEST_F(FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSettlInstSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest, EncodeDecodeRoundtrip) {
    char buffer[24];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setSettlInstSource(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    DlvyInstGrp::NoDlvyInst decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSettlInstSource(), component.getSettlInstSource());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_DlvyInstGrp_NoDlvyInstComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
