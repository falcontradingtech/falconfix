---
description: Scaffold a new src module (lib + headers + tests + CMake)
agent: agent
---
Create module `${input:name:module name, snake_case}`.

1. `src/include/${input:name}/` — public headers (`#pragma once`, SPDX, namespace `falconfix`). Expose interfaces as `I<Name>` if it needs mocking.
2. `src/cpp/${input:name}/` — impl + `CMakeLists.txt`: `add_lib_custom(falconfix_${input:name} STATIC <deps>)`.
3. `add_subdirectory(${input:name})` in `src/cpp/CMakeLists.txt`.
4. Tests: use skill `add-module-test` → `tests/${input:name}/` with `add_test_custom(${input:name}_tests falconfix_${input:name})`.
5. Respect layer direction (skill `architecture-map`); return `FFStatus` (skill `error-codes`).
6. Configure once, build `falconfix_${input:name}` and `${input:name}_tests`.
