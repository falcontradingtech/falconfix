// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ExecAllocGrp::NoExecs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ExecAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class ExecAllocGrp_NoExecsComponentTest : public ::testing::Test {
protected:
    ExecAllocGrp::NoExecs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(ExecAllocGrp_NoExecsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setLastQty(3.14f);
    component.setLastPx(3.14f);
    component.setLastParPx(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, SetLastQtyAndLastQtyMatch) {
    const double test_value = 123.456;
    component.setLastQty(test_value);
    EXPECT_EQ(component.getLastQty(), test_value);
    EXPECT_TRUE(component.hasLastQty());
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, SetLastPxAndLastPxMatch) {
    const double test_value = 123.456;
    component.setLastPx(test_value);
    EXPECT_EQ(component.getLastPx(), test_value);
    EXPECT_TRUE(component.hasLastPx());
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, SetLastParPxAndLastParPxMatch) {
    const double test_value = 123.456;
    component.setLastParPx(test_value);
    EXPECT_EQ(component.getLastParPx(), test_value);
    EXPECT_TRUE(component.hasLastParPx());
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, SetLastCapacityAndLastCapacityMatch) {
    const char test_value = 'X';
    component.setLastCapacity(test_value);
    EXPECT_EQ(component.getLastCapacity(), test_value);
    EXPECT_TRUE(component.hasLastCapacity());
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setLastQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[326];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    ExecAllocGrp::NoExecs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(ExecAllocGrp_NoExecsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
