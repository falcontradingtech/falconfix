// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SecListGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SecListGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SecListGrpComponentTest : public ::testing::Test {
protected:
    SecListGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SecListGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setRoundLot(3.14f);
    component.setMinTradeVol(3.14f);
    component.setExpirationCycle(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SecListGrpComponentTest, SetRoundLotAndRoundLotMatch) {
    const double test_value = 123.456;
    component.setRoundLot(test_value);
    EXPECT_EQ(component.getRoundLot(), test_value);
    EXPECT_TRUE(component.hasRoundLot());
}

TEST_F(SecListGrpComponentTest, SetMinTradeVolAndMinTradeVolMatch) {
    const double test_value = 123.456;
    component.setMinTradeVol(test_value);
    EXPECT_EQ(component.getMinTradeVol(), test_value);
    EXPECT_TRUE(component.hasMinTradeVol());
}

TEST_F(SecListGrpComponentTest, SetExpirationCycleAndExpirationCycleMatch) {
    const int64_t test_value = 12345;
    component.setExpirationCycle(test_value);
    EXPECT_EQ(component.getExpirationCycle(), test_value);
    EXPECT_TRUE(component.hasExpirationCycle());
}

TEST_F(SecListGrpComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(SecListGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setRoundLot(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SecListGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[582];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    SecListGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(SecListGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
