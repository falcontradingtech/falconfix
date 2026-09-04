// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrumentExtension (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/InstrumentExtension.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class InstrumentExtensionComponentTest : public ::testing::Test {
protected:
    InstrumentExtension component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(InstrumentExtensionComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setDeliveryForm(42);
    component.setPctAtRisk(3.14f);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(InstrumentExtensionComponentTest, SetDeliveryFormAndDeliveryFormMatch) {
    const int64_t test_value = 12345;
    component.setDeliveryForm(test_value);
    EXPECT_EQ(component.getDeliveryForm(), test_value);
    EXPECT_TRUE(component.hasDeliveryForm());
}

TEST_F(InstrumentExtensionComponentTest, SetPctAtRiskAndPctAtRiskMatch) {
    const double test_value = 123.456;
    component.setPctAtRisk(test_value);
    EXPECT_EQ(component.getPctAtRisk(), test_value);
    EXPECT_TRUE(component.hasPctAtRisk());
}

TEST_F(InstrumentExtensionComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setDeliveryForm(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(InstrumentExtensionComponentTest, EncodeDecodeRoundtrip) {
    char buffer[108];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    InstrumentExtension decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(InstrumentExtensionComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
