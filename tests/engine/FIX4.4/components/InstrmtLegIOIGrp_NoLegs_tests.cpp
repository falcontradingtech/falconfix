// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrmtLegIOIGrp::NoLegs (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/InstrmtLegIOIGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest : public ::testing::Test {
protected:
    InstrmtLegIOIGrp::NoLegs component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, ResetClearsAllFields) {
    // Set some fields
    { const std::string_view test_value{"AAA"}; component.setLegIOIQty(test_value); }
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, SetLegIOIQtyAndLegIOIQtyMatch) {
    EXPECT_FALSE(component.hasLegIOIQty());
}

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    { const std::string_view test_value{"AAA"}; component.setLegIOIQty(test_value); }
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    { const std::string_view test_value{"AAA"}; component.getRefInstrumentLeg().setLegSymbol(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtLegIOIGrp::NoLegs decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefInstrumentLeg().setLegSymbol(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegSymbolSfx(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegSecurityID(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegSecurityIDSource(std::string_view{"AAA"});
    component.getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().emplace_back();
    component.getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltID(std::string_view{"AAA"});
    component.getRefInstrumentLeg().getRefLegSecAltIDGrp().getRefNoLegSecurityAltIDGroup().back().setLegSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegProduct(12345);
    component.getRefInstrumentLeg().setLegCFICode(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegSecurityType(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegSecuritySubType(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegMaturityMonthYear(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegMaturityDate(12345);
    component.getRefInstrumentLeg().setLegCouponPaymentDate(12345);
    component.getRefInstrumentLeg().setLegIssueDate(12345);
    component.getRefInstrumentLeg().setLegRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegRepurchaseTerm(12345);
    component.getRefInstrumentLeg().setLegRepurchaseRate(123.456);
    component.getRefInstrumentLeg().setLegFactor(123.456);
    component.getRefInstrumentLeg().setLegCreditRating(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegInstrRegistry(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegCountryOfIssue(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegLocaleOfIssue(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegRedemptionDate(12345);
    component.getRefInstrumentLeg().setLegStrikePrice(123.456);
    component.getRefInstrumentLeg().setLegStrikeCurrency(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegOptAttribute('X');
    component.getRefInstrumentLeg().setLegContractMultiplier(123.456);
    component.getRefInstrumentLeg().setLegCouponRate(123.456);
    component.getRefInstrumentLeg().setLegSecurityExchange(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegIssuer(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setEncodedLegIssuerLen(12345);
    component.getRefInstrumentLeg().setEncodedLegIssuer(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setEncodedLegSecurityDescLen(12345);
    component.getRefInstrumentLeg().setEncodedLegSecurityDesc(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegRatioQty(123.456);
    component.getRefInstrumentLeg().setLegSide('X');
    component.getRefInstrumentLeg().setLegCurrency(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegPool(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegDatedDate(12345);
    component.getRefInstrumentLeg().setLegContractSettlMonth(std::string_view{"AAA"});
    component.getRefInstrumentLeg().setLegInterestAccrualDate(12345);
    component.setLegIOIQty(std::string_view{"AAA"});
    component.getRefLegStipulations().getRefNoLegStipulationsGroup().emplace_back();
    component.getRefLegStipulations().getRefNoLegStipulationsGroup().back().setLegStipulationType(std::string_view{"AAA"});
    component.getRefLegStipulations().getRefNoLegStipulationsGroup().back().setLegStipulationValue(std::string_view{"AAA"});
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtLegIOIGrp::NoLegs decoded;
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

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_InstrmtLegIOIGrp_NoLegsComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
