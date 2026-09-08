---
name: error-codes
description: FalconFIX status/error contract — FFStatus, FF_* macros, and the error code domains/names. Load instead of reading utils/error_codes.h when returning or checking errors.
---
# Status & error codes (`utils/error_codes.h`, namespace `falconfix::errors`)

`FFStatus` = `errors::Status{ Code code; int32_t tag; int32_t reason; Domain domain; }`; `.ok()` / `explicit operator bool`. `Code` is `int32_t` (0 = success, negatives = errors). Success: `FF_OK()`.

## Build an error — macro takes the UNQUALIFIED code name
| Macro | Domain | Extra arg | Codes namespace |
|---|---|---|---|
| `FF_SYS_ERR(code)` | system | — | `code::system::*` |
| `FF_CONFIG_ERR(code)` | config | — | `code::config::*` |
| `FF_SOCKET_ERR(code)` | socket | — | `code::socket::*` |
| `FF_TLS_ERR(code)` | tls | — | `code::tls::*` |
| `FF_FIX_PARSE_ERR(code, tag)` | fix_parse | FIX tag | `code::fix::parse::*` |
| `FF_FIX_VALIDATION_ERR(code, tag)` | fix_validation | FIX tag | `code::fix::validation::*` |
| `FF_FIX_SESSION_ERR(code, reason)` | fix_session | reason | `code::fix::session::*` |
| `FF_APP_ERR(code, reason)` | application | reason | `code::application::*` |
e.g. `return FF_SOCKET_ERR(NOT_OPEN);`, `return FF_FIX_SESSION_ERR(SEQUENCE_GAP, 0);`.

## Code names by domain (pick the closest; don't invent)
- **system**: UNKNOWN, NOT_IMPLEMENTED, BAD_ALLOC, THREAD_FAILURE, RESOURCE_BUSY, OUT_OF_MEMORY, PERMISSION_DENIED, INTERRUPTED
- **config**: MISSING, INVALID, XML_PARSE_FAILED
- **socket**: ACCEPT/CONNECT/BIND/LISTEN/READ/WRITE/CLOSE/RESOLVE_FAILED, INVALID_INTERFACE, INTERFACE_BIND, DISCONNECTED, CLOSED, NOT_OPEN, ALREADY_CONNECTED, NOT_CONNECTED, UNAVAILABLE, RECV/SEND/OPERATION_TIMEOUT, BUFFER_OVERFLOW, **BUFFER_UNDERFLOW** (=need more bytes), INVALID_FRAME, END_OF_FILE, UNKNOWN
- **tls**: FAILURE, HANDSHAKE_FAILED, CERTIFICATE_ERROR, OPERATION_NOT_SUPPORTED
- **fix::parse**: UNKNOWN_FIELD_ID, INVALID_FIELD_VALUE, UNEXPECTED_FIELD, OUT_OF_RANGE, DUPLICATE_TAG, MALFORMED_TAG, MISSING_EQUALS, MISSING_SOH, DECODE_FAILED, INVALID_CHECKSUM, INVALID_LENGTH, INVALID_MESSAGE_TYPE
- **fix::validation**: MISSING_REQUIRED, INVALID_FIELD_VALUE, REQUIRED_GROUP_EMPTY, GROUP_COUNT_MISMATCH, COMPONENT_VALIDATION_FAILED, ENCODE_FAILED
- **fix::session**: UNSUPPORTED_PROTOCOL, SESSION_CREATE_FAILED, SESSION_NOT_FOUND, SEQUENCE_GAP, SESSION_REJECT, LOGON_FAILED, NOT_LOGGED_ON, LOGOUT_FAILED, HEARTBEAT_TIMEOUT, TEST_REQUEST_TIMEOUT, RESEND_REQUIRED, RESEND_FAILED, SEQUENCE_RESET_FAILED, REJECTED_BY_APPLICATION, INVALID_SEQUENCE_RESET
- **application**: REJECTED, CALLBACK_FAILED, ENGINE_ALREADY_RUNNING

## Helpers
`errors::success()`, `errors::fail(domain,code,tag,reason)`; typed `errors::socket(c)/tls(c)/config(c)/fix_parse(c,tag)/fix_session(c,reason)/application(c,reason)`; `domain_name(d)`, `code_name(c)`, `format_error(status)` for logs. Check "need more bytes" with `isIncompleteTCPFrame(rc)` (skill `engine-layout`).
