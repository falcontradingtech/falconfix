// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated component tests. Do not edit by hand.
// Component: QuotReqGrp (FIX4.4)

#include <gtest/gtest.h>
#include <string_view>
#include <cstring>

#include <FIX4.4/components/QuotReqGrp.h>
#include <FIX4.4/utils/serializer.h>
#include <utils/fast_buffer.h>

using namespace fix44::components;

class QuotReqGrpComponentTest : public ::testing::Test {
protected:
    QuotReqGrp component;

    void SetUp() override {
        component.reset();
    }
};

TEST_F(QuotReqGrpComponentTest, ResetClearsAllFields) {
    // Set some fields
    component.setPrevClosePx(3.14f);
    component.setQuoteRequestType(42);
    component.setQuoteType(42);
    
    // Reset component
    component.reset();
    
    // Verify all fields are cleared
    EXPECT_FALSE(component.hasAnySet());
}

TEST_F(QuotReqGrpComponentTest, SetPrevClosePxAndPrevClosePxMatch) {
    const double test_value = 123.456;
    component.setPrevClosePx(test_value);
    EXPECT_EQ(component.getPrevClosePx(), test_value);
    EXPECT_TRUE(component.hasPrevClosePx());
}

TEST_F(QuotReqGrpComponentTest, SetQuoteRequestTypeAndQuoteRequestTypeMatch) {
    const int64_t test_value = 12345;
    component.setQuoteRequestType(test_value);
    EXPECT_EQ(component.getQuoteRequestType(), test_value);
    EXPECT_TRUE(component.hasQuoteRequestType());
}

TEST_F(QuotReqGrpComponentTest, SetQuoteTypeAndQuoteTypeMatch) {
    const int64_t test_value = 12345;
    component.setQuoteType(test_value);
    EXPECT_EQ(component.getQuoteType(), test_value);
    EXPECT_TRUE(component.hasQuoteType());
}

TEST_F(QuotReqGrpComponentTest, SetTradeOriginationDateAndTradeOriginationDateMatch) {
}

TEST_F(QuotReqGrpComponentTest, SetSideAndSideMatch) {
    const char test_value = 'X';
    component.setSide(test_value);
    EXPECT_EQ(component.getSide(), test_value);
    EXPECT_TRUE(component.hasSide());
}

TEST_F(QuotReqGrpComponentTest, SetQtyTypeAndQtyTypeMatch) {
    const int64_t test_value = 12345;
    component.setQtyType(test_value);
    EXPECT_EQ(component.getQtyType(), test_value);
    EXPECT_TRUE(component.hasQtyType());
}

TEST_F(QuotReqGrpComponentTest, SetSettlTypeAndSettlTypeMatch) {
    const char test_value = 'X';
    component.setSettlType(test_value);
    EXPECT_EQ(component.getSettlType(), test_value);
    EXPECT_TRUE(component.hasSettlType());
}

TEST_F(QuotReqGrpComponentTest, SetSettlDateAndSettlDateMatch) {
}

TEST_F(QuotReqGrpComponentTest, SetSettlDate2AndSettlDate2Match) {
}

TEST_F(QuotReqGrpComponentTest, SetOrderQty2AndOrderQty2Match) {
    const double test_value = 123.456;
    component.setOrderQty2(test_value);
    EXPECT_EQ(component.getOrderQty2(), test_value);
    EXPECT_TRUE(component.hasOrderQty2());
}

TEST_F(QuotReqGrpComponentTest, SetAcctIDSourceAndAcctIDSourceMatch) {
    const int64_t test_value = 12345;
    component.setAcctIDSource(test_value);
    EXPECT_EQ(component.getAcctIDSource(), test_value);
    EXPECT_TRUE(component.hasAcctIDSource());
}

TEST_F(QuotReqGrpComponentTest, SetAccountTypeAndAccountTypeMatch) {
    const int64_t test_value = 12345;
    component.setAccountType(test_value);
    EXPECT_EQ(component.getAccountType(), test_value);
    EXPECT_TRUE(component.hasAccountType());
}

TEST_F(QuotReqGrpComponentTest, SetQuotePriceTypeAndQuotePriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setQuotePriceType(test_value);
    EXPECT_EQ(component.getQuotePriceType(), test_value);
    EXPECT_TRUE(component.hasQuotePriceType());
}

TEST_F(QuotReqGrpComponentTest, SetOrdTypeAndOrdTypeMatch) {
    const char test_value = 'X';
    component.setOrdType(test_value);
    EXPECT_EQ(component.getOrdType(), test_value);
    EXPECT_TRUE(component.hasOrdType());
}

TEST_F(QuotReqGrpComponentTest, SetValidUntilTimeAndValidUntilTimeMatch) {
}

TEST_F(QuotReqGrpComponentTest, SetExpireTimeAndExpireTimeMatch) {
}

TEST_F(QuotReqGrpComponentTest, SetTransactTimeAndTransactTimeMatch) {
}

TEST_F(QuotReqGrpComponentTest, SetPriceTypeAndPriceTypeMatch) {
    const int64_t test_value = 12345;
    component.setPriceType(test_value);
    EXPECT_EQ(component.getPriceType(), test_value);
    EXPECT_TRUE(component.hasPriceType());
}

TEST_F(QuotReqGrpComponentTest, SetPriceAndPriceMatch) {
    const double test_value = 123.456;
    component.setPrice(test_value);
    EXPECT_EQ(component.getPrice(), test_value);
    EXPECT_TRUE(component.hasPrice());
}

TEST_F(QuotReqGrpComponentTest, SetPrice2AndPrice2Match) {
    const double test_value = 123.456;
    component.setPrice2(test_value);
    EXPECT_EQ(component.getPrice2(), test_value);
    EXPECT_TRUE(component.hasPrice2());
}

TEST_F(QuotReqGrpComponentTest, HasAnySetTracksPresence) {
    EXPECT_FALSE(component.hasAnySet());
    
    component.setPrevClosePx(42);
    EXPECT_TRUE(component.hasAnySet());
}

TEST_F(QuotReqGrpComponentTest, EncodeDecodeRoundtrip) {
    char buffer[1104];
    
    // Encode empty component
    char *p = buffer;
    p = component.encode(p, true);
    std::size_t encoded_size = p - buffer;
    
    // Verify encode succeeded
    EXPECT_GT(encoded_size, 0);
    EXPECT_LE(encoded_size, component.compute_buffer_size());
    
    // Decode back
    QuotReqGrp decoded;
    const char *q = buffer;
    bool decode_result = decoded.decode(q, p);
    EXPECT_TRUE(decode_result);
}

TEST_F(QuotReqGrpComponentTest, CheckRequiredWhenEmpty) {
    EXPECT_TRUE(component.checkRequired());
}
