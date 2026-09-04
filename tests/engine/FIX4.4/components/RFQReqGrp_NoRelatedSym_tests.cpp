// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RFQReqGrp::NoRelatedSym (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RFQReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class RFQReqGrp_NoRelatedSymComponentTest : public ::testing::Test {
protected:
    RFQReqGrp::NoRelatedSym component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPrevClosePx(3.14f);
    component.setQuoteRequestType(42);
    component.setQuoteType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, SetPrevClosePxAndPrevClosePxMatch) {
    const double test_value = 123.456;
    component.setPrevClosePx(test_value);
    EXPECT_EQ(component.getPrevClosePx(), test_value);
    EXPECT_TRUE(component.hasPrevClosePx());
}

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, SetQuoteRequestTypeAndQuoteRequestTypeMatch) {
    const int64_t test_value = 12345;
    component.setQuoteRequestType(test_value);
    EXPECT_EQ(component.getQuoteRequestType(), test_value);
    EXPECT_TRUE(component.hasQuoteRequestType());
}

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, SetQuoteTypeAndQuoteTypeMatch) {
    const int64_t test_value = 12345;
    component.setQuoteType(test_value);
    EXPECT_EQ(component.getQuoteType(), test_value);
    EXPECT_TRUE(component.hasQuoteType());
}

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPrevClosePx(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, EncodeDecodeRoundtrip) {
    char buffer[306];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    RFQReqGrp::NoRelatedSym decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(RFQReqGrp_NoRelatedSymComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
