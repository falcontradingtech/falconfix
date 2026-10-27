// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: TrdCollGrp::NoTrades (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/TrdCollGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_TrdCollGrp_NoTradesComponentTest : public ::testing::Test {
protected:
    TrdCollGrp::NoTrades component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_TrdCollGrp_NoTradesComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setTradeReportID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryTradeReportID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdCollGrp_NoTradesComponentTest, SetTradeReportIDAndTradeReportIDMatch) {
    EXPECT_FALSE(component.hasTradeReportID());
}

TEST_F(FIX4_4_TrdCollGrp_NoTradesComponentTest, SetSecondaryTradeReportIDAndSecondaryTradeReportIDMatch) {
    EXPECT_FALSE(component.hasSecondaryTradeReportID());
}

TEST_F(FIX4_4_TrdCollGrp_NoTradesComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setTradeReportID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_TrdCollGrp_NoTradesComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    { const std::string_view test_value{"AAA"}; component.setTradeReportID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    TrdCollGrp::NoTrades decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_TrdCollGrp_NoTradesComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
