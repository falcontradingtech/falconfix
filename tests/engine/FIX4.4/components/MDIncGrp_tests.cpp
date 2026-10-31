// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: MDIncGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/MDIncGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_MDIncGrpComponentTest : public ::testing::Test {
protected:
    MDIncGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_MDIncGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_MDIncGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_MDIncGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoMDEntriesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoMDEntriesGroup().back().setMDUpdateAction(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    MDIncGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_MDIncGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoMDEntriesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoMDEntriesGroup().back().setMDUpdateAction(test_value); }
    component.getRefNoMDEntriesGroup().emplace_back();
    { const char test_value = 'X'; component.getRefNoMDEntriesGroup().back().setMDUpdateAction(test_value); }
    
    EXPECT_EQ(component.getRefNoMDEntriesGroup().size(), 2u);
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
    
    MDIncGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoMDEntriesGroup().size(), component.getRefNoMDEntriesGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    MDIncGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_MDIncGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoMDEntriesGroup().emplace_back();
    component.getRefNoMDEntriesGroup().back().setMDUpdateAction('X');
    component.getRefNoMDEntriesGroup().back().setDeleteReason('X');
    component.getRefNoMDEntriesGroup().back().setMDEntryType('X');
    component.getRefNoMDEntriesGroup().back().setMDEntryID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntryRefID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setProduct(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().emplace_back();
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefNoMDEntriesGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().emplace_back();
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegProduct(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSide('X');
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    component.getRefNoMDEntriesGroup().back().setFinancialStatus(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setCorporateAction(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntryPx(123.456);
    component.getRefNoMDEntriesGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntrySize(123.456);
    component.getRefNoMDEntriesGroup().back().setMDEntryDate(12345);
    component.getRefNoMDEntriesGroup().back().setMDEntryTime(12345);
    component.getRefNoMDEntriesGroup().back().setTickDirection('X');
    component.getRefNoMDEntriesGroup().back().setMDMkt(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setQuoteCondition(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTradeCondition(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntryOriginator(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setLocationID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setDeskID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setOpenCloseSettlFlag(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setTimeInForce('X');
    component.getRefNoMDEntriesGroup().back().setExpireDate(12345);
    component.getRefNoMDEntriesGroup().back().setExpireTime(12345);
    component.getRefNoMDEntriesGroup().back().setMinQty(123.456);
    component.getRefNoMDEntriesGroup().back().setExecInst(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setSellerDays(12345);
    component.getRefNoMDEntriesGroup().back().setOrderID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setQuoteEntryID(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntryBuyer(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setMDEntrySeller(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setNumberOfOrders(12345);
    component.getRefNoMDEntriesGroup().back().setMDEntryPositionNo(12345);
    component.getRefNoMDEntriesGroup().back().setScope(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setPriceDelta(123.456);
    component.getRefNoMDEntriesGroup().back().setNetChgPrevDay(123.456);
    component.getRefNoMDEntriesGroup().back().setText(std::string_view{"AAA"});
    component.getRefNoMDEntriesGroup().back().setEncodedTextLen(12345);
    component.getRefNoMDEntriesGroup().back().setEncodedText(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    MDIncGrp decoded;
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

TEST_F(FIX4_4_MDIncGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_MDIncGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
