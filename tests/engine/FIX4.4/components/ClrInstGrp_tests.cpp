// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ClrInstGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ClrInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class ClrInstGrpComponentTest : public ::testing::Test {
protected:
    ClrInstGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(ClrInstGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setClearingInstruction(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(ClrInstGrpComponentTest, SetClearingInstructionAndClearingInstructionMatch) {
    const int64_t test_value = 12345;
    component.setClearingInstruction(test_value);
    EXPECT_EQ(component.getClearingInstruction(), test_value);
    EXPECT_TRUE(component.hasClearingInstruction());
}

TEST_F(ClrInstGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setClearingInstruction(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(ClrInstGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[50];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    ClrInstGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(ClrInstGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
