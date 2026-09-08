---
name: build-test
description: Configure, build and test FalconFIX incrementally with CMake presets (Windows/Linux). Use before claiming a change works.
---
# Build & test

Always reuse the preset build dir; never create ad-hoc `build-*` dirs.

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
