// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RgstDtlsGrp::NoRegistDtls (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RgstDtlsGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest : public ::testing::Test {
protected:
    RgstDtlsGrp::NoRegistDtls component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setRegistDtls(test_value); }
    { const std::string_view test_value{"AAA"}; component.setRegistEmail(test_value); }
    { const std::string_view test_value{"AAA"}; component.setMailingDtls(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetRegistDtlsAndRegistDtlsMatch) {
    EXPECT_FALSE(component.hasRegistDtls());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetRegistEmailAndRegistEmailMatch) {
    EXPECT_FALSE(component.hasRegistEmail());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetMailingDtlsAndMailingDtlsMatch) {
    EXPECT_FALSE(component.hasMailingDtls());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetMailingInstAndMailingInstMatch) {
    EXPECT_FALSE(component.hasMailingInst());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetOwnerTypeAndOwnerTypeMatch) {
    const int64_t test_value = 12345;
    component.setOwnerType(test_value);
    EXPECT_EQ(component.getOwnerType(), test_value);
    EXPECT_TRUE(component.hasOwnerType());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetDateOfBirthAndDateOfBirthMatch) {
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetInvestorCountryOfResidenceAndInvestorCountryOfResidenceMatch) {
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setRegistDtls(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setOwnerType(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    RgstDtlsGrp::NoRegistDtls decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getOwnerType(), component.getOwnerType());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.setRegistDtls(std::string_view{"AAA"});
    component.setRegistEmail(std::string_view{"AAA"});
    component.setMailingDtls(std::string_view{"AAA"});
    component.setMailingInst(std::string_view{"AAA"});
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().emplace_back();
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().back().setNestedPartySubID(std::string_view{"AAA"});
    component.getRefNestedParties().getRefNoNestedPartyIDsGroup().back().getRefNstdPtysSubGrp().getRefNoNestedPartySubIDsGroup().back().setNestedPartySubIDType(12345);
    component.setOwnerType(12345);
    component.setDateOfBirth(12345);
    component.setInvestorCountryOfResidence(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    RgstDtlsGrp::NoRegistDtls decoded;
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

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_RgstDtlsGrp_NoRegistDtlsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
