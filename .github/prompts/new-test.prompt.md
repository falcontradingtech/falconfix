---
description: Add a GTest suite/case for a FalconFIX class
agent: agent
---
Write tests for ${input:target:class or function, e.g. falconfix::TCPInitiatorConnection}.

1. Locate the header in `src/include/`; read only the public API.
2. Put tests in `tests/<module>/<topic>_tests.cpp` (create if missing; reuse `tests/support/` helpers, mock `I*` interfaces).
3. Cover: happy path, error/failure status, edge (empty/boundary). Name `TEST(ModuleTests, Action_Condition_Expected)`.
4. If new dir: `add_test_custom(<module>_tests <libs>)` in its `CMakeLists.txt` and `add_subdirectory` in `tests/CMakeLists.txt`.
5. Build in background: `cmake --build --preset windows-debug --target <module>_tests`; then `ctest --preset windows-debug -R <module>`.
Report only: files touched, test names, pass/fail.
