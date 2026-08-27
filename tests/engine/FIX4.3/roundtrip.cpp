// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated roundtrip tests. Do not edit by hand.
// FIX version: FIX4.3

#include <gtest/gtest.h>

#include <string_view>
#include <type_traits>
#include <utility>

#include <FIX4.3/utils/all_messages.h>
#include <FIX4.3/samples/fix_messages.h>

#include <utils/error_codes.h>
#include <utils/fast_buffer.h>

template <typename MsgT>
inline void runRoundTripFor() {}

namespace {

template<typename T, typename = void>
struct has_set_string : std::false_type {};

template<typename T>
struct has_set_string<T, std::void_t<decltype(std::declval<T&>().setString(std::declval<std::string_view>()))>> : std::true_type {};

template<typename T, typename = void>
struct has_to_string : std::false_type {};

template<typename T>
struct has_to_string<T, std::void_t<decltype(std::declval<const T&>().toString(std::declval<falconfix::FastBuffer&>()))>> : std::true_type {};

template<typename MsgT>
void runRoundTripFor(std::string_view sample_name) {
	if constexpr (!has_set_string<MsgT>::value || !has_to_string<MsgT>::value) {
		GTEST_SKIP() << "roundtrip unsupported for this message API";
	}
	else {
	const std::string_view fixraw = fix43::samples::map_samples[sample_name];
	ASSERT_FALSE(fixraw.empty()) << "missing sample for " << sample_name;

	MsgT msg;
	const auto decode_rc = msg.setString(fixraw);
	ASSERT_TRUE(decode_rc.ok())
		<< "decode failed err=" << decode_rc.code
		<< " (" << falconfix::errors::error_message(decode_rc) << ")";

	falconfix::FastBuffer outbuf;
	const auto encode_rc = msg.toString(outbuf);
	ASSERT_TRUE(encode_rc.ok())
		<< "encode failed err=" << encode_rc.code
		<< " (" << falconfix::errors::error_message(encode_rc) << ")";
	ASSERT_EQ(outbuf.view(), fixraw) << "Roundtrip mismatch";
	}
}


} // namespace

TEST(RoundTrip_FIX4_3, Heartbeat_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Heartbeat>("Heartbeat");
}

TEST(RoundTrip_FIX4_3, TestRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::TestRequest>("TestRequest");
}

TEST(RoundTrip_FIX4_3, ResendRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ResendRequest>("ResendRequest");
}

TEST(RoundTrip_FIX4_3, Reject_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Reject>("Reject");
}

TEST(RoundTrip_FIX4_3, SequenceReset_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SequenceReset>("SequenceReset");
}

TEST(RoundTrip_FIX4_3, Logout_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Logout>("Logout");
}

TEST(RoundTrip_FIX4_3, IOI_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::IOI>("IOI");
}

TEST(RoundTrip_FIX4_3, Advertisement_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Advertisement>("Advertisement");
}

TEST(RoundTrip_FIX4_3, ExecutionReport_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ExecutionReport>("ExecutionReport");
}

TEST(RoundTrip_FIX4_3, OrderCancelReject_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderCancelReject>("OrderCancelReject");
}

TEST(RoundTrip_FIX4_3, Logon_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Logon>("Logon");
}

TEST(RoundTrip_FIX4_3, News_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::News>("News");
}

TEST(RoundTrip_FIX4_3, Email_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Email>("Email");
}

TEST(RoundTrip_FIX4_3, NewOrderSingle_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::NewOrderSingle>("NewOrderSingle");
}

TEST(RoundTrip_FIX4_3, NewOrderList_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::NewOrderList>("NewOrderList");
}

TEST(RoundTrip_FIX4_3, OrderCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderCancelRequest>("OrderCancelRequest");
}

TEST(RoundTrip_FIX4_3, OrderCancelReplaceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderCancelReplaceRequest>("OrderCancelReplaceRequest");
}

TEST(RoundTrip_FIX4_3, OrderStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderStatusRequest>("OrderStatusRequest");
}

TEST(RoundTrip_FIX4_3, Allocation_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Allocation>("Allocation");
}

TEST(RoundTrip_FIX4_3, ListCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ListCancelRequest>("ListCancelRequest");
}

TEST(RoundTrip_FIX4_3, ListExecute_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ListExecute>("ListExecute");
}

TEST(RoundTrip_FIX4_3, ListStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ListStatusRequest>("ListStatusRequest");
}

TEST(RoundTrip_FIX4_3, ListStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ListStatus>("ListStatus");
}

TEST(RoundTrip_FIX4_3, AllocationAck_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::AllocationAck>("AllocationAck");
}

TEST(RoundTrip_FIX4_3, DontKnowTrade_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::DontKnowTrade>("DontKnowTrade");
}

TEST(RoundTrip_FIX4_3, QuoteRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::QuoteRequest>("QuoteRequest");
}

TEST(RoundTrip_FIX4_3, Quote_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::Quote>("Quote");
}

TEST(RoundTrip_FIX4_3, SettlementInstructions_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SettlementInstructions>("SettlementInstructions");
}

TEST(RoundTrip_FIX4_3, MarketDataRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MarketDataRequest>("MarketDataRequest");
}

TEST(RoundTrip_FIX4_3, MarketDataSnapshotFullRefresh_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MarketDataSnapshotFullRefresh>("MarketDataSnapshotFullRefresh");
}

TEST(RoundTrip_FIX4_3, MarketDataIncrementalRefresh_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MarketDataIncrementalRefresh>("MarketDataIncrementalRefresh");
}

TEST(RoundTrip_FIX4_3, MarketDataRequestReject_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MarketDataRequestReject>("MarketDataRequestReject");
}

TEST(RoundTrip_FIX4_3, QuoteCancel_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::QuoteCancel>("QuoteCancel");
}

TEST(RoundTrip_FIX4_3, QuoteStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::QuoteStatusRequest>("QuoteStatusRequest");
}

TEST(RoundTrip_FIX4_3, MassQuoteAcknowledgement_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MassQuoteAcknowledgement>("MassQuoteAcknowledgement");
}

TEST(RoundTrip_FIX4_3, SecurityDefinitionRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityDefinitionRequest>("SecurityDefinitionRequest");
}

TEST(RoundTrip_FIX4_3, SecurityDefinition_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityDefinition>("SecurityDefinition");
}

TEST(RoundTrip_FIX4_3, SecurityStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityStatusRequest>("SecurityStatusRequest");
}

TEST(RoundTrip_FIX4_3, SecurityStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityStatus>("SecurityStatus");
}

TEST(RoundTrip_FIX4_3, TradingSessionStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::TradingSessionStatusRequest>("TradingSessionStatusRequest");
}

TEST(RoundTrip_FIX4_3, TradingSessionStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::TradingSessionStatus>("TradingSessionStatus");
}

TEST(RoundTrip_FIX4_3, MassQuote_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MassQuote>("MassQuote");
}

TEST(RoundTrip_FIX4_3, BusinessMessageReject_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::BusinessMessageReject>("BusinessMessageReject");
}

TEST(RoundTrip_FIX4_3, BidRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::BidRequest>("BidRequest");
}

TEST(RoundTrip_FIX4_3, BidResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::BidResponse>("BidResponse");
}

TEST(RoundTrip_FIX4_3, ListStrikePrice_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::ListStrikePrice>("ListStrikePrice");
}

TEST(RoundTrip_FIX4_3, XMLnonFIX_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::XMLnonFIX>("XMLnonFIX");
}

TEST(RoundTrip_FIX4_3, RegistrationInstructions_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::RegistrationInstructions>("RegistrationInstructions");
}

TEST(RoundTrip_FIX4_3, RegistrationInstructionsResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::RegistrationInstructionsResponse>("RegistrationInstructionsResponse");
}

TEST(RoundTrip_FIX4_3, OrderMassCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderMassCancelRequest>("OrderMassCancelRequest");
}

TEST(RoundTrip_FIX4_3, OrderMassCancelReport_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderMassCancelReport>("OrderMassCancelReport");
}

TEST(RoundTrip_FIX4_3, NewOrderCross_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::NewOrderCross>("NewOrderCross");
}

TEST(RoundTrip_FIX4_3, CrossOrderCancelReplaceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::CrossOrderCancelReplaceRequest>("CrossOrderCancelReplaceRequest");
}

TEST(RoundTrip_FIX4_3, CrossOrderCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::CrossOrderCancelRequest>("CrossOrderCancelRequest");
}

TEST(RoundTrip_FIX4_3, SecurityTypeRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityTypeRequest>("SecurityTypeRequest");
}

TEST(RoundTrip_FIX4_3, SecurityTypes_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityTypes>("SecurityTypes");
}

TEST(RoundTrip_FIX4_3, SecurityListRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityListRequest>("SecurityListRequest");
}

TEST(RoundTrip_FIX4_3, SecurityList_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::SecurityList>("SecurityList");
}

TEST(RoundTrip_FIX4_3, DerivativeSecurityListRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::DerivativeSecurityListRequest>("DerivativeSecurityListRequest");
}

TEST(RoundTrip_FIX4_3, DerivativeSecurityList_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::DerivativeSecurityList>("DerivativeSecurityList");
}

TEST(RoundTrip_FIX4_3, NewOrderMultileg_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::NewOrderMultileg>("NewOrderMultileg");
}

TEST(RoundTrip_FIX4_3, MultilegOrderCancelReplaceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::MultilegOrderCancelReplaceRequest>("MultilegOrderCancelReplaceRequest");
}

TEST(RoundTrip_FIX4_3, TradeCaptureReportRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::TradeCaptureReportRequest>("TradeCaptureReportRequest");
}

TEST(RoundTrip_FIX4_3, TradeCaptureReport_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::TradeCaptureReport>("TradeCaptureReport");
}

TEST(RoundTrip_FIX4_3, OrderMassStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::OrderMassStatusRequest>("OrderMassStatusRequest");
}

TEST(RoundTrip_FIX4_3, QuoteRequestReject_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::QuoteRequestReject>("QuoteRequestReject");
}

TEST(RoundTrip_FIX4_3, RFQRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::RFQRequest>("RFQRequest");
}

TEST(RoundTrip_FIX4_3, QuoteStatusReport_MatchesGeneratedSample) {
	runRoundTripFor<fix43::messages::QuoteStatusReport>("QuoteStatusReport");
}

