// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotEntryGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotEntryGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class QuotEntryGrpComponentTest : public ::testing::Test {
protected:
    QuotEntryGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(QuotEntryGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(QuotEntryGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(QuotEntryGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[50];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    QuotEntryGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(QuotEntryGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
