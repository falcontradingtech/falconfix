// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PtysSubGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PtysSubGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class PtysSubGrpComponentTest : public ::testing::Test {
protected:
    PtysSubGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(PtysSubGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPartySubIDType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(PtysSubGrpComponentTest, SetPartySubIDTypeAndPartySubIDTypeMatch) {
    const int64_t test_value = 12345;
    component.setPartySubIDType(test_value);
    EXPECT_EQ(component.getPartySubIDType(), test_value);
    EXPECT_TRUE(component.hasPartySubIDType());
}

TEST_F(PtysSubGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPartySubIDType(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(PtysSubGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[124];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    PtysSubGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(PtysSubGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
