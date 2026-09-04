// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NstdPtysSubGrp::NoNestedPartySubIDs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NstdPtysSubGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest : public ::testing::Test {
protected:
    NstdPtysSubGrp::NoNestedPartySubIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setNestedPartySubIDType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, SetNestedPartySubIDTypeAndNestedPartySubIDTypeMatch) {
    const int64_t test_value = 12345;
    component.setNestedPartySubIDType(test_value);
    EXPECT_EQ(component.getNestedPartySubIDType(), test_value);
    EXPECT_TRUE(component.hasNestedPartySubIDType());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setNestedPartySubIDType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[124];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setNestedPartySubIDType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    NstdPtysSubGrp::NoNestedPartySubIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getNestedPartySubIDType(), component.getNestedPartySubIDType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
