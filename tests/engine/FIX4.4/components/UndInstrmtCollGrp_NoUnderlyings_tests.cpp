// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: UndInstrmtCollGrp::NoUnderlyings (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/UndInstrmtCollGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest : public ::testing::Test {
protected:
    UndInstrmtCollGrp::NoUnderlyings component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const int64_t test_value = 12345; component.setCollAction(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, SetCollActionAndCollActionMatch) {
    const int64_t test_value = 12345;
    component.setCollAction(test_value);
    EXPECT_EQ(component.getCollAction(), test_value);
    EXPECT_TRUE(component.hasCollAction());
}

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const int64_t test_value = 12345; component.setCollAction(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setCollAction(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    UndInstrmtCollGrp::NoUnderlyings decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getCollAction(), component.getCollAction());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().emplace_back();
    component.getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().emplace_back();
    component.getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipType(std::string_view{"AAA"});
    component.getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipValue(std::string_view{"AAA"});
    component.setCollAction(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    UndInstrmtCollGrp::NoUnderlyings decoded;
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

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_UndInstrmtCollGrp_NoUnderlyingsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
