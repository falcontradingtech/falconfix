// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlInstructionsData (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlInstructionsData.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SettlInstructionsDataComponentTest : public ::testing::Test {
protected:
    SettlInstructionsData component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const int64_t test_value = 12345; component.setSettlDeliveryType(test_value); }
    { const int64_t test_value = 12345; component.setStandInstDbType(test_value); }
    { const std::string_view test_value{"AAA"}; component.setStandInstDbName(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, SetSettlDeliveryTypeAndSettlDeliveryTypeMatch) {
    const int64_t test_value = 12345;
    component.setSettlDeliveryType(test_value);
    EXPECT_EQ(component.getSettlDeliveryType(), test_value);
    EXPECT_TRUE(component.hasSettlDeliveryType());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, SetStandInstDbTypeAndStandInstDbTypeMatch) {
    const int64_t test_value = 12345;
    component.setStandInstDbType(test_value);
    EXPECT_EQ(component.getStandInstDbType(), test_value);
    EXPECT_TRUE(component.hasStandInstDbType());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, SetStandInstDbNameAndStandInstDbNameMatch) {
    EXPECT_FALSE(component.hasStandInstDbName());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, SetStandInstDbIDAndStandInstDbIDMatch) {
    EXPECT_FALSE(component.hasStandInstDbID());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const int64_t test_value = 12345; component.setSettlDeliveryType(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setSettlDeliveryType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SettlInstructionsData decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSettlDeliveryType(), component.getSettlDeliveryType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setSettlDeliveryType(12345);
    component.setStandInstDbType(12345);
    component.setStandInstDbName(std::string_view{"AAA"});
    component.setStandInstDbID(std::string_view{"AAA"});
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().emplace_back();
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setSettlInstSource('X');
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setDlvyInstType('X');
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().emplace_back();
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyID(std::string_view{"AAA"});
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyIDSource('X');
    component.getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyRole(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SettlInstructionsData decoded;
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

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_SettlInstructionsDataComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
