---
name: architecture-map
description: FalconFIX module map, message flow, ownership, status/error contract, threading. Load before touching more than one module or when unsure where code belongs.
---
# Architecture map (namespace `falconfix`)

## Wiring (what `FIXRuntime::start()` does, per session in `SessionSettings`)
```
FIXApplication (user)  ──▶ FIXRuntime(app, settings, ISocketContext&)
  for each SessionConfig:
	LogFactory::createSessionLog → MessageStoreFactory::create
	ProtocolFactory::createSession(app, cfg, log, store)   // picks FIX4.x/engine/session.h by BeginString
	app.onCreate(sessionID)
	ConnectionFactory::create(socketEngine, session, cfg)   // TCPInitiatorConnection | TCPAcceptorConnection
	session.bindConnection(connection); registry.add(session); connection.start()
  m_timerThread: every 1000 ms → session.onTimer() then connection.onTimer()
```
`stop()`: flips `m_running`, joins timer thread, `doLogout("engine shutdown")` each session, then `detach()` + `close()` each connection. `FIXRuntime` also exposes `findSession(sid)`, `sendToTarget<Msg>(msg, sid)` (casts to the version's `SessionType`, checks `Protocol::VERSION`), `sendLogout(sid, reason)`.

## Lifecycle of one FIX session (in generated `engine/session.h`)
Logon handshake: initiator sends Logon (`LogonSent`), on valid Logon reply → `LoggedOn` + `app.onLogon`; acceptor validates identity/HeartBtInt/EncryptMethod first, replies or logs out. Heartbeat/TestRequest driven by `onTimer` using `SessionState::last{Sent,Received}Ms`. Gap detected → ResendRequest (`awaitingResend`), replays from `MessageStore::getOutgoing` with PossDup. Logout: `doLogout` sends Logout (`LogoutSent`) → on ack `Disconnected`; may `resetSeqNumsOnDisconnect`.

## Inbound message flow (bytes → app)
```
socket onRead ─▶ IConnection ─▶ SessionBase::onReceive(data,size)
  ─▶ TCPFrameView: frame bytes into one full FIX msg (beginString, msgType,
	 sender/target, seqNum, body); partial ⇒ socket::BUFFER_UNDERFLOW → wait for more
  ─▶ session validates seqnum/identity/sendingTime, updates state, stores incoming
  ─▶ inbound_dispatch::dispatchTyped<Msg>: msg.setString(...) parses,
	 wraps FIXMessageRef ─▶ app.fromAdmin (Msg::IS_ADMIN) | app.fromApp
```
Outbound: `runtime.sendToTarget<Msg>` / session builds via `outbound_prepare`, `app.toAdmin`/`toApp` may inspect/modify, store outgoing, `connection.write*`.

## Layers (top may call down; never up)
| Layer | Dir | Key types | Job |
|---|---|---|---|
| app | `app/` | `FIXApplication` (onCreate/onLogon/onLogout, toAdmin/fromAdmin/toApp/fromApp), `FIXMessageRef`, `ProtocolVersion` | user callbacks |
| runtime | `runtime/` | `FIXRuntime`, `ProtocolFactory` | owns sessions + connections, timer thread |
| session | `session/` | `SessionBase` (onConnected/onReceive/onDisconnected/onTimer/doLogout), `SessionState{SessionStatus}`, `SessionID`, `SessionConfig`, `SessionSettings`, `SessionRegistry`, `SessionSchedule` | FIX state machine, seqnums, heartbeat |
| engine (generated) | `src/engine/FIX4.x/` | `engine/session.h`, `engine/protocol.h`, messages/components/fields | parse/serialize, concrete `SessionBase` impl |
| connection | `connection/` | `IConnection` (start/write/writeAsync/close/detach/onTimer, `TransportState`), `TCPInitiatorConnection`, `TCPAcceptorConnection`, `ConnectionFactory` | bytes ⇄ session (`session.onReceive(data,size)`) |
| socket | `socket/` (`falconfix::socket`) | `ISocketContext` (start(threads)/stop/createTCPSocket/createTCPServer), `ITCPSocket`, `ITCPServer`, `SocketOptions`, `TCPMode` Plain/SSL; impl `socket/boost/*` | async I/O |
| store | `store/` | `MessageStore` (storeOutgoing/storeIncoming/getOutgoing, next*/set*/incr*SeqNum, reset), `Memory|File|NullMessageStore`, `MessageStoreFactory` | persistence for resend |
| logger | `logger/` | `EngineLog` (`SpdlogEngineLog`, `NullEngineLog`), `MessageLog`, `LogFactory` | logs |
| config | `config/` | `FIXConfig::parse/parseFile` (INI: `[DEFAULT]` + `[SESSION]` sections), `FIXConfig::ParseError` (throws) | settings source |
| utils | `utils/` | `errors::Status`=`FFStatus`, `FastBuffer`, `FixedString`, `TickClock`, `IFile/File` | primitives |

## SessionStatus
`Disconnected → Connecting → Connected → LogonSent → LoggedOn → LogoutSent|LogoutReceived → Closing → Disconnected`. Flags in `SessionState`: sentLogon/receivedLogon/sentLogout/receivedLogout/awaitingResend/awaitingTestResponse, lastSentMs/lastReceivedMs. Change status only inside the engine session impl.

## Status contract
Everything on the hot path returns `FFStatus` (`errors::Status{code,tag,reason,domain}`, `ok()`); build with `FF_OK()`, `FF_SYS_ERR`, `FF_CONFIG_ERR`, `FF_SOCKET_ERR`, `FF_TLS_ERR`, `FF_FIX_PARSE_ERR(code,tag)`, `FF_FIX_VALIDATION_ERR(code,tag)`, `FF_FIX_SESSION_ERR(code,reason)`, `FF_APP_ERR(code,reason)`. Codes in `utils/error_codes.h` per `errors::code::<domain>`. Exceptions only in config parsing.

## Ownership / threading
`FIXRuntime` owns sessions (`unique_ptr<SessionBase>`) and connections; connection holds `shared_ptr<ITCPSocket>` and references to session + `ISocketContext`; app is a reference. Socket callbacks (`onConnected/onRead/onClosed/onError`) are static `void*` ctx trampolines on the socket thread(s); `onTimer` runs on the runtime timer thread. Each concrete session serializes its work with a `std::recursive_mutex` and buffers via `FastBuffer m_in`/`m_out` (inbound accumulates until a full frame). State shared across threads is `std::atomic` (`m_running`, `TransportState`). All methods `noexcept`; no blocking in callbacks.

## Test switch
`FALCONFIX_ENABLE_TEST_API` exposes `*ForTest()` hooks on `SessionBase` (seqnums, schedule, test-request); tests use `tests/support/` builders. `FALCONFIX_ENABLE_FIX4x` selects compiled protocol versions.

## Message type-erasure
App callbacks receive `FIXMessageRef{void* ptr, msgType, protocol, possDup, possResend}`; use `ref.is<fix44::ExecutionReport>()` then `ref.as<Msg>()`. Never store the ref past the callback. Generated message/field/enum structs live in `src/engine/FIX4.x/` — see skill `engine-layout`.
