---
applyTo: "src/include/store/**,src/cpp/store/**,src/include/config/**,src/cpp/config/**,src/include/logger/**,src/cpp/logger/**"
---
# Store / config / logger rules
- `MessageStore` interface: `storeOutgoing/storeIncoming/getOutgoing`, `next|set|incr{Sender,Target}SeqNum`, `reset`. New backends derive from `MessageStoreBase`, register in `MessageStoreFactory`, add `StoreType` enum value. All `noexcept`, return `FFStatus`.
- `FIXConfig` is INI-like: `[DEFAULT]` then `[SESSION]` blocks; keys resolved session→default via `getResolved/tryGetResolved`. Parsing is the only place exceptions are allowed (`FIXConfig::ParseError`). `SessionSettings` converts it into `SessionConfig` structs.
- Logging via `EngineLog` / `MessageLog` obtained from `LogFactory`; never call spdlog directly outside `SpdlogEngineLog`. `Null*` variants exist for tests — prefer them.
- Tests: `tests/store/`, `tests/config/`, `tests/logger/`; use `MemoryMessageStore` and in-memory config text, not files.
