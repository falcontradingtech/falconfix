---
name: build-test
description: Configure, build and test FalconFIX incrementally with CMake presets (Windows/Linux). Use before claiming a change works.
---
# Build & test

Always reuse the preset build dir; never create ad-hoc `build-*` dirs.
**Before building, check if `build/windows-debug` (or `build/linux-debug`) already exists.** If it does, skip the configure step and build incrementally — do not delete/reconfigure it (that forces a full rebuild and wastes time). Only run the configure step the first time or after `CMakeLists.txt`/preset changes.

| Step | Windows | Linux |
|---|---|---|
| Configure (once) | `cmake --preset windows-debug` | `cmake --preset linux-debug` |
| Build all | `cmake --build --preset windows-debug` | `cmake --build --preset linux-debug` |
| Build one target | `cmake --build --preset windows-debug --target <t>` | same with linux preset |
| Test all | `ctest --preset windows-debug --output-on-failure` | `ctest --preset linux-debug --output-on-failure` |
| Test subset | add `-R <regex>` | add `-R <regex>` |
| Re-run failed | add `--rerun-failed` | same |

Targets: libs `falconfix_<module>`, tests `<module>_tests`, apps per `apps/`.
Run builds/ctest in background and poll; if no output for 3 min, stop and ask the user.
Read only the first error; fix root cause; rebuild the single target.

## Test executables (Windows Debug)

Prefer `ctest`. To run a suite directly (e.g. `--gtest_filter`), the built `.exe` lives under `build/windows-debug/tests/...`:

| Suite | Executable |
|---|---|
| all | `build/windows-debug/tests/Debug/all_tests.exe` |
| app | `build/windows-debug/tests/app/Debug/app_tests.exe` |
| config | `build/windows-debug/tests/config/Debug/config_tests.exe` |
| connection | `build/windows-debug/tests/connection/Debug/connection_tests.exe` |
| session | `build/windows-debug/tests/session/Debug/session_tests.exe` |
| engine FIX4.2 components | `build/windows-debug/tests/engine/FIX4.2/components/Debug/fix42_components_tests.exe` |
| engine FIX4.2 utils | `build/windows-debug/tests/engine/FIX4.2/utils/Debug/fix42_utils_tests.exe` |
| engine FIX4.3 components | `build/windows-debug/tests/engine/FIX4.3/components/Debug/fix43_components_tests.exe` |
| engine FIX4.3 utils | `build/windows-debug/tests/engine/FIX4.3/utils/Debug/fix43_utils_tests.exe` |

Pattern: `build/windows-debug/tests/<module>/Debug/<module>_tests.exe`; engine suites nest under `tests/engine/FIX4.x/<component>/Debug/fix4x_<component>_tests.exe`. Filter with `.\<exe> --gtest_filter="*Name*"`.

## Capturing session/socket logs in tests

Test runs suppress logs by default: `tests/main.cpp` calls `falconfix::LogFactory::disableLogging()` before `RUN_ALL_TESTS()`. Configs with `LogType=screen` still print nothing until this is disabled.

To trace session/socket event flow (e.g. "logout sent", "logout received", "disconnected") when diagnosing a failing integration test, temporarily comment out that line, rebuild the suite, run the single test, then restore it. Do not commit the change.
