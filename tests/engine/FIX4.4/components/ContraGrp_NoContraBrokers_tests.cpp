// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ContraGrp::NoContraBrokers (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ContraGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_ContraGrp_NoContraBrokersComponentTest : public ::testing::Test {
protected:
    ContraGrp::NoContraBrokers component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_ContraGrp_NoContraBrokersComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setContraTradeQty(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_ContraGrp_NoContraBrokersComponentTest, SetContraTradeQtyAndContraTradeQtyMatch) {
    const double test_value = 123.456;
    component.setContraTradeQty(test_value);
    EXPECT_EQ(component.getContraTradeQty(), test_value);
    EXPECT_TRUE(component.hasContraTradeQty());
}

TEST_F(FIX4_4_ContraGrp_NoContraBrokersComponentTest, SetContraTradeTimeAndContraTradeTimeMatch) {
}

TEST_F(FIX4_4_ContraGrp_NoContraBrokersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setContraTradeQty(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_ContraGrp_NoContraBrokersComponentTest, EncodeDecodeRoundtrip) {
    char buffer[324];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setContraTradeQty(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    ContraGrp::NoContraBrokers decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getContraTradeQty(), component.getContraTradeQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_ContraGrp_NoContraBrokersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
