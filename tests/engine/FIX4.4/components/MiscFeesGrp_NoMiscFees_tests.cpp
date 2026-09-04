// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MiscFeesGrp::NoMiscFees (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MiscFeesGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest : public ::testing::Test {
protected:
    MiscFeesGrp::NoMiscFees component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setMiscFeeAmt(3.14f);
    component.setMiscFeeType('A');
    component.setMiscFeeBasis(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, SetMiscFeeAmtAndMiscFeeAmtMatch) {
    const double test_value = 123.456;
    component.setMiscFeeAmt(test_value);
    EXPECT_EQ(component.getMiscFeeAmt(), test_value);
    EXPECT_TRUE(component.hasMiscFeeAmt());
}

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, SetMiscFeeTypeAndMiscFeeTypeMatch) {
    const char test_value = 'X';
    component.setMiscFeeType(test_value);
    EXPECT_EQ(component.getMiscFeeType(), test_value);
    EXPECT_TRUE(component.hasMiscFeeType());
}

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, SetMiscFeeBasisAndMiscFeeBasisMatch) {
    const int64_t test_value = 12345;
    component.setMiscFeeBasis(test_value);
    EXPECT_EQ(component.getMiscFeeBasis(), test_value);
    EXPECT_TRUE(component.hasMiscFeeBasis());
}

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setMiscFeeAmt(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[194];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setMiscFeeAmt(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    MiscFeesGrp::NoMiscFees decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getMiscFeeAmt(), component.getMiscFeeAmt());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_MiscFeesGrp_NoMiscFeesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
