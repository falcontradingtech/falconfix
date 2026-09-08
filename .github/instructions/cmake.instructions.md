---
applyTo: "**/CMakeLists.txt,cmake/**,CMakePresets.json"
---
# CMake rules

- Use macros only: `add_lib_custom(<target> STATIC|SHARED|INTERFACE [deps])`, `add_test_custom(<name> [libs])`, `add_app_custom(<name> [deps])`, `add_benchmark_custom(<name> [deps])`. Sources are globbed from the current dir.
- Targets are `falconfix_<module>`; generated protocol lib is `falconfix_core`; tests `<module>_tests`; benchmarks `<module>_benchmark`.
- Do not edit root `CMakeLists.txt` or `cmake/*.cmake` without approval.
- Presets: `windows-debug|windows-release|linux-debug|linux-release` (configure, build, test). Never create ad-hoc build dirs.
- Min CMake 3.16; no `file(GLOB)` outside the macros; no absolute paths.
