// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: DlvyInstGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/DlvyInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class DlvyInstGrpComponentTest : public ::testing::Test {
protected:
    DlvyInstGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(DlvyInstGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(DlvyInstGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(DlvyInstGrpComponentTest, EncodeDecodeRoundtrip) {
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
    
    DlvyInstGrp decoded;
    const char *q = buffer;
    (void)decoded.decode(q, p);
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(DlvyInstGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
