// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrmtMDReqGrp::NoRelatedSym (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/InstrmtMDReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest : public ::testing::Test {
protected:
    InstrmtMDReqGrp::NoRelatedSym component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    { const std::string_view test_value{"AAA"}; component.getRefInstrument().setSymbol(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtMDReqGrp::NoRelatedSym decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefInstrument().setProduct(12345);
    component.getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefInstrument().setMaturityDate(12345);
    component.getRefInstrument().setPutOrCall(12345);
    component.getRefInstrument().setCouponPaymentDate(12345);
    component.getRefInstrument().setIssueDate(12345);
    component.getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefInstrument().setRepurchaseTerm(12345);
    component.getRefInstrument().setRepurchaseRate(123.456);
    component.getRefInstrument().setFactor(123.456);
    component.getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefInstrument().setRedemptionDate(12345);
    component.getRefInstrument().setStrikePrice(123.456);
    component.getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefInstrument().setOptAttribute('X');
    component.getRefInstrument().setContractMultiplier(123.456);
    component.getRefInstrument().setCouponRate(123.456);
    component.getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefInstrument().setCPProgram(12345);
    component.getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefInstrument().setDatedDate(12345);
    component.getRefInstrument().setInterestAccrualDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().emplace_back();
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().emplace_back();
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().emplace_back();
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipType(std::string_view{"AAA"});
    component.getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipValue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().emplace_back();
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().emplace_back();
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegProduct(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSide('X');
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtMDReqGrp::NoRelatedSym decoded;
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

TEST_F(FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_InstrmtMDReqGrp_NoRelatedSymComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
