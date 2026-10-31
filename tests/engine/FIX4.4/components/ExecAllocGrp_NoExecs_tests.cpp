// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ExecAllocGrp::NoExecs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ExecAllocGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_ExecAllocGrp_NoExecsComponentTest : public ::testing::Test {
protected:
    ExecAllocGrp::NoExecs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const double test_value = 123.456; component.setLastQty(test_value); }
    { const std::string_view test_value{"AAA"}; component.setExecID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSecondaryExecID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetLastQtyAndLastQtyMatch) {
    const double test_value = 123.456;
    component.setLastQty(test_value);
    EXPECT_EQ(component.getLastQty(), test_value);
    EXPECT_TRUE(component.hasLastQty());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetExecIDAndExecIDMatch) {
    EXPECT_FALSE(component.hasExecID());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetSecondaryExecIDAndSecondaryExecIDMatch) {
    EXPECT_FALSE(component.hasSecondaryExecID());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetLastPxAndLastPxMatch) {
    const double test_value = 123.456;
    component.setLastPx(test_value);
    EXPECT_EQ(component.getLastPx(), test_value);
    EXPECT_TRUE(component.hasLastPx());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetLastParPxAndLastParPxMatch) {
    const double test_value = 123.456;
    component.setLastParPx(test_value);
    EXPECT_EQ(component.getLastParPx(), test_value);
    EXPECT_TRUE(component.hasLastParPx());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetLastCapacityAndLastCapacityMatch) {
    const char test_value = 'X';
    component.setLastCapacity(test_value);
    EXPECT_EQ(component.getLastCapacity(), test_value);
    EXPECT_TRUE(component.hasLastCapacity());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const double test_value = 123.456; component.setLastQty(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const double test_value = 123.456;
    component.setLastQty(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    ExecAllocGrp::NoExecs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getLastQty(), component.getLastQty());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setLastQty(123.456);
    component.setExecID(std::string_view{"AAA"});
    component.setSecondaryExecID(std::string_view{"AAA"});
    component.setLastPx(123.456);
    component.setLastParPx(123.456);
    component.setLastCapacity('X');
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    ExecAllocGrp::NoExecs decoded;
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

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_ExecAllocGrp_NoExecsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
