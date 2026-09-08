---
description: Add a handler for a FIX message in the application layer
agent: agent
---
Handle ${input:msg:FIX message, e.g. NewOrderSingle} in ${input:cb:fromApp or fromAdmin}.

Load skill `engine-layout`. In the `FIXApplication` impl (see `tests/app/app_test.cpp`):
1. In `${input:cb}`, branch with `if (msg.is<fix44::${input:msg}>())` then `const auto &m = msg.as<fix44::${input:msg}>();`.
2. Use generated getters (skill `engine-layout` for field access); never parse raw bytes.
3. Return `FF_OK()` on success or `FF_APP_ERR(REJECTED, 0)` / `FF_FIX_SESSION_ERR(...)` (skill `error-codes`); never throw.
4. Add a test in `tests/app/` or the relevant suite: message accepted, rejected, wrong-type ignored.
Don't touch `src/engine/**`.
