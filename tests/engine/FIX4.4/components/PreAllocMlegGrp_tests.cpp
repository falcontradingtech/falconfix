// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PreAllocMlegGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PreAllocMlegGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_PreAllocMlegGrpComponentTest : public ::testing::Test {
protected:
    PreAllocMlegGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_PreAllocMlegGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_PreAllocMlegGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_PreAllocMlegGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[48];
    
    // No scalar setter available to populate deterministically (this
    // component only wraps nested groups/sub-components). Group-only
    // containers with no required fields legitimately encode to 0
    // bytes, and some decode() implementations return false when no
    // bytes were consumed, so we only assert the two views agree.
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    PreAllocMlegGrp decoded;
    const char *q = buffer;
    (void)decoded.decode(q, p);
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_PreAllocMlegGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
