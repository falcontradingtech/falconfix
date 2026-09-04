// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PositionQty (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PositionQty.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PositionQtyComponentTest : public ::testing::Test {
protected:
    PositionQty component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PositionQtyComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLongQty(3.14f);
    component.setShortQty(3.14f);
    component.setPosQtyStatus(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PositionQtyComponentTest, SetLongQtyAndLongQtyMatch) {
    const double test_value = 123.456;
    component.setLongQty(test_value);
    EXPECT_EQ(component.getLongQty(), test_value);
    EXPECT_TRUE(component.hasLongQty());
}

TEST_F(PositionQtyComponentTest, SetShortQtyAndShortQtyMatch) {
    const double test_value = 123.456;
    component.setShortQty(test_value);
    EXPECT_EQ(component.getShortQty(), test_value);
    EXPECT_TRUE(component.hasShortQty());
}

TEST_F(PositionQtyComponentTest, SetPosQtyStatusAndPosQtyStatusMatch) {
    const int64_t test_value = 12345;
    component.setPosQtyStatus(test_value);
    EXPECT_EQ(component.getPosQtyStatus(), test_value);
    EXPECT_TRUE(component.hasPosQtyStatus());
}

TEST_F(PositionQtyComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLongQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PositionQtyComponentTest, EncodeDecodeRoundtrip) {
    char buffer[240];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    PositionQty decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(PositionQtyComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
