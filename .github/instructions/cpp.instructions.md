---
applyTo: "src/include/**/*.h,src/cpp/**/*.{h,cpp},apps/**/*.{h,cpp},tests/**/*.{h,cpp},benchmark/**/*.{h,cpp}"
---
# C++ rules

## Header (`src/include/<module>/name.h`)
```cpp
// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>                 // std first

#include <connection/i_connection.h>   // project, angle brackets, alphabetical

namespace falconfix {

class TCPInitiatorConnection final : public IConnection {
	SessionBase &m_session;        // members first, m_camelCase
	std::size_t m_nextIndex{0};

public:
	explicit TCPInitiatorConnection(SessionBase &session) noexcept;
	FFStatus start() noexcept override;

private:
	void advanceToNextEndpoint() noexcept;
};

} // namespace falconfix
```
`.cpp`: include own header first, then std, then project.

## Naming
Classes/structs/enums `PascalCase`; interfaces `IName`; methods/functions/locals `camelCase`; members `m_camelCase`; constants `UPPER_CASE`; files `snake_case`.

## Rules
- `std::unique_ptr`/`std::shared_ptr` for ownership; references/`std::span`/`std::string_view` for non-owning.
- `explicit` single-arg ctors; delete copy unless copying is meaningful; default move.
- `noexcept` on hot-path/I/O methods; return `FFStatus` (skill `error-codes`) or `std::expected`; exceptions only for config/parse errors.
- `constexpr`/`const` where possible; `[[nodiscard]]` on status-returning functions.
- Functions ≤ 50 lines; extract otherwise.
- No blocking calls in connection/session paths; async I/O via socket context.
- Doxygen `///` on public API only.
