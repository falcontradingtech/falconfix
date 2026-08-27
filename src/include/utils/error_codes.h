// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#include <utils/string_utils.h>

namespace falconfix::errors {

using Code = int32_t;
using ErrorCode = Code;

enum class Domain : uint8_t {
    none = 0,
    system,
    config,
    socket,
    tls,
    fix_parse,
    fix_validation,
    fix_session,
    application
};

struct Status {
    Code code{0};
    int32_t tag{0};
    int32_t reason{0};
    Domain domain{Domain::none};

    [[nodiscard]] constexpr bool ok() const noexcept { return code == 0; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return ok(); }
};

[[nodiscard]] inline constexpr Status success() noexcept { return {}; }

[[nodiscard]] inline constexpr Status fail(Domain domain, Code code, int32_t tag = 0, int32_t reason = 0) noexcept {
    return Status{code, tag, reason, domain};
}

namespace code {

static constexpr Code SUCCESS = 0;

namespace system {
    static constexpr Code UNKNOWN           = -1;
    static constexpr Code NOT_IMPLEMENTED   = -2;
    static constexpr Code BAD_ALLOC         = -3;
    static constexpr Code THREAD_FAILURE    = -4;
    static constexpr Code RESOURCE_BUSY     = -5;
    static constexpr Code OUT_OF_MEMORY     = -6;
    static constexpr Code PERMISSION_DENIED = -7;
    static constexpr Code INTERRUPTED       = -8;
}

namespace config {
    static constexpr Code MISSING          = -1000;
    static constexpr Code INVALID          = -1001;
    static constexpr Code XML_PARSE_FAILED = -1002;
}

namespace socket {
    static constexpr Code ACCEPT_FAILED     = -2000;
    static constexpr Code CONNECT_FAILED    = -2001;
    static constexpr Code BIND_FAILED       = -2002;
    static constexpr Code LISTEN_FAILED     = -2003;
    static constexpr Code READ_FAILED       = -2004;
    static constexpr Code WRITE_FAILED      = -2005;
    static constexpr Code CLOSE_FAILED      = -2006;
    static constexpr Code RESOLVE_FAILED    = -2007;
    static constexpr Code INVALID_INTERFACE = -2008;
    static constexpr Code INTERFACE_BIND    = -2009;
    static constexpr Code DISCONNECTED      = -2010;
    static constexpr Code CLOSED            = -2011;
    static constexpr Code NOT_OPEN          = -2012;
    static constexpr Code ALREADY_CONNECTED = -2013;
    static constexpr Code NOT_CONNECTED     = -2014;
    static constexpr Code UNAVAILABLE       = -2015;
    static constexpr Code RECV_TIMEOUT      = -2020;
    static constexpr Code SEND_TIMEOUT      = -2021;
    static constexpr Code OPERATION_TIMEOUT = -2022;
    static constexpr Code BUFFER_OVERFLOW   = -2080;
    static constexpr Code BUFFER_UNDERFLOW  = -2081;
    static constexpr Code INVALID_FRAME     = -2082;
    static constexpr Code END_OF_FILE       = -2083;
    static constexpr Code UNKNOWN           = -2999;
}

namespace tls {
    static constexpr Code FAILURE                 = -3000;
    static constexpr Code HANDSHAKE_FAILED        = -3001;
    static constexpr Code CERTIFICATE_ERROR       = -3002;
    static constexpr Code OPERATION_NOT_SUPPORTED = -3003;
}

namespace fix {

namespace parse {
    static constexpr Code UNKNOWN_FIELD_ID    = -4000;
    static constexpr Code INVALID_FIELD_VALUE = -4001;
    static constexpr Code UNEXPECTED_FIELD    = -4002;
    static constexpr Code OUT_OF_RANGE        = -4003;
    static constexpr Code DUPLICATE_TAG       = -4004;
    static constexpr Code MALFORMED_TAG       = -4005;
    static constexpr Code MISSING_EQUALS      = -4006;
    static constexpr Code MISSING_SOH         = -4007;
    static constexpr Code DECODE_FAILED       = -4008;
    static constexpr Code INVALID_CHECKSUM    = -4009;
    static constexpr Code INVALID_LENGTH      = -4010;
    static constexpr Code INVALID_MESSAGE_TYPE = -4011;
}

namespace validation {
    static constexpr Code MISSING_REQUIRED            = -4200;
    static constexpr Code INVALID_FIELD_VALUE         = -4201;
    static constexpr Code REQUIRED_GROUP_EMPTY        = -4202;
    static constexpr Code GROUP_COUNT_MISMATCH        = -4203;
    static constexpr Code COMPONENT_VALIDATION_FAILED = -4204;
    static constexpr Code ENCODE_FAILED               = -4205;
}

namespace session {
    static constexpr Code UNSUPPORTED_PROTOCOL      = -5000;
    static constexpr Code SESSION_CREATE_FAILED     = -5001;
    static constexpr Code SESSION_NOT_FOUND         = -5002;
    static constexpr Code SEQUENCE_GAP              = -5003;
    static constexpr Code SESSION_REJECT            = -5004;
    static constexpr Code LOGON_FAILED              = -5005;
    static constexpr Code NOT_LOGGED_ON             = -5006;
    static constexpr Code LOGOUT_FAILED             = -5007;
    static constexpr Code HEARTBEAT_TIMEOUT         = -5008;
    static constexpr Code TEST_REQUEST_TIMEOUT      = -5009;
    static constexpr Code RESEND_REQUIRED           = -5010;
    static constexpr Code RESEND_FAILED             = -5011;
    static constexpr Code SEQUENCE_RESET_FAILED     = -5012;
    static constexpr Code REJECTED_BY_APPLICATION   = -5013;
    static constexpr Code INVALID_SEQUENCE_RESET = -5014;
}

} // namespace fix

namespace application {
    static constexpr Code REJECTED                  = -6000;
    static constexpr Code CALLBACK_FAILED           = -6001;
    static constexpr Code ENGINE_ALREADY_RUNNING    = -6002;
}

} // namespace code

[[nodiscard]] inline constexpr Status system(Code c) noexcept { return fail(Domain::system, c); }
[[nodiscard]] inline constexpr Status socket(Code c) noexcept { return fail(Domain::socket, c); }
[[nodiscard]] inline constexpr Status tls(Code c) noexcept { return fail(Domain::tls, c); }
[[nodiscard]] inline constexpr Status config(Code c) noexcept { return fail(Domain::config, c); }

[[nodiscard]] inline constexpr Status fix_parse(Code c, int32_t tag = 0) noexcept {
    return fail(Domain::fix_parse, c, tag);
}

[[nodiscard]] inline constexpr Status fix_validation(Code c, int32_t tag = 0) noexcept {
    return fail(Domain::fix_validation, c, tag);
}

[[nodiscard]] inline constexpr Status fix_session(Code c, int32_t reason = 0) noexcept {
    return fail(Domain::fix_session, c, 0, reason);
}

[[nodiscard]] inline constexpr Status application(Code c, int32_t reason = 0) noexcept {
    return fail(Domain::application, c, 0, reason);
}

[[nodiscard]] inline constexpr std::string_view domain_name(Domain d) noexcept {
    switch (d) {
    case Domain::none: return "none";
    case Domain::system: return "system";
    case Domain::config: return "config";
    case Domain::socket: return "socket";
    case Domain::tls: return "tls";
    case Domain::fix_parse: return "fix_parse";
    case Domain::fix_validation: return "fix_validation";
    case Domain::fix_session: return "fix_session";
    case Domain::application: return "application";
    default: return "unknown";
    }
}

[[nodiscard]] inline constexpr std::string_view code_name(Code c) noexcept {
    switch (c) {
    case code::SUCCESS: return "success";

    case code::system::UNKNOWN: return "system::unknown";
    case code::system::NOT_IMPLEMENTED: return "system::not_implemented";
    case code::system::BAD_ALLOC: return "system::bad_alloc";
    case code::system::THREAD_FAILURE: return "system::thread_failure";
    case code::system::RESOURCE_BUSY: return "system::resource_busy";
    case code::system::OUT_OF_MEMORY: return "system::out_of_memory";
    case code::system::PERMISSION_DENIED: return "system::permission_denied";
    case code::system::INTERRUPTED: return "system::interrupted";

    case code::config::MISSING: return "config::missing";
    case code::config::INVALID: return "config::invalid";
    case code::config::XML_PARSE_FAILED: return "config::xml_parse_failed";

    case code::socket::ACCEPT_FAILED: return "socket::accept_failed";
    case code::socket::CONNECT_FAILED: return "socket::connect_failed";
    case code::socket::BIND_FAILED: return "socket::bind_failed";
    case code::socket::LISTEN_FAILED: return "socket::listen_failed";
    case code::socket::READ_FAILED: return "socket::read_failed";
    case code::socket::WRITE_FAILED: return "socket::write_failed";
    case code::socket::CLOSE_FAILED: return "socket::close_failed";
    case code::socket::RESOLVE_FAILED: return "socket::resolve_failed";
    case code::socket::INVALID_INTERFACE: return "socket::invalid_interface";
    case code::socket::INTERFACE_BIND: return "socket::interface_bind";
    case code::socket::DISCONNECTED: return "socket::disconnected";
    case code::socket::CLOSED: return "socket::closed";
    case code::socket::NOT_OPEN: return "socket::not_open";
    case code::socket::ALREADY_CONNECTED: return "socket::already_connected";
    case code::socket::NOT_CONNECTED: return "socket::not_connected";
    case code::socket::UNAVAILABLE: return "socket::unavailable";
    case code::socket::RECV_TIMEOUT: return "socket::recv_timeout";
    case code::socket::SEND_TIMEOUT: return "socket::send_timeout";
    case code::socket::OPERATION_TIMEOUT: return "socket::operation_timeout";
    case code::socket::BUFFER_OVERFLOW: return "socket::buffer_overflow";
    case code::socket::BUFFER_UNDERFLOW: return "socket::buffer_underflow";
    case code::socket::INVALID_FRAME: return "socket::invalid_frame";
    case code::socket::END_OF_FILE: return "socket::end_of_file";
    case code::socket::UNKNOWN: return "socket::unknown";

    case code::tls::FAILURE: return "tls::failure";
    case code::tls::HANDSHAKE_FAILED: return "tls::handshake_failed";
    case code::tls::CERTIFICATE_ERROR: return "tls::certificate_error";
    case code::tls::OPERATION_NOT_SUPPORTED: return "tls::operation_not_supported";

    case code::fix::parse::UNKNOWN_FIELD_ID: return "fix::parse::unknown_field_id";
    case code::fix::parse::INVALID_FIELD_VALUE: return "fix::parse::invalid_field_value";
    case code::fix::parse::UNEXPECTED_FIELD: return "fix::parse::unexpected_field";
    case code::fix::parse::OUT_OF_RANGE: return "fix::parse::out_of_range";
    case code::fix::parse::DUPLICATE_TAG: return "fix::parse::duplicate_tag";
    case code::fix::parse::MALFORMED_TAG: return "fix::parse::malformed_tag";
    case code::fix::parse::MISSING_EQUALS: return "fix::parse::missing_equals";
    case code::fix::parse::MISSING_SOH: return "fix::parse::missing_soh";
    case code::fix::parse::DECODE_FAILED: return "fix::parse::decode_failed";
    case code::fix::parse::INVALID_CHECKSUM: return "fix::parse::invalid_checksum";
    case code::fix::parse::INVALID_LENGTH: return "fix::parse::invalid_length";
    case code::fix::parse::INVALID_MESSAGE_TYPE: return "fix::parse::invalid_message_type";

    case code::fix::validation::MISSING_REQUIRED: return "fix::validation::missing_required";
    case code::fix::validation::INVALID_FIELD_VALUE: return "fix::validation::invalid_field_value";
    case code::fix::validation::REQUIRED_GROUP_EMPTY: return "fix::validation::required_group_empty";
    case code::fix::validation::GROUP_COUNT_MISMATCH: return "fix::validation::group_count_mismatch";
    case code::fix::validation::COMPONENT_VALIDATION_FAILED: return "fix::validation::component_validation_failed";
    case code::fix::validation::ENCODE_FAILED: return "fix::validation::encode_failed";

    case code::fix::session::UNSUPPORTED_PROTOCOL: return "fix::session::unsupported_protocol";
    case code::fix::session::SESSION_CREATE_FAILED: return "fix::session::session_create_failed";
    case code::fix::session::SESSION_NOT_FOUND: return "fix::session::session_not_found";
    case code::fix::session::SEQUENCE_GAP: return "fix::session::sequence_gap";
    case code::fix::session::SESSION_REJECT: return "fix::session::session_reject";
    case code::fix::session::LOGON_FAILED: return "fix::session::logon_failed";
    case code::fix::session::NOT_LOGGED_ON: return "fix::session::not_logged_on";
    case code::fix::session::LOGOUT_FAILED: return "fix::session::logout_failed";
    case code::fix::session::HEARTBEAT_TIMEOUT: return "fix::session::heartbeat_timeout";
    case code::fix::session::TEST_REQUEST_TIMEOUT: return "fix::session::test_request_timeout";
    case code::fix::session::RESEND_REQUIRED: return "fix::session::resend_required";
    case code::fix::session::RESEND_FAILED: return "fix::session::resend_failed";
    case code::fix::session::SEQUENCE_RESET_FAILED: return "fix::session::sequence_reset_failed";
    case code::fix::session::REJECTED_BY_APPLICATION: return "fix::session::rejected_by_application";
    case code::fix::session::INVALID_SEQUENCE_RESET: return "fix::session::invalid_sequence_reset";

    case code::application::REJECTED: return "application::rejected";
    case code::application::CALLBACK_FAILED: return "application::callback_failed";
    case code::application::ENGINE_ALREADY_RUNNING: return "application::engine_already_running";

    default: return "unknown";
    }
}

[[nodiscard]] inline constexpr std::string_view error_message(Code c) noexcept {
    return code_name(c);
}

[[nodiscard]] inline constexpr std::string_view error_message(const Status &s) noexcept {
    return code_name(s.code);
}

inline void format_error(const Status &s, char *out) noexcept {
    auto name = code_name(s.code);

    std::memcpy(out, name.data(), name.size());
    char *p = out + name.size();

    if (s.tag) {
        *p++ = ' ';
        *p++ = 't'; *p++ = 'a'; *p++ = 'g'; *p++ = '=';
        p += falconfix::itoa(s.tag, p);
    }

    if (s.reason) {
        *p++ = ' ';
        *p++ = 'r'; *p++ = 'e'; *p++ = 'a'; *p++ = 's'; *p++ = 'o'; *p++ = 'n'; *p++ = '=';
        p += falconfix::itoa(s.reason, p);
    }

    *p = '\0';
}

[[nodiscard]] inline std::string format_error(const Status &s) {
    std::string out;
    out.reserve(128);

    out += "code=";
    out += std::to_string(s.code);
    out += " ";
    out += code_name(s.code);

    out += " domain=";
    out += domain_name(s.domain);

    if (s.tag != 0) {
        out += " tag=";
        out += std::to_string(s.tag);
    }

    if (s.reason != 0) {
        out += " reason=";
        out += std::to_string(s.reason);
    }

    return out;
}

} // namespace falconfix::errors

using FFStatus = falconfix::errors::Status;

#define FF_OK() \
    ::falconfix::errors::success()

#define FF_SYS_ERR(code_) \
    ::falconfix::errors::system(::falconfix::errors::code::system::code_)

#define FF_CONFIG_ERR(code_) \
    ::falconfix::errors::config(::falconfix::errors::code::config::code_)

#define FF_SOCKET_ERR(code_) \
    ::falconfix::errors::socket(::falconfix::errors::code::socket::code_)

#define FF_TLS_ERR(code_) \
    ::falconfix::errors::tls(::falconfix::errors::code::tls::code_)

#define FF_FIX_PARSE_ERR(code_, tag_) \
    ::falconfix::errors::fix_parse(::falconfix::errors::code::fix::parse::code_, static_cast<int32_t>(tag_))

#define FF_FIX_VALIDATION_ERR(code_, tag_) \
    ::falconfix::errors::fix_validation(::falconfix::errors::code::fix::validation::code_, static_cast<int32_t>(tag_))

#define FF_FIX_SESSION_ERR(code_, reason_) \
    ::falconfix::errors::fix_session(::falconfix::errors::code::fix::session::code_, static_cast<int32_t>(reason_))

#define FF_APP_ERR(code_, reason_) \
    ::falconfix::errors::application(::falconfix::errors::code::application::code_, static_cast<int32_t>(reason_))
