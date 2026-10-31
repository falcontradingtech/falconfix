// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotEntryGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotEntryGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotEntryGrpComponentTest : public ::testing::Test {
protected:
    QuotEntryGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotEntryGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_QuotEntryGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoQuoteEntriesGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoQuoteEntriesGroup().back().setQuoteEntryID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotEntryGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotEntryGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoQuoteEntriesGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoQuoteEntriesGroup().back().setQuoteEntryID(test_value); }
    component.getRefNoQuoteEntriesGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoQuoteEntriesGroup().back().setQuoteEntryID(test_value); }
    
    EXPECT_EQ(component.getRefNoQuoteEntriesGroup().size(), 2u);
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
    
    QuotEntryGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoQuoteEntriesGroup().size(), component.getRefNoQuoteEntriesGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    QuotEntryGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_QuotEntryGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoQuoteEntriesGroup().emplace_back();
    component.getRefNoQuoteEntriesGroup().back().setQuoteEntryID(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setProduct(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().emplace_back();
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegProduct(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegSide('X');
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().getRefInstrmtLegGrp().getRefNoLegsGroup().back().getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    component.getRefNoQuoteEntriesGroup().back().setBidPx(123.456);
    component.getRefNoQuoteEntriesGroup().back().setOfferPx(123.456);
    component.getRefNoQuoteEntriesGroup().back().setBidSize(123.456);
    component.getRefNoQuoteEntriesGroup().back().setOfferSize(123.456);
    component.getRefNoQuoteEntriesGroup().back().setValidUntilTime(12345);
    component.getRefNoQuoteEntriesGroup().back().setBidSpotRate(123.456);
    component.getRefNoQuoteEntriesGroup().back().setOfferSpotRate(123.456);
    component.getRefNoQuoteEntriesGroup().back().setBidForwardPoints(123.456);
    component.getRefNoQuoteEntriesGroup().back().setOfferForwardPoints(123.456);
    component.getRefNoQuoteEntriesGroup().back().setMidPx(123.456);
    component.getRefNoQuoteEntriesGroup().back().setBidYield(123.456);
    component.getRefNoQuoteEntriesGroup().back().setMidYield(123.456);
    component.getRefNoQuoteEntriesGroup().back().setOfferYield(123.456);
    component.getRefNoQuoteEntriesGroup().back().setTransactTime(12345);
    component.getRefNoQuoteEntriesGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoQuoteEntriesGroup().back().setSettlDate(12345);
    component.getRefNoQuoteEntriesGroup().back().setOrdType('X');
    component.getRefNoQuoteEntriesGroup().back().setSettlDate2(12345);
    component.getRefNoQuoteEntriesGroup().back().setOrderQty2(123.456);
    component.getRefNoQuoteEntriesGroup().back().setBidForwardPoints2(123.456);
    component.getRefNoQuoteEntriesGroup().back().setOfferForwardPoints2(123.456);
    component.getRefNoQuoteEntriesGroup().back().setCurrency(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotEntryGrp decoded;
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

TEST_F(FIX4_4_QuotEntryGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_QuotEntryGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
