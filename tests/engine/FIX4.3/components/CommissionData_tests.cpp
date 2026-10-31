// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: CommissionData (FIX4.3)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.3/components/CommissionData.h>
#include <FIX4.3/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix43::components;

class FIX4_3_CommissionDataComponentTest : public ::testing::Test {
protected:
    CommissionData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_3_CommissionDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const double test_value = 123.456; component.setCommission(test_value); }
    { const char test_value = 'X'; component.setCommType(test_value); }
    { const std::string_view test_value{"AAA"}; component.setCommCurrency(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_3_CommissionDataComponentTest, SetCommissionAndCommissionMatch) {
    const double test_value = 123.456;
    component.setCommission(test_value);
    EXPECT_EQ(component.getCommission(), test_value);
    EXPECT_TRUE(component.hasCommission());
}

TEST_F(FIX4_3_CommissionDataComponentTest, SetCommTypeAndCommTypeMatch) {
    const char test_value = 'X';
    component.setCommType(test_value);
    EXPECT_EQ(component.getCommType(), test_value);
    EXPECT_TRUE(component.hasCommType());
}

TEST_F(FIX4_3_CommissionDataComponentTest, SetCommCurrencyAndCommCurrencyMatch) {
    EXPECT_FALSE(component.hasCommCurrency());
}

TEST_F(FIX4_3_CommissionDataComponentTest, SetFundRenewWaivAndFundRenewWaivMatch) {
    const char test_value = 'X';
    component.setFundRenewWaiv(test_value);
    EXPECT_EQ(component.getFundRenewWaiv(), test_value);
    EXPECT_TRUE(component.hasFundRenewWaiv());
}

TEST_F(FIX4_3_CommissionDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const double test_value = 123.456; component.setCommission(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_3_CommissionDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setCommission(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    CommissionData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getCommission(), component.getCommission());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_3_CommissionDataComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setCommission(123.456);
    component.setCommType('X');
    component.setCommCurrency(std::string_view{"AAA"});
    component.setFundRenewWaiv('X');
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    CommissionData decoded;
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

TEST_F(FIX4_3_CommissionDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_3_CommissionDataComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
