---
applyTo: "src/include/session/**,src/cpp/session/**,src/include/runtime/**,src/cpp/runtime/**,src/include/app/**"
---
# Session / runtime rules
- `SessionBase` is the abstract contract; the concrete session is generated (`src/engine/FIX4.x/engine/session.h`, class `fix44::engine::Session`). Behavior change ⇒ falcongen, not the generated file.
- Concrete session holds: `m_sid`, `m_app&`, `m_cfg`, `m_log`, `m_store`, `m_state` (`SessionState`), `m_out`/`m_in` (`FastBuffer`), `m_connection*`, and a `std::recursive_mutex m_mutex` — every public entry (`onReceive/onConnected/onDisconnected/onTimer/send/doLogout`) locks it; nested calls rely on it being *recursive* (don't swap for `std::mutex`).
- `SessionStatus` transitions only inside the session impl; runtime/connection never set status. Statuses: `Disconnected→Connecting→Connected→LogonSent→LoggedOn→LogoutSent|LogoutReceived→Closing`.
- Entry points are `noexcept`, return `FFStatus`; `FF_FIX_SESSION_ERR(code, reason)`, codes `code::fix::session::*` (skill `error-codes`).
- Seqnums live ONLY in `MessageStore` (`next/set/incr{Sender,Target}SeqNum`); `SessionState` holds timing/flags, not counters. Reset via `resetOnLogon/Logout/Disconnect` (`SessionConfig::Connection`).
- Admin handlers: `handle{Logon,Logout,Heartbeat,TestRequest,ResendRequest,Reject,BusinessMessageReject,SequenceReset}Received`. Senders: `sendLogon/Logout/Heartbeat/TestRequest/ResendRequest/SequenceResetGapFill/Reject/BusinessReject`, `resendApplicationMessage(StoredMessageView)`. Validators: `validateSessionIdentity`, `validateSendingTime` (gated by `validateSendingTime`/`maxLatencyMs`).
- Timing driven by `onTimer` using `SessionState::last{Sent,Received}Ms`, `heartBtIntMs`, `logoutTimeoutMs`; flags `disableHeartbeat/ignoreTestRequest/enableResendRequest`. No sleeps/timers inside the session.
- App callbacks (`FIXApplication::to*/from*`) receive `FIXMessageRef`; a non-ok return rejects/skips the message — never throw across them.
- `FIXRuntime` is the only owner of sessions/connections; `SessionRegistry` maps `SessionID`→session; `sendToTarget<Msg>` casts to `Protocol::SessionType` after checking `Protocol::VERSION`.
- Tests: `tests/session/*_tests.cpp`, `TEST(FIXSessionTests, ...)`, `tests/support/` builders, `*ForTest()` hooks under `FALCONFIX_ENABLE_TEST_API` (set/get seqnums, schedule, awaiting-test-response).

## Invariants (session)
Hold points to keep consistent: update seqnums through `m_store` exactly once per accepted message (and don't double-count on resend); set `LoggedOn` only after `onLogon` and store persistence; reach every helper with `m_mutex` held (it is recursive by design); resend replays application messages via `resendApplicationMessage` and GapFills admin gaps via `sendSequenceResetGapFill`; `validateSendingTime` honors `maxLatencyMs`; schedule checks go through `session_schedule` (weekly/weekday wrap, timezone/`useLocalTime`); a pending logout disconnects after `logoutTimeoutMs`; apply `reset*` flags at their matching transition; set PossDup/PossResend on resent messages.
