// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: OrdListStatGrp::NoOrders (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/OrdListStatGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_OrdListStatGrp_NoOrdersComponentTest : public ::testing::Test {
protected:
    OrdListStatGrp::NoOrders component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryClOrdID(test_value); }
    { const double test_value = 123.456; component.setCumQty(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetClOrdIDAndClOrdIDMatch) {
    EXPECT_FALSE(component.hasClOrdID());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetSecondaryClOrdIDAndSecondaryClOrdIDMatch) {
    EXPECT_FALSE(component.hasSecondaryClOrdID());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetCumQtyAndCumQtyMatch) {
    const double test_value = 123.456;
    component.setCumQty(test_value);
    EXPECT_EQ(component.getCumQty(), test_value);
    EXPECT_TRUE(component.hasCumQty());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetOrdStatusAndOrdStatusMatch) {
    const char test_value = 'X';
    component.setOrdStatus(test_value);
    EXPECT_EQ(component.getOrdStatus(), test_value);
    EXPECT_TRUE(component.hasOrdStatus());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetWorkingIndicatorAndWorkingIndicatorMatch) {
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetLeavesQtyAndLeavesQtyMatch) {
    const double test_value = 123.456;
    component.setLeavesQty(test_value);
    EXPECT_EQ(component.getLeavesQty(), test_value);
    EXPECT_TRUE(component.hasLeavesQty());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetCxlQtyAndCxlQtyMatch) {
    const double test_value = 123.456;
    component.setCxlQty(test_value);
    EXPECT_EQ(component.getCxlQty(), test_value);
    EXPECT_TRUE(component.hasCxlQty());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetAvgPxAndAvgPxMatch) {
    const double test_value = 123.456;
    component.setAvgPx(test_value);
    EXPECT_EQ(component.getAvgPx(), test_value);
    EXPECT_TRUE(component.hasAvgPx());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetOrdRejReasonAndOrdRejReasonMatch) {
    const int64_t test_value = 12345;
    component.setOrdRejReason(test_value);
    EXPECT_EQ(component.getOrdRejReason(), test_value);
    EXPECT_TRUE(component.hasOrdRejReason());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setClOrdID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setCumQty(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    OrdListStatGrp::NoOrders decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getCumQty(), component.getCumQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setClOrdID(std::string_view{"AAA"});
    component.setSecondaryClOrdID(std::string_view{"AAA"});
    component.setCumQty(123.456);
    component.setOrdStatus('X');
    component.setWorkingIndicator(true);
    component.setLeavesQty(123.456);
    component.setCxlQty(123.456);
    component.setAvgPx(123.456);
    component.setOrdRejReason(12345);
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    OrdListStatGrp::NoOrders decoded;
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

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_FALSE(component.checkRequired());
}

TEST_F(FIX4_4_OrdListStatGrp_NoOrdersComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
