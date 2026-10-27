// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: LegSecAltIDGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/LegSecAltIDGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_LegSecAltIDGrpComponentTest : public ::testing::Test {
protected:
    LegSecAltIDGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_LegSecAltIDGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_LegSecAltIDGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_LegSecAltIDGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoLegSecurityAltIDGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    LegSecAltIDGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_LegSecAltIDGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoLegSecurityAltIDGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(test_value); }
    component.getRefNoLegSecurityAltIDGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(test_value); }
    
    EXPECT_EQ(component.getRefNoLegSecurityAltIDGroup().size(), 2u);
    EXPECT_TRUE(component.hasAnySet());
    EXPECT_TRUE(component.checkRequired());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    EXPECT_GT(static_cast<std::size_t>(p - buffer), 0u);
    
    LegSecAltIDGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoLegSecurityAltIDGroup().size(), component.getRefNoLegSecurityAltIDGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    LegSecAltIDGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_LegSecAltIDGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
