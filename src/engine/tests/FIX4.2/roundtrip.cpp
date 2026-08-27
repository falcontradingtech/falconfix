// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated roundtrip tests. Do not edit by hand.
// FIX version: FIX4.2

#include <gtest/gtest.h>

#include <string_view>
#include <type_traits>
#include <utility>

#include <FIX4.2/utils/all_messages.h>
#include <FIX4.2/samples/fix_messages.h>

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
	const std::string_view fixraw = fix42::samples::map_samples[sample_name];
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

TEST(RoundTrip_FIX4_2, Heartbeat_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Heartbeat>("Heartbeat");
}

TEST(RoundTrip_FIX4_2, TestRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::TestRequest>("TestRequest");
}

TEST(RoundTrip_FIX4_2, ResendRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ResendRequest>("ResendRequest");
}

TEST(RoundTrip_FIX4_2, Reject_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Reject>("Reject");
}

TEST(RoundTrip_FIX4_2, SequenceReset_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::SequenceReset>("SequenceReset");
}

TEST(RoundTrip_FIX4_2, Logout_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Logout>("Logout");
}

TEST(RoundTrip_FIX4_2, IOI_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::IOI>("IOI");
}

TEST(RoundTrip_FIX4_2, Advertisement_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Advertisement>("Advertisement");
}

TEST(RoundTrip_FIX4_2, ExecutionReport_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ExecutionReport>("ExecutionReport");
}

TEST(RoundTrip_FIX4_2, OrderCancelReject_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::OrderCancelReject>("OrderCancelReject");
}

TEST(RoundTrip_FIX4_2, Logon_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Logon>("Logon");
}

TEST(RoundTrip_FIX4_2, News_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::News>("News");
}

TEST(RoundTrip_FIX4_2, Email_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Email>("Email");
}

TEST(RoundTrip_FIX4_2, NewOrderSingle_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::NewOrderSingle>("NewOrderSingle");
}

TEST(RoundTrip_FIX4_2, NewOrderList_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::NewOrderList>("NewOrderList");
}

TEST(RoundTrip_FIX4_2, OrderCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::OrderCancelRequest>("OrderCancelRequest");
}

TEST(RoundTrip_FIX4_2, OrderCancelReplaceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::OrderCancelReplaceRequest>("OrderCancelReplaceRequest");
}

TEST(RoundTrip_FIX4_2, OrderStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::OrderStatusRequest>("OrderStatusRequest");
}

TEST(RoundTrip_FIX4_2, Allocation_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Allocation>("Allocation");
}

TEST(RoundTrip_FIX4_2, ListCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ListCancelRequest>("ListCancelRequest");
}

TEST(RoundTrip_FIX4_2, ListExecute_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ListExecute>("ListExecute");
}

TEST(RoundTrip_FIX4_2, ListStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ListStatusRequest>("ListStatusRequest");
}

TEST(RoundTrip_FIX4_2, ListStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ListStatus>("ListStatus");
}

TEST(RoundTrip_FIX4_2, AllocationInstructionAck_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::AllocationInstructionAck>("AllocationInstructionAck");
}

TEST(RoundTrip_FIX4_2, DontKnowTrade_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::DontKnowTrade>("DontKnowTrade");
}

TEST(RoundTrip_FIX4_2, QuoteRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::QuoteRequest>("QuoteRequest");
}

TEST(RoundTrip_FIX4_2, Quote_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::Quote>("Quote");
}

TEST(RoundTrip_FIX4_2, SettlementInstructions_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::SettlementInstructions>("SettlementInstructions");
}

TEST(RoundTrip_FIX4_2, MarketDataRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::MarketDataRequest>("MarketDataRequest");
}

TEST(RoundTrip_FIX4_2, MarketDataSnapshotFullRefresh_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::MarketDataSnapshotFullRefresh>("MarketDataSnapshotFullRefresh");
}

TEST(RoundTrip_FIX4_2, MarketDataIncrementalRefresh_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::MarketDataIncrementalRefresh>("MarketDataIncrementalRefresh");
}

TEST(RoundTrip_FIX4_2, MarketDataRequestReject_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::MarketDataRequestReject>("MarketDataRequestReject");
}

TEST(RoundTrip_FIX4_2, QuoteCancel_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::QuoteCancel>("QuoteCancel");
}

TEST(RoundTrip_FIX4_2, QuoteStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::QuoteStatusRequest>("QuoteStatusRequest");
}

TEST(RoundTrip_FIX4_2, QuoteAcknowledgement_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::QuoteAcknowledgement>("QuoteAcknowledgement");
}

TEST(RoundTrip_FIX4_2, SecurityDefinitionRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::SecurityDefinitionRequest>("SecurityDefinitionRequest");
}

TEST(RoundTrip_FIX4_2, SecurityDefinition_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::SecurityDefinition>("SecurityDefinition");
}

TEST(RoundTrip_FIX4_2, SecurityStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::SecurityStatusRequest>("SecurityStatusRequest");
}

TEST(RoundTrip_FIX4_2, SecurityStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::SecurityStatus>("SecurityStatus");
}

TEST(RoundTrip_FIX4_2, TradingSessionStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::TradingSessionStatusRequest>("TradingSessionStatusRequest");
}

TEST(RoundTrip_FIX4_2, TradingSessionStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::TradingSessionStatus>("TradingSessionStatus");
}

TEST(RoundTrip_FIX4_2, MassQuote_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::MassQuote>("MassQuote");
}

TEST(RoundTrip_FIX4_2, BusinessMessageReject_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::BusinessMessageReject>("BusinessMessageReject");
}

TEST(RoundTrip_FIX4_2, BidRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::BidRequest>("BidRequest");
}

TEST(RoundTrip_FIX4_2, BidResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::BidResponse>("BidResponse");
}

TEST(RoundTrip_FIX4_2, ListStrikePrice_MatchesGeneratedSample) {
	runRoundTripFor<fix42::messages::ListStrikePrice>("ListStrikePrice");
}

