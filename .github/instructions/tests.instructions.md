---
applyTo: "tests/**"
---
# GTest rules

- Location: `tests/<module>/<topic>_tests.cpp`; shared helpers in `tests/support/`; no per-suite `main` (`tests/main.cpp` is linked by `add_test_custom`).
- Name: `TEST(ModuleTests, Action_Condition_Expected)`; fixtures `TEST_F(ModuleTests, ...)` when setup is shared.
- Every function: happy path + error + edge (empty/boundary/null).
- `ASSERT_*` for preconditions, `EXPECT_*` for verification; one behavior per test.
- Mock interfaces (`IConnection`, `ITCPSocket`) with GMock; never hit real network, files, or `sleep`.
- Deterministic, order-independent, < 1 s each.
- New suite: `add_test_custom(<name>_tests <linked libs...>)` in `tests/<module>/CMakeLists.txt`; sources are globbed.
- `tests/engine/**` is generated — regenerate via falcongen instead of editing.
- Run: `ctest --preset windows-debug -R <Suite> --output-on-failure`.
