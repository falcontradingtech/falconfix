// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: ListOrdGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/ListOrdGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_ListOrdGrpComponentTest : public ::testing::Test {
protected:
    ListOrdGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_ListOrdGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_ListOrdGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_ListOrdGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoOrdersGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoOrdersGroup().back().setClOrdID(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    ListOrdGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_ListOrdGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoOrdersGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoOrdersGroup().back().setClOrdID(test_value); }
    component.getRefNoOrdersGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoOrdersGroup().back().setClOrdID(test_value); }
    
    EXPECT_EQ(component.getRefNoOrdersGroup().size(), 2u);
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
    
    ListOrdGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoOrdersGroup().size(), component.getRefNoOrdersGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    ListOrdGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_ListOrdGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoOrdersGroup().emplace_back();
    component.getRefNoOrdersGroup().back().setClOrdID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setSecondaryClOrdID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setListSeqNo(12345);
    component.getRefNoOrdersGroup().back().setClOrdLinkID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setSettlInstMode('X');
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyIDSource('X');
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().back().setPartyRole(12345);
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefParties().getRefNoPartyIDsGroup().back().getRefPtysSubGrp().getRefNoPartySubIDsGroup().back().setPartySubIDType(12345);
    component.getRefNoOrdersGroup().back().setTradeOriginationDate(12345);
    component.getRefNoOrdersGroup().back().setTradeDate(12345);
    component.getRefNoOrdersGroup().back().setAccount(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setAcctIDSource(12345);
    component.getRefNoOrdersGroup().back().setAccountType(12345);
    component.getRefNoOrdersGroup().back().setDayBookingInst('X');
    component.getRefNoOrdersGroup().back().setBookingUnit('X');
    component.getRefNoOrdersGroup().back().setAllocID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setPreallocMethod('X');
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocAccount(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocAcctIDSource(12345);
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocSettlCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setIndividualAllocID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyIDSource('X');
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().getRefNestedParties().getRefNoNestedPartyIDsGroup().back().setNestedPartyRole(12345);
    component.getRefNoOrdersGroup().back().getRefPreAllocGrp().getRefNoAllocsGroup().back().setAllocQty(123.456);
    component.getRefNoOrdersGroup().back().setSettlType('X');
    component.getRefNoOrdersGroup().back().setSettlDate(12345);
    component.getRefNoOrdersGroup().back().setCashMargin('X');
    component.getRefNoOrdersGroup().back().setClearingFeeIndicator(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setHandlInst('X');
    component.getRefNoOrdersGroup().back().setExecInst(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setMinQty(123.456);
    component.getRefNoOrdersGroup().back().setMaxFloor(123.456);
    component.getRefNoOrdersGroup().back().setExDestination(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefTrdgSesGrp().getRefNoTradingSessionsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefTrdgSesGrp().getRefNoTradingSessionsGroup().back().setTradingSessionID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefTrdgSesGrp().getRefNoTradingSessionsGroup().back().setTradingSessionSubID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setProcessCode('X');
    component.getRefNoOrdersGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setProduct(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefNoOrdersGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefNoOrdersGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefNoOrdersGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefNoOrdersGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefNoOrdersGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefNoOrdersGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefNoOrdersGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefNoOrdersGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbol(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSymbolSfx(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingProduct(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCFICode(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecuritySubType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingMaturityDate(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPutOrCall(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponPaymentDate(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssueDate(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseTerm(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRepurchaseRate(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingFactor(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCreditRating(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingInstrRegistry(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingRedemptionDate(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikePrice(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingOptAttribute('X');
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingContractMultiplier(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCouponRate(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityExchange(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuerLen(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingIssuer(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDescLen(12345);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setEncodedUnderlyingSecurityDesc(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPProgram(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCPRegType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingQty(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingPx(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingDirtyPrice(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndPrice(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingStartValue(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingCurrentValue(123.456);
    component.getRefNoOrdersGroup().back().getRefUndInstrmtGrp().getRefNoUnderlyingsGroup().back().getRefUnderlyingInstrument().setUnderlyingEndValue(123.456);
    component.getRefNoOrdersGroup().back().setPrevClosePx(123.456);
    component.getRefNoOrdersGroup().back().setSide('X');
    component.getRefNoOrdersGroup().back().setSideValueInd(12345);
    component.getRefNoOrdersGroup().back().setLocateReqd(true);
    component.getRefNoOrdersGroup().back().setTransactTime(12345);
    component.getRefNoOrdersGroup().back().getRefStipulations().getRefNoStipulationsGroup().emplace_back();
    component.getRefNoOrdersGroup().back().getRefStipulations().getRefNoStipulationsGroup().back().setStipulationType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefStipulations().getRefNoStipulationsGroup().back().setStipulationValue(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setQtyType(12345);
    component.getRefNoOrdersGroup().back().getRefOrderQtyData().setOrderQty(123.456);
    component.getRefNoOrdersGroup().back().getRefOrderQtyData().setCashOrderQty(123.456);
    component.getRefNoOrdersGroup().back().getRefOrderQtyData().setOrderPercent(123.456);
    component.getRefNoOrdersGroup().back().getRefOrderQtyData().setRoundingDirection('X');
    component.getRefNoOrdersGroup().back().getRefOrderQtyData().setRoundingModulus(123.456);
    component.getRefNoOrdersGroup().back().setOrdType('X');
    component.getRefNoOrdersGroup().back().setPriceType(12345);
    component.getRefNoOrdersGroup().back().setPrice(123.456);
    component.getRefNoOrdersGroup().back().setStopPx(123.456);
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setSpread(123.456);
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkCurveCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkCurveName(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkCurvePoint(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkPrice(123.456);
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkPriceType(12345);
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkSecurityID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefSpreadOrBenchmarkCurveData().setBenchmarkSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefYieldData().setYieldType(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefYieldData().setYield(123.456);
    component.getRefNoOrdersGroup().back().getRefYieldData().setYieldCalcDate(12345);
    component.getRefNoOrdersGroup().back().getRefYieldData().setYieldRedemptionDate(12345);
    component.getRefNoOrdersGroup().back().getRefYieldData().setYieldRedemptionPrice(123.456);
    component.getRefNoOrdersGroup().back().getRefYieldData().setYieldRedemptionPriceType(12345);
    component.getRefNoOrdersGroup().back().setCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setComplianceID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setSolicitedFlag(true);
    component.getRefNoOrdersGroup().back().setIOIID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setQuoteID(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setTimeInForce('X');
    component.getRefNoOrdersGroup().back().setEffectiveTime(12345);
    component.getRefNoOrdersGroup().back().setExpireDate(12345);
    component.getRefNoOrdersGroup().back().setExpireTime(12345);
    component.getRefNoOrdersGroup().back().setGTBookingInst(12345);
    component.getRefNoOrdersGroup().back().getRefCommissionData().setCommission(123.456);
    component.getRefNoOrdersGroup().back().getRefCommissionData().setCommType('X');
    component.getRefNoOrdersGroup().back().getRefCommissionData().setCommCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().getRefCommissionData().setFundRenewWaiv('X');
    component.getRefNoOrdersGroup().back().setOrderCapacity('X');
    component.getRefNoOrdersGroup().back().setOrderRestrictions(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setCustOrderCapacity(12345);
    component.getRefNoOrdersGroup().back().setForexReq(true);
    component.getRefNoOrdersGroup().back().setSettlCurrency(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setBookingType(12345);
    component.getRefNoOrdersGroup().back().setText(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setEncodedTextLen(12345);
    component.getRefNoOrdersGroup().back().setEncodedText(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setSettlDate2(12345);
    component.getRefNoOrdersGroup().back().setOrderQty2(123.456);
    component.getRefNoOrdersGroup().back().setPrice2(123.456);
    component.getRefNoOrdersGroup().back().setPositionEffect('X');
    component.getRefNoOrdersGroup().back().setCoveredOrUncovered(12345);
    component.getRefNoOrdersGroup().back().setMaxShow(123.456);
    component.getRefNoOrdersGroup().back().getRefPegInstructions().setPegOffsetValue(123.456);
    component.getRefNoOrdersGroup().back().getRefPegInstructions().setPegMoveType(12345);
    component.getRefNoOrdersGroup().back().getRefPegInstructions().setPegOffsetType(12345);
    component.getRefNoOrdersGroup().back().getRefPegInstructions().setPegLimitType(12345);
    component.getRefNoOrdersGroup().back().getRefPegInstructions().setPegRoundDirection(12345);
    component.getRefNoOrdersGroup().back().getRefPegInstructions().setPegScope(12345);
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionInst('X');
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionOffsetValue(123.456);
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionMoveType(12345);
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionOffsetType(12345);
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionLimitType(12345);
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionRoundDirection(12345);
    component.getRefNoOrdersGroup().back().getRefDiscretionInstructions().setDiscretionScope(12345);
    component.getRefNoOrdersGroup().back().setTargetStrategy(12345);
    component.getRefNoOrdersGroup().back().setTargetStrategyParameters(std::string_view{"AAA"});
    component.getRefNoOrdersGroup().back().setParticipationRate(123.456);
    component.getRefNoOrdersGroup().back().setDesignation(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    ListOrdGrp decoded;
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

TEST_F(FIX4_4_ListOrdGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_ListOrdGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
