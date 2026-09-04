// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: DlvyInstGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/DlvyInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class DlvyInstGrpComponentTest : public ::testing::Test {
protected:
    DlvyInstGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(DlvyInstGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setSettlInstSource('A');
    component.setDlvyInstType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(DlvyInstGrpComponentTest, SetSettlInstSourceAndSettlInstSourceMatch) {
    const char test_value = 'X';
    component.setSettlInstSource(test_value);
    EXPECT_EQ(component.getSettlInstSource(), test_value);
    EXPECT_TRUE(component.hasSettlInstSource());
}

TEST_F(DlvyInstGrpComponentTest, SetDlvyInstTypeAndDlvyInstTypeMatch) {
    const char test_value = 'X';
    component.setDlvyInstType(test_value);
    EXPECT_EQ(component.getDlvyInstType(), test_value);
    EXPECT_TRUE(component.hasDlvyInstType());
}

TEST_F(DlvyInstGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setSettlInstSource('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(DlvyInstGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[24];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    DlvyInstGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(DlvyInstGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
