# System Architecture

## Modules
```
FalconFIX
├── Connection  - Network I/O (TCP/TLS)
├── Session     - Session lifecycle
├── Runtime     - Message dispatching
├── Engine      - Generated protocol layer
└── Utils       - Logging, config, errors
```

## Key Design Patterns

### State Machine
- Connection: Created → Connected → Closed
- Session: Idle → LoggingOn → LoggedOn → LoggingOut → Idle
- Message: Received → Validated → Routed → Executed

### Ownership
- Connections own sockets
- SessionManager owns Sessions
- Sessions own handlers
- Runtime is injected, not owned

### Error Handling
- Use `std::expected<T, Error>` everywhere
- Never throw exceptions in I/O paths
- Log all errors to spdlog

## Threading
- Connection I/O: async (io_context)
- Message dispatch: thread pool
- Session state: atomic
- No blocking calls

## Boundaries
```
Public API       src/include/
   ↓
Session/Runtime  src/session/, src/runtime/
   ↓
Connection       src/connection/
   ↓
Engine (RO)      src/engine/
```

Connection should NOT know about Session.
Session should NOT know about raw bytes.
Runtime should NOT know about TCP.

## Validation Points
1. Network decode
2. Message schema
3. Session rules
4. Business logic
