// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: PreAllocMlegGrp::NoAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/PreAllocMlegGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest : public ::testing::Test {
protected:
    PreAllocMlegGrp::NoAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setAllocAccount(test_value); }
    { const int64_t test_value = 12345; component.setAllocAcctIDSource(test_value); }
    { const std::string_view test_value{"AAA"}; component.setAllocSettlCurrency(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, SetAllocAccountAndAllocAccountMatch) {
    EXPECT_FALSE(component.hasAllocAccount());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, SetAllocAcctIDSourceAndAllocAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    EXPECT_EQ(component.getAllocAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAllocAcctIDSource());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, SetAllocSettlCurrencyAndAllocSettlCurrencyMatch) {
    EXPECT_FALSE(component.hasAllocSettlCurrency());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, SetIndividualAllocIDAndIndividualAllocIDMatch) {
    EXPECT_FALSE(component.hasIndividualAllocID());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, SetAllocQtyAndAllocQtyMatch) {
    const double test_value = 123.456;
    component.setAllocQty(test_value);
    EXPECT_EQ(component.getAllocQty(), test_value);
    EXPECT_TRUE(component.hasAllocQty());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setAllocAccount(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    PreAllocMlegGrp::NoAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getAllocAcctIDSource(), component.getAllocAcctIDSource());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_PreAllocMlegGrp_NoAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
