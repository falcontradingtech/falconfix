// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDReqGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class MDReqGrpComponentTest : public ::testing::Test {
protected:
    MDReqGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(MDReqGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setMDEntryType('A');
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(MDReqGrpComponentTest, SetMDEntryTypeAndMDEntryTypeMatch) {
    const char test_value = 'X';
    component.setMDEntryType(test_value);
    EXPECT_EQ(component.getMDEntryType(), test_value);
    EXPECT_TRUE(component.hasMDEntryType());
}

TEST_F(MDReqGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setMDEntryType('A');
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(MDReqGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[12];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    MDReqGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(MDReqGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
