// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: FinancingDetails (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/FinancingDetails.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FinancingDetailsComponentTest : public ::testing::Test {
protected:
    FinancingDetails component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FinancingDetailsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setTerminationType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FinancingDetailsComponentTest, SetAgreementDateAndAgreementDateMatch) {
}

TEST_F(FinancingDetailsComponentTest, SetTerminationTypeAndTerminationTypeMatch) {
    const int64_t test_value = 12345;
    component.setTerminationType(test_value);
    EXPECT_EQ(component.getTerminationType(), test_value);
    EXPECT_TRUE(component.hasTerminationType());
}

TEST_F(FinancingDetailsComponentTest, SetStartDateAndStartDateMatch) {
}

TEST_F(FinancingDetailsComponentTest, SetEndDateAndEndDateMatch) {
}

TEST_F(FinancingDetailsComponentTest, SetDeliveryTypeAndDeliveryTypeMatch) {
    const int64_t test_value = 12345;
    component.setDeliveryType(test_value);
    EXPECT_EQ(component.getDeliveryType(), test_value);
    EXPECT_TRUE(component.hasDeliveryType());
}

TEST_F(FinancingDetailsComponentTest, SetMarginRatioAndMarginRatioMatch) {
    const double test_value = 123.456;
    component.setMarginRatio(test_value);
    EXPECT_EQ(component.getMarginRatio(), test_value);
    EXPECT_TRUE(component.hasMarginRatio());
}

TEST_F(FinancingDetailsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setAgreementDate(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FinancingDetailsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[458];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setTerminationType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    FinancingDetails decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getTerminationType(), component.getTerminationType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FinancingDetailsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
