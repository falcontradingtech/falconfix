// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SecListGrp::NoRelatedSym (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SecListGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class SecListGrp_NoRelatedSymComponentTest : public ::testing::Test {
protected:
    SecListGrp::NoRelatedSym component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(SecListGrp_NoRelatedSymComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setRoundLot(3.14f);
    component.setMinTradeVol(3.14f);
    component.setExpirationCycle(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, SetRoundLotAndRoundLotMatch) {
    const double test_value = 123.456;
    component.setRoundLot(test_value);
    EXPECT_EQ(component.getRoundLot(), test_value);
    EXPECT_TRUE(component.hasRoundLot());
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, SetMinTradeVolAndMinTradeVolMatch) {
    const double test_value = 123.456;
    component.setMinTradeVol(test_value);
    EXPECT_EQ(component.getMinTradeVol(), test_value);
    EXPECT_TRUE(component.hasMinTradeVol());
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, SetExpirationCycleAndExpirationCycleMatch) {
    const int64_t test_value = 12345;
    component.setExpirationCycle(test_value);
    EXPECT_EQ(component.getExpirationCycle(), test_value);
    EXPECT_TRUE(component.hasExpirationCycle());
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setRoundLot(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, EncodeDecodeRoundtrip) {
    char buffer[582];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setRoundLot(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SecListGrp::NoRelatedSym decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getRoundLot(), component.getRoundLot());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(SecListGrp_NoRelatedSymComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
