---
applyTo: "src/include/connection/**,src/cpp/connection/**,src/include/socket/**,src/cpp/socket/**"
---
# Connection / socket rules
- `IConnection` = transport only: `start/write/writeAsync/close/detach/onTimer`; `TransportState{Idle,Connecting,Connected,Closing}` is an atomic changed only via `transitionTransportState(expected,desired)` (CAS) / `setTransportState`. It knows `SessionBase` only to call `onConnected/onReceive(data,size)/onDisconnected`; no FIX parsing, no seqnums.
- `ITCPSocket`: `connect(host,port,iface)`, `write` (returns bytes, sync) vs `writeAsync` (returns bool, queued — check `pendingWrites()`), `close`, `isOpen`, `setCallbacks`, `setOptions`, `peerIP/peerPort`. `write`/`writeAsync` are `[[nodiscard]]` — always act on the return so no bytes are dropped.
- Callbacks are a `SocketCallbacks` struct of C function pointers `+ void* m_ctx`, defaulted to `detail::noop*`; delivered as `onConnected/onClosed/onRead(SocketReadView)/onError(ErrorCode,msg)` on socket-context thread(s). Must be `noexcept`, non-blocking, hold no lock across a call into the session. `SocketReadView{m_data,m_size}` is a non-owning window — copy before returning; it is invalid after the callback.
- Get sockets only via `ISocketContext::createTCPSocket/createTCPServer(TCPMode Plain|SSL)`; `ISocketContext::start(threadCount)` runs the event loop threads.
- `SocketOptions` (built from `SessionConfig::Connection`): `m_noDelay`(true), `m_fastOpen`, `m_cork`, `m_send/recvBufferSize`(0=OS), `m_connect/send/recvTimeoutMs`(0=none), `m_maxWriteChunkSize`(64KB — writes larger than this are chunked), TLS `m_tls{Cert,Key,Ca}File/ServerName/VerifyPeer`.
- Initiator: endpoint list with `currentEndpoint/advanceToNextEndpoint/resetToPrimaryEndpoint` (failover across `socketConnectEndpoints`, retry every `reconnectIntervalMs`). Acceptor: server socket per session, validate identity before handing to session.
- Errors: `FF_SOCKET_ERR(code)` / `FF_TLS_ERR(code)`; codes in `code::socket::*` / `code::tls::*` (skill `error-codes`). Partial FIX frame ⇒ `socket::BUFFER_UNDERFLOW` (`isIncompleteTCPFrame`) — not an error, wait for more bytes.
- Boost impl lives in `socket/boost/*`; anything else must go through the `I*` interfaces so tests can mock them.

## Invariants (sockets/connection)
Threading: callbacks run on socket threads, `onTimer` on the runtime timer thread, `write` may come from the app thread — shared state is synchronized. Hold points to keep consistent: always act on the `[[nodiscard]]` write result and respect `pendingWrites` backpressure; copy `SocketReadView` before returning (never use it after the callback); accumulate partial reads until a full frame; change `TransportState` only via the CAS helper; after a successful failover call `resetToPrimaryEndpoint`; when `useSSL` is set, propagate the TLS cert/key paths; on shutdown `detach` before `close`.
- Tests: `tests/connection/`, `tests/socket/`; mock `ITCPSocket`/`ISocketContext`; never open real sockets except in explicit integration tests already present.
