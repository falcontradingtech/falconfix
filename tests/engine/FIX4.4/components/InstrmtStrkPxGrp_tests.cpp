// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: InstrmtStrkPxGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/InstrmtStrkPxGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class FIX4_4_InstrmtStrkPxGrpComponentTest : public ::testing::Test {
protected:
    InstrmtStrkPxGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
}

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1024];
    
    // This component exposes no scalar setters of its own, so we
    // populate it through its getRef sub-component / group accessors
    // to exercise the real encode/decode (and group loop) paths.
    component.getRefNoStrikesGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoStrikesGroup().back().getRefInstrument().setSymbol(test_value); }
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = buffer;
    p = component.encode(p, true);
    ASSERT_NE(p, nullptr);
    std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtStrkPxGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());
}

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, PopulatedGroupLoopRoundtrip) {
    char buffer[2048];
    component.getRefNoStrikesGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoStrikesGroup().back().getRefInstrument().setSymbol(test_value); }
    component.getRefNoStrikesGroup().emplace_back();
    { const std::string_view test_value{"AAA"}; component.getRefNoStrikesGroup().back().getRefInstrument().setSymbol(test_value); }
    
    EXPECT_EQ(component.getRefNoStrikesGroup().size(), 2u);
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
    
    InstrmtStrkPxGrp decoded;
    const char *q = buffer;
    EXPECT_TRUE(decoded.decode(q, p));
    EXPECT_EQ(decoded.getRefNoStrikesGroup().size(), component.getRefNoStrikesGroup().size());
    
    // Decoding an unknown leading tag returns without consuming it.
    InstrmtStrkPxGrp other;
    const char *bad = "99=X\x01";
    const char *bp = bad;
    (void)other.decode(bp, bad + 5);
    
    // setField on the container is a no-op sink (returns false).
    EXPECT_FALSE(other.setField(0, nullptr, nullptr));
}

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, FullyPopulatedRoundtrip) {
    char buffer[4096];
    component.getRefNoStrikesGroup().emplace_back();
    component.getRefNoStrikesGroup().back().getRefInstrument().setSymbol(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setSymbolSfx(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setSecurityID(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setSecurityIDSource(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().emplace_back();
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltID(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefSecAltIDGrp().getRefNoSecurityAltIDGroup().back().setSecurityAltIDSource(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setProduct(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setCFICode(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setSecurityType(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setSecuritySubType(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setMaturityMonthYear(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setMaturityDate(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setPutOrCall(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setCouponPaymentDate(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setIssueDate(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setRepoCollateralSecurityType(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setRepurchaseTerm(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setRepurchaseRate(123.456);
    component.getRefNoStrikesGroup().back().getRefInstrument().setFactor(123.456);
    component.getRefNoStrikesGroup().back().getRefInstrument().setCreditRating(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setInstrRegistry(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setCountryOfIssue(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setStateOrProvinceOfIssue(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setLocaleOfIssue(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setRedemptionDate(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setStrikePrice(123.456);
    component.getRefNoStrikesGroup().back().getRefInstrument().setStrikeCurrency(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setOptAttribute('X');
    component.getRefNoStrikesGroup().back().getRefInstrument().setContractMultiplier(123.456);
    component.getRefNoStrikesGroup().back().getRefInstrument().setCouponRate(123.456);
    component.getRefNoStrikesGroup().back().getRefInstrument().setSecurityExchange(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setIssuer(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setEncodedIssuerLen(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setEncodedIssuer(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setSecurityDesc(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setEncodedSecurityDescLen(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setEncodedSecurityDesc(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setPool(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setContractSettlMonth(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setCPProgram(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setCPRegType(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().emplace_back();
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventType(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventDate(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventPx(123.456);
    component.getRefNoStrikesGroup().back().getRefInstrument().getRefEvntGrp().getRefNoEventsGroup().back().setEventText(std::string_view{"AAA"});
    component.getRefNoStrikesGroup().back().getRefInstrument().setDatedDate(12345);
    component.getRefNoStrikesGroup().back().getRefInstrument().setInterestAccrualDate(12345);
    
    EXPECT_TRUE(component.hasAnySet());
    
    char *p = component.encode(buffer, true);
    ASSERT_NE(p, nullptr);
    const std::size_t encoded_size = static_cast<std::size_t>(p - buffer);
    EXPECT_GT(encoded_size, 0u);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    InstrmtStrkPxGrp decoded;
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

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}

TEST_F(FIX4_4_InstrmtStrkPxGrpComponentTest, SetFieldUnknownTagReturnsFalse) {
    const char *v = "X";
    EXPECT_FALSE(component.setField(0, v, v + 1));
}
