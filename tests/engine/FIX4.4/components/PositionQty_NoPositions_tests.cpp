// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PositionQty::NoPositions (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PositionQty.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PositionQty_NoPositionsComponentTest : public ::testing::Test {
protected:
    PositionQty::NoPositions component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PositionQty_NoPositionsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLongQty(3.14f);
    component.setShortQty(3.14f);
    component.setPosQtyStatus(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PositionQty_NoPositionsComponentTest, SetLongQtyAndLongQtyMatch) {
    const double test_value = 123.456;
    component.setLongQty(test_value);
    EXPECT_EQ(component.getLongQty(), test_value);
    EXPECT_TRUE(component.hasLongQty());
}

TEST_F(PositionQty_NoPositionsComponentTest, SetShortQtyAndShortQtyMatch) {
    const double test_value = 123.456;
    component.setShortQty(test_value);
    EXPECT_EQ(component.getShortQty(), test_value);
    EXPECT_TRUE(component.hasShortQty());
}

TEST_F(PositionQty_NoPositionsComponentTest, SetPosQtyStatusAndPosQtyStatusMatch) {
    const int64_t test_value = 12345;
    component.setPosQtyStatus(test_value);
    EXPECT_EQ(component.getPosQtyStatus(), test_value);
    EXPECT_TRUE(component.hasPosQtyStatus());
}

TEST_F(PositionQty_NoPositionsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLongQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PositionQty_NoPositionsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[240];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setLongQty(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    PositionQty::NoPositions decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getLongQty(), component.getLongQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(PositionQty_NoPositionsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
