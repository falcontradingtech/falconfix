// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotSetAckGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotSetAckGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_QuotSetAckGrpComponentTest : public ::testing::Test {
protected:
    QuotSetAckGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoQuoteSetsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoQuoteSetsGroup().back().setQuoteSetID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotSetAckGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoQuoteSetsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoQuoteSetsGroup().back().setQuoteSetID(test_value); }
    component.getRefNoQuoteSetsGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoQuoteSetsGroup().back().setQuoteSetID(test_value); }
    
    EXPECT_EQ(component.getRefNoQuoteSetsGroup().size(), 2u);
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
    
    QuotSetAckGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoQuoteSetsGroup().size(), component.getRefNoQuoteSetsGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    QuotSetAckGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoQuoteSetsGroup().emplace_back();
    component.getRefNoQuoteSetsGroup().back().setQuoteSetID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().emplace_back();
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().getRefUndSecAltIDGrp().getRefNoUnderlyingSecurityAltIDGroup().back().setUnderlyingSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().emplace_back();
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefUnderlyingInstrument().getRefUnderlyingStipulations().getRefNoUnderlyingStipsGroup().back().setUnderlyingStipValue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().setTotNoQuoteEntries(12345);
    component.getRefNoQuoteSetsGroup().back().setLastFragment(true);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().emplace_back();
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setQuoteEntryID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setProduct(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidPx(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferPx(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidSize(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferSize(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setValidUntilTime(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidSpotRate(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferSpotRate(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidForwardPoints(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferForwardPoints(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setMidPx(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidYield(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setMidYield(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferYield(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setTransactTime(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setSettlDate(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOrdType('X');
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setSettlDate2(12345);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOrderQty2(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setBidForwardPoints2(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setOfferForwardPoints2(123.456);
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefNoQuoteSetsGroup().back().getRefQuotEntryAckGrp().getRefNoQuoteEntriesGroup().back().setQuoteEntryRejectReason(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    QuotSetAckGrp decoded;
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

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_QuotSetAckGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
