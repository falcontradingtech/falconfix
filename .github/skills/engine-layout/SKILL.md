---
name: engine-layout
description: How generated FIX code under src/engine/FIX4.x is organized and consumed (messages, fields, Protocol, Session, dispatch). Load when reading/using generated protocol code or wiring a new message handler.
---
# Engine layout (generated — edit falcongen, never these files)

Per version `src/engine/include/FIX4.x/`:
| Dir | Content |
|---|---|
| `core/types.h` | scalar aliases `fix44::types::{INT,PRICE,QTY,UTCDATEONLY,...}` |
| `core/enums.h`, `core/fields.h` | field enums and tag/field definitions |
| `messages/*.h` | one struct per FIX message (`ExecutionReport`, `NewOrderSingle`, ...); each has `static constexpr MSGTYPE`, `using Protocol = ...` |
| `components/*.h` | repeating-group / component structs (`AllocGrp`, `Parties`, ...) |
| `engine/protocol.h` | `struct Protocol { VERSION, BEGIN_STRING="FIX4.4" }` + fwd `class Session` |
| `engine/session.h` | concrete `SessionBase` impl (state machine, seqnums, resend) |
| `engine/inbound_dispatch.h` / `outbound_prepare.h` | decode→route / encode helpers |
| `engine/tcp_frame_view.h` | frames raw bytes into complete FIX messages |
| `utils/all_messages.h`, `utils/datetime.h` | includes-all, time helpers |

## Using a message in app callbacks
`FIXMessageRef` is a type-erased view (`void* ptr`, `msgType`, `protocol`, `possDup/possResend`):
```cpp
FFStatus fromApp(FIXMessageRef m, const SessionID &id) noexcept override {
	if (m.is<fix44::ExecutionReport>()) {
		const auto &er = m.as<fix44::ExecutionReport>();
		...
	}
	return FF_OK();
}
```
`m.is<Msg>()` checks protocol + `Msg::MSGTYPE`; `m.as<Msg>()` casts. Never keep the ref/ptr past the callback.

`ProtocolFactory::createSession` selects the version by `SessionConfig` BeginString; only versions compiled in (`FALCONFIX_ENABLE_FIX4x`) are available. Apps link `falconfix_core` for the generated types.

## Generated contracts (rely on these, don't reimplement)
- Message struct `M`: `M::MSGTYPE`, `M::IS_ADMIN`, `using Protocol`, `M::setString(begin,end)` (parse, returns `FFStatus`), field getters/setters, `reset()`.
- `Protocol`: `VERSION`, `BEGIN_STRING`, `using SessionType` (the concrete `SessionBase`).
- `engine/tcp_frame_view.h` → `TCPFrameView` (msgType, sender/target, seqNum, bodyLength, possDup/possResend); `isIncompleteTCPFrame(rc)` = need more bytes (`socket::BUFFER_UNDERFLOW`).
- `engine/inbound_dispatch.h` → `dispatchTyped<Msg>(app, sid, frame)` parses then calls `app.fromAdmin`/`fromApp` by `Msg::IS_ADMIN`; `engine/outbound_prepare.h` for send.
