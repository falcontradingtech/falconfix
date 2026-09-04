// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MiscFeesGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MiscFeesGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class MiscFeesGrpComponentTest : public ::testing::Test {
protected:
    MiscFeesGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(MiscFeesGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setMiscFeeAmt(3.14f);
    component.setMiscFeeType('A');
    component.setMiscFeeBasis(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(MiscFeesGrpComponentTest, SetMiscFeeAmtAndMiscFeeAmtMatch) {
    const double test_value = 123.456;
    component.setMiscFeeAmt(test_value);
    EXPECT_EQ(component.getMiscFeeAmt(), test_value);
    EXPECT_TRUE(component.hasMiscFeeAmt());
}

TEST_F(MiscFeesGrpComponentTest, SetMiscFeeTypeAndMiscFeeTypeMatch) {
    const char test_value = 'X';
    component.setMiscFeeType(test_value);
    EXPECT_EQ(component.getMiscFeeType(), test_value);
    EXPECT_TRUE(component.hasMiscFeeType());
}

TEST_F(MiscFeesGrpComponentTest, SetMiscFeeBasisAndMiscFeeBasisMatch) {
    const int64_t test_value = 12345;
    component.setMiscFeeBasis(test_value);
    EXPECT_EQ(component.getMiscFeeBasis(), test_value);
    EXPECT_TRUE(component.hasMiscFeeBasis());
}

TEST_F(MiscFeesGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setMiscFeeAmt(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(MiscFeesGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[194];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    MiscFeesGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(MiscFeesGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
