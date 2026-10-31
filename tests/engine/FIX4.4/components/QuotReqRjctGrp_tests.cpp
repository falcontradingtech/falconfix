// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotReqRjctGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotReqRjctGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotReqRjctGrpComponentTest : public ::testing::Test {
protected:
    QuotReqRjctGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoRelatedSymGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoRelatedSymGroup().back().getRefInstrument().setSymbol(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotReqRjctGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoRelatedSymGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoRelatedSymGroup().back().getRefInstrument().setSymbol(test_value); }
    component.getRefNoRelatedSymGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoRelatedSymGroup().back().getRefInstrument().setSymbol(test_value); }
    
    EXPECT_EQ(component.getRefNoRelatedSymGroup().size(), 2u);
    EXPECT_TRUE(component.hasAnySet());
    // We populate each entry's decode anchor (first field) to drive the
    // group encode/decode loop. That does not necessarily satisfy every
    // REQUIRED field of the entry, so we only exercise checkRequired()
    // for coverage here; its truth value is asserted by the dedicated
    // CheckRequiredWhenEmpty test.
    (void)component.checkRequired();
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    EXPECT_GT(static_cast<std::size_t>(p - buffer), 0u);
    
    QuotReqRjctGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoRelatedSymGroup().size(), component.getRefNoRelatedSymGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    QuotReqRjctGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoRelatedSymGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setProduct(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setAgreementDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setAgreementID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setAgreementDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setAgreementCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setTerminationType(12345);
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setStartDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setEndDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setDeliveryType(12345);
    component.getRefNoRelatedSymGroup().back().getRefFinancingDetails().setMarginRatio(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefNoRelatedSymGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefNoRelatedSymGroup().back().setPrevClosePx(123.456);
    component.getRefNoRelatedSymGroup().back().setQuoteRequestType(12345);
    component.getRefNoRelatedSymGroup().back().setQuoteType(12345);
    component.getRefNoRelatedSymGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().setTradeOriginationDate(12345);
    component.getRefNoRelatedSymGroup().back().setSide('X');
    component.getRefNoRelatedSymGroup().back().setQtyType(12345);
    component.getRefNoRelatedSymGroup().back().getRefOrderQtyData().setOrderQty(123.456);
    component.getRefNoRelatedSymGroup().back().getRefOrderQtyData().setCashOrderQty(123.456);
    component.getRefNoRelatedSymGroup().back().getRefOrderQtyData().setOrderPercent(123.456);
    component.getRefNoRelatedSymGroup().back().getRefOrderQtyData().setRoundingDirection('X');
    component.getRefNoRelatedSymGroup().back().getRefOrderQtyData().setRoundingModulus(123.456);
    component.getRefNoRelatedSymGroup().back().setSettlType('X');
    component.getRefNoRelatedSymGroup().back().setSettlDate(12345);
    component.getRefNoRelatedSymGroup().back().setSettlDate2(12345);
    component.getRefNoRelatedSymGroup().back().setOrderQty2(123.456);
    component.getRefNoRelatedSymGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefStipulations().getRefNoStipulationsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefStipulations().getRefNoStipulationsGroup().back().setStipulationType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefStipulations().getRefNoStipulationsGroup().back().setStipulationValue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().setAccount(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().setAcctIDSource(12345);
    component.getRefNoRelatedSymGroup().back().setAccountType(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegProduct(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSide('X');
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().setLegQty(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().setLegSwapType(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().setLegSettlType('X');
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().setLegSettlDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegStipulations().getRefNoLegStipulationsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegStipulations().getRefNoLegStipulationsGroup().back().setLegStipulationType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegStipulations().getRefNoLegStipulationsGroup().back().setLegStipulationValue(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegBenchmarkCurveData().setLegBenchmarkCurveCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegBenchmarkCurveData().setLegBenchmarkCurveName(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegBenchmarkCurveData().setLegBenchmarkCurvePoint(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegBenchmarkCurveData().setLegBenchmarkPrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefQuotReqLegsGrp().getRefNoLegsGroup().back().getRefLegBenchmarkCurveData().setLegBenchmarkPriceType(12345);
    component.getRefNoRelatedSymGroup().back().getRefQuotQualGrp().getRefNoQuoteQualifiersGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefQuotQualGrp().getRefNoQuoteQualifiersGroup().back().setQuoteQualifier('X');
    component.getRefNoRelatedSymGroup().back().setQuotePriceType(12345);
    component.getRefNoRelatedSymGroup().back().setOrdType('X');
    component.getRefNoRelatedSymGroup().back().setExpireTime(12345);
    component.getRefNoRelatedSymGroup().back().setTransactTime(12345);
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setSpread(123.456);
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkCurveCurrency(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkCurveName(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkCurvePoint(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkPrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkPriceType(12345);
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkSecurityID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().setPriceType(12345);
    component.getRefNoRelatedSymGroup().back().setPrice(123.456);
    component.getRefNoRelatedSymGroup().back().setPrice2(123.456);
    component.getRefNoRelatedSymGroup().back().getRefYieldData().setYieldType(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefYieldData().setYield(123.456);
    component.getRefNoRelatedSymGroup().back().getRefYieldData().setYieldCalcDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefYieldData().setYieldRedemptionDate(12345);
    component.getRefNoRelatedSymGroup().back().getRefYieldData().setYieldRedemptionPrice(123.456);
    component.getRefNoRelatedSymGroup().back().getRefYieldData().setYieldRedemptionPriceType(12345);
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefNoRelatedSymGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotReqRjctGrp decoded;
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

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_QuotReqRjctGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
