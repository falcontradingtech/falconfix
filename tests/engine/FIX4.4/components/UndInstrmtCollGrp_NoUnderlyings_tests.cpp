// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: UndInstrmtCollGrp::NoUnderlyings (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/UndInstrmtCollGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class UndInstrmtCollGrp_NoUnderlyingsComponentTest : public ::testing::Test {
protected:
    UndInstrmtCollGrp::NoUnderlyings component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(UndInstrmtCollGrp_NoUnderlyingsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setCollAction(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(UndInstrmtCollGrp_NoUnderlyingsComponentTest, SetCollActionAndCollActionMatch) {
    const int64_t test_value = 12345;
    component.setCollAction(test_value);
    EXPECT_EQ(component.getCollAction(), test_value);
    EXPECT_TRUE(component.hasCollAction());
}

TEST_F(UndInstrmtCollGrp_NoUnderlyingsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setCollAction(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(UndInstrmtCollGrp_NoUnderlyingsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[50];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    UndInstrmtCollGrp::NoUnderlyings decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(UndInstrmtCollGrp_NoUnderlyingsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
