// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
// Auto-generated roundtrip tests. Do not edit by hand.
// FIX version: FIX4.4

#include <gtest/gtest.h>

#include <string_view>
#include <type_traits>
#include <utility>

#include <FIX4.4/utils/all_messages.h>
#include <FIX4.4/samples/fix_messages.h>

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
	const std::string_view fixraw = fix44::samples::map_samples[sample_name];
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

TEST(RoundTrip_FIX4_4, Heartbeat_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Heartbeat>("Heartbeat");
}

TEST(RoundTrip_FIX4_4, TestRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TestRequest>("TestRequest");
}

TEST(RoundTrip_FIX4_4, ResendRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ResendRequest>("ResendRequest");
}

TEST(RoundTrip_FIX4_4, Reject_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Reject>("Reject");
}

TEST(RoundTrip_FIX4_4, SequenceReset_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SequenceReset>("SequenceReset");
}

TEST(RoundTrip_FIX4_4, Logout_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Logout>("Logout");
}

TEST(RoundTrip_FIX4_4, IOI_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::IOI>("IOI");
}

TEST(RoundTrip_FIX4_4, Advertisement_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Advertisement>("Advertisement");
}

TEST(RoundTrip_FIX4_4, ExecutionReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ExecutionReport>("ExecutionReport");
}

TEST(RoundTrip_FIX4_4, OrderCancelReject_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderCancelReject>("OrderCancelReject");
}

TEST(RoundTrip_FIX4_4, Logon_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Logon>("Logon");
}

TEST(RoundTrip_FIX4_4, News_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::News>("News");
}

TEST(RoundTrip_FIX4_4, Email_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Email>("Email");
}

TEST(RoundTrip_FIX4_4, NewOrderSingle_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::NewOrderSingle>("NewOrderSingle");
}

TEST(RoundTrip_FIX4_4, NewOrderList_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::NewOrderList>("NewOrderList");
}

TEST(RoundTrip_FIX4_4, OrderCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderCancelRequest>("OrderCancelRequest");
}

TEST(RoundTrip_FIX4_4, OrderCancelReplaceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderCancelReplaceRequest>("OrderCancelReplaceRequest");
}

TEST(RoundTrip_FIX4_4, OrderStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderStatusRequest>("OrderStatusRequest");
}

TEST(RoundTrip_FIX4_4, AllocationInstruction_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::AllocationInstruction>("AllocationInstruction");
}

TEST(RoundTrip_FIX4_4, ListCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ListCancelRequest>("ListCancelRequest");
}

TEST(RoundTrip_FIX4_4, ListExecute_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ListExecute>("ListExecute");
}

TEST(RoundTrip_FIX4_4, ListStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ListStatusRequest>("ListStatusRequest");
}

TEST(RoundTrip_FIX4_4, ListStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ListStatus>("ListStatus");
}

TEST(RoundTrip_FIX4_4, AllocationInstructionAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::AllocationInstructionAck>("AllocationInstructionAck");
}

TEST(RoundTrip_FIX4_4, DontKnowTrade_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::DontKnowTrade>("DontKnowTrade");
}

TEST(RoundTrip_FIX4_4, QuoteRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::QuoteRequest>("QuoteRequest");
}

TEST(RoundTrip_FIX4_4, Quote_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Quote>("Quote");
}

TEST(RoundTrip_FIX4_4, SettlementInstructions_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SettlementInstructions>("SettlementInstructions");
}

TEST(RoundTrip_FIX4_4, MarketDataRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MarketDataRequest>("MarketDataRequest");
}

TEST(RoundTrip_FIX4_4, MarketDataSnapshotFullRefresh_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MarketDataSnapshotFullRefresh>("MarketDataSnapshotFullRefresh");
}

TEST(RoundTrip_FIX4_4, MarketDataIncrementalRefresh_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MarketDataIncrementalRefresh>("MarketDataIncrementalRefresh");
}

TEST(RoundTrip_FIX4_4, MarketDataRequestReject_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MarketDataRequestReject>("MarketDataRequestReject");
}

TEST(RoundTrip_FIX4_4, QuoteCancel_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::QuoteCancel>("QuoteCancel");
}

TEST(RoundTrip_FIX4_4, QuoteStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::QuoteStatusRequest>("QuoteStatusRequest");
}

TEST(RoundTrip_FIX4_4, MassQuoteAcknowledgement_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MassQuoteAcknowledgement>("MassQuoteAcknowledgement");
}

TEST(RoundTrip_FIX4_4, SecurityDefinitionRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityDefinitionRequest>("SecurityDefinitionRequest");
}

TEST(RoundTrip_FIX4_4, SecurityDefinition_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityDefinition>("SecurityDefinition");
}

TEST(RoundTrip_FIX4_4, SecurityStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityStatusRequest>("SecurityStatusRequest");
}

TEST(RoundTrip_FIX4_4, SecurityStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityStatus>("SecurityStatus");
}

TEST(RoundTrip_FIX4_4, TradingSessionStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TradingSessionStatusRequest>("TradingSessionStatusRequest");
}

TEST(RoundTrip_FIX4_4, TradingSessionStatus_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TradingSessionStatus>("TradingSessionStatus");
}

TEST(RoundTrip_FIX4_4, MassQuote_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MassQuote>("MassQuote");
}

TEST(RoundTrip_FIX4_4, BusinessMessageReject_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::BusinessMessageReject>("BusinessMessageReject");
}

TEST(RoundTrip_FIX4_4, BidRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::BidRequest>("BidRequest");
}

TEST(RoundTrip_FIX4_4, BidResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::BidResponse>("BidResponse");
}

TEST(RoundTrip_FIX4_4, ListStrikePrice_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ListStrikePrice>("ListStrikePrice");
}

TEST(RoundTrip_FIX4_4, XMLnonFIX_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::XMLnonFIX>("XMLnonFIX");
}

TEST(RoundTrip_FIX4_4, RegistrationInstructions_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::RegistrationInstructions>("RegistrationInstructions");
}

TEST(RoundTrip_FIX4_4, RegistrationInstructionsResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::RegistrationInstructionsResponse>("RegistrationInstructionsResponse");
}

TEST(RoundTrip_FIX4_4, OrderMassCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderMassCancelRequest>("OrderMassCancelRequest");
}

TEST(RoundTrip_FIX4_4, OrderMassCancelReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderMassCancelReport>("OrderMassCancelReport");
}

TEST(RoundTrip_FIX4_4, NewOrderCross_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::NewOrderCross>("NewOrderCross");
}

TEST(RoundTrip_FIX4_4, CrossOrderCancelReplaceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CrossOrderCancelReplaceRequest>("CrossOrderCancelReplaceRequest");
}

TEST(RoundTrip_FIX4_4, CrossOrderCancelRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CrossOrderCancelRequest>("CrossOrderCancelRequest");
}

TEST(RoundTrip_FIX4_4, SecurityTypeRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityTypeRequest>("SecurityTypeRequest");
}

TEST(RoundTrip_FIX4_4, SecurityTypes_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityTypes>("SecurityTypes");
}

TEST(RoundTrip_FIX4_4, SecurityListRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityListRequest>("SecurityListRequest");
}

TEST(RoundTrip_FIX4_4, SecurityList_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SecurityList>("SecurityList");
}

TEST(RoundTrip_FIX4_4, DerivativeSecurityListRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::DerivativeSecurityListRequest>("DerivativeSecurityListRequest");
}

TEST(RoundTrip_FIX4_4, DerivativeSecurityList_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::DerivativeSecurityList>("DerivativeSecurityList");
}

TEST(RoundTrip_FIX4_4, NewOrderMultileg_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::NewOrderMultileg>("NewOrderMultileg");
}

TEST(RoundTrip_FIX4_4, MultilegOrderCancelReplace_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::MultilegOrderCancelReplace>("MultilegOrderCancelReplace");
}

TEST(RoundTrip_FIX4_4, TradeCaptureReportRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TradeCaptureReportRequest>("TradeCaptureReportRequest");
}

TEST(RoundTrip_FIX4_4, TradeCaptureReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TradeCaptureReport>("TradeCaptureReport");
}

TEST(RoundTrip_FIX4_4, OrderMassStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::OrderMassStatusRequest>("OrderMassStatusRequest");
}

TEST(RoundTrip_FIX4_4, QuoteRequestReject_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::QuoteRequestReject>("QuoteRequestReject");
}

TEST(RoundTrip_FIX4_4, RFQRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::RFQRequest>("RFQRequest");
}

TEST(RoundTrip_FIX4_4, QuoteStatusReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::QuoteStatusReport>("QuoteStatusReport");
}

TEST(RoundTrip_FIX4_4, QuoteResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::QuoteResponse>("QuoteResponse");
}

TEST(RoundTrip_FIX4_4, Confirmation_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::Confirmation>("Confirmation");
}

TEST(RoundTrip_FIX4_4, PositionMaintenanceRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::PositionMaintenanceRequest>("PositionMaintenanceRequest");
}

TEST(RoundTrip_FIX4_4, PositionMaintenanceReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::PositionMaintenanceReport>("PositionMaintenanceReport");
}

TEST(RoundTrip_FIX4_4, RequestForPositions_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::RequestForPositions>("RequestForPositions");
}

TEST(RoundTrip_FIX4_4, RequestForPositionsAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::RequestForPositionsAck>("RequestForPositionsAck");
}

TEST(RoundTrip_FIX4_4, PositionReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::PositionReport>("PositionReport");
}

TEST(RoundTrip_FIX4_4, TradeCaptureReportRequestAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TradeCaptureReportRequestAck>("TradeCaptureReportRequestAck");
}

TEST(RoundTrip_FIX4_4, TradeCaptureReportAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::TradeCaptureReportAck>("TradeCaptureReportAck");
}

TEST(RoundTrip_FIX4_4, AllocationReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::AllocationReport>("AllocationReport");
}

TEST(RoundTrip_FIX4_4, AllocationReportAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::AllocationReportAck>("AllocationReportAck");
}

TEST(RoundTrip_FIX4_4, ConfirmationAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ConfirmationAck>("ConfirmationAck");
}

TEST(RoundTrip_FIX4_4, SettlementInstructionRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::SettlementInstructionRequest>("SettlementInstructionRequest");
}

TEST(RoundTrip_FIX4_4, AssignmentReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::AssignmentReport>("AssignmentReport");
}

TEST(RoundTrip_FIX4_4, CollateralRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CollateralRequest>("CollateralRequest");
}

TEST(RoundTrip_FIX4_4, CollateralAssignment_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CollateralAssignment>("CollateralAssignment");
}

TEST(RoundTrip_FIX4_4, CollateralResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CollateralResponse>("CollateralResponse");
}

TEST(RoundTrip_FIX4_4, CollateralReport_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CollateralReport>("CollateralReport");
}

TEST(RoundTrip_FIX4_4, CollateralInquiry_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CollateralInquiry>("CollateralInquiry");
}

TEST(RoundTrip_FIX4_4, NetworkCounterpartySystemStatusRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::NetworkCounterpartySystemStatusRequest>("NetworkCounterpartySystemStatusRequest");
}

TEST(RoundTrip_FIX4_4, NetworkCounterpartySystemStatusResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::NetworkCounterpartySystemStatusResponse>("NetworkCounterpartySystemStatusResponse");
}

TEST(RoundTrip_FIX4_4, UserRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::UserRequest>("UserRequest");
}

TEST(RoundTrip_FIX4_4, UserResponse_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::UserResponse>("UserResponse");
}

TEST(RoundTrip_FIX4_4, CollateralInquiryAck_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::CollateralInquiryAck>("CollateralInquiryAck");
}

TEST(RoundTrip_FIX4_4, ConfirmationRequest_MatchesGeneratedSample) {
	runRoundTripFor<fix44::messages::ConfirmationRequest>("ConfirmationRequest");
}

