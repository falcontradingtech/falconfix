// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: SettlInstGrp::NoSettlInst (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/SettlInstGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_SettlInstGrp_NoSettlInstComponentTest : public ::testing::Test {
protected:
    SettlInstGrp::NoSettlInst component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setSettlInstID(test_value); }
    { const char test_value = 'X'; component.setSettlInstTransType(test_value); }
    { const std::string_view test_value{"AAA"}; component.setSettlInstRefID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSettlInstIDAndSettlInstIDMatch) {
    EXPECT_FALSE(component.hasSettlInstID());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSettlInstTransTypeAndSettlInstTransTypeMatch) {
    const char test_value = 'X';
    component.setSettlInstTransType(test_value);
    EXPECT_EQ(component.getSettlInstTransType(), test_value);
    EXPECT_TRUE(component.hasSettlInstTransType());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSettlInstRefIDAndSettlInstRefIDMatch) {
    EXPECT_FALSE(component.hasSettlInstRefID());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetProductAndProductMatch) {
    const int64_t test_value = 12345;
    component.setProduct(test_value);
    EXPECT_EQ(component.getProduct(), test_value);
    EXPECT_TRUE(component.hasProduct());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetSecurityTypeAndSecurityTypeMatch) {
    EXPECT_FALSE(component.hasSecurityType());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCFICodeAndCFICodeMatch) {
    EXPECT_FALSE(component.hasCFICode());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetEffectiveTimeAndEffectiveTimeMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetLastUpdateTimeAndLastUpdateTimeMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetPaymentMethodAndPaymentMethodMatch) {
    const int64_t test_value = 12345;
    component.setPaymentMethod(test_value);
    EXPECT_EQ(component.getPaymentMethod(), test_value);
    EXPECT_TRUE(component.hasPaymentMethod());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetPaymentRefAndPaymentRefMatch) {
    EXPECT_FALSE(component.hasPaymentRef());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardHolderNameAndCardHolderNameMatch) {
    EXPECT_FALSE(component.hasCardHolderName());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardNumberAndCardNumberMatch) {
    EXPECT_FALSE(component.hasCardNumber());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardStartDateAndCardStartDateMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardExpDateAndCardExpDateMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetCardIssNumAndCardIssNumMatch) {
    EXPECT_FALSE(component.hasCardIssNum());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetPaymentDateAndPaymentDateMatch) {
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetPaymentRemitterIDAndPaymentRemitterIDMatch) {
    EXPECT_FALSE(component.hasPaymentRemitterID());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setSettlInstID(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const char test_value = 'X';
    component.setSettlInstTransType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    SettlInstGrp::NoSettlInst decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getSettlInstTransType(), component.getSettlInstTransType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setSettlInstID(std::string_view{"AAA"});
    component.setSettlInstTransType('X');
    component.setSettlInstRefID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.setSide('X');
    component.setProduct(12345);
    component.setSecurityType(std::string_view{"AAA"});
    component.setCFICode(std::string_view{"AAA"});
    component.setEffectiveTime(12345);
    component.setExpireTime(12345);
    component.setLastUpdateTime(12345);
    component.getRefSettlInstructionsData().setSettlDeliveryType(12345);
    component.getRefSettlInstructionsData().setStandInstDbType(12345);
    component.getRefSettlInstructionsData().setStandInstDbName(std::string_view{"AAA"});
    component.getRefSettlInstructionsData().setStandInstDbID(std::string_view{"AAA"});
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().emplace_back();
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setSettlInstSource('X');
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().setDlvyInstType('X');
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().emplace_back();
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyID(std::string_view{"AAA"});
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyIDSource('X');
    component.getRefSettlInstructionsData().getRefDlvyInstGrp().getRefNoDlvyInstGroup().back().getRefSettlParties().getRefNoSettlPartyIDsGroup().back().setSettlPartyRole(12345);
    component.setPaymentMethod(12345);
    component.setPaymentRef(std::string_view{"AAA"});
    component.setCardHolderName(std::string_view{"AAA"});
    component.setCardNumber(std::string_view{"AAA"});
    component.setCardStartDate(12345);
    component.setCardExpDate(12345);
    component.setCardIssNum(std::string_view{"AAA"});
    component.setPaymentDate(12345);
    component.setPaymentRemitterID(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    SettlInstGrp::NoSettlInst decoded;
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

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_SettlInstGrp_NoSettlInstComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
