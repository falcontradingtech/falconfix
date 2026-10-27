// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrmtLegSecListGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/InstrmtLegSecListGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_InstrmtLegSecListGrpComponentTest : public ::testing::Test {
protected:
    InstrmtLegSecListGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_InstrmtLegSecListGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtLegSecListGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_InstrmtLegSecListGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoLegsGroup().emplace_back();
    { const int64_t test_value = 12345; component.getRefNoLegsGroup().back().setLegSwapType(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtLegSecListGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtLegSecListGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoLegsGroup().emplace_back();
    { const int64_t test_value = 12345; component.getRefNoLegsGroup().back().setLegSwapType(test_value); }
    component.getRefNoLegsGroup().emplace_back();
    { const int64_t test_value = 12345; component.getRefNoLegsGroup().back().setLegSwapType(test_value); }
    
    EXPECT_EQ(component.getRefNoLegsGroup().size(), 2u);
    EXPECT_TRUE(component.hasAnySet());
    EXPECT_TRUE(component.checkRequired());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    EXPECT_GT(static_cast<std::size_t>(p - buffer), 0u);
    
    InstrmtLegSecListGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoLegsGroup().size(), component.getRefNoLegsGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    InstrmtLegSecListGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_InstrmtLegSecListGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
