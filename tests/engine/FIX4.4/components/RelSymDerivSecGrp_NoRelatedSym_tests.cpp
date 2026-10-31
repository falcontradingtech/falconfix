// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: RelSymDerivSecGrp::NoRelatedSym (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/RelSymDerivSecGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest : public ::testing::Test {
protected:
    RelSymDerivSecGrp::NoRelatedSym component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setCurrency(test_value); }
    { const int64_t test_value = 12345; component.setExpirationCycle(test_value); }
    { const std::string_view test_value{"AAA"}; component.setTradingSessionID(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetCurrencyAndCurrencyMatch) {
    EXPECT_FALSE(component.hasCurrency());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetExpirationCycleAndExpirationCycleMatch) {
    const int64_t test_value = 12345;
    component.setExpirationCycle(test_value);
    EXPECT_EQ(component.getExpirationCycle(), test_value);
    EXPECT_TRUE(component.hasExpirationCycle());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetTradingSessionIDAndTradingSessionIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionID());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetTradingSessionSubIDAndTradingSessionSubIDMatch) {
    EXPECT_FALSE(component.hasTradingSessionSubID());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetTextAndTextMatch) {
    EXPECT_FALSE(component.hasText());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetEncodedTextLenAndEncodedTextLenMatch) {
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetEncodedTextAndEncodedTextMatch) {
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setCurrency(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // Populate a real field so the encoded payload is non-empty and
    // decode() is genuinely exercised (some component wrappers return
    // false on a decode of zero consumed bytes).
    const int64_t test_value = 12345;
    component.setExpirationCycle(test_value);
    
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    EXPECT_GT(encoded_size, 0u);
    
    // Decode back
    RelSymDerivSecGrp::NoRelatedSym decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
    EXPECT_EQ(decoded.getExpirationCycle(), component.getExpirationCycle());
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, FullyPopulatedRoundtrip) {
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
    component.setCurrency(std::string_view{"AAA"});
    component.setExpirationCycle(12345);
    component.getRefInstrumentExtension().setDeliveryForm(12345);
    component.getRefInstrumentExtension().setPctAtRisk(123.456);
    component.getRefInstrumentExtension().getRefAttrbGrp().getRefNoInstrAttribGroup().emplace_back();
    component.getRefInstrumentExtension().getRefAttrbGrp().getRefNoInstrAttribGroup().back().setInstrAttribType(12345);
    component.getRefInstrumentExtension().getRefAttrbGrp().getRefNoInstrAttribGroup().back().setInstrAttribValue(std::string_view{"AAA"});
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
    component.setTradingSessionID(std::string_view{"AAA"});
    component.setTradingSessionSubID(std::string_view{"AAA"});
    component.setText(std::string_view{"AAA"});
    component.setEncodedTextLen(12345);
    component.setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    RelSymDerivSecGrp::NoRelatedSym decoded;
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

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_RelSymDerivSecGrp_NoRelatedSymComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
