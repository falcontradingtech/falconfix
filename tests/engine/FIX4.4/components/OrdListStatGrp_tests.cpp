// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: OrdListStatGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/OrdListStatGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class OrdListStatGrpComponentTest : public ::testing::Test {
protected:
    OrdListStatGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(OrdListStatGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setCumQty(3.14f);
    component.setOrdStatus('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(OrdListStatGrpComponentTest, SetCumQtyAndCumQtyMatch) {
    const double test_value = 123.456;
    component.setCumQty(test_value);
    EXPECT_EQ(component.getCumQty(), test_value);
    EXPECT_TRUE(component.hasCumQty());
}

TEST_F(OrdListStatGrpComponentTest, SetOrdStatusAndOrdStatusMatch) {
    const char test_value = 'X';
    component.setOrdStatus(test_value);
    EXPECT_EQ(component.getOrdStatus(), test_value);
    EXPECT_TRUE(component.hasOrdStatus());
}

TEST_F(OrdListStatGrpComponentTest, SetWorkingIndicatorAndWorkingIndicatorMatch) {
}

TEST_F(OrdListStatGrpComponentTest, SetLeavesQtyAndLeavesQtyMatch) {
    const double test_value = 123.456;
    component.setLeavesQty(test_value);
    EXPECT_EQ(component.getLeavesQty(), test_value);
    EXPECT_TRUE(component.hasLeavesQty());
}

TEST_F(OrdListStatGrpComponentTest, SetCxlQtyAndCxlQtyMatch) {
    const double test_value = 123.456;
    component.setCxlQty(test_value);
    EXPECT_EQ(component.getCxlQty(), test_value);
    EXPECT_TRUE(component.hasCxlQty());
}

TEST_F(OrdListStatGrpComponentTest, SetAvgPxAndAvgPxMatch) {
    const double test_value = 123.456;
    component.setAvgPx(test_value);
    EXPECT_EQ(component.getAvgPx(), test_value);
    EXPECT_TRUE(component.hasAvgPx());
}

TEST_F(OrdListStatGrpComponentTest, SetOrdRejReasonAndOrdRejReasonMatch) {
    const int64_t test_value = 12345;
    component.setOrdRejReason(test_value);
    EXPECT_EQ(component.getOrdRejReason(), test_value);
    EXPECT_TRUE(component.hasOrdRejReason());
}

TEST_F(OrdListStatGrpComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(OrdListStatGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setCumQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(OrdListStatGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[638];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    OrdListStatGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(OrdListStatGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
