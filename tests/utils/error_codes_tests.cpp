// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <algorithm>
#include <utils/error_codes.h>

namespace {

using namespace falconfix::errors;

class ErrorCodesTests : public ::testing::Test {
protected:
	// Test fixtures can be added here if needed
};

// ============================================================================
// SUCCESS CODE TESTS
// ============================================================================

TEST_F(ErrorCodesTests, SuccessCodeIsZero) {
	EXPECT_EQ(code::SUCCESS, 0);
}

TEST_F(ErrorCodesTests, SuccessStatusIsOk) {
	auto status = success();
	EXPECT_TRUE(status.ok());
	EXPECT_TRUE(static_cast<bool>(status));
}

TEST_F(ErrorCodesTests, SuccessCodeStringRepresentation) {
	EXPECT_EQ(code_name(code::SUCCESS), "success");
}

// ============================================================================
// SYSTEM ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, SystemErrorCodesAreNegative) {
	EXPECT_LT(code::system::UNKNOWN, 0);
	EXPECT_LT(code::system::NOT_IMPLEMENTED, 0);
	EXPECT_LT(code::system::BAD_ALLOC, 0);
	EXPECT_LT(code::system::THREAD_FAILURE, 0);
	EXPECT_LT(code::system::RESOURCE_BUSY, 0);
	EXPECT_LT(code::system::OUT_OF_MEMORY, 0);
	EXPECT_LT(code::system::PERMISSION_DENIED, 0);
	EXPECT_LT(code::system::INTERRUPTED, 0);
}

TEST_F(ErrorCodesTests, SystemErrorCodesAreUnique) {
	std::vector<Code> codes = {
		code::system::UNKNOWN,
		code::system::NOT_IMPLEMENTED,
		code::system::BAD_ALLOC,
		code::system::THREAD_FAILURE,
		code::system::RESOURCE_BUSY,
		code::system::OUT_OF_MEMORY,
		code::system::PERMISSION_DENIED,
		code::system::INTERRUPTED
	};
	std::sort(codes.begin(), codes.end());
	EXPECT_EQ(codes.size(), 8);
	EXPECT_EQ(std::unique(codes.begin(), codes.end()), codes.end());
}

TEST_F(ErrorCodesTests, SystemErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::system::UNKNOWN), "system::unknown");
	EXPECT_EQ(code_name(code::system::NOT_IMPLEMENTED), "system::not_implemented");
	EXPECT_EQ(code_name(code::system::BAD_ALLOC), "system::bad_alloc");
	EXPECT_EQ(code_name(code::system::THREAD_FAILURE), "system::thread_failure");
	EXPECT_EQ(code_name(code::system::RESOURCE_BUSY), "system::resource_busy");
	EXPECT_EQ(code_name(code::system::OUT_OF_MEMORY), "system::out_of_memory");
	EXPECT_EQ(code_name(code::system::PERMISSION_DENIED), "system::permission_denied");
	EXPECT_EQ(code_name(code::system::INTERRUPTED), "system::interrupted");
}

TEST_F(ErrorCodesTests, SystemStatusFactory) {
	auto status = system(code::system::BAD_ALLOC);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::system::BAD_ALLOC);
	EXPECT_EQ(status.domain, Domain::system);
}

// ============================================================================
// CONFIG ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, ConfigErrorCodesAreInRange) {
	EXPECT_EQ(code::config::MISSING, -1000);
	EXPECT_EQ(code::config::INVALID, -1001);
	EXPECT_EQ(code::config::XML_PARSE_FAILED, -1002);
}

TEST_F(ErrorCodesTests, ConfigErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::config::MISSING), "config::missing");
	EXPECT_EQ(code_name(code::config::INVALID), "config::invalid");
	EXPECT_EQ(code_name(code::config::XML_PARSE_FAILED), "config::xml_parse_failed");
}

TEST_F(ErrorCodesTests, ConfigStatusFactory) {
	auto status = config(code::config::INVALID);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::config::INVALID);
	EXPECT_EQ(status.domain, Domain::config);
}

// ============================================================================
// SOCKET ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, SocketErrorCodesAreInRange) {
	// All socket error codes should be in the -2000 to -2999 range
	EXPECT_GE(code::socket::ACCEPT_FAILED, -2999);
	EXPECT_LE(code::socket::ACCEPT_FAILED, -2000);
	EXPECT_GE(code::socket::UNKNOWN, -2999);
	EXPECT_LE(code::socket::UNKNOWN, -2000);
}

TEST_F(ErrorCodesTests, SocketErrorCodesAreUnique) {
	std::vector<Code> codes = {
		code::socket::ACCEPT_FAILED,
		code::socket::CONNECT_FAILED,
		code::socket::BIND_FAILED,
		code::socket::LISTEN_FAILED,
		code::socket::READ_FAILED,
		code::socket::WRITE_FAILED,
		code::socket::CLOSE_FAILED,
		code::socket::RESOLVE_FAILED,
		code::socket::INVALID_INTERFACE,
		code::socket::INTERFACE_BIND,
		code::socket::DISCONNECTED,
		code::socket::CLOSED,
		code::socket::NOT_OPEN,
		code::socket::ALREADY_CONNECTED,
		code::socket::NOT_CONNECTED,
		code::socket::UNAVAILABLE,
		code::socket::RECV_TIMEOUT,
		code::socket::SEND_TIMEOUT,
		code::socket::OPERATION_TIMEOUT,
		code::socket::BUFFER_OVERFLOW,
		code::socket::BUFFER_UNDERFLOW,
		code::socket::INVALID_FRAME,
		code::socket::END_OF_FILE,
		code::socket::UNKNOWN
	};
	std::sort(codes.begin(), codes.end());
	EXPECT_EQ(std::unique(codes.begin(), codes.end()), codes.end());
}

TEST_F(ErrorCodesTests, SocketStatusFactory) {
	auto status = socket(code::socket::CONNECT_FAILED);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::socket::CONNECT_FAILED);
	EXPECT_EQ(status.domain, Domain::socket);
}

TEST_F(ErrorCodesTests, SocketErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::socket::ACCEPT_FAILED), "socket::accept_failed");
	EXPECT_EQ(code_name(code::socket::CONNECT_FAILED), "socket::connect_failed");
	EXPECT_EQ(code_name(code::socket::RECV_TIMEOUT), "socket::recv_timeout");
	EXPECT_EQ(code_name(code::socket::SEND_TIMEOUT), "socket::send_timeout");
}

// ============================================================================
// TLS ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, TlsErrorCodesAreInRange) {
	EXPECT_EQ(code::tls::FAILURE, -3000);
	EXPECT_EQ(code::tls::HANDSHAKE_FAILED, -3001);
	EXPECT_EQ(code::tls::CERTIFICATE_ERROR, -3002);
	EXPECT_EQ(code::tls::OPERATION_NOT_SUPPORTED, -3003);
}

TEST_F(ErrorCodesTests, TlsStatusFactory) {
	auto status = tls(code::tls::HANDSHAKE_FAILED);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::tls::HANDSHAKE_FAILED);
	EXPECT_EQ(status.domain, Domain::tls);
}

TEST_F(ErrorCodesTests, TlsErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::tls::FAILURE), "tls::failure");
	EXPECT_EQ(code_name(code::tls::HANDSHAKE_FAILED), "tls::handshake_failed");
	EXPECT_EQ(code_name(code::tls::CERTIFICATE_ERROR), "tls::certificate_error");
	EXPECT_EQ(code_name(code::tls::OPERATION_NOT_SUPPORTED), "tls::operation_not_supported");
}

// ============================================================================
// FIX PARSE ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, FixParseErrorCodesAreInRange) {
	EXPECT_GE(code::fix::parse::UNKNOWN_FIELD_ID, -4999);
	EXPECT_LE(code::fix::parse::UNKNOWN_FIELD_ID, -4000);
}

TEST_F(ErrorCodesTests, FixParseStatusFactory) {
	auto status = fix_parse(code::fix::parse::UNKNOWN_FIELD_ID, 100);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::fix::parse::UNKNOWN_FIELD_ID);
	EXPECT_EQ(status.tag, 100);
	EXPECT_EQ(status.domain, Domain::fix_parse);
}

TEST_F(ErrorCodesTests, FixParseErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::fix::parse::UNKNOWN_FIELD_ID), "fix::parse::unknown_field_id");
	EXPECT_EQ(code_name(code::fix::parse::INVALID_FIELD_VALUE), "fix::parse::invalid_field_value");
	EXPECT_EQ(code_name(code::fix::parse::DUPLICATE_TAG), "fix::parse::duplicate_tag");
	EXPECT_EQ(code_name(code::fix::parse::MALFORMED_TAG), "fix::parse::malformed_tag");
}

// ============================================================================
// FIX VALIDATION ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, FixValidationStatusFactory) {
	auto status = fix_validation(code::fix::validation::MISSING_REQUIRED, 108);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::fix::validation::MISSING_REQUIRED);
	EXPECT_EQ(status.tag, 108);
	EXPECT_EQ(status.domain, Domain::fix_validation);
}

TEST_F(ErrorCodesTests, FixValidationErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::fix::validation::MISSING_REQUIRED), "fix::validation::missing_required");
	EXPECT_EQ(code_name(code::fix::validation::INVALID_FIELD_VALUE), "fix::validation::invalid_field_value");
}

// ============================================================================
// FIX SESSION ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, FixSessionStatusFactory) {
	auto status = fix_session(code::fix::session::LOGON_FAILED, 1);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::fix::session::LOGON_FAILED);
	EXPECT_EQ(status.reason, 1);
	EXPECT_EQ(status.domain, Domain::fix_session);
}

TEST_F(ErrorCodesTests, FixSessionErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::fix::session::UNSUPPORTED_PROTOCOL), "fix::session::unsupported_protocol");
	EXPECT_EQ(code_name(code::fix::session::LOGON_FAILED), "fix::session::logon_failed");
	EXPECT_EQ(code_name(code::fix::session::HEARTBEAT_TIMEOUT), "fix::session::heartbeat_timeout");
}

// ============================================================================
// APPLICATION ERROR CODES
// ============================================================================

TEST_F(ErrorCodesTests, ApplicationStatusFactory) {
	auto status = application(code::application::REJECTED, 100);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::application::REJECTED);
	EXPECT_EQ(status.reason, 100);
	EXPECT_EQ(status.domain, Domain::application);
}

TEST_F(ErrorCodesTests, ApplicationErrorCodeStringRepresentations) {
	EXPECT_EQ(code_name(code::application::REJECTED), "application::rejected");
	EXPECT_EQ(code_name(code::application::CALLBACK_FAILED), "application::callback_failed");
	EXPECT_EQ(code_name(code::application::ENGINE_ALREADY_RUNNING), "application::engine_already_running");
}

// ============================================================================
// DOMAIN TESTS
// ============================================================================

TEST_F(ErrorCodesTests, AllDomainsAreDefined) {
	EXPECT_EQ(static_cast<int>(Domain::none), 0);
	EXPECT_EQ(static_cast<int>(Domain::system), 1);
	EXPECT_EQ(static_cast<int>(Domain::config), 2);
	EXPECT_EQ(static_cast<int>(Domain::socket), 3);
	EXPECT_EQ(static_cast<int>(Domain::tls), 4);
	EXPECT_EQ(static_cast<int>(Domain::fix_parse), 5);
	EXPECT_EQ(static_cast<int>(Domain::fix_validation), 6);
	EXPECT_EQ(static_cast<int>(Domain::fix_session), 7);
	EXPECT_EQ(static_cast<int>(Domain::application), 8);
}

TEST_F(ErrorCodesTests, DomainNameReturnsCorrectStrings) {
	EXPECT_EQ(domain_name(Domain::none), "none");
	EXPECT_EQ(domain_name(Domain::system), "system");
	EXPECT_EQ(domain_name(Domain::config), "config");
	EXPECT_EQ(domain_name(Domain::socket), "socket");
	EXPECT_EQ(domain_name(Domain::tls), "tls");
	EXPECT_EQ(domain_name(Domain::fix_parse), "fix_parse");
	EXPECT_EQ(domain_name(Domain::fix_validation), "fix_validation");
	EXPECT_EQ(domain_name(Domain::fix_session), "fix_session");
	EXPECT_EQ(domain_name(Domain::application), "application");
}

// ============================================================================
// STATUS STRUCT TESTS
// ============================================================================

TEST_F(ErrorCodesTests, StatusDefaultConstructorIsSuccess) {
	Status status;
	EXPECT_TRUE(status.ok());
	EXPECT_EQ(status.code, 0);
	EXPECT_EQ(status.tag, 0);
	EXPECT_EQ(status.reason, 0);
	EXPECT_EQ(status.domain, Domain::none);
}

TEST_F(ErrorCodesTests, StatusCanBeUsedInBooleanContext) {
	Status successStatus = success();
	Status failStatus = system(code::system::UNKNOWN);

	if (successStatus) {
		// Success path
		EXPECT_TRUE(true);
	} else {
		FAIL() << "Success status should be truthy";
	}

	if (failStatus) {
		FAIL() << "Fail status should be falsy";
	} else {
		// Failure path
		EXPECT_TRUE(true);
	}
}

TEST_F(ErrorCodesTests, FailFactoryCreatesProperStatus) {
	auto status = fail(Domain::socket, code::socket::CONNECT_FAILED, 50, 1000);
	EXPECT_FALSE(status.ok());
	EXPECT_EQ(status.code, code::socket::CONNECT_FAILED);
	EXPECT_EQ(status.tag, 50);
	EXPECT_EQ(status.reason, 1000);
	EXPECT_EQ(status.domain, Domain::socket);
}

// ============================================================================
// EDGE CASES AND PROPERTY TESTS
// ============================================================================

TEST_F(ErrorCodesTests, SuccessStatusNeverHasErrorCode) {
	auto status = success();
	EXPECT_EQ(status.code, 0);
	EXPECT_EQ(status.domain, Domain::none);
	EXPECT_TRUE(status.ok());
}

TEST_F(ErrorCodesTests, ErrorStatusAlwaysHasNonZeroCode) {
	auto s1 = system(code::system::UNKNOWN);
	auto s2 = socket(code::socket::CONNECT_FAILED);
	auto s3 = tls(code::tls::FAILURE);

	EXPECT_NE(s1.code, 0);
	EXPECT_NE(s2.code, 0);
	EXPECT_NE(s3.code, 0);
}

TEST_F(ErrorCodesTests, CodeNamesHandleUnknownCodes) {
	// Unknown code should return something
	auto name = code_name(-999999);
	EXPECT_FALSE(name.empty() || name == "unknown");
}

TEST_F(ErrorCodesTests, DomainNameHandlesInvalidDomain) {
	auto name = domain_name(static_cast<Domain>(255));
	EXPECT_EQ(name, "unknown");
}

} // anonymous namespace
