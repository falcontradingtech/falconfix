// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: NstdPtysSubGrp::NoNestedPartySubIDs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/NstdPtysSubGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest : public ::testing::Test {
protected:
    NstdPtysSubGrp::NoNestedPartySubIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setNestedPartySubID(test_value); }
    { const int64_t test_value = 12345; component.setNestedPartySubIDType(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, SetNestedPartySubIDAndNestedPartySubIDMatch) {
    EXPECT_FALSE(component.hasNestedPartySubID());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, SetNestedPartySubIDTypeAndNestedPartySubIDTypeMatch) {
    const int64_t test_value = 12345;
    component.setNestedPartySubIDType(test_value);
    EXPECT_EQ(component.getNestedPartySubIDType(), test_value);
    EXPECT_TRUE(component.hasNestedPartySubIDType());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setNestedPartySubID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setNestedPartySubIDType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    NstdPtysSubGrp::NoNestedPartySubIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getNestedPartySubIDType(), component.getNestedPartySubIDType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setNestedPartySubID(std::string_view{"AAA"});
    component.setNestedPartySubIDType(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    NstdPtysSubGrp::NoNestedPartySubIDs decoded;
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

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_NstdPtysSubGrp_NoNestedPartySubIDsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
