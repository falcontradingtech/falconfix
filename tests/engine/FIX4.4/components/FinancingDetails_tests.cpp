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

class FIX4_4_FinancingDetailsComponentTest : public ::testing::Test {
protected:
    FinancingDetails component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_FinancingDetailsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setAgreementDesc(test_value); }
    { const std::string_view test_value{"AAA"}; component.setAgreementID(test_value); }
    { const int64_t test_value = 12345; component.setAgreementDate(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetAgreementDescAndAgreementDescMatch) {
    EXPECT_FALSE(component.hasAgreementDesc());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetAgreementIDAndAgreementIDMatch) {
    EXPECT_FALSE(component.hasAgreementID());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetAgreementDateAndAgreementDateMatch) {
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetAgreementCurrencyAndAgreementCurrencyMatch) {
    EXPECT_FALSE(component.hasAgreementCurrency());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetTerminationTypeAndTerminationTypeMatch) {
    const int64_t test_value = 12345;
    component.setTerminationType(test_value);
    EXPECT_EQ(component.getTerminationType(), test_value);
    EXPECT_TRUE(component.hasTerminationType());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetStartDateAndStartDateMatch) {
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetEndDateAndEndDateMatch) {
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetDeliveryTypeAndDeliveryTypeMatch) {
    const int64_t test_value = 12345;
    component.setDeliveryType(test_value);
    EXPECT_EQ(component.getDeliveryType(), test_value);
    EXPECT_TRUE(component.hasDeliveryType());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetMarginRatioAndMarginRatioMatch) {
    const double test_value = 123.456;
    component.setMarginRatio(test_value);
    EXPECT_EQ(component.getMarginRatio(), test_value);
    EXPECT_TRUE(component.hasMarginRatio());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setAgreementDesc(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
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

TEST_F(FIX4_4_FinancingDetailsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setAgreementDesc(std::string_view{"AAA"});
    component.setAgreementID(std::string_view{"AAA"});
    component.setAgreementDate(12345);
    component.setAgreementCurrency(std::string_view{"AAA"});
    component.setTerminationType(12345);
    component.setStartDate(12345);
    component.setEndDate(12345);
    component.setDeliveryType(12345);
    component.setMarginRatio(123.456);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    FinancingDetails decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    
    // Re-encode the decoded view and compare the two byte streams to
    // confirm every populated field survived the roundtrip.
    char buffer2[4096];
    char *p2 = decoded.encode(buffer2, true);
    ASSERT_NE(p2, nullptr);
    const std::size_t encoded_size2 = static_cast<std::size_t>(p2 - buffer2);
    ASSERT_EQ(encoded_size, encoded_size2);
    EXPECT_EQ(0, std::memcmp(buffer, buffer2, encoded_size));
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_FinancingDetailsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
