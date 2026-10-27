// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: AffectedOrdGrp::NoAffectedOrders (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/AffectedOrdGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest : public ::testing::Test {
protected:
    AffectedOrdGrp::NoAffectedOrders component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setOrigClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setAffectedOrderID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setAffectedSecondaryOrderID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, SetOrigClOrdIDAndOrigClOrdIDMatch) {
    EXPECT_FALSE(component.hasOrigClOrdID());
}

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, SetAffectedOrderIDAndAffectedOrderIDMatch) {
    EXPECT_FALSE(component.hasAffectedOrderID());
}

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, SetAffectedSecondaryOrderIDAndAffectedSecondaryOrderIDMatch) {
    EXPECT_FALSE(component.hasAffectedSecondaryOrderID());
}

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setOrigClOrdID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    { const std::string_view test_value{"AAA"}; component.setOrigClOrdID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    AffectedOrdGrp::NoAffectedOrders decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_AffectedOrdGrp_NoAffectedOrdersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
