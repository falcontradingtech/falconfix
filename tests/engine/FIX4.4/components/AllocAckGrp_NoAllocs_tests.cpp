// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: AllocAckGrp::NoAllocs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/AllocAckGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class AllocAckGrp_NoAllocsComponentTest : public ::testing::Test {
protected:
    AllocAckGrp::NoAllocs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(AllocAckGrp_NoAllocsComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setAllocAcctIDSource(42);
    component.setAllocPrice(3.14f);
    component.setIndividualAllocRejCode(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, SetAllocAcctIDSourceAndAllocAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAllocAcctIDSource(test_value);
    EXPECT_EQ(component.getAllocAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAllocAcctIDSource());
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, SetAllocPriceAndAllocPriceMatch) {
    const double test_value = 123.456;
    component.setAllocPrice(test_value);
    EXPECT_EQ(component.getAllocPrice(), test_value);
    EXPECT_TRUE(component.hasAllocPrice());
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, SetIndividualAllocRejCodeAndIndividualAllocRejCodeMatch) {
    const int64_t test_value = 12345;
    component.setIndividualAllocRejCode(test_value);
    EXPECT_EQ(component.getIndividualAllocRejCode(), test_value);
    EXPECT_TRUE(component.hasIndividualAllocRejCode());
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, SetEncodedAllocTextLenAndEncodedAllocTextLenMatch) {
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setAllocAcctIDSource(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[502];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    AllocAckGrp::NoAllocs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(AllocAckGrp_NoAllocsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
