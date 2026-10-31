// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RgstDistInstGrp::NoDistribInsts (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RgstDistInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest : public ::testing::Test {
protected:
    RgstDistInstGrp::NoDistribInsts component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const int64_t test_value = 12345; component.setDistribPaymentMethod(test_value); }
    { const double test_value = 123.456; component.setDistribPercentage(test_value); }
    { const std::string_view test_value{"AAA"}; component.setCashDistribCurr(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetDistribPaymentMethodAndDistribPaymentMethodMatch) {
    const int64_t test_value = 12345;
    component.setDistribPaymentMethod(test_value);
    EXPECT_EQ(component.getDistribPaymentMethod(), test_value);
    EXPECT_TRUE(component.hasDistribPaymentMethod());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetDistribPercentageAndDistribPercentageMatch) {
    const double test_value = 123.456;
    component.setDistribPercentage(test_value);
    EXPECT_EQ(component.getDistribPercentage(), test_value);
    EXPECT_TRUE(component.hasDistribPercentage());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetCashDistribCurrAndCashDistribCurrMatch) {
    EXPECT_FALSE(component.hasCashDistribCurr());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetCashDistribAgentNameAndCashDistribAgentNameMatch) {
    EXPECT_FALSE(component.hasCashDistribAgentName());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetCashDistribAgentCodeAndCashDistribAgentCodeMatch) {
    EXPECT_FALSE(component.hasCashDistribAgentCode());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetCashDistribAgentAcctNumberAndCashDistribAgentAcctNumberMatch) {
    EXPECT_FALSE(component.hasCashDistribAgentAcctNumber());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetCashDistribPayRefAndCashDistribPayRefMatch) {
    EXPECT_FALSE(component.hasCashDistribPayRef());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetCashDistribAgentAcctNameAndCashDistribAgentAcctNameMatch) {
    EXPECT_FALSE(component.hasCashDistribAgentAcctName());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const int64_t test_value = 12345; component.setDistribPaymentMethod(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setDistribPaymentMethod(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    RgstDistInstGrp::NoDistribInsts decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getDistribPaymentMethod(), component.getDistribPaymentMethod());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setDistribPaymentMethod(12345);
    component.setDistribPercentage(123.456);
    component.setCashDistribCurr(std::string_view{"AAA"});
    component.setCashDistribAgentName(std::string_view{"AAA"});
    component.setCashDistribAgentCode(std::string_view{"AAA"});
    component.setCashDistribAgentAcctNumber(std::string_view{"AAA"});
    component.setCashDistribPayRef(std::string_view{"AAA"});
    component.setCashDistribAgentAcctName(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    RgstDistInstGrp::NoDistribInsts decoded;
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

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_RgstDistInstGrp_NoDistribInstsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
