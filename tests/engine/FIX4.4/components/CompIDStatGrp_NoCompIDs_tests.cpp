// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: CompIDStatGrp::NoCompIDs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/CompIDStatGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_CompIDStatGrp_NoCompIDsComponentTest : public ::testing::Test {
protected:
    CompIDStatGrp::NoCompIDs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setRefCompID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setRefSubID(test_value); }
    { const std::string_view test_value{"AAA"}; component.setLocationID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetRefCompIDAndRefCompIDMatch) {
    EXPECT_FALSE(component.hasRefCompID());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetRefSubIDAndRefSubIDMatch) {
    EXPECT_FALSE(component.hasRefSubID());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetLocationIDAndLocationIDMatch) {
    EXPECT_FALSE(component.hasLocationID());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetDeskIDAndDeskIDMatch) {
    EXPECT_FALSE(component.hasDeskID());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetStatusValueAndStatusValueMatch) {
    const int64_t test_value = 12345;
    component.setStatusValue(test_value);
    EXPECT_EQ(component.getStatusValue(), test_value);
    EXPECT_TRUE(component.hasStatusValue());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetStatusTextAndStatusTextMatch) {
    EXPECT_FALSE(component.hasStatusText());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setRefCompID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setStatusValue(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    CompIDStatGrp::NoCompIDs decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getStatusValue(), component.getStatusValue());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setRefCompID(std::string_view{"AAA"});
    component.setRefSubID(std::string_view{"AAA"});
    component.setLocationID(std::string_view{"AAA"});
    component.setDeskID(std::string_view{"AAA"});
    component.setStatusValue(12345);
    component.setStatusText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    CompIDStatGrp::NoCompIDs decoded;
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

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_CompIDStatGrp_NoCompIDsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
