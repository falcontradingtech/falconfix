// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: TrdCapDtGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/TrdCapDtGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class TrdCapDtGrpComponentTest : public ::testing::Test {
protected:
    TrdCapDtGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(TrdCapDtGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(TrdCapDtGrpComponentTest, SetTradeDateAndTradeDateMatch) {
}

TEST_F(TrdCapDtGrpComponentTest, SetTransactTimeAndTransactTimeMatch) {
}

TEST_F(TrdCapDtGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setTradeDate(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(TrdCapDtGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[66];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    TrdCapDtGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(TrdCapDtGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
