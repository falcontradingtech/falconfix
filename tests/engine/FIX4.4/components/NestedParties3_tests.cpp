// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NestedParties3 (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NestedParties3.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_NestedParties3ComponentTest : public ::testing::Test {
protected:
    NestedParties3 component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_NestedParties3ComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_NestedParties3ComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_NestedParties3ComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoNested3PartyIDsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoNested3PartyIDsGroup().back().setNested3PartyID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    NestedParties3 decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_NestedParties3ComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoNested3PartyIDsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoNested3PartyIDsGroup().back().setNested3PartyID(test_value); }
    component.getRefNoNested3PartyIDsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoNested3PartyIDsGroup().back().setNested3PartyID(test_value); }
    
    EXPECT_EQ(component.getRefNoNested3PartyIDsGroup().size(), 2u);
    EXPECT_TRUE(component.hasAnySet());
    EXPECT_TRUE(component.checkRequired());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    EXPECT_GT(static_cast<std::size_t>(p - buffer), 0u);
    
    NestedParties3 decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoNested3PartyIDsGroup().size(), component.getRefNoNested3PartyIDsGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    NestedParties3 other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_NestedParties3ComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
